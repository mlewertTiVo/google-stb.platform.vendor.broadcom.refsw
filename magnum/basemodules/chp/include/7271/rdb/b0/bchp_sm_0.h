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

#ifndef BCHP_SM_0_H__
#define BCHP_SM_0_H__

/***************************************************************************
 *SM_0 - Subcarrier Modulator SM_0
 ***************************************************************************/
#define BCHP_SM_0_SM_REV_ID                      0x206e2280 /* [RO] REVISION ID REGISTER */
#define BCHP_SM_0_PG_CNTRL                       0x206e228c /* [RW] PHASE GENERATOR CONTROL REGISTER */
#define BCHP_SM_0_PG_CONFIG                      0x206e2290 /* [RW] PHASE GENERATOR CONFIG REGISTER */
#define BCHP_SM_0_SC_FREQ_0                      0x206e2294 /* [RW] SUBCARRIER FREQUENCY 0 */
#define BCHP_SM_0_COMP_CNTRL                     0x206e2298 /* [RW] COMPOSITOR CONTROL REGISTER */
#define BCHP_SM_0_SA_CH2                         0x206e229c /* [RO] CHANNEL2 SA STATUS REGISTER */
#define BCHP_SM_0_SA_CH1                         0x206e22a0 /* [RO] CHANNEL1 SA STATUS REGISTER */
#define BCHP_SM_0_SA_CH0                         0x206e22a4 /* [RO] CHANNEL0 SA STATUS REGISTER */
#define BCHP_SM_0_SA_CONFIG                      0x206e22a8 /* [RW] SA CONFIGURATION REGISTER */
#define BCHP_SM_0_COMP_CONFIG                    0x206e22ac /* [RW] COMPOSITOR CONFIG REGISTER */

/***************************************************************************
 *SM_REV_ID - REVISION ID REGISTER
 ***************************************************************************/
/* SM_0 :: SM_REV_ID :: reserved0 [31:16] */
#define BCHP_SM_0_SM_REV_ID_reserved0_MASK                         0xffff0000
#define BCHP_SM_0_SM_REV_ID_reserved0_SHIFT                        16

/* SM_0 :: SM_REV_ID :: REVISION_ID [15:00] */
#define BCHP_SM_0_SM_REV_ID_REVISION_ID_MASK                       0x0000ffff
#define BCHP_SM_0_SM_REV_ID_REVISION_ID_SHIFT                      0
#define BCHP_SM_0_SM_REV_ID_REVISION_ID_DEFAULT                    0x00004000

/***************************************************************************
 *PG_CNTRL - PHASE GENERATOR CONTROL REGISTER
 ***************************************************************************/
/* SM_0 :: PG_CNTRL :: reserved0 [31:21] */
#define BCHP_SM_0_PG_CNTRL_reserved0_MASK                          0xffe00000
#define BCHP_SM_0_PG_CNTRL_reserved0_SHIFT                         21

/* SM_0 :: PG_CNTRL :: PIXEL_FRAC_ENABLE [20:20] */
#define BCHP_SM_0_PG_CNTRL_PIXEL_FRAC_ENABLE_MASK                  0x00100000
#define BCHP_SM_0_PG_CNTRL_PIXEL_FRAC_ENABLE_SHIFT                 20
#define BCHP_SM_0_PG_CNTRL_PIXEL_FRAC_ENABLE_DEFAULT               0x00000000
#define BCHP_SM_0_PG_CNTRL_PIXEL_FRAC_ENABLE_ON                    1
#define BCHP_SM_0_PG_CNTRL_PIXEL_FRAC_ENABLE_OFF                   0

/* SM_0 :: PG_CNTRL :: INIT_PHASE [19:10] */
#define BCHP_SM_0_PG_CNTRL_INIT_PHASE_MASK                         0x000ffc00
#define BCHP_SM_0_PG_CNTRL_INIT_PHASE_SHIFT                        10

