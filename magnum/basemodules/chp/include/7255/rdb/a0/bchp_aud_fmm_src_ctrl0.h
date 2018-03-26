/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2017 Broadcom. All rights reserved.
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
 * Date:           Generated on               Tue Mar 14 11:30:18 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1255
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#ifndef BCHP_AUD_FMM_SRC_CTRL0_H__
#define BCHP_AUD_FMM_SRC_CTRL0_H__

/***************************************************************************
 *AUD_FMM_SRC_CTRL0 - Audio SRC Control
 ***************************************************************************/
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP         0x20ca2000 /* [RW][32] Ramp Step Size */
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL       0x20ca2004 /* [RO][32] Read Bank Select */
#define BCHP_AUD_FMM_SRC_CTRL0_FLT_STAT          0x20ca2008 /* [RO][32] Filter Status */
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE        0x20ca200c /* [RW][32] Alternate Enable */
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG          0x20ca2010 /* [RW][32] Miscellaneous Configuration */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET        0x20ca2014 /* [RW][32] Stream Reset */
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE      0x20ca2018 /* [RO][32] Group Enable */
#define BCHP_AUD_FMM_SRC_CTRL0_DIAG              0x20ca201c /* [RO][32] Diagnostic */

/***************************************************************************
 *RAMP_STEP - Ramp Step Size
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: RAMP_STEP :: reserved0 [31:16] */
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP_reserved0_MASK            0xffff0000
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP_reserved0_SHIFT           16

/* AUD_FMM_SRC_CTRL0 :: RAMP_STEP :: STEP_SIZE [15:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP_STEP_SIZE_MASK            0x0000ffff
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP_STEP_SIZE_SHIFT           0
#define BCHP_AUD_FMM_SRC_CTRL0_RAMP_STEP_STEP_SIZE_DEFAULT         0x00000200

/***************************************************************************
 *RD_BANK_SEL - Read Bank Select
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: RD_BANK_SEL :: reserved0 [31:12] */
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL_reserved0_MASK          0xfffff000
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL_reserved0_SHIFT         12

/* AUD_FMM_SRC_CTRL0 :: RD_BANK_SEL :: ARRAY [11:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL_ARRAY_MASK              0x00000fff
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL_ARRAY_SHIFT             0
#define BCHP_AUD_FMM_SRC_CTRL0_RD_BANK_SEL_ARRAY_DEFAULT           0x00000000

/***************************************************************************
 *FLT_STAT - Filter Status
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: FLT_STAT :: reserved0 [31:24] */
#define BCHP_AUD_FMM_SRC_CTRL0_FLT_STAT_reserved0_MASK             0xff000000
#define BCHP_AUD_FMM_SRC_CTRL0_FLT_STAT_reserved0_SHIFT            24

/* AUD_FMM_SRC_CTRL0 :: FLT_STAT :: ARRAY [23:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_FLT_STAT_ARRAY_MASK                 0x00ffffff
#define BCHP_AUD_FMM_SRC_CTRL0_FLT_STAT_ARRAY_SHIFT                0

/***************************************************************************
 *ALT_ENABLE - Alternate Enable
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: ALT_ENABLE :: reserved0 [31:12] */
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE_reserved0_MASK           0xfffff000
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE_reserved0_SHIFT          12

/* AUD_FMM_SRC_CTRL0 :: ALT_ENABLE :: ARRAY [11:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE_ARRAY_MASK               0x00000fff
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE_ARRAY_SHIFT              0
#define BCHP_AUD_FMM_SRC_CTRL0_ALT_ENABLE_ARRAY_DEFAULT            0x00000000

/***************************************************************************
 *MISC_CFG - Miscellaneous Configuration
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: MISC_CFG :: reserved0 [31:02] */
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG_reserved0_MASK             0xfffffffc
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG_reserved0_SHIFT            2

/* AUD_FMM_SRC_CTRL0 :: MISC_CFG :: MISC_TEST [01:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG_MISC_TEST_MASK             0x00000003
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG_MISC_TEST_SHIFT            0
#define BCHP_AUD_FMM_SRC_CTRL0_MISC_CFG_MISC_TEST_DEFAULT          0x00000000

/***************************************************************************
 *STRM_RESET - Stream Reset
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: reserved0 [31:19] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_reserved0_MASK           0xfff80000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_reserved0_SHIFT          19

/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: ARB_STATE [18:18] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_ARB_STATE_MASK           0x00040000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_ARB_STATE_SHIFT          18
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_ARB_STATE_DEFAULT        0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_ARB_STATE_Reset          1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_ARB_STATE_No_Reset       0

/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: SP_STATE [17:17] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_SP_STATE_MASK            0x00020000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_SP_STATE_SHIFT           17
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_SP_STATE_DEFAULT         0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_SP_STATE_Reset           1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_SP_STATE_No_Reset        0

/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: FCI_STATE [16:16] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_FCI_STATE_MASK           0x00010000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_FCI_STATE_SHIFT          16
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_FCI_STATE_DEFAULT        0x00000000

/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: reserved1 [15:12] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_reserved1_MASK           0x0000f000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_reserved1_SHIFT          12

/* AUD_FMM_SRC_CTRL0 :: STRM_RESET :: PAIR_RESET [11:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_PAIR_RESET_MASK          0x00000fff
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_PAIR_RESET_SHIFT         0
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_PAIR_RESET_DEFAULT       0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_PAIR_RESET_Reset         1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_RESET_PAIR_RESET_No_Reset      0

/***************************************************************************
 *GROUP_ENABLE - Group Enable
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: GROUP_ENABLE :: reserved0 [31:12] */
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE_reserved0_MASK         0xfffff000
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE_reserved0_SHIFT        12

