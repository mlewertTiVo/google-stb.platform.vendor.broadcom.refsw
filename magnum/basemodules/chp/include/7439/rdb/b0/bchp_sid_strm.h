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
 * Date:           Generated on               Mon Jul 28 13:28:38 2014
 *                 Full Compile MD5 Checksum  c5ff84748c7fa0d54c801cf0c03aeeca
 *                     (minus title and desc)
 *                 MD5 Checksum               62ec62fd8717a3b00aa7784ade95abce
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     14541
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_SID_STRM_H__
#define BCHP_SID_STRM_H__

/***************************************************************************
 *SID_STRM - Stream Input
 ***************************************************************************/
#define BCHP_SID_STRM_CTL                        0x00bc0800 /* [RW] Stream control */
#define BCHP_SID_STRM_RAW_BYTE_CNT               0x00bc0808 /* [RO] Raw Byte Count */
#define BCHP_SID_STRM_BSHIFT_POS                 0x00bc080c /* [RO] Stream Bit-shifter position */
#define BCHP_SID_STRM_START_ADDR                 0x00bc0810 /* [RW] Stream Start Address */
#define BCHP_SID_STRM_LENGTH                     0x00bc0814 /* [RW] Stream Length */
#define BCHP_SID_STRM_RD_RAW_BYTE                0x00bc0820 /* [RO] Stream Read Raw Byte */
#define BCHP_SID_STRM_SKIP_RAW_BYTES             0x00bc0824 /* [RW] Skip Raw Bytes */
#define BCHP_SID_STRM_LD_BSHIFT_CNT              0x00bc0828 /* [RW] Load Bit Shift Cnt */
#define BCHP_SID_STRM_JPEG_MARKER                0x00bc082c /* [RO] JPEG Marker Code */
#define BCHP_SID_STRM_RD_RAW_WORD_BE             0x00bc0830 /* [RO] Read Raw Word Big-endian */
#define BCHP_SID_STRM_RD_RAW_LWORD_BE            0x00bc0834 /* [RO] Read Raw LWord Big-endian */
#define BCHP_SID_STRM_RD_RAW_WORD_LE             0x00bc0838 /* [RO] Read Raw Word Little-endian */
#define BCHP_SID_STRM_RD_RAW_LWORD_LE            0x00bc083c /* [RO] Read Raw LWord Little-endian */
#define BCHP_SID_STRM_PEEK_RAW_BYTE              0x00bc0840 /* [RO] Stream Peek Raw Byte */
#define BCHP_SID_STRM_CRC                        0x00bc0844 /* [RW] Stream CRC */
#define BCHP_SID_STRM_DEBUG                      0x00bc087c /* [RO] Stream Debug */

/***************************************************************************
 *CTL - Stream control
 ***************************************************************************/
/* SID_STRM :: CTL :: reserved0 [31:18] */
#define BCHP_SID_STRM_CTL_reserved0_MASK                           0xfffc0000
#define BCHP_SID_STRM_CTL_reserved0_SHIFT                          18

/* SID_STRM :: CTL :: LReg_Full [17:17] */
#define BCHP_SID_STRM_CTL_LReg_Full_MASK                           0x00020000
#define BCHP_SID_STRM_CTL_LReg_Full_SHIFT                          17
#define BCHP_SID_STRM_CTL_LReg_Full_DEFAULT                        0x00000000

/* SID_STRM :: CTL :: AReg_Full [16:16] */
#define BCHP_SID_STRM_CTL_AReg_Full_MASK                           0x00010000
#define BCHP_SID_STRM_CTL_AReg_Full_SHIFT                          16
#define BCHP_SID_STRM_CTL_AReg_Full_DEFAULT                        0x00000000

/* SID_STRM :: CTL :: reserved1 [15:06] */
#define BCHP_SID_STRM_CTL_reserved1_MASK                           0x0000ffc0
#define BCHP_SID_STRM_CTL_reserved1_SHIFT                          6

