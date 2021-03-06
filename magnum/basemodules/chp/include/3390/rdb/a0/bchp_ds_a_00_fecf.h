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
 * Date:           Generated on               Mon Sep 15 10:12:25 2014
 *                 Full Compile MD5 Checksum  ef22086ebd4065e4fea50dbc64f17e5e
 *                     (minus title and desc)
 *                 MD5 Checksum               39fcae49037a6337517df43bfc24b21f
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     14796
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_DS_A_00_FECF_H__
#define BCHP_DS_A_00_FECF_H__

/***************************************************************************
 *DS_A_00_FECF - Downstream FEC Frond End Registers
 ***************************************************************************/
#define BCHP_DS_A_00_FECF_CTL1                   0x02220c00 /* [RW] FEC Control Register 1 */
#define BCHP_DS_A_00_FECF_CTL2                   0x02220c04 /* [RW] FEC Control Register */
#define BCHP_DS_A_00_FECF_LOCK_CTL1              0x02220c08 /* [RW] FEC Lock/Unlock Detection Control Register1 */
#define BCHP_DS_A_00_FECF_LOCK_CTL2              0x02220c0c /* [RW] FEC Lock/Unlock Detection Control Register2 */
#define BCHP_DS_A_00_FECF_LOCK_CTL3              0x02220c10 /* [RW] FEC Lock/Unlock Detection Control Register3 */
#define BCHP_DS_A_00_FECF_STATUS                 0x02220c14 /* [RW] FEC Status Register */

/***************************************************************************
 *CTL1 - FEC Control Register 1
 ***************************************************************************/
/* DS_A_00_FECF :: CTL1 :: ECO_SPARE_2 [31:24] */
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_2_MASK                    0xff000000
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_2_SHIFT                   24
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_2_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL1 :: ECO_SPARE_1 [23:21] */
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_1_MASK                    0x00e00000
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_1_SHIFT                   21
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_1_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL1 :: EMAX_A [20:16] */
#define BCHP_DS_A_00_FECF_CTL1_EMAX_A_MASK                         0x001f0000
#define BCHP_DS_A_00_FECF_CTL1_EMAX_A_SHIFT                        16
#define BCHP_DS_A_00_FECF_CTL1_EMAX_A_DEFAULT                      0x00000000

/* DS_A_00_FECF :: CTL1 :: ID_B [15:12] */
#define BCHP_DS_A_00_FECF_CTL1_ID_B_MASK                           0x0000f000
#define BCHP_DS_A_00_FECF_CTL1_ID_B_SHIFT                          12
#define BCHP_DS_A_00_FECF_CTL1_ID_B_DEFAULT                        0x00000000

/* DS_A_00_FECF :: CTL1 :: ECO_SPARE_0 [11:11] */
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_0_MASK                    0x00000800
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_0_SHIFT                   11
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_0_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL1 :: AUTO_B [10:10] */
#define BCHP_DS_A_00_FECF_CTL1_AUTO_B_MASK                         0x00000400
#define BCHP_DS_A_00_FECF_CTL1_AUTO_B_SHIFT                        10
#define BCHP_DS_A_00_FECF_CTL1_AUTO_B_DEFAULT                      0x00000000

/* DS_A_00_FECF :: CTL1 :: ECO_SPARE_4 [09:09] */
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_4_MASK                    0x00000200
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_4_SHIFT                   9
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_4_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL1 :: SPINV [08:08] */
#define BCHP_DS_A_00_FECF_CTL1_SPINV_MASK                          0x00000100
#define BCHP_DS_A_00_FECF_CTL1_SPINV_SHIFT                         8
#define BCHP_DS_A_00_FECF_CTL1_SPINV_DEFAULT                       0x00000000

/* DS_A_00_FECF :: CTL1 :: BPS [07:05] */
#define BCHP_DS_A_00_FECF_CTL1_BPS_MASK                            0x000000e0
#define BCHP_DS_A_00_FECF_CTL1_BPS_SHIFT                           5
#define BCHP_DS_A_00_FECF_CTL1_BPS_DEFAULT                         0x00000000

/* DS_A_00_FECF :: CTL1 :: ANNEX [04:04] */
#define BCHP_DS_A_00_FECF_CTL1_ANNEX_MASK                          0x00000010
#define BCHP_DS_A_00_FECF_CTL1_ANNEX_SHIFT                         4
#define BCHP_DS_A_00_FECF_CTL1_ANNEX_DEFAULT                       0x00000000

