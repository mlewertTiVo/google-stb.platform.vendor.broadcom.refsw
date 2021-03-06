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
 * Date:           Generated on               Mon Nov 17 11:53:46 2014
 *                 Full Compile MD5 Checksum  7c712d144f2919fdac508431993b36d7
 *                     (minus title and desc)
 *                 MD5 Checksum               bc950b877a17d97d6325a810f8bd832e
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15193
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_CAP_1_H__
#define BCHP_CAP_1_H__

/***************************************************************************
 *CAP_1 - Capture 1 Registers
 ***************************************************************************/
#define BCHP_CAP_1_REVISION                      0x00680200 /* [RO] Capture Engine Revision ID */
#define BCHP_CAP_1_PIC_SIZE                      0x00680204 /* [DBRW] Capture Vertical and Horizontal Size */
#define BCHP_CAP_1_PIC_OFFSET                    0x00680208 /* [DBRW] Capture 2D or 3D Left View Vertical and Horizontal Start Offset */
#define BCHP_CAP_1_PIC_OFFSET_R                  0x0068020c /* [DBRW] Capture 3D Right View Horizontal Start Offset */
#define BCHP_CAP_1_BVB_IN_SIZE                   0x00680210 /* [DBRW] Capture BVB Source Vertical and Horizontal Size */
#define BCHP_CAP_1_MSTART                        0x00680214 /* [DBRW] Capture Memory Buffer Starting Address */
#define BCHP_CAP_1_MSTART_R                      0x00680218 /* [DBRW] Capture Memory Buffer Right Window Starting Address */
#define BCHP_CAP_1_PITCH                         0x0068021c /* [DBRW] Capture Memory Buffer Pitch */
#define BCHP_CAP_1_CTRL                          0x00680220 /* [RW] Capture Control */
#define BCHP_CAP_1_MODE                          0x00680224 /* [DBRW] Capture Mode Control */
#define BCHP_CAP_1_BYTE_ORDER                    0x00680228 /* [RW] Capture Byte Order Control */
#define BCHP_CAP_1_RX_CTRL                       0x0068022c /* [RW] Capture BVB Receiver Control */
#define BCHP_CAP_1_TRIG_CTRL                     0x00680230 /* [RW] Capture RDMA Trigger Control */
#define BCHP_CAP_1_BVB_TRIG_0_CFG                0x00680234 /* [RW] Capture BVB Trigger 0 Configuration */
#define BCHP_CAP_1_BVB_TRIG_1_CFG                0x00680238 /* [RW] Capture BVB Trigger 1 Configuration */
#define BCHP_CAP_1_LINE_CMP_TRIG_0_CFG           0x0068023c /* [RW] Capture Line Compare Trigger 0 Configuration */
#define BCHP_CAP_1_LINE_CMP_TRIG_1_CFG           0x00680240 /* [RW] Capture Line Compare Trigger 1 Configuration */
#define BCHP_CAP_1_STATUS                        0x00680244 /* [RO] Capture Engine Status */
#define BCHP_CAP_1_BVB_STATUS                    0x00680248 /* [RO] Capture BVB Status */
#define BCHP_CAP_1_BVB_STATUS_CLEAR              0x0068024c /* [WO] Capture BVB Status Clear */
#define BCHP_CAP_1_FIFO_STATUS                   0x00680250 /* [RO] Capture FIFO Status */
#define BCHP_CAP_1_FIFO_STATUS_CLEAR             0x00680254 /* [WO] Capture FIFO Status Clear */
#define BCHP_CAP_1_DEBUG_STATUS                  0x00680258 /* [RO] Capture Debug Status Register */
#define BCHP_CAP_1_SCRATCH                       0x0068027c /* [RW] Scratch Register */

#endif /* #ifndef BCHP_CAP_1_H__ */

/* End of File */
