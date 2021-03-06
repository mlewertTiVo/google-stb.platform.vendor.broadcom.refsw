/***************************************************************************
 *     Copyright (c) 2003-2013, Broadcom Corporation
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
 * [File Description:]
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/
#include "bstd.h"
#include "bast.h"
#include "bast_priv.h"
#include "bast_g3.h"
#include "bast_g3_priv.h"
#include "bast_73465_priv.h"
#include "bchp_clkgen.h"


BDBG_MODULE(bast_73465_priv);


/* local functions */
void BAST_g3_P_GetRegisterAddress(BAST_ChannelHandle h, uint32_t reg, uint32_t *pAddr);

const uint32_t BAST_g3_ChannelIntrID[BAST_G3_MAX_CHANNELS][BAST_g3_MaxIntID] = {
   /* channel 0 interrupts */
   {
      BCHP_INT_ID_SDS_LOCK_IS_0,
      BCHP_INT_ID_SDS_NOT_LOCK_IS_0,
      BCHP_INT_ID_SDS_BTM_IS_0,
      BCHP_INT_ID_SDS_BRTM_IS_0,
      BCHP_INT_ID_SDS_GENTM_IS1_0,
      BCHP_INT_ID_SDS_GENTM_IS2_0,
      BCHP_INT_ID_SDS_GENTM_IS3_0,
      BCHP_INT_ID_DISEQC_TIMER1_0,
      BCHP_INT_ID_DISEQC_TIMER2_0,
      BCHP_INT_ID_SDS_sar_vol_gt_hi_thrsh_0,
      BCHP_INT_ID_SDS_sar_vol_lt_lo_thrsh_0,
      BCHP_INT_ID_DSDN_IS_0,
      BCHP_INT_ID_DISEQC_tx_fifo_a_empty_0,
      BCHP_INT_ID_SDS_HP_IS_0,
      BCHP_INT_ID_MI2C_IS_0,
      BCHP_INT_ID_TURBO_LOCK_IS_0,
      BCHP_INT_ID_TURBO_NOT_LOCK_IS_0,
      BCHP_INT_ID_TURBO_SYNC_0,
      BCHP_INT_ID_AFEC_LOCK_IS_0,
      BCHP_INT_ID_AFEC_NOT_LOCK_IS_0,
      BCHP_INT_ID_HP_FRAME_BOUNDARY_0,
      0
   },
   /* channel 1 interrupts */
   {
      BCHP_INT_ID_SDS_LOCK_IS_1,
      BCHP_INT_ID_SDS_NOT_LOCK_IS_1,
      BCHP_INT_ID_SDS_BTM_IS_1,
      BCHP_INT_ID_SDS_BRTM_IS_1,
      BCHP_INT_ID_SDS_GENTM_IS1_1,
      BCHP_INT_ID_SDS_GENTM_IS2_1,
      BCHP_INT_ID_SDS_GENTM_IS3_1,
      BCHP_INT_ID_DISEQC_TIMER1_1,
      BCHP_INT_ID_DISEQC_TIMER2_1,
      BCHP_INT_ID_SDS_sar_vol_gt_hi_thrsh_1,
      BCHP_INT_ID_SDS_sar_vol_lt_lo_thrsh_1,
      BCHP_INT_ID_DSDN_IS_1,
      BCHP_INT_ID_DISEQC_tx_fifo_a_empty_1,
      BCHP_INT_ID_SDS_HP_IS_1,
      BCHP_INT_ID_MI2C_IS_1,
      BCHP_INT_ID_TURBO_LOCK_IS_1,
      BCHP_INT_ID_TURBO_NOT_LOCK_IS_1,
      BCHP_INT_ID_TURBO_SYNC_1,
      BCHP_INT_ID_AFEC_LOCK_IS_1,
      BCHP_INT_ID_AFEC_NOT_LOCK_IS_1,
      BCHP_INT_ID_HP_FRAME_BOUNDARY_1,
      0
   }
};


/******************************************************************************
 BAST_g3_P_InitHandle()
******************************************************************************/
BERR_Code BAST_g3_P_InitHandle(BAST_Handle h)
{
   BAST_g3_P_Handle *hDev = (BAST_g3_P_Handle *)(h->pImpl);

   hDev->xtalFreq = BAST_G3_XTAL_FREQ;
   return BERR_SUCCESS;
}


