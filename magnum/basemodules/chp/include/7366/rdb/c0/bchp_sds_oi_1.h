/****************************************************************************
 *     Copyright (c) 1999-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Thu Feb 12 15:15:53 2015
 *                 Full Compile MD5 Checksum  ca339b82db08da0250a17ca09932699d
 *                     (minus title and desc)
 *                 MD5 Checksum               502556bfbdc2f4341f93db8b4326b3ab
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_SDS_OI_1_H__
#define BCHP_SDS_OI_1_H__

/***************************************************************************
 *SDS_OI_1 - SDS Output Interface Register Set
 ***************************************************************************/
#define BCHP_SDS_OI_1_OIFCTL00                   0x01241480 /* [RW] Output Interface Control 00 (Formerly, OIFCTL2,OIFCTL,PSCTL) */
#define BCHP_SDS_OI_1_OIFCTL01                   0x01241484 /* [RW] Output Interface Control 01 (Formerly, OIFCTL4,OIFCTL3[15:0]) */
#define BCHP_SDS_OI_1_OIFCTL02                   0x01241488 /* [RW] Output Interface Control 02 (Formerly, OIFCTL5) */
#define BCHP_SDS_OI_1_OPLL                       0x0124148c /* [RW] Output Interface NCO clock rate numerator */
#define BCHP_SDS_OI_1_OPLL2                      0x01241490 /* [RW] Output Interface NCO clock rate delta */
#define BCHP_SDS_OI_1_FERC                       0x01241494 /* [RW] Output Interface MPEG frame error counter */
#define BCHP_SDS_OI_1_FRC                        0x01241498 /* [RW] Output Interface MPEG frame counter */
#define BCHP_SDS_OI_1_OSIGPN                     0x0124149c /* [RO] Output Interface signature analyzer */
#define BCHP_SDS_OI_1_OSUBD                      0x012414a0 /* [RW] Output Interface PLL Sub-divider control */
#define BCHP_SDS_OI_1_OCOEF                      0x012414a4 /* [RW] Output Interface Loop Filter Coefficient */
#define BCHP_SDS_OI_1_OFI                        0x012414a8 /* [RW] Output Interface Loop Filter Integrator */
#define BCHP_SDS_OI_1_OPLL_NPDIV                 0x012414b0 /* [RW] Output Interface PLL Feedback Divider Control */
#define BCHP_SDS_OI_1_OPLL_MDIV_CTRL             0x012414b4 /* [RW] Output Interface PLL Post-divider Control */
#define BCHP_SDS_OI_1_OPLL_CTRL                  0x012414b8 /* [RW] Output Interface PLL Control */
#define BCHP_SDS_OI_1_OPLL_SSC_CTRL1             0x012414bc /* [RW] Output Interface PLL Spread Spectrum Control 1 */
#define BCHP_SDS_OI_1_OPLL_SSC_CTRL0             0x012414c0 /* [RW] Output Interface PLL Spread Spectrum Control 0 */
#define BCHP_SDS_OI_1_OPLL_STATUS                0x012414c4 /* [RO] Output Interface PLL Status */
#define BCHP_SDS_OI_1_OPLL_PWRDN_RST             0x012414c8 /* [RW] Output Interface PLL Power Down and Reset Control */
#define BCHP_SDS_OI_1_OPLL_CTRL2                 0x012414cc /* [RW] Output Interface PLL Control 2 */

#endif /* #ifndef BCHP_SDS_OI_1_H__ */

/* End of File */
