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
 * Date:           Generated on         Tue Nov 17 17:15:52 2009
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

#ifndef BCHP_CAP_2_H__
#define BCHP_CAP_2_H__

/***************************************************************************
 *CAP_2 - Capture 0 Registers
 ***************************************************************************/
#define BCHP_CAP_2_REVISION                      0x00788400 /* Capture Engine Revision ID */
#define BCHP_CAP_2_PIC_SIZE                      0x00788404 /* Capture Vertical and Horizontal Size */
#define BCHP_CAP_2_PIC_OFFSET                    0x00788408 /* Capture Vertical and Horizontal Start Offset */
#define BCHP_CAP_2_BVB_IN_SIZE                   0x0078840c /* Capture BVB Source Vertical and Horizontal Size */
#define BCHP_CAP_2_MSTART                        0x00788410 /* Capture Memory Buffer Starting Address */
#define BCHP_CAP_2_PITCH                         0x00788414 /* Capture Memory Buffer Pitch */
#define BCHP_CAP_2_CTRL                          0x00788418 /* Capture Control */
#define BCHP_CAP_2_BYTE_ORDER                    0x00788424 /* Capture Byte Order Control */
#define BCHP_CAP_2_RX_CTRL                       0x00788428 /* Capture BVB Receiver Control */
#define BCHP_CAP_2_TRIG_CTRL                     0x0078842c /* Capture RDMA Trigger Control */
#define BCHP_CAP_2_BVB_TRIG_0_CFG                0x00788430 /* Capture BVB Trigger 0 Configuration */
#define BCHP_CAP_2_BVB_TRIG_1_CFG                0x00788434 /* Capture BVB Trigger 1 Configuration */
#define BCHP_CAP_2_LINE_CMP_TRIG_0_CFG           0x00788438 /* Capture Line Compare Trigger 0 Configuration */
#define BCHP_CAP_2_LINE_CMP_TRIG_1_CFG           0x0078843c /* Capture Line Compare Trigger 1 Configuration */
#define BCHP_CAP_2_STATUS                        0x00788440 /* Capture Engine Status */
#define BCHP_CAP_2_BVB_STATUS                    0x00788444 /* Capture BVB Status */
#define BCHP_CAP_2_BVB_STATUS_CLEAR              0x00788448 /* Capture BVB Status Clear */
#define BCHP_CAP_2_FIFO_STATUS                   0x0078844c /* Capture FIFO Status */
#define BCHP_CAP_2_FIFO_STATUS_CLEAR             0x00788450 /* Capture FIFO Status Clear */
#define BCHP_CAP_2_SCRATCH                       0x0078847c /* Scratch Register */

#endif /* #ifndef BCHP_CAP_2_H__ */

/* End of File */
