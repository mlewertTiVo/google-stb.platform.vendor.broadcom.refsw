/***************************************************************************
 *     Copyright (c) 1999-2012, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Thu Apr 19 15:04:14 2012
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#ifndef BCHP_DS_WFE_OBA_H__
#define BCHP_DS_WFE_OBA_H__

/***************************************************************************
 *DS_WFE_OBA - WFE OBADC Control Registers
 ***************************************************************************/
#define BCHP_DS_WFE_OBA_CNTL0                    0x00e0f000 /* cntl0 */
#define BCHP_DS_WFE_OBA_CNTL1                    0x00e0f004 /* cntl1 */
#define BCHP_DS_WFE_OBA_CNTL2                    0x00e0f008 /* cntl2 */
#define BCHP_DS_WFE_OBA_CNTL3                    0x00e0f00c /* cntl3 */

/***************************************************************************
 *CNTL0 - cntl0
 ***************************************************************************/
/* DS_WFE_OBA :: CNTL0 :: Spare0 [31:28] */
#define BCHP_DS_WFE_OBA_CNTL0_Spare0_MASK                          0xf0000000
#define BCHP_DS_WFE_OBA_CNTL0_Spare0_SHIFT                         28
#define BCHP_DS_WFE_OBA_CNTL0_Spare0_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL0 :: RTERM [27:25] */
#define BCHP_DS_WFE_OBA_CNTL0_RTERM_MASK                           0x0e000000
#define BCHP_DS_WFE_OBA_CNTL0_RTERM_SHIFT                          25
#define BCHP_DS_WFE_OBA_CNTL0_RTERM_DEFAULT                        0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_LESS_LAT [24:24] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LESS_LAT_MASK                    0x01000000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LESS_LAT_SHIFT                   24
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LESS_LAT_DEFAULT                 0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_FORMAT_MSB [23:23] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_MSB_MASK                  0x00800000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_MSB_SHIFT                 23
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_MSB_DEFAULT               0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_FL_DLY [22:22] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_DLY_MASK                      0x00400000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_DLY_SHIFT                     22
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_DLY_DEFAULT                   0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC3_DLY [21:21] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_DLY_MASK                   0x00200000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_DLY_SHIFT                  21
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_DLY_DEFAULT                0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC2_DLY [20:20] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_DLY_MASK                   0x00100000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_DLY_SHIFT                  20
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_DLY_DEFAULT                0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC1_DLY [19:19] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_DLY_MASK                   0x00080000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_DLY_SHIFT                  19
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_DLY_DEFAULT                0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_FL_EDGE [18:18] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_EDGE_MASK                     0x00040000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_EDGE_SHIFT                    18
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FL_EDGE_DEFAULT                  0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC3_EDGE [17:17] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_EDGE_MASK                  0x00020000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_EDGE_SHIFT                 17
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC3_EDGE_DEFAULT               0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC2_EDGE [16:16] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_EDGE_MASK                  0x00010000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_EDGE_SHIFT                 16
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC2_EDGE_DEFAULT               0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MDAC1_EDGE [15:15] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_EDGE_MASK                  0x00008000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_EDGE_SHIFT                 15
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MDAC1_EDGE_DEFAULT               0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_SATBYP [14:14] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_SATBYP_MASK                      0x00004000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_SATBYP_SHIFT                     14
#define BCHP_DS_WFE_OBA_CNTL0_DGS_SATBYP_DEFAULT                   0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_FORMAT_LSB [13:13] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_LSB_MASK                  0x00002000
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_LSB_SHIFT                 13
#define BCHP_DS_WFE_OBA_CNTL0_DGS_FORMAT_LSB_DEFAULT               0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_DIVCNTL [12:11] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DIVCNTL_MASK                     0x00001800
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DIVCNTL_SHIFT                    11
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DIVCNTL_DEFAULT                  0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_LSFR_MODE [10:10] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LSFR_MODE_MASK                   0x00000400
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LSFR_MODE_SHIFT                  10
#define BCHP_DS_WFE_OBA_CNTL0_DGS_LSFR_MODE_DEFAULT                0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_DMUX_CLKINV [09:09] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKINV_MASK                 0x00000200
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKINV_SHIFT                9
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKINV_DEFAULT              0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_DMUX_CLKDIS [08:08] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKDIS_MASK                 0x00000100
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKDIS_SHIFT                8
#define BCHP_DS_WFE_OBA_CNTL0_DGS_DMUX_CLKDIS_DEFAULT              0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_RST [07:07] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_RST_MASK                         0x00000080
#define BCHP_DS_WFE_OBA_CNTL0_DGS_RST_SHIFT                        7
#define BCHP_DS_WFE_OBA_CNTL0_DGS_RST_DEFAULT                      0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_NC [06:04] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_NC_MASK                          0x00000070
#define BCHP_DS_WFE_OBA_CNTL0_DGS_NC_SHIFT                         4
#define BCHP_DS_WFE_OBA_CNTL0_DGS_NC_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_MASK [03:02] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MASK_MASK                        0x0000000c
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MASK_SHIFT                       2
#define BCHP_DS_WFE_OBA_CNTL0_DGS_MASK_DEFAULT                     0x00000000

