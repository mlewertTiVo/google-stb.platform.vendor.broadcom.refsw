/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Wed Mar 27 10:49:16 2013
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_SDS_CG_1_H__
#define BCHP_SDS_CG_1_H__

/***************************************************************************
 *SDS_CG_1 - 1 SDS Clockgen Register Set
 ***************************************************************************/
#define BCHP_SDS_CG_1_RSTCTL                     0x01241000 /* Reset Control */
#define BCHP_SDS_CG_1_CGDIV00                    0x01241010 /* Clock Generator Divider register 0 (Formerly,CGDIV4,CGDIV3,CGDIV7[7:4],CGMISC,CGCTRL) */
#define BCHP_SDS_CG_1_CGDIV01                    0x01241014 /* Clock Generator Divider register 1 (Formerly,CGDIV11,CGDIV10,CGDIV9,CGDIV8) */
#define BCHP_SDS_CG_1_SPLL_NPDIV                 0x01241020 /* Sample Clock PLL Feedback Divider Control */
#define BCHP_SDS_CG_1_SPLL_MDIV_CTRL             0x01241024 /* Sample Clock PLL Post-divider Control */
#define BCHP_SDS_CG_1_SPLL_CTRL                  0x01241028 /* Sample Clock PLL Control */
#define BCHP_SDS_CG_1_SPLL_SSC_CTRL1             0x0124102c /* Sample Clock PLL Spread Spectrum Control 1 */
#define BCHP_SDS_CG_1_SPLL_SSC_CTRL0             0x01241030 /* Sample Clock PLL Spread Spectrum Control 0 */
#define BCHP_SDS_CG_1_SPLL_STATUS                0x01241034 /* Sample Clock PLL Status */
#define BCHP_SDS_CG_1_SPLL_PWRDN_RST             0x01241038 /* Sample Clock PLL Power Down and Reset Control */
#define BCHP_SDS_CG_1_PMCG_CTL                   0x0124103c /* Per Module Clock Gating Control */

#endif /* #ifndef BCHP_SDS_CG_1_H__ */

/* End of File */
