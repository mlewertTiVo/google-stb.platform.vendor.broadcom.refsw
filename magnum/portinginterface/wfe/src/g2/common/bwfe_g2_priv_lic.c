/***************************************************************************
 *     Copyright (c) 2005-2013, Broadcom Corporation
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
#include "bwfe.h"
#include "bwfe_priv.h"
#include "bwfe_g2_priv.h"

BDBG_MODULE(bwfe_g2_priv_lic);

#define BWFE_DEBUG_LIC_TAP_COMPUTER(x) /*x*/


#ifndef BWFE_EXCLUDE_LIC_TAP_COMPUTER
static double EEt[BWFE_LIC_S * BWFE_LIC_L1][BWFE_LIC_S * BWFE_LIC_L1];  /* matrix(s*L x s*L) */
static double Rt[BWFE_LIC_S * BWFE_LIC_L1][BWFE_LIC_L1];                /* matrix(s*L x L) */
static double bvect[BWFE_LIC_S * BWFE_LIC_L1];
static double tapOut[BWFE_LIC_L1][BWFE_LIC_S * BWFE_LIC_L1];   /* matrix(L x s*L) */
static double tapCoeff[BWFE_LIC_S * BWFE_LIC_L1][BWFE_LIC_L1]; /* matrix(s*L x L) */
static uint32_t indx[BWFE_LIC_S * BWFE_LIC_L1];
#endif


/******************************************************************************
 BWFE_g2_Lic_P_PowerUp()
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_PowerUp(BWFE_ChannelHandle h, bool bEnableLic, bool bEnableNr)
{
   BWFE_g2_P_ChannelHandle *hChn = (BWFE_g2_P_ChannelHandle *)h->pImpl;
   BERR_Code retCode = BERR_SUCCESS;
   uint8_t slice;
   
   if (h->bReference)
      return BERR_NOT_SUPPORTED;
   
   if (!h->bEnabled)
      return BWFE_ERR_POWERED_DOWN;
   
   BDBG_MSG(("LIC%d pwrup", h->channel));
   BWFE_P_OrRegister(h, BCHP_WFE_ANA_CLK_CTRL_IN, 0x00000006); /* enable lic, nr clock */

   /* reset lic taps */
   BWFE_g2_Lic_P_ResetTaps(h);

#if (BCHP_CHIP==4550)
   for (slice = 0; slice < h->totalSlices; slice++)
   {
      /* set dco level for ping and pong lanes per slice */
      BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_LICDCO_SLC, 0x04000400);
   }
