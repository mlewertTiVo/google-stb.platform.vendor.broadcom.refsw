/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Tue May  3 15:23:06 2016
 *                 Full Compile MD5 Checksum  1da1c3226a9ac5d06cc27c06c1aefe52
 *                     (minus title and desc)
 *                 MD5 Checksum               a58ec1309d39852a0c8fb54050da5f77
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     930
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_LEAP_WDG_H__
#define BCHP_LEAP_WDG_H__

/***************************************************************************
 *LEAP_WDG - CPU Watchdog Registers
 ***************************************************************************/
#define BCHP_LEAP_WDG_WDOGLOAD                   0x000ea000 /* [RW] Watchdog Load */
#define BCHP_LEAP_WDG_WDOGVALUE                  0x000ea004 /* [RO] Watchdog Value */
#define BCHP_LEAP_WDG_WDOGCONTROL                0x000ea008 /* [RW] Watchdog Control */
#define BCHP_LEAP_WDG_WDOGINTCLR                 0x000ea00c /* [WO] Watchdog Clear Interrupt */
#define BCHP_LEAP_WDG_WDOGRIS                    0x000ea010 /* [RO] Watchdog Raw Interrupt Status */
#define BCHP_LEAP_WDG_WDOGMIS                    0x000ea014 /* [RO] Watchdog Interrupt Status */
#define BCHP_LEAP_WDG_WDOGLOCK                   0x000eac00 /* [RW] Watchdog Lock */
#define BCHP_LEAP_WDG_WDOGITCR                   0x000eaf00 /* [RW] Watchdog Integration Test Control */
#define BCHP_LEAP_WDG_WDOGITOP                   0x000eaf04 /* [WO] Watchdog Integration Test Output Set */
#define BCHP_LEAP_WDG_WDOGPERIPHID0              0x000eafe0 /* [RO] Peripheral Identification Register */
#define BCHP_LEAP_WDG_WDOGPERIPHID1              0x000eafe4 /* [RO] Peripheral Identification Register */
#define BCHP_LEAP_WDG_WDOGPERIPHID2              0x000eafe8 /* [RO] Peripheral Identification Register */
#define BCHP_LEAP_WDG_WDOGPERIPHID3              0x000eafec /* [RO] Peripheral Identification Register */
#define BCHP_LEAP_WDG_WDOGPCELLID0               0x000eaff0 /* [RO] PrimeCell Identification Register */
#define BCHP_LEAP_WDG_WDOGPCELLID1               0x000eaff4 /* [RO] PrimeCell Identification Register */
#define BCHP_LEAP_WDG_WDOGPCELLID2               0x000eaff8 /* [RO] PrimeCell Identification Register */
#define BCHP_LEAP_WDG_WDOGPCELLID3               0x000eaffc /* [RO] PrimeCell Identification Register */

/***************************************************************************
 *WDOGLOAD - Watchdog Load
 ***************************************************************************/
/* LEAP_WDG :: WDOGLOAD :: WDOGLOAD [31:00] */
#define BCHP_LEAP_WDG_WDOGLOAD_WDOGLOAD_MASK                       0xffffffff
#define BCHP_LEAP_WDG_WDOGLOAD_WDOGLOAD_SHIFT                      0
#define BCHP_LEAP_WDG_WDOGLOAD_WDOGLOAD_DEFAULT                    0xffffffff

/***************************************************************************
 *WDOGVALUE - Watchdog Value
 ***************************************************************************/
/* LEAP_WDG :: WDOGVALUE :: WDOGVALUE [31:00] */
#define BCHP_LEAP_WDG_WDOGVALUE_WDOGVALUE_MASK                     0xffffffff
#define BCHP_LEAP_WDG_WDOGVALUE_WDOGVALUE_SHIFT                    0
#define BCHP_LEAP_WDG_WDOGVALUE_WDOGVALUE_DEFAULT                  0xffffffff

/***************************************************************************
 *WDOGCONTROL - Watchdog Control
 ***************************************************************************/
/* LEAP_WDG :: WDOGCONTROL :: reserved0 [31:02] */
#define BCHP_LEAP_WDG_WDOGCONTROL_reserved0_MASK                   0xfffffffc
#define BCHP_LEAP_WDG_WDOGCONTROL_reserved0_SHIFT                  2

/* LEAP_WDG :: WDOGCONTROL :: WDOGCONTROL [01:00] */
#define BCHP_LEAP_WDG_WDOGCONTROL_WDOGCONTROL_MASK                 0x00000003
#define BCHP_LEAP_WDG_WDOGCONTROL_WDOGCONTROL_SHIFT                0
#define BCHP_LEAP_WDG_WDOGCONTROL_WDOGCONTROL_DEFAULT              0x00000000

/***************************************************************************
 *WDOGINTCLR - Watchdog Clear Interrupt
 ***************************************************************************/
/* LEAP_WDG :: WDOGINTCLR :: reserved0 [31:02] */
#define BCHP_LEAP_WDG_WDOGINTCLR_reserved0_MASK                    0xfffffffc
#define BCHP_LEAP_WDG_WDOGINTCLR_reserved0_SHIFT                   2

