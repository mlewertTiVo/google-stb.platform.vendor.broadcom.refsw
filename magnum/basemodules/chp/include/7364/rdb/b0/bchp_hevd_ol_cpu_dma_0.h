/****************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Wed Apr  1 11:23:11 2015
 *                 Full Compile MD5 Checksum  267f8e92d9b43928c0a06f1ab29c511c
 *                     (minus title and desc)
 *                 MD5 Checksum               0548f7f0a8e20364fd383a7aa29c0b86
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15956
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_HEVD_OL_CPU_DMA_0_H__
#define BCHP_HEVD_OL_CPU_DMA_0_H__

/***************************************************************************
 *HEVD_OL_CPU_DMA_0
 ***************************************************************************/
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_SD_ADDR       0x00100400 /* [RW] DMA 0 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR      0x00100404 /* [RW] DMA 0 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN           0x00100408 /* [RW] DMA 0 Length */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_SD_ADDR       0x00100410 /* [RW] DMA 1 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR      0x00100414 /* [RW] DMA 1 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN           0x00100418 /* [RW] DMA 1 Length */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_SD_ADDR       0x00100420 /* [RW] DMA 2 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR      0x00100424 /* [RW] DMA 2 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN           0x00100428 /* [RW] DMA 2 Length */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_SD_ADDR       0x00100430 /* [RW] DMA 3 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR      0x00100434 /* [RW] DMA 3 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN           0x00100438 /* [RW] DMA 3 Length */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS            0x00100440 /* [RO] REG_DMA_STATUS */

