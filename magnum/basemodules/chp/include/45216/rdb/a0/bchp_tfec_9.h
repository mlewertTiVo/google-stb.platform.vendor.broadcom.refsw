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

#ifndef BCHP_TFEC_9_H__
#define BCHP_TFEC_9_H__

/***************************************************************************
 *TFEC_9 - SDS TFEC Register Set
 ***************************************************************************/
#define BCHP_TFEC_9_TFECTL                       0x05900000 /* Advanced FEC Control */
#define BCHP_TFEC_9_TNBLK                        0x05900014 /* RS total block count */
#define BCHP_TFEC_9_TCBLK                        0x05900018 /* RS corrected block count */
#define BCHP_TFEC_9_TBBLK                        0x0590001c /* RS bad block count */
#define BCHP_TFEC_9_TCSYM                        0x05900020 /* RS corrected symbol count */
#define BCHP_TFEC_9_TFMT                         0x05900024 /* RS parameters */
#define BCHP_TFEC_9_TPAK                         0x05900028 /* MPEG packetizer parameters */
#define BCHP_TFEC_9_TSSQ                         0x0590002c /* Advanced FEC symbol sequence */
#define BCHP_TFEC_9_TSYN                         0x05900030 /* MPEG synchronizer parameters */
#define BCHP_TFEC_9_TTUR                         0x05900034 /* Advanced FEC configuration parameters */
#define BCHP_TFEC_9_TZPK                         0x05900038 /* RS synchronizer parameters */
#define BCHP_TFEC_9_TZSY                         0x0590003c /* RS synchronizer aquisition and retention parameters */
#define BCHP_TFEC_9_TITR                         0x05900040 /* Iterative decoder parameters */
#define BCHP_TFEC_9_TCIL                         0x05900044 /* Iterative decoder parameters */
#define BCHP_TFEC_9_TRSD                         0x05900048 /* RS decoder parameters */
#define BCHP_TFEC_9_TPN                          0x0590004c /* Iterative decoder internal point signature analyzer output */
#define BCHP_TFEC_9_TSIGCNT                      0x05900050 /* Signature analyzer data length count */
#define BCHP_TFEC_9_TSIGITD                      0x05900054 /* Iterative decoder signature analyzer output */
#define BCHP_TFEC_9_TSIGXPT                      0x05900058 /* Transport output signature analyzer output */
#define BCHP_TFEC_9_TTPCTL                       0x0590005c /* Advanced FEC testport control */
#define BCHP_TFEC_9_TRAWISR                      0x05900060 /* Advanced FEC raw interrupt status */

#endif /* #ifndef BCHP_TFEC_9_H__ */

/* End of File */
