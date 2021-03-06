/***************************************************************************
 *     Copyright (c) 1999-2009, Broadcom Corporation
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
 * Date:           Generated on         Tue Nov 17 17:21:28 2009
 *                 MD5 Checksum         c5a869a181cd53ce96d34b0e7ab357f3
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

#ifndef BCHP_KBD2_H__
#define BCHP_KBD2_H__

/***************************************************************************
 *KBD2 - IR Keyboard/Remote Receiver 2
 ***************************************************************************/
#define BCHP_KBD2_STATUS                         0x00406640 /* KEYBOARD/REMOTE RECEIVER STATUS REGISTER */
#define BCHP_KBD2_DIVISOR                        0x00406644 /* KEYBOARD/REMOTE RECEIVER DIVISOR */
#define BCHP_KBD2_FILTER1                        0x00406648 /* KEYBOARD FILTER1 */
#define BCHP_KBD2_DATA1                          0x0040664c /* KEYBOARD/REMOTE RECEIVER DATA WORD 1 (MS Word) */
#define BCHP_KBD2_DATA0                          0x00406650 /* KEYBOARD/REMOTE RECEIVER DATA WORD 0 (LS Word) */
#define BCHP_KBD2_CMD                            0x00406654 /* KEYBOARD/REMOTE RECEIVER COMMAND REGISTER */
#define BCHP_KBD2_CIR_ADDR                       0x00406658 /* CONSUMER IR DECODER ADDRESS REGISTER */
#define BCHP_KBD2_CIR_DATA                       0x0040665c /* CONSUMER IR DECODER DATA (LSB) */
#define BCHP_KBD2_KBD_MASK0                      0x00406660 /* KEBOARD INPUT MASK (LSB) */
#define BCHP_KBD2_KBD_MASK1                      0x00406664 /* KEBOARD INPUT MASK (MSB) */
#define BCHP_KBD2_KBD_PAT0                       0x00406668 /* KEBOARD INPUT PATTERN MATCH (LSB) */
#define BCHP_KBD2_KBD_PAT1                       0x0040666c /* KEBOARD INPUT PATTERN MATCH (MSB) */
#define BCHP_KBD2_STATUS2                        0x0040667c /* KEYBOARD/REMOTE RECEIVER STATUS2 REGISTER */

#endif /* #ifndef BCHP_KBD2_H__ */

/* End of File */