/* LEAP_WDG :: WDOGINTCLR :: WDOGINTCLR [01:00] */
#define BCHP_LEAP_WDG_WDOGINTCLR_WDOGINTCLR_MASK                   0x00000003
#define BCHP_LEAP_WDG_WDOGINTCLR_WDOGINTCLR_SHIFT                  0

/***************************************************************************
 *WDOGRIS - Watchdog Raw Interrupt Status
 ***************************************************************************/
/* LEAP_WDG :: WDOGRIS :: reserved0 [31:01] */
#define BCHP_LEAP_WDG_WDOGRIS_reserved0_MASK                       0xfffffffe
#define BCHP_LEAP_WDG_WDOGRIS_reserved0_SHIFT                      1

/* LEAP_WDG :: WDOGRIS :: WDOGRIS [00:00] */
#define BCHP_LEAP_WDG_WDOGRIS_WDOGRIS_MASK                         0x00000001
#define BCHP_LEAP_WDG_WDOGRIS_WDOGRIS_SHIFT                        0
#define BCHP_LEAP_WDG_WDOGRIS_WDOGRIS_DEFAULT                      0x00000000

/***************************************************************************
 *WDOGMIS - Watchdog Interrupt Status
 ***************************************************************************/
/* LEAP_WDG :: WDOGMIS :: reserved0 [31:01] */
#define BCHP_LEAP_WDG_WDOGMIS_reserved0_MASK                       0xfffffffe
#define BCHP_LEAP_WDG_WDOGMIS_reserved0_SHIFT                      1

/* LEAP_WDG :: WDOGMIS :: WDOGMIS [00:00] */
#define BCHP_LEAP_WDG_WDOGMIS_WDOGMIS_MASK                         0x00000001
#define BCHP_LEAP_WDG_WDOGMIS_WDOGMIS_SHIFT                        0
#define BCHP_LEAP_WDG_WDOGMIS_WDOGMIS_DEFAULT                      0x00000000

/***************************************************************************
 *WDOGLOCK - Watchdog Lock
 ***************************************************************************/
/* LEAP_WDG :: WDOGLOCK :: WDOGLOCK [31:00] */
#define BCHP_LEAP_WDG_WDOGLOCK_WDOGLOCK_MASK                       0xffffffff
#define BCHP_LEAP_WDG_WDOGLOCK_WDOGLOCK_SHIFT                      0
#define BCHP_LEAP_WDG_WDOGLOCK_WDOGLOCK_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGITCR - Watchdog Integration Test Control
 ***************************************************************************/
/* LEAP_WDG :: WDOGITCR :: reserved0 [31:01] */
#define BCHP_LEAP_WDG_WDOGITCR_reserved0_MASK                      0xfffffffe
#define BCHP_LEAP_WDG_WDOGITCR_reserved0_SHIFT                     1

/* LEAP_WDG :: WDOGITCR :: WDOGITCR [00:00] */
#define BCHP_LEAP_WDG_WDOGITCR_WDOGITCR_MASK                       0x00000001
#define BCHP_LEAP_WDG_WDOGITCR_WDOGITCR_SHIFT                      0
#define BCHP_LEAP_WDG_WDOGITCR_WDOGITCR_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGITOP - Watchdog Integration Test Output Set
 ***************************************************************************/
/* LEAP_WDG :: WDOGITOP :: reserved0 [31:02] */
#define BCHP_LEAP_WDG_WDOGITOP_reserved0_MASK                      0xfffffffc
#define BCHP_LEAP_WDG_WDOGITOP_reserved0_SHIFT                     2

/* LEAP_WDG :: WDOGITOP :: WDOGITOP [01:00] */
#define BCHP_LEAP_WDG_WDOGITOP_WDOGITOP_MASK                       0x00000003
#define BCHP_LEAP_WDG_WDOGITOP_WDOGITOP_SHIFT                      0
#define BCHP_LEAP_WDG_WDOGITOP_WDOGITOP_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGPERIPHID0 - Peripheral Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPERIPHID0 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPERIPHID0_reserved0_MASK                 0xffffff00
#define BCHP_LEAP_WDG_WDOGPERIPHID0_reserved0_SHIFT                8

/* LEAP_WDG :: WDOGPERIPHID0 :: WDOGPERIPHID0 [07:00] */
#define BCHP_LEAP_WDG_WDOGPERIPHID0_WDOGPERIPHID0_MASK             0x000000ff
#define BCHP_LEAP_WDG_WDOGPERIPHID0_WDOGPERIPHID0_SHIFT            0
#define BCHP_LEAP_WDG_WDOGPERIPHID0_WDOGPERIPHID0_DEFAULT          0x00000005

/***************************************************************************
 *WDOGPERIPHID1 - Peripheral Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPERIPHID1 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPERIPHID1_reserved0_MASK                 0xffffff00
#define BCHP_LEAP_WDG_WDOGPERIPHID1_reserved0_SHIFT                8

/* LEAP_WDG :: WDOGPERIPHID1 :: WDOGPERIPHID1 [07:00] */
#define BCHP_LEAP_WDG_WDOGPERIPHID1_WDOGPERIPHID1_MASK             0x000000ff
#define BCHP_LEAP_WDG_WDOGPERIPHID1_WDOGPERIPHID1_SHIFT            0
#define BCHP_LEAP_WDG_WDOGPERIPHID1_WDOGPERIPHID1_DEFAULT          0x00000018