#endif

   if (!bEnableNr)
   {
      /* bypass lic2 */
      BDBG_MSG(("bypass NR%d", h->channel));
      BWFE_P_ReadModifyWriteRegister(h, BCHP_WFE_CORE_NRNOTCHCTL, ~0x00000030, 0x00000020);  /* select dco inputs */
      BWFE_P_OrRegister(h, BCHP_WFE_CORE_NRDCOCTL_THR, 0x00000006);       /* freeze and bypass NR DCO for AGC THR */

      for (slice = 0; slice < h->totalSlices; slice++)
      {
         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PI_SLC, 0x00000006);    /* freeze and bypass NR DCO for ping */
         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PO_SLC, 0x00000006);    /* freeze and bypass NR DCO for pong */

         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PI_SLC, 0x00000006);  /* freeze and bypass NR NOTCH for ping */
         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PO_SLC, 0x00000006);  /* freeze and bypass NR NOTCH for pong */

         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRAGCCTL_PI_SLC, 0x00100006);    /* freeze and bypass NR AGC for ping */
         BWFE_P_OrSliceReg(h, slice, BCHP_WFE_CORE_NRAGCCTL_PO_SLC, 0x00100006);    /* freeze and bypass NR AGC for pong */
      }
   }
   else
   {
      /* initialize lic2 */
      BWFE_P_ReadModifyWriteRegister(h, BCHP_WFE_CORE_NRNOTCHCTL, ~0x00000030, 0x00000010);  /* select dco outputs */
      BWFE_P_WriteRegister(h, BCHP_WFE_CORE_NRDCOCTL_THR, 0x000000A1);     /* reset NR DCO for AGC THR: beta_dco=10, unfreeze, unbypass */
      BWFE_P_AndRegister(h, BCHP_WFE_CORE_NRDCOCTL_THR, ~0x00000001);      /* release reset */
      
      BWFE_P_OrRegister(h, BCHP_WFE_CORE_NRAGCTHR, 0x00100000);            /* use agc threshold offset */

      for (slice = 0; slice < h->totalSlices; slice++)
      {
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRDCOCTL_PI_SLC, 0x00000031);  /* reset NR DCO for ping: acq beta_dco=3, unfreeze, unbypass */
         BWFE_P_AndSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PI_SLC, ~0x00000001);        /* release reset */
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRDCOCTL_PO_SLC, 0x00000031);  /* reset NR DCO for pong: acq beta_dco=3, unfreeze, unbypass */
         BWFE_P_AndSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PO_SLC, ~0x00000001);        /* release reset */

         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PI_SLC, 0x000000A1);   /* reset NR NOTCH for ping: beta_dco=10, unfreeze, unbypass */
         BWFE_P_AndSliceReg(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PI_SLC, ~0x00000001);         /* release reset */
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PO_SLC, 0x000000A1);   /* reset NR NOTCH for pong: beta_dco=10, unfreeze, unbypass */
         BWFE_P_AndSliceReg(h, slice, BCHP_WFE_CORE_NRNOTCHCTL_PO_SLC, ~0x00000001);         /* release reset */
      }

      for (slice = 0; slice < h->totalSlices; slice++)
      {
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_REG_NR_AGC_LF_INT_WDATA_PI_SLC, 0x90000000);  /* clear agc integrators */
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_REG_NR_AGC_LF_INT_WDATA_PO_SLC, 0x90000000);  /* clear agc integrators */

         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRAGCCTL_PI_SLC, 0x00005000 | ((hChn->nrAgcK1 & 0xF) << 4)); /* ping: set acq agc_k1, unfreeze, unbypass */
         BWFE_P_WriteSliceRegister(h, slice, BCHP_WFE_CORE_NRAGCCTL_PO_SLC, 0x00005000 | ((hChn->nrAgcK1 & 0xF) << 4)); /* pong: set acq agc_k1, unfreeze, unbypass */
      }

      BKNI_Sleep(50);

      for (slice = 0; slice < h->totalSlices; slice++)
      {
         BWFE_P_ReadModifyWriteSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PI_SLC, ~0x000000F0, 0x000000C0);   /* tracking ping: beta_dco=12 */
         BWFE_P_ReadModifyWriteSliceReg(h, slice, BCHP_WFE_CORE_NRDCOCTL_PO_SLC, ~0x000000F0, 0x000000C0);   /* tracking pong: beta_dco=12 */
      }
   }

   if (!bEnableLic)
   {
      BDBG_MSG(("bypass LIC%d", h->channel));
      BWFE_P_OrRegister(h, BCHP_WFE_CORE_LICCTL1, 0x00000008);       /* bypass lic */
      BWFE_P_AndRegister(h, BCHP_WFE_ANA_CLK_CTRL_IN, ~0x00000004);  /* disable lic clock */
   }
   else
   {
      /* unbypass lic */
      BWFE_P_AndRegister(h, BCHP_WFE_CORE_LICCTL1, ~0x00000008);
   }

   /* enable LIC output last */
   BWFE_P_OrRegister(h, BCHP_WFE_CORE_LICCTL1, 0x00000010);
   return retCode;
}


