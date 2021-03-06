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
 * Date:           Generated on               Wed Apr  1 11:23:13 2015
 *                 Full Compile MD5 Checksum  267f8e92d9b43928c0a06f1ab29c511c
 *                     (minus title and desc)
 *                 MD5 Checksum               0548f7f0a8e20364fd383a7aa29c0b86
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15956
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_MPM_CPU_DATA_MEM_H__
#define BCHP_MPM_CPU_DATA_MEM_H__

/***************************************************************************
 *MPM_CPU_DATA_MEM - CPU Data Memory
 ***************************************************************************/

/***************************************************************************
 *WORD%i - Data RAM
 ***************************************************************************/
#define BCHP_MPM_CPU_DATA_MEM_WORDi_ARRAY_BASE                     0x01510000
#define BCHP_MPM_CPU_DATA_MEM_WORDi_ARRAY_START                    0
#define BCHP_MPM_CPU_DATA_MEM_WORDi_ARRAY_END                      4095
#define BCHP_MPM_CPU_DATA_MEM_WORDi_ARRAY_ELEMENT_SIZE             32

/***************************************************************************
 *WORD%i - Data RAM
 ***************************************************************************/
/* MPM_CPU_DATA_MEM :: WORDi :: WORD [31:00] */
#define BCHP_MPM_CPU_DATA_MEM_WORDi_WORD_MASK                      0xffffffff
#define BCHP_MPM_CPU_DATA_MEM_WORDi_WORD_SHIFT                     0


#endif /* #ifndef BCHP_MPM_CPU_DATA_MEM_H__ */

/* End of File */
