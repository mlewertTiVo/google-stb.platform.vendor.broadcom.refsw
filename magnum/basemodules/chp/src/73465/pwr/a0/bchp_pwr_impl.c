/***************************************************************************
*     Copyright (c) 2006-2014, Broadcom Corporation*
*     All Rights Reserved*
*     Confidential Property of Broadcom Corporation*
*
*  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
*  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
*  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
*
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* Module Description:
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/

/***************************************************************
*
* This file maps the power resource control to register writes.
* The templates are auto-generated by generate_chp_pwr.pl,
* but must be filled-in manually.
*
***************************************************************/

#include "bchp.h"
#include "bchp_priv.h"
#include "bdbg.h"
#include "bkni.h"

#include "bchp_clkgen.h"
#include "bchp_hdmi_tx_phy.h"
#include "bchp_aio_misc.h"
#include "bchp_uhfr.h"
#include "bchp_sun_top_ctrl.h"

BDBG_MODULE(BCHP_PWR_IMPL);

static void BCHP_PWR_P_HW_SVD0_CLK_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
    BDBG_MSG(("HW_SVD0_CLK: %s", activate?"on":"off"));

    if (activate) {
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_SCB_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, mask);
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_ALTERNATE_SCB_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, mask);
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_108_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, mask);
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_AVD_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, mask);
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_CPU_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, mask);
    }
    else {
        mask =  BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_CPU_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, 0);
        mask = BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_AVD_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, 0);
        mask =  BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_108_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, 0);
        mask =  BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_ALTERNATE_SCB_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, 0);
        mask =  BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE_SVD_SCB_CLOCK_ENABLE_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_CLOCK_ENABLE, mask, 0);
    }
}

static void BCHP_PWR_P_HW_SVD0_PWR_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BSTD_UNUSED(handle);
    BSTD_UNUSED(mask);

    BDBG_MSG(("HW_SVD0_PWR: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_SVD_TOP_INST_POWER_SWITCH_MEMORY_SVD_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SVD_TOP_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}

static void BCHP_PWR_P_HW_AIO_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_AIO: %s", activate?"on":"off"));

    reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_AIO);
    mask = ( BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_AIO_AIO_ALTERNATE_SCB_CLOCK_ENABLE_AIO_MASK |
             BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_AIO_AIO_ALTERNATE2_108_CLOCK_ENABLE_AIO_MASK);
    reg &= ~mask;
    reg |= activate?mask:0;
    BREG_Write32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_AIO, reg);
}

static void BCHP_PWR_P_HW_AIO_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_AIO_SRAM: %s", activate?"on":"off"));

        mask = BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_AIO_AIO_POWER_SWITCH_MEMORY_AIO_MASK;

        if(activate) {
            BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_AIO, mask, 2);
            BKNI_Delay(10);
            BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_AIO, mask, 0);
            BKNI_Delay(10);
        } else {
            BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_AIO, mask, mask);
        }

}

static void BCHP_PWR_P_HW_RAAGA0_CLK_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_RAAGA0_CLK: %s", activate?"on":"off"));

    mask = (BCHP_CLKGEN_RAAGA_DSP_TOP_INST_CLOCK_ENABLE_RAAGA_SCB_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_RAAGA_DSP_TOP_INST_CLOCK_ENABLE_RAAGA_108_CLOCK_ENABLE_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_RAAGA_DSP_TOP_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_RAAGA0_DSP_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_RAAGA0_DSP: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_RAAGA_DSP_TOP_INST_CLOCK_ENABLE_RAAGA_DSP_CLOCK_ENABLE_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_RAAGA_DSP_TOP_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_RAAGA0_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_RAAGA0_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_RAAGA_DSP_TOP_INST_POWER_SWITCH_MEMORY_RAAGA_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_RAAGA_DSP_TOP_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_RAAGA_DSP_TOP_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_RAAGA_DSP_TOP_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}

static void BCHP_PWR_P_HW_BVN_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_BVN: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_BVN_TOP_INST_CLOCK_ENABLE_BVN_SCB_CLOCK_ENABLE_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_BVN_TOP_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_BVN_108M_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_BVN_108M: %s", activate?"on":"off"));

     mask = (BCHP_CLKGEN_BVN_TOP_INST_CLOCK_ENABLE_BVN_216_CLOCK_ENABLE_MASK |
             BCHP_CLKGEN_BVN_TOP_INST_CLOCK_ENABLE_BVN_108_CLOCK_ENABLE_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_BVN_TOP_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_BVN_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_BVN_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_BVN_TOP_INST_POWER_SWITCH_MEMORY_BVN_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_BVN_TOP_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_BVN_TOP_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_BVN_TOP_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}