/* SM_0 :: PG_CNTRL :: ACTIVE_PHASE_OFFSET [09:00] */
#define BCHP_SM_0_PG_CNTRL_ACTIVE_PHASE_OFFSET_MASK                0x000003ff
#define BCHP_SM_0_PG_CNTRL_ACTIVE_PHASE_OFFSET_SHIFT               0
#define BCHP_SM_0_PG_CNTRL_ACTIVE_PHASE_OFFSET_DEFAULT             0x0000005e

/***************************************************************************
 *PG_CONFIG - PHASE GENERATOR CONFIG REGISTER
 ***************************************************************************/
/* SM_0 :: PG_CONFIG :: reserved0 [31:05] */
#define BCHP_SM_0_PG_CONFIG_reserved0_MASK                         0xffffffe0
#define BCHP_SM_0_PG_CONFIG_reserved0_SHIFT                        5

/* SM_0 :: PG_CONFIG :: U_CORING_ENABLE [04:04] */
#define BCHP_SM_0_PG_CONFIG_U_CORING_ENABLE_MASK                   0x00000010
#define BCHP_SM_0_PG_CONFIG_U_CORING_ENABLE_SHIFT                  4
#define BCHP_SM_0_PG_CONFIG_U_CORING_ENABLE_DEFAULT                0x00000000
#define BCHP_SM_0_PG_CONFIG_U_CORING_ENABLE_ON                     1
#define BCHP_SM_0_PG_CONFIG_U_CORING_ENABLE_OFF                    0

/* SM_0 :: PG_CONFIG :: V_CORING_ENABLE [03:03] */
#define BCHP_SM_0_PG_CONFIG_V_CORING_ENABLE_MASK                   0x00000008
#define BCHP_SM_0_PG_CONFIG_V_CORING_ENABLE_SHIFT                  3
#define BCHP_SM_0_PG_CONFIG_V_CORING_ENABLE_DEFAULT                0x00000000
#define BCHP_SM_0_PG_CONFIG_V_CORING_ENABLE_ON                     1
#define BCHP_SM_0_PG_CONFIG_V_CORING_ENABLE_OFF                    0

/* SM_0 :: PG_CONFIG :: reserved1 [02:00] */
#define BCHP_SM_0_PG_CONFIG_reserved1_MASK                         0x00000007
#define BCHP_SM_0_PG_CONFIG_reserved1_SHIFT                        0

/***************************************************************************
 *SC_FREQ_0 - SUBCARRIER FREQUENCY 0
 ***************************************************************************/
/* SM_0 :: SC_FREQ_0 :: FRE0 [31:00] */
#define BCHP_SM_0_SC_FREQ_0_FRE0_MASK                              0xffffffff
#define BCHP_SM_0_SC_FREQ_0_FRE0_SHIFT                             0

/***************************************************************************
 *COMP_CNTRL - COMPOSITOR CONTROL REGISTER
 ***************************************************************************/
/* SM_0 :: COMP_CNTRL :: reserved0 [31:28] */
#define BCHP_SM_0_COMP_CNTRL_reserved0_MASK                        0xf0000000
#define BCHP_SM_0_COMP_CNTRL_reserved0_SHIFT                       28

/* SM_0 :: COMP_CNTRL :: CHROMA_OFFSET [27:16] */
#define BCHP_SM_0_COMP_CNTRL_CHROMA_OFFSET_MASK                    0x0fff0000
#define BCHP_SM_0_COMP_CNTRL_CHROMA_OFFSET_SHIFT                   16
#define BCHP_SM_0_COMP_CNTRL_CHROMA_OFFSET_DEFAULT                 0x00000000

/* SM_0 :: COMP_CNTRL :: reserved1 [15:06] */
#define BCHP_SM_0_COMP_CNTRL_reserved1_MASK                        0x0000ffc0
#define BCHP_SM_0_COMP_CNTRL_reserved1_SHIFT                       6

/* SM_0 :: COMP_CNTRL :: COMP_OUT_SEL [05:04] */
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_MASK                     0x00000030
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_SHIFT                    4
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_DEFAULT                  0x00000000
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_COMPONENT                0
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_CHROMA_OUT               1
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_Y_AND_SIN                2
#define BCHP_SM_0_COMP_CNTRL_COMP_OUT_SEL_COMPOSITE                3

