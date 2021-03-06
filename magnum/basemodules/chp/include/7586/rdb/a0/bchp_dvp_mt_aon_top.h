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
 * Date:           Generated on               Wed Feb 11 10:13:58 2015
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

#ifndef BCHP_DVP_MT_AON_TOP_H__
#define BCHP_DVP_MT_AON_TOP_H__

/***************************************************************************
 *DVP_MT_AON_TOP - Top-Level
 ***************************************************************************/
#define BCHP_DVP_MT_AON_TOP_SW_INIT              0x01583000 /* [RW] MHL AON CBUS SW-INIT */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL         0x01583004 /* [RW] MHL CBUS PHY Control Register */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL         0x01583008 /* [RW] RCAL PHY Control Register */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS 0x0158300c /* [RO] RCAL and CBUS PHY Status register */

/***************************************************************************
 *SW_INIT - MHL AON CBUS SW-INIT
 ***************************************************************************/
/* DVP_MT_AON_TOP :: SW_INIT :: reserved0 [31:05] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_reserved0_MASK                 0xffffffe0
#define BCHP_DVP_MT_AON_TOP_SW_INIT_reserved0_SHIFT                5

/* DVP_MT_AON_TOP :: SW_INIT :: DDC_REQUESTER [04:04] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_DDC_REQUESTER_MASK             0x00000010
#define BCHP_DVP_MT_AON_TOP_SW_INIT_DDC_REQUESTER_SHIFT            4
#define BCHP_DVP_MT_AON_TOP_SW_INIT_DDC_REQUESTER_DEFAULT          0x00000000

/* DVP_MT_AON_TOP :: SW_INIT :: MSC_RESPONDER [03:03] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_RESPONDER_MASK             0x00000008
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_RESPONDER_SHIFT            3
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_RESPONDER_DEFAULT          0x00000000

/* DVP_MT_AON_TOP :: SW_INIT :: MSC_REQUESTER [02:02] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_REQUESTER_MASK             0x00000004
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_REQUESTER_SHIFT            2
#define BCHP_DVP_MT_AON_TOP_SW_INIT_MSC_REQUESTER_DEFAULT          0x00000000

/* DVP_MT_AON_TOP :: SW_INIT :: CBUS [01:01] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_CBUS_MASK                      0x00000002
#define BCHP_DVP_MT_AON_TOP_SW_INIT_CBUS_SHIFT                     1
#define BCHP_DVP_MT_AON_TOP_SW_INIT_CBUS_DEFAULT                   0x00000000

/* DVP_MT_AON_TOP :: SW_INIT :: ALL [00:00] */
#define BCHP_DVP_MT_AON_TOP_SW_INIT_ALL_MASK                       0x00000001
#define BCHP_DVP_MT_AON_TOP_SW_INIT_ALL_SHIFT                      0
#define BCHP_DVP_MT_AON_TOP_SW_INIT_ALL_DEFAULT                    0x00000000

/***************************************************************************
 *CBUS_PHY_CTL - MHL CBUS PHY Control Register
 ***************************************************************************/
/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: reserved0 [31:15] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_reserved0_MASK            0xffff8000
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_reserved0_SHIFT           15

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: CBUS_RX_EN [14:14] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RX_EN_MASK           0x00004000
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RX_EN_SHIFT          14
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RX_EN_DEFAULT        0x00000001

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: CBUS_RZM_EN [13:13] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RZM_EN_MASK          0x00002000
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RZM_EN_SHIFT         13
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_RZM_EN_DEFAULT       0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: CBUS_IMP_DET_RST [12:12] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_IMP_DET_RST_MASK     0x00001000
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_IMP_DET_RST_SHIFT    12
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_IMP_DET_RST_DEFAULT  0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: CBUS_BYPASS_ENB [11:11] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_BYPASS_ENB_MASK      0x00000800
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_BYPASS_ENB_SHIFT     11
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CBUS_BYPASS_ENB_DEFAULT   0x00000001

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: OV_PROTECT_ENB [10:10] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_OV_PROTECT_ENB_MASK       0x00000400
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_OV_PROTECT_ENB_SHIFT      10
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_OV_PROTECT_ENB_DEFAULT    0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: MODE [09:08] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_MODE_MASK                 0x00000300
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_MODE_SHIFT                8
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_MODE_DEFAULT              0x00000001

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: RSVD_07_07 [07:07] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RSVD_07_07_MASK           0x00000080
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RSVD_07_07_SHIFT          7
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RSVD_07_07_DEFAULT        0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: RMON_RSEL [06:06] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RMON_RSEL_MASK            0x00000040
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RMON_RSEL_SHIFT           6
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RMON_RSEL_DEFAULT         0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: RCAL_RZM_BYPEN [05:05] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_BYPEN_MASK       0x00000020
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_BYPEN_SHIFT      5
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_BYPEN_DEFAULT    0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: RCAL_RZM_IN [04:03] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_IN_MASK          0x00000018
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_IN_SHIFT         3
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_RCAL_RZM_IN_DEFAULT       0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: HPD_PD_TOGGLE [02:02] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_PD_TOGGLE_MASK        0x00000004
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_PD_TOGGLE_SHIFT       2
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_PD_TOGGLE_DEFAULT     0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: CEC_PU_TOGGLE [01:01] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CEC_PU_TOGGLE_MASK        0x00000002
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CEC_PU_TOGGLE_SHIFT       1
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_CEC_PU_TOGGLE_DEFAULT     0x00000000

