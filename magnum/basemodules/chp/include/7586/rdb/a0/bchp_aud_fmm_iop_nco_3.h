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
 * Date:           Generated on               Wed Feb 11 10:13:57 2015
 *                 Full Compile MD5 Checksum  f7f4bd55341805fcfe958ba5e47e65f4
 *                     (minus title and desc)
 *                 MD5 Checksum               95b679a9655597a92593cae55222c397
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_AUD_FMM_IOP_NCO_3_H__
#define BCHP_AUD_FMM_IOP_NCO_3_H__

/***************************************************************************
 *AUD_FMM_IOP_NCO_3
 ***************************************************************************/
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_CONTROL 0x00cb1200 /* [RW] Rate Manager Controls */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_RATE_RATIO 0x00cb1204 /* [RW] Rate Manager Output Rate Setting I */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_SAMPLE_INC 0x00cb1208 /* [RW] Rate Manager Output Rate Setting II */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_PHASE_INC 0x00cb120c /* [RW] Rate Manager NCO Phase Increment */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_INTEGRATOR_HI 0x00cb1210 /* [WO] Loop Filter Integrator HI Value */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_INTEGRATOR_LO 0x00cb1214 /* [WO] Loop Filter Integrator LO Value */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_OFFSET 0x00cb1218 /* [RW] Rate Manager Output Formatting */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_FORMAT 0x00cb121c /* [RW] Rate Manager Output Formatting */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_SKIP_REPEAT_CONTROL 0x00cb1220 /* [RW] Time Base Skip or Repeat Control Register */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_SKIP_REPEAT_GAP 0x00cb1224 /* [RW] Time Base Skip or Repeat Gap Count Register */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_SKIP_REPEAT_NUMBER 0x00cb1228 /* [RW] Time Base Skip or Repeat Count Register */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_INTEGRATOR_HI_STATUS 0x00cb122c /* [RO] Loop Filter Integrator HI Status */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_INTEGRATOR_LO_STATUS 0x00cb1230 /* [RO] Loop Filter Integrator LO Status */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_STATUS 0x00cb1234 /* [RO] Rate Manager Status Register */
#define BCHP_AUD_FMM_IOP_NCO_3_MCLK_GEN_0_STATUS_CLEAR 0x00cb1238 /* [RW] Rate Manager Status Clear Register */

#endif /* #ifndef BCHP_AUD_FMM_IOP_NCO_3_H__ */

/* End of File */
