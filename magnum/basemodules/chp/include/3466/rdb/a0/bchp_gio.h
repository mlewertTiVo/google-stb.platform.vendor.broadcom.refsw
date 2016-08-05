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
 * Date:           Generated on               Tue May  3 15:23:05 2016
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

#ifndef BCHP_GIO_H__
#define BCHP_GIO_H__

/***************************************************************************
 *GIO - GPIO
 ***************************************************************************/
#define BCHP_GIO_RSVDO                           0x04821a00 /* [RW] RESERVED0 */
#define BCHP_GIO_RSVD1                           0x04821a04 /* [RW] RESERVED1 */
#define BCHP_GIO_RSVD2                           0x04821a08 /* [RW] RESERVED2 */
#define BCHP_GIO_EC_LO                           0x04821a0c /* [RW] GENERAL PURPOSE I/O EDGE CONFIGURATION */
#define BCHP_GIO_EI_LO                           0x04821a10 /* [RW] GENERAL PURPOSE I/O EDGE INSENSITIVE */
#define BCHP_GIO_MASK_LO                         0x04821a14 /* [RW] GENERAL PURPOSE I/O INTERRUPT MASK */
#define BCHP_GIO_LEVEL_LO                        0x04821a18 /* [RW] GENERAL PURPOSE I/O INTERRUPT TYPE */
#define BCHP_GIO_STAT_LO                         0x04821a1c /* [RW] GENERAL PURPOSE I/O INTERRUPT STATUS */

/***************************************************************************
 *RSVDO - RESERVED0
 ***************************************************************************/
/* GIO :: RSVDO :: reserved0 [31:00] */
#define BCHP_GIO_RSVDO_reserved0_MASK                              0xffffffff
#define BCHP_GIO_RSVDO_reserved0_SHIFT                             0

/***************************************************************************
 *RSVD1 - RESERVED1
 ***************************************************************************/
/* GIO :: RSVD1 :: reserved0 [31:00] */
#define BCHP_GIO_RSVD1_reserved0_MASK                              0xffffffff
#define BCHP_GIO_RSVD1_reserved0_SHIFT                             0

/***************************************************************************
 *RSVD2 - RESERVED2
 ***************************************************************************/
/* GIO :: RSVD2 :: reserved0 [31:00] */
#define BCHP_GIO_RSVD2_reserved0_MASK                              0xffffffff
#define BCHP_GIO_RSVD2_reserved0_SHIFT                             0

/***************************************************************************
 *EC_LO - GENERAL PURPOSE I/O EDGE CONFIGURATION
 ***************************************************************************/
/* GIO :: EC_LO :: reserved0 [31:10] */
#define BCHP_GIO_EC_LO_reserved0_MASK                              0xfffffc00
#define BCHP_GIO_EC_LO_reserved0_SHIFT                             10

/* GIO :: EC_LO :: edge_config [09:00] */
#define BCHP_GIO_EC_LO_edge_config_MASK                            0x000003ff
#define BCHP_GIO_EC_LO_edge_config_SHIFT                           0
#define BCHP_GIO_EC_LO_edge_config_DEFAULT                         0x00000000

/***************************************************************************
 *EI_LO - GENERAL PURPOSE I/O EDGE INSENSITIVE
 ***************************************************************************/
/* GIO :: EI_LO :: reserved0 [31:10] */
#define BCHP_GIO_EI_LO_reserved0_MASK                              0xfffffc00
#define BCHP_GIO_EI_LO_reserved0_SHIFT                             10

/* GIO :: EI_LO :: edge_insensitive [09:00] */
#define BCHP_GIO_EI_LO_edge_insensitive_MASK                       0x000003ff
#define BCHP_GIO_EI_LO_edge_insensitive_SHIFT                      0
#define BCHP_GIO_EI_LO_edge_insensitive_DEFAULT                    0x00000000

/***************************************************************************
 *MASK_LO - GENERAL PURPOSE I/O INTERRUPT MASK
 ***************************************************************************/
/* GIO :: MASK_LO :: reserved0 [31:10] */
#define BCHP_GIO_MASK_LO_reserved0_MASK                            0xfffffc00
#define BCHP_GIO_MASK_LO_reserved0_SHIFT                           10

/* GIO :: MASK_LO :: irq_mask [09:00] */
#define BCHP_GIO_MASK_LO_irq_mask_MASK                             0x000003ff
#define BCHP_GIO_MASK_LO_irq_mask_SHIFT                            0
#define BCHP_GIO_MASK_LO_irq_mask_DEFAULT                          0x00000000

/***************************************************************************
 *LEVEL_LO - GENERAL PURPOSE I/O INTERRUPT TYPE
 ***************************************************************************/
/* GIO :: LEVEL_LO :: reserved0 [31:10] */
#define BCHP_GIO_LEVEL_LO_reserved0_MASK                           0xfffffc00
#define BCHP_GIO_LEVEL_LO_reserved0_SHIFT                          10

/* GIO :: LEVEL_LO :: level [09:00] */
#define BCHP_GIO_LEVEL_LO_level_MASK                               0x000003ff
#define BCHP_GIO_LEVEL_LO_level_SHIFT                              0
#define BCHP_GIO_LEVEL_LO_level_DEFAULT                            0x00000000

/***************************************************************************
 *STAT_LO - GENERAL PURPOSE I/O INTERRUPT STATUS
 ***************************************************************************/
/* GIO :: STAT_LO :: reserved0 [31:10] */
#define BCHP_GIO_STAT_LO_reserved0_MASK                            0xfffffc00
#define BCHP_GIO_STAT_LO_reserved0_SHIFT                           10

/* GIO :: STAT_LO :: irq_status [09:00] */
#define BCHP_GIO_STAT_LO_irq_status_MASK                           0x000003ff
#define BCHP_GIO_STAT_LO_irq_status_SHIFT                          0
#define BCHP_GIO_STAT_LO_irq_status_DEFAULT                        0x00000000

#endif /* #ifndef BCHP_GIO_H__ */

/* End of File */
