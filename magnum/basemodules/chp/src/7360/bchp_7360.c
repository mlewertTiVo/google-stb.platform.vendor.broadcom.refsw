/***************************************************************************
 *     Copyright (c) 2006-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
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
 *   See Module Overview below.
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/
#include "bstd.h"
#include "bdbg.h"
#include "bkni.h"
#include "breg_mem.h"
#include "bchp.h"
#include "bchp_priv.h"
#include "bchp_7360.h"
#include "bchp_sun_top_ctrl.h"
#include "bchp_decode_sd_0.h"
#include "bchp_decode_ip_shim_0.h"
#include "bchp_sun_gisb_arb.h"
#include "bchp_memc_ddr23_shim_addr_cntl_0.h"
#include "bchp_memc_ddr_0.h"
#include "bchp_clkgen.h"
#include "bchp_pwr.h"

#include "bchp_avs_ro_registers_0.h"
#include "bchp_avs.h"
#include "bchp_raaga_dsp_misc.h"
#include "bchp_pwr_resources_priv.h"


BDBG_MODULE(BCHP);

/* Miscellaneous macros. */
#define BCHP_P_MAJOR_REV_SHIFT          (4)

/* Chip info and features */
typedef struct BCHP_P_7360_Info
{
    uint32_t      ulChipIdReg; /* index into the table. */

    /* Chip Id */
    uint16_t      usChipId;

    /* Major revision */
    uint16_t      usMajor;

    /* Minor revision */
    uint16_t      usMinor;

    /* TODO: Other features or infos if needed */
} BCHP_P_7360_Info;


/* Lookup table for chip features and etc.
 * The are many times when the chip device id register
 * not conforming to the standard numbering convention. We do
 * it this way to work-around those problems.
 *
 * TODO: Update this table to support new revisions.
 */
static const BCHP_P_7360_Info s_aChipInfoTable[] =
{
#if BCHP_VER == BCHP_VER_A0 || BCHP_VER == BCHP_VER_A1 || BCHP_VER == BCHP_VER_A2
    /* A0,A1,A2 code will run on A0,A1,A2 */
   {0x73600000, BCHP_BCM7360, BCHP_MAJOR_A, BCHP_MINOR_0},
   {0x73600001, BCHP_BCM7360, BCHP_MAJOR_A, BCHP_MINOR_1},
   {0x73600002, BCHP_BCM7360, BCHP_MAJOR_A, BCHP_MINOR_2},
#else
    #error "Port required"
#endif
};

/* Chip context */
typedef struct BCHP_P_7360_Context
{
    uint32_t                           ulBlackMagic;
    BREG_Handle                        hRegister;
    const BCHP_P_7360_Info            *pChipInfo;
    BCHP_P_AvsHandle                  hAvsHandle;
} BCHP_P_7360_Context;

/* Max entry of lookup table */
#define BCHP_P_CHIP_INFO_MAX_ENTRY \
    (sizeof(s_aChipInfoTable) / sizeof(BCHP_P_7360_Info))

/* This macro checks for a validity of a handle, and
 * cast to context pointer. */
#define BCHP_P_GET_CONTEXT(handle, context) \
{ \
    if(!(handle) || \
       !((handle)->chipHandle) || \
       (((BCHP_P_7360_Context*)((handle)->chipHandle))->ulBlackMagic != \
       sizeof(BCHP_P_7360_Context))) \
    { \
        BDBG_ERR(("Corrupted context handle\n")); \
        (context) = NULL; \
    } \
    else \
    { \
        (context) = (BCHP_P_7360_Context*)((handle)->chipHandle); \
    } \
    BDBG_ASSERT(context); \
}

/* Static function prototypes */
static BERR_Code BCHP_P_Close7360
    ( BCHP_Handle                      hChip );

static BERR_Code BCHP_P_GetChipInfoComformWithBaseClass
    ( const BCHP_Handle                hChip,
      uint16_t                        *pusChipId,
      uint16_t                        *pusChipRev );

static BERR_Code BCHP_P_GetChipInfo
    ( const BCHP_Handle                hChip,
      uint16_t                        *pusChipId,
      uint16_t                        *pusChipMajorRev,
      uint16_t                        *pusChipMinorRev );