static void BCHP_PWR_P_HW_VEC_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_VEC_AIO: %s", activate?"on":"off"));

    mask = (BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_VEC_SCB_CLOCK_ENABLE_VEC_MASK |
            BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_VEC_ALTERNATE_216_CLOCK_ENABLE_VEC_MASK|
            BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_VEC_216_CLOCK_ENABLE_VEC_MASK |
            BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_AIO_ALTERNATE_108_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_VEC_108_CLOCK_ENABLE_VEC_MASK);
     BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_VEC_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_VEC_SRAM: %s", activate?"on":"off"));

    mask =  BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_VEC_VEC_POWER_SWITCH_MEMORY_VEC_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_VEC, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_VEC, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_POWER_SWITCH_MEMORY_VEC, mask, mask);
    }
}

static void BCHP_PWR_P_HW_VDC_DAC_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_VDC_DAC: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_DISABLE_DISABLE_VEC_DACADC_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_DISABLE, mask, activate?0:mask);

    mask = BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC_VEC_QDAC_216_CLOCK_ENABLE_VEC_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_ENABLE_VEC, mask, activate?mask:0);
}


static void BCHP_PWR_P_HW_VDC_656_OUT_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_VDC_656_OUT: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_DISABLE_DISABLE_VEC_ITU656_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_VEC_AIO_TOP_INST_CLOCK_DISABLE, mask, activate?0:mask);
}

static void BCHP_PWR_P_HW_XPT_108M_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_XPT_108M: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_CORE_XPT_INST_CLOCK_ENABLE_XPT_108_CLOCK_ENABLE_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_XPT_XMEMIF_Control(BCHP_Handle handle, bool activate)
{
   uint32_t mask;
    BDBG_MSG(("HW_XPT_XMEMIF: %s", activate?"on":"off"));

    mask = (BCHP_CLKGEN_CORE_XPT_INST_CLOCK_ENABLE_XPT_SCB_CLOCK_ENABLE_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_XPT_RMX_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
    BDBG_MSG(("HW_XPT_RMX: %s", activate?"on":"off"));

    mask = (BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE_DISABLE_XPT_20P25_CLOCK_MASK |
            BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE_DISABLE_XPT_27_CLOCK_MASK |
            BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE_DISABLE_XPT_40P5_CLOCK_MASK |
            BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE_DISABLE_XPT_54_CLOCK_MASK |
            BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE_DISABLE_XPT_81_CLOCK_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_CLOCK_DISABLE, mask, activate?0:mask);
}

static void BCHP_PWR_P_HW_XPT_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_XPT_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_CORE_XPT_INST_POWER_SWITCH_MEMORY_XPT_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_CORE_XPT_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}
static void BCHP_PWR_P_HW_XPT_WAKEUP_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
        uint32_t chipVer;

    BDBG_MSG(("HW_XPT_WAKEUP: %s", activate?"on":"off"));

        chipVer = BREG_Read32(handle->regHandle, BCHP_SUN_TOP_CTRL_PRODUCT_ID);
        if(chipVer == 0x73460012){
                mask = BCHP_CLKGEN_PM_CLOCK_108_ALIVE_SEL_CLOCK_108_CG_FTM_MASK;
                BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PM_CLOCK_108_ALIVE_SEL, mask, activate?mask:0);
    }

    mask = BCHP_CLKGEN_PM_CLOCK_216_ALIVE_SEL_CLOCK_216_CG_XPT_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PM_CLOCK_216_ALIVE_SEL, mask, activate?mask:0);

    mask = BCHP_CLKGEN_PM_PLL_ALIVE_SEL_PLL_SYS0_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PM_PLL_ALIVE_SEL, mask, activate?mask:0);

}

static void BCHP_PWR_P_HW_HDMI_TX_CLK_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_HDMI_TX_CLK: %s", activate?"on":"off"));

    reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_CLOCK_DISABLE);
    mask = ( BCHP_CLKGEN_DVP_HT_INST_CLOCK_DISABLE_DISABLE_DVPHT_IIC_MASTER_CLOCK_MASK );
    reg &= ~mask;
    reg |= activate?0:mask;
    BREG_Write32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_CLOCK_DISABLE, reg);

        mask = (BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_216_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_108_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_ALTERNATE_216_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_ALTERNATE_108_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_CLK_MAX_ENABLE_MASK |
            BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE_DVPHT_CLK_VEC_ENABLE_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_CLOCK_ENABLE, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_HDMI_TX_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_HDMI_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_DVP_HT_INST_POWER_SWITCH_MEMORY_DVPHT_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_DVP_HT_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}

