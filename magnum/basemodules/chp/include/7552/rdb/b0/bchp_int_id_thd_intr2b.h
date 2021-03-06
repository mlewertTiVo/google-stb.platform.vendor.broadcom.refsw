/***************************************************************************
 *     Copyright (c) 1999-2011, Broadcom Corporation
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
 * Date:           Generated on         Thu Aug  4 13:55:37 2011
 *                 MD5 Checksum         83f408cc25eb2d099cc58e22e4e239e9
 *
 * Compiled with:  RDB Utility          unknown
 *                 RDB Parser           3.0
 *                 generate_int_id.pl   1.0
 *                 Perl Interpreter     5.008005
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#include "bchp.h"
#include "bchp_thd_intr2b.h"

#ifndef BCHP_INT_ID_THD_INTR2B_H__
#define BCHP_INT_ID_THD_INTR2B_H__

#define BCHP_INT_ID_RS_CBERC_B_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CBERC_B_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_CBERC_C_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CBERC_C_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_CBERC_SAT_INTR         BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CBERC_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_CERC_B_SAT_INTR        BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CERC_B_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_CERC_C_SAT_INTR        BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CERC_C_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_CERC_SAT_INTR          BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_CERC_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_NBERC_B_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_NBERC_B_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_NBERC_C_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_NBERC_C_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_NBERC_SAT_INTR         BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_NBERC_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_B_INTR            BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_B_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_C_INTR            BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_C_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_INTR              BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_LOSS_B_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_LOSS_B_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_LOSS_C_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_LOSS_C_INTR_SHIFT)
#define BCHP_INT_ID_RS_SYNC_LOSS_INTR         BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_SYNC_LOSS_INTR_SHIFT)
#define BCHP_INT_ID_RS_UBERC_B_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_UBERC_B_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_UBERC_C_SAT_INTR       BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_UBERC_C_SAT_INTR_SHIFT)
#define BCHP_INT_ID_RS_UBERC_SAT_INTR         BCHP_INT_ID_CREATE(BCHP_THD_INTR2B_CPU_STATUS, BCHP_THD_INTR2B_CPU_STATUS_RS_UBERC_SAT_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_THD_INTR2B_H__ */

/* End of File */