/***************************************************************************
 *WDOGPERIPHID2 - Peripheral Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPERIPHID2 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPERIPHID2_reserved0_MASK                 0xffffff00
#define BCHP_LEAP_WDG_WDOGPERIPHID2_reserved0_SHIFT                8

/* LEAP_WDG :: WDOGPERIPHID2 :: WDOGPERIPHID2 [07:00] */
#define BCHP_LEAP_WDG_WDOGPERIPHID2_WDOGPERIPHID2_MASK             0x000000ff
#define BCHP_LEAP_WDG_WDOGPERIPHID2_WDOGPERIPHID2_SHIFT            0
#define BCHP_LEAP_WDG_WDOGPERIPHID2_WDOGPERIPHID2_DEFAULT          0x00000004

/***************************************************************************
 *WDOGPERIPHID3 - Peripheral Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPERIPHID3 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPERIPHID3_reserved0_MASK                 0xffffff00
#define BCHP_LEAP_WDG_WDOGPERIPHID3_reserved0_SHIFT                8

/* LEAP_WDG :: WDOGPERIPHID3 :: WDOGPERIPHID3 [07:00] */
#define BCHP_LEAP_WDG_WDOGPERIPHID3_WDOGPERIPHID3_MASK             0x000000ff
#define BCHP_LEAP_WDG_WDOGPERIPHID3_WDOGPERIPHID3_SHIFT            0
#define BCHP_LEAP_WDG_WDOGPERIPHID3_WDOGPERIPHID3_DEFAULT          0x00000000

/***************************************************************************
 *WDOGPCELLID0 - PrimeCell Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPCELLID0 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPCELLID0_reserved0_MASK                  0xffffff00
#define BCHP_LEAP_WDG_WDOGPCELLID0_reserved0_SHIFT                 8

/* LEAP_WDG :: WDOGPCELLID0 :: WDOGPCELLID0 [07:00] */
#define BCHP_LEAP_WDG_WDOGPCELLID0_WDOGPCELLID0_MASK               0x000000ff
#define BCHP_LEAP_WDG_WDOGPCELLID0_WDOGPCELLID0_SHIFT              0
#define BCHP_LEAP_WDG_WDOGPCELLID0_WDOGPCELLID0_DEFAULT            0x0000000d

/***************************************************************************
 *WDOGPCELLID1 - PrimeCell Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPCELLID1 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPCELLID1_reserved0_MASK                  0xffffff00
#define BCHP_LEAP_WDG_WDOGPCELLID1_reserved0_SHIFT                 8

/* LEAP_WDG :: WDOGPCELLID1 :: WDOGPCELLID1 [07:00] */
#define BCHP_LEAP_WDG_WDOGPCELLID1_WDOGPCELLID1_MASK               0x000000ff
#define BCHP_LEAP_WDG_WDOGPCELLID1_WDOGPCELLID1_SHIFT              0
#define BCHP_LEAP_WDG_WDOGPCELLID1_WDOGPCELLID1_DEFAULT            0x000000f0

/***************************************************************************
 *WDOGPCELLID2 - PrimeCell Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPCELLID2 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPCELLID2_reserved0_MASK                  0xffffff00
#define BCHP_LEAP_WDG_WDOGPCELLID2_reserved0_SHIFT                 8

/* LEAP_WDG :: WDOGPCELLID2 :: WDOGPCELLID2 [07:00] */
#define BCHP_LEAP_WDG_WDOGPCELLID2_WDOGPCELLID2_MASK               0x000000ff
#define BCHP_LEAP_WDG_WDOGPCELLID2_WDOGPCELLID2_SHIFT              0
#define BCHP_LEAP_WDG_WDOGPCELLID2_WDOGPCELLID2_DEFAULT            0x00000005

/***************************************************************************
 *WDOGPCELLID3 - PrimeCell Identification Register
 ***************************************************************************/
/* LEAP_WDG :: WDOGPCELLID3 :: reserved0 [31:08] */
#define BCHP_LEAP_WDG_WDOGPCELLID3_reserved0_MASK                  0xffffff00
#define BCHP_LEAP_WDG_WDOGPCELLID3_reserved0_SHIFT                 8

/* LEAP_WDG :: WDOGPCELLID3 :: WDOGPCELLID3 [07:00] */
#define BCHP_LEAP_WDG_WDOGPCELLID3_WDOGPCELLID3_MASK               0x000000ff
#define BCHP_LEAP_WDG_WDOGPCELLID3_WDOGPCELLID3_SHIFT              0
#define BCHP_LEAP_WDG_WDOGPCELLID3_WDOGPCELLID3_DEFAULT            0x000000b1

#endif /* #ifndef BCHP_LEAP_WDG_H__ */

/* End of File */