/* AUD_FMM_SRC_CTRL0 :: GROUP_ENABLE :: ARRAY [11:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE_ARRAY_MASK             0x00000fff
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE_ARRAY_SHIFT            0
#define BCHP_AUD_FMM_SRC_CTRL0_GROUP_ENABLE_ARRAY_DEFAULT          0x00000000

/***************************************************************************
 *DIAG - Diagnostic
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: DIAG :: DATA [31:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_DIAG_DATA_MASK                      0xffffffff
#define BCHP_AUD_FMM_SRC_CTRL0_DIAG_DATA_SHIFT                     0

/***************************************************************************
 *STRM_CFG%i - Stream Configuration
 ***************************************************************************/
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ARRAY_BASE                0x20ca2020
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ARRAY_START               0
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ARRAY_END                 11
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ARRAY_ELEMENT_SIZE        32

/***************************************************************************
 *STRM_CFG%i - Stream Configuration
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: reserved0 [31:26] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_reserved0_MASK            0xfc000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_reserved0_SHIFT           26

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: FCI_ID [25:16] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_FCI_ID_MASK               0x03ff0000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_FCI_ID_SHIFT              16
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_FCI_ID_DEFAULT            0x00000000

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: reserved1 [15:10] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_reserved1_MASK            0x0000fc00
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_reserved1_SHIFT           10

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: RAMP_ZERO_DETECT_ENA [09:09] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ZERO_DETECT_ENA_MASK 0x00000200
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ZERO_DETECT_ENA_SHIFT 9
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ZERO_DETECT_ENA_DEFAULT 0x00000001
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ZERO_DETECT_ENA_Enable 1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ZERO_DETECT_ENA_Disable 0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: ENABLE [08:08] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ENABLE_MASK               0x00000100
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ENABLE_SHIFT              8
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ENABLE_DEFAULT            0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ENABLE_Enable             1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_ENABLE_Disable            0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: PRIORITY [07:07] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_PRIORITY_MASK             0x00000080
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_PRIORITY_SHIFT            7
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_PRIORITY_DEFAULT          0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_PRIORITY_High             1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_PRIORITY_Normal           0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: RAMP_ENA [06:06] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ENA_MASK             0x00000040
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ENA_SHIFT            6
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ENA_DEFAULT          0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ENA_Enable           1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_RAMP_ENA_Disable          0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: MUTE_ENA [05:05] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_MUTE_ENA_MASK             0x00000020
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_MUTE_ENA_SHIFT            5
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_MUTE_ENA_DEFAULT          0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_MUTE_ENA_Enable           1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_MUTE_ENA_Disable          0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: STARTUP_RAMP_ENA [04:04] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_STARTUP_RAMP_ENA_MASK     0x00000010
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_STARTUP_RAMP_ENA_SHIFT    4
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_STARTUP_RAMP_ENA_DEFAULT  0x00000000
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_STARTUP_RAMP_ENA_Enable   1
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_STARTUP_RAMP_ENA_Disable  0

/* AUD_FMM_SRC_CTRL0 :: STRM_CFGi :: GROUP [03:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_GROUP_MASK                0x0000000f
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_GROUP_SHIFT               0
#define BCHP_AUD_FMM_SRC_CTRL0_STRM_CFGi_GROUP_DEFAULT             0x00000000


/***************************************************************************
 *SRC_CFG%i - SRC Configuration
 ***************************************************************************/
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_ARRAY_BASE                 0x20ca2800
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_ARRAY_START                0
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_ARRAY_END                  11
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_ARRAY_ELEMENT_SIZE         32

/***************************************************************************
 *SRC_CFG%i - SRC Configuration
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: reserved0 [31:28] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved0_MASK             0xf0000000
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved0_SHIFT            28

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: reserved_for_eco1 [27:27] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco1_MASK     0x08000000
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco1_SHIFT    27

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: TM_BASE_ADDR [26:16] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_TM_BASE_ADDR_MASK          0x07ff0000
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_TM_BASE_ADDR_SHIFT         16

/* union - case FIR [15:08] */
/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: FIR :: NUM_COEFF [15:08] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_FIR_NUM_COEFF_MASK         0x0000ff00
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_FIR_NUM_COEFF_SHIFT        8

