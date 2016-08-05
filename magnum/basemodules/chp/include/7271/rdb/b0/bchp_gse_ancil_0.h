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
 * Date:           Generated on               Thu Jun 16 14:47:12 2016
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

#ifndef BCHP_GSE_ANCIL_0_H__
#define BCHP_GSE_ANCIL_0_H__

/***************************************************************************
 *GSE_ANCIL_0 - GSE_ANCIL_0 registers
 ***************************************************************************/
#define BCHP_GSE_ANCIL_0_REVID                   0x206ead00 /* [RO] GSTAR Encoder Revision ID Register */
#define BCHP_GSE_ANCIL_0_CONTROL                 0x206ead08 /* [RW] Control Register */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP         0x206ead0c /* [RW] Top Field Active Line and Pedestal Enable Register */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT         0x206ead10 /* [RW] Bottom Field Active Line and Pedestal Enable Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK0        0x206ead24 /* [RW] Bank 0 Line 1 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK0        0x206ead28 /* [RW] Bank 0 Line 2 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK0        0x206ead2c /* [RW] Bank 0 Line 3 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK0        0x206ead30 /* [RW] Bank 0 Line 4 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK0        0x206ead34 /* [RW] Bank 0 Line 5 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK1        0x206ead38 /* [RW] Bank 1 Line 1 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK1        0x206ead3c /* [RW] Bank 1 Line 2 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK1        0x206ead40 /* [RW] Bank 1 Line 3 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK1        0x206ead44 /* [RW] Bank 1 Line 4 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK1        0x206ead48 /* [RW] Bank 1 Line 5 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK2        0x206ead4c /* [RW] Bank 2 Line 1 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK2        0x206ead50 /* [RW] Bank 2 Line 2 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK2        0x206ead54 /* [RW] Bank 2 Line 3 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK2        0x206ead58 /* [RW] Bank 2 Line 4 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK2        0x206ead5c /* [RW] Bank 2 Line 5 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK3        0x206ead60 /* [RW] Bank 3 Line 1 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK3        0x206ead64 /* [RW] Bank 3 Line 2 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK3        0x206ead68 /* [RW] Bank 3 Line 3 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK3        0x206ead6c /* [RW] Bank 3 Line 4 Data Register */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK3        0x206ead70 /* [RW] Bank 3 Line 5 Data Register */
#define BCHP_GSE_ANCIL_0_NULL                    0x206ead74 /* [RW] NULL Register */
#define BCHP_GSE_ANCIL_0_WRPTR                   0x206ead78 /* [RW] Write Pointer Register */
#define BCHP_GSE_ANCIL_0_RDPTR                   0x206ead7c /* [RW] Read Pointer Register */
#define BCHP_GSE_ANCIL_0_STATUS                  0x206ead80 /* [RW] Status Register */

/***************************************************************************
 *REVID - GSTAR Encoder Revision ID Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: REVID :: reserved0 [31:16] */
#define BCHP_GSE_ANCIL_0_REVID_reserved0_MASK                      0xffff0000
#define BCHP_GSE_ANCIL_0_REVID_reserved0_SHIFT                     16

/* GSE_ANCIL_0 :: REVID :: REVID [15:00] */
#define BCHP_GSE_ANCIL_0_REVID_REVID_MASK                          0x0000ffff
#define BCHP_GSE_ANCIL_0_REVID_REVID_SHIFT                         0
#define BCHP_GSE_ANCIL_0_REVID_REVID_DEFAULT                       0x00004000

/***************************************************************************
 *CONTROL - Control Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: CONTROL :: reserved_for_eco0 [31:27] */
#define BCHP_GSE_ANCIL_0_CONTROL_reserved_for_eco0_MASK            0xf8000000
#define BCHP_GSE_ANCIL_0_CONTROL_reserved_for_eco0_SHIFT           27
#define BCHP_GSE_ANCIL_0_CONTROL_reserved_for_eco0_DEFAULT         0x00000000

/* GSE_ANCIL_0 :: CONTROL :: FIFO_FREEZE [26:26] */
#define BCHP_GSE_ANCIL_0_CONTROL_FIFO_FREEZE_MASK                  0x04000000
#define BCHP_GSE_ANCIL_0_CONTROL_FIFO_FREEZE_SHIFT                 26
#define BCHP_GSE_ANCIL_0_CONTROL_FIFO_FREEZE_DEFAULT               0x00000000

