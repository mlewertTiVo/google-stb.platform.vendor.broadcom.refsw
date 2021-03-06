/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
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
 * Date:           Generated on         Wed Mar 27 10:49:18 2013
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

#ifndef BCHP_TFEC_3_H__
#define BCHP_TFEC_3_H__

/***************************************************************************
 *TFEC_3 - 3 SDS TFEC Register Set
 ***************************************************************************/
#define BCHP_TFEC_3_TFECTL                       0x0124b000 /* Advanced FEC Control */
#define BCHP_TFEC_3_TNBLK                        0x0124b014 /* RS total block count */
#define BCHP_TFEC_3_TCBLK                        0x0124b018 /* RS corrected block count */
#define BCHP_TFEC_3_TBBLK                        0x0124b01c /* RS bad block count */
#define BCHP_TFEC_3_TCSYM                        0x0124b020 /* RS corrected symbol count */
#define BCHP_TFEC_3_TFMT                         0x0124b024 /* RS parameters */
#define BCHP_TFEC_3_TPAK                         0x0124b028 /* MPEG packetizer parameters */
#define BCHP_TFEC_3_TSSQ                         0x0124b02c /* Advanced FEC symbol sequence */
#define BCHP_TFEC_3_TSYN                         0x0124b030 /* MPEG synchronizer parameters */
#define BCHP_TFEC_3_TTUR                         0x0124b034 /* Advanced FEC configuration parameters */
#define BCHP_TFEC_3_TZPK                         0x0124b038 /* RS synchronizer parameters */
#define BCHP_TFEC_3_TZSY                         0x0124b03c /* RS synchronizer aquisition and retention parameters */
#define BCHP_TFEC_3_TITR                         0x0124b040 /* Iterative decoder parameters */
#define BCHP_TFEC_3_TCIL                         0x0124b044 /* Iterative decoder parameters */
#define BCHP_TFEC_3_TRSD                         0x0124b048 /* RS decoder parameters */
#define BCHP_TFEC_3_TPN                          0x0124b04c /* Iterative decoder internal point signature analyzer output */
#define BCHP_TFEC_3_TSIGCNT                      0x0124b050 /* Signature analyzer data length count */
#define BCHP_TFEC_3_TSIGITD                      0x0124b054 /* Iterative decoder signature analyzer output */
#define BCHP_TFEC_3_TSIGXPT                      0x0124b058 /* Transport output signature analyzer output */
#define BCHP_TFEC_3_TTPCTL                       0x0124b05c /* Advanced FEC testport control */
#define BCHP_TFEC_3_TRAWISR                      0x0124b060 /* Advanced FEC raw interrupt status */

#endif /* #ifndef BCHP_TFEC_3_H__ */

/* End of File */