/* DS_A_00_FECF :: CTL1 :: AUTO_SPECTRUM [03:03] */
#define BCHP_DS_A_00_FECF_CTL1_AUTO_SPECTRUM_MASK                  0x00000008
#define BCHP_DS_A_00_FECF_CTL1_AUTO_SPECTRUM_SHIFT                 3
#define BCHP_DS_A_00_FECF_CTL1_AUTO_SPECTRUM_DEFAULT               0x00000000

/* DS_A_00_FECF :: CTL1 :: ECO_SPARE_3 [02:02] */
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_3_MASK                    0x00000004
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_3_SHIFT                   2
#define BCHP_DS_A_00_FECF_CTL1_ECO_SPARE_3_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL1 :: ENABLE [01:01] */
#define BCHP_DS_A_00_FECF_CTL1_ENABLE_MASK                         0x00000002
#define BCHP_DS_A_00_FECF_CTL1_ENABLE_SHIFT                        1
#define BCHP_DS_A_00_FECF_CTL1_ENABLE_DEFAULT                      0x00000000

/* DS_A_00_FECF :: CTL1 :: RESET [00:00] */
#define BCHP_DS_A_00_FECF_CTL1_RESET_MASK                          0x00000001
#define BCHP_DS_A_00_FECF_CTL1_RESET_SHIFT                         0
#define BCHP_DS_A_00_FECF_CTL1_RESET_DEFAULT                       0x00000001

/***************************************************************************
 *CTL2 - FEC Control Register
 ***************************************************************************/
/* DS_A_00_FECF :: CTL2 :: ECO_SPARE_0 [31:18] */
#define BCHP_DS_A_00_FECF_CTL2_ECO_SPARE_0_MASK                    0xfffc0000
#define BCHP_DS_A_00_FECF_CTL2_ECO_SPARE_0_SHIFT                   18
#define BCHP_DS_A_00_FECF_CTL2_ECO_SPARE_0_DEFAULT                 0x00000000

/* DS_A_00_FECF :: CTL2 :: ALIGN [17:17] */
#define BCHP_DS_A_00_FECF_CTL2_ALIGN_MASK                          0x00020000
#define BCHP_DS_A_00_FECF_CTL2_ALIGN_SHIFT                         17
#define BCHP_DS_A_00_FECF_CTL2_ALIGN_DEFAULT                       0x00000001

/* DS_A_00_FECF :: CTL2 :: MEMORY_NUMBER [16:15] */
#define BCHP_DS_A_00_FECF_CTL2_MEMORY_NUMBER_MASK                  0x00018000
#define BCHP_DS_A_00_FECF_CTL2_MEMORY_NUMBER_SHIFT                 15
#define BCHP_DS_A_00_FECF_CTL2_MEMORY_NUMBER_DEFAULT               0x00000000

/* DS_A_00_FECF :: CTL2 :: BASE_ADDRESS [14:00] */
#define BCHP_DS_A_00_FECF_CTL2_BASE_ADDRESS_MASK                   0x00007fff
#define BCHP_DS_A_00_FECF_CTL2_BASE_ADDRESS_SHIFT                  0
#define BCHP_DS_A_00_FECF_CTL2_BASE_ADDRESS_DEFAULT                0x00000000

/***************************************************************************
 *LOCK_CTL1 - FEC Lock/Unlock Detection Control Register1
 ***************************************************************************/
/* DS_A_00_FECF :: LOCK_CTL1 :: ECO_SPARE_0 [31:18] */
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_0_MASK               0xfffc0000
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_0_SHIFT              18
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_0_DEFAULT            0x00000000

/* DS_A_00_FECF :: LOCK_CTL1 :: N_Nsp_B [17:08] */
#define BCHP_DS_A_00_FECF_LOCK_CTL1_N_Nsp_B_MASK                   0x0003ff00
#define BCHP_DS_A_00_FECF_LOCK_CTL1_N_Nsp_B_SHIFT                  8
#define BCHP_DS_A_00_FECF_LOCK_CTL1_N_Nsp_B_DEFAULT                0x00000000

