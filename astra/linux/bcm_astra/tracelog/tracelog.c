/******************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *****************************************************************************/

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "tzioc_drv.h"
#include "tzioc_sys_msg.h"
#include "tracelog.h"

//#include "bchp_common.h"

#ifdef BCHP_MEMC_TRACELOG_0_0_REG_START

#include "bchp_memc_tracelog_0_0.h"

#define TRACELOG_REG_SIZE \
    (BCHP_MEMC_TRACELOG_0_0_REG_END - BCHP_MEMC_TRACELOG_0_0_REG_START + 4)

#define TRACELOG_REG_OFFSET(reg) \
    (BCHP_MEMC_TRACELOG_0_0_ ##reg - BCHP_MEMC_TRACELOG_0_0_REG_START)

#define TRACELOG_REG_FIELD_MASK(reg, field) \
    (BCHP_MEMC_TRACELOG_0_0_##reg##_##field##_MASK)

#define TRACELOG_REG_FIELD_SHIFT(reg, field) \
    (BCHP_MEMC_TRACELOG_0_0_##reg##_##field##_SHIFT)

#else

#define STB_MEMC_TRACELOG_CONTROL                       0x4
#define STB_MEMC_TRACELOG_COUNT_MATCHES_TOTAL           0x30
#define STB_MEMC_TRACELOG_BUFFER_PTR                    0x50
#define STB_MEMC_TRACELOG_BUFFER_SIZE                   0x58


/* MEMC_TRACELOG :: CONTROL :: BUFFER_DRAM [01:01] */
#define STB_MEMC_TRACELOG_CONTROL_BUFFER_DRAM_MASK            0x00000002
#define STB_MEMC_TRACELOG_CONTROL_BUFFER_DRAM_SHIFT           1

/* MEMC_TRACELOG :: CONTROL :: FORMAT16 [00:00] */
#define STB_MEMC_TRACELOG_CONTROL_FORMAT16_MASK               0x00000001
#define STB_MEMC_TRACELOG_CONTROL_FORMAT16_SHIFT              0


#define TRACELOG_REG_SIZE                       0
#define TRACELOG_REG_OFFSET(reg)                STB_MEMC_TRACELOG_##reg
#define TRACELOG_REG_FIELD_MASK(reg, field)     STB_MEMC_TRACELOG_##reg##_##field##_MASK
#define TRACELOG_REG_FIELD_SHIFT(reg, field)    STB_MEMC_TRACELOG_##reg##_##field##_SHIFT

#endif

/* Tracelog formats:
 * - HW trace buffer uses 128-bit per entry in binary format;
 * - SW trace dump uses 50-char per entry in ASCII format:
 *   0xnnnnnnnnnnnn: event=0xnnnnnnnn, index=0xnnnnnnnn\n
 */
#define TRACE_ENTRY_BIN_SIZE 16
#define TRACE_ENTRY_ASC_SIZE 50

/*
 * Function Declarations
 */

static int tracelog_mdev_open(
    struct inode *inode,
    struct file *file);

static int tracelog_mdev_release(
    struct inode *ignored,
    struct file *file);

static ssize_t tracelog_mdev_read(
    struct file *file,
    char __user *buf,
    size_t len,
    loff_t *off);

struct tracelog_device {
    /* tracelog misc device */
    struct miscdevice *mdev;

    bool enabled;

    uint32_t tracelogBase;
    uint32_t tracelogSize;
    uint32_t sentinelBase;
    uint32_t sentinelSize;

    uint32_t traceBuffPaddr;
    uint32_t traceBuffSize;

    void *pTracelogRegs;
    void *pTraceBuff;
    void *pTraceDump;
    size_t bufOffset;
};

static char tracelog_mdev_name[16] = "tracelog";
module_param_string(tracelogmdevname, tracelog_mdev_name, sizeof(tracelog_mdev_name), 0);

static const struct file_operations tracelog_mdev_fops = {
    .owner          = THIS_MODULE,
    .read           = tracelog_mdev_read,
    .write          = NULL,
    .open           = tracelog_mdev_open,
    .release        = tracelog_mdev_release,
};

static struct miscdevice tracelog_mdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = tracelog_mdev_name,
    .fops = &tracelog_mdev_fops,
};

static struct tracelog_device tracelog_dev;
static struct tracelog_device *tldev = &tracelog_dev;

/*
 * Misc Device Functions
 */

static int tracelog_mdev_open(
    struct inode *inode,
    struct file *file)
{
    int err = 0;
    err = generic_file_open(inode, file);
    if (unlikely(err)) {
        LOGE("Failed to open file!");
        return err;
    }
    return 0;
}

static int tracelog_mdev_release(
    struct inode *ignored,
    struct file *file)
{
    return 0;
}

static ssize_t tracelog_mdev_read(
    struct file *file,
    char __user *buf,
    size_t len,
    loff_t *loff)
{
    off_t offset;
    int entries, entryStart, entryEnd;
    int i,lenWritten=0;
    void * tempbuf;
    if (!file || !buf || !loff) {
        LOGE("Invalid argument in tracelog read call");
        return -EINVAL;
    }

    if (!tldev->enabled) {
        LOGE("Tracelog is not enabled");
        return 0;
    }

    /* convert to 32-bit offset */
    offset = (off_t)*loff;

    /* assume postmortal read, i.e. all events have been flushed */
    entries = *(volatile uint32_t *)
        (tldev->pTracelogRegs + TRACELOG_REG_OFFSET(COUNT_MATCHES_TOTAL));

    entryStart = (offset      ) / TRACE_ENTRY_ASC_SIZE;
    entryEnd   = ((offset + len) / TRACE_ENTRY_ASC_SIZE)-1;

    if (entryStart >= entries)
        return 0;

    if (entryEnd >= entries) {
        entryEnd = entries - 1;
    }
    len = (entryEnd - entryStart + 1)* TRACE_ENTRY_ASC_SIZE;
    tempbuf = tldev->pTraceBuff + tldev->bufOffset;

    for (i = entryStart; i <= entryEnd; i++) {
        uint32_t *data = (uint32_t *)( tempbuf + TRACE_ENTRY_BIN_SIZE * i);
        char *dump = (char *)(tldev->pTraceDump + TRACE_ENTRY_ASC_SIZE * i);

        uint32_t valid;
        uint32_t event;
        uint32_t index;
        uint32_t timestampLo;
        uint32_t timestampHi;

        valid = (data[3] >> 15) & 0x1;
        if (!valid)
            break;

        event       = (data[0]);
        index       = (data[1] - tldev->sentinelBase) / 4;
        timestampLo = (data[2]);
        timestampHi = (data[3] >> 16);

        snprintf(
            dump, TRACE_ENTRY_ASC_SIZE,
            "0x%04x%08x: event=0x%08x index=0x%08x",
            (unsigned int)(timestampHi & 0xFFFF),
            (unsigned int)timestampLo,
            (unsigned int)event,
            (unsigned int)index);

        /* use new line to terminate each entry */
        dump[49] = '\n';
        lenWritten+=TRACE_ENTRY_ASC_SIZE;
        tldev->bufOffset +=TRACE_ENTRY_BIN_SIZE;
        if(tldev->bufOffset >= tldev->traceBuffSize)
           tldev->bufOffset -= tldev->traceBuffSize ;
    }
    /*LOGE("len=%d lenWritten=%d\n", len, lenWritten);*/
    if (copy_to_user(buf, tldev->pTraceDump + offset, lenWritten))
        return -EFAULT;

    *loff = (loff_t)offset + lenWritten;
    return lenWritten;
}

int tracelog_init(void)
{
    int err = 0;
    uint32_t control, buffer_ptr, buffer_size;

    if (tldev->enabled) {
        LOGE("Traclog already enabled");
        return -1;
    }

    /* map tracelog registers */
    tldev->pTracelogRegs = ioremap_nocache(
        tldev->tracelogBase,
        tldev->tracelogSize);

    if (!tldev->pTracelogRegs) {
        LOGE("Failed to map tracelog registers");
        err = -EIO;
        goto ERR_EXIT;
    }

    /* verify tracelog control */
    control = *(volatile uint32_t *)
        (tldev->pTracelogRegs + TRACELOG_REG_OFFSET(CONTROL));

    if (!(control & TRACELOG_REG_FIELD_MASK(CONTROL, BUFFER_DRAM)) &&
        !(control & TRACELOG_REG_FIELD_MASK(CONTROL, FORMAT16))) {
        LOGE("Invalid HW tracelog control");
        err = -EINVAL;
        goto ERR_EXIT;
    }

    /* verify trace buffer */
    buffer_ptr = *(volatile uint32_t *)
        (tldev->pTracelogRegs + TRACELOG_REG_OFFSET(BUFFER_PTR));

    buffer_size = *(volatile uint32_t *)
        (tldev->pTracelogRegs + TRACELOG_REG_OFFSET(BUFFER_SIZE));

    if (buffer_ptr != tldev->traceBuffPaddr ||
        buffer_size != tldev->traceBuffSize) {
        LOGE("Invalid HW tracelog buffer");
        err = -EINVAL;
        goto ERR_EXIT;
    }

    /* map trace buffer */
#if IOREMAP_SHARED_MEM
    tldev->pTraceBuff = ioremap_nocache(buffer_ptr, buffer_size);
#else
    tldev->pTraceBuff = (void *)_tzioc_paddr2vaddr(buffer_ptr);
#endif

    if (!tldev->pTraceBuff) {
        LOGE("Failed to map HW trace buffer");
        err = -EIO;
        goto ERR_EXIT;
    }

    /* alloc trace dump buffer */
    tldev->pTraceDump = kmalloc(
        buffer_size / TRACE_ENTRY_BIN_SIZE * TRACE_ENTRY_ASC_SIZE,
        GFP_KERNEL);

    if (!tldev->pTraceDump) {
        LOGE("Failed to alloc tracelog dump buffer");
        err = -ENOMEM;
        goto ERR_EXIT;
    }
    tldev->bufOffset = 0;
    /* register misc device */
    err = misc_register(&tracelog_mdev);

    if (err) {
        LOGE("Failed to register tracelog misc device");
        goto ERR_EXIT;
    }

    tldev->mdev = &tracelog_mdev;

    tldev->enabled = true;
    LOGI("Tracelog is enabled");

    return 0;

 ERR_EXIT:
    tracelog_exit();
    return -1;
}

void tracelog_exit(void)
{
    if (tldev->mdev) {
        misc_deregister(tldev->mdev);
        tldev->mdev = 0;
    }

    if (tldev->pTraceDump) {
        kfree(tldev->pTraceDump);
        tldev->pTraceDump = 0;
    }

    if (tldev->pTraceBuff) {
#if IOREMAP_SHARED_MEM
        iounmap(tldev->pTraceBuff);
#endif
        tldev->pTraceBuff = 0;
    }

    if (tldev->pTracelogRegs) {
        iounmap(tldev->pTracelogRegs);
        tldev->pTracelogRegs = 0;
    }

    tldev->enabled = false;
    LOGI("Tracelog is disabled");

}

int tracelog_msg_proc(struct tzioc_msg_hdr *pHdr)
{
    int err = 0;

    struct sys_msg_tracelog_on_cmd *pCmd;
    LOGD("Tracelog msg processing");

    switch (pHdr->ucType) {

    case SYS_MSG_TRACELOG_ON:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen == 0) {
            LOGE("Invalid tracelog on cmd received");
            err = -EINVAL;
            break;
        }

        if (tldev->enabled) {
            LOGE("Ignoring tracelog on cmd, tracelog is already on");
            err = -EINVAL;
            break;
        }

        pCmd = (struct sys_msg_tracelog_on_cmd *)TZIOC_MSG_PAYLOAD(pHdr);

        if (!(pCmd->tracelogBase &&
              pCmd->sentinelBase &&
              pCmd->sentinelSize &&
              pCmd->traceBuffPaddr &&
              pCmd->traceBuffSize)) {
            err = -EINVAL;
            break;
        }

        tldev->tracelogBase = pCmd->tracelogBase;
        tldev->tracelogSize = pCmd->tracelogSize;
        tldev->sentinelBase = pCmd->sentinelBase;
        tldev->sentinelSize = pCmd->sentinelSize;

        tldev->traceBuffPaddr = pCmd->traceBuffPaddr;
        tldev->traceBuffSize  = pCmd->traceBuffSize;

        if (tracelog_init()) {
            LOGE("Failed to init tracelog driver");
            err = -EINVAL;
            break;
        }
        break;

    case SYS_MSG_TRACELOG_OFF:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen != 0) {
            LOGE("Invalid tracelog off cmd received");
            err = -EINVAL;
            break;
        }

        tracelog_exit();
        break;

    default:
        LOGW("Unknown tracelog msg %d", pHdr->ucType);
        err = -ENOENT;
    }
    return err;
}