static void BCHP_PWR_P_HW_HDMI_TX_PHY_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_HDMI_TX_PHY: %s", activate?"on":"off"));

    reg = BREG_Read32(handle->regHandle, BCHP_HDMI_TX_PHY_POWERDOWN_CTL);
    mask = ( BCHP_MASK(HDMI_TX_PHY_POWERDOWN_CTL, RNDGEN_PWRDN) |
             BCHP_MASK(HDMI_TX_PHY_POWERDOWN_CTL, PLL_PWRDN)  |
             BCHP_MASK(HDMI_TX_PHY_POWERDOWN_CTL, BIAS_PWRDN) |
             BCHP_MASK(HDMI_TX_PHY_POWERDOWN_CTL, PHY_PWRDN));
    if (activate) {
        reg &= ~mask;
    }
    else {
        reg |= mask;
    }
    BREG_Write32(handle->regHandle, BCHP_HDMI_TX_PHY_POWERDOWN_CTL, reg) ;
}

static void BCHP_PWR_P_HW_M2MC_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
    BDBG_MSG(("HW_M2MC: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_GRAPHICS_INST_M2MC_CLOCK_ENABLE_GFX_M2MC_CLOCK_ENABLE_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_M2MC_CLOCK_ENABLE, mask, activate ? mask : 0);

    mask = BCHP_CLKGEN_GRAPHICS_INST_CLOCK_DISABLE_DISABLE_GFX_M2MC_CORE_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_CLOCK_DISABLE, mask, activate ? 0: mask);
}

static void BCHP_PWR_P_HW_V3D_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
    BDBG_MSG(("HW_V3D: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_GRAPHICS_INST_V3D_CLOCK_ENABLE_GFX_V3D_CLOCK_ENABLE_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_V3D_CLOCK_ENABLE, mask, activate ? mask : 0);
}

static void BCHP_PWR_P_HW_M2MC_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_GFX_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_GRAPHICS_INST_POWER_SWITCH_MEMORY_GFX_POWER_SWITCH_MEMORY_MASK;
    if(activate) {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_POWER_SWITCH_MEMORY, mask, 2);
        BKNI_Delay(10);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_POWER_SWITCH_MEMORY, mask, 0);
        BKNI_Delay(10);
    } else {
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_POWER_SWITCH_MEMORY, mask, mask);
    }
}

static void BCHP_PWR_P_HW_GFX_108M_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;
    BDBG_MSG(("HW_GFX_108M: %s", activate?"on":"off"));

    mask = (BCHP_CLKGEN_GRAPHICS_INST_CLOCK_ENABLE_GFX_108_CLOCK_ENABLE_MASK |
            BCHP_CLKGEN_GRAPHICS_INST_CLOCK_ENABLE_GFX_SCB_CLOCK_ENABLE_MASK);
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_GRAPHICS_INST_CLOCK_ENABLE, mask, activate ? mask : 0);
}

static void BCHP_PWR_P_HW_DMA_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_DMA: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_SECTOP_INST_CLOCK_ENABLE_M2MDMA_M2MDMA_ALTERNATE_SCB_CLOCK_ENABLE_M2MDMA_MASK;

    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SECTOP_INST_CLOCK_ENABLE_M2MDMA, mask, activate?mask:0);
}

static void BCHP_PWR_P_HW_SCD0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_SCD0: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE_DISABLE_SC0_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE, mask, activate?0:mask);
}

static void BCHP_PWR_P_HW_SCD1_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_SCD1: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE_DISABLE_SC1_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE, mask, activate?0:mask);
}


static void BCHP_PWR_P_HW_MDM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_MDM: %s", activate?"on":"off"));

    reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PAD_CLOCK_DISABLE);
    mask = ( BCHP_CLKGEN_PAD_CLOCK_DISABLE_DISABLE_PAD_OUTPUT_CODECMCLK_CLOCK_MASK );
    reg &= ~mask;
    reg |= activate?0:mask;
    BREG_Write32(handle->regHandle, BCHP_CLKGEN_PAD_CLOCK_DISABLE, reg);

    mask = BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE_DISABLE_SYSCTRL_SOFTMODEM_CLOCK_MASK;
    BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_SYS_CTRL_INST_CLOCK_DISABLE, mask, activate?0:mask);
}