/* DS_A_00_FECF :: LOCK_CTL1 :: ECO_SPARE_1 [07:00] */
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_1_MASK               0x000000ff
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_1_SHIFT              0
#define BCHP_DS_A_00_FECF_LOCK_CTL1_ECO_SPARE_1_DEFAULT            0x00000000

/***************************************************************************
 *LOCK_CTL2 - FEC Lock/Unlock Detection Control Register2
 ***************************************************************************/
/* DS_A_00_FECF :: LOCK_CTL2 :: F_Hg [31:24] */
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hg_MASK                      0xff000000
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hg_SHIFT                     24
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hg_DEFAULT                   0x00000000

/* DS_A_00_FECF :: LOCK_CTL2 :: F_Hscan [23:16] */
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hscan_MASK                   0x00ff0000
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hscan_SHIFT                  16
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hscan_DEFAULT                0x00000000

/* DS_A_00_FECF :: LOCK_CTL2 :: F_Hb [15:08] */
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hb_MASK                      0x0000ff00
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hb_SHIFT                     8
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hb_DEFAULT                   0x00000000

/* DS_A_00_FECF :: LOCK_CTL2 :: F_Hbt [07:00] */
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hbt_MASK                     0x000000ff
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hbt_SHIFT                    0
#define BCHP_DS_A_00_FECF_LOCK_CTL2_F_Hbt_DEFAULT                  0x00000000

/***************************************************************************
 *LOCK_CTL3 - FEC Lock/Unlock Detection Control Register3
 ***************************************************************************/
/* DS_A_00_FECF :: LOCK_CTL3 :: M_MPEGgc [31:24] */
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGgc_MASK                  0xff000000
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGgc_SHIFT                 24
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGgc_DEFAULT               0x00000000

/* DS_A_00_FECF :: LOCK_CTL3 :: M_MPEGb [23:12] */
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGb_MASK                   0x00fff000
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGb_SHIFT                  12
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGb_DEFAULT                0x00000000

/* DS_A_00_FECF :: LOCK_CTL3 :: M_MPEGt [11:00] */
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGt_MASK                   0x00000fff
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGt_SHIFT                  0
#define BCHP_DS_A_00_FECF_LOCK_CTL3_M_MPEGt_DEFAULT                0x00000000

/***************************************************************************
 *STATUS - FEC Status Register
 ***************************************************************************/
/* DS_A_00_FECF :: STATUS :: ECO_SPARE_4 [31:21] */
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_4_MASK                  0xffe00000
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_4_SHIFT                 21
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_4_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: SPECTRUM_LOCKED [20:20] */
#define BCHP_DS_A_00_FECF_STATUS_SPECTRUM_LOCKED_MASK              0x00100000
#define BCHP_DS_A_00_FECF_STATUS_SPECTRUM_LOCKED_SHIFT             20
#define BCHP_DS_A_00_FECF_STATUS_SPECTRUM_LOCKED_DEFAULT           0x00000000

/* DS_A_00_FECF :: STATUS :: IDEXT_B [19:16] */
#define BCHP_DS_A_00_FECF_STATUS_IDEXT_B_MASK                      0x000f0000
#define BCHP_DS_A_00_FECF_STATUS_IDEXT_B_SHIFT                     16
#define BCHP_DS_A_00_FECF_STATUS_IDEXT_B_DEFAULT                   0x00000000

/* DS_A_00_FECF :: STATUS :: ECO_SPARE_3 [15:15] */
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_3_MASK                  0x00008000
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_3_SHIFT                 15
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_3_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: FRAME_U_TO_L [14:14] */
#define BCHP_DS_A_00_FECF_STATUS_FRAME_U_TO_L_MASK                 0x00004000
#define BCHP_DS_A_00_FECF_STATUS_FRAME_U_TO_L_SHIFT                14
#define BCHP_DS_A_00_FECF_STATUS_FRAME_U_TO_L_DEFAULT              0x00000000

/* DS_A_00_FECF :: STATUS :: FRAME_L_TO_U [13:13] */
#define BCHP_DS_A_00_FECF_STATUS_FRAME_L_TO_U_MASK                 0x00002000
#define BCHP_DS_A_00_FECF_STATUS_FRAME_L_TO_U_SHIFT                13
#define BCHP_DS_A_00_FECF_STATUS_FRAME_L_TO_U_DEFAULT              0x00000000

