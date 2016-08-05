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
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Thu Jun 16 14:47:10 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1009
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_HIF_TOP_CTRL_H__
#define BCHP_HIF_TOP_CTRL_H__

/***************************************************************************
 *HIF_TOP_CTRL - HIF Top Control Registers
 ***************************************************************************/
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL          0x20201b00 /* [RW] External IRQ Active Level Control Register */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL            0x20201b04 /* [RW] SPI test port select register */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0               0x20201b0c /* [RW] HIF Power Management Control0 Register */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1               0x20201b10 /* [RW] HIF Power Management Control1 Register */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2               0x20201b14 /* [RW] HIF Power Management Control Register:used to control SDIO_0 (CARD) */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3               0x20201b18 /* [RW] HIF Power Management Control Register:used to control SDIO_1 (EMMC) */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE             0x20201b1c /* [RO] HIF Decoded Flash Type */
#define BCHP_HIF_TOP_CTRL_PDA_CTRL               0x20201b20 /* [RW] HIF PDA Control Register */
#define BCHP_HIF_TOP_CTRL_PDA_OUT_STATUS         0x20201b24 /* [RO] HIF PDA Out Status */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL          0x20201b30 /* [RW] CPU MODE Control register (PUBLIC) */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS        0x20201b34 /* [RO] CPU MODE Control status register (PUBLIC) */
#define BCHP_HIF_TOP_CTRL_SCRATCH                0x20201b3c /* [RW] HIF Scratch Register */

/***************************************************************************
 *EXT_IRQ_LEVEL - External IRQ Active Level Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: reserved0 [31:04] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_MASK             0xfffffff0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_reserved0_SHIFT            4

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_3_level [03:03] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_MASK       0x00000008
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_SHIFT      3
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_3_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_2_level [02:02] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_MASK       0x00000004
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_SHIFT      2
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_2_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_1_level [01:01] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_MASK       0x00000002
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_SHIFT      1
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_1_level_HIGH       1

/* HIF_TOP_CTRL :: EXT_IRQ_LEVEL :: ext_irq_0_level [00:00] */
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_MASK       0x00000001
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_SHIFT      0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_LOW        0
#define BCHP_HIF_TOP_CTRL_EXT_IRQ_LEVEL_ext_irq_0_level_HIGH       1

/***************************************************************************
 *SPI_DBG_SEL - SPI test port select register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SPI_DBG_SEL :: reserved0 [31:03] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved0_MASK               0xfffffff8
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved0_SHIFT              3

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: DISABLE_MSPI_FLUSH [02:02] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_DISABLE_MSPI_FLUSH_MASK      0x00000004
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_DISABLE_MSPI_FLUSH_SHIFT     2
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_DISABLE_MSPI_FLUSH_DEFAULT   0x00000000

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: SPI_RBUS_TIMER_EN [01:01] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_RBUS_TIMER_EN_MASK       0x00000002
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_RBUS_TIMER_EN_SHIFT      1
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_SPI_RBUS_TIMER_EN_DEFAULT    0x00000000

/* HIF_TOP_CTRL :: SPI_DBG_SEL :: reserved1 [00:00] */
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved1_MASK               0x00000001
#define BCHP_HIF_TOP_CTRL_SPI_DBG_SEL_reserved1_SHIFT              0