/* DS_WFE_OBA :: CNTL0 :: DGS_OBTC [01:01] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OBTC_MASK                        0x00000002
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OBTC_SHIFT                       1
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OBTC_DEFAULT                     0x00000001

/* DS_WFE_OBA :: CNTL0 :: DGS_OUTCLK_INV [00:00] */
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OUTCLK_INV_MASK                  0x00000001
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OUTCLK_INV_SHIFT                 0
#define BCHP_DS_WFE_OBA_CNTL0_DGS_OUTCLK_INV_DEFAULT               0x00000000

/***************************************************************************
 *CNTL1 - cntl1
 ***************************************************************************/
/* DS_WFE_OBA :: CNTL1 :: AMP_CM_CNTL [31:30] */
#define BCHP_DS_WFE_OBA_CNTL1_AMP_CM_CNTL_MASK                     0xc0000000
#define BCHP_DS_WFE_OBA_CNTL1_AMP_CM_CNTL_SHIFT                    30
#define BCHP_DS_WFE_OBA_CNTL1_AMP_CM_CNTL_DEFAULT                  0x00000001

/* DS_WFE_OBA :: CNTL1 :: IMDAC3 [29:27] */
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC3_MASK                          0x38000000
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC3_SHIFT                         27
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC3_DEFAULT                       0x00000003

/* DS_WFE_OBA :: CNTL1 :: IAMBIASCMI [26:24] */
#define BCHP_DS_WFE_OBA_CNTL1_IAMBIASCMI_MASK                      0x07000000
#define BCHP_DS_WFE_OBA_CNTL1_IAMBIASCMI_SHIFT                     24
#define BCHP_DS_WFE_OBA_CNTL1_IAMBIASCMI_DEFAULT                   0x00000003

/* DS_WFE_OBA :: CNTL1 :: IMDAC2 [23:21] */
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC2_MASK                          0x00e00000
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC2_SHIFT                         21
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC2_DEFAULT                       0x00000003

/* DS_WFE_OBA :: CNTL1 :: IMDAC1 [20:18] */
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC1_MASK                          0x001c0000
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC1_SHIFT                         18
#define BCHP_DS_WFE_OBA_CNTL1_IMDAC1_DEFAULT                       0x00000003

/* DS_WFE_OBA :: CNTL1 :: ISHA [17:15] */
#define BCHP_DS_WFE_OBA_CNTL1_ISHA_MASK                            0x00038000
#define BCHP_DS_WFE_OBA_CNTL1_ISHA_SHIFT                           15
#define BCHP_DS_WFE_OBA_CNTL1_ISHA_DEFAULT                         0x00000003

/* DS_WFE_OBA :: CNTL1 :: IBUF [14:12] */
#define BCHP_DS_WFE_OBA_CNTL1_IBUF_MASK                            0x00007000
#define BCHP_DS_WFE_OBA_CNTL1_IBUF_SHIFT                           12
#define BCHP_DS_WFE_OBA_CNTL1_IBUF_DEFAULT                         0x00000003

/* DS_WFE_OBA :: CNTL1 :: IVCMBUF [11:10] */
#define BCHP_DS_WFE_OBA_CNTL1_IVCMBUF_MASK                         0x00000c00
#define BCHP_DS_WFE_OBA_CNTL1_IVCMBUF_SHIFT                        10
#define BCHP_DS_WFE_OBA_CNTL1_IVCMBUF_DEFAULT                      0x00000001

/* DS_WFE_OBA :: CNTL1 :: IREFGENB [09:08] */
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENB_MASK                        0x00000300
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENB_SHIFT                       8
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENB_DEFAULT                     0x00000001

/* DS_WFE_OBA :: CNTL1 :: IREFGENT [07:06] */
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENT_MASK                        0x000000c0
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENT_SHIFT                       6
#define BCHP_DS_WFE_OBA_CNTL1_IREFGENT_DEFAULT                     0x00000001

/* DS_WFE_OBA :: CNTL1 :: IFLASH_MDAC23 [05:04] */
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC23_MASK                   0x00000030
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC23_SHIFT                  4
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC23_DEFAULT                0x00000001

/* DS_WFE_OBA :: CNTL1 :: IFLASH_MDAC1 [03:02] */
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC1_MASK                    0x0000000c
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC1_SHIFT                   2
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH_MDAC1_DEFAULT                 0x00000001

