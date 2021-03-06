/***************************************************************************
 *     Copyright (c) 1999-2007, Broadcom Corporation
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
 * Date:           Generated on         Mon Jul 23 11:56:55 2007
 *                 MD5 Checksum         61f9c4d8dcdcd06017506dddbf23f434
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008004
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_DTG_H__
#define BCHP_DTG_H__

/***************************************************************************
 *DTG - ITU 656 Timing Control
 ***************************************************************************/
#define BCHP_DTG_DTG_REV_ID                      0x00181300 /* Revision ID register */
#define BCHP_DTG_DTG_CONFIG                      0x00181308 /* DTG Configuration Register */
#define BCHP_DTG_CCIR_PCL                        0x00181310 /* PCL Registers for CCIR control signals */
#define BCHP_DTG_DVI_PCL                         0x00181314 /* PCL Registers for DVI control signals */
#define BCHP_DTG_CNTL_PCL                        0x00181318 /* PCL Registers for Misc. control signals */
#define BCHP_DTG_RAM_ADDR                        0x0018131c /* DTG Starting Address Register */
#define BCHP_DTG_DTG_BVB_SIZE                    0x00181320 /* BVB Size Register. */
#define BCHP_DTG_DTG_BVB_RSTATUS                 0x00181324 /* BVB status read Register. */
#define BCHP_DTG_DTG_BVB_CSTATUS                 0x00181328 /* BVB status clear Register. */
#define BCHP_DTG_DTG_TRIGGER_0                   0x0018132c /* DTG Trigger Register 0 */
#define BCHP_DTG_DTG_TRIGGER_1                   0x00181330 /* DTG Trigger Register 1 */
#define BCHP_DTG_DTG_CTRL_STAT                   0x00181334 /* DTG Control Bus Status Register */
#define BCHP_DTG_DTG_LCNTR                       0x00181338 /* DTG Line Counter Register */

#endif /* #ifndef BCHP_DTG_H__ */

/* End of File */