/***************************************************************************
 *PM_CTRL0 - HIF Power Management Control0 Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL0 :: EBI_PM_IN_DRIVE_INACTIVE [31:30] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_MASK   0xc0000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_SHIFT  30
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_Low    1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_PM_IN_DRIVE_INACTIVE_HIGH   2

/* HIF_TOP_CTRL :: PM_CTRL0 :: NAND_DATA_PM_IN_DRIVE_INACTIVE [29:28] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_MASK 0x30000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_SHIFT 28
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL0 :: NAND_CTRL_PM_IN_DRIVE_INACTIVE [27:26] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_MASK 0x0c000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_SHIFT 26
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_NOT_USED 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL0 :: EBI_DATA_PM_OUT_CTRL [25:24] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_MASK       0x03000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_SHIFT      24
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_DATA_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL0 :: EBI_ADDR_PM_OUT_CTRL [23:22] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_MASK       0x00c00000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_SHIFT      22
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_ADDR_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL0 :: EBI_CTRL_PM_OUT_CTRL [21:20] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_MASK       0x00300000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_SHIFT      20
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_EBI_CTRL_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL0 :: NAND_DATA_PM_OUT_CTRL [19:18] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_MASK      0x000c0000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_SHIFT     18
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_DEFAULT   0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_LOW       1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_HIGH      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_DATA_PM_OUT_CTRL_Tristate  3

/* HIF_TOP_CTRL :: PM_CTRL0 :: NAND_CTRL_PM_OUT_CTRL [17:16] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_MASK      0x00030000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_SHIFT     16
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_DEFAULT   0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_LOW       1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_HIGH      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_NAND_CTRL_PM_OUT_CTRL_Tristate  3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS7 [15:14] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_MASK            0x0000c000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_SHIFT           14
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS7_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS6 [13:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_MASK            0x00003000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_SHIFT           12
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS6_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS5 [11:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_MASK            0x00000c00
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_SHIFT           10
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS5_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS4 [09:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_MASK            0x00000300
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_SHIFT           8
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS4_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS3 [07:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_MASK            0x000000c0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_SHIFT           6
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS3_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS2 [05:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_MASK            0x00000030
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_SHIFT           4
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS2_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS1 [03:02] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_MASK            0x0000000c
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_SHIFT           2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS1_Tristate        3

/* HIF_TOP_CTRL :: PM_CTRL0 :: PM_OUT_CTRL_CS0 [01:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_MASK            0x00000003
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_SHIFT           0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_DEFAULT         0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_FUNCTIONAL      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_LOW             1
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_HIGH            2
#define BCHP_HIF_TOP_CTRL_PM_CTRL0_PM_OUT_CTRL_CS0_Tristate        3

/***************************************************************************
 *PM_CTRL1 - HIF Power Management Control1 Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL1 :: reserved0 [31:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_reserved0_MASK                  0xfffff000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_reserved0_SHIFT                 12

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_PM_IN_DRIVE_INACTIVE [11:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_MASK   0x00000c00
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_SHIFT  10
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_Low    1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_PM_IN_DRIVE_INACTIVE_HIGH   2

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_WPB_PM_OUT_CTRL [09:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_MASK        0x00000300
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_SHIFT       8
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_DEFAULT     0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_FUNCTIONAL  0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_LOW         1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_HIGH        2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_WPB_PM_OUT_CTRL_Tristate    3

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_HOLDB_PM_OUT_CTRL [07:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_MASK      0x000000c0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_SHIFT     6
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_DEFAULT   0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_LOW       1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_HIGH      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_HOLDB_PM_OUT_CTRL_Tristate  3

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_MISO_PM_OUT_CTRL [05:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_MASK       0x00000030
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_SHIFT      4
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MISO_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_MOSI_PM_OUT_CTRL [03:02] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_MASK       0x0000000c
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_SHIFT      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_MOSI_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL1 :: SPI_CLK_PM_OUT_CTRL [01:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_MASK        0x00000003
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_SHIFT       0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_DEFAULT     0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_FUNCTIONAL  0
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_LOW         1
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_HIGH        2
#define BCHP_HIF_TOP_CTRL_PM_CTRL1_SPI_CLK_PM_OUT_CTRL_Tristate    3

/***************************************************************************
 *PM_CTRL2 - HIF Power Management Control Register:used to control SDIO_0 (CARD)
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL2 :: reserved0 [31:24] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_reserved0_MASK                  0xff000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_reserved0_SHIFT                 24

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE [23:22] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_MASK 0x00c00000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_SHIFT 22
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE [21:20] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_MASK 0x00300000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_SHIFT 20
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_DATA_PM_IN_DRIVE_INACTIVE [19:18] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_MASK 0x000c0000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_SHIFT 18
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_CMD_PM_IN_DRIVE_INACTIVE [17:16] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_MASK 0x00030000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_SHIFT 16
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL2 :: reserved1 [15:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_reserved1_MASK                  0x0000f000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_reserved1_SHIFT                 12

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_VOLTAGE_PM_OUT_CTRL [11:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_MASK   0x00000c00
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_SHIFT  10
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_LOW    1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_HIGH   2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_VOLTAGE_PM_OUT_CTRL_Tristate 3

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_POWER_PM_OUT_CTRL [09:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_MASK     0x00000300
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_SHIFT    8
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_DEFAULT  0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_LOW      1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_HIGH     2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_POWER_PM_OUT_CTRL_Tristate 3

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_LED_PM_OUT_CTRL [07:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_MASK       0x000000c0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_SHIFT      6
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_LED_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_DATA_PM_OUT_CTRL [05:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_MASK      0x00000030
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_SHIFT     4
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_DEFAULT   0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_LOW       1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_HIGH      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_DATA_PM_OUT_CTRL_Tristate  3

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_CMD_PM_OUT_CTRL [03:02] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_MASK       0x0000000c
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_SHIFT      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CMD_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL2 :: SDIO_CLK_PM_OUT_CTRL [01:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_MASK       0x00000003
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_SHIFT      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL2_SDIO_CLK_PM_OUT_CTRL_Tristate   3

/***************************************************************************
 *PM_CTRL3 - HIF Power Management Control Register:used to control SDIO_1 (EMMC)
 ***************************************************************************/
