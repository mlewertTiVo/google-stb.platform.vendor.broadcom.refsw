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
 * Date:           Generated on         Mon Jul 23 11:55:57 2007
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

#ifndef BCHP_GSE_SEC_H__
#define BCHP_GSE_SEC_H__

/***************************************************************************
 *GSE_SEC - SEC GS Encoder
 ***************************************************************************/
#define BCHP_GSE_SEC_REVID                       0x00186400 /* GSTAR Encoder Revision ID Register */
#define BCHP_GSE_SEC_CONTROL                     0x00186408 /* Control Register */
#define BCHP_GSE_SEC_ACTIVE_LINE_TOP             0x0018640c /* Top Field Active Line and Pedestal Enable Register */
#define BCHP_GSE_SEC_ACTIVE_LINE_BOT             0x00186410 /* Bottom Field Active Line and Pedestal Enable Register */
#define BCHP_GSE_SEC_GAIN_TOP                    0x00186414 /* Top Field Gain Register */
#define BCHP_GSE_SEC_GAIN_EXT_TOP                0x00186418 /* Top Field Line5 Gain Register */
#define BCHP_GSE_SEC_GAIN_BOT                    0x0018641c /* Bottom Field Gain Register */
#define BCHP_GSE_SEC_GAIN_EXT_BOT                0x00186420 /* Bottom Field Line5 Gain Register */
#define BCHP_GSE_SEC_DATA_LINE1_BANK0            0x00186424 /* Bank 0 Line 1 Data Register */
#define BCHP_GSE_SEC_DATA_LINE2_BANK0            0x00186428 /* Bank 0 Line 2 Data Register */
#define BCHP_GSE_SEC_DATA_LINE3_BANK0            0x0018642c /* Bank 0 Line 3 Data Register */
#define BCHP_GSE_SEC_DATA_LINE4_BANK0            0x00186430 /* Bank 0 Line 4 Data Register */
#define BCHP_GSE_SEC_DATA_LINE5_BANK0            0x00186434 /* Bank 0 Line 5 Data Register */
#define BCHP_GSE_SEC_DATA_LINE1_BANK1            0x00186438 /* Bank 1 Line 1 Data Register */
#define BCHP_GSE_SEC_DATA_LINE2_BANK1            0x0018643c /* Bank 1 Line 2 Data Register */
#define BCHP_GSE_SEC_DATA_LINE3_BANK1            0x00186440 /* Bank 1 Line 3 Data Register */
#define BCHP_GSE_SEC_DATA_LINE4_BANK1            0x00186444 /* Bank 1 Line 4 Data Register */
#define BCHP_GSE_SEC_DATA_LINE5_BANK1            0x00186448 /* Bank 1 Line 5 Data Register */
#define BCHP_GSE_SEC_DATA_LINE1_BANK2            0x0018644c /* Bank 2 Line 1 Data Register */
#define BCHP_GSE_SEC_DATA_LINE2_BANK2            0x00186450 /* Bank 2 Line 2 Data Register */
#define BCHP_GSE_SEC_DATA_LINE3_BANK2            0x00186454 /* Bank 2 Line 3 Data Register */
#define BCHP_GSE_SEC_DATA_LINE4_BANK2            0x00186458 /* Bank 2 Line 4 Data Register */
#define BCHP_GSE_SEC_DATA_LINE5_BANK2            0x0018645c /* Bank 2 Line 5 Data Register */
#define BCHP_GSE_SEC_DATA_LINE1_BANK3            0x00186460 /* Bank 3 Line 1 Data Register */
#define BCHP_GSE_SEC_DATA_LINE2_BANK3            0x00186464 /* Bank 3 Line 2 Data Register */
#define BCHP_GSE_SEC_DATA_LINE3_BANK3            0x00186468 /* Bank 3 Line 3 Data Register */
#define BCHP_GSE_SEC_DATA_LINE4_BANK3            0x0018646c /* Bank 3 Line 4 Data Register */
#define BCHP_GSE_SEC_DATA_LINE5_BANK3            0x00186470 /* Bank 3 Line 5 Data Register */
#define BCHP_GSE_SEC_NULL                        0x00186474 /* NULL Register */
#define BCHP_GSE_SEC_WRPTR                       0x00186478 /* Write Pointer Register */
#define BCHP_GSE_SEC_RDPTR                       0x0018647c /* Read Pointer Register */
#define BCHP_GSE_SEC_STATUS                      0x00186480 /* Status Register */

#endif /* #ifndef BCHP_GSE_SEC_H__ */

/* End of File */
