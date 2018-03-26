/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.
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
 * Date:           Generated on               Wed May  3 10:59:24 2017
 *                 Full Compile MD5 Checksum  7702430a1e773a09e35f56c96ad0db97
 *                     (minus title and desc)
 *                 MD5 Checksum               f55a731609f2ef845af82a0a973340f6
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     1395
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              home/pntruong/sbin/generate_int_id.pl
 *                 DVTSWVER                   LOCAL home/pntruong/sbin/generate_int_id.pl
 *
 *
********************************************************************************/

#include "bchp.h"
#include "bchp_hvd_intr2_0.h"

#ifndef BCHP_INT_ID_HVD_INTR2_0_H__
#define BCHP_INT_ID_HVD_INTR2_0_H__

#define BCHP_INT_ID_AVD_MBOX_INTR             BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_MBOX_INTR_SHIFT)
#define BCHP_INT_ID_AVD_PFRI_INTR             BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_PFRI_INTR_SHIFT)
#define BCHP_INT_ID_AVD_RGR_BRIDGE_INTR       BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_RGR_BRIDGE_INTR_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR0              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR0_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR1              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR1_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR2              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR2_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR3              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR3_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR4              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR4_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR5              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR5_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR6              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR6_SHIFT)
#define BCHP_INT_ID_AVD_SW_INTR7              BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_SW_INTR7_SHIFT)
#define BCHP_INT_ID_AVD_WATCHDOG_INTR         BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_AVD_WATCHDOG_INTR_SHIFT)
#define BCHP_INT_ID_VICH_IL2_INST_RD_INTR     BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_VICH_IL2_INST_RD_INTR_SHIFT)
#define BCHP_INT_ID_VICH_IL_INST_RD_INTR      BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_VICH_IL_INST_RD_INTR_SHIFT)
#define BCHP_INT_ID_VICH_OL_INST_RD_INTR      BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_VICH_OL_INST_RD_INTR_SHIFT)
#define BCHP_INT_ID_VICH_REG_INTR             BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_VICH_REG_INTR_SHIFT)
#define BCHP_INT_ID_VICH_SCB_WR_INTR          BCHP_INT_ID_CREATE(BCHP_HVD_INTR2_0_CPU_STATUS, BCHP_HVD_INTR2_0_CPU_STATUS_VICH_SCB_WR_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_HVD_INTR2_0_H__ */

/* End of File */