/***************************************************************************
 *CH0_SD_ADDR - DMA 0 SDRAM address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH0_SD_ADDR :: Sd_Addr [31:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_SD_ADDR_Sd_Addr_MASK            0xfffffffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_SD_ADDR_Sd_Addr_SHIFT           2

/* HEVD_OL_CPU_DMA_0 :: CH0_SD_ADDR :: reserved0 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_SD_ADDR_reserved0_MASK          0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_SD_ADDR_reserved0_SHIFT         0

/***************************************************************************
 *CH0_LCL_ADDR - DMA 0 Local Memory Address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH0_LCL_ADDR :: reserved0 [31:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_reserved0_MASK         0xffffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_reserved0_SHIFT        14

/* HEVD_OL_CPU_DMA_0 :: CH0_LCL_ADDR :: Addr [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_Addr_MASK              0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_Addr_SHIFT             2

/* HEVD_OL_CPU_DMA_0 :: CH0_LCL_ADDR :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_reserved1_MASK         0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LCL_ADDR_reserved1_SHIFT        0

/***************************************************************************
 *CH0_LEN - DMA 0 Length
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: Swap [31:31] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Swap_MASK                   0x80000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Swap_SHIFT                  31

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: Alt [30:30] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Alt_MASK                    0x40000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Alt_SHIFT                   30

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: Direction [29:28] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Direction_MASK              0x30000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Direction_SHIFT             28

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: NoInc [27:27] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_NoInc_MASK                  0x08000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_NoInc_SHIFT                 27

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: reserved0 [26:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_reserved0_MASK              0x07ffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_reserved0_SHIFT             14

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: Length [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Length_MASK                 0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_Length_SHIFT                2

/* HEVD_OL_CPU_DMA_0 :: CH0_LEN :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_reserved1_MASK              0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH0_LEN_reserved1_SHIFT             0

/***************************************************************************
 *CH1_SD_ADDR - DMA 1 SDRAM address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH1_SD_ADDR :: Sd_Addr [31:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_SD_ADDR_Sd_Addr_MASK            0xfffffffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_SD_ADDR_Sd_Addr_SHIFT           2

/* HEVD_OL_CPU_DMA_0 :: CH1_SD_ADDR :: reserved0 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_SD_ADDR_reserved0_MASK          0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_SD_ADDR_reserved0_SHIFT         0

/***************************************************************************
 *CH1_LCL_ADDR - DMA 1 Local Memory Address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH1_LCL_ADDR :: reserved0 [31:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_reserved0_MASK         0xffffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_reserved0_SHIFT        14

/* HEVD_OL_CPU_DMA_0 :: CH1_LCL_ADDR :: Addr [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_Addr_MASK              0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_Addr_SHIFT             2

/* HEVD_OL_CPU_DMA_0 :: CH1_LCL_ADDR :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_reserved1_MASK         0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LCL_ADDR_reserved1_SHIFT        0

/***************************************************************************
 *CH1_LEN - DMA 1 Length
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: Swap [31:31] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Swap_MASK                   0x80000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Swap_SHIFT                  31

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: Alt [30:30] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Alt_MASK                    0x40000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Alt_SHIFT                   30

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: Direction [29:28] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Direction_MASK              0x30000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Direction_SHIFT             28

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: NoInc [27:27] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_NoInc_MASK                  0x08000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_NoInc_SHIFT                 27

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: reserved0 [26:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_reserved0_MASK              0x07ffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_reserved0_SHIFT             14

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: Length [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Length_MASK                 0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_Length_SHIFT                2

/* HEVD_OL_CPU_DMA_0 :: CH1_LEN :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_reserved1_MASK              0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH1_LEN_reserved1_SHIFT             0

/***************************************************************************
 *CH2_SD_ADDR - DMA 2 SDRAM address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH2_SD_ADDR :: Sd_Addr [31:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_SD_ADDR_Sd_Addr_MASK            0xfffffffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_SD_ADDR_Sd_Addr_SHIFT           2

/* HEVD_OL_CPU_DMA_0 :: CH2_SD_ADDR :: reserved0 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_SD_ADDR_reserved0_MASK          0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_SD_ADDR_reserved0_SHIFT         0

/***************************************************************************
 *CH2_LCL_ADDR - DMA 2 Local Memory Address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH2_LCL_ADDR :: reserved0 [31:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_reserved0_MASK         0xffffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_reserved0_SHIFT        14

/* HEVD_OL_CPU_DMA_0 :: CH2_LCL_ADDR :: Addr [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_Addr_MASK              0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_Addr_SHIFT             2

/* HEVD_OL_CPU_DMA_0 :: CH2_LCL_ADDR :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_reserved1_MASK         0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LCL_ADDR_reserved1_SHIFT        0

/***************************************************************************
 *CH2_LEN - DMA 2 Length
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: Swap [31:31] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Swap_MASK                   0x80000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Swap_SHIFT                  31

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: Alt [30:30] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Alt_MASK                    0x40000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Alt_SHIFT                   30

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: Direction [29:28] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Direction_MASK              0x30000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Direction_SHIFT             28

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: NoInc [27:27] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_NoInc_MASK                  0x08000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_NoInc_SHIFT                 27

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: reserved0 [26:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_reserved0_MASK              0x07ffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_reserved0_SHIFT             14

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: Length [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Length_MASK                 0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_Length_SHIFT                2

/* HEVD_OL_CPU_DMA_0 :: CH2_LEN :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_reserved1_MASK              0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH2_LEN_reserved1_SHIFT             0

/***************************************************************************
 *CH3_SD_ADDR - DMA 3 SDRAM address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH3_SD_ADDR :: Sd_Addr [31:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_SD_ADDR_Sd_Addr_MASK            0xfffffffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_SD_ADDR_Sd_Addr_SHIFT           2

/* HEVD_OL_CPU_DMA_0 :: CH3_SD_ADDR :: reserved0 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_SD_ADDR_reserved0_MASK          0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_SD_ADDR_reserved0_SHIFT         0

/***************************************************************************
 *CH3_LCL_ADDR - DMA 3 Local Memory Address
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH3_LCL_ADDR :: reserved0 [31:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_reserved0_MASK         0xffffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_reserved0_SHIFT        14

/* HEVD_OL_CPU_DMA_0 :: CH3_LCL_ADDR :: Addr [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_Addr_MASK              0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_Addr_SHIFT             2

/* HEVD_OL_CPU_DMA_0 :: CH3_LCL_ADDR :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_reserved1_MASK         0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LCL_ADDR_reserved1_SHIFT        0

/***************************************************************************
 *CH3_LEN - DMA 3 Length
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: Swap [31:31] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Swap_MASK                   0x80000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Swap_SHIFT                  31

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: Alt [30:30] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Alt_MASK                    0x40000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Alt_SHIFT                   30

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: Direction [29:28] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Direction_MASK              0x30000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Direction_SHIFT             28

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: NoInc [27:27] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_NoInc_MASK                  0x08000000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_NoInc_SHIFT                 27

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: reserved0 [26:14] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_reserved0_MASK              0x07ffc000
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_reserved0_SHIFT             14

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: Length [13:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Length_MASK                 0x00003ffc
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_Length_SHIFT                2

/* HEVD_OL_CPU_DMA_0 :: CH3_LEN :: reserved1 [01:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_reserved1_MASK              0x00000003
#define BCHP_HEVD_OL_CPU_DMA_0_CH3_LEN_reserved1_SHIFT             0

/***************************************************************************
 *STATUS - REG_DMA_STATUS
 ***************************************************************************/
/* HEVD_OL_CPU_DMA_0 :: STATUS :: reserved0 [31:04] */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_reserved0_MASK               0xfffffff0
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_reserved0_SHIFT              4

/* HEVD_OL_CPU_DMA_0 :: STATUS :: Act3 [03:03] */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act3_MASK                    0x00000008
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act3_SHIFT                   3

/* HEVD_OL_CPU_DMA_0 :: STATUS :: Act2 [02:02] */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act2_MASK                    0x00000004
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act2_SHIFT                   2

/* HEVD_OL_CPU_DMA_0 :: STATUS :: Act1 [01:01] */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act1_MASK                    0x00000002
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act1_SHIFT                   1

/* HEVD_OL_CPU_DMA_0 :: STATUS :: Act0 [00:00] */
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act0_MASK                    0x00000001
#define BCHP_HEVD_OL_CPU_DMA_0_STATUS_Act0_SHIFT                   0

#endif /* #ifndef BCHP_HEVD_OL_CPU_DMA_0_H__ */

/* End of File */
