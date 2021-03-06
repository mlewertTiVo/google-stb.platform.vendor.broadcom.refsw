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
 * Date:           Generated on               Wed Feb 11 10:13:59 2015
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

#ifndef BCHP_MPM_WDOG_H__
#define BCHP_MPM_WDOG_H__

/***************************************************************************
 *MPM_WDOG - Watch Dog Registers
 ***************************************************************************/
#define BCHP_MPM_WDOG_WDOGLOAD                   0x01581000 /* [RW] Watchdog Load */
#define BCHP_MPM_WDOG_WDOGVALUE                  0x01581004 /* [RO] Watchdog Value */
#define BCHP_MPM_WDOG_WDOGCONTROL                0x01581008 /* [RW] Watchdog Control */
#define BCHP_MPM_WDOG_WDOGINTCLR                 0x0158100c /* [WO] Watchdog Clear Interrupt */
#define BCHP_MPM_WDOG_WDOGRIS                    0x01581010 /* [RO] Watchdog Raw Interrupt Status */
#define BCHP_MPM_WDOG_WDOGMIS                    0x01581014 /* [RO] Watchdog Interrupt Status */
#define BCHP_MPM_WDOG_WDOGLOCK                   0x01581c00 /* [RW] Watchdog Lock */
#define BCHP_MPM_WDOG_WDOGITCR                   0x01581f00 /* [RW] Watchdog Integration Test Control */
#define BCHP_MPM_WDOG_WDOGITOP                   0x01581f04 /* [WO] Watchdog Integration Test Output Set */
#define BCHP_MPM_WDOG_WDOGPERIPHID0              0x01581fe0 /* [RO] Peripheral Identification Register */
#define BCHP_MPM_WDOG_WDOGPERIPHID1              0x01581fe4 /* [RO] Peripheral Identification Register */
#define BCHP_MPM_WDOG_WDOGPERIPHID2              0x01581fe8 /* [RO] Peripheral Identification Register */
#define BCHP_MPM_WDOG_WDOGPERIPHID3              0x01581fec /* [RO] Peripheral Identification Register */
#define BCHP_MPM_WDOG_WDOGPCELLID0               0x01581ff0 /* [RO] PrimeCell Identification Register */
#define BCHP_MPM_WDOG_WDOGPCELLID1               0x01581ff4 /* [RO] PrimeCell Identification Register */
#define BCHP_MPM_WDOG_WDOGPCELLID2               0x01581ff8 /* [RO] PrimeCell Identification Register */
#define BCHP_MPM_WDOG_WDOGPCELLID3               0x01581ffc /* [RO] PrimeCell Identification Register */

/***************************************************************************
 *WDOGLOAD - Watchdog Load
 ***************************************************************************/
/* MPM_WDOG :: WDOGLOAD :: WDOGLOAD [31:00] */
#define BCHP_MPM_WDOG_WDOGLOAD_WDOGLOAD_MASK                       0xffffffff
#define BCHP_MPM_WDOG_WDOGLOAD_WDOGLOAD_SHIFT                      0
#define BCHP_MPM_WDOG_WDOGLOAD_WDOGLOAD_DEFAULT                    0xffffffff

/***************************************************************************
 *WDOGVALUE - Watchdog Value
 ***************************************************************************/
/* MPM_WDOG :: WDOGVALUE :: WDOGVALUE [31:00] */
#define BCHP_MPM_WDOG_WDOGVALUE_WDOGVALUE_MASK                     0xffffffff
#define BCHP_MPM_WDOG_WDOGVALUE_WDOGVALUE_SHIFT                    0
#define BCHP_MPM_WDOG_WDOGVALUE_WDOGVALUE_DEFAULT                  0xffffffff

/***************************************************************************
 *WDOGCONTROL - Watchdog Control
 ***************************************************************************/
/* MPM_WDOG :: WDOGCONTROL :: reserved0 [31:02] */
#define BCHP_MPM_WDOG_WDOGCONTROL_reserved0_MASK                   0xfffffffc
#define BCHP_MPM_WDOG_WDOGCONTROL_reserved0_SHIFT                  2

/* MPM_WDOG :: WDOGCONTROL :: WDOGCONTROL [01:00] */
#define BCHP_MPM_WDOG_WDOGCONTROL_WDOGCONTROL_MASK                 0x00000003
#define BCHP_MPM_WDOG_WDOGCONTROL_WDOGCONTROL_SHIFT                0
#define BCHP_MPM_WDOG_WDOGCONTROL_WDOGCONTROL_DEFAULT              0x00000000

/***************************************************************************
 *WDOGINTCLR - Watchdog Clear Interrupt
 ***************************************************************************/
/* MPM_WDOG :: WDOGINTCLR :: reserved0 [31:02] */
#define BCHP_MPM_WDOG_WDOGINTCLR_reserved0_MASK                    0xfffffffc
#define BCHP_MPM_WDOG_WDOGINTCLR_reserved0_SHIFT                   2

