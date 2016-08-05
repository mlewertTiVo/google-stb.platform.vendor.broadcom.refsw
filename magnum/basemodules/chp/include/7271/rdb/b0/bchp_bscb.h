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

#ifndef BCHP_BSCB_H__
#define BCHP_BSCB_H__

/***************************************************************************
 *BSCB - Broadcom Serial Control Master B
 ***************************************************************************/
#define BCHP_BSCB_CHIP_ADDRESS                   0x20419a80 /* [RW] BSC Chip Address And Read/Write Control */
#define BCHP_BSCB_DATA_IN0                       0x20419a84 /* [RW] BSC Write Data Register 0 */
#define BCHP_BSCB_DATA_IN1                       0x20419a88 /* [RW] BSC Write Data Register 1 */
#define BCHP_BSCB_DATA_IN2                       0x20419a8c /* [RW] BSC Write Data Register 2 */
#define BCHP_BSCB_DATA_IN3                       0x20419a90 /* [RW] BSC Write Data Register 3 */
#define BCHP_BSCB_DATA_IN4                       0x20419a94 /* [RW] BSC Write Data Register 4 */
#define BCHP_BSCB_DATA_IN5                       0x20419a98 /* [RW] BSC Write Data Register 5 */
#define BCHP_BSCB_DATA_IN6                       0x20419a9c /* [RW] BSC Write Data Register 6 */
#define BCHP_BSCB_DATA_IN7                       0x20419aa0 /* [RW] BSC Write Data Register 7 */
#define BCHP_BSCB_CNT_REG                        0x20419aa4 /* [RW] BSC Transfer Count Register */
#define BCHP_BSCB_CTL_REG                        0x20419aa8 /* [RW] BSC Control Register */
#define BCHP_BSCB_IIC_ENABLE                     0x20419aac /* [RW] BSC Read/Write Enable And Interrupt */
#define BCHP_BSCB_DATA_OUT0                      0x20419ab0 /* [RO] BSC Read Data Register 0 */
#define BCHP_BSCB_DATA_OUT1                      0x20419ab4 /* [RO] BSC Read Data Register 1 */
#define BCHP_BSCB_DATA_OUT2                      0x20419ab8 /* [RO] BSC Read Data Register 2 */
#define BCHP_BSCB_DATA_OUT3                      0x20419abc /* [RO] BSC Read Data Register 3 */
#define BCHP_BSCB_DATA_OUT4                      0x20419ac0 /* [RO] BSC Read Data Register 4 */
#define BCHP_BSCB_DATA_OUT5                      0x20419ac4 /* [RO] BSC Read Data Register 5 */
#define BCHP_BSCB_DATA_OUT6                      0x20419ac8 /* [RO] BSC Read Data Register 6 */
#define BCHP_BSCB_DATA_OUT7                      0x20419acc /* [RO] BSC Read Data Register 7 */
#define BCHP_BSCB_CTLHI_REG                      0x20419ad0 /* [RW] BSC Control Register */
#define BCHP_BSCB_SCL_PARAM                      0x20419ad4 /* [RW] BSC SCL Parameter Register */

/***************************************************************************
 *CHIP_ADDRESS - BSC Chip Address And Read/Write Control
 ***************************************************************************/
/* BSCB :: CHIP_ADDRESS :: reserved0 [31:08] */
#define BCHP_BSCB_CHIP_ADDRESS_reserved0_MASK                      0xffffff00
#define BCHP_BSCB_CHIP_ADDRESS_reserved0_SHIFT                     8

/* BSCB :: CHIP_ADDRESS :: CHIP_ADDRESS [07:01] */
#define BCHP_BSCB_CHIP_ADDRESS_CHIP_ADDRESS_MASK                   0x000000fe
#define BCHP_BSCB_CHIP_ADDRESS_CHIP_ADDRESS_SHIFT                  1
#define BCHP_BSCB_CHIP_ADDRESS_CHIP_ADDRESS_DEFAULT                0x00000000