/* DVP_MT_AON_TOP :: CBUS_PHY_CTL :: HPD_CBUSB_TOGGLE [00:00] */
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_CBUSB_TOGGLE_MASK     0x00000001
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_CBUSB_TOGGLE_SHIFT    0
#define BCHP_DVP_MT_AON_TOP_CBUS_PHY_CTL_HPD_CBUSB_TOGGLE_DEFAULT  0x00000000

/***************************************************************************
 *RCAL_PHY_CTL - RCAL PHY Control Register
 ***************************************************************************/
/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: reserved0 [31:16] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_reserved0_MASK            0xffff0000
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_reserved0_SHIFT           16

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: POWER_DOWN [15:15] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_POWER_DOWN_MASK           0x00008000
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_POWER_DOWN_SHIFT          15
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_POWER_DOWN_DEFAULT        0x00000001

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: RESETB [14:14] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RESETB_MASK               0x00004000
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RESETB_SHIFT              14
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RESETB_DEFAULT            0x00000001

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: BYPASS_VALUE [13:09] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_VALUE_MASK         0x00003e00
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_VALUE_SHIFT        9
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_VALUE_DEFAULT      0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: BYPASS_EN [08:08] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_EN_MASK            0x00000100
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_EN_SHIFT           8
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_BYPASS_EN_DEFAULT         0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: RSVD_07_06 [07:06] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RSVD_07_06_MASK           0x000000c0
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RSVD_07_06_SHIFT          6
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_RSVD_07_06_DEFAULT        0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: TEST_RATIO [05:05] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TEST_RATIO_MASK           0x00000020
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TEST_RATIO_SHIFT          5
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TEST_RATIO_DEFAULT        0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: TESTEN [04:04] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TESTEN_MASK               0x00000010
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TESTEN_SHIFT              4
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_TESTEN_DEFAULT            0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: SEL_AVG [03:02] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_AVG_MASK              0x0000000c
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_AVG_SHIFT             2
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_AVG_DEFAULT           0x00000000

/* DVP_MT_AON_TOP :: RCAL_PHY_CTL :: SEL_DIV [01:00] */
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_DIV_MASK              0x00000003
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_DIV_SHIFT             0
#define BCHP_DVP_MT_AON_TOP_RCAL_PHY_CTL_SEL_DIV_DEFAULT           0x00000000

/***************************************************************************
 *RCAL_CBUS_PHY_STATUS - RCAL and CBUS PHY Status register
 ***************************************************************************/
/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: reserved0 [31:26] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_reserved0_MASK    0xfc000000
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_reserved0_SHIFT   26

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: CBUS_IMP_CHANGE [25:25] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CBUS_IMP_CHANGE_MASK 0x02000000
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CBUS_IMP_CHANGE_SHIFT 25

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: CBUS_ZOK [24:24] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CBUS_ZOK_MASK     0x01000000
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CBUS_ZOK_SHIFT    24

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: RMON_SEL [23:21] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RMON_SEL_MASK     0x00e00000
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RMON_SEL_SHIFT    21

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: CALCODE [20:16] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CALCODE_MASK      0x001f0000
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_CALCODE_SHIFT     16

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: reserved1 [15:09] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_reserved1_MASK    0x0000fe00
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_reserved1_SHIFT   9

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: RCAL_DONE [08:08] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RCAL_DONE_MASK    0x00000100
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RCAL_DONE_SHIFT   8

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: RCAL_RSVD_7 [07:07] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RCAL_RSVD_7_MASK  0x00000080
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_RCAL_RSVD_7_SHIFT 7

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: STRT_AVG [06:06] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_STRT_AVG_MASK     0x00000040
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_STRT_AVG_SHIFT    6

/* DVP_MT_AON_TOP :: RCAL_CBUS_PHY_STATUS :: INT_CALCODE [05:00] */
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_INT_CALCODE_MASK  0x0000003f
#define BCHP_DVP_MT_AON_TOP_RCAL_CBUS_PHY_STATUS_INT_CALCODE_SHIFT 0

#endif /* #ifndef BCHP_DVP_MT_AON_TOP_H__ */

/* End of File */
