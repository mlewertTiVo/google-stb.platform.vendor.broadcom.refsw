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
 * Date:           Generated on               Thu Jun 16 14:47:09 2016
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

#ifndef BCHP_SDSRC_0_H__
#define BCHP_SDSRC_0_H__

/***************************************************************************
 *SDSRC_0 - Sample Rate converter SDSRC_0
 ***************************************************************************/
#define BCHP_SDSRC_0_SRC_REV_ID                  0x206e2300 /* [RO] Revision ID register */
#define BCHP_SDSRC_0_SRC_CONTROL                 0x206e2304 /* [RW] Sample Rate Converter control register */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1             0x206e2308 /* [RW] Analog Scale Mult Value and offset Register for options 0,1 */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3             0x206e230c /* [RW] Analog Scale Mult value and Offset Register for options 2,3 */

/***************************************************************************
 *SRC_REV_ID - Revision ID register
 ***************************************************************************/
/* SDSRC_0 :: SRC_REV_ID :: reserved0 [31:16] */
#define BCHP_SDSRC_0_SRC_REV_ID_reserved0_MASK                     0xffff0000
#define BCHP_SDSRC_0_SRC_REV_ID_reserved0_SHIFT                    16

/* SDSRC_0 :: SRC_REV_ID :: REVISION_ID [15:00] */
#define BCHP_SDSRC_0_SRC_REV_ID_REVISION_ID_MASK                   0x0000ffff
#define BCHP_SDSRC_0_SRC_REV_ID_REVISION_ID_SHIFT                  0
#define BCHP_SDSRC_0_SRC_REV_ID_REVISION_ID_DEFAULT                0x00004000

/***************************************************************************
 *SRC_CONTROL - Sample Rate Converter control register
 ***************************************************************************/
/* SDSRC_0 :: SRC_CONTROL :: reserved0 [31:16] */
#define BCHP_SDSRC_0_SRC_CONTROL_reserved0_MASK                    0xffff0000
#define BCHP_SDSRC_0_SRC_CONTROL_reserved0_SHIFT                   16

/* SDSRC_0 :: SRC_CONTROL :: reserved_for_eco1 [15:09] */
#define BCHP_SDSRC_0_SRC_CONTROL_reserved_for_eco1_MASK            0x0000fe00
#define BCHP_SDSRC_0_SRC_CONTROL_reserved_for_eco1_SHIFT           9
#define BCHP_SDSRC_0_SRC_CONTROL_reserved_for_eco1_DEFAULT         0x00000000

/* SDSRC_0 :: SRC_CONTROL :: HSYNC [08:07] */
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_MASK                        0x00000180
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_SHIFT                       7
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_DEFAULT                     0x00000000
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_Minus_6dB                   0
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_Minus_11dB                  1
#define BCHP_SDSRC_0_SRC_CONTROL_HSYNC_Minus_18dB                  2

/* SDSRC_0 :: SRC_CONTROL :: H_CLAMP [06:06] */
#define BCHP_SDSRC_0_SRC_CONTROL_H_CLAMP_MASK                      0x00000040
#define BCHP_SDSRC_0_SRC_CONTROL_H_CLAMP_SHIFT                     6
#define BCHP_SDSRC_0_SRC_CONTROL_H_CLAMP_DEFAULT                   0x00000000
#define BCHP_SDSRC_0_SRC_CONTROL_H_CLAMP_UNSIGNED                  0
#define BCHP_SDSRC_0_SRC_CONTROL_H_CLAMP_SIGNED                    1

/* SDSRC_0 :: SRC_CONTROL :: CH12_CLAMP [05:05] */
#define BCHP_SDSRC_0_SRC_CONTROL_CH12_CLAMP_MASK                   0x00000020
#define BCHP_SDSRC_0_SRC_CONTROL_CH12_CLAMP_SHIFT                  5
#define BCHP_SDSRC_0_SRC_CONTROL_CH12_CLAMP_DEFAULT                0x00000001
#define BCHP_SDSRC_0_SRC_CONTROL_CH12_CLAMP_UNSIGNED               0
#define BCHP_SDSRC_0_SRC_CONTROL_CH12_CLAMP_SIGNED                 1