/* BSCB :: CHIP_ADDRESS :: SPARE [00:00] */
#define BCHP_BSCB_CHIP_ADDRESS_SPARE_MASK                          0x00000001
#define BCHP_BSCB_CHIP_ADDRESS_SPARE_SHIFT                         0
#define BCHP_BSCB_CHIP_ADDRESS_SPARE_DEFAULT                       0x00000000

/***************************************************************************
 *DATA_IN0 - BSC Write Data Register 0
 ***************************************************************************/
/* BSCB :: DATA_IN0 :: DATA_IN0 [31:00] */
#define BCHP_BSCB_DATA_IN0_DATA_IN0_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN0_DATA_IN0_SHIFT                          0

/***************************************************************************
 *DATA_IN1 - BSC Write Data Register 1
 ***************************************************************************/
/* BSCB :: DATA_IN1 :: DATA_IN1 [31:00] */
#define BCHP_BSCB_DATA_IN1_DATA_IN1_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN1_DATA_IN1_SHIFT                          0

/***************************************************************************
 *DATA_IN2 - BSC Write Data Register 2
 ***************************************************************************/
/* BSCB :: DATA_IN2 :: DATA_IN2 [31:00] */
#define BCHP_BSCB_DATA_IN2_DATA_IN2_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN2_DATA_IN2_SHIFT                          0

/***************************************************************************
 *DATA_IN3 - BSC Write Data Register 3
 ***************************************************************************/
/* BSCB :: DATA_IN3 :: DATA_IN3 [31:00] */
#define BCHP_BSCB_DATA_IN3_DATA_IN3_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN3_DATA_IN3_SHIFT                          0

/***************************************************************************
 *DATA_IN4 - BSC Write Data Register 4
 ***************************************************************************/
/* BSCB :: DATA_IN4 :: DATA_IN4 [31:00] */
#define BCHP_BSCB_DATA_IN4_DATA_IN4_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN4_DATA_IN4_SHIFT                          0

/***************************************************************************
 *DATA_IN5 - BSC Write Data Register 5
 ***************************************************************************/
/* BSCB :: DATA_IN5 :: DATA_IN5 [31:00] */
#define BCHP_BSCB_DATA_IN5_DATA_IN5_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN5_DATA_IN5_SHIFT                          0

/***************************************************************************
 *DATA_IN6 - BSC Write Data Register 6
 ***************************************************************************/
/* BSCB :: DATA_IN6 :: DATA_IN6 [31:00] */
#define BCHP_BSCB_DATA_IN6_DATA_IN6_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN6_DATA_IN6_SHIFT                          0

/***************************************************************************
 *DATA_IN7 - BSC Write Data Register 7
 ***************************************************************************/
/* BSCB :: DATA_IN7 :: DATA_IN7 [31:00] */
#define BCHP_BSCB_DATA_IN7_DATA_IN7_MASK                           0xffffffff
#define BCHP_BSCB_DATA_IN7_DATA_IN7_SHIFT                          0

/***************************************************************************
 *CNT_REG - BSC Transfer Count Register
 ***************************************************************************/
/* BSCB :: CNT_REG :: reserved0 [31:12] */
#define BCHP_BSCB_CNT_REG_reserved0_MASK                           0xfffff000
#define BCHP_BSCB_CNT_REG_reserved0_SHIFT                          12

/* BSCB :: CNT_REG :: CNT_REG2 [11:06] */
#define BCHP_BSCB_CNT_REG_CNT_REG2_MASK                            0x00000fc0
#define BCHP_BSCB_CNT_REG_CNT_REG2_SHIFT                           6
#define BCHP_BSCB_CNT_REG_CNT_REG2_DEFAULT                         0x00000000

/* BSCB :: CNT_REG :: CNT_REG1 [05:00] */
#define BCHP_BSCB_CNT_REG_CNT_REG1_MASK                            0x0000003f
#define BCHP_BSCB_CNT_REG_CNT_REG1_SHIFT                           0
#define BCHP_BSCB_CNT_REG_CNT_REG1_DEFAULT                         0x00000000

