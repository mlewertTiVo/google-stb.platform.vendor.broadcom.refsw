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
 * Date:           Generated on               Mon Sep 15 10:12:21 2014
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

#ifndef BCHP_DS_A_23_FECF_H__
#define BCHP_DS_A_23_FECF_H__

/***************************************************************************
 *DS_A_23_FECF - Downstream FEC Frond End Registers
 ***************************************************************************/
#define BCHP_DS_A_23_FECF_CTL1                   0x0224ec00 /* [RW] FEC Control Register 1 */
#define BCHP_DS_A_23_FECF_CTL2                   0x0224ec04 /* [RW] FEC Control Register */
#define BCHP_DS_A_23_FECF_LOCK_CTL1              0x0224ec08 /* [RW] FEC Lock/Unlock Detection Control Register1 */
#define BCHP_DS_A_23_FECF_LOCK_CTL2              0x0224ec0c /* [RW] FEC Lock/Unlock Detection Control Register2 */
#define BCHP_DS_A_23_FECF_LOCK_CTL3              0x0224ec10 /* [RW] FEC Lock/Unlock Detection Control Register3 */
#define BCHP_DS_A_23_FECF_STATUS                 0x0224ec14 /* [RW] FEC Status Register */

/***************************************************************************
 *HIST_%i - History memory content
 ***************************************************************************/
#define BCHP_DS_A_23_FECF_HIST_i_ARRAY_BASE                        0x0224ec18
#define BCHP_DS_A_23_FECF_HIST_i_ARRAY_START                       0
#define BCHP_DS_A_23_FECF_HIST_i_ARRAY_END                         89
#define BCHP_DS_A_23_FECF_HIST_i_ARRAY_ELEMENT_SIZE                32

/***************************************************************************
 *HIST_%i - History memory content
 ***************************************************************************/
/* DS_A_23_FECF :: HIST_i :: DATA [31:00] */
#define BCHP_DS_A_23_FECF_HIST_i_DATA_MASK                         0xffffffff
#define BCHP_DS_A_23_FECF_HIST_i_DATA_SHIFT                        0
#define BCHP_DS_A_23_FECF_HIST_i_DATA_DEFAULT                      0x00000000


#endif /* #ifndef BCHP_DS_A_23_FECF_H__ */

/* End of File */