/******************************************************************************
 BAST_g3_P_InitConfig()
******************************************************************************/
BERR_Code BAST_g3_P_InitConfig(BAST_ChannelHandle h)
{
   BAST_g3_P_ChannelHandle *hChn = (BAST_g3_P_ChannelHandle *)h->pImpl;
   BERR_Code retCode = BERR_SUCCESS;

   if (h->channel == 0)
   {
      hChn->bHasDiseqc = true;
   #ifndef BAST_EXCLUDE_EXT_TUNER
      hChn->bExternalTuner = false;
   #endif
      hChn->bHasTunerRefPll = true;
      hChn->bHasAfec = true;
      hChn->bHasTfec = true;
      hChn->tunerRefPllChannel = 0;

      hChn->xportSettings.bSerial = false;
      hChn->xportSettings.bClkInv = true;
      hChn->xportSettings.bClkSup = true;
      hChn->xportSettings.bVldInv = false;
      hChn->xportSettings.bSyncInv = false;
      hChn->xportSettings.bErrInv = false;
      hChn->xportSettings.bXbert = false;
      hChn->xportSettings.bTei = true;
      hChn->xportSettings.bDelay = false;
      hChn->xportSettings.bSync1 = false;
      hChn->xportSettings.bHead4 = false;
      hChn->xportSettings.bDelHeader = false;
      hChn->xportSettings.bOpllBypass = true;
      hChn->xportSettings.bchMpegErrorMode = BAST_BchMpegErrorMode_eBchAndCrc8;
   }
   else if (h->channel == 1)
   {
      hChn->bHasDiseqc = true;
   #ifndef BAST_EXCLUDE_EXT_TUNER
      hChn->bExternalTuner = false;
   #endif
      hChn->bHasTunerRefPll = false;
      hChn->bHasAfec = true;
      hChn->bHasTfec = true;
      hChn->tunerRefPllChannel = 0;

      hChn->xportSettings.bSerial = false;
      hChn->xportSettings.bClkInv = true;
      hChn->xportSettings.bClkSup = true;
      hChn->xportSettings.bVldInv = false;
      hChn->xportSettings.bSyncInv = false;
      hChn->xportSettings.bErrInv = false;
      hChn->xportSettings.bXbert = false;
      hChn->xportSettings.bTei = true;
      hChn->xportSettings.bDelay = false;
      hChn->xportSettings.bSync1 = false;
      hChn->xportSettings.bHead4 = false;
      hChn->xportSettings.bDelHeader = false;
      hChn->xportSettings.bOpllBypass = true;
      hChn->xportSettings.bchMpegErrorMode = BAST_BchMpegErrorMode_eBchAndCrc8;
   }
   else
   {
      BDBG_ERR(("BAST_g3_P_InitConfig(): invalid channnel number"));
      BERR_TRACE(retCode = BERR_INVALID_PARAMETER);
   }
   return retCode;
}


/******************************************************************************
 BAST_g3_P_GetRegisterAddress()
******************************************************************************/
void BAST_g3_P_GetRegisterAddress_isrsafe(BAST_ChannelHandle h, uint32_t reg, uint32_t *pAddr)
{
   *pAddr = reg;

   if (h->channel > 0)
   {
      if ((reg >= 0x700000) && (reg <= 0x700CFF))
      {
         /* SDS register access */
         *pAddr &= 0x00000FFF;
         *pAddr |= 0x00701000;
      }
      else if ((reg >= 0x702000) && (reg <= 0x7023FF))
      {
         /* TFEC register access */
         *pAddr &= 0x00000FFF;
         *pAddr |= 0x00703000;
      }
      else if ((reg >= 0x704000) && (reg <= 0x704FFF))
      {
         /* AFEC register access */
         *pAddr &= 0x00000FFF;
         *pAddr |= 0x00705000;
      }
   }
}