/* HIF_TOP_CTRL :: PM_CTRL3 :: reserved0 [31:24] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_reserved0_MASK                  0xff000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_reserved0_SHIFT                 24

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE [23:22] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_MASK 0x00c00000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_SHIFT 22
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CARD_DETECT_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE [21:20] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_MASK 0x00300000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_SHIFT 20
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_WRITE_PROTECT_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_DATA_PM_IN_DRIVE_INACTIVE [19:18] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_MASK 0x000c0000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_SHIFT 18
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_CMD_PM_IN_DRIVE_INACTIVE [17:16] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_MASK 0x00030000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_SHIFT 16
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_Low 1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_IN_DRIVE_INACTIVE_HIGH 2

/* HIF_TOP_CTRL :: PM_CTRL3 :: reserved1 [15:12] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_reserved1_MASK                  0x0000f000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_reserved1_SHIFT                 12

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_VOLTAGE_PM_OUT_CTRL [11:10] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_MASK   0x00000c00
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_SHIFT  10
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_DEFAULT 0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_LOW    1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_HIGH   2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_VOLTAGE_PM_OUT_CTRL_Tristate 3

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_POWER_PM_OUT_CTRL [09:08] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_MASK     0x00000300
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_SHIFT    8
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_DEFAULT  0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_LOW      1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_HIGH     2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_POWER_PM_OUT_CTRL_Tristate 3

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_LED_PM_OUT_CTRL [07:06] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_MASK       0x000000c0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_SHIFT      6
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_LED_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_DATA_PM_OUT_CTRL [05:04] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_MASK      0x00000030
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_SHIFT     4
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_DEFAULT   0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_LOW       1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_HIGH      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_DATA_PM_OUT_CTRL_Tristate  3

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_CMD_PM_OUT_CTRL [03:02] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_MASK       0x0000000c
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_SHIFT      2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CMD_PM_OUT_CTRL_Tristate   3

/* HIF_TOP_CTRL :: PM_CTRL3 :: SDIO_CLK_PM_OUT_CTRL [01:00] */
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_MASK       0x00000003
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_SHIFT      0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_DEFAULT    0x00000000
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_FUNCTIONAL 0
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_LOW        1
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_HIGH       2
#define BCHP_HIF_TOP_CTRL_PM_CTRL3_SDIO_CLK_PM_OUT_CTRL_Tristate   3

/***************************************************************************
 *FLASH_TYPE - HIF Decoded Flash Type
 ***************************************************************************/
/* HIF_TOP_CTRL :: FLASH_TYPE :: reserved0 [31:03] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_MASK                0xfffffff8
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_reserved0_SHIFT               3

/* HIF_TOP_CTRL :: FLASH_TYPE :: InvalidStrap [02:02] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_MASK             0x00000004
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_SHIFT            2
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_InvalidStrap_DEFAULT          0x00000000

/* HIF_TOP_CTRL :: FLASH_TYPE :: FLASH_TYPE [01:00] */
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_MASK               0x00000003
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_SHIFT              0
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_ParallelNOR        0
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_Nand               1
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_SerialNOR          2
#define BCHP_HIF_TOP_CTRL_FLASH_TYPE_FLASH_TYPE_eMMC               3