/***************************************************************************
 *CTL_REG - BSC Control Register
 ***************************************************************************/
/* BSCB :: CTL_REG :: reserved0 [31:11] */
#define BCHP_BSCB_CTL_REG_reserved0_MASK                           0xfffff800
#define BCHP_BSCB_CTL_REG_reserved0_SHIFT                          11

/* BSCB :: CTL_REG :: SDA_DELAY_SEL [10:08] */
#define BCHP_BSCB_CTL_REG_SDA_DELAY_SEL_MASK                       0x00000700
#define BCHP_BSCB_CTL_REG_SDA_DELAY_SEL_SHIFT                      8
#define BCHP_BSCB_CTL_REG_SDA_DELAY_SEL_DEFAULT                    0x00000000

/* BSCB :: CTL_REG :: DIV_CLK [07:07] */
#define BCHP_BSCB_CTL_REG_DIV_CLK_MASK                             0x00000080
#define BCHP_BSCB_CTL_REG_DIV_CLK_SHIFT                            7
#define BCHP_BSCB_CTL_REG_DIV_CLK_DEFAULT                          0x00000000

/* BSCB :: CTL_REG :: INT_EN [06:06] */
#define BCHP_BSCB_CTL_REG_INT_EN_MASK                              0x00000040
#define BCHP_BSCB_CTL_REG_INT_EN_SHIFT                             6
#define BCHP_BSCB_CTL_REG_INT_EN_DEFAULT                           0x00000000

/* BSCB :: CTL_REG :: SCL_SEL [05:04] */
#define BCHP_BSCB_CTL_REG_SCL_SEL_MASK                             0x00000030
#define BCHP_BSCB_CTL_REG_SCL_SEL_SHIFT                            4
#define BCHP_BSCB_CTL_REG_SCL_SEL_DEFAULT                          0x00000000

/* BSCB :: CTL_REG :: DELAY_DIS [03:03] */
#define BCHP_BSCB_CTL_REG_DELAY_DIS_MASK                           0x00000008
#define BCHP_BSCB_CTL_REG_DELAY_DIS_SHIFT                          3
#define BCHP_BSCB_CTL_REG_DELAY_DIS_DEFAULT                        0x00000000

/* BSCB :: CTL_REG :: DEGLITCH_DIS [02:02] */
#define BCHP_BSCB_CTL_REG_DEGLITCH_DIS_MASK                        0x00000004
#define BCHP_BSCB_CTL_REG_DEGLITCH_DIS_SHIFT                       2
#define BCHP_BSCB_CTL_REG_DEGLITCH_DIS_DEFAULT                     0x00000000

/* BSCB :: CTL_REG :: DTF [01:00] */
#define BCHP_BSCB_CTL_REG_DTF_MASK                                 0x00000003
#define BCHP_BSCB_CTL_REG_DTF_SHIFT                                0
#define BCHP_BSCB_CTL_REG_DTF_DEFAULT                              0x00000000

/***************************************************************************
 *IIC_ENABLE - BSC Read/Write Enable And Interrupt
 ***************************************************************************/
/* BSCB :: IIC_ENABLE :: reserved0 [31:07] */
#define BCHP_BSCB_IIC_ENABLE_reserved0_MASK                        0xffffff80
#define BCHP_BSCB_IIC_ENABLE_reserved0_SHIFT                       7

/* BSCB :: IIC_ENABLE :: RESTART [06:06] */
#define BCHP_BSCB_IIC_ENABLE_RESTART_MASK                          0x00000040
#define BCHP_BSCB_IIC_ENABLE_RESTART_SHIFT                         6
#define BCHP_BSCB_IIC_ENABLE_RESTART_DEFAULT                       0x00000000

