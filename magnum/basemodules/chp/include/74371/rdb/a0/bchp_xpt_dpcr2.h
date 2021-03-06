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
 * Date:           Generated on               Mon Nov 17 11:53:48 2014
 *                 Full Compile MD5 Checksum  7c712d144f2919fdac508431993b36d7
 *                     (minus title and desc)
 *                 MD5 Checksum               bc950b877a17d97d6325a810f8bd832e
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15193
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_XPT_DPCR2_H__
#define BCHP_XPT_DPCR2_H__

/***************************************************************************
 *XPT_DPCR2 - XPT DPCR2 Control Registers
 ***************************************************************************/
#define BCHP_XPT_DPCR2_PID_CH                    0x00a02100 /* [RW] Data Transport PCR PID Channel Register */
#define BCHP_XPT_DPCR2_CTRL                      0x00a02104 /* [RW] Data Transport PCR Control Register */
#define BCHP_XPT_DPCR2_INTR_STATUS_REG           0x00a02108 /* [RW] Interrupt Status Register */
#define BCHP_XPT_DPCR2_INTR_STATUS_REG_EN        0x00a0210c /* [RW] Interrupt Status Enable Register */
#define BCHP_XPT_DPCR2_STC_EXT_CTRL              0x00a02110 /* [RW] Data Transport PCR STC Extension Control Register */
#define BCHP_XPT_DPCR2_MAX_PCR_ERROR             0x00a02120 /* [RW] Data Transport PCR Max PCR Error Register */
#define BCHP_XPT_DPCR2_SEND_BASE                 0x00a02124 /* [RW] Data Transport PCR Send Base Register */
#define BCHP_XPT_DPCR2_SEND_EXT                  0x00a02128 /* [RW] Data Transport PCR Send Extension Register */
#define BCHP_XPT_DPCR2_STC_EXT_CTRL27            0x00a0212c /* [RO] Data Transport PCR STC Extension Control Register (Test Only) */
#define BCHP_XPT_DPCR2_STC_HI                    0x00a02130 /* [RO] Data Transport PCR STC MSBs Register */
#define BCHP_XPT_DPCR2_STC_LO                    0x00a02134 /* [RO] Data Transport PCR STC LSBs Register */
#define BCHP_XPT_DPCR2_PWM_CTRLVALUE             0x00a02138 /* [RO] Data Transport PCR PWM Control Value Register */
#define BCHP_XPT_DPCR2_LAST_PCR_HI               0x00a0213c /* [RO] Data Transport PCR Last PCR MSBs Register */
#define BCHP_XPT_DPCR2_LAST_PCR_LO               0x00a02140 /* [RO] Data Transport PCR Last PCR LSBs Register */
#define BCHP_XPT_DPCR2_STC_BASE_LSBS             0x00a02148 /* [RO] Data Transport PCR STC Base LSBs Register */
#define BCHP_XPT_DPCR2_PHASE_ERROR               0x00a0214c /* [RO] Timebase Last Phase Error */
#define BCHP_XPT_DPCR2_LOOP_CTRL                 0x00a02150 /* [RW] Timebase Control */
#define BCHP_XPT_DPCR2_REF_PCR_PRESCALE          0x00a02154 /* [RW] Timebase Frequency Reference Prescale Control */
#define BCHP_XPT_DPCR2_REF_PCR_INC               0x00a02158 /* [RW] Timebase Frequency Reference Increment Control */
#define BCHP_XPT_DPCR2_CENTER                    0x00a0215c /* [RW] Timebase Center Frequency */
#define BCHP_XPT_DPCR2_ACCUM_VALUE               0x00a02160 /* [RW] Timebase Loop Filter Integrator */
#define BCHP_XPT_DPCR2_PCR_COUNT                 0x00a02164 /* [RO] Data Transport PCR Phase Error Register */
#define BCHP_XPT_DPCR2_SOFT_PCR_CTRL             0x00a02168 /* [RW] Data Transport Soft PCR Control Register */
#define BCHP_XPT_DPCR2_SOFT_PCR_BASE             0x00a0216c /* [RW] Data Transport Soft PCR BASE Register */
#define BCHP_XPT_DPCR2_SOFT_PCR_EXT              0x00a02170 /* [RW] Data Transport Soft PCR Extension Register */
#define BCHP_XPT_DPCR2_PHASE_ERROR_CLAMP         0x00a02174 /* [RW] Timebase Phase Error Control */

#endif /* #ifndef BCHP_XPT_DPCR2_H__ */

/* End of File */