static BERR_Code BCHP_P_GetFeature
    ( const BCHP_Handle                hChip,
      const BCHP_Feature               eFeature,
      void                            *pFeatureValue );

static void BCHP_P_MonitorPvt
    ( BCHP_Handle                      hChip,
      BCHP_AvsSettings                *pSettings );

static BERR_Code BCHP_P_GetAvsData
    ( BCHP_Handle                      hChip,
      BCHP_AvsData                    *pData );

static BERR_Code BCHP_P_StandbyMode
    ( BCHP_Handle                      hChip,
      bool                             activate );

static BERR_Code BCHP_P_ResetMagnumCores
    ( const BCHP_Handle                hChip );
/***************************************************************************
 * Open BCM7360 Chip.
 *
 */
BERR_Code BCHP_Open7360
    ( BCHP_Handle                     *phChip,
      BREG_Handle                      hRegister )
{
    BCHP_P_Context *pChip;
    BCHP_P_7360_Context *p7360Chip;
    uint32_t ulChipIdReg;
    uint32_t ulIdx;
    uint32_t ulVal;
    BERR_Code rc;

    BDBG_ENTER(BCHP_Open7360);

    if((!phChip) ||
       (!hRegister))
    {
        BDBG_ERR(("Invalid parameter\n"));
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    /* If error ocurr user get a NULL *phChip */
    *phChip = NULL;

    /* Alloc the base chip context. */
    pChip = (BCHP_P_Context*)(BKNI_Malloc(sizeof(BCHP_P_Context)));
    if(!pChip)
    {
        return BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
    }

    /* Clear out the context and set defaults. */
    BKNI_Memset((void*)pChip, 0x0, sizeof(BCHP_P_Context));

    p7360Chip = (BCHP_P_7360_Context*)
        (BKNI_Malloc(sizeof(BCHP_P_7360_Context)));
    if(!p7360Chip)
    {
        BKNI_Free(pChip);
        return BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
    }

    /* Clear out the context and set defaults. */
    BKNI_Memset((void*)p7360Chip, 0x0, sizeof(BCHP_P_7360_Context));

    /* Fill up the base chip context. */
    pChip->chipHandle       = (void*)p7360Chip;
    pChip->regHandle        = hRegister;
    pChip->pCloseFunc       = BCHP_P_Close7360;
    pChip->pGetChipInfoFunc = BCHP_P_GetChipInfoComformWithBaseClass;
    pChip->pGetFeatureFunc  = BCHP_P_GetFeature;
    pChip->pMonitorPvtFunc  = BCHP_P_MonitorPvt;
    pChip->pGetAvsDataFunc  = BCHP_P_GetAvsData;
    pChip->pStandbyModeFunc = BCHP_P_StandbyMode;

    /* Fill up the chip context. */
    p7360Chip->ulBlackMagic = sizeof(BCHP_P_7360_Context);
    p7360Chip->hRegister    = hRegister;

    /* Open BCHP_PWR - but first, do a reset on some of the Magnum controlled cores so
    * that they don't interfere with BCHP_PWR_Open's powering up/down.
    */
    BCHP_P_ResetMagnumCores( pChip );
    /* Note: PWR_Open MUST go here (before AvsOpen) */

    /* Open BCHP_PWR */
    rc = BCHP_PWR_Open(&pChip->pwrManager, pChip);
    if (rc) {
        BKNI_Free(pChip);
        BKNI_Free(p7360Chip);
        return BERR_TRACE(rc);
    }

    /* Open AVS module */
    BCHP_P_AvsOpen(&p7360Chip->hAvsHandle, pChip);
    if(!p7360Chip->hAvsHandle)
    {
        /*BCHP_PWR_Close(pChip->pwrManager); <--- Add this when adding PWR_Open */
        BKNI_Free(pChip);
        BKNI_Free(p7360Chip);
        return BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
    }

    /* Chip Family Register id is use for indexing into the table. */
    ulChipIdReg = BREG_Read32(hRegister, BCHP_SUN_TOP_CTRL_CHIP_FAMILY_ID);

/* decompose 32 bit chip id for use with printf format string %x%c%d
Example: 0x73600000 becomes "7360A0" */
#define PRINT_CHIP(CHIPID) \
    ((CHIPID)>>16), ((((CHIPID)&0xF0)>>4)+'A'), ((CHIPID)&0x0F)

    for(ulIdx = 0; ulIdx < BCHP_P_CHIP_INFO_MAX_ENTRY; ulIdx++)
    {
        BDBG_MSG(("Supported Chip Family and revision: %x%c%d", PRINT_CHIP(s_aChipInfoTable[ulIdx].ulChipIdReg)));
        BDBG_MSG(("Supported Chip ID: %x", s_aChipInfoTable[ulIdx].usChipId));
        BDBG_MSG(("\n"));
    }

    /* Lookup corresponding chip id. */
    for(ulIdx = 0; ulIdx < BCHP_P_CHIP_INFO_MAX_ENTRY; ulIdx++)
    {
        const BCHP_P_7360_Info *compareChipInfo = &s_aChipInfoTable[ulIdx];

        if(compareChipInfo->ulChipIdReg == ulChipIdReg)
        {
            /* Chip Information. */
            p7360Chip->pChipInfo = compareChipInfo;
            break;
        }
        else if (ulIdx == BCHP_P_CHIP_INFO_MAX_ENTRY - 1 && compareChipInfo->usMajor == (ulChipIdReg&0xF0)>>4)
        {
            /* This is a future minor revision. We will allow it with a WRN. */
            BDBG_WRN(("An unknown minor revision %x%c%d has been detected. Certain operations may result in erratic behavior. Please confirm this chip revision is supported with this software.",
                PRINT_CHIP(ulChipIdReg)));
            p7360Chip->pChipInfo = compareChipInfo;
            break;
        }
    }

    if(!p7360Chip->pChipInfo)
    {
        BKNI_Free(p7360Chip);
        BKNI_Free(pChip);
        BDBG_ERR(("*****************************************************************\n"));
        BDBG_ERR(("ERROR ERROR ERROR ERROR \n"));
        BDBG_ERR(("Unsupported Revision: %x%c%d", PRINT_CHIP(ulChipIdReg)));
        BDBG_ERR(("*****************************************************************\n"));
    phChip = NULL;
    BDBG_ASSERT(phChip);
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }
    BDBG_MSG(("found %x%c%d", PRINT_CHIP(p7360Chip->pChipInfo->ulChipIdReg)));

    /* All done. now return the new fresh context to user. */
    *phChip = (BCHP_Handle)pChip;

#if BCHP_PWR_RESOURCE_MAGNUM_CONTROLLED
    BCHP_PWR_AcquireResource(pChip, BCHP_PWR_RESOURCE_MAGNUM_CONTROLLED);
#endif

    /* Clear AVD/SVD shutdown enable bit */
    BREG_Write32(hRegister, BCHP_DECODE_IP_SHIM_0_SOFTSHUTDOWN_CTRL_REG, 0x0);

    /* TODO: Bring up the clocks */
    BDBG_MSG(("Hack Hack,programming BCHP_SUN_GISB_ARB_REQ_MASK, this should be done in CFE"));
    /* This mask controls which clients can be GISB master. */

    ulVal = BREG_Read32(hRegister, BCHP_SUN_GISB_ARB_REQ_MASK);
    ulVal &= ~( BCHP_MASK(SUN_GISB_ARB_REQ_MASK, avd_0) |
            BCHP_MASK(SUN_GISB_ARB_REQ_MASK, raaga) |
            BCHP_MASK(SUN_GISB_ARB_REQ_MASK, rdc) );
    BREG_Write32(hRegister, BCHP_SUN_GISB_ARB_REQ_MASK, ulVal);

    /* increase tuner LDO voltage */

    ulVal = BREG_Read32(hRegister, BCHP_SUN_TOP_CTRL_GENERAL_CTRL_NO_SCAN_1);
    ulVal |=  (BCHP_FIELD_DATA(SUN_TOP_CTRL_GENERAL_CTRL_NO_SCAN_1, ldo_vregcntl, 0x70));
    ulVal |=  (BCHP_FIELD_DATA(SUN_TOP_CTRL_GENERAL_CTRL_NO_SCAN_1, ldo_pwrdn, 0));
    ulVal |=  (BCHP_FIELD_DATA(SUN_TOP_CTRL_GENERAL_CTRL_NO_SCAN_1, ldo_vregcntl_en, 1));
    BREG_Write32(hRegister, BCHP_SUN_TOP_CTRL_GENERAL_CTRL_NO_SCAN_1, ulVal);

    if ( ulChipIdReg == 0x73600000 ||
        ulChipIdReg == 0x73600001 )
    {
        /* Set M2MC clk to 324M */
        ulVal = BREG_Read32 (hRegister, BCHP_CLKGEN_INTERNAL_MUX_SELECT);
        ulVal |=  (BCHP_FIELD_DATA(CLKGEN_INTERNAL_MUX_SELECT, GFX_M2MC_CORE_CLOCK, 0x1));
        BREG_Write32(hRegister, BCHP_CLKGEN_INTERNAL_MUX_SELECT, ulVal);
    }


#if BCHP_PWR_RESOURCE_MAGNUM_CONTROLLED
    BCHP_PWR_ReleaseResource(pChip, BCHP_PWR_RESOURCE_MAGNUM_CONTROLLED);
#endif

    BDBG_LEAVE(BCHP_Open7360);
    return BERR_SUCCESS;
}


/***************************************************************************
 * {private}
 *
 */
static BERR_Code BCHP_P_Close7360
    ( BCHP_Handle                      hChip )
{
    BCHP_P_7360_Context *p7360Chip;

    BDBG_ENTER(BCHP_P_Close7360);

    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    if(!p7360Chip)
    {
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    if (p7360Chip->hAvsHandle) {
        BCHP_P_AvsClose(p7360Chip->hAvsHandle);
        p7360Chip->hAvsHandle = NULL;
    }

    /* Note: PWR_Close goes here (after AvsClose) */
    BCHP_PWR_Close(hChip->pwrManager);

    /* Invalidate the magic number. */
    p7360Chip->ulBlackMagic = 0;

    BKNI_Free((void*)p7360Chip);
    BKNI_Free((void*)hChip);

    BDBG_LEAVE(BCHP_P_Close7360);
    return BERR_SUCCESS;
}


/***************************************************************************
 * {private}
 *
 */
static BERR_Code BCHP_P_GetChipInfoComformWithBaseClass
    ( const BCHP_Handle                hChip,
      uint16_t                        *pusChipId,
      uint16_t                        *pusChipRev )

{
    BERR_Code eStatus;
    uint16_t usMajor=0;
    uint16_t usMinor=0;

    eStatus = BERR_TRACE(BCHP_P_GetChipInfo(hChip, pusChipId,
        &usMajor, &usMinor));
    if(BERR_SUCCESS != eStatus)
    {
        return eStatus;
    }

    if(pusChipRev)
    {
        *pusChipRev = ((usMajor << BCHP_P_MAJOR_REV_SHIFT) + usMinor);
    }

    return BERR_SUCCESS;
}


/***************************************************************************
 * {private}
 *
 */
static BERR_Code BCHP_P_GetChipInfo
    ( const BCHP_Handle                hChip,
      uint16_t                        *pusChipId,
      uint16_t                        *pusChipMajorRev,
      uint16_t                        *pusChipMinorRev )
{
    const BCHP_P_7360_Context *p7360Chip;

    BDBG_ENTER(BCHP_P_GetChipInfo);

    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    if(!p7360Chip)
    {
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    if(pusChipId)
    {
        *pusChipId = p7360Chip->pChipInfo->usChipId;
    }

    if(pusChipMajorRev)
    {
        *pusChipMajorRev = p7360Chip->pChipInfo->usMajor;
    }

    if(pusChipMinorRev)
    {
        *pusChipMinorRev = p7360Chip->pChipInfo->usMinor;
    }

    BDBG_LEAVE(BCHP_P_GetChipInfo);
    return BERR_SUCCESS;
}

/***************************************************************************
 * {private}
 *
 */
static BERR_Code BCHP_P_GetFeature
    ( const BCHP_Handle                hChip,
      const BCHP_Feature               eFeature,
      void                            *pFeatureValue )
{
    BERR_Code            rc = BERR_UNKNOWN;
    BCHP_P_7360_Context *p7360Chip;
    uint32_t             ulBondStatus;
    uint32_t             uiReg;
    uint32_t             avd_freq;

    BDBG_ENTER(BCHP_P_GetFeature);

    /* get base context */
    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    /* read bond-out status common for many features */
    ulBondStatus = BREG_Read32(p7360Chip->hRegister,
        BCHP_SUN_TOP_CTRL_OTP_OPTION_STATUS_0);

    /* which feature? */
    switch (eFeature)
    {
    case BCHP_Feature_e3DGraphicsCapable:
        /* 3D capable? (bool) */
        *(bool *)pFeatureValue = false;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_eDvoPortCapable:
        /* dvo port capable? (bool) */
        *(bool *)pFeatureValue = false;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_eMacrovisionCapable:
        /* macrovision capable? (bool) */
        *(bool *)pFeatureValue = BCHP_GET_FIELD_DATA(ulBondStatus,
            SUN_TOP_CTRL_OTP_OPTION_STATUS_0, otp_option_macrovision_disable) ? false : true;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_eMpegDecoderCount:
        /* number of MPEG decoders (int) */
        *(int *)pFeatureValue = 1;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_eHdcpCapable:
        /* HDCP capable? (bool) */
        *(bool *)pFeatureValue = BCHP_GET_FIELD_DATA(ulBondStatus,
            SUN_TOP_CTRL_OTP_OPTION_STATUS_0, otp_option_hdcp_disable ) ? false : true;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_e3desCapable:
        /* 3DES capable? (bool) */
        *(bool *)pFeatureValue = true;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_e1080pCapable:
        /* 1080p Capable? (bool) */
        *(bool *)pFeatureValue = true;
        rc = BERR_SUCCESS;
        break;

    case BCHP_Feature_eAVDCoreFreq:

       uiReg = BREG_Read32(p7360Chip->hRegister, BCHP_CLKGEN_PLL_AVD_MIPS_PLL_CHANNEL_CTRL_CH_2);
       avd_freq = BCHP_GET_FIELD_DATA(uiReg, CLKGEN_PLL_AVD_MIPS_PLL_CHANNEL_CTRL_CH_2, MDIV_CH2);
       if(avd_freq == 0)
           *(int *)pFeatureValue = 0;
       else
           *(int *)pFeatureValue = 3006/avd_freq;

       rc = BERR_SUCCESS;
       break;

    default:
        rc = BCHP_P_GetDefaultFeature(hChip, eFeature, pFeatureValue);
        break;
    }

    /* return result */
    BDBG_LEAVE(BCHP_P_GetFeature);
    return rc;
}

/* This gets called regularly to handle the AVS processing */
static void BCHP_P_MonitorPvt( BCHP_Handle hChip, BCHP_AvsSettings *pSettings )
{
    BCHP_P_7360_Context *p7360Chip;

    BDBG_ENTER(BCHP_P_MonitorPvt);
    BSTD_UNUSED(pSettings);

    /* get base context */
    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    if (p7360Chip->hAvsHandle)
        BCHP_P_AvsMonitorPvt(p7360Chip->hAvsHandle);

    BDBG_LEAVE(BCHP_P_MonitorPvt);
}

static BERR_Code BCHP_P_GetAvsData( BCHP_Handle hChip, BCHP_AvsData *pData )
{
    BCHP_P_7360_Context *p7360Chip;

    BDBG_ASSERT(pData);

    BDBG_ENTER(BCHP_GetAVdata);

    /* get base context */
    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    if (p7360Chip->hAvsHandle)
        BCHP_P_AvsGetData(p7360Chip->hAvsHandle, pData);

    BDBG_LEAVE(BCHP_GetAVdata);
    return BERR_SUCCESS;
}

static BERR_Code BCHP_P_StandbyMode( BCHP_Handle hChip, bool activate )
{
    BCHP_P_7360_Context *p7360Chip;

    BDBG_ENTER(BCHP_P_StandbyMode);

    /* get base context */
    BCHP_P_GET_CONTEXT(hChip, p7360Chip);

    /* Do anything required for CHP Standby changes */

    if (p7360Chip->hAvsHandle)
        BCHP_P_AvsStandbyMode(p7360Chip->hAvsHandle, activate);

    BDBG_LEAVE(BCHP_P_StandbyMode);
    return BERR_SUCCESS;
}
static void BCHP_P_ResetRaagaCore(const BCHP_Handle hChip, const BREG_Handle hReg)
{
    uint32_t val = 0;
    BCHP_PWR_P_HW_ControlId(hChip, BCHP_PWR_HW_PLL_AVD_CH3, true);
    BCHP_PWR_P_HW_ControlId(hChip, BCHP_PWR_HW_RAAGA0_CLK, true);
    BCHP_PWR_P_HW_ControlId(hChip, BCHP_PWR_HW_RAAGA0_DSP, true);
    val = BREG_Read32(hReg,BCHP_RAAGA_DSP_MISC_SOFT_INIT) ;
    val = (val & ~(BCHP_MASK(RAAGA_DSP_MISC_SOFT_INIT, DO_SW_INIT)))|
    (BCHP_FIELD_DATA(RAAGA_DSP_MISC_SOFT_INIT, DO_SW_INIT,1));
    BREG_Write32(hReg,BCHP_RAAGA_DSP_MISC_SOFT_INIT, val);
    val = BREG_Read32(hReg,BCHP_RAAGA_DSP_MISC_REVISION) ;
    val = BREG_Read32(hReg,BCHP_RAAGA_DSP_MISC_REVISION) ;
    BDBG_MSG(("REV ID VAL = 0x%x", val));
    val = BREG_Read32(hReg,BCHP_RAAGA_DSP_MISC_SOFT_INIT) ;
    val &=  ~(BCHP_MASK(RAAGA_DSP_MISC_SOFT_INIT,INIT_PROC_B));
    BREG_Write32(hReg,BCHP_RAAGA_DSP_MISC_SOFT_INIT, val);
    val = 0;
    val = (BCHP_FIELD_DATA(SUN_TOP_CTRL_SW_INIT_0_SET, raaga_sw_init,1));
    BREG_Write32(hReg,BCHP_SUN_TOP_CTRL_SW_INIT_0_SET, val);
    BKNI_Delay(2);
    val = 0;
    val = (BCHP_FIELD_DATA(SUN_TOP_CTRL_SW_INIT_0_CLEAR, raaga_sw_init,1));
    BREG_Write32(hReg,BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR, val);
    return;
}

static BERR_Code BCHP_P_ResetMagnumCores
    ( const BCHP_Handle                hChip )

{
    /* Reset some cores. This is needed to avoid L1 interrupts before BXXX_Open can be called per core. */
    /* Note, SW_INIT set/clear registers don't need read-modify-write. */
    BREG_Write32(hChip->regHandle, BCHP_SUN_TOP_CTRL_SW_INIT_0_SET,
           BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, xpt_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, avd0_sw_init, 1 )    /* avd0_sw_init */
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, vec_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, aio_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, bvn_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, raaga_sw_init, 1)
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_SET, dvp_ht_sw_init, 1));

    /* Now clear the reset. */
    BREG_Write32(hChip->regHandle, BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR,
           BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, xpt_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, avd0_sw_init, 1 )    /* avd0_sw_init */
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, vec_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, aio_sw_init, 1 )
        | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, bvn_sw_init, 1 )
         | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, raaga_sw_init, 1)
         | BCHP_FIELD_DATA( SUN_TOP_CTRL_SW_INIT_0_CLEAR, dvp_ht_sw_init, 1));

    BCHP_P_ResetRaagaCore(hChip, hChip->regHandle);
    return BERR_SUCCESS;
}
/* End of File */