static void BCHP_PWR_P_HW_UHF_INPUT_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_UHF_INPUT: %s", activate?"on":"off"));

    if(activate) {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE);
        mask = ( BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE_DISABLE_UHFR_ALWAYSON_CLOCK_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE);
        mask = ( BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE_UHFR_108_CLOCK_ENABLE_MASK );
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_UHFR_AUTOPD1);
        mask = BCHP_UHFR_AUTOPD1_MANUAL_PD_MASK;
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_UHFR_AUTOPD1, reg);
    } else {
        reg = BREG_Read32(handle->regHandle, BCHP_UHFR_AUTOPD1);
        mask = BCHP_UHFR_AUTOPD1_MANUAL_PD_MASK;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_UHFR_AUTOPD1, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE);
        mask = ( BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE_UHFR_108_CLOCK_ENABLE_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_ENABLE, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE);
        mask = ( BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE_DISABLE_UHFR_ALWAYSON_CLOCK_MASK );
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_CLOCK_DISABLE, reg);
    }
}

static void BCHP_PWR_P_HW_UHF_INPUT_SRAM_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_UHF_INPUT_SRAM: %s", activate?"on":"off"));

    mask = BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY_UHFR_POWER_SWITCH_MEMORY_MASK;

    if(activate) {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY);
        reg &= ~mask;
        reg |= 2;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY, reg);
        BKNI_Delay(10);
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY);
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY, reg);
        BKNI_Delay(10);
    } else {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY);
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_UHFR_TOP_INST_POWER_SWITCH_MEMORY, reg);
    }
}

static void BCHP_PWR_P_HW_PLL_AVD_CH0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_AVD_CH0: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0, mask, 0);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0, mask, mask);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_0, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_AVD_CH1_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_AVD_CH1: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1_CLOCK_DIS_CH1_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1, mask, 0);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1_POST_DIVIDER_HOLD_CH1_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1_POST_DIVIDER_HOLD_CH1_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1, mask, mask);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1_CLOCK_DIS_CH1_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_1, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_AVD_CH2_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_AVD_CH2: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2_CLOCK_DIS_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2, mask, 0);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2_POST_DIVIDER_HOLD_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2_POST_DIVIDER_HOLD_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2, mask, mask);
        mask = BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2_CLOCK_DIS_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_CHANNEL_CTRL_CH_2, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_AVD_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_PLL_AVD: %s", activate?"on":"off"));

    if (activate) {
        uint32_t reg, cnt=50;

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PM_PLL_LDO_POWERUP);
        mask = ( BCHP_CLKGEN_PM_PLL_LDO_POWERUP_ISO_CLOCK_PLL_AVD_MASK );
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PM_PLL_LDO_POWERUP, reg);

        mask = (BCHP_CLKGEN_PLL_AVD_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AVD_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_RESET, mask, 0);

        mask = BCHP_CLKGEN_PLL_AVD_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_PWRDN , mask, 0);

        /* Check for PLL lock */
        while(cnt--) {
            BKNI_Delay(10);
            reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_LOCK_STATUS);
            if (BCHP_GET_FIELD_DATA(reg, CLKGEN_PLL_AVD_PLL_LOCK_STATUS, LOCK))
            break;
        }

        if(!cnt)
            BDBG_ERR(("AVD PLL Lock Timeout"));
    } else {
        mask = BCHP_CLKGEN_PLL_AVD_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_PWRDN , mask, mask);

        mask = (BCHP_CLKGEN_PLL_AVD_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AVD_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AVD_PLL_RESET, mask, mask);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PM_PLL_LDO_POWERUP);
        mask = ( BCHP_CLKGEN_PM_PLL_LDO_POWERUP_ISO_CLOCK_PLL_AVD_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PM_PLL_LDO_POWERUP, reg);
    }
}

static void BCHP_PWR_P_HW_AUD_PLL0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_AUD_PLL0: %s", activate?"on":"off"));

    if(activate) {
        mask = (BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET, mask, 0);

        mask = BCHP_CLKGEN_PLL_AUDIO0_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO0_PLL_PWRDN, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_AUDIO0_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO0_PLL_PWRDN, mask, mask);

        mask = (BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO0_PLL_RESET, mask, mask);
    }
}

static void BCHP_PWR_P_HW_AUD_PLL1_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_AUD_PLL1: %s", activate?"on":"off"));

    if(activate) {
        mask = (BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET, mask, 0);

        mask = BCHP_CLKGEN_PLL_AUDIO1_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO1_PLL_PWRDN, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_AUDIO1_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO1_PLL_PWRDN, mask, mask);

        mask = (BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_AUDIO1_PLL_RESET, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_SCD_CH0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_SCD_CH0: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, 0);
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, mask);
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, mask);
    }

}