/* GSE_ANCIL_0 :: CONTROL :: NULL_MODE [25:25] */
#define BCHP_GSE_ANCIL_0_CONTROL_NULL_MODE_MASK                    0x02000000
#define BCHP_GSE_ANCIL_0_CONTROL_NULL_MODE_SHIFT                   25
#define BCHP_GSE_ANCIL_0_CONTROL_NULL_MODE_DEFAULT                 0x00000000

/* GSE_ANCIL_0 :: CONTROL :: BIT_WIDTH [24:16] */
#define BCHP_GSE_ANCIL_0_CONTROL_BIT_WIDTH_MASK                    0x01ff0000
#define BCHP_GSE_ANCIL_0_CONTROL_BIT_WIDTH_SHIFT                   16
#define BCHP_GSE_ANCIL_0_CONTROL_BIT_WIDTH_DEFAULT                 0x000001ad
#define BCHP_GSE_ANCIL_0_CONTROL_BIT_WIDTH_NTSC                    429
#define BCHP_GSE_ANCIL_0_CONTROL_BIT_WIDTH_PAL                     432

/* GSE_ANCIL_0 :: CONTROL :: reserved1 [15:07] */
#define BCHP_GSE_ANCIL_0_CONTROL_reserved1_MASK                    0x0000ff80
#define BCHP_GSE_ANCIL_0_CONTROL_reserved1_SHIFT                   7

/* GSE_ANCIL_0 :: CONTROL :: PARITY_TYPE [06:06] */
#define BCHP_GSE_ANCIL_0_CONTROL_PARITY_TYPE_MASK                  0x00000040
#define BCHP_GSE_ANCIL_0_CONTROL_PARITY_TYPE_SHIFT                 6
#define BCHP_GSE_ANCIL_0_CONTROL_PARITY_TYPE_DEFAULT               0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_PARITY_TYPE_EVEN                  0
#define BCHP_GSE_ANCIL_0_CONTROL_PARITY_TYPE_ODD                   1

/* GSE_ANCIL_0 :: CONTROL :: TOP_FLD_PARITY [05:05] */
#define BCHP_GSE_ANCIL_0_CONTROL_TOP_FLD_PARITY_MASK               0x00000020
#define BCHP_GSE_ANCIL_0_CONTROL_TOP_FLD_PARITY_SHIFT              5
#define BCHP_GSE_ANCIL_0_CONTROL_TOP_FLD_PARITY_DEFAULT            0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_TOP_FLD_PARITY_MANUAL             0
#define BCHP_GSE_ANCIL_0_CONTROL_TOP_FLD_PARITY_AUTOMATIC          1

/* GSE_ANCIL_0 :: CONTROL :: BOT_FLD_PARITY [04:04] */
#define BCHP_GSE_ANCIL_0_CONTROL_BOT_FLD_PARITY_MASK               0x00000010
#define BCHP_GSE_ANCIL_0_CONTROL_BOT_FLD_PARITY_SHIFT              4
#define BCHP_GSE_ANCIL_0_CONTROL_BOT_FLD_PARITY_DEFAULT            0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_BOT_FLD_PARITY_MANUAL             0
#define BCHP_GSE_ANCIL_0_CONTROL_BOT_FLD_PARITY_AUTOMATIC          1

/* GSE_ANCIL_0 :: CONTROL :: BYTE_SWAP_656_ANCIL [03:03] */
#define BCHP_GSE_ANCIL_0_CONTROL_BYTE_SWAP_656_ANCIL_MASK          0x00000008
#define BCHP_GSE_ANCIL_0_CONTROL_BYTE_SWAP_656_ANCIL_SHIFT         3
#define BCHP_GSE_ANCIL_0_CONTROL_BYTE_SWAP_656_ANCIL_DEFAULT       0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_BYTE_SWAP_656_ANCIL_LITTLE_ENDIAN 0
#define BCHP_GSE_ANCIL_0_CONTROL_BYTE_SWAP_656_ANCIL_BIG_ENDIAN    1

