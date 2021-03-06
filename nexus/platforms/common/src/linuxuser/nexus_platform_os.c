/***************************************************************************
*     (c)2004-2014 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
*  and may only be used, duplicated, modified or distributed pursuant to the terms and
*  conditions of a separate, written license agreement executed between you and Broadcom
*  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
*  no license (express or implied), right to use, or waiver of any kind with respect to the
*  Software, and Broadcom expressly reserves all rights in and to the Software and all
*  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
*  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
*  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
*  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
*  and to use this information only in connection with your use of Broadcom integrated circuit products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
*  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
*  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
*  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
*  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
*  USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
*  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
*  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
*  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
*  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
*  ANY LIMITED REMEDY.
*
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* API Description:
*   API name: Platform linuxuser
*    linuxuser OS routines
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/

#include "nexus_platform.h"
#include "nexus_platform_priv.h"
#include "bkni.h"
#include "bcm_driver.h"
#include "bdbg_log.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <signal.h>
#include "nexus_base_statistics.h"


#if !B_REFSW_SYSTEM_MODE_SERVER
#include "nexus_platform_debug_log.h"
#include "nexus_core_utils.h"
#endif

BDBG_MODULE(nexus_platform_os);
BDBG_FILE_MODULE(nexus_statistics_isr);

#define BDBG_MSG_TRACE(X) /* BDBG_WRN(X) */
#define NEXUS_RUNAWAY_L1_THRESHOLD 10000

#ifdef B_REFSW_ANDROID
#define THREAD_STACK_SIZE (16*1024)
#else
#define THREAD_STACK_SIZE (8*1024)
#endif

#define B_ISR_STACK_GUARD   0
#define B_ISR_STACK_GUARD_STACK_SIZE    3000
/*
This fails on exit of the playback utility
#define B_ISR_STACK_GUARD_STACK_SIZE    2800
*/
#define B_ISR_STACK_MONITOR    0
#if B_ISR_STACK_GUARD
#define B_ISR_STACK_MONITOR_STACK_SIZE  (B_ISR_STACK_GUARD_STACK_SIZE - 768 /* flux for internal stack used by the STACK MONITOR itself */ )
#else
#define B_ISR_STACK_MONITOR_STACK_SIZE  4096
#endif

int g_NEXUS_driverFd = -1;

typedef struct IsrTableEntry
{
    const char *name;
    NEXUS_Core_InterruptFunction pFunction;
    void *pFuncParam;
    int iFuncParam;

    unsigned count;
    bool print;
} IsrTableEntry;

typedef struct NEXUS_Platform_Os_State {
    bool NEXUS_magnum_init;
    bool interruptDone;
    uint32_t mask[NEXUS_NUM_L1_REGISTERS];
#define NUM_IRQS (32*NEXUS_NUM_L1_REGISTERS)
    IsrTableEntry table[NUM_IRQS];
    NEXUS_TimerHandle debugTimer;
    pthread_t interruptThread;
    NEXUS_Platform_P_DebugLog debugLog;
    int memFd;
    int memFdCached;
    pthread_mutex_t lockUpdate32;
} NEXUS_Platform_Os_State;

static NEXUS_Platform_Os_State g_NEXUS_Platform_Os_State;

NEXUS_P_STACKDEPTH_AGGREGATE();
#if B_ISR_STACK_MONITOR
static unsigned *s_NEXUS_Platform_P_IsrStack;
static void NEXUS_Platform_P_IsrGetStack(void) {
   unsigned stack[B_ISR_STACK_MONITOR_STACK_SIZE/sizeof(unsigned)+64 /* flux to prevent NEXUS_Platform_P_IsrTestStack on stepping to itself */];
   s_NEXUS_Platform_P_IsrStack = stack;
   return ;
}

static size_t NEXUS_Platform_P_IsrTestStack(unsigned *stack,bool test)
{
    const unsigned pattern = 0x57ACDA7A;
    size_t result = 0;
    unsigned i;

    if(test) {
        /* find first modified entry on stack */
        for(i=0; i<B_ISR_STACK_MONITOR_STACK_SIZE/sizeof(unsigned);i++) {
            if(stack[i] != pattern) {
                result = B_ISR_STACK_MONITOR_STACK_SIZE - sizeof(unsigned)*i;
                break;
            }
        }
    }
    /* fill stack with pattern */
    for(i=0; i<B_ISR_STACK_MONITOR_STACK_SIZE/sizeof(unsigned);i++) {
        stack[i] = pattern;
    }
    BDBG_MSG_TRACE(("fill:%p..%p", stack, stack+i));
    return result;
}
static void (*s_NEXUS_Platform_P_IsrGetStack)(void) = NEXUS_Platform_P_IsrGetStack;
static size_t (*s_NEXUS_Platform_P_IsrTestStack)(unsigned *stack, bool test) = NEXUS_Platform_P_IsrTestStack;
#endif /* B_ISR_STACK_MONITOR */

#if B_REFSW_SYSTEM_MODE_CLIENT
extern void NEXUS_Platform_P_InterruptUpdate_isrsafe(void);
extern void NEXUS_Platform_P_WaitForInterrupt(tbcm_linux_dd_interrupt *isrData);
#else
static void NEXUS_Platform_P_InterruptUpdate_isrsafe(void)
{
    (void)ioctl(g_NEXUS_driverFd, BRCM_IOCTL_CHANGE_REQUEST, 0);
    return;
}
static void NEXUS_Platform_P_WaitForInterrupt(tbcm_linux_dd_interrupt *isrData)
{
    int rc;

    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_WAIT_FOR_INTERRUPTS, isrData);
    if ( rc ) {
        if ( errno != EIO )
            BERR_TRACE(BERR_OS_ERROR);
    }
    return;
}
#endif