/* SID_STRM :: CTL :: CRC_OutBE [05:05] */
#define BCHP_SID_STRM_CTL_CRC_OutBE_MASK                           0x00000020
#define BCHP_SID_STRM_CTL_CRC_OutBE_SHIFT                          5
#define BCHP_SID_STRM_CTL_CRC_OutBE_DEFAULT                        0x00000000

/* SID_STRM :: CTL :: CRC_InBE [04:04] */
#define BCHP_SID_STRM_CTL_CRC_InBE_MASK                            0x00000010
#define BCHP_SID_STRM_CTL_CRC_InBE_SHIFT                           4
#define BCHP_SID_STRM_CTL_CRC_InBE_DEFAULT                         0x00000000

/* SID_STRM :: CTL :: reserved2 [03:03] */
#define BCHP_SID_STRM_CTL_reserved2_MASK                           0x00000008
#define BCHP_SID_STRM_CTL_reserved2_SHIFT                          3

/* SID_STRM :: CTL :: Jpeg_Emu [02:02] */
#define BCHP_SID_STRM_CTL_Jpeg_Emu_MASK                            0x00000004
#define BCHP_SID_STRM_CTL_Jpeg_Emu_SHIFT                           2
#define BCHP_SID_STRM_CTL_Jpeg_Emu_DEFAULT                         0x00000000

/* SID_STRM :: CTL :: FEnd [01:01] */
#define BCHP_SID_STRM_CTL_FEnd_MASK                                0x00000002
#define BCHP_SID_STRM_CTL_FEnd_SHIFT                               1
#define BCHP_SID_STRM_CTL_FEnd_DEFAULT                             0x00000000

/* SID_STRM :: CTL :: MEnd [00:00] */
#define BCHP_SID_STRM_CTL_MEnd_MASK                                0x00000001
#define BCHP_SID_STRM_CTL_MEnd_SHIFT                               0
#define BCHP_SID_STRM_CTL_MEnd_DEFAULT                             0x00000000

/***************************************************************************
 *RAW_BYTE_CNT - Raw Byte Count
 ***************************************************************************/
/* SID_STRM :: RAW_BYTE_CNT :: ByteCnt [31:00] */
#define BCHP_SID_STRM_RAW_BYTE_CNT_ByteCnt_MASK                    0xffffffff
#define BCHP_SID_STRM_RAW_BYTE_CNT_ByteCnt_SHIFT                   0
#define BCHP_SID_STRM_RAW_BYTE_CNT_ByteCnt_DEFAULT                 0x00000000

/***************************************************************************
 *BSHIFT_POS - Stream Bit-shifter position
 ***************************************************************************/
/* SID_STRM :: BSHIFT_POS :: reserved0 [31:04] */
#define BCHP_SID_STRM_BSHIFT_POS_reserved0_MASK                    0xfffffff0
#define BCHP_SID_STRM_BSHIFT_POS_reserved0_SHIFT                   4

/* SID_STRM :: BSHIFT_POS :: BitPos [03:00] */
#define BCHP_SID_STRM_BSHIFT_POS_BitPos_MASK                       0x0000000f
#define BCHP_SID_STRM_BSHIFT_POS_BitPos_SHIFT                      0
#define BCHP_SID_STRM_BSHIFT_POS_BitPos_DEFAULT                    0x00000000

/***************************************************************************
 *START_ADDR - Stream Start Address
 ***************************************************************************/
/* SID_STRM :: START_ADDR :: Addr [31:00] */
#define BCHP_SID_STRM_START_ADDR_Addr_MASK                         0xffffffff
#define BCHP_SID_STRM_START_ADDR_Addr_SHIFT                        0
#define BCHP_SID_STRM_START_ADDR_Addr_DEFAULT                      0x00000000

/***************************************************************************
 *LENGTH - Stream Length
 ***************************************************************************/