/* SDSRC_0 :: SRC_CONTROL :: CH0_CLAMP [04:04] */
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_CLAMP_MASK                    0x00000010
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_CLAMP_SHIFT                   4
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_CLAMP_DEFAULT                 0x00000000
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_CLAMP_UNSIGNED                0
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_CLAMP_SIGNED                  1

/* SDSRC_0 :: SRC_CONTROL :: CH1_2 [03:02] */
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_MASK                        0x0000000c
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_SHIFT                       2
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_DEFAULT                     0x00000002
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_Minus_6dB                   0
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_Minus_11dB                  1
#define BCHP_SDSRC_0_SRC_CONTROL_CH1_2_Minus_18dB                  2

/* SDSRC_0 :: SRC_CONTROL :: CH0 [01:00] */
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_MASK                          0x00000003
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_SHIFT                         0
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_DEFAULT                       0x00000002
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_Minus_6dB                     0
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_Minus_11dB                    1
#define BCHP_SDSRC_0_SRC_CONTROL_CH0_Minus_18dB                    2

/***************************************************************************
 *SRC_ANA_SCL_0_1 - Analog Scale Mult Value and offset Register for options 0,1
 ***************************************************************************/
/* SDSRC_0 :: SRC_ANA_SCL_0_1 :: reserved0 [31:26] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_reserved0_MASK                0xfc000000
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_reserved0_SHIFT               26

/* SDSRC_0 :: SRC_ANA_SCL_0_1 :: OFFSET_1 [25:22] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_1_MASK                 0x03c00000
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_1_SHIFT                22
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_1_DEFAULT              0x00000000

/* SDSRC_0 :: SRC_ANA_SCL_0_1 :: VALUE_1 [21:13] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_1_MASK                  0x003fe000
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_1_SHIFT                 13
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_1_DEFAULT               0x00000100

/* SDSRC_0 :: SRC_ANA_SCL_0_1 :: OFFSET_0 [12:09] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_0_MASK                 0x00001e00
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_0_SHIFT                9
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_OFFSET_0_DEFAULT              0x00000000

/* SDSRC_0 :: SRC_ANA_SCL_0_1 :: VALUE_0 [08:00] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_0_MASK                  0x000001ff
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_0_SHIFT                 0
#define BCHP_SDSRC_0_SRC_ANA_SCL_0_1_VALUE_0_DEFAULT               0x00000100

/***************************************************************************
 *SRC_ANA_SCL_2_3 - Analog Scale Mult value and Offset Register for options 2,3
 ***************************************************************************/
/* SDSRC_0 :: SRC_ANA_SCL_2_3 :: reserved0 [31:26] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_reserved0_MASK                0xfc000000
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_reserved0_SHIFT               26

/* SDSRC_0 :: SRC_ANA_SCL_2_3 :: OFFSET_3 [25:22] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_3_MASK                 0x03c00000
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_3_SHIFT                22
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_3_DEFAULT              0x00000000

/* SDSRC_0 :: SRC_ANA_SCL_2_3 :: VALUE_3 [21:13] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_3_MASK                  0x003fe000
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_3_SHIFT                 13
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_3_DEFAULT               0x00000100

/* SDSRC_0 :: SRC_ANA_SCL_2_3 :: OFFSET_2 [12:09] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_2_MASK                 0x00001e00
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_2_SHIFT                9
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_OFFSET_2_DEFAULT              0x00000000

/* SDSRC_0 :: SRC_ANA_SCL_2_3 :: VALUE_2 [08:00] */
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_2_MASK                  0x000001ff
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_2_SHIFT                 0
#define BCHP_SDSRC_0_SRC_ANA_SCL_2_3_VALUE_2_DEFAULT               0x00000100

#endif /* #ifndef BCHP_SDSRC_0_H__ */

/* End of File */