BTRC_MODULE(L1_ISR, ENABLE);
static void NEXUS_Platform_P_IsrTask(void)
{
    tbcm_linux_dd_interrupt isrData;
    uint32_t status[NEXUS_NUM_L1_REGISTERS];
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
#if B_ISR_STACK_MONITOR
    size_t stackUsed;
    unsigned stackMonitorCount = 0;
#endif
    NEXUS_P_STACKDEPTH_STATE();

#if B_ISR_STACK_MONITOR
    s_NEXUS_Platform_P_IsrGetStack();
    s_NEXUS_Platform_P_IsrTestStack(s_NEXUS_Platform_P_IsrStack,false);

    BDBG_LOG(("isr_stack:%p", &isrData));
#endif

    BKNI_Memset(&isrData, 0, sizeof(isrData));
    isrData.timeout = 10000;

    while(!state->interruptDone) {
        unsigned i;
        uint32_t mask[NEXUS_NUM_L1_REGISTERS];
        uint32_t test_one = 0;

        for (i=0;i<NEXUS_NUM_L1_REGISTERS;i++) {
            mask[i] = state->mask[i];
            test_one |= mask[i];
        }

        if (!test_one)
        {
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 1000*1000*1;
            nanosleep(&ts, NULL);
            continue;
        }
        
        for (i=0;i<NEXUS_NUM_L1_REGISTERS;i++) {
            isrData.interruptmask[i] = mask[i];
            isrData.interruptstatus[i] = 0;
        }

        NEXUS_Platform_P_WaitForInterrupt(&isrData);
        /* If there's a timeout, then the system is not getting
        any interrupts. This isn't common, but isn't necessarily bad.
        Adding debug output here can cause a problem during system takedown. */

        /* If there is or isn't an error (which is most likely a timeout), the
        W0/W1 status returned is still good. */
        test_one = 0;
        for (i=0;i<NEXUS_NUM_L1_REGISTERS;i++) {
            status[i] = isrData.interruptstatus[i];
            test_one |= status[i];
            BDBG_MSG_TRACE(("isr status[%u]=%#x", i, status[i]));
        }

        /* Check for any pending interrupts */
        if( test_one )
        {
            NEXUS_P_STACKDEPTH_START();
            BKNI_EnterCriticalSection();
            BTRC_TRACE(L1_ISR, START);

            /* Reload mask to avoid stray interrupts */
            for (i=0;i<NEXUS_NUM_L1_REGISTERS;i++) {
                unsigned bit;
                mask[i] = state->mask[i];

                /* Scan all bits of each register */
                for ( bit = 0; bit < 32; bit++ )
                {
                    uint32_t statusMask = (1<<bit);
                    if ( status[i] & statusMask & mask[i] )
                    {
                        unsigned irq = i*32+bit;
                        IsrTableEntry *pEntry;

                        /* print on runaway L1 */
                        if (++state->table[irq].count % NEXUS_RUNAWAY_L1_THRESHOLD == 0) {
                            if (!state->table[irq].print) {
                                BDBG_WRN(("### %s (W%d, bit %d) fired %d times", state->table[irq].name, i, bit, state->table[irq].count));

                                state->table[irq].print = true; /* only print once to maximize chance that system keeps running */
                            }
                        }

                        pEntry = &state->table[irq];
                        if ( pEntry->pFunction )
                        {
                            pEntry->pFunction(pEntry->pFuncParam, pEntry->iFuncParam);
                        }
                        else
                        {
                            BDBG_ERR(("Stray Interrupt %d", irq));
                        }
                    }
                }
            }
            BTRC_TRACE(L1_ISR, STOP);
            BKNI_LeaveCriticalSection();
            NEXUS_P_STACKDEPTH_STOP();
#if B_ISR_STACK_MONITOR
            stackMonitorCount++;
            if(stackMonitorCount > 1000) {
                stackUsed = s_NEXUS_Platform_P_IsrTestStack(s_NEXUS_Platform_P_IsrStack,true);
                BDBG_MODULE_LOG(nexus_statistics_isr,("isr stack monitor %u%s", stackUsed,stackUsed>=B_ISR_STACK_MONITOR_STACK_SIZE?" MAX":""));
                stackMonitorCount = 0;
            }
#endif
        }
    }
#if NEXUS_P_STACKDEPTH_STATS
    BDBG_MODULE_LOG(nexus_statistics_isr,("isr stack usage max:%u", g_stackAggregate.max));
#endif
#if B_ISR_STACK_MONITOR
    if(stackMonitorCount) {
        stackUsed = s_NEXUS_Platform_P_IsrTestStack(s_NEXUS_Platform_P_IsrStack,true);
        BDBG_MODULE_LOG(nexus_statistics_isr,("isr stack monitor %u%s", stackUsed,stackUsed>=B_ISR_STACK_MONITOR_STACK_SIZE?" MAX":""));
    }
#endif
    NEXUS_BTRC_REPORT(L1_ISR);
    return;
}

#if B_ISR_STACK_GUARD
/* When running stack guard we start NEXUS_Platform_P_IsrTask in
 * a way that it has only B_ISR_STACK_GUARD_STACK_SIZE bytes for stack prior to hitting page that is inaccessible to write.
 * This way when ISR thread uses more then B_ISR_STACK_GUARD_STACK_SIZE, application crashes with 'Segmentation fault'.
 * Then generated coredump is likely to produce backtrace that would would point straight to the call-chain that has caused stack overflow.
 *
 * While in debugger (for example looking at backtrace), the following would print current stack pointer:
 * "print /x $sp"
 * And this, if current thread is ISR task, would print current stack depth (in bytes)
 * "print  s_NEXUS_IsrStack - $sp"
 *
 * Note:
 * To achieve this required layout we do peculiar stuff with stack alignment, and go extra steps to prevent compiler optimizations
 * from affecting this special layout.
 */

#include <alloca.h>
#include <sys/mman.h>


static void (*s_NEXUS_Platform_P_IsrTask)(void) = NEXUS_Platform_P_IsrTask;
static unsigned long s_NEXUS_IsrStack;