/******************************************************************************
 BWFE_g2_Lic_P_PowerDown()
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_PowerDown(BWFE_ChannelHandle h)
{
   BERR_Code retCode = BERR_SUCCESS;
   
   if (h->bReference)
      return BERR_NOT_SUPPORTED;
   
   if (!h->bEnabled)
      return BWFE_ERR_POWERED_DOWN;
   
   BDBG_MSG(("LIC%d pwrdown", h->channel));
   BWFE_P_AndRegister(h, BCHP_WFE_CORE_LICCTL1, ~0x00000010);     /* disable LIC output */
   BWFE_P_AndRegister(h, BCHP_WFE_CORE_LICCTL2, ~0x000000FF);     /* deselect lanes */
   BWFE_P_AndRegister(h, BCHP_WFE_ANA_CLK_CTRL_IN, ~0x00000006);  /* disable lic, nr clock */
   return retCode;
}


/******************************************************************************
 BWFE_g2_Lic_P_ResetTaps()
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_ResetTaps(BWFE_ChannelHandle h)
{
   BERR_Code retCode = BERR_SUCCESS;
   
   if (h->bReference)
      return BERR_NOT_SUPPORTED;
   
   /* reset LIC and read default taps */
   BWFE_P_ToggleBit(h, BCHP_WFE_CORE_LICCTL1, 0x00000007);
#ifndef BWFE_EXCLUDE_LIC_TAP_COMPUTER
   retCode = BWFE_g2_Lic_P_ReadTaps(h);
#endif
   
   return retCode;
}


#ifndef BWFE_EXCLUDE_LIC_TAP_COMPUTER
/******************************************************************************
 BWFE_g2_Lic_P_ReadTaps()
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_ReadTaps(BWFE_ChannelHandle h)
{
   BWFE_g2_P_ChannelHandle *hChn = (BWFE_g2_P_ChannelHandle *)h->pImpl;
   BERR_Code retCode = BERR_SUCCESS;
   uint8_t lane, tap;
   uint32_t val;
   
   if (h->bReference)
      return BERR_NOT_SUPPORTED;
   
   for (lane = 0; lane < BWFE_NUM_LANES; lane++)
   {
      /* lane selection */
      BWFE_P_ReadModifyWriteRegister(h, BCHP_WFE_CORE_LICCTL2, ~0x00000003, 1 << lane);
      
      /* read lic taps */
      BWFE_P_WriteRegister(h, BCHP_WFE_CORE_LICCOEFA, 0);   /* auto-incrementing address */
      for (tap = 0; tap < BWFE_NUM_LIC_TAPS; tap++)
      {
         BWFE_P_ReadRegister(h, BCHP_WFE_CORE_LICCOEFD, &val);
         hChn->licCoeff[lane][tap] = val;
      }
   }
   
   /* deselect lanes */
   BWFE_P_AndRegister(h, BCHP_WFE_CORE_LICCTL1, ~0x00000003);
   
   return retCode;
}


/******************************************************************************
 BWFE_g2_Lic_P_WriteTaps()
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_WriteTaps(BWFE_ChannelHandle h)
{
   BWFE_g2_P_ChannelHandle *hChn = (BWFE_g2_P_ChannelHandle *)h->pImpl;
   BERR_Code retCode = BERR_SUCCESS;
   uint8_t lane, tap;
   uint32_t val;

   if (h->bReference)
      return BERR_NOT_SUPPORTED;

   for (lane = 0; lane < BWFE_LIC_L1; lane++)
   {
      /* lane selection */
      BWFE_P_ReadModifyWriteRegister(h, BCHP_WFE_CORE_LICCTL2, ~0x000000FF, 1 << lane);

      /* write lic taps */
      BWFE_P_WriteRegister(h, BCHP_WFE_CORE_LICCOEFA, 0);   /* auto-incrementing address */
      for (tap = 0; tap < BWFE_NUM_LIC_TAPS; tap++)
      {
         val = hChn->licCoeff[lane][tap];
         BWFE_P_WriteRegister(h, BCHP_WFE_CORE_LICCOEFD, val);
      }

      /* load lic coeffs */
      BWFE_P_ToggleBit(h, BCHP_WFE_CORE_LICCOEFEN, 0x00000001);
   }

   /* deselect lanes */
   BWFE_P_AndRegister(h, BCHP_WFE_CORE_LICCTL2, ~0x000000FF);
   return retCode;
}


