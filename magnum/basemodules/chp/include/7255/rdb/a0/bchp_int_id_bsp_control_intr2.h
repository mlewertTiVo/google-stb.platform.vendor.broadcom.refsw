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
 * Date:           Generated on               Tue Mar 14 11:30:52 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     1255
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/generate_int_id.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#include "bchp.h"
#include "bchp_bsp_control_intr2.h"

#ifndef BCHP_INT_ID_BSP_CONTROL_INTR2_H__
#define BCHP_INT_ID_BSP_CONTROL_INTR2_H__

#define BCHP_INT_ID_BSP_TO_HOST_CPU1_INTR     BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_BSP_TO_HOST_CPU1_INTR_SHIFT)
#define BCHP_INT_ID_BSP_TO_HOST_INTR          BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_BSP_TO_HOST_INTR_SHIFT)
#define BCHP_INT_ID_OCMDQUE1_INTR             BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_OCMDQUE1_INTR_SHIFT)
#define BCHP_INT_ID_OCMDQUE2_INTR             BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_OCMDQUE2_INTR_SHIFT)
#define BCHP_INT_ID_OCMDQUE_CR_INTR           BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_OCMDQUE_CR_INTR_SHIFT)
#define BCHP_INT_ID_PKL_INTR                  BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_PKL_INTR_SHIFT)
#define BCHP_INT_ID_SCPU_SCB_INIT_ACK         BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_SCPU_SCB_INIT_ACK_SHIFT)
#define BCHP_INT_ID_SECTOP_GR_ERROR_INTR      BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_SECTOP_GR_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_VISTA_INTR                BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_VISTA_INTR_SHIFT)
#define BCHP_INT_ID_ZZYZX_INTR                BCHP_INT_ID_CREATE(BCHP_BSP_CONTROL_INTR2_CPU_STATUS, BCHP_BSP_CONTROL_INTR2_CPU_STATUS_ZZYZX_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_BSP_CONTROL_INTR2_H__ */

/* End of File */