static void NEXUS_Platform_P_IsrLauncher(unsigned stackUse)
{
    uint8_t *extra = alloca(stackUse);
    unsigned long protected;

    BDBG_MSG(("ISR launcher:%p %p %p", &extra, extra + stackUse, extra));
    BKNI_Memset(extra, 0, stackUse);
    protected = (unsigned long)(extra - 4096);
    protected = protected&~(4095);
    BDBG_LOG(("ISR StackGuard: protected %#x..%#x", protected, protected+4096));
    s_NEXUS_IsrStack = (unsigned long)extra;
    mprotect((void *)protected, 4096, PROT_READ);
    BDBG_LOG(("ISR StackGuard: stack to use %#x...%#x %u", (unsigned long)extra, (unsigned long)protected, (unsigned long)extra - protected));
    s_NEXUS_Platform_P_IsrTask();
}
static void (*s_NEXUS_Platform_P_IsrLauncher)(unsigned stackUse) = NEXUS_Platform_P_IsrLauncher;

static void NEXUS_Platform_P_StartIsrTask(void)
{
    unsigned stack=0;
    unsigned long sp;
    unsigned stackSpacer;


    BDBG_CASSERT(B_ISR_STACK_GUARD_STACK_SIZE<4096);
    BDBG_MSG(("ISR thread:%p", &stack));
    sp = (unsigned long)&stack;
    stackSpacer = (sp&4095)+(4096 - B_ISR_STACK_GUARD_STACK_SIZE);
    BDBG_MSG(("stackSpacer %d sp:%#x(%#x)\n",stackSpacer, sp, sp-stackSpacer));

    s_NEXUS_Platform_P_IsrLauncher(stackSpacer);
}
#else /* B_ISR_STACK_GUARD_STACK_SIZE */
#define NEXUS_Platform_P_StartIsrTask NEXUS_Platform_P_IsrTask
#endif /* B_ISR_STACK_GUARD_STACK_SIZE */


static void *NEXUS_Platform_P_IsrThread(void *pParam)
{
    BSTD_UNUSED(pParam);
    prctl(PR_SET_NAME,"nx_platform_isr",0,0,0);
    NEXUS_Profile_MarkThread("isr");
    NEXUS_Platform_P_StartIsrTask();
    return NULL;
}

void NEXUS_Platform_P_MonitorOS(void)
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
    unsigned irq;
    for (irq=0;irq<NUM_IRQS;irq++) {
        if (state->table[irq].print) {
            unsigned i = irq / 32;
            unsigned bit = irq % 32;
            BDBG_WRN(("%s (W%d, bit %d) fired %d times", state->table[irq].name, i, bit, state->table[irq].count));
        }
    }
    BKNI_EnterCriticalSection();
    for (irq=0;irq<NUM_IRQS;irq++) {
        state->table[irq].count = 0;
        state->table[irq].print = false;
    }
    BKNI_LeaveCriticalSection();
}

#if !B_REFSW_SYSTEM_MODE_SERVER
static void NEXUS_Platform_P_DebugCallback(void *context, NEXUS_ModuleHandle handle, const char *name, const NEXUS_ModuleSettings *settings)
{
    if ( !strcmp( (const char *)context , "ls") && settings->dbgPrint ) {
        /* Print out nexus modules that have a "Print" module defined */
        BDBG_LOG(("%s", name ));     
    } 
    else if (!strcmp((const char *)context , "mma")) {
        NEXUS_Module_Lock(g_NEXUS_platformHandles.core);
        NEXUS_Core_DumpHeaps_priv(NULL);
        NEXUS_Module_Unlock(g_NEXUS_platformHandles.core);
    }
    else {
        if (settings->dbgModules && settings->dbgPrint){
            if (!strcmp(context, settings->dbgModules) || !strcmp(context, name) || !strcmp("all", context)) {
                /* We cannot lock platform, because this callback is from timer context with platform module already locked */
                if (handle != NEXUS_MODULE_SELF) NEXUS_Module_Lock(handle);
                settings->dbgPrint();
                if (handle != NEXUS_MODULE_SELF) NEXUS_Module_Unlock(handle);
            }
        }
    }
}

char findDelimiter( char *dbgInfo ) {

    const char delimiters[] = " .,;:";

    int dbgInfoLen = b_strlen( dbgInfo );
    int nrDelimiters=b_strlen( delimiters );
    int i,j;
    char rc = 0;

    for ( i=0; i < dbgInfoLen ; i++ ) {
        for ( j=0; j < nrDelimiters ; j++ ) {
            if ( delimiters[j] == *(dbgInfo+i) )  {
                break;
            }
        }
        if ( j < nrDelimiters ) {
            rc = (int)delimiters[j];
            break;
        }
    }
    return rc;
}

static void NEXUS_Platform_P_DebugTimer(void *context)
{
#define DEBUG_INFO_LEN 256
    char debug_info[DEBUG_INFO_LEN];
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
    int rc;
    BSTD_UNUSED(context);

    state->debugTimer = NULL;

#if B_REFSW_SYSTEM_MODE_CLIENT
    rc = -1;
#else
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_GET_DEBUG, debug_info);
#endif

    if(rc==0 && debug_info[0]){
        char delim = findDelimiter( debug_info );
        const char *debug_list = debug_info;
        for(;;) {
            char buf[DEBUG_INFO_LEN];
            char *end = NULL;
            size_t name_len;

            if ( delim ) {
                end = strchr(debug_list, delim );
            }

            if (!end) {
                if(*debug_list) {
                    NEXUS_Module_EnumerateAll(NEXUS_Platform_P_DebugCallback, (char *)debug_list);
                }
                break;
            }
            name_len = end-debug_list;
            if(name_len>0 && name_len<sizeof(buf)) {
                strncpy(buf, debug_list, name_len);
                buf[name_len] = '\0';
                NEXUS_Module_EnumerateAll(NEXUS_Platform_P_DebugCallback, buf);
            }
            debug_list = end+1;
        }
    }
    state->debugTimer = NEXUS_ScheduleTimer(500, NEXUS_Platform_P_DebugTimer, NULL);