#ifdef BWFE_FLOAT_MODE
/******************************************************************************
 BWFE_g2_Lic_P_CalcMatrixEEt() - EEt = E * E'
 input EEt: matrix(s*L x s*L), d: matrix(L x 1)
******************************************************************************/
void BWFE_g2_Lic_P_CalcMatrixEEt(double *d)
{
   double x_frac, sin_xfrac, sin_x, sinc_x;
   double vec[2];
   int32_t x_int;
   uint8_t i, j, m, n;

   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      for (j = i; j < BWFE_LIC_L1; j++)
      {
         x_frac = d[BWFE_LIC_L1 - 1 - j] - d[BWFE_LIC_L1 - 1 - i];

         if (BWFE_ABS(x_frac) < BWFE_SINC_LBOUND_FP)
         {
            /* case lower bound exceeded */
            for (m = 0; m < BWFE_LIC_S; m++)
            {
               for (n = 0; n < BWFE_LIC_S; n++)
               {
                  /* set diagonals to one, others to zero */
                  if ((m == n) && (j == i))
                     EEt[i * BWFE_LIC_S + m][j * BWFE_LIC_S + n] = 1.0;
                  else
                     EEt[i * BWFE_LIC_S + m][j * BWFE_LIC_S + n] = 0.0;
               }
            }
         }
         else
         {
            /* calculate sin_xfrac = sin(PI*x_frac) */
            BWFE_P_CordicRotationMode_FP(vec, x_frac, true);   /* unit mode */
            sin_xfrac = vec[1];
            for (m = 0; m < BWFE_LIC_S; m++)
            {
               x_int = m * BWFE_LIC_L1 + j - i;
               sin_x = sin_xfrac;
               
               if (x_int % 2 != 0)
                  sin_x = -sin_x;
               
               /* calculate sinc(x) */
               sinc_x = sin_x / ((x_int + x_frac) * BWFE_PI_FP);
               
               for (n = 0; n < BWFE_LIC_S - m; n++)
                  EEt[i * BWFE_LIC_S + n][j * BWFE_LIC_S + n + m] = sinc_x;
            }

            for (m = 1; m < BWFE_LIC_S; m++)
            {
               x_int = -m * BWFE_LIC_L1 + j - i;
               sin_x = sin_xfrac;
               
               if (x_int % 2 != 0)
                  sin_x = -sin_x;

               /* calculate sinc(x) */
               sinc_x = sin_x / ((x_int + x_frac) * BWFE_PI_FP);

               for (n = 0; n < BWFE_LIC_S - m; n++)
                  EEt[i * BWFE_LIC_S + n + m][j * BWFE_LIC_S + n] = sinc_x;
            }
         }
      }

      for (j = 0; j < i; j++)
      {
         for (m = 0; m < BWFE_LIC_S; m++)
         {
            /* use symmetry */
            for (n = 0; n < BWFE_LIC_S; n++)
               EEt[i * BWFE_LIC_S + m][j * BWFE_LIC_S + n] = EEt[j * BWFE_LIC_S + n][i * BWFE_LIC_S + m];
         }
      }
   }
}