/* GSE_ANCIL_0 :: CONTROL :: reserved2 [02:02] */
#define BCHP_GSE_ANCIL_0_CONTROL_reserved2_MASK                    0x00000004
#define BCHP_GSE_ANCIL_0_CONTROL_reserved2_SHIFT                   2

/* GSE_ANCIL_0 :: CONTROL :: SHIFT_DIRECTION [01:01] */
#define BCHP_GSE_ANCIL_0_CONTROL_SHIFT_DIRECTION_MASK              0x00000002
#define BCHP_GSE_ANCIL_0_CONTROL_SHIFT_DIRECTION_SHIFT             1
#define BCHP_GSE_ANCIL_0_CONTROL_SHIFT_DIRECTION_DEFAULT           0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_SHIFT_DIRECTION_LSB2MSB           0
#define BCHP_GSE_ANCIL_0_CONTROL_SHIFT_DIRECTION_MSB2LSB           1

/* GSE_ANCIL_0 :: CONTROL :: ENABLE [00:00] */
#define BCHP_GSE_ANCIL_0_CONTROL_ENABLE_MASK                       0x00000001
#define BCHP_GSE_ANCIL_0_CONTROL_ENABLE_SHIFT                      0
#define BCHP_GSE_ANCIL_0_CONTROL_ENABLE_DEFAULT                    0x00000000
#define BCHP_GSE_ANCIL_0_CONTROL_ENABLE_DISABLED                   0
#define BCHP_GSE_ANCIL_0_CONTROL_ENABLE_ENABLED                    1

/***************************************************************************
 *ACTIVE_LINE_TOP - Top Field Active Line and Pedestal Enable Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: ACTIVE_LINE [31:16] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_ACTIVE_LINE_MASK          0xffff0000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_ACTIVE_LINE_SHIFT         16
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_ACTIVE_LINE_DEFAULT       0x00000000

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: reserved0 [15:13] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_reserved0_MASK            0x0000e000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_reserved0_SHIFT           13

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: PED_LINE5 [12:12] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE5_MASK            0x00001000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE5_SHIFT           12
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE5_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE5_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE5_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: PED_LINE4 [11:11] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE4_MASK            0x00000800
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE4_SHIFT           11
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE4_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE4_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE4_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: PED_LINE3 [10:10] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE3_MASK            0x00000400
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE3_SHIFT           10
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE3_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE3_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE3_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: PED_LINE2 [09:09] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE2_MASK            0x00000200
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE2_SHIFT           9
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE2_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE2_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE2_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: PED_LINE1 [08:08] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE1_MASK            0x00000100
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE1_SHIFT           8
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE1_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE1_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_PED_LINE1_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: reserved1 [07:07] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_reserved1_MASK            0x00000080
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_reserved1_SHIFT           7

/* GSE_ANCIL_0 :: ACTIVE_LINE_TOP :: BASE [06:00] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_BASE_MASK                 0x0000007f
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_BASE_SHIFT                0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_TOP_BASE_DEFAULT              0x0000000a

/***************************************************************************
 *ACTIVE_LINE_BOT - Bottom Field Active Line and Pedestal Enable Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: ACTIVE_LINE [31:16] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_ACTIVE_LINE_MASK          0xffff0000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_ACTIVE_LINE_SHIFT         16
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_ACTIVE_LINE_DEFAULT       0x00000000

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: reserved0 [15:13] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_reserved0_MASK            0x0000e000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_reserved0_SHIFT           13

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: PED_LINE5 [12:12] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE5_MASK            0x00001000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE5_SHIFT           12
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE5_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE5_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE5_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: PED_LINE4 [11:11] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE4_MASK            0x00000800
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE4_SHIFT           11
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE4_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE4_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE4_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: PED_LINE3 [10:10] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE3_MASK            0x00000400
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE3_SHIFT           10
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE3_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE3_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE3_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: PED_LINE2 [09:09] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE2_MASK            0x00000200
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE2_SHIFT           9
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE2_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE2_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE2_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: PED_LINE1 [08:08] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE1_MASK            0x00000100
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE1_SHIFT           8
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE1_DEFAULT         0x00000000
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE1_DISABLE         0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_PED_LINE1_ENABLE          1

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: reserved1 [07:07] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_reserved1_MASK            0x00000080
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_reserved1_SHIFT           7

/* GSE_ANCIL_0 :: ACTIVE_LINE_BOT :: BASE [06:00] */
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_BASE_MASK                 0x0000007f
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_BASE_SHIFT                0
#define BCHP_GSE_ANCIL_0_ACTIVE_LINE_BOT_BASE_DEFAULT              0x0000000a