#if NEXUS_P_STACKDEPTH_STATS
    {
        static int timer_tick = 0;
        if(timer_tick++>10) {
            unsigned count, max, average;
            timer_tick = 0;
            BKNI_EnterCriticalSection();
            count = g_stackAggregate.count;
            max = g_stackAggregate.max;
            average = count ? g_stackAggregate.total/count : 0;
            g_stackAggregate.count = 0;
            g_stackAggregate.max = 0;
            g_stackAggregate.total = 0;
            BKNI_LeaveCriticalSection();
            BDBG_MODULE_LOG(nexus_statistics_isr,("isr stack usage max:%u average:%u count:%u", max, average, count));
        }
    }
#endif /* NEXUS_P_STACKDEPTH_STATS */
    return;
}
#endif /* !B_REFSW_SYSTEM_MODE_SERVER */

NEXUS_Error NEXUS_Platform_P_InitOSMem(void)
{
    int rc;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
#if B_REFSW_SYSTEM_MODE_CLIENT
    static const char memDev[] = "/dev/null";
#else
    static const char memDev[] = "/dev/brcm0";
#endif

    /* Open /dev/mem for memory mapping */
    state->memFd = open(memDev, O_RDWR|O_SYNC);
    if ( state->memFd < 0 )
    {
        BDBG_ERR(("Unable to open %s for uncached memory: %d", memDev, errno));
        return BERR_TRACE(BERR_OS_ERROR);
    }
    rc = fcntl(state->memFd, F_SETFD, FD_CLOEXEC);
    if (rc) BERR_TRACE(rc); /* keep going */
    state->memFdCached = open(memDev, O_RDWR);
    if ( state->memFdCached < 0 )
    {
        BDBG_ERR(("Unable to open %s for cached memory: %d", memDev, errno));
        close(state->memFd);
        return BERR_TRACE(BERR_OS_ERROR);
    }
    rc = fcntl(state->memFdCached, F_SETFD, FD_CLOEXEC);
    if (rc) BERR_TRACE(rc); /* keep going */
#if B_REFSW_SYSTEM_MODE_SERVER
    g_NEXUS_driverFd = state->memFdCached;
#endif

#if NEXUS_USE_CMA
    rc = NEXUS_Platform_P_InitCma();
    if (rc) return BERR_TRACE(rc);
#endif

    return 0;
}

void NEXUS_Platform_P_UninitOSMem(void)
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

#if NEXUS_USE_CMA
    NEXUS_Platform_P_UnInitCma();
#endif
    close(state->memFdCached);
    close(state->memFd);
    return;
}

NEXUS_Error NEXUS_Platform_P_InitOS(void)
{
    int rc;
    pthread_attr_t threadAttr;
    struct sched_param schedParam;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    NEXUS_Platform_P_DebugLog_Init(&state->debugLog, NULL);

    /* Open user-mode driver */
    g_NEXUS_driverFd = open(
#if B_REFSW_SYSTEM_MODE_CLIENT
                            "/dev/null"
#else
                            "/dev/brcm0"
#endif
                            , O_RDWR);
    if ( g_NEXUS_driverFd < 0 )
    {
        BDBG_ERR(("Unable to open user-mode driver"));
        /* give message which points to solution */
        switch (errno)
        {
        case ENXIO:
            BDBG_ERR(("bcmdriver has not been installed. Are you running the nexus script?"));
            break;
        case ENOENT:
            BDBG_ERR(("/dev/brcm0 does not exist. Are you running the nexus script?"));
            break;
        default:
            BDBG_ERR(("/dev/brcm0 error: %d", errno));
            break;
        }
        rc = BERR_TRACE(BERR_OS_ERROR);
        goto err_bcmdriver_open;
    }
#if !B_REFSW_SYSTEM_MODE_CLIENT
    {
        struct bcmdriver_version get_version;
        get_version.version = 0;
        rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_GET_VERSION, &get_version);
        if (rc!=0 || get_version.version != BCMDRIVER_VERSION) {
            BDBG_ERR(("Not supported bcmdriver version %u != %u", get_version.version, BCMDRIVER_VERSION));
            rc = BERR_TRACE(BERR_OS_ERROR);
#if BCHP_CHIP != 11360
            return rc;
#endif
        }
    }
#endif
    rc = fcntl(g_NEXUS_driverFd, F_SETFD, FD_CLOEXEC);
    if (rc) BERR_TRACE(rc); /* keep going */
#if B_REFSW_SYSTEM_MODE_CLIENT
    g_platformMemory.max_dcache_line_size = 4096;
#else
    if ( ioctl(g_NEXUS_driverFd, BRCM_IOCTL_INT_RESET, 0) )
    {
        BDBG_ERR(("cannot reset interrupts. another instance of nexus is probably running."));
        rc = BERR_TRACE(BERR_OS_ERROR);
        goto err_bcmdriver_setup;
    }

    /* If MEM's alignment is not set to the MIPS L1 and (if present) L2 cache line size,
    we will have cache coherency problems (which lead to major system failures).
    This code verifies that Nexus's MEM configuration is compatible with the MIPS cache line size.
    If this code fails, please check to make sure the Linux kernel is configured right, then modify nexus_core_features.h to match.
    use g_platformMemory to pass OS value to NEXUS_Platform_P_SetCoreModuleSettings */
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_GET_DCACHE_LINE_SIZE, &g_platformMemory.max_dcache_line_size);
    if (rc) {
        BDBG_ERR(("Nexus requires a new bcmdriver.ko ioctl API BRCM_IOCTL_GET_DCACHE_LINE_SIZE. Are you running with an old bcmdriver.ko?"));
        rc = BERR_TRACE(BERR_OS_ERROR);
        goto err_bcmdriver_setup;
    }
