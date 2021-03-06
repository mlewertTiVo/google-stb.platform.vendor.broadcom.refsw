/***************************************************************************
 *     Copyright (c) 2006-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 *******************************************************************************/
#ifndef _BRTP_PARSER_H263_H__
#define _BRTP_PARSER_H263_H__

#include "brtp.h"
#include "brtp_parser.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct brtp_parser_h263_cfg {
	size_t meta_len;
	void *meta;
}brtp_parser_h263_cfg;

extern const brtp_parser_type *brtp_parser_h263;

void brtp_parser_h263_default_cfg(brtp_parser_h263_cfg *cfg);
brtp_parser_t brtp_parser_h263_create(const brtp_parser_h263_cfg *cfg);

#ifdef __cplusplus
}
#endif

#endif /* _BRTP_PARSER_H263_H__ */