static void BCHP_PWR_P_HW_PLL_SCD_CH1_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_SCD_CH1: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_1_CLOCK_DIS_CH1_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_1, mask, 0);
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, mask);
        mask = BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_CHANNEL_CTRL_CH_0, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_SCD_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_SCD_PLL: %s", activate?"on":"off"));

    if(activate) {
        mask = (BCHP_CLKGEN_PLL_SC_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_SC_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_RESET, mask, 0);

        mask = BCHP_CLKGEN_PLL_SC_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_PWRDN, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_SC_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_PWRDN, mask, mask);

        mask = (BCHP_CLKGEN_PLL_SC_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_SC_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_SC_PLL_RESET, mask, mask);
    }
}

static void BCHP_PWR_P_HW_PLL_VCXO_CH0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_VCXO_CH0: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0, mask, 0);
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0, mask, mask);
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_0, mask, mask);
    }

}

static void BCHP_PWR_P_HW_PLL_VCXO_CH2_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_VCXO_CH2: %s", activate?"on":"off"));

    if(activate) {
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2_CLOCK_DIS_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2, mask, 0);
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2_POST_DIVIDER_HOLD_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2_POST_DIVIDER_HOLD_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2, mask, mask);
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2_CLOCK_DIS_CH2_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_CHANNEL_CTRL_CH_2, mask, mask);
    }
}

#if 0
static void BCHP_PWR_P_HW_PLL_VCXO_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask;

    BDBG_MSG(("HW_PLL_VCXO: %s", activate?"on":"off"));

    if(activate) {
        mask = (BCHP_CLKGEN_PLL_VCXO_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_VCXO_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_RESET, mask, 0);

        mask = BCHP_CLKGEN_PLL_VCXO_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_PWRDN, mask, 0);
    } else {
        mask = BCHP_CLKGEN_PLL_VCXO_PLL_PWRDN_PWRDN_PLL_MASK;
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_PWRDN, mask, mask);

        mask = (BCHP_CLKGEN_PLL_VCXO_PLL_RESET_RESETD_MASK |
                BCHP_CLKGEN_PLL_VCXO_PLL_RESET_RESETA_MASK);
        BREG_AtomicUpdate32(handle->regHandle, BCHP_CLKGEN_PLL_VCXO_PLL_RESET, mask, mask);
    }
}
#endif

static void BCHP_PWR_P_HW_PLL_RAAGA_PLL_CH0_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_PLL_RAAGA_PLL_CH0: %s", activate?"on":"off"));

    if(activate) {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0, reg);
    } else {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0_POST_DIVIDER_HOLD_CH0_MASK );
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0_CLOCK_DIS_CH0_MASK );
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_CHANNEL_CTRL_CH_0, reg);
    }
}

static void BCHP_PWR_P_HW_PLL_RAAGA_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_PLL_RAAGA: %s", activate?"on":"off"));

    if(activate) {
        uint32_t reg, cnt=50;

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN_PWRDN_PLL_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_RESET);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_RESET_RESETD_MASK |
                 BCHP_CLKGEN_PLL_RAAGA_PLL_RESET_RESETA_MASK );
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_RESET, reg);

        /* Check for PLL lock */
        while(cnt--) {
            BKNI_Delay(10);
            reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_LOCK_STATUS);
            if (BCHP_GET_FIELD_DATA(reg, CLKGEN_PLL_RAAGA_PLL_LOCK_STATUS, LOCK))
            break;
        }

        if(!cnt)
            BDBG_ERR(("RAAGA PLL Lock Timeout"));
        }
        else {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_RESET);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_RESET_RESETD_MASK |
                 BCHP_CLKGEN_PLL_RAAGA_PLL_RESET_RESETA_MASK );
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_RESET, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN);
        mask = ( BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN_PWRDN_PLL_MASK );
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_RAAGA_PLL_PWRDN, reg);
    }
}

static void BCHP_PWR_P_HW_PLL_MOCA_CH4_Control(BCHP_Handle handle, bool activate)
{
    uint32_t mask, reg;

    BDBG_MSG(("HW_PLL_MOCA_CH4: %s", activate?"on":"off"));

    if(activate) {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4);
        mask = BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4_CLOCK_DIS_CH4_MASK;
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4);
        mask = BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4_POST_DIVIDER_HOLD_CH4_MASK;
        reg &= ~mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4, reg);
    } else {
        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4);
        mask = BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4_POST_DIVIDER_HOLD_CH4_MASK;
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4, reg);

        reg = BREG_Read32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4);
        mask = BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4_CLOCK_DIS_CH4_MASK;
        reg &= ~mask;
        reg |= mask;
        BREG_Write32(handle->regHandle, BCHP_CLKGEN_PLL_MOCA_PLL_CHANNEL_CTRL_CH_4, reg);
    }
}
