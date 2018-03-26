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
 * Date:           Generated on               Tue Mar 14 11:30:19 2017
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

#ifndef BCHP_AVS_PVT_MNTR_CONFIG_H__
#define BCHP_AVS_PVT_MNTR_CONFIG_H__

/***************************************************************************
 *AVS_PVT_MNTR_CONFIG - AVS PVT Monitor Configuration Core
 ***************************************************************************/
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL   0x204d2100 /* [RW][32] Control bits for PVT monitor */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE 0x204d2108 /* [RW][32] Enabling TP mode to use data from tp_in/out pins */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE 0x204d2110 /* [RW][32] SW must set this bit to 1 to modify DAC_CODE, MIN_DAC_CODE and MAX_DAC_CODE */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE        0x204d2114 /* [RW][32] Represents the input code of the DAC */
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE    0x204d2118 /* [RW][32] SW may set this to a lower value to prevent the DAC from driving too high of a voltage */
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE    0x204d211c /* [RW][32] SW may set this to a higher value to prevent the DAC from driving too low of a voltage */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB 0x204d2120 /* [RW][32] SW controlled PVTMON_resetb */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR 0x204d2124 /* [RW][32] Control register to drive control bits for analog SRAM indicator */

/***************************************************************************
 *PVT_MNTR_CTRL - Control bits for PVT monitor
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_CTRL :: reserved0 [31:19] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL_reserved0_MASK      0xfff80000
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL_reserved0_SHIFT     19

/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_CTRL :: pvtmon_ctrl [18:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL_pvtmon_ctrl_MASK    0x0007ffff
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL_pvtmon_ctrl_SHIFT   0
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_CTRL_pvtmon_ctrl_DEFAULT 0x00000000

/***************************************************************************
 *PVT_MNTR_TP_MODE_ENABLE - Enabling TP mode to use data from tp_in/out pins
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_TP_MODE_ENABLE :: reserved0 [31:01] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE_reserved0_MASK 0xfffffffe
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE_reserved0_SHIFT 1

/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_TP_MODE_ENABLE :: tp_mode_en [00:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE_tp_mode_en_MASK 0x00000001
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE_tp_mode_en_SHIFT 0
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_TP_MODE_ENABLE_tp_mode_en_DEFAULT 0x00000000

/***************************************************************************
 *DAC_CODE_PROGRAMMING_ENABLE - SW must set this bit to 1 to modify DAC_CODE, MIN_DAC_CODE and MAX_DAC_CODE
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: DAC_CODE_PROGRAMMING_ENABLE :: reserved0 [31:01] */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE_reserved0_MASK 0xfffffffe
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE_reserved0_SHIFT 1

/* AVS_PVT_MNTR_CONFIG :: DAC_CODE_PROGRAMMING_ENABLE :: code_program_en [00:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE_code_program_en_MASK 0x00000001
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE_code_program_en_SHIFT 0
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_PROGRAMMING_ENABLE_code_program_en_DEFAULT 0x00000000

/***************************************************************************
 *DAC_CODE - Represents the input code of the DAC
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: DAC_CODE :: reserved0 [31:10] */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_reserved0_MASK           0xfffffc00
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_reserved0_SHIFT          10

/* AVS_PVT_MNTR_CONFIG :: DAC_CODE :: dac_code [09:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_dac_code_MASK            0x000003ff
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_dac_code_SHIFT           0
#define BCHP_AVS_PVT_MNTR_CONFIG_DAC_CODE_dac_code_DEFAULT         0x00000000

/***************************************************************************
 *MIN_DAC_CODE - SW may set this to a lower value to prevent the DAC from driving too high of a voltage
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: MIN_DAC_CODE :: reserved0 [31:10] */
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE_reserved0_MASK       0xfffffc00
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE_reserved0_SHIFT      10

/* AVS_PVT_MNTR_CONFIG :: MIN_DAC_CODE :: min_dac_code [09:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE_min_dac_code_MASK    0x000003ff
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE_min_dac_code_SHIFT   0
#define BCHP_AVS_PVT_MNTR_CONFIG_MIN_DAC_CODE_min_dac_code_DEFAULT 0x00000000

/***************************************************************************
 *MAX_DAC_CODE - SW may set this to a higher value to prevent the DAC from driving too low of a voltage
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: MAX_DAC_CODE :: reserved0 [31:10] */
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE_reserved0_MASK       0xfffffc00
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE_reserved0_SHIFT      10

/* AVS_PVT_MNTR_CONFIG :: MAX_DAC_CODE :: max_dac_code [09:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE_max_dac_code_MASK    0x000003ff
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE_max_dac_code_SHIFT   0
#define BCHP_AVS_PVT_MNTR_CONFIG_MAX_DAC_CODE_max_dac_code_DEFAULT 0x000003ff

/***************************************************************************
 *PVT_MNTR_SW_RESETB - SW controlled PVTMON_resetb
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_SW_RESETB :: reserved0 [31:01] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB_reserved0_MASK 0xfffffffe
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB_reserved0_SHIFT 1

/* AVS_PVT_MNTR_CONFIG :: PVT_MNTR_SW_RESETB :: pvtmon_sw_resetb [00:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB_pvtmon_sw_resetb_MASK 0x00000001
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB_pvtmon_sw_resetb_SHIFT 0
#define BCHP_AVS_PVT_MNTR_CONFIG_PVT_MNTR_SW_RESETB_pvtmon_sw_resetb_DEFAULT 0x00000001

/***************************************************************************
 *AVS_TOP_ANALOG_SRAM_INDICATOR - Control register to drive control bits for analog SRAM indicator
 ***************************************************************************/
/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: reserved0 [31:09] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved0_MASK 0xfffffe00
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved0_SHIFT 9

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: enable [08:08] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_enable_MASK 0x00000100
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_enable_SHIFT 8
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_enable_DEFAULT 0x00000001

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: reserved1 [07:05] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved1_MASK 0x000000e0
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved1_SHIFT 5

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: over_drive [04:04] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_over_drive_MASK 0x00000010
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_over_drive_SHIFT 4
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_over_drive_DEFAULT 0x00000001

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: reserved2 [03:02] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved2_MASK 0x0000000c
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_reserved2_SHIFT 2

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: type [01:01] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_type_MASK 0x00000002
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_type_SHIFT 1
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_type_DEFAULT 0x00000001

/* AVS_PVT_MNTR_CONFIG :: AVS_TOP_ANALOG_SRAM_INDICATOR :: process [00:00] */
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_process_MASK 0x00000001
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_process_SHIFT 0
#define BCHP_AVS_PVT_MNTR_CONFIG_AVS_TOP_ANALOG_SRAM_INDICATOR_process_DEFAULT 0x00000001

#endif /* #ifndef BCHP_AVS_PVT_MNTR_CONFIG_H__ */

/* End of File */