/***************************************************************************
 *DATA_LINE1_BANK0 - Bank 0 Line 1 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE1_BANK0 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK0_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK0_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK0_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE2_BANK0 - Bank 0 Line 2 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE2_BANK0 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK0_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK0_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK0_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE3_BANK0 - Bank 0 Line 3 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE3_BANK0 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK0_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK0_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK0_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE4_BANK0 - Bank 0 Line 4 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE4_BANK0 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK0_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK0_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK0_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE5_BANK0 - Bank 0 Line 5 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE5_BANK0 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK0_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK0_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK0_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE1_BANK1 - Bank 1 Line 1 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE1_BANK1 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK1_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK1_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK1_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE2_BANK1 - Bank 1 Line 2 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE2_BANK1 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK1_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK1_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK1_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE3_BANK1 - Bank 1 Line 3 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE3_BANK1 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK1_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK1_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK1_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE4_BANK1 - Bank 1 Line 4 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE4_BANK1 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK1_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK1_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK1_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE5_BANK1 - Bank 1 Line 5 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE5_BANK1 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK1_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK1_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK1_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE1_BANK2 - Bank 2 Line 1 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE1_BANK2 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK2_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK2_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK2_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE2_BANK2 - Bank 2 Line 2 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE2_BANK2 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK2_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK2_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK2_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE3_BANK2 - Bank 2 Line 3 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE3_BANK2 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK2_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK2_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK2_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE4_BANK2 - Bank 2 Line 4 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE4_BANK2 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK2_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK2_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK2_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE5_BANK2 - Bank 2 Line 5 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE5_BANK2 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK2_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK2_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK2_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE1_BANK3 - Bank 3 Line 1 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE1_BANK3 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK3_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK3_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE1_BANK3_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE2_BANK3 - Bank 3 Line 2 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE2_BANK3 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK3_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK3_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE2_BANK3_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE3_BANK3 - Bank 3 Line 3 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE3_BANK3 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK3_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK3_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE3_BANK3_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE4_BANK3 - Bank 3 Line 4 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE4_BANK3 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK3_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK3_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE4_BANK3_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *DATA_LINE5_BANK3 - Bank 3 Line 5 Data Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: DATA_LINE5_BANK3 :: VALUE [31:00] */
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK3_VALUE_MASK               0xffffffff
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK3_VALUE_SHIFT              0
#define BCHP_GSE_ANCIL_0_DATA_LINE5_BANK3_VALUE_DEFAULT            0x00000000

/***************************************************************************
 *NULL - NULL Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: NULL :: reserved0 [31:12] */
#define BCHP_GSE_ANCIL_0_NULL_reserved0_MASK                       0xfffff000
#define BCHP_GSE_ANCIL_0_NULL_reserved0_SHIFT                      12

/* GSE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK3 [11:11] */
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK3_MASK               0x00000800
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK3_SHIFT              11
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK3_DEFAULT            0x00000000

/* GSE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK2 [10:10] */
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK2_MASK               0x00000400
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK2_SHIFT              10
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK2_DEFAULT            0x00000000

/* GSE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK1 [09:09] */
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK1_MASK               0x00000200
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK1_SHIFT              9
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK1_DEFAULT            0x00000000

/* GSE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK0 [08:08] */
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK0_MASK               0x00000100
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK0_SHIFT              8
#define BCHP_GSE_ANCIL_0_NULL_NULL_ENABLE_BANK0_DEFAULT            0x00000000

/* GSE_ANCIL_0 :: NULL :: CHARACTER [07:00] */
#define BCHP_GSE_ANCIL_0_NULL_CHARACTER_MASK                       0x000000ff
#define BCHP_GSE_ANCIL_0_NULL_CHARACTER_SHIFT                      0
#define BCHP_GSE_ANCIL_0_NULL_CHARACTER_DEFAULT                    0x00000000

