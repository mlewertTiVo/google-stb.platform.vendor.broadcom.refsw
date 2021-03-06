/***************************************************************************
 *     Copyright (c) 1999-2012, Broadcom Corporation
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
 * Date:           Generated on         Tue Feb 28 11:03:21 2012
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

#ifndef BCHP_UARTB_H__
#define BCHP_UARTB_H__

/***************************************************************************
 *UARTB - UART B
 ***************************************************************************/
#define BCHP_UARTB_RBR                           0x00406b40 /* Receive Buffer Register */
#define BCHP_UARTB_THR                           0x00406b40 /* Transmit Holding Register */
#define BCHP_UARTB_DLH                           0x00406b44 /* Divisor Latch High */
#define BCHP_UARTB_DLL                           0x00406b40 /* Divisor Latch Low */
#define BCHP_UARTB_IER                           0x00406b44 /* Interrupt Enable Register */
#define BCHP_UARTB_IIR                           0x00406b48 /* Interrupt Identity Register */
#define BCHP_UARTB_FCR                           0x00406b48 /* FIFO Control Register */
#define BCHP_UARTB_LCR                           0x00406b4c /* Line Control Register */
#define BCHP_UARTB_MCR                           0x00406b50 /* Modem Control Register */
#define BCHP_UARTB_LSR                           0x00406b54 /* Line Status Register */
#define BCHP_UARTB_MSR                           0x00406b58 /* Modem Status Register */
#define BCHP_UARTB_SCR                           0x00406b5c /* Scratchpad Register */

#endif /* #ifndef BCHP_UARTB_H__ */

/* End of File */
