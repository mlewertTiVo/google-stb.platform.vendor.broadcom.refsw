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
 * Date:           Generated on              Mon Sep 23 09:50:33 2013
 *                 Full Compile MD5 Checksum fcccce298b546dd6a1f4cbad288478da
 *                   (minus title and desc)  
 *                 MD5 Checksum              211556602e37a33262598b3d5eeba81c
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_HEVD_IL_CPU_REGS_2_0_H__
#define BCHP_HEVD_IL_CPU_REGS_2_0_H__

/***************************************************************************
 *HEVD_IL_CPU_REGS_2_0
 ***************************************************************************/
#define BCHP_HEVD_IL_CPU_REGS_2_0_HST2CPU_MBX    0x00050000 /* Host 2 CPU mailbox register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU2HST_MBX    0x00050004 /* CPU to Host mailbox register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_MBX_STAT       0x00050008 /* Mailbox status flags */
#define BCHP_HEVD_IL_CPU_REGS_2_0_INST_BASE      0x0005000c /* Instruction base address register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_INT_ENA    0x00050010 /* CPU interrupt enable */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_INT_STAT   0x00050014 /* CPU interrupt status */
#define BCHP_HEVD_IL_CPU_REGS_2_0_HST2CPU_STAT   0x00050018 /* Host to CPU status register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU2HST_STAT   0x0005001c /* CPU to Host status register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_INTGEN_SET 0x00050020 /* CPU interrupt set register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_INTGEN_CLR 0x00050024 /* CPU interrupt clear register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_ICACHE_MISS 0x00050028 /* Instruction cache miss counter */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CPU_INTGEN_MASK 0x0005002c /* CPU interrupt mask register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DRAM_RD_CNTR   0x00050030 /* CPU DRAM Read access Counter */
#define BCHP_HEVD_IL_CPU_REGS_2_0_END_OF_CODE    0x00050034 /* End of code register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_GLOBAL_IO_BASE 0x00050038 /* Global IO base register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DEBUG_TRACE_FIFO_WR 0x0005003c /* CPU debug trace fifo write */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DEBUG_TRACE_FIFO_RD 0x00050040 /* CPU debug trace fifo read */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DEBUG_TRACE_FIFO_CTL 0x00050044 /* CPU debug trace fifo control */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DRAM_WR_CNTR   0x00050048 /* CPU DRAM Write access Counter */
#define BCHP_HEVD_IL_CPU_REGS_2_0_WATCHDOG_TMR   0x0005004c /* Watchdog timer register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_SDRAM_STATUS   0x00050050 /* SDRAM Status register */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DEBUG_CTL      0x00050054 /* Debug Control */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CMD_REG0       0x00050060 /* Command register 0 */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CMD_REG1       0x00050064 /* Command register 1 */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CMD_REG2       0x00050068 /* Command register 2 */
#define BCHP_HEVD_IL_CPU_REGS_2_0_CMD_REG3       0x0005006c /* Command register 3 */
#define BCHP_HEVD_IL_CPU_REGS_2_0_DEC_VERSION    0x00050108 /* Decoder versions */

#endif /* #ifndef BCHP_HEVD_IL_CPU_REGS_2_0_H__ */

/* End of File */