/******************************************************************************
 BWFE_g2_Lic_P_CalcMatrixEPt() - EPt = E * P'
 input Rt: matrix(s*L x L), d: matrix(L x 1)
******************************************************************************/
void BWFE_g2_Lic_P_CalcMatrixEPt(double *d)
{
   double sin_xfrac, sin_x, sinc_x;
   double vec[2];
   uint8_t d_prime, mod_i, z1;
   int32_t x_int;
   uint8_t i, j, k;

   d_prime = (BWFE_LIC_S - 1) / 2;
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      /* TBD no main tap therefore mod_i = i? */
      mod_i = i % BWFE_LIC_L;
      if (i > mod_i)
         z1 = (d_prime + 1) * BWFE_LIC_L1 + mod_i;
      else
         z1 = d_prime * BWFE_LIC_L1 + mod_i;
      
      for (j = 0; j < BWFE_LIC_L1; j++)
      {
         /* calculate sin_xfrac = sin(PI*d[BWFE_NUM_LANES-1-j]) */
         BWFE_P_CordicRotationMode_FP(vec, d[BWFE_LIC_L1 - 1 - j], true);   /* unit mode */
         sin_xfrac = vec[1];
         
         for (k = 0; k < BWFE_LIC_S; k++)
         {
            x_int = z1 - k * BWFE_LIC_L1 - j;
            if (BWFE_ABS(d[BWFE_LIC_L1 - 1 - j]) < BWFE_SINC_LBOUND_FP)
            {
               /* case lower bound exceeded */
               if (x_int)
                  sinc_x = 0.0;
               else
                  sinc_x = 1.0;
            }
            else
            {
               if (x_int % 2)
                  sin_x = sin_xfrac;
               else
                  sin_x = -sin_xfrac;
               
               /* calculate sinc(x) */
               sinc_x = sin_x / ((x_int - d[BWFE_LIC_L1 - 1 - j]) * BWFE_PI_FP);
            }
            Rt[j * BWFE_LIC_S + k][i] = sinc_x;
         }
      }
   }
}


#if 0
/**************************************************************
 BWFE_g2_Lic_P_CalcMatrixGaussJ - reduced row echelon form
**************************************************************/
#define BWFE_SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}
void BWFE_g2_Lic_P_CalcMatrixGaussJ(double **a, int n, double **b, int m)
{
	uint32_t indxc[BWFE_LIC_S * BWFE_LIC_L1];
   uint32_t indxr[BWFE_LIC_S * BWFE_LIC_L1];
   uint32_t ipiv[BWFE_LIC_S * BWFE_LIC_L1];
	uint8_t i, icol = 0, irow = 0;
   uint8_t j, k, l, ll;
	double big, dum, pivinv, temp;
   
   n = BWFE_LIC_S * BWFE_LIC_L1;
   m = BWFE_LIC_L;
   
   for (j = 0; j < n; j++)
      ipiv[j] = 0;
   
   for (i = 0; i < n; i++)
   {
      big = 0.0;
      for (j = 0; j < n; j++)
      {
         if (ipiv[j] != 1)
         {
            for (k = 0; k < n; k++)
            {
               if (ipiv[k] == 0)
               {
                  if (BWFE_ABS(a[j][k]) >= big)
                  {
                     big = BWFE_ABS(a[j][k]);
                     irow = j;
                     icol = k;
                  }
               }
            }
         }
      }
      
		++(ipiv[icol]);
		if (irow != icol)
      {
			for (l = 0; l < n; l++)
            BWFE_SWAP(a[irow][l], a[icol][l])
			for (l = 0; l < m; l++)
            BWFE_SWAP(b[irow][l], b[icol][l])
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol][icol] == 0.0)
      {
         BDBG_ERR(("Singular Matrix!\n"));
      }
      
		pivinv = 1.0 / a[icol][icol];
		a[icol][icol] = 1.0;
		for (l = 0; l < n; l++)
         a[icol][l] *= pivinv;
		for (l = 0; l < m; l++)
         b[icol][l] *= pivinv;
      
		for (ll = 0; ll < n; ll++)
      {
			if (ll != icol)
         {
				dum = a[ll][icol];
				a[ll][icol] = 0.0;
				for (l = 0; l < n; l++)
               a[ll][l] -= a[icol][l] * dum;
				for (l = 0; l < m; l++)
               b[ll][l] -= b[icol][l] * dum;
			}
      }
	}
   
	for (l = n - 1; l >= 0; l--)
   {
		if (indxr[l] != indxc[l])
      {
			for (k = 0; k < n; k++)
				BWFE_SWAP(a[k][indxr[l]], a[k][indxc[l]]);
      }
	}
}
#endif


