/***************************************************************************
 *     Copyright (c) 1999-2010, Broadcom Corporation
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
 * Date:           Generated on         Fri Jan 22 20:03:05 2010
 *                 MD5 Checksum         a2d1f2163f65e87d228a0fb491cb442d
 *
 * Compiled with:  RDB Utility          unknown
 *                 RDB Parser           3.0
 *                 generate_int_id.pl   1.0
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#include "bchp.h"
#include "bchp_memc_l2_0.h"

#ifndef BCHP_INT_ID_MEMC_L2_0_H__
#define BCHP_INT_ID_MEMC_L2_0_H__

#define BCHP_INT_ID_ARC_0_INTR                BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_ARC_0_INTR_SHIFT)
#define BCHP_INT_ID_ARC_1_INTR                BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_ARC_1_INTR_SHIFT)
#define BCHP_INT_ID_ARC_2_INTR                BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_ARC_2_INTR_SHIFT)
#define BCHP_INT_ID_ARC_3_INTR                BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_ARC_3_INTR_SHIFT)
#define BCHP_INT_ID_BSP_ARCH_INTR             BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_BSP_ARCH_INTR_SHIFT)
#define BCHP_INT_ID_BSP_WRCH_INTR             BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_BSP_WRCH_INTR_SHIFT)
#define BCHP_INT_ID_GSIZE_VIOL_INTR_PFRI_0    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_GSIZE_VIOL_INTR_PFRI_0_SHIFT)
#define BCHP_INT_ID_GSIZE_VIOL_INTR_PFRI_1    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_GSIZE_VIOL_INTR_PFRI_1_SHIFT)
#define BCHP_INT_ID_GSIZE_VIOL_INTR_PFRI_2    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_GSIZE_VIOL_INTR_PFRI_2_SHIFT)
#define BCHP_INT_ID_GSIZE_VIOL_INTR_PFRI_3    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_GSIZE_VIOL_INTR_PFRI_3_SHIFT)
#define BCHP_INT_ID_INVALID_CMD_INTR          BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_INVALID_CMD_INTR_SHIFT)
#define BCHP_INT_ID_INVALID_MSTART_INTR       BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_INVALID_MSTART_INTR_SHIFT)
#define BCHP_INT_ID_INVALID_NMBX_INTR         BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_INVALID_NMBX_INTR_SHIFT)
#define BCHP_INT_ID_MISSING_EOG_INTR_PFRI_0   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_EOG_INTR_PFRI_0_SHIFT)
#define BCHP_INT_ID_MISSING_EOG_INTR_PFRI_1   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_EOG_INTR_PFRI_1_SHIFT)
#define BCHP_INT_ID_MISSING_EOG_INTR_PFRI_2   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_EOG_INTR_PFRI_2_SHIFT)
#define BCHP_INT_ID_MISSING_EOG_INTR_PFRI_3   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_EOG_INTR_PFRI_3_SHIFT)
#define BCHP_INT_ID_MISSING_LAST_WRITE_INTR   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_LAST_WRITE_INTR_SHIFT)
#define BCHP_INT_ID_MISSING_SOG_INTR_PFRI_0   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_SOG_INTR_PFRI_0_SHIFT)
#define BCHP_INT_ID_MISSING_SOG_INTR_PFRI_1   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_SOG_INTR_PFRI_1_SHIFT)
#define BCHP_INT_ID_MISSING_SOG_INTR_PFRI_2   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_SOG_INTR_PFRI_2_SHIFT)
#define BCHP_INT_ID_MISSING_SOG_INTR_PFRI_3   BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_MISSING_SOG_INTR_PFRI_3_SHIFT)
#define BCHP_INT_ID_NO_REQ_INTR               BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_NO_REQ_INTR_SHIFT)
#define BCHP_INT_ID_PAGE_BREAK_INTR_PFRI_0    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_PAGE_BREAK_INTR_PFRI_0_SHIFT)
#define BCHP_INT_ID_PAGE_BREAK_INTR_PFRI_1    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_PAGE_BREAK_INTR_PFRI_1_SHIFT)
#define BCHP_INT_ID_PAGE_BREAK_INTR_PFRI_2    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_PAGE_BREAK_INTR_PFRI_2_SHIFT)
#define BCHP_INT_ID_PAGE_BREAK_INTR_PFRI_3    BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_PAGE_BREAK_INTR_PFRI_3_SHIFT)
#define BCHP_INT_ID_RGR_BRIDGE_INTR           BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_RGR_BRIDGE_INTR_SHIFT)
#define BCHP_INT_ID_SM_TIMEOUT_INTR           BCHP_INT_ID_CREATE(BCHP_MEMC_L2_0_R5F_STATUS, BCHP_MEMC_L2_0_R5F_STATUS_SM_TIMEOUT_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_MEMC_L2_0_H__ */

/* End of File */