/* union - case IIR [15:08] */
/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: IIR :: STEP_SIZE [15:12] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_STEP_SIZE_MASK         0x0000f000
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_STEP_SIZE_SHIFT        12

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: IIR :: reserved_for_eco0 [11:11] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_reserved_for_eco0_MASK 0x00000800
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_reserved_for_eco0_SHIFT 11

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: IIR :: NUM_CASCADE [10:08] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_NUM_CASCADE_MASK       0x00000700
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_IIR_NUM_CASCADE_SHIFT      8

/* union - case LI [15:08] */
/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: LI :: reserved_for_eco0 [15:08] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_LI_reserved_for_eco0_MASK  0x0000ff00
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_LI_reserved_for_eco0_SHIFT 8

/* union - case SRC [15:08] */
/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: SRC :: reserved_for_eco0 [15:08] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_reserved_for_eco0_MASK 0x0000ff00
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_reserved_for_eco0_SHIFT 8

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: reserved_for_eco2 [07:06] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco2_MASK     0x000000c0
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco2_SHIFT    6

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: WR_BANK_SEL [05:05] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_WR_BANK_SEL_MASK           0x00000020
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_WR_BANK_SEL_SHIFT          5

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: SRC_ONLY [04:04] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_ONLY_MASK              0x00000010
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_ONLY_SHIFT             4
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_ONLY_FIR               0
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_ONLY_NoFIR             1

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: reserved_for_eco3 [03:03] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco3_MASK     0x00000008
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_reserved_for_eco3_SHIFT    3

/* AUD_FMM_SRC_CTRL0 :: SRC_CFGi :: SRC_TYPE [02:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_MASK              0x00000007
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_SHIFT             0
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_Bypass            0
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_X1                1
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_Up2               2
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_Down2             3
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_Up4               4
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_Down4             5
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_L_Int             6
#define BCHP_AUD_FMM_SRC_CTRL0_SRC_CFGi_SRC_TYPE_IIR               7


/***************************************************************************
 *CF_SEL%i - SRC Coefficient Configuration
 ***************************************************************************/
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_ARRAY_BASE                  0x20ca2830
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_ARRAY_START                 0
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_ARRAY_END                   11
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_ARRAY_ELEMENT_SIZE          32

/***************************************************************************
 *CF_SEL%i - SRC Coefficient Configuration
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: CF_SELi :: reserved0 [31:28] */
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_reserved0_MASK              0xf0000000
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_reserved0_SHIFT             28

/* AUD_FMM_SRC_CTRL0 :: CF_SELi :: MI_SKEW [27:20] */
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_MI_SKEW_MASK                0x0ff00000
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_MI_SKEW_SHIFT               20

/* AUD_FMM_SRC_CTRL0 :: CF_SELi :: CF_BASE_ADDR1 [19:10] */
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_CF_BASE_ADDR1_MASK          0x000ffc00
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_CF_BASE_ADDR1_SHIFT         10

/* AUD_FMM_SRC_CTRL0 :: CF_SELi :: CF_BASE_ADDR0 [09:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_CF_BASE_ADDR0_MASK          0x000003ff
#define BCHP_AUD_FMM_SRC_CTRL0_CF_SELi_CF_BASE_ADDR0_SHIFT         0


/***************************************************************************
 *COEFF%i - Coefficients/Parameters for Programmable Filters
 ***************************************************************************/
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_ARRAY_BASE                   0x20ca2860
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_ARRAY_START                  0
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_ARRAY_END                    231
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_ARRAY_ELEMENT_SIZE           32

/***************************************************************************
 *COEFF%i - Coefficients/Parameters for Programmable Filters
 ***************************************************************************/
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: reserved0 [31:28] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_reserved0_MASK               0xf0000000
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_reserved0_SHIFT              28

/* union - case FIR [27:00] */
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: FIR :: DAT [27:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_FIR_DAT_MASK                 0x0fffffff
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_FIR_DAT_SHIFT                0

/* union - case IIR [27:00] */
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: IIR :: DAT [27:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_IIR_DAT_MASK                 0x0fffffff
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_IIR_DAT_SHIFT                0

/* union - case LI_NUM [27:00] */
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: LI_NUM :: NUMERATOR [27:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_NUM_NUMERATOR_MASK        0x0fffffff
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_NUM_NUMERATOR_SHIFT       0

/* union - case LI_DEN [27:00] */
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: LI_DEN :: DENOMINATOR [27:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_DEN_DENOMINATOR_MASK      0x0fffffff
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_DEN_DENOMINATOR_SHIFT     0

/* union - case LI_SCALE [27:00] */
/* AUD_FMM_SRC_CTRL0 :: COEFFi :: LI_SCALE :: DEN_SCALE [27:00] */
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_SCALE_DEN_SCALE_MASK      0x0fffffff
#define BCHP_AUD_FMM_SRC_CTRL0_COEFFi_LI_SCALE_DEN_SCALE_SHIFT     0


#endif /* #ifndef BCHP_AUD_FMM_SRC_CTRL0_H__ */

/* End of File */