/* DS_WFE_OBA :: CNTL1 :: IFLASH6 [01:00] */
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH6_MASK                         0x00000003
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH6_SHIFT                        0
#define BCHP_DS_WFE_OBA_CNTL1_IFLASH6_DEFAULT                      0x00000001

/***************************************************************************
 *CNTL2 - cntl2
 ***************************************************************************/
/* DS_WFE_OBA :: CNTL2 :: FLASH6_TCNTL [31:30] */
#define BCHP_DS_WFE_OBA_CNTL2_FLASH6_TCNTL_MASK                    0xc0000000
#define BCHP_DS_WFE_OBA_CNTL2_FLASH6_TCNTL_SHIFT                   30
#define BCHP_DS_WFE_OBA_CNTL2_FLASH6_TCNTL_DEFAULT                 0x00000003

/* DS_WFE_OBA :: CNTL2 :: MDAC23_TCNTL [29:28] */
#define BCHP_DS_WFE_OBA_CNTL2_MDAC23_TCNTL_MASK                    0x30000000
#define BCHP_DS_WFE_OBA_CNTL2_MDAC23_TCNTL_SHIFT                   28
#define BCHP_DS_WFE_OBA_CNTL2_MDAC23_TCNTL_DEFAULT                 0x00000001

/* DS_WFE_OBA :: CNTL2 :: VBUFCOM_SEL [27:26] */
#define BCHP_DS_WFE_OBA_CNTL2_VBUFCOM_SEL_MASK                     0x0c000000
#define BCHP_DS_WFE_OBA_CNTL2_VBUFCOM_SEL_SHIFT                    26
#define BCHP_DS_WFE_OBA_CNTL2_VBUFCOM_SEL_DEFAULT                  0x00000001

/* DS_WFE_OBA :: CNTL2 :: LDO_VSS05_OFFSET [25:24] */
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VSS05_OFFSET_MASK                0x03000000
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VSS05_OFFSET_SHIFT               24
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VSS05_OFFSET_DEFAULT             0x00000002

/* DS_WFE_OBA :: CNTL2 :: LDO_VDD15DIFF [23:22] */
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VDD15DIFF_MASK                   0x00c00000
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VDD15DIFF_SHIFT                  22
#define BCHP_DS_WFE_OBA_CNTL2_LDO_VDD15DIFF_DEFAULT                0x00000001

/* DS_WFE_OBA :: CNTL2 :: TESTSEL [21:19] */
#define BCHP_DS_WFE_OBA_CNTL2_TESTSEL_MASK                         0x00380000
#define BCHP_DS_WFE_OBA_CNTL2_TESTSEL_SHIFT                        19
#define BCHP_DS_WFE_OBA_CNTL2_TESTSEL_DEFAULT                      0x00000000

/* DS_WFE_OBA :: CNTL2 :: Spare1 [18:18] */
#define BCHP_DS_WFE_OBA_CNTL2_Spare1_MASK                          0x00040000
#define BCHP_DS_WFE_OBA_CNTL2_Spare1_SHIFT                         18
#define BCHP_DS_WFE_OBA_CNTL2_Spare1_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL2 :: MDAC1_TCNTL [17:16] */
#define BCHP_DS_WFE_OBA_CNTL2_MDAC1_TCNTL_MASK                     0x00030000
#define BCHP_DS_WFE_OBA_CNTL2_MDAC1_TCNTL_SHIFT                    16
#define BCHP_DS_WFE_OBA_CNTL2_MDAC1_TCNTL_DEFAULT                  0x00000001

/* DS_WFE_OBA :: CNTL2 :: TNON_CNTL [15:13] */
#define BCHP_DS_WFE_OBA_CNTL2_TNON_CNTL_MASK                       0x0000e000
#define BCHP_DS_WFE_OBA_CNTL2_TNON_CNTL_SHIFT                      13
#define BCHP_DS_WFE_OBA_CNTL2_TNON_CNTL_DEFAULT                    0x00000004

/* DS_WFE_OBA :: CNTL2 :: BG_CTAT [12:11] */
#define BCHP_DS_WFE_OBA_CNTL2_BG_CTAT_MASK                         0x00001800
#define BCHP_DS_WFE_OBA_CNTL2_BG_CTAT_SHIFT                        11
#define BCHP_DS_WFE_OBA_CNTL2_BG_CTAT_DEFAULT                      0x00000000

/* DS_WFE_OBA :: CNTL2 :: BG_PTAT [10:09] */
#define BCHP_DS_WFE_OBA_CNTL2_BG_PTAT_MASK                         0x00000600
#define BCHP_DS_WFE_OBA_CNTL2_BG_PTAT_SHIFT                        9
#define BCHP_DS_WFE_OBA_CNTL2_BG_PTAT_DEFAULT                      0x00000000