#endif

    /* Launch interrupt thread */
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
    if (!NEXUS_GetEnv("not_realtime_isr")) {
        pthread_attr_setschedpolicy(&threadAttr, SCHED_FIFO);
        pthread_attr_getschedparam(&threadAttr, &schedParam);
        schedParam.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_attr_setschedparam(&threadAttr, &schedParam);
    }
    pthread_attr_setstacksize(&threadAttr, THREAD_STACK_SIZE);
    BDBG_MSG(("Launching ISR task"));
    rc = pthread_create(&state->interruptThread,
                        &threadAttr,
                        NEXUS_Platform_P_IsrThread,
                        NULL);
    if ( rc )
    {
        BDBG_ERR(("Unable to create ISR task"));
        BERR_TRACE(BERR_OS_ERROR);
        goto err_thread;
    }
    if (!NEXUS_GetEnv("not_realtime_isr")) {
        pthread_setschedparam(state->interruptThread, SCHED_FIFO, &schedParam);
    }

    pthread_mutex_init(&state->lockUpdate32, NULL);

#if !B_REFSW_SYSTEM_MODE_SERVER
    state->debugTimer = NEXUS_ScheduleTimer(500, NEXUS_Platform_P_DebugTimer, NULL);
#endif

#if NEXUS_CPU_ARM && NEXUS_POWER_MANAGEMENT && !B_REFSW_SYSTEM_MODE_CLIENT
    if (!g_NEXUS_platformHandles.baseOnlyInit) {
        (void)NEXUS_Platform_P_InitWakeupDriver();
    }
#endif
    /* Success */
    return BERR_SUCCESS;

err_thread:
#if !B_REFSW_SYSTEM_MODE_CLIENT
err_bcmdriver_setup:
#endif
    close(g_NEXUS_driverFd);
    g_NEXUS_driverFd = -1;
err_bcmdriver_open:

    /* Failed */
    return BERR_OS_ERROR;
}

NEXUS_Error NEXUS_Platform_P_UninitOS(void)
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

#if NEXUS_CPU_ARM && NEXUS_POWER_MANAGEMENT && !B_REFSW_SYSTEM_MODE_CLIENT
    if (!g_NEXUS_platformHandles.baseOnlyInit) {
        NEXUS_Platform_P_UninitWakeupDriver();
    }
#endif

#if !B_REFSW_SYSTEM_MODE_SERVER
    if(state->debugTimer) {
        NEXUS_CancelTimer(state->debugTimer);
        state->debugTimer = NULL;
    }
#endif
    pthread_mutex_destroy(&state->lockUpdate32);

    state->interruptDone = true;
    /* BRCM_IOCTL_CHANGE_REQUEST is needed to force BRCM_IOCTL_WAIT_FOR_INTERRUPTS to return immediately */
    NEXUS_Platform_P_InterruptUpdate_isrsafe();
    pthread_join(state->interruptThread, NULL);

    close(g_NEXUS_driverFd);
    NEXUS_Platform_P_DebugLog_Uninit(&state->debugLog);
    return BERR_SUCCESS;
}

#if !NEXUS_USE_CMA
NEXUS_Error NEXUS_Platform_P_GetHostMemory(NEXUS_PlatformMemory *pMemory)
{
    unsigned total = 0;
    /* try 2.6.31 filesystem first. we cannot do a compile-time test for linux kernel version here
    because it's linux user mode. */
    {
        unsigned i;
        BDBG_CASSERT(NEXUS_MAX_HEAPS);
        for (i=0;i<NEXUS_MAX_HEAPS;i++) {
            char buf[256];
            const char *bmem_dir = NEXUS_GetEnv("bmem_override");
            FILE *f;
            unsigned n, base, length;

            if (!bmem_dir) {
                /* normal sys fs directory */
                bmem_dir = "/sys/devices/platform/brcmstb";
            }

            BKNI_Snprintf(buf, 256, "%s/bmem.%d", bmem_dir, i);
            f = fopen(buf, "r");
            if (!f) break;
            if (!fgets(buf, 256, f)) {
                n = 0;
            }
            else {
                buf[255] = 0; /* ensure no memory overrun */
                n = sscanf(buf, "0x%x 0x%x", &base, &length);
            }
            fclose(f);
            /* even on MEMC0, base should never be 0 because it is used by the OS */
            if (n == 2 && base && length) {
                /* we got it */
                pMemory->osRegion[total].base = base;
                pMemory->osRegion[total].length = length;
                total++;
            }
            else {
                break;
            }
        }
    }

    /* if there are no hits, try 2.6.18 file system */
    if (!total) {
        const char bmeminfo2618[] =  "/proc/bcmdriver/meminfo";
        FILE *pFile;
        pFile = fopen(bmeminfo2618, "r");
        if (pFile) {
            char buf[512];
            char *pStart, *pEnd;

            if ( fgets(buf, sizeof(buf), pFile) )
            {
                pStart = &buf[13];      /* "13" should point us to the "space" after "Kernel Memory " see umdrv.c */
                pStart += strspn(pStart, " ");
                pEnd = strchr(pStart, ' ');
                if ( pEnd )
                {
                    *pEnd = 0;
                    pMemory->osRegion[total].base = atoi(pStart);
                    /* size is unknown, so hardcode a large number and rely on NEXUS_Platform_P_AdjustBmemRegions to reduce */
                    pMemory->osRegion[total].length = 256*1024*1024;
                    total++;
                }
            }
            fclose(pFile);
        }
    }
    if (!total) {
        BDBG_ERR(("unable to learn OS memory"));
        return BERR_TRACE(NEXUS_UNKNOWN);
    }
    return 0;
}
#endif


