/***************************************************************************
 * Copyright (C) 2018 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 *
 * Module Description: Application to generate RULs for splash screen.
 *                     This is a slightly modified copy of vdc_dump.c
 *                     ported to Nucleus
 *
 ***************************************************************************/
#ifndef BSPLASH_BOARD_H__
#define BSPLASH_BOARD_H__

/* For register exclusions */
#include "bchp_common.h"
#include "bchp_irq0.h"
#include "bchp_scirq0.h"
#include "bchp_ebi.h"
#include "bchp_gio.h"
#include "bchp_rdc.h"

/* These are the registers that need to be excluded from the register dump either
   because they interrupt the CPU or disturb settings done elsewhere like the CFE
   If you do not want to program certain registers - add them to this macro.
*/
#define BSPLASH_ADDRESS_IN_RANGE(addr, rangeLow, rangeHigh) \
	(((addr)>=(rangeLow)) && ((addr) <=(rangeHigh)))

#if (BCHP_VER >= BCHP_VER_D0)
#define BSPLASH_ADDRESS_IN_RANGE_HVD(addr) ( \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_HVD_INTR2_0_REG_START, BCHP_HVD_INTR2_0_REG_END)  || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_HVD_INTR2_1_REG_START, BCHP_HVD_INTR2_1_REG_END)  || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_HVD_INTR2_2_REG_START, BCHP_HVD_INTR2_2_REG_END))
#else
#define BSPLASH_ADDRESS_IN_RANGE_HVD(addr) ( \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_SHVD_INTR2_0_REG_START, BCHP_HVD_INTR2_0_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_SHVD_INTR2_1_REG_START, BCHP_HVD_INTR2_1_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_SHVD_INTR2_2_REG_START, BCHP_HVD_INTR2_2_REG_END))
#endif

#define BSPLASH_REGDUMP_EXCLUDE(addr)   ( \
	BSPLASH_ADDRESS_IN_RANGE_HVD(addr) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_BVNF_INTR2_0_REG_START, BCHP_BVNF_INTR2_5_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_BVNB_INTR2_REG_START, BCHP_BVNB_INTR2_REG_END)     || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_HIF_INTR2_REG_START, BCHP_HIF_CPU_INTR1_REG_END)   || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_XPT_BUS_IF_REG_START, BCHP_XPT_XPU_REG_END)        || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_RAAGA_DSP_INTH_REG_START, BCHP_RAAGA_DSP_INTH_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_M2MC_L2_REG_START, BCHP_M2MC_L2_REG_END)  || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_M2MC1_L2_REG_START, BCHP_M2MC1_L2_REG_END)  || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_HDMI_TX_INTR2_REG_START, BCHP_HDMI_TX_INTR2_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_VIDEO_ENC_INTR2_REG_START, BCHP_VIDEO_ENC_INTR2_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_BVNB_INTR2_REG_START, BCHP_BVNB_INTR2_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_MEMC_ARC_0_REG_START, BCHP_MEMC_ARC_0_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_MEMC_ARC_1_REG_START, BCHP_MEMC_ARC_1_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_MEMC_ARC_2_REG_START, BCHP_MEMC_ARC_2_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_EBI_CS_BASE_0, BCHP_EBI_ECR) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_TIMER_REG_START, BCHP_TIMER_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_GIO_ODEN_LO, BCHP_GIO_STAT_EXT) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_IRQ0_IRQEN, BCHP_IRQ0_IRQEN) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_SCIRQ0_SCIRQEN, BCHP_SCIRQ0_SCIRQEN) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_TIMER_TIMER_IE0, BCHP_TIMER_TIMER_IE0) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_RAAGA_DSP_SEC0_REG_START, BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_REG_END)   || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_BSP_CMDBUF_REG_START, BCHP_XPT_SECURITY_NS_REG_END)   || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_RDC_REG_START, BCHP_RDC_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_MEMC_L2_1_0_REG_START, BCHP_MEMC_L2_1_0_REG_END) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_SUN_TOP_CTRL_PIN_MUX_CTRL_15, BCHP_SUN_TOP_CTRL_PIN_MUX_CTRL_15) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_AON_PIN_CTRL_PIN_MUX_CTRL_0, BCHP_AON_PIN_CTRL_PIN_MUX_CTRL_0) || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_AVS_CPU_PROG_MEM_REG_START, BCHP_AVS_PMB_REGISTERS_REG_END)  || \
	BSPLASH_ADDRESS_IN_RANGE(addr, BCHP_AON_CTRL_SYSTEM_DATA_RAMi_ARRAY_BASE , (BCHP_AON_CTRL_SYSTEM_DATA_RAMi_ARRAY_BASE + (15) * 4)) \
)

/* number of mem heap handles  */
#define SPLASH_NUM_MEM        3

/* heap for RUL bufs */
#define SPLASH_RUL_MEM        2   /* idx to BMMA_Handle array */

/* number of surface */
#define SPLASH_NUM_SURFACE    2

/* surface 0 info */
#define SPLASH_SURF0_MEM      2   /* idx to BMMA_Handle array */
#define SPLASH_SURF0_PXL_FMT  BPXL_eR5_G6_B5
#define SPLASH_SURF0_BMP      "splash.bmp"

/* surface 1 info */
#define SPLASH_SURF1_MEM      2   /* idx to BMMA_Handle array */
#define SPLASH_SURF1_PXL_FMT  BPXL_eR5_G6_B5
#define SPLASH_SURF1_BMP      "splash.bmp"

/* number of display */
#define SPLASH_NUM_DISPLAY    2

#ifdef  CFG_SPLASH_PAL
/* display 0 info */
#define SPLASH_DISP0_FMT      BFMT_VideoFmt_e576p_50Hz
#define SPLASH_DISP0_SUR      0   /* idx to splash surface buffer array */

/* display 1 info */
#define SPLASH_DISP1_FMT      BFMT_VideoFmt_ePAL_I
#define SPLASH_DISP1_SUR      1   /* idx to splash surface buffer array */

#else
/* display 0 info */
#define SPLASH_DISP0_FMT      BFMT_VideoFmt_e480p
#define SPLASH_DISP0_SUR      0   /* idx to splash surface buffer array */

/* display 1 info */
#define SPLASH_DISP1_FMT      BFMT_VideoFmt_eNTSC
#define SPLASH_DISP1_SUR      1   /* idx to splash surface buffer array */
#endif

#define SPLASH_NUM_SVIDEO_OUTPUTS      0
#define SPLASH_NUM_COMPOSITE_OUTPUTS   1
#define SPLASH_NUM_COMPONENT_OUTPUTS   1

#define BRCM_DAC_SVIDEO_LUMA           BVDC_Dac_5
#define BRCM_DAC_SVIDEO_CHROMA         BVDC_Dac_6
#define BRCM_DAC_COMPOSITE_0           BVDC_Dac_3

#define BRCM_DAC_Y                     BVDC_Dac_0
#define BRCM_DAC_PR                    BVDC_Dac_2
#define BRCM_DAC_PB                    BVDC_Dac_1

#define SPLASH_SURFACE_PXL_TYPE BPXL_eR5_G6_B5 /* BPXL_eA8_R8_G8_B8 */

#endif /* BSPLASH_BOARD_H__ */

/* End of File */