/* SID_STRM :: LENGTH :: Last [31:31] */
#define BCHP_SID_STRM_LENGTH_Last_MASK                             0x80000000
#define BCHP_SID_STRM_LENGTH_Last_SHIFT                            31
#define BCHP_SID_STRM_LENGTH_Last_DEFAULT                          0x00000001

/* SID_STRM :: LENGTH :: Length [30:00] */
#define BCHP_SID_STRM_LENGTH_Length_MASK                           0x7fffffff
#define BCHP_SID_STRM_LENGTH_Length_SHIFT                          0
#define BCHP_SID_STRM_LENGTH_Length_DEFAULT                        0x00000000

/***************************************************************************
 *RD_RAW_BYTE - Stream Read Raw Byte
 ***************************************************************************/
/* SID_STRM :: RD_RAW_BYTE :: Rsvd [31:08] */
#define BCHP_SID_STRM_RD_RAW_BYTE_Rsvd_MASK                        0xffffff00
#define BCHP_SID_STRM_RD_RAW_BYTE_Rsvd_SHIFT                       8

/* SID_STRM :: RD_RAW_BYTE :: RawByte [07:00] */
#define BCHP_SID_STRM_RD_RAW_BYTE_RawByte_MASK                     0x000000ff
#define BCHP_SID_STRM_RD_RAW_BYTE_RawByte_SHIFT                    0
#define BCHP_SID_STRM_RD_RAW_BYTE_RawByte_DEFAULT                  0x00000000

/***************************************************************************
 *SKIP_RAW_BYTES - Skip Raw Bytes
 ***************************************************************************/
/* SID_STRM :: SKIP_RAW_BYTES :: SkipCnt [31:00] */
#define BCHP_SID_STRM_SKIP_RAW_BYTES_SkipCnt_MASK                  0xffffffff
#define BCHP_SID_STRM_SKIP_RAW_BYTES_SkipCnt_SHIFT                 0
#define BCHP_SID_STRM_SKIP_RAW_BYTES_SkipCnt_DEFAULT               0x00000000

/***************************************************************************
 *LD_BSHIFT_CNT - Load Bit Shift Cnt
 ***************************************************************************/
/* SID_STRM :: LD_BSHIFT_CNT :: BShiftCnt [31:00] */
#define BCHP_SID_STRM_LD_BSHIFT_CNT_BShiftCnt_MASK                 0xffffffff
#define BCHP_SID_STRM_LD_BSHIFT_CNT_BShiftCnt_SHIFT                0
#define BCHP_SID_STRM_LD_BSHIFT_CNT_BShiftCnt_DEFAULT              0x00000000

/***************************************************************************
 *JPEG_MARKER - JPEG Marker Code
 ***************************************************************************/
/* SID_STRM :: JPEG_MARKER :: Rsvd [31:08] */
#define BCHP_SID_STRM_JPEG_MARKER_Rsvd_MASK                        0xffffff00
#define BCHP_SID_STRM_JPEG_MARKER_Rsvd_SHIFT                       8

/* SID_STRM :: JPEG_MARKER :: Marker [07:00] */
#define BCHP_SID_STRM_JPEG_MARKER_Marker_MASK                      0x000000ff
#define BCHP_SID_STRM_JPEG_MARKER_Marker_SHIFT                     0
#define BCHP_SID_STRM_JPEG_MARKER_Marker_DEFAULT                   0x00000000

/***************************************************************************
 *RD_RAW_WORD_BE - Read Raw Word Big-endian
 ***************************************************************************/
/* SID_STRM :: RD_RAW_WORD_BE :: Rsvd [31:16] */
#define BCHP_SID_STRM_RD_RAW_WORD_BE_Rsvd_MASK                     0xffff0000
#define BCHP_SID_STRM_RD_RAW_WORD_BE_Rsvd_SHIFT                    16

