/****************************************************************************
 *     Copyright (c) 1999-2015, Broadcom Corporation
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
 * Date:           Generated on               Tue Aug 25 10:46:55 2015
 *                 Full Compile MD5 Checksum  add20cb7888302c2ee8be1277223a4e4
 *                     (minus title and desc)
 *                 MD5 Checksum               f64b4ec86ad9ad7523e5d75b1dc732c5
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     126
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
 ***************************************************************************/

#ifndef BCHP_XPT_DPCR3_H__
#define BCHP_XPT_DPCR3_H__

/***************************************************************************
 *XPT_DPCR3 - XPT DPCR3 Control Registers
 ***************************************************************************/
#define BCHP_XPT_DPCR3_PID_CH                    0x00a07180 /* [RW] Data Transport PCR PID Channel Register */
#define BCHP_XPT_DPCR3_CTRL                      0x00a07184 /* [RW] Data Transport PCR Control Register */
#define BCHP_XPT_DPCR3_INTR_STATUS_REG           0x00a07188 /* [RW] Interrupt Status Register */
#define BCHP_XPT_DPCR3_INTR_STATUS_REG_EN        0x00a0718c /* [RW] Interrupt Status Enable Register */
#define BCHP_XPT_DPCR3_STC_EXT_CTRL              0x00a07190 /* [RW] Data Transport PCR STC Extension Control Register */
#define BCHP_XPT_DPCR3_MAX_PCR_ERROR             0x00a071a0 /* [RW] Data Transport PCR Max PCR Error Register */
#define BCHP_XPT_DPCR3_SEND_BASE                 0x00a071a4 /* [RW] Data Transport PCR Send Base Register */
#define BCHP_XPT_DPCR3_SEND_EXT                  0x00a071a8 /* [RW] Data Transport PCR Send Extension Register */
#define BCHP_XPT_DPCR3_STC_EXT_CTRL27            0x00a071ac /* [RO] Data Transport PCR STC Extension Control Register (Test Only) */
#define BCHP_XPT_DPCR3_STC_HI                    0x00a071b0 /* [RO] Data Transport PCR STC MSBs Register */
#define BCHP_XPT_DPCR3_STC_LO                    0x00a071b4 /* [RO] Data Transport PCR STC LSBs Register */
#define BCHP_XPT_DPCR3_PWM_CTRLVALUE             0x00a071b8 /* [RO] Data Transport PCR PWM Control Value Register */
#define BCHP_XPT_DPCR3_LAST_PCR_HI               0x00a071bc /* [RO] Data Transport PCR Last PCR MSBs Register */
#define BCHP_XPT_DPCR3_LAST_PCR_LO               0x00a071c0 /* [RO] Data Transport PCR Last PCR LSBs Register */
#define BCHP_XPT_DPCR3_STC_BASE_LSBS             0x00a071c8 /* [RO] Data Transport PCR STC Base LSBs Register */
#define BCHP_XPT_DPCR3_PHASE_ERROR               0x00a071cc /* [RO] Timebase Last Phase Error */
#define BCHP_XPT_DPCR3_LOOP_CTRL                 0x00a071d0 /* [RW] Timebase Control */
#define BCHP_XPT_DPCR3_REF_PCR_PRESCALE          0x00a071d4 /* [RW] Timebase Frequency Reference Prescale Control */
#define BCHP_XPT_DPCR3_REF_PCR_INC               0x00a071d8 /* [RW] Timebase Frequency Reference Increment Control */
#define BCHP_XPT_DPCR3_CENTER                    0x00a071dc /* [RW] Timebase Center Frequency */
#define BCHP_XPT_DPCR3_ACCUM_VALUE               0x00a071e0 /* [RW] Timebase Loop Filter Integrator */
#define BCHP_XPT_DPCR3_PCR_COUNT                 0x00a071e4 /* [RO] Data Transport PCR Phase Error Register */
#define BCHP_XPT_DPCR3_SOFT_PCR_CTRL             0x00a071e8 /* [RW] Data Transport Soft PCR Control Register */
#define BCHP_XPT_DPCR3_SOFT_PCR_BASE             0x00a071ec /* [RW] Data Transport Soft PCR BASE Register */
#define BCHP_XPT_DPCR3_SOFT_PCR_EXT              0x00a071f0 /* [RW] Data Transport Soft PCR Extension Register */
#define BCHP_XPT_DPCR3_PHASE_ERROR_CLAMP         0x00a071f4 /* [RW] Timebase Phase Error Control */

#endif /* #ifndef BCHP_XPT_DPCR3_H__ */

/* End of File */