/******************************************************************************
 BAST_g3_P_ReadRegister_isrsafe()
******************************************************************************/
BERR_Code BAST_g3_P_ReadRegister_isrsafe(
   BAST_ChannelHandle h,     /* [in] BAST channel handle */
   uint32_t           reg,   /* [in] address of register to read */
   uint32_t           *val   /* [out] contains data that was read */
)
{
   BAST_g3_P_Handle *hDev = h->pDevice->pImpl;
   uint32_t addr;

   BAST_g3_P_GetRegisterAddress_isrsafe(h, reg, &addr);
   *val = BREG_Read32(hDev->hRegister, addr);
   return BERR_SUCCESS;
}


/******************************************************************************
 BAST_g3_P_WriteRegister_isrsafe()
******************************************************************************/
BERR_Code BAST_g3_P_WriteRegister_isrsafe(
   BAST_ChannelHandle h,     /* [in] BAST channel handle */
   uint32_t           reg,   /* [in] address of register to write */
   uint32_t           *val   /* [in] contains data to be written */
)
{
   BERR_Code retCode = BERR_TIMEOUT;
   BAST_g3_P_Handle *hDev = h->pDevice->pImpl;
   uint32_t addr, retry, max_retry = 10, val2, wait_time;

   /* determine if wait time is necessary depending on baud rate */
   BAST_g3_P_GetRegisterWriteWaitTime_isrsafe(h, reg, &wait_time);
   BAST_g3_P_GetRegisterAddress_isrsafe(h, reg, &addr);

   for (retry = 0; retry < max_retry; retry++)
   {
      BREG_Write32(hDev->hRegister, addr, *val);
      if (wait_time > 0)
      {
         BKNI_Delay(wait_time);

         if (reg == BCHP_SDS_EQ_LUPD)
         {
            /* skip readback for this indirect register */
            retCode = BERR_SUCCESS;
            break;
         }

         val2 =  BREG_Read32(hDev->hRegister, addr);
         if (val2 == *val)
         {
            /* readback successful */
            retCode = BERR_SUCCESS;
            break;
         }
      }
      else
      {
         /* case if no delay required */
         retCode = BERR_SUCCESS;
         break;
      }
   }

   if (retry > 0)
   {
      BDBG_MSG(("BAST_g3_P_WriteRegister_isrsafe(0x%X): %d retries", reg, retry));
   }

   return retCode;
}


#ifndef BAST_EXCLUDE_LDPC
/******************************************************************************
 BAST_g3_P_GetAfecClock_isrsafe() - updates fecFreq
******************************************************************************/
BERR_Code BAST_g3_P_GetAfecClock_isrsafe(BAST_ChannelHandle h, uint32_t *pFreq)
{
   BAST_g3_P_Handle *hDev = h->pDevice->pImpl;
   uint32_t ndiv, pdiv, mdiv, pll_div, ctrl;

   if (BAST_g3_P_IsLdpcOn_isrsafe(h))
   {
      /* for 7346: */
      /* AFEC_0 = xtal * CLKGEN_PLL_RAAGA_PLL_DIV[12:3] / (2 * CLKGEN_PLL_RAAGA_PLL_DIV[2:0] * AFEC_GLOBAL_CLK_CNTRL[7:0]) */
      /* AFEC_1 = xtal * CLKGEN_PLL_VCXO_PLL_DIV[12:3] / (2 * CLKGEN_PLL_VCXO_PLL_DIV[2:0] * AFEC_GLOBAL_CLK_CNTRL[7:0]) */
      if (h->channel == 0)
      {
         pll_div = BREG_Read32(hDev->hRegister, BCHP_CLKGEN_PLL_RAAGA_PLL_DIV);
      }
      else
      {
         pll_div = BREG_Read32(hDev->hRegister, BCHP_CLKGEN_PLL_VCXO_PLL_DIV);
      }
      ctrl = BREG_Read32(hDev->hRegister, BCHP_AFEC_GLOBAL_CLK_CNTRL);
      ndiv = (pll_div >> 3) & 0x3FF;
      pdiv = pll_div & 0x7;
      mdiv = ctrl & 0xFF;
      *pFreq = (hDev->xtalFreq * ndiv) / (2 * pdiv * mdiv);

      /* BDBG_ERR(("afec_freq=%u", *pFreq)); */
      return BERR_SUCCESS;
   }
   else
   {
      *pFreq = 0;
      return BERR_NOT_INITIALIZED;
   }
}
#endif