/* BSCB :: IIC_ENABLE :: NO_START [05:05] */
#define BCHP_BSCB_IIC_ENABLE_NO_START_MASK                         0x00000020
#define BCHP_BSCB_IIC_ENABLE_NO_START_SHIFT                        5
#define BCHP_BSCB_IIC_ENABLE_NO_START_DEFAULT                      0x00000000

/* BSCB :: IIC_ENABLE :: NO_STOP [04:04] */
#define BCHP_BSCB_IIC_ENABLE_NO_STOP_MASK                          0x00000010
#define BCHP_BSCB_IIC_ENABLE_NO_STOP_SHIFT                         4
#define BCHP_BSCB_IIC_ENABLE_NO_STOP_DEFAULT                       0x00000000

/* BSCB :: IIC_ENABLE :: reserved1 [03:03] */
#define BCHP_BSCB_IIC_ENABLE_reserved1_MASK                        0x00000008
#define BCHP_BSCB_IIC_ENABLE_reserved1_SHIFT                       3

/* BSCB :: IIC_ENABLE :: NO_ACK [02:02] */
#define BCHP_BSCB_IIC_ENABLE_NO_ACK_MASK                           0x00000004
#define BCHP_BSCB_IIC_ENABLE_NO_ACK_SHIFT                          2
#define BCHP_BSCB_IIC_ENABLE_NO_ACK_DEFAULT                        0x00000000

/* BSCB :: IIC_ENABLE :: INTRP [01:01] */
#define BCHP_BSCB_IIC_ENABLE_INTRP_MASK                            0x00000002
#define BCHP_BSCB_IIC_ENABLE_INTRP_SHIFT                           1
#define BCHP_BSCB_IIC_ENABLE_INTRP_DEFAULT                         0x00000000

/* BSCB :: IIC_ENABLE :: ENABLE [00:00] */
#define BCHP_BSCB_IIC_ENABLE_ENABLE_MASK                           0x00000001
#define BCHP_BSCB_IIC_ENABLE_ENABLE_SHIFT                          0
#define BCHP_BSCB_IIC_ENABLE_ENABLE_DEFAULT                        0x00000000

/***************************************************************************
 *DATA_OUT0 - BSC Read Data Register 0
 ***************************************************************************/
/* BSCB :: DATA_OUT0 :: DATA_OUT0 [31:00] */
#define BCHP_BSCB_DATA_OUT0_DATA_OUT0_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT0_DATA_OUT0_SHIFT                        0

/***************************************************************************
 *DATA_OUT1 - BSC Read Data Register 1
 ***************************************************************************/
/* BSCB :: DATA_OUT1 :: DATA_OUT1 [31:00] */
#define BCHP_BSCB_DATA_OUT1_DATA_OUT1_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT1_DATA_OUT1_SHIFT                        0

/***************************************************************************
 *DATA_OUT2 - BSC Read Data Register 2
 ***************************************************************************/
/* BSCB :: DATA_OUT2 :: DATA_OUT2 [31:00] */
#define BCHP_BSCB_DATA_OUT2_DATA_OUT2_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT2_DATA_OUT2_SHIFT                        0

/***************************************************************************
 *DATA_OUT3 - BSC Read Data Register 3
 ***************************************************************************/
/* BSCB :: DATA_OUT3 :: DATA_OUT3 [31:00] */
#define BCHP_BSCB_DATA_OUT3_DATA_OUT3_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT3_DATA_OUT3_SHIFT                        0

/***************************************************************************
 *DATA_OUT4 - BSC Read Data Register 4
 ***************************************************************************/
/* BSCB :: DATA_OUT4 :: DATA_OUT4 [31:00] */
#define BCHP_BSCB_DATA_OUT4_DATA_OUT4_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT4_DATA_OUT4_SHIFT                        0

/***************************************************************************
 *DATA_OUT5 - BSC Read Data Register 5
 ***************************************************************************/
/* BSCB :: DATA_OUT5 :: DATA_OUT5 [31:00] */
#define BCHP_BSCB_DATA_OUT5_DATA_OUT5_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT5_DATA_OUT5_SHIFT                        0