#if !B_REFSW_SYSTEM_MODE_SERVER
static NEXUS_Error NEXUS_Platform_P_ConnectInterrupt_isr(
    unsigned irqNum,
    NEXUS_Core_InterruptFunction pIsrFunc,
    void *pFuncParam,
    int iFuncParam
    )
{
    IsrTableEntry *pEntry;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    /* Connect does not automatically enable the interrupt */
    if (irqNum >= NUM_IRQS) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }
    pEntry = &state->table[irqNum];
    if (pEntry->pFunction) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }
    pEntry->pFunction = pIsrFunc;
    pEntry->pFuncParam = pFuncParam;
    pEntry->iFuncParam = iFuncParam;

    {
        const BINT_P_IntMap *intMap;
        unsigned i;
        /* use BINT's record of managed L2's (and their corresponding L1's) to validate the L1 connect */
        intMap = g_pCoreHandles->bint_map;
        BDBG_ASSERT(intMap);

        pEntry->name = "unknown";

        /* find the first L2 that has this L1 */
        for (i=0;intMap[i].L1Shift!=-1;i++) {
            if (BINT_MAP_GET_L1SHIFT(&intMap[i]) == irqNum) {
                pEntry->name = intMap[i].L2Name; /* use BINT's L2 name for the L1 name. in most cases it is a meaningful name. */
                break;
            }
        }
    }

    return BERR_SUCCESS;
}


/***************************************************************************
Summary:
    Hook to connect a L1 interrupt to the OS
See Also:
    NEXUS_Platform_P_DisconnectInterrupt
 ***************************************************************************/
NEXUS_Error NEXUS_Platform_P_ConnectInterrupt(
    unsigned irqNum,
    NEXUS_Core_InterruptFunction pIsrFunc,
    void *pFuncParam,
    int iFuncParam
    )
{
    NEXUS_Error errCode;

    BKNI_EnterCriticalSection();
    errCode = NEXUS_Platform_P_ConnectInterrupt_isr(irqNum, pIsrFunc, pFuncParam, iFuncParam);
    BKNI_LeaveCriticalSection();

    return errCode;
}

static void NEXUS_Platform_P_DisconnectInterrupt_isr(
    unsigned irqNum
    )
{
    IsrTableEntry *pEntry;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
    
    NEXUS_Platform_P_DisableInterrupt_isr(irqNum);

    if (irqNum >= NUM_IRQS) {
        BERR_TRACE(NEXUS_INVALID_PARAMETER);
        return;
    }
    pEntry = &state->table[irqNum];
    pEntry->pFunction = NULL;
    pEntry->pFuncParam = NULL;
    pEntry->iFuncParam = 0;
}


/***************************************************************************
Summary:
    Hook to disconnect a L1 interrupt from the OS
See Also:
    NEXUS_Platform_P_ConnectInterrupt
 ***************************************************************************/
void NEXUS_Platform_P_DisconnectInterrupt(
    unsigned irqNum
    )
{
    BKNI_EnterCriticalSection();
    NEXUS_Platform_P_DisconnectInterrupt_isr(irqNum);
    BKNI_LeaveCriticalSection();
}

/***************************************************************************
Summary:
    Hook to enable an L1 interrupt
See Also:
    NEXUS_Platform_P_DisableInterrupt_isr
 ***************************************************************************/
NEXUS_Error NEXUS_Platform_P_EnableInterrupt(
    unsigned irqNum
    )
{
    NEXUS_Error errCode;

    BKNI_EnterCriticalSection();
    errCode = NEXUS_Platform_P_EnableInterrupt_isr(irqNum);
    BKNI_LeaveCriticalSection();

    return errCode;
}

/***************************************************************************
Summary:
    Hook to enable an L1 interrupt
See Also:
    NEXUS_Platform_P_DisableInterrupt_isr
 ***************************************************************************/
NEXUS_Error NEXUS_Platform_P_EnableInterrupt_isr(
    unsigned irqNum
    )
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    if (irqNum >= NUM_IRQS) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }
    state->mask[irqNum/32] |= 1<<(irqNum%32);

    NEXUS_Platform_P_InterruptUpdate_isrsafe();

    return BERR_SUCCESS;
}

/***************************************************************************
Summary:
    Hook to disable an L1 interrupt
See Also:
    NEXUS_Platform_P_EnableInterrupt_isr
 ***************************************************************************/
void NEXUS_Platform_P_DisableInterrupt(
    unsigned irqNum
    )
{
    BKNI_EnterCriticalSection();
    NEXUS_Platform_P_DisableInterrupt_isr(irqNum);
    BKNI_LeaveCriticalSection();
}

/***************************************************************************
Summary:
    Hook to disable an L1 interrupt
See Also:
    NEXUS_Platform_P_EnableInterrupt_isr
 ***************************************************************************/
void NEXUS_Platform_P_DisableInterrupt_isr(
    unsigned irqNum
    )
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    if (irqNum >= NUM_IRQS) {
        BERR_TRACE(NEXUS_INVALID_PARAMETER);
        return;
    }
    state->mask[irqNum/32] &= ~(1<<(irqNum%32));

    NEXUS_Platform_P_InterruptUpdate_isrsafe();
}
#endif /* !B_REFSW_SYSTEM_MODE_SERVER */

/***************************************************************************
Summary:
    Map physical memory into virtual space
Returns:
    Valid address on success, NULL for failure.
See Also:
    NEXUS_Platform_P_UnmapMemory
 ***************************************************************************/