/* MPM_WDOG :: WDOGINTCLR :: WDOGINTCLR [01:00] */
#define BCHP_MPM_WDOG_WDOGINTCLR_WDOGINTCLR_MASK                   0x00000003
#define BCHP_MPM_WDOG_WDOGINTCLR_WDOGINTCLR_SHIFT                  0

/***************************************************************************
 *WDOGRIS - Watchdog Raw Interrupt Status
 ***************************************************************************/
/* MPM_WDOG :: WDOGRIS :: reserved0 [31:01] */
#define BCHP_MPM_WDOG_WDOGRIS_reserved0_MASK                       0xfffffffe
#define BCHP_MPM_WDOG_WDOGRIS_reserved0_SHIFT                      1

/* MPM_WDOG :: WDOGRIS :: WDOGRIS [00:00] */
#define BCHP_MPM_WDOG_WDOGRIS_WDOGRIS_MASK                         0x00000001
#define BCHP_MPM_WDOG_WDOGRIS_WDOGRIS_SHIFT                        0
#define BCHP_MPM_WDOG_WDOGRIS_WDOGRIS_DEFAULT                      0x00000000

/***************************************************************************
 *WDOGMIS - Watchdog Interrupt Status
 ***************************************************************************/
/* MPM_WDOG :: WDOGMIS :: reserved0 [31:01] */
#define BCHP_MPM_WDOG_WDOGMIS_reserved0_MASK                       0xfffffffe
#define BCHP_MPM_WDOG_WDOGMIS_reserved0_SHIFT                      1

/* MPM_WDOG :: WDOGMIS :: WDOGMIS [00:00] */
#define BCHP_MPM_WDOG_WDOGMIS_WDOGMIS_MASK                         0x00000001
#define BCHP_MPM_WDOG_WDOGMIS_WDOGMIS_SHIFT                        0
#define BCHP_MPM_WDOG_WDOGMIS_WDOGMIS_DEFAULT                      0x00000000

/***************************************************************************
 *WDOGLOCK - Watchdog Lock
 ***************************************************************************/
/* MPM_WDOG :: WDOGLOCK :: WDOGLOCK [31:00] */
#define BCHP_MPM_WDOG_WDOGLOCK_WDOGLOCK_MASK                       0xffffffff
#define BCHP_MPM_WDOG_WDOGLOCK_WDOGLOCK_SHIFT                      0
#define BCHP_MPM_WDOG_WDOGLOCK_WDOGLOCK_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGITCR - Watchdog Integration Test Control
 ***************************************************************************/
/* MPM_WDOG :: WDOGITCR :: reserved0 [31:01] */
#define BCHP_MPM_WDOG_WDOGITCR_reserved0_MASK                      0xfffffffe
#define BCHP_MPM_WDOG_WDOGITCR_reserved0_SHIFT                     1

/* MPM_WDOG :: WDOGITCR :: WDOGITCR [00:00] */
#define BCHP_MPM_WDOG_WDOGITCR_WDOGITCR_MASK                       0x00000001
#define BCHP_MPM_WDOG_WDOGITCR_WDOGITCR_SHIFT                      0
#define BCHP_MPM_WDOG_WDOGITCR_WDOGITCR_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGITOP - Watchdog Integration Test Output Set
 ***************************************************************************/
/* MPM_WDOG :: WDOGITOP :: reserved0 [31:02] */
#define BCHP_MPM_WDOG_WDOGITOP_reserved0_MASK                      0xfffffffc
#define BCHP_MPM_WDOG_WDOGITOP_reserved0_SHIFT                     2

/* MPM_WDOG :: WDOGITOP :: WDOGITOP [01:00] */
#define BCHP_MPM_WDOG_WDOGITOP_WDOGITOP_MASK                       0x00000003
#define BCHP_MPM_WDOG_WDOGITOP_WDOGITOP_SHIFT                      0
#define BCHP_MPM_WDOG_WDOGITOP_WDOGITOP_DEFAULT                    0x00000000

/***************************************************************************
 *WDOGPERIPHID0 - Peripheral Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPERIPHID0 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPERIPHID0_reserved0_MASK                 0xffffff00
#define BCHP_MPM_WDOG_WDOGPERIPHID0_reserved0_SHIFT                8

/* MPM_WDOG :: WDOGPERIPHID0 :: WDOGPERIPHID0 [07:00] */
#define BCHP_MPM_WDOG_WDOGPERIPHID0_WDOGPERIPHID0_MASK             0x000000ff
#define BCHP_MPM_WDOG_WDOGPERIPHID0_WDOGPERIPHID0_SHIFT            0
#define BCHP_MPM_WDOG_WDOGPERIPHID0_WDOGPERIPHID0_DEFAULT          0x00000005

/***************************************************************************
 *WDOGPERIPHID1 - Peripheral Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPERIPHID1 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPERIPHID1_reserved0_MASK                 0xffffff00
#define BCHP_MPM_WDOG_WDOGPERIPHID1_reserved0_SHIFT                8

/* MPM_WDOG :: WDOGPERIPHID1 :: WDOGPERIPHID1 [07:00] */
#define BCHP_MPM_WDOG_WDOGPERIPHID1_WDOGPERIPHID1_MASK             0x000000ff
#define BCHP_MPM_WDOG_WDOGPERIPHID1_WDOGPERIPHID1_SHIFT            0
#define BCHP_MPM_WDOG_WDOGPERIPHID1_WDOGPERIPHID1_DEFAULT          0x00000018