/* SM_0 :: COMP_CNTRL :: COMBINE_CHROMA [03:03] */
#define BCHP_SM_0_COMP_CNTRL_COMBINE_CHROMA_MASK                   0x00000008
#define BCHP_SM_0_COMP_CNTRL_COMBINE_CHROMA_SHIFT                  3
#define BCHP_SM_0_COMP_CNTRL_COMBINE_CHROMA_DEFAULT                0x00000000
#define BCHP_SM_0_COMP_CNTRL_COMBINE_CHROMA_ON                     1
#define BCHP_SM_0_COMP_CNTRL_COMBINE_CHROMA_OFF                    0

/* SM_0 :: COMP_CNTRL :: reserved2 [02:02] */
#define BCHP_SM_0_COMP_CNTRL_reserved2_MASK                        0x00000004
#define BCHP_SM_0_COMP_CNTRL_reserved2_SHIFT                       2

/* SM_0 :: COMP_CNTRL :: SIN_COEF_EN [01:01] */
#define BCHP_SM_0_COMP_CNTRL_SIN_COEF_EN_MASK                      0x00000002
#define BCHP_SM_0_COMP_CNTRL_SIN_COEF_EN_SHIFT                     1
#define BCHP_SM_0_COMP_CNTRL_SIN_COEF_EN_DEFAULT                   0x00000000
#define BCHP_SM_0_COMP_CNTRL_SIN_COEF_EN_USE_SIN                   1
#define BCHP_SM_0_COMP_CNTRL_SIN_COEF_EN_USE_ONE                   0

/* SM_0 :: COMP_CNTRL :: COS_COEF_EN [00:00] */
#define BCHP_SM_0_COMP_CNTRL_COS_COEF_EN_MASK                      0x00000001
#define BCHP_SM_0_COMP_CNTRL_COS_COEF_EN_SHIFT                     0
#define BCHP_SM_0_COMP_CNTRL_COS_COEF_EN_DEFAULT                   0x00000000
#define BCHP_SM_0_COMP_CNTRL_COS_COEF_EN_USE_COS                   1
#define BCHP_SM_0_COMP_CNTRL_COS_COEF_EN_USE_ONE                   0

/***************************************************************************
 *SA_CH2 - CHANNEL2 SA STATUS REGISTER
 ***************************************************************************/
/* SM_0 :: SA_CH2 :: STATUS [31:00] */
#define BCHP_SM_0_SA_CH2_STATUS_MASK                               0xffffffff
#define BCHP_SM_0_SA_CH2_STATUS_SHIFT                              0

/***************************************************************************
 *SA_CH1 - CHANNEL1 SA STATUS REGISTER
 ***************************************************************************/
/* SM_0 :: SA_CH1 :: STATUS [31:00] */
#define BCHP_SM_0_SA_CH1_STATUS_MASK                               0xffffffff
#define BCHP_SM_0_SA_CH1_STATUS_SHIFT                              0

/***************************************************************************
 *SA_CH0 - CHANNEL0 SA STATUS REGISTER
 ***************************************************************************/
/* SM_0 :: SA_CH0 :: STATUS [31:00] */
#define BCHP_SM_0_SA_CH0_STATUS_MASK                               0xffffffff
#define BCHP_SM_0_SA_CH0_STATUS_SHIFT                              0

/***************************************************************************
 *SA_CONFIG - SA CONFIGURATION REGISTER
 ***************************************************************************/
/* SM_0 :: SA_CONFIG :: reserved0 [31:13] */
#define BCHP_SM_0_SA_CONFIG_reserved0_MASK                         0xffffe000
#define BCHP_SM_0_SA_CONFIG_reserved0_SHIFT                        13

/* SM_0 :: SA_CONFIG :: SEL [12:11] */
#define BCHP_SM_0_SA_CONFIG_SEL_MASK                               0x00001800
#define BCHP_SM_0_SA_CONFIG_SEL_SHIFT                              11
#define BCHP_SM_0_SA_CONFIG_SEL_DEFAULT                            0x00000000