void *NEXUS_Platform_P_MapMemory(
    NEXUS_Addr offset,
    size_t length,
    NEXUS_MemoryMapType type)
{
    void *pMem;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
    int memFd = (type == NEXUS_MemoryMapType_eCached) ? state->memFdCached : state->memFd;
    int flags = MAP_SHARED;

    BDBG_ASSERT(length > 0);
#if B_REFSW_SYSTEM_MODE_CLIENT
    flags = MAP_PRIVATE|MAP_ANONYMOUS;
#endif

    pMem = mmap64(0, length, PROT_READ|PROT_WRITE, flags, memFd, offset);
    BDBG_MSG(("mmap offset=" BDBG_UINT64_FMT "=>%p size=%u fd=%d", BDBG_UINT64_ARG(offset), pMem, (unsigned)length, memFd));

    if ( MAP_FAILED == pMem )
    {
        BDBG_ERR(("mmap failed: offset=" BDBG_UINT64_FMT " size=%u (%s)", BDBG_UINT64_ARG(offset), (unsigned)length, type==NEXUS_MemoryMapType_eCached?"cached":"uncached"));
        BERR_TRACE(BERR_OS_ERROR);
        pMem = NULL;
    }
#if B_REFSW_SYSTEM_MODE_CLIENT
    if(pMem) {
        NEXUS_Platform_P_ClientMapMemory(pMem, length, offset, type == NEXUS_MemoryMapType_eCached);
    }
#endif

    return pMem;
}

/***************************************************************************
Summary:
    Unmap a virtual address
See Also:
    NEXUS_Platform_P_MapMemory
 ***************************************************************************/
void NEXUS_Platform_P_UnmapMemory(
    void *pMem,
    size_t length,
    NEXUS_MemoryMapType type
    )
{
    BDBG_MSG(("unmap: addr:%p size:%u", pMem, (unsigned)length));
    BDBG_ASSERT(NULL != pMem);
    BSTD_UNUSED(type);
#if B_REFSW_SYSTEM_MODE_CLIENT
    NEXUS_Platform_P_ClientUnmapMemory(pMem,length);
#endif
    (void)munmap(pMem, length);
}

/* in userspace, this is the same */
void *NEXUS_Platform_P_MapRegisterMemory(unsigned long offset, unsigned long length)
{
    return NEXUS_Platform_P_MapMemory(offset,length,false);
}

void NEXUS_Platform_P_UnmapRegisterMemory(void *pMem,unsigned long length)
{
    NEXUS_Platform_P_UnmapMemory(pMem, length, NEXUS_MemoryMapType_eUncached);
    return;
}

/***************************************************************************
Summary:
    Reset any pending L1 interrupts
 ***************************************************************************/
#if !B_REFSW_SYSTEM_MODE_CLIENT
void NEXUS_Platform_P_ResetInterrupts(void)
{
    if ( ioctl(g_NEXUS_driverFd, BRCM_IOCTL_INT_RESET, 0) )
    {
        BERR_TRACE(BERR_OS_ERROR);
    }
}
#endif

/* This function is dangerous and not meant for general use. It is needed for
test code that needs a special ioctl to the usermode driver. It must be extern'd. */
int b_get_umdrv_fd(void)
{
    return g_NEXUS_driverFd;
}

void NEXUS_Platform_P_Os_SystemUpdate32_isrsafe(const NEXUS_Core_PreInitState *preInitState, uint32_t reg, uint32_t mask, uint32_t value, bool systemRegister)
{
#if !NEXUS_CPU_ARM
    systemRegister = true;
#endif
    if(systemRegister) {
        /* serialize magnum and the OS using an OS-specific lock */
        t_bcm_linux_dd_atomic_update data;
        int rc;

        data.reg = reg;
        data.mask = mask;
        data.value = value;

        BDBG_MSG_TRACE(("atomic update: reg=%08x %08x %08x", reg, mask, value));

        rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_ATOMIC_UPDATE, &data);
        if (rc) {rc = BERR_TRACE(rc);} /* REG basemodule can't do anything with the error, so fallthrough */
    } else {
        NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;
        pthread_mutex_lock(&state->lockUpdate32);
        preInitState->privateState.regSettings.systemUpdate32_isrsafe(preInitState->hReg, reg, mask, value, false);
        pthread_mutex_unlock(&state->lockUpdate32);
    }
    return;
}

NEXUS_Error
NEXUS_Platform_P_Magnum_Init(void)
{
    BERR_Code rc;
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    if(!state->NEXUS_magnum_init) {
        BKNI_Memset(state, 0, sizeof(*state));
        rc = BKNI_Init();
        if(rc!=BERR_SUCCESS) {return BERR_TRACE(rc);}
        rc = BDBG_Init();
        if(rc!=BERR_SUCCESS) {rc=BERR_TRACE(rc); BKNI_Uninit();return rc;}
        rc = NEXUS_Base_Core_Init();
        if(rc!=BERR_SUCCESS) {rc=BERR_TRACE(rc); BDBG_Uninit();BKNI_Uninit();return rc;}
        state->NEXUS_magnum_init = true;
    }
    return BERR_SUCCESS;
}

void
NEXUS_Platform_P_Magnum_Uninit(void)
{
    NEXUS_Platform_Os_State *state = &g_NEXUS_Platform_Os_State;

    if(state->NEXUS_magnum_init) {
        NEXUS_Base_Core_Uninit();
        BDBG_Uninit();
        BKNI_Uninit();
        BKNI_Memset(state, 0, sizeof(*state));
    }
    return;
}

#if NEXUS_HAS_AUDIO && NEXUS_HAS_SOFT_AUDIO
/***************************************************************************
Summary:
Read reserved memory
***************************************************************************/
uint32_t NEXUS_Platform_P_ReadReserved(
    uint32_t physicalAddress
    )
{
    t_bcm_linux_rw_reserved res;

    res.address = physicalAddress;
    res.value = 0;
    if (ioctl(g_NEXUS_driverFd, BRCM_IOCTL_READ_RESERVED, &res))
        BERR_TRACE(BERR_OS_ERROR);
    return res.value;
}

/***************************************************************************
Summary:
Write reserved memory
***************************************************************************/
void NEXUS_Platform_P_WriteReserved(
    uint32_t physicalAddress,
    uint32_t value
    )
{
    t_bcm_linux_rw_reserved res;

    res.address = physicalAddress;
    res.value = value;
    if (ioctl(g_NEXUS_driverFd, BRCM_IOCTL_WRITE_RESERVED, &res))
        BERR_TRACE(BERR_OS_ERROR);
}