/* DS_WFE_OBA :: CNTL2 :: ILDO [08:06] */
#define BCHP_DS_WFE_OBA_CNTL2_ILDO_MASK                            0x000001c0
#define BCHP_DS_WFE_OBA_CNTL2_ILDO_SHIFT                           6
#define BCHP_DS_WFE_OBA_CNTL2_ILDO_DEFAULT                         0x00000003

/* DS_WFE_OBA :: CNTL2 :: IAMPBIAS [05:03] */
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIAS_MASK                        0x00000038
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIAS_SHIFT                       3
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIAS_DEFAULT                     0x00000003

/* DS_WFE_OBA :: CNTL2 :: IAMPBIASNB1 [02:00] */
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIASNB1_MASK                     0x00000007
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIASNB1_SHIFT                    0
#define BCHP_DS_WFE_OBA_CNTL2_IAMPBIASNB1_DEFAULT                  0x00000003

/***************************************************************************
 *CNTL3 - cntl3
 ***************************************************************************/
/* DS_WFE_OBA :: CNTL3 :: RSTB [31:31] */
#define BCHP_DS_WFE_OBA_CNTL3_RSTB_MASK                            0x80000000
#define BCHP_DS_WFE_OBA_CNTL3_RSTB_SHIFT                           31
#define BCHP_DS_WFE_OBA_CNTL3_RSTB_DEFAULT                         0x00000001

/* DS_WFE_OBA :: CNTL3 :: PWRUP [30:30] */
#define BCHP_DS_WFE_OBA_CNTL3_PWRUP_MASK                           0x40000000
#define BCHP_DS_WFE_OBA_CNTL3_PWRUP_SHIFT                          30
#define BCHP_DS_WFE_OBA_CNTL3_PWRUP_DEFAULT                        0x00000000

/* DS_WFE_OBA :: CNTL3 :: Spare2 [29:27] */
#define BCHP_DS_WFE_OBA_CNTL3_Spare2_MASK                          0x38000000
#define BCHP_DS_WFE_OBA_CNTL3_Spare2_SHIFT                         27
#define BCHP_DS_WFE_OBA_CNTL3_Spare2_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL3 :: TESTEN [26:26] */
#define BCHP_DS_WFE_OBA_CNTL3_TESTEN_MASK                          0x04000000
#define BCHP_DS_WFE_OBA_CNTL3_TESTEN_SHIFT                         26
#define BCHP_DS_WFE_OBA_CNTL3_TESTEN_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL3 :: Spare3 [25:20] */
#define BCHP_DS_WFE_OBA_CNTL3_Spare3_MASK                          0x03f00000
#define BCHP_DS_WFE_OBA_CNTL3_Spare3_SHIFT                         20
#define BCHP_DS_WFE_OBA_CNTL3_Spare3_DEFAULT                       0x00000000

/* DS_WFE_OBA :: CNTL3 :: CLK_DET [19:19] */
#define BCHP_DS_WFE_OBA_CNTL3_CLK_DET_MASK                         0x00080000
#define BCHP_DS_WFE_OBA_CNTL3_CLK_DET_SHIFT                        19
#define BCHP_DS_WFE_OBA_CNTL3_CLK_DET_DEFAULT                      0x00000000

/* DS_WFE_OBA :: CNTL3 :: CLK_PHASE [18:17] */
#define BCHP_DS_WFE_OBA_CNTL3_CLK_PHASE_MASK                       0x00060000
#define BCHP_DS_WFE_OBA_CNTL3_CLK_PHASE_SHIFT                      17
#define BCHP_DS_WFE_OBA_CNTL3_CLK_PHASE_DEFAULT                    0x00000000

/* DS_WFE_OBA :: CNTL3 :: CLK_RATE [16:16] */
#define BCHP_DS_WFE_OBA_CNTL3_CLK_RATE_MASK                        0x00010000
#define BCHP_DS_WFE_OBA_CNTL3_CLK_RATE_SHIFT                       16
#define BCHP_DS_WFE_OBA_CNTL3_CLK_RATE_DEFAULT                     0x00000000

/* DS_WFE_OBA :: CNTL3 :: Spare4 [15:00] */
#define BCHP_DS_WFE_OBA_CNTL3_Spare4_MASK                          0x0000ffff
#define BCHP_DS_WFE_OBA_CNTL3_Spare4_SHIFT                         0
#define BCHP_DS_WFE_OBA_CNTL3_Spare4_DEFAULT                       0x00000000

#endif /* #ifndef BCHP_DS_WFE_OBA_H__ */

/* End of File */