/* SM_0 :: SA_CONFIG :: EDGE_SELECT [10:09] */
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_MASK                       0x00000600
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_SHIFT                      9
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_DEFAULT                    0x00000000
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_RISE_EDGE                  0
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_FALL_EDGE                  1
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_BOTH_EDGES                 2
#define BCHP_SM_0_SA_CONFIG_EDGE_SELECT_NOT_DEFINED                3

/* SM_0 :: SA_CONFIG :: SA_CH0_EN [08:08] */
#define BCHP_SM_0_SA_CONFIG_SA_CH0_EN_MASK                         0x00000100
#define BCHP_SM_0_SA_CONFIG_SA_CH0_EN_SHIFT                        8
#define BCHP_SM_0_SA_CONFIG_SA_CH0_EN_DEFAULT                      0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH0_EN_ON                           1
#define BCHP_SM_0_SA_CONFIG_SA_CH0_EN_OFF                          0

/* SM_0 :: SA_CONFIG :: SA_CH1_EN [07:07] */
#define BCHP_SM_0_SA_CONFIG_SA_CH1_EN_MASK                         0x00000080
#define BCHP_SM_0_SA_CONFIG_SA_CH1_EN_SHIFT                        7
#define BCHP_SM_0_SA_CONFIG_SA_CH1_EN_DEFAULT                      0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH1_EN_ON                           1
#define BCHP_SM_0_SA_CONFIG_SA_CH1_EN_OFF                          0

/* SM_0 :: SA_CONFIG :: SA_CH2_EN [06:06] */
#define BCHP_SM_0_SA_CONFIG_SA_CH2_EN_MASK                         0x00000040
#define BCHP_SM_0_SA_CONFIG_SA_CH2_EN_SHIFT                        6
#define BCHP_SM_0_SA_CONFIG_SA_CH2_EN_DEFAULT                      0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH2_EN_ON                           1
#define BCHP_SM_0_SA_CONFIG_SA_CH2_EN_OFF                          0

/* SM_0 :: SA_CONFIG :: SA_CH0_CLEAR [05:05] */
#define BCHP_SM_0_SA_CONFIG_SA_CH0_CLEAR_MASK                      0x00000020
#define BCHP_SM_0_SA_CONFIG_SA_CH0_CLEAR_SHIFT                     5
#define BCHP_SM_0_SA_CONFIG_SA_CH0_CLEAR_DEFAULT                   0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH0_CLEAR_ON                        1
#define BCHP_SM_0_SA_CONFIG_SA_CH0_CLEAR_OFF                       0

/* SM_0 :: SA_CONFIG :: SA_CH1_CLEAR [04:04] */
#define BCHP_SM_0_SA_CONFIG_SA_CH1_CLEAR_MASK                      0x00000010
#define BCHP_SM_0_SA_CONFIG_SA_CH1_CLEAR_SHIFT                     4
#define BCHP_SM_0_SA_CONFIG_SA_CH1_CLEAR_DEFAULT                   0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH1_CLEAR_ON                        1
#define BCHP_SM_0_SA_CONFIG_SA_CH1_CLEAR_OFF                       0

/* SM_0 :: SA_CONFIG :: SA_CH2_CLEAR [03:03] */
#define BCHP_SM_0_SA_CONFIG_SA_CH2_CLEAR_MASK                      0x00000008
#define BCHP_SM_0_SA_CONFIG_SA_CH2_CLEAR_SHIFT                     3
#define BCHP_SM_0_SA_CONFIG_SA_CH2_CLEAR_DEFAULT                   0x00000000
#define BCHP_SM_0_SA_CONFIG_SA_CH2_CLEAR_ON                        1
#define BCHP_SM_0_SA_CONFIG_SA_CH2_CLEAR_OFF                       0

/* SM_0 :: SA_CONFIG :: SA_PROBE_RATE [02:00] */
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_MASK                     0x00000007
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_SHIFT                    0
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_DEFAULT                  0x00000001
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_ZERO                     0
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_WINDOW               1
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_2WINDOWS             2
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_3WINDOWS             3
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_4WINDOWS             4
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_5WINDOWS             5
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_6WINDOWS             6
#define BCHP_SM_0_SA_CONFIG_SA_PROBE_RATE_PER_7WINDOWS             7

