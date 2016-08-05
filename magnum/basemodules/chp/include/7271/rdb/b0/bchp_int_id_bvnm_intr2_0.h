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
 * Date:           Generated on               Thu Jun 16 14:48:13 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     1009
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/generate_int_id.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#include "bchp.h"
#include "bchp_bvnm_intr2_0.h"

#ifndef BCHP_INT_ID_BVNM_INTR2_0_H__
#define BCHP_INT_ID_BVNM_INTR2_0_H__

#define BCHP_INT_ID_DNR_0_ERR_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_DNR_0_ERR_INTR_SHIFT)
#define BCHP_INT_ID_DNR_1_ERR_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_DNR_1_ERR_INTR_SHIFT)
#define BCHP_INT_ID_HSCL_0_INTR               BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_HSCL_0_INTR_SHIFT)
#define BCHP_INT_ID_HSCL_1_INTR               BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_HSCL_1_INTR_SHIFT)
#define BCHP_INT_ID_MDI_0_BVB_IN_INTR         BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_MDI_0_BVB_IN_INTR_SHIFT)
#define BCHP_INT_ID_MDI_1_BVB_IN_INTR         BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_MDI_1_BVB_IN_INTR_SHIFT)
#define BCHP_INT_ID_MVP_TOP_0_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_MVP_TOP_0_INTR_SHIFT)
#define BCHP_INT_ID_MVP_TOP_1_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_MVP_TOP_1_INTR_SHIFT)
#define BCHP_INT_ID_SCL_0_ERR_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_SCL_0_ERR_INTR_SHIFT)
#define BCHP_INT_ID_SCL_1_ERR_INTR            BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_SCL_1_ERR_INTR_SHIFT)
#define BCHP_INT_ID_XSRC_0_ERR_INTR           BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_XSRC_0_ERR_INTR_SHIFT)
#define BCHP_INT_ID_XSRC_1_ERR_INTR           BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_XSRC_1_ERR_INTR_SHIFT)
#define BCHP_INT_ID_XSRC_2_ERR_INTR           BCHP_INT_ID_CREATE(BCHP_BVNM_INTR2_0_R5F_STATUS, BCHP_BVNM_INTR2_0_R5F_STATUS_XSRC_2_ERR_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_BVNM_INTR2_0_H__ */

/* End of File */
