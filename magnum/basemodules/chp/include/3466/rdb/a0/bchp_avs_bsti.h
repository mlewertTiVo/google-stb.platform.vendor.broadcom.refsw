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
 *
 * Date:           Generated on               Tue May  3 15:23:06 2016
 *                 Full Compile MD5 Checksum  1da1c3226a9ac5d06cc27c06c1aefe52
 *                     (minus title and desc)
 *                 MD5 Checksum               a58ec1309d39852a0c8fb54050da5f77
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     930
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_AVS_BSTI_H__
#define BCHP_AVS_BSTI_H__

/***************************************************************************
 *AVS_BSTI - BSTI Registers
 ***************************************************************************/
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA        0x04811400 /* [RW] Dedicate to issue BSTI commands which includes access type, address and write data. (NOTE: A write to the register will trigger a transaction on the BSTI) */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA          0x04811404 /* [RO] Indicate the status of BSTI and read data */

/***************************************************************************
 *BSTI_CMD_ADDR_WDATA - Dedicate to issue BSTI commands which includes access type, address and write data. (NOTE: A write to the register will trigger a transaction on the BSTI)
 ***************************************************************************/
/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: reserved0 [31:29] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved0_MASK           0xe0000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved0_SHIFT          29

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: read_write [28:28] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_MASK          0x10000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_SHIFT         28
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_DEFAULT       0x00000000

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: reserved1 [27:26] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved1_MASK           0x0c000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved1_SHIFT          26

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: address [25:16] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_MASK             0x03ff0000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_SHIFT            16
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_DEFAULT          0x00000000

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: write_data [15:00] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_MASK          0x0000ffff
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_SHIFT         0
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_DEFAULT       0x00000000

/***************************************************************************
 *BSTI_STATUS_RDATA - Indicate the status of BSTI and read data
 ***************************************************************************/
/* AVS_BSTI :: BSTI_STATUS_RDATA :: reserved0 [31:17] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_reserved0_MASK             0xfffe0000
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_reserved0_SHIFT            17

/* AVS_BSTI :: BSTI_STATUS_RDATA :: busy [16:16] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_MASK                  0x00010000
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_SHIFT                 16
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_DEFAULT               0x00000000

/* AVS_BSTI :: BSTI_STATUS_RDATA :: read_data [15:00] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_MASK             0x0000ffff
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_SHIFT            0
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_DEFAULT          0x00000000

#endif /* #ifndef BCHP_AVS_BSTI_H__ */

/* End of File */