/***************************************************************************
 *WDOGPERIPHID2 - Peripheral Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPERIPHID2 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPERIPHID2_reserved0_MASK                 0xffffff00
#define BCHP_MPM_WDOG_WDOGPERIPHID2_reserved0_SHIFT                8

/* MPM_WDOG :: WDOGPERIPHID2 :: WDOGPERIPHID2 [07:00] */
#define BCHP_MPM_WDOG_WDOGPERIPHID2_WDOGPERIPHID2_MASK             0x000000ff
#define BCHP_MPM_WDOG_WDOGPERIPHID2_WDOGPERIPHID2_SHIFT            0
#define BCHP_MPM_WDOG_WDOGPERIPHID2_WDOGPERIPHID2_DEFAULT          0x00000014

/***************************************************************************
 *WDOGPERIPHID3 - Peripheral Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPERIPHID3 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPERIPHID3_reserved0_MASK                 0xffffff00
#define BCHP_MPM_WDOG_WDOGPERIPHID3_reserved0_SHIFT                8

/* MPM_WDOG :: WDOGPERIPHID3 :: WDOGPERIPHID3 [07:00] */
#define BCHP_MPM_WDOG_WDOGPERIPHID3_WDOGPERIPHID3_MASK             0x000000ff
#define BCHP_MPM_WDOG_WDOGPERIPHID3_WDOGPERIPHID3_SHIFT            0
#define BCHP_MPM_WDOG_WDOGPERIPHID3_WDOGPERIPHID3_DEFAULT          0x00000000

/***************************************************************************
 *WDOGPCELLID0 - PrimeCell Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPCELLID0 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPCELLID0_reserved0_MASK                  0xffffff00
#define BCHP_MPM_WDOG_WDOGPCELLID0_reserved0_SHIFT                 8

/* MPM_WDOG :: WDOGPCELLID0 :: WDOGPCELLID0 [07:00] */
#define BCHP_MPM_WDOG_WDOGPCELLID0_WDOGPCELLID0_MASK               0x000000ff
#define BCHP_MPM_WDOG_WDOGPCELLID0_WDOGPCELLID0_SHIFT              0
#define BCHP_MPM_WDOG_WDOGPCELLID0_WDOGPCELLID0_DEFAULT            0x0000000d

/***************************************************************************
 *WDOGPCELLID1 - PrimeCell Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPCELLID1 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPCELLID1_reserved0_MASK                  0xffffff00
#define BCHP_MPM_WDOG_WDOGPCELLID1_reserved0_SHIFT                 8

/* MPM_WDOG :: WDOGPCELLID1 :: WDOGPCELLID1 [07:00] */
#define BCHP_MPM_WDOG_WDOGPCELLID1_WDOGPCELLID1_MASK               0x000000ff
#define BCHP_MPM_WDOG_WDOGPCELLID1_WDOGPCELLID1_SHIFT              0
#define BCHP_MPM_WDOG_WDOGPCELLID1_WDOGPCELLID1_DEFAULT            0x000000f0

/***************************************************************************
 *WDOGPCELLID2 - PrimeCell Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPCELLID2 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPCELLID2_reserved0_MASK                  0xffffff00
#define BCHP_MPM_WDOG_WDOGPCELLID2_reserved0_SHIFT                 8

/* MPM_WDOG :: WDOGPCELLID2 :: WDOGPCELLID2 [07:00] */
#define BCHP_MPM_WDOG_WDOGPCELLID2_WDOGPCELLID2_MASK               0x000000ff
#define BCHP_MPM_WDOG_WDOGPCELLID2_WDOGPCELLID2_SHIFT              0
#define BCHP_MPM_WDOG_WDOGPCELLID2_WDOGPCELLID2_DEFAULT            0x00000005

/***************************************************************************
 *WDOGPCELLID3 - PrimeCell Identification Register
 ***************************************************************************/
/* MPM_WDOG :: WDOGPCELLID3 :: reserved0 [31:08] */
#define BCHP_MPM_WDOG_WDOGPCELLID3_reserved0_MASK                  0xffffff00
#define BCHP_MPM_WDOG_WDOGPCELLID3_reserved0_SHIFT                 8

/* MPM_WDOG :: WDOGPCELLID3 :: WDOGPCELLID3 [07:00] */
#define BCHP_MPM_WDOG_WDOGPCELLID3_WDOGPCELLID3_MASK               0x000000ff
#define BCHP_MPM_WDOG_WDOGPCELLID3_WDOGPCELLID3_SHIFT              0
#define BCHP_MPM_WDOG_WDOGPCELLID3_WDOGPCELLID3_DEFAULT            0x000000b1

#endif /* #ifndef BCHP_MPM_WDOG_H__ */

/* End of File */