/**************************************************************
 BWFE_g2_Lic_P_CalcMatrixLuDecomp()
**************************************************************/
#define BWFE_TINY 1.0e-20
void BWFE_g2_Lic_P_CalcMatrixLuDecomp(int n, double *d)
{
   uint8_t i, j, k, i_max = 0;
   double big, dum, sum, temp;
   double vv[BWFE_LIC_S * BWFE_LIC_L1];

   n = BWFE_LIC_S * BWFE_LIC_L1;
   *d = 1.0;
   
   for (i = 0; i < n; i++)
   {
      big = 0.0;
      for (j = 0; j < n; j++)
      {
         if ((temp = BWFE_ABS(EEt[i][j])) > big)
            big = temp;
      }
      
      if (big == 0.0)
      {
         /* check for singular matrix */
         BDBG_ERR(("Singular Matrix!\n"));
      }
      vv[i] = 1.0 / big;
   }
   
   for (j = 0; j < n; j++)
   {
      for (i = 0; i < j; i++)
      {
         sum = EEt[i][j];
         for (k = 0; k < i; k++)
            sum -= EEt[i][k] * EEt[k][j];
         EEt[i][j] = sum;
      }
      
      big = 0.0;
      for (i = j; i < n; i++)
      {
         sum = EEt[i][j];
         for (k = 0; k < j; k++)
            sum -= EEt[i][k] * EEt[k][j];
         EEt[i][j] = sum;
         if ((dum = vv[i] * BWFE_ABS(sum)) >= big)
         {
            big = dum;
            i_max = i;
         }
      }
      if (j != i_max)
      {
         for (k = 0; k < n; k++)
         {
            dum = EEt[i_max][k];
            EEt[i_max][k] = EEt[j][k];
            EEt[j][k] = dum;
         }
         *d = -(*d);
         vv[i_max] = vv[j];
      }
      indx[j] = i_max;
      
      if (EEt[j][j] == 0.0)
         EEt[j][j] = BWFE_TINY;
         
      if (j != (n - 1)) 
      {
         dum = 1.0 / (EEt[j][j]);
         for (i = j + 1; i < n; i++)
            EEt[i][j] *= dum;
      }
   }
}


/**************************************************************
 BWFE_g2_Lic_P_CalcMatrixLuBackSub()
**************************************************************/
void BWFE_g2_Lic_P_CalcMatrixLuBackSub(int n, double b[])
{
   int i, ii = 0, ip, j;
   double sum;
   
   for (i = 0; i < n; i++)
   {
      ip = indx[i];
      sum = b[ip];
      b[ip] = b[i];
      if (ii)
      {
         for (j = ii - 1; j < i; j++)
            sum -= EEt[i][j]*b[j];
      }
      else if (sum != 0.0)
      {
         ii = i + 1;
      }
      b[i] = sum;
   }
   
   for (i = n - 1; i >= 0; i--)
   {
      sum = b[i];
      for (j = i + 1; j < n; j++)
         sum -= EEt[i][j] * b[j];
      b[i] = sum / EEt[i][i];
   }
}
#endif


