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
 * Date:           Generated on               Fri Aug 15 15:20:54 2014
 *                 Full Compile MD5 Checksum  a68bc62e9dd3be19fcad480c369d60fd
 *                     (minus title and desc)
 *                 MD5 Checksum               14382795d76d8497c2dd1bcf3f5d36da
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     14541
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_SDS_SNR_1_H__
#define BCHP_SDS_SNR_1_H__

/***************************************************************************
 *SDS_SNR_1 - SDS SNR Register Set
 ***************************************************************************/
#define BCHP_SDS_SNR_1_SNRCTL                    0x01241500 /* [RW] SNR Control Register */
#define BCHP_SDS_SNR_1_SNRHT                     0x01241504 /* [RW] SNR Estimator high threshold. */
#define BCHP_SDS_SNR_1_SNRLT                     0x01241508 /* [RW] SNR Estimator low threshold. */
#define BCHP_SDS_SNR_1_SNRE                      0x0124150c /* [RW] SNR Estimate. */
#define BCHP_SDS_SNR_1_SNORETP                   0x01241510 /* [RW] SNOR Estimate total power. */
#define BCHP_SDS_SNR_1_SNORESP                   0x01241514 /* [RW] SNOR Estimate signal power. */
#define BCHP_SDS_SNR_1_SNORECTL                  0x01241518 /* [RW] SNORE Control Register. */

#endif /* #ifndef BCHP_SDS_SNR_1_H__ */

/* End of File */