/***************************************************************************
 *PDA_CTRL - HIF PDA Control Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: PDA_CTRL :: reserved0 [31:01] */
#define BCHP_HIF_TOP_CTRL_PDA_CTRL_reserved0_MASK                  0xfffffffe
#define BCHP_HIF_TOP_CTRL_PDA_CTRL_reserved0_SHIFT                 1

/* HIF_TOP_CTRL :: PDA_CTRL :: BOOTSRAM_PDA_IN [00:00] */
#define BCHP_HIF_TOP_CTRL_PDA_CTRL_BOOTSRAM_PDA_IN_MASK            0x00000001
#define BCHP_HIF_TOP_CTRL_PDA_CTRL_BOOTSRAM_PDA_IN_SHIFT           0
#define BCHP_HIF_TOP_CTRL_PDA_CTRL_BOOTSRAM_PDA_IN_DEFAULT         0x00000000

/***************************************************************************
 *PDA_OUT_STATUS - HIF PDA Out Status
 ***************************************************************************/
/* HIF_TOP_CTRL :: PDA_OUT_STATUS :: reserved0 [31:01] */
#define BCHP_HIF_TOP_CTRL_PDA_OUT_STATUS_reserved0_MASK            0xfffffffe
#define BCHP_HIF_TOP_CTRL_PDA_OUT_STATUS_reserved0_SHIFT           1

/* HIF_TOP_CTRL :: PDA_OUT_STATUS :: BOOTSRAM_PDA_OUT [00:00] */
#define BCHP_HIF_TOP_CTRL_PDA_OUT_STATUS_BOOTSRAM_PDA_OUT_MASK     0x00000001
#define BCHP_HIF_TOP_CTRL_PDA_OUT_STATUS_BOOTSRAM_PDA_OUT_SHIFT    0

/***************************************************************************
 *CPU_MODE_CTRL - CPU MODE Control register (PUBLIC)
 ***************************************************************************/
/* HIF_TOP_CTRL :: CPU_MODE_CTRL :: reserved0 [31:02] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_reserved0_MASK             0xfffffffc
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_reserved0_SHIFT            2

/* HIF_TOP_CTRL :: CPU_MODE_CTRL :: CPU_MODE [01:00] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_MASK              0x00000003
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_SHIFT             0
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_DEFAULT           0x00000000
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_QuadSMP           0
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_Sandbox_3_1       1
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_Sandbox_2_2       2
#define BCHP_HIF_TOP_CTRL_CPU_MODE_CTRL_CPU_MODE_Sandbox_1_3       3

/***************************************************************************
 *CPU_MODE_STATUS - CPU MODE Control status register (PUBLIC)
 ***************************************************************************/
/* HIF_TOP_CTRL :: CPU_MODE_STATUS :: reserved0 [31:08] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_reserved0_MASK           0xffffff00
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_reserved0_SHIFT          8

/* HIF_TOP_CTRL :: CPU_MODE_STATUS :: SPARE_STATUS [07:05] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_SPARE_STATUS_MASK        0x000000e0
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_SPARE_STATUS_SHIFT       5

/* HIF_TOP_CTRL :: CPU_MODE_STATUS :: CPU_MODE_STATUS [04:03] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_CPU_MODE_STATUS_MASK     0x00000018
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_CPU_MODE_STATUS_SHIFT    3

/* HIF_TOP_CTRL :: CPU_MODE_STATUS :: CPU_MODE_OTP [02:00] */
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_CPU_MODE_OTP_MASK        0x00000007
#define BCHP_HIF_TOP_CTRL_CPU_MODE_STATUS_CPU_MODE_OTP_SHIFT       0

/***************************************************************************
 *SCRATCH - HIF Scratch Register
 ***************************************************************************/
/* HIF_TOP_CTRL :: SCRATCH :: SCRATCH_BITS [31:00] */
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_MASK                0xffffffff
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_SHIFT               0
#define BCHP_HIF_TOP_CTRL_SCRATCH_SCRATCH_BITS_DEFAULT             0x00000000

#endif /* #ifndef BCHP_HIF_TOP_CTRL_H__ */

/* End of File */