/***************************************************************************
 *COMP_CONFIG - COMPOSITOR CONFIG REGISTER
 ***************************************************************************/
/* SM_0 :: COMP_CONFIG :: reserved0 [31:19] */
#define BCHP_SM_0_COMP_CONFIG_reserved0_MASK                       0xfff80000
#define BCHP_SM_0_COMP_CONFIG_reserved0_SHIFT                      19

/* SM_0 :: COMP_CONFIG :: COMPOSITE_CLAMP_SEL [18:18] */
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_CLAMP_SEL_MASK             0x00040000
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_CLAMP_SEL_SHIFT            18
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_CLAMP_SEL_DEFAULT          0x00000000
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_CLAMP_SEL_CLAMP            1
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_CLAMP_SEL_NO_CLAMP         0

/* SM_0 :: COMP_CONFIG :: U_LINE_SEL [17:17] */
#define BCHP_SM_0_COMP_CONFIG_U_LINE_SEL_MASK                      0x00020000
#define BCHP_SM_0_COMP_CONFIG_U_LINE_SEL_SHIFT                     17
#define BCHP_SM_0_COMP_CONFIG_U_LINE_SEL_DEFAULT                   0x00000000
#define BCHP_SM_0_COMP_CONFIG_U_LINE_SEL_ALT                       1
#define BCHP_SM_0_COMP_CONFIG_U_LINE_SEL_FIXED                     0

/* SM_0 :: COMP_CONFIG :: V_LINE_SEL [16:16] */
#define BCHP_SM_0_COMP_CONFIG_V_LINE_SEL_MASK                      0x00010000
#define BCHP_SM_0_COMP_CONFIG_V_LINE_SEL_SHIFT                     16
#define BCHP_SM_0_COMP_CONFIG_V_LINE_SEL_DEFAULT                   0x00000000
#define BCHP_SM_0_COMP_CONFIG_V_LINE_SEL_ALT                       1
#define BCHP_SM_0_COMP_CONFIG_V_LINE_SEL_FIXED                     0

/* SM_0 :: COMP_CONFIG :: U_FIXED_LINE [15:15] */
#define BCHP_SM_0_COMP_CONFIG_U_FIXED_LINE_MASK                    0x00008000
#define BCHP_SM_0_COMP_CONFIG_U_FIXED_LINE_SHIFT                   15
#define BCHP_SM_0_COMP_CONFIG_U_FIXED_LINE_DEFAULT                 0x00000000
#define BCHP_SM_0_COMP_CONFIG_U_FIXED_LINE_ONE                     1
#define BCHP_SM_0_COMP_CONFIG_U_FIXED_LINE_ZERO                    0

/* SM_0 :: COMP_CONFIG :: V_FIXED_LINE [14:14] */
#define BCHP_SM_0_COMP_CONFIG_V_FIXED_LINE_MASK                    0x00004000
#define BCHP_SM_0_COMP_CONFIG_V_FIXED_LINE_SHIFT                   14
#define BCHP_SM_0_COMP_CONFIG_V_FIXED_LINE_DEFAULT                 0x00000000
#define BCHP_SM_0_COMP_CONFIG_V_FIXED_LINE_ONE                     1
#define BCHP_SM_0_COMP_CONFIG_V_FIXED_LINE_ZERO                    0

/* SM_0 :: COMP_CONFIG :: reserved1 [13:12] */
#define BCHP_SM_0_COMP_CONFIG_reserved1_MASK                       0x00003000
#define BCHP_SM_0_COMP_CONFIG_reserved1_SHIFT                      12

/* SM_0 :: COMP_CONFIG :: COMPOSITE_OFFSET [11:00] */
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_OFFSET_MASK                0x00000fff
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_OFFSET_SHIFT               0
#define BCHP_SM_0_COMP_CONFIG_COMPOSITE_OFFSET_DEFAULT             0x00000000

#endif /* #ifndef BCHP_SM_0_H__ */

/* End of File */
