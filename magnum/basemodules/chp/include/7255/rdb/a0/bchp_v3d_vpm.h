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
 * Date:           Generated on               Tue Mar 14 11:30:19 2017
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

#ifndef BCHP_V3D_VPM_H__
#define BCHP_V3D_VPM_H__

/***************************************************************************
 *V3D_VPM - V3D Vertex Pipe Memory Allocation Registers
 ***************************************************************************/
#define BCHP_V3D_VPM_VPACNTL                     0x21300500 /* [RW][32] V3D Vertex Pipe Memory Allocation */
#define BCHP_V3D_VPM_VPMBASE                     0x21300504 /* [RW][32] V3D Vertex Pipe Memory Allocation */

/***************************************************************************
 *VPACNTL - V3D Vertex Pipe Memory Allocation
 ***************************************************************************/
/* V3D_VPM :: VPACNTL :: reserved0 [31:14] */
#define BCHP_V3D_VPM_VPACNTL_reserved0_MASK                        0xffffc000
#define BCHP_V3D_VPM_VPACNTL_reserved0_SHIFT                       14

/* V3D_VPM :: VPACNTL :: TIME_OUT_EN [13:13] */
#define BCHP_V3D_VPM_VPACNTL_TIME_OUT_EN_MASK                      0x00002000
#define BCHP_V3D_VPM_VPACNTL_TIME_OUT_EN_SHIFT                     13
#define BCHP_V3D_VPM_VPACNTL_TIME_OUT_EN_DEFAULT                   0x00000000

/* V3D_VPM :: VPACNTL :: ALLOC_EN [12:12] */
#define BCHP_V3D_VPM_VPACNTL_ALLOC_EN_MASK                         0x00001000
#define BCHP_V3D_VPM_VPACNTL_ALLOC_EN_SHIFT                        12
#define BCHP_V3D_VPM_VPACNTL_ALLOC_EN_DEFAULT                      0x00000000

/* V3D_VPM :: VPACNTL :: BIN_ALLOC_TIME_OUT [11:09] */
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_TIME_OUT_MASK               0x00000e00
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_TIME_OUT_SHIFT              9
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_TIME_OUT_DEFAULT            0x00000000

/* V3D_VPM :: VPACNTL :: REN_ALLOC_TIME_OUT [08:06] */
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_TIME_OUT_MASK               0x000001c0
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_TIME_OUT_SHIFT              6
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_TIME_OUT_DEFAULT            0x00000000

/* V3D_VPM :: VPACNTL :: BIN_ALLOC_LIMIT [05:03] */
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_LIMIT_MASK                  0x00000038
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_LIMIT_SHIFT                 3
#define BCHP_V3D_VPM_VPACNTL_BIN_ALLOC_LIMIT_DEFAULT               0x00000000

/* V3D_VPM :: VPACNTL :: REN_ALLOC_LIMIT [02:00] */
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_LIMIT_MASK                  0x00000007
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_LIMIT_SHIFT                 0
#define BCHP_V3D_VPM_VPACNTL_REN_ALLOC_LIMIT_DEFAULT               0x00000000

/***************************************************************************
 *VPMBASE - V3D Vertex Pipe Memory Allocation
 ***************************************************************************/
/* V3D_VPM :: VPMBASE :: reserved0 [31:05] */
#define BCHP_V3D_VPM_VPMBASE_reserved0_MASK                        0xffffffe0
#define BCHP_V3D_VPM_VPMBASE_reserved0_SHIFT                       5

/* V3D_VPM :: VPMBASE :: VPMBASE [04:00] */
#define BCHP_V3D_VPM_VPMBASE_VPMBASE_MASK                          0x0000001f
#define BCHP_V3D_VPM_VPMBASE_VPMBASE_SHIFT                         0
#define BCHP_V3D_VPM_VPMBASE_VPMBASE_DEFAULT                       0x00000000

#endif /* #ifndef BCHP_V3D_VPM_H__ */

/* End of File */