/* SID_STRM :: RD_RAW_WORD_BE :: RawWord [15:00] */
#define BCHP_SID_STRM_RD_RAW_WORD_BE_RawWord_MASK                  0x0000ffff
#define BCHP_SID_STRM_RD_RAW_WORD_BE_RawWord_SHIFT                 0
#define BCHP_SID_STRM_RD_RAW_WORD_BE_RawWord_DEFAULT               0x00000000

/***************************************************************************
 *RD_RAW_LWORD_BE - Read Raw LWord Big-endian
 ***************************************************************************/
/* SID_STRM :: RD_RAW_LWORD_BE :: RawLW [31:00] */
#define BCHP_SID_STRM_RD_RAW_LWORD_BE_RawLW_MASK                   0xffffffff
#define BCHP_SID_STRM_RD_RAW_LWORD_BE_RawLW_SHIFT                  0
#define BCHP_SID_STRM_RD_RAW_LWORD_BE_RawLW_DEFAULT                0x00000000

/***************************************************************************
 *RD_RAW_WORD_LE - Read Raw Word Little-endian
 ***************************************************************************/
/* SID_STRM :: RD_RAW_WORD_LE :: Rsvd [31:16] */
#define BCHP_SID_STRM_RD_RAW_WORD_LE_Rsvd_MASK                     0xffff0000
#define BCHP_SID_STRM_RD_RAW_WORD_LE_Rsvd_SHIFT                    16

/* SID_STRM :: RD_RAW_WORD_LE :: RawWord [15:00] */
#define BCHP_SID_STRM_RD_RAW_WORD_LE_RawWord_MASK                  0x0000ffff
#define BCHP_SID_STRM_RD_RAW_WORD_LE_RawWord_SHIFT                 0
#define BCHP_SID_STRM_RD_RAW_WORD_LE_RawWord_DEFAULT               0x00000000

/***************************************************************************
 *RD_RAW_LWORD_LE - Read Raw LWord Little-endian
 ***************************************************************************/
/* SID_STRM :: RD_RAW_LWORD_LE :: RawLW [31:00] */
#define BCHP_SID_STRM_RD_RAW_LWORD_LE_RawLW_MASK                   0xffffffff
#define BCHP_SID_STRM_RD_RAW_LWORD_LE_RawLW_SHIFT                  0
#define BCHP_SID_STRM_RD_RAW_LWORD_LE_RawLW_DEFAULT                0x00000000

/***************************************************************************
 *PEEK_RAW_BYTE - Stream Peek Raw Byte
 ***************************************************************************/
/* SID_STRM :: PEEK_RAW_BYTE :: Rsvd [31:08] */
#define BCHP_SID_STRM_PEEK_RAW_BYTE_Rsvd_MASK                      0xffffff00
#define BCHP_SID_STRM_PEEK_RAW_BYTE_Rsvd_SHIFT                     8

/* SID_STRM :: PEEK_RAW_BYTE :: RawByte [07:00] */
#define BCHP_SID_STRM_PEEK_RAW_BYTE_RawByte_MASK                   0x000000ff
#define BCHP_SID_STRM_PEEK_RAW_BYTE_RawByte_SHIFT                  0
#define BCHP_SID_STRM_PEEK_RAW_BYTE_RawByte_DEFAULT                0x00000000

/***************************************************************************
 *CRC - Stream CRC
 ***************************************************************************/
/* SID_STRM :: CRC :: CRC [31:00] */
#define BCHP_SID_STRM_CRC_CRC_MASK                                 0xffffffff
#define BCHP_SID_STRM_CRC_CRC_SHIFT                                0
#define BCHP_SID_STRM_CRC_CRC_DEFAULT                              0x00000000

/***************************************************************************
 *DEBUG - Stream Debug
 ***************************************************************************/
/* SID_STRM :: DEBUG :: DebugBits [31:00] */
#define BCHP_SID_STRM_DEBUG_DebugBits_MASK                         0xffffffff
#define BCHP_SID_STRM_DEBUG_DebugBits_SHIFT                        0

#endif /* #ifndef BCHP_SID_STRM_H__ */

/* End of File */
