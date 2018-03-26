/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2017 Broadcom. All rights reserved.
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
 * Date:           Generated on               Tue Mar 14 11:30:18 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1255
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#ifndef BCHP_SCB_H__
#define BCHP_SCB_H__

/***************************************************************************
 *SCB - Smart Card B
 ***************************************************************************/
#define BCHP_SCB_UART_CMD                        0x2040a900 /* [RW][32] Smart Card Uart Command */
#define BCHP_SCB_IF_CMD                          0x2040a904 /* [RW][32] Smart Card Interface Command */
#define BCHP_SCB_CLK_CMD                         0x2040a908 /* [RW][32] Smart Card Clock Command */
#define BCHP_SCB_PROTO_CMD                       0x2040a90c /* [RW][32] Smart Card Protocol Command */
#define BCHP_SCB_PRESCALE                        0x2040a910 /* [RW][32] Smart Card Clock Prescale */
#define BCHP_SCB_TGUARD                          0x2040a914 /* [RW][32] Smart Card Transmit Guard Time */
#define BCHP_SCB_TRANSMIT                        0x2040a918 /* [RW][32] Smart Card Transmit */
#define BCHP_SCB_RECEIVE                         0x2040a91c /* [RO][32] Smart Card Receive */
#define BCHP_SCB_TLEN                            0x2040a924 /* [RW][32] Smart Card Transmit Length */
#define BCHP_SCB_FLOW_CMD                        0x2040a928 /* [RW][32] Smart Card Flow Command */
#define BCHP_SCB_RLEN                            0x2040a930 /* [RO][32] Smart Card Receive Length */
#define BCHP_SCB_STATUS                          0x2040a934 /* [RO][32] Smart Card Status */
#define BCHP_SCB_BGT                             0x2040a944 /* [RW][32] Smart Card Block Guard Time */
#define BCHP_SCB_TIMER_CMD                       0x2040a948 /* [RW][32] Smart Card Timer Command */
#define BCHP_SCB_INTR_EN                         0x2040a950 /* [RW][32] Smart Card Interrupt Enable */
#define BCHP_SCB_INTR_STAT                       0x2040a958 /* [RO][32] Smart Card Interrupt Status */
#define BCHP_SCB_TIMER_CMP                       0x2040a960 /* [RW][32] Smart Card General Purpose Timer Compare */
#define BCHP_SCB_TIMER_CNT                       0x2040a968 /* [RO][32] Smart Card General Purpose Timer Count */
#define BCHP_SCB_WAIT                            0x2040a970 /* [RW][32] Smart Card Waiting Timer */
#define BCHP_SCB_EVENT1_CNT                      0x2040a980 /* [RO][32] Smart Card Event 1 Count */
#define BCHP_SCB_EVENT1_CMP                      0x2040a988 /* [RW][32] Smart Card Event 1 Compare */
#define BCHP_SCB_EVENT1_CMD                      0x2040a990 /* [RW][32] Smart Card Event 1 Command */
#define BCHP_SCB_EVENT2_CMP                      0x2040a9a0 /* [RW][32] Smart Card Event 2 Compare */
#define BCHP_SCB_EVENT2_CNT                      0x2040a9a8 /* [RO][32] Smart Card Event 2 Count */
#define BCHP_SCB_EVENT2_CMD                      0x2040a9b0 /* [RW][32] Smart Card Event 2 Command */
#define BCHP_SCB_SMODE_TICK                      0x2040a9c0 /* [RW][32] Smart Card Synchronous Mode Timebase */
#define BCHP_SCB_IOIF_TICK                       0x2040a9c4 /* [RW][32] Smart Card I/O Interface Timebase */
#define BCHP_SCB_SMODE_RLEN                      0x2040a9c8 /* [RW][32] Smart Card Synchronous Receive Length */
#define BCHP_SCB_AFE_CMD_1                       0x2040a9cc /* [RW][32] Smart Card Analog Front End Command 1 */
#define BCHP_SCB_AFE_CMD_2                       0x2040a9d0 /* [RW][32] Smart Card Analog Front End Command 2 */
#define BCHP_SCB_TMON_CMP                        0x2040a9d4 /* [RW][32] Smart Card Temperature Monitor Compare Threshold */
#define BCHP_SCB_TMON_TEMP                       0x2040a9d8 /* [RO][32] Smart Card Interface Contact Temperature */
#define BCHP_SCB_REVISION_ID                     0x2040a9fc /* [RO][32] Smart Card Revision ID */

#endif /* #ifndef BCHP_SCB_H__ */

/* End of File */