/******************************************************************************
 BWFE_g2_Lic_P_ComputeTaps()
 inputs: h->corrDelay
 outputs: h->licCoeff
******************************************************************************/
BERR_Code BWFE_g2_Lic_P_ComputeTaps(BWFE_ChannelHandle h)
{
   BERR_Code retCode = BERR_NOT_SUPPORTED;
   BWFE_g2_P_ChannelHandle *hChn = (BWFE_g2_P_ChannelHandle *)h->pImpl;
#ifdef BWFE_FLOAT_MODE
   int32_t val;
   uint8_t i, j, k, n;
#endif
   
   if (h->bReference)
      return BERR_NOT_SUPPORTED;
   
#ifdef BWFE_FLOAT_MODE
   double vec[2], delay[BWFE_LIC_L1], sin_delay[BWFE_LIC_L1];
   double dummy;
   
#if 0
   for (slice = 0; slice < BWFE_NUM_SLICES; slice++)
   {
   #if 0
      /* TBD enforce n to be multiple of L? */
      if( LICn1 % BWFE_NUM_LANES > 0)
         n = LICn1 + (BWFE_NUM_LANES - LICn1 % BWFE_NUM_LANES);
      else
         n = LICn1;
      hL = 2*n/BWFE_NUM_LANES + 1;
      t = BWFE_NUM_LIC_TAPS + hL - 1;
   #endif
      
      /* current LIC2 delays based on new corr values, limit change to +/-BWFE_DELAY_DELTA  */
      for (lane = 0; lane < BWFE_NUM_LANES; lane++)
      {
         if (hChn->bLicTrackingOn)
         {
            /* get delta from prev delays and limit change */
            deltaDelay = hChn->corrDelay[slice] - hChn->licDelay[slice][lane];
            if (deltaDelay >= BWFE_DELAY_DELTA_FP)
               hChn->licDelay[slice][lane] += BWFE_DELAY_DELTA_FP;
            else if (deltaDelay <= -BWFE_DELAY_DELTA_FP)
               hChn->licDelay[slice][lane] -= BWFE_DELAY_DELTA_FP;
            else
               hChn->licDelay[slice][lane] = hChn->corrDelay[slice];
         }
         else
         {
            /* use new correlator values */
            hChn->licDelay[slice][lane] = hChn->corrDelay[slice];
         }
         delay[lane] = (double)(hChn->licDelay[slice][lane] / BWFE_DELAY_SCALE_FP);
      }
   }
#else
   /* only need half of all demux lanes since demux lanes are the same */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      delay[i] = (double)hChn->corrDelay[i] / 2147483648.0; /* scale down 2^31 */
      BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("delay[%d]=%08X->(%08X%08X)\n", i, hChn->corrDelay[i], *(((int*)&delay[i])+1), *((int*)&delay[i])));
   }
#endif

   /* calculate sin(x) used by sin(x)/x later, note delay already norm by PI */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      /* calculate sin(x) using cordic */
      BWFE_P_CordicRotationMode_FP(vec, delay[i], true); /* unit mode */
      sin_delay[i] = vec[1];
   }

   /* perform E*E' direct closed form calculations, exploits symmetry */
   BWFE_g2_Lic_P_CalcMatrixEEt(delay);

#if 0
   /* print matrix */
   BKNI_Printf("\nMatrix EEt:\n");
   for (i = 0; i < BWFE_LIC_S * BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_S * BWFE_LIC_L1; j++)
         BKNI_Printf("%08X%08X   \n", *(((int*)&EEt[i][j])+1), *((int*)&EEt[i][j]));
      BKNI_Printf("\n");
   }
   return retCode;
#endif

   /* calculate Rt = E*P'  */
   BWFE_g2_Lic_P_CalcMatrixEPt(delay);

#if 0
   /* print matrix */
   BKNI_Printf("\nMatrix EPt:\n");
   for (j = 0; j < BWFE_LIC_L1; j++)
   {
      for (i = 0; i < BWFE_LIC_S * BWFE_LIC_L1; i++)
         BKNI_Printf("%08X%08X   \n", *(((int*)&Rt[i][j])+1), *((int*)&Rt[i][j]));
      BKNI_Printf("\n");
   }
   return retCode;
#endif

   /* solve (E*E')*R' = E*P' */
