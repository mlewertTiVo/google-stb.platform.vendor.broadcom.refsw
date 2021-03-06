/****************************************************************************
 *     Copyright (c) 1999-2015, Broadcom Corporation
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
 * Date:           Generated on               Tue Mar 10 21:58:48 2015
 *                 Full Compile MD5 Checksum  5e1590010d5c1b57e50bc6f4261c6964
 *                     (minus title and desc)
 *                 MD5 Checksum               48022a5123f15fb90760ff6d5877a474
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15839
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_UARTC_H__
#define BCHP_UARTC_H__

/***************************************************************************
 *UARTC - UART C
 ***************************************************************************/
#define BCHP_UARTC_RBR                           0x00406880 /* [RO] Receive Buffer Register */
#define BCHP_UARTC_THR                           0x00406880 /* [WO] Transmit Holding Register */
#define BCHP_UARTC_DLL                           0x00406880 /* [RW] Divisor Latch Low */
#define BCHP_UARTC_DLH                           0x00406884 /* [RW] Divisor Latch High */
#define BCHP_UARTC_IER                           0x00406884 /* [RW] Interrupt Enable Register */
#define BCHP_UARTC_IIR                           0x00406888 /* [RO] Interrupt Identity Register */
#define BCHP_UARTC_FCR                           0x00406888 /* [WO] FIFO Control Register */
#define BCHP_UARTC_LCR                           0x0040688c /* [RW] Line Control Register */
#define BCHP_UARTC_MCR                           0x00406890 /* [RW] Modem Control Register */
#define BCHP_UARTC_LSR                           0x00406894 /* [RO] Line Status Register */
#define BCHP_UARTC_MSR                           0x00406898 /* [RO] Modem Status Register */
#define BCHP_UARTC_SCR                           0x0040689c /* [RW] Scratchpad Register */

#endif /* #ifndef BCHP_UARTC_H__ */

/* End of File */
