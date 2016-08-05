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

#ifndef BCHP_BSCD_H__
#define BCHP_BSCD_H__

/***************************************************************************
 *BSCD - Broadcom Serial Control Master D
 ***************************************************************************/
#define BCHP_BSCD_CHIP_ADDRESS                   0x20419b80 /* [RW] BSC Chip Address And Read/Write Control */
#define BCHP_BSCD_DATA_IN0                       0x20419b84 /* [RW] BSC Write Data Register 0 */
#define BCHP_BSCD_DATA_IN1                       0x20419b88 /* [RW] BSC Write Data Register 1 */
#define BCHP_BSCD_DATA_IN2                       0x20419b8c /* [RW] BSC Write Data Register 2 */
#define BCHP_BSCD_DATA_IN3                       0x20419b90 /* [RW] BSC Write Data Register 3 */
#define BCHP_BSCD_DATA_IN4                       0x20419b94 /* [RW] BSC Write Data Register 4 */
#define BCHP_BSCD_DATA_IN5                       0x20419b98 /* [RW] BSC Write Data Register 5 */
#define BCHP_BSCD_DATA_IN6                       0x20419b9c /* [RW] BSC Write Data Register 6 */
#define BCHP_BSCD_DATA_IN7                       0x20419ba0 /* [RW] BSC Write Data Register 7 */
#define BCHP_BSCD_CNT_REG                        0x20419ba4 /* [RW] BSC Transfer Count Register */
#define BCHP_BSCD_CTL_REG                        0x20419ba8 /* [RW] BSC Control Register */
#define BCHP_BSCD_IIC_ENABLE                     0x20419bac /* [RW] BSC Read/Write Enable And Interrupt */
#define BCHP_BSCD_DATA_OUT0                      0x20419bb0 /* [RO] BSC Read Data Register 0 */
#define BCHP_BSCD_DATA_OUT1                      0x20419bb4 /* [RO] BSC Read Data Register 1 */
#define BCHP_BSCD_DATA_OUT2                      0x20419bb8 /* [RO] BSC Read Data Register 2 */
#define BCHP_BSCD_DATA_OUT3                      0x20419bbc /* [RO] BSC Read Data Register 3 */
#define BCHP_BSCD_DATA_OUT4                      0x20419bc0 /* [RO] BSC Read Data Register 4 */
#define BCHP_BSCD_DATA_OUT5                      0x20419bc4 /* [RO] BSC Read Data Register 5 */
#define BCHP_BSCD_DATA_OUT6                      0x20419bc8 /* [RO] BSC Read Data Register 6 */
#define BCHP_BSCD_DATA_OUT7                      0x20419bcc /* [RO] BSC Read Data Register 7 */
#define BCHP_BSCD_CTLHI_REG                      0x20419bd0 /* [RW] BSC Control Register */
#define BCHP_BSCD_SCL_PARAM                      0x20419bd4 /* [RW] BSC SCL Parameter Register */

#endif /* #ifndef BCHP_BSCD_H__ */

/* End of File */