/***************************************************************************
 *DATA_OUT6 - BSC Read Data Register 6
 ***************************************************************************/
/* BSCB :: DATA_OUT6 :: DATA_OUT6 [31:00] */
#define BCHP_BSCB_DATA_OUT6_DATA_OUT6_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT6_DATA_OUT6_SHIFT                        0

/***************************************************************************
 *DATA_OUT7 - BSC Read Data Register 7
 ***************************************************************************/
/* BSCB :: DATA_OUT7 :: DATA_OUT7 [31:00] */
#define BCHP_BSCB_DATA_OUT7_DATA_OUT7_MASK                         0xffffffff
#define BCHP_BSCB_DATA_OUT7_DATA_OUT7_SHIFT                        0

/***************************************************************************
 *CTLHI_REG - BSC Control Register
 ***************************************************************************/
/* BSCB :: CTLHI_REG :: reserved0 [31:10] */
#define BCHP_BSCB_CTLHI_REG_reserved0_MASK                         0xfffffc00
#define BCHP_BSCB_CTLHI_REG_reserved0_SHIFT                        10

/* BSCB :: CTLHI_REG :: reserved_for_eco1 [09:08] */
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco1_MASK                 0x00000300
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco1_SHIFT                8
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco1_DEFAULT              0x00000000

/* BSCB :: CTLHI_REG :: INPUT_SWITCHING_LEVEL [07:07] */
#define BCHP_BSCB_CTLHI_REG_INPUT_SWITCHING_LEVEL_MASK             0x00000080
#define BCHP_BSCB_CTLHI_REG_INPUT_SWITCHING_LEVEL_SHIFT            7
#define BCHP_BSCB_CTLHI_REG_INPUT_SWITCHING_LEVEL_DEFAULT          0x00000000

/* BSCB :: CTLHI_REG :: DATA_REG_SIZE [06:06] */
#define BCHP_BSCB_CTLHI_REG_DATA_REG_SIZE_MASK                     0x00000040
#define BCHP_BSCB_CTLHI_REG_DATA_REG_SIZE_SHIFT                    6
#define BCHP_BSCB_CTLHI_REG_DATA_REG_SIZE_DEFAULT                  0x00000000

/* BSCB :: CTLHI_REG :: reserved_for_eco2 [05:02] */
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco2_MASK                 0x0000003c
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco2_SHIFT                2
#define BCHP_BSCB_CTLHI_REG_reserved_for_eco2_DEFAULT              0x00000000

/* BSCB :: CTLHI_REG :: IGNORE_ACK [01:01] */
#define BCHP_BSCB_CTLHI_REG_IGNORE_ACK_MASK                        0x00000002
#define BCHP_BSCB_CTLHI_REG_IGNORE_ACK_SHIFT                       1
#define BCHP_BSCB_CTLHI_REG_IGNORE_ACK_DEFAULT                     0x00000000

/* BSCB :: CTLHI_REG :: WAIT_DIS [00:00] */
#define BCHP_BSCB_CTLHI_REG_WAIT_DIS_MASK                          0x00000001
#define BCHP_BSCB_CTLHI_REG_WAIT_DIS_SHIFT                         0
#define BCHP_BSCB_CTLHI_REG_WAIT_DIS_DEFAULT                       0x00000000

/***************************************************************************
 *SCL_PARAM - BSC SCL Parameter Register
 ***************************************************************************/
/* BSCB :: SCL_PARAM :: reserved_for_eco0 [31:00] */
#define BCHP_BSCB_SCL_PARAM_reserved_for_eco0_MASK                 0xffffffff
#define BCHP_BSCB_SCL_PARAM_reserved_for_eco0_SHIFT                0
#define BCHP_BSCB_SCL_PARAM_reserved_for_eco0_DEFAULT              0x00000000

#endif /* #ifndef BCHP_BSCB_H__ */

/* End of File */