#if 0
   /* gauss-jordan elimination method */
   BWFE_g2_Lic_P_CalcMatrixGaussJ((double**)EEt, BWFE_NUM_LIC_TAPS * BWFE_NUM_LANES, (double**)Rt, BWFE_NUM_LANES);
#else
   /* LU decomposition method */
   BWFE_g2_Lic_P_CalcMatrixLuDecomp(BWFE_LIC_S * BWFE_LIC_L1, &dummy);

   /* solve one column at a time */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_S * BWFE_LIC_L1; j++)
         bvect[j] = Rt[j][i];
      BWFE_g2_Lic_P_CalcMatrixLuBackSub(BWFE_LIC_S * BWFE_LIC_L1, bvect);
      for (j = 0; j < BWFE_LIC_S * BWFE_LIC_L1; j++)
         Rt[j][i] = bvect[j];
   }
#endif

#if 0
   /* print matrix */
   BKNI_Printf("\nMatrix Rt:\n");
   for (i = 0; i < BWFE_LIC_S * BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_L1; j++)
         BKNI_Printf("%08X%08X   \n", *(((int*)&Rt[i][j])+1), *((int*)&Rt[i][j]));
      BKNI_Printf("\n");
   }
   return retCode;
#endif

   /* sort taps in intermediate array */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_L1; j++)
      {
         for (k = 0; k < BWFE_LIC_S; k++)
         {
            tapOut[i][j + k * BWFE_LIC_L1] = Rt[i * BWFE_LIC_S + k][BWFE_LIC_L1 - 1 - j];
            BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("tapOut[%d][%d]=%d (%08X%08X)\n", i, j+k*BWFE_LIC_L1, (int)tapOut[i][j+k*BWFE_LIC_L1], *(((int*)&tapOut[i][j+k*BWFE_LIC_L1])+1), *((int*)&tapOut[i][j+k*BWFE_LIC_L1])));
         }
      }
      BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("\n"));
   }

   /* final coefficients */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      n = 0;
      for (j = 0; j < BWFE_LIC_S; j++)
      {
         for (k = 0; k < BWFE_LIC_L1; k++)
         {
            tapCoeff[n][i] = tapOut[k][i + j * BWFE_LIC_L1];
            n++;
         }
      }
   }

   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_S * BWFE_LIC_L1; j++)
      {
         //BKNI_Printf("%18d ", BWFE_ROUND(65536.0 * tapCoeff[i][j]));  /* 2^16=65536   (was 2^18 == 262144)  */
         BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("tapCoeff[%d][%d]=(%08X%08X)\n", j, i, *(((int*)&tapCoeff[j][i])+1), *((int*)&tapCoeff[j][i])));
      }
      BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("\n"));
   }

   /* zero out lic coefficients */
   BKNI_Memset(hChn->licCoeff, 0, BWFE_NUM_LIC_TAPS * BWFE_LIC_L1);

   /* format lic taps for register programming TC2.16 */
   for (i = 0; i < BWFE_LIC_L1; i++)
   {
      for (j = 0; j < BWFE_LIC_S * BWFE_LIC_L1; j++)
      {
         val = BWFE_ROUND(65536.0 * tapCoeff[j][i]);
         hChn->licCoeff[i][j] = (uint32_t)val;
         BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("licCoef[%d][%d]=%d\n", i, j, hChn->licCoeff[i][j]));
      }
      BWFE_DEBUG_LIC_TAP_COMPUTER(BKNI_Printf("\n"));
   }

   /* write lic coefficients */
   BWFE_g2_Lic_P_WriteTaps(h);

   /* restore input to correlator */
   BWFE_P_WriteRegister(h, BCHP_WFE_CORE_CORRINSEL, hChn->corrInputSelect & 0x3);

   /* signal ready event */
   BKNI_SetEvent(hChn->hWfeReady);
   hChn->bLicTapCalcComplete = true;
#else
   /* TBD fixed point implementation */
#endif
   
   return retCode;
}
#endif