/***************************************************************************
 *WRPTR - Write Pointer Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: WRPTR :: reserved0 [31:03] */
#define BCHP_GSE_ANCIL_0_WRPTR_reserved0_MASK                      0xfffffff8
#define BCHP_GSE_ANCIL_0_WRPTR_reserved0_SHIFT                     3

/* GSE_ANCIL_0 :: WRPTR :: VALUE [02:00] */
#define BCHP_GSE_ANCIL_0_WRPTR_VALUE_MASK                          0x00000007
#define BCHP_GSE_ANCIL_0_WRPTR_VALUE_SHIFT                         0
#define BCHP_GSE_ANCIL_0_WRPTR_VALUE_DEFAULT                       0x00000000

/***************************************************************************
 *RDPTR - Read Pointer Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: RDPTR :: reserved0 [31:03] */
#define BCHP_GSE_ANCIL_0_RDPTR_reserved0_MASK                      0xfffffff8
#define BCHP_GSE_ANCIL_0_RDPTR_reserved0_SHIFT                     3

/* GSE_ANCIL_0 :: RDPTR :: VALUE [02:00] */
#define BCHP_GSE_ANCIL_0_RDPTR_VALUE_MASK                          0x00000007
#define BCHP_GSE_ANCIL_0_RDPTR_VALUE_SHIFT                         0
#define BCHP_GSE_ANCIL_0_RDPTR_VALUE_DEFAULT                       0x00000000

/***************************************************************************
 *STATUS - Status Register
 ***************************************************************************/
/* GSE_ANCIL_0 :: STATUS :: reserved0 [31:06] */
#define BCHP_GSE_ANCIL_0_STATUS_reserved0_MASK                     0xffffffc0
#define BCHP_GSE_ANCIL_0_STATUS_reserved0_SHIFT                    6

/* GSE_ANCIL_0 :: STATUS :: FIFO_OVERFLOW [05:05] */
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_OVERFLOW_MASK                 0x00000020
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_OVERFLOW_SHIFT                5
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_OVERFLOW_DEFAULT              0x00000000

/* GSE_ANCIL_0 :: STATUS :: FIFO_UNDERFLOW [04:04] */
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_UNDERFLOW_MASK                0x00000010
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_UNDERFLOW_SHIFT               4
#define BCHP_GSE_ANCIL_0_STATUS_FIFO_UNDERFLOW_DEFAULT             0x00000000

/* GSE_ANCIL_0 :: STATUS :: BANK3_656_ANCIL_DATA [03:03] */
#define BCHP_GSE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_MASK          0x00000008
#define BCHP_GSE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_SHIFT         3
#define BCHP_GSE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_DEFAULT       0x00000000
#define BCHP_GSE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_XMIT          1
#define BCHP_GSE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_NOT_XMIT      0

/* GSE_ANCIL_0 :: STATUS :: BANK2_656_ANCIL_DATA [02:02] */
#define BCHP_GSE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_MASK          0x00000004
#define BCHP_GSE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_SHIFT         2
#define BCHP_GSE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_DEFAULT       0x00000000
#define BCHP_GSE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_XMIT          1
#define BCHP_GSE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_NOT_XMIT      0

/* GSE_ANCIL_0 :: STATUS :: BANK1_656_ANCIL_DATA [01:01] */
#define BCHP_GSE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_MASK          0x00000002
#define BCHP_GSE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_SHIFT         1
#define BCHP_GSE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_DEFAULT       0x00000000
#define BCHP_GSE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_XMIT          1
#define BCHP_GSE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_NOT_XMIT      0

/* GSE_ANCIL_0 :: STATUS :: BANK0_656_ANCIL_DATA [00:00] */
#define BCHP_GSE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_MASK          0x00000001
#define BCHP_GSE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_SHIFT         0
#define BCHP_GSE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_DEFAULT       0x00000000
#define BCHP_GSE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_XMIT          1
#define BCHP_GSE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_NOT_XMIT      0

#endif /* #ifndef BCHP_GSE_ANCIL_0_H__ */

/* End of File */
