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
 * Date:           Generated on              Thu Feb 20 15:33:52 2014
 *                 Full Compile MD5 Checksum 1766fea499add5f6ee91330ef96d35c5
 *                   (minus title and desc)
 *                 MD5 Checksum              4c358fb5b94802f03aec82d8df2c9afa
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 *
 ***************************************************************************/



/****************************************************************************
 ***************************************************************************/

#ifndef BCHP_AFEC1_7_H__
#define BCHP_AFEC1_7_H__

/***************************************************************************
 *AFEC1_7 - AFEC Register Set for Channel 1
 ***************************************************************************/
#define BCHP_AFEC1_7_RST                         0x06702000 /* AFEC core channel-based reset register */
#define BCHP_AFEC1_7_CNTR_CTRL                   0x06702004 /* AFEC counter-control register */
#define BCHP_AFEC1_7_MODCOD_PARAM_0              0x06702008 /* AFEC MODCOD parameter register */
#define BCHP_AFEC1_7_MODCOD_PARAM_1              0x0670200c /* AFEC MODCOD parameter register */
#define BCHP_AFEC1_7_MODCOD_STATS_CONFIG         0x06702010 /* MODCOD statistics configuration block */
#define BCHP_AFEC1_7_LDPC_ITER_CNT               0x06702014 /* LDPC Iteration counter */
#define BCHP_AFEC1_7_LDPC_FAIL_CNT               0x06702018 /* LDPC FAIL COUNTER */
#define BCHP_AFEC1_7_LDPC_FRM_CNT                0x0670201c /* LDPC FRAME COUNTER */
#define BCHP_AFEC1_7_LDPC_CONFIG                 0x06702020 /* LDPC Configuration Register */
#define BCHP_AFEC1_7_LDPC_STATUS                 0x06702024 /* LDPC Status Register */
#define BCHP_AFEC1_7_LDPC_MET_CRC                0x06702028 /* Metric Generator Signature */
#define BCHP_AFEC1_7_LDPC_EDGE_CRC               0x0670202c /* Edge Output Signature */
#define BCHP_AFEC1_7_LDPC_PSL_CTL                0x06702030 /* Power Saving Loop Control register */
#define BCHP_AFEC1_7_LDPC_PSL_INT_THRES          0x06702034 /* PSL Integrator Threshold */
#define BCHP_AFEC1_7_LDPC_PSL_INT                0x06702038 /* PSL Integrator Value */
#define BCHP_AFEC1_7_LDPC_PSL_AVE                0x0670203c /* PSL Integrator Average Value */
#define BCHP_AFEC1_7_LDPC_PSL_XCS                0x06702040 /* PSL Excess Value */
#define BCHP_AFEC1_7_LDPC_PSL_FILTER             0x06702044 /* PSL Filter for ACM */
#define BCHP_AFEC1_7_BCH_TPSIG                   0x06702050 /* BCH Block Signature Analyzer */
#define BCHP_AFEC1_7_BCH_SMTH_FIFO_MIN_LEVEL     0x06702054 /* BCH Smoother Fifo Min Level */
#define BCHP_AFEC1_7_BCH_SMTH_FIFO_MAX_LEVEL     0x06702058 /* BCH Smoother Fifo Max Level */
#define BCHP_AFEC1_7_BCH_CTRL                    0x06702064 /* BCH Decoder Control Register */
#define BCHP_AFEC1_7_BCH_DECNBLK                 0x06702068 /* BCH Number of Block Counter */
#define BCHP_AFEC1_7_BCH_DECCBLK                 0x0670206c /* BCH Number of Corrected Block Counter */
#define BCHP_AFEC1_7_BCH_DECBBLK                 0x06702070 /* BCH Number of Bad Block Counter */
#define BCHP_AFEC1_7_BCH_DECCBIT                 0x06702074 /* BCH Number of Corrected Bit Counter */
#define BCHP_AFEC1_7_BCH_DECMCOR                 0x06702078 /* BCH Number of Miscorrected Block Counter */
#define BCHP_AFEC1_7_BCH_BBHDR0                  0x0670207c /* BBHEADER Register 0 */
#define BCHP_AFEC1_7_BCH_BBHDR1                  0x06702080 /* BBHEADER Register 1 */
#define BCHP_AFEC1_7_BCH_BBHDR2                  0x06702084 /* BBHEADER Register 2 */
#define BCHP_AFEC1_7_BCH_BBHDR3                  0x06702088 /* BBHEADER Register 3 */
#define BCHP_AFEC1_7_BCH_BBHDR4                  0x0670208c /* BBHEADER Register 4 */
#define BCHP_AFEC1_7_BCH_BBHDR5                  0x067020c0 /* BBHEADER Register 5 */
#define BCHP_AFEC1_7_BCH_MPLCK                   0x067020c4 /* MPEG Lock Detector Configuration Register */
#define BCHP_AFEC1_7_BCH_MPCFG                   0x067020c8 /* MPEG Packetizer Configuration Register */
#define BCHP_AFEC1_7_BCH_SMCFG                   0x067020cc /* Smoother FIFO Configuration Register */
#define BCHP_AFEC1_7_CH_FRMCYCLES                0x06702a00 /* Channel frame period  Register */
#define BCHP_AFEC1_7_BIST_PARAM_0                0x06702a04 /* Channel-based BIST Configuration Register 0 */
#define BCHP_AFEC1_7_BIST_PARAM_1                0x06702a08 /* Channel-based BIST Configuration Register 1 */
#define BCHP_AFEC1_7_FAKEFRM_PARAM               0x06702a0c /* Parameters related to the fake frame generation during early termination */

#endif /* #ifndef BCHP_AFEC1_7_H__ */

/* End of File */
