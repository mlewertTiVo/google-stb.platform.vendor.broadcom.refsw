/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Thu Jun 16 14:47:09 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1009
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_XPT_DPCR_PP_H__
#define BCHP_XPT_DPCR_PP_H__

/***************************************************************************
 *XPT_DPCR_PP - XPT DPCR Packet Processor Control Registers
 ***************************************************************************/
#define BCHP_XPT_DPCR_PP_PP_CTRL                 0x20a02400 /* [RW] Data Transport PCR Packet Processor Control Register */
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET         0x20a02404 /* [RW] Data Transport PCR Packet Processor Fixed Offset Register */

/***************************************************************************
 *PP_CTRL - Data Transport PCR Packet Processor Control Register
 ***************************************************************************/
/* XPT_DPCR_PP :: PP_CTRL :: reserved0 [31:23] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved0_MASK                    0xff800000
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved0_SHIFT                   23

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_AFID_FIELD [22:22] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_MASK 0x00400000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_SHIFT 22
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CFF_FIELD [21:21] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_MASK  0x00200000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_SHIFT 21
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_RTS00_FIELD [20:20] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_MASK 0x00100000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_SHIFT 20
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_AFS_FIELD [19:19] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_MASK  0x00080000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_SHIFT 19
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CC_FIELD [18:18] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_MASK   0x00040000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_SHIFT  18
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_OCF_FIELD [17:17] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_MASK  0x00020000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_SHIFT 17
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CF_FIELD [16:16] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_MASK   0x00010000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_SHIFT  16
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: reserved1 [15:10] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved1_MASK                    0x0000fc00
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved1_SHIFT                   10

/* XPT_DPCR_PP :: PP_CTRL :: PP_SOFT_INIT [09:09] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_MASK                 0x00000200
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_SHIFT                9
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_DEFAULT              0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_PKT_ERR_DIS [08:08] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_MASK               0x00000100
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_SHIFT              8
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_DEFAULT            0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: reserved2 [07:05] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved2_MASK                    0x000000e0
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved2_SHIFT                   5

/* XPT_DPCR_PP :: PP_CTRL :: PP_JITTER_TIMESTAMP_MODE [04:03] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_MASK     0x00000018
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_SHIFT    3
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_DEFAULT  0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_FIXED_OFFSET_EN [02:02] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_MASK           0x00000004
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_SHIFT          2
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_DEFAULT        0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_PLAYBACK_PCR_JITTER_DIS [01:01] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_MASK   0x00000002
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_SHIFT  1
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_DEFAULT 0x00000000

/* XPT_DPCR_PP :: PP_CTRL :: PP_LIVE_PCR_JITTER_DIS [00:00] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_MASK       0x00000001
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_SHIFT      0
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_DEFAULT    0x00000000

/***************************************************************************
 *PP_FIXED_OFFSET - Data Transport PCR Packet Processor Fixed Offset Register
 ***************************************************************************/
/* XPT_DPCR_PP :: PP_FIXED_OFFSET :: PP_FIXED_OFFSET [31:00] */
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_MASK      0xffffffff
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_SHIFT     0
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_DEFAULT   0x00000000

#endif /* #ifndef BCHP_XPT_DPCR_PP_H__ */

/* End of File */