/* DS_A_00_FECF :: STATUS :: FRAME_LOCK [12:12] */
#define BCHP_DS_A_00_FECF_STATUS_FRAME_LOCK_MASK                   0x00001000
#define BCHP_DS_A_00_FECF_STATUS_FRAME_LOCK_SHIFT                  12
#define BCHP_DS_A_00_FECF_STATUS_FRAME_LOCK_DEFAULT                0x00000000

/* DS_A_00_FECF :: STATUS :: ECO_SPARE_2 [11:11] */
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_2_MASK                  0x00000800
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_2_SHIFT                 11
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_2_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: NODE_U_TO_L [10:10] */
#define BCHP_DS_A_00_FECF_STATUS_NODE_U_TO_L_MASK                  0x00000400
#define BCHP_DS_A_00_FECF_STATUS_NODE_U_TO_L_SHIFT                 10
#define BCHP_DS_A_00_FECF_STATUS_NODE_U_TO_L_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: NODE_L_TO_U [09:09] */
#define BCHP_DS_A_00_FECF_STATUS_NODE_L_TO_U_MASK                  0x00000200
#define BCHP_DS_A_00_FECF_STATUS_NODE_L_TO_U_SHIFT                 9
#define BCHP_DS_A_00_FECF_STATUS_NODE_L_TO_U_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: NODE_LOCK [08:08] */
#define BCHP_DS_A_00_FECF_STATUS_NODE_LOCK_MASK                    0x00000100
#define BCHP_DS_A_00_FECF_STATUS_NODE_LOCK_SHIFT                   8
#define BCHP_DS_A_00_FECF_STATUS_NODE_LOCK_DEFAULT                 0x00000000

/* DS_A_00_FECF :: STATUS :: ECO_SPARE_1 [07:07] */
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_1_MASK                  0x00000080
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_1_SHIFT                 7
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_1_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: MPEG_U_TO_L [06:06] */
#define BCHP_DS_A_00_FECF_STATUS_MPEG_U_TO_L_MASK                  0x00000040
#define BCHP_DS_A_00_FECF_STATUS_MPEG_U_TO_L_SHIFT                 6
#define BCHP_DS_A_00_FECF_STATUS_MPEG_U_TO_L_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: MPEG_L_TO_U [05:05] */
#define BCHP_DS_A_00_FECF_STATUS_MPEG_L_TO_U_MASK                  0x00000020
#define BCHP_DS_A_00_FECF_STATUS_MPEG_L_TO_U_SHIFT                 5
#define BCHP_DS_A_00_FECF_STATUS_MPEG_L_TO_U_DEFAULT               0x00000000

/* DS_A_00_FECF :: STATUS :: MPEG_LOCK [04:04] */
#define BCHP_DS_A_00_FECF_STATUS_MPEG_LOCK_MASK                    0x00000010
#define BCHP_DS_A_00_FECF_STATUS_MPEG_LOCK_SHIFT                   4
#define BCHP_DS_A_00_FECF_STATUS_MPEG_LOCK_DEFAULT                 0x00000000

/* DS_A_00_FECF :: STATUS :: ECO_SPARE_0 [03:00] */
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_0_MASK                  0x0000000f
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_0_SHIFT                 0
#define BCHP_DS_A_00_FECF_STATUS_ECO_SPARE_0_DEFAULT               0x00000000

/***************************************************************************
 *HIST_%i - History memory content
 ***************************************************************************/
#define BCHP_DS_A_00_FECF_HIST_i_ARRAY_BASE                        0x02220c18
#define BCHP_DS_A_00_FECF_HIST_i_ARRAY_START                       0
#define BCHP_DS_A_00_FECF_HIST_i_ARRAY_END                         89
#define BCHP_DS_A_00_FECF_HIST_i_ARRAY_ELEMENT_SIZE                32

/***************************************************************************
 *HIST_%i - History memory content
 ***************************************************************************/
/* DS_A_00_FECF :: HIST_i :: DATA [31:00] */
#define BCHP_DS_A_00_FECF_HIST_i_DATA_MASK                         0xffffffff
#define BCHP_DS_A_00_FECF_HIST_i_DATA_SHIFT                        0
#define BCHP_DS_A_00_FECF_HIST_i_DATA_DEFAULT                      0x00000000


#endif /* #ifndef BCHP_DS_A_00_FECF_H__ */

/* End of File */