/***************************************************************************
Summary:
Read core register
***************************************************************************/
uint32_t NEXUS_Platform_P_ReadCoreReg(
    uint32_t offset
    )
{
    t_bcm_linux_rw_core_register reg;

    reg.offset = offset;
    reg.value = 0;
    if (ioctl(g_NEXUS_driverFd, BRCM_IOCTL_READ_CORE_REGISTER, &reg))
        BERR_TRACE(BERR_OS_ERROR);
    return reg.value;
}

/***************************************************************************
Summary:
Write core register
***************************************************************************/
void NEXUS_Platform_P_WriteCoreReg(
    uint32_t offset,
    uint32_t value
    )
{
    t_bcm_linux_rw_core_register reg;

    reg.offset = offset;
    reg.value = value;
    if (ioctl(g_NEXUS_driverFd, BRCM_IOCTL_WRITE_CORE_REGISTER, &reg))
        BERR_TRACE(BERR_OS_ERROR);
}

/***************************************************************************
Summary:
Read CMT Control Register
***************************************************************************/
uint32_t NEXUS_Platform_P_ReadCmtControl(void)
{
    int rc;
    uint32_t value;
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_READ_CMT_CONTROL, &value);
    if(rc!=0) {
        (void)BERR_TRACE(NEXUS_OS_ERROR);
        value = 0;
    }
    return value;
}

/***************************************************************************
Summary:
Write CMT Control Register
***************************************************************************/
void NEXUS_Platform_P_WriteCmtControl(
    uint32_t value
    )
{
    int rc;
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_WRITE_CMT_CONTROL, value);
    if(rc!=0) {
        (void)BERR_TRACE(NEXUS_OS_ERROR);
    }
    return;
}
#endif

#if B_HAS_BPROFILE || B_HAS_TRC
#include "bperf_counter.h"
#ifdef b_perf_write_cfg
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "bcm_driver.h"
void b_perf_write_cfg(unsigned select, unsigned data)
{
    int driver_fd = g_NEXUS_driverFd;
    int rc;
    t_bcm_linux_write_cp0_25 arg;

    arg.select = select;
    arg.value = data;
    rc = ioctl(driver_fd, BRCM_IOCTL_WRITE_CP0_25, &arg);
    if(rc<0) {(void)BERR_TRACE(BERR_OS_ERROR);goto err_ioctl;}
err_ioctl:
    return;
}
#endif /*b_perf_write_cfg*/
#endif /* B_HAS_BPROFILE || B_HAS_TRC */

void NEXUS_Platform_P_StopCallbacks(void *interfaceHandle)
{
    NEXUS_Base_P_StopCallbacks(interfaceHandle);
    return;
}

void NEXUS_Platform_P_StartCallbacks(void *interfaceHandle)
{
    NEXUS_Base_P_StartCallbacks(interfaceHandle);
    return;
}

#if !B_REFSW_SYSTEM_MODE_SERVER
void NEXUS_Platform_P_AddBoardStatus(NEXUS_PlatformStatus *pStatus)
{
    unsigned id = NEXUS_Platform_P_ReadBoardId();
    pStatus->boardId.major = id >> 28;
    pStatus->boardId.minor = (id >> 24) & 0xF;
}
#endif

NEXUS_Error NEXUS_Platform_P_SetStandbyExclusionRegion(unsigned heapIndex)
{
#if !B_REFSW_SYSTEM_MODE_SERVER && !B_REFSW_SYSTEM_MODE_CLIENT
    t_bcm_linux_pm_mem_exclude s;
    NEXUS_MemoryStatus status;
    int rc;

    if (!g_pCoreHandles->heap[heapIndex].nexus) {
        return NEXUS_SUCCESS;
    }
    rc = NEXUS_Heap_GetStatus(g_pCoreHandles->heap[heapIndex].nexus, &status);
    if (rc) return BERR_TRACE(rc);

    s.addr = status.offset;
    s.len = status.size;
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_PM_MEM_EXCLUDE, &s);
    if (rc) return BERR_TRACE(rc);
#endif
    BSTD_UNUSED(heapIndex);
    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_Platform_P_AddDynamicRegion(NEXUS_Addr addr, unsigned size)
{
    int rc;
    struct bcmdriver_dynamic_region region;

    region.base = addr;
    region.length = size;
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_DYNAMIC_REGION_ADD, &region);
    if(rc!=0) {return BERR_TRACE(BERR_NOT_SUPPORTED);}
    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_Platform_P_RemoveDynamicRegion(NEXUS_Addr addr, unsigned size)
{
    int rc;
    struct bcmdriver_dynamic_region region;

    region.base = addr;
    region.length = size;
    rc = ioctl(g_NEXUS_driverFd, BRCM_IOCTL_DYNAMIC_REGION_REMOVE, &region);
    if(rc!=0) {return BERR_TRACE(BERR_NOT_SUPPORTED);}
    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_Platform_P_InitializeThermalMonitor(void)
{
#if NEXUS_CPU_ARM && NEXUS_POWER_MANAGEMENT && !B_REFSW_SYSTEM_MODE_CLIENT
    int rc;

    if (!g_NEXUS_platformHandles.baseOnlyInit) {
        rc = NEXUS_Platform_P_InitThermalMonitor();
        if (rc) return BERR_TRACE(rc);
    }
#endif
    return NEXUS_SUCCESS;
}

void NEXUS_Platform_P_UninitializeThermalMonitor(void)
{
#if NEXUS_CPU_ARM && NEXUS_POWER_MANAGEMENT && !B_REFSW_SYSTEM_MODE_CLIENT
    if (!g_NEXUS_platformHandles.baseOnlyInit) {
        NEXUS_Platform_P_UninitThermalMonitor();
    }
#endif
}
