/***************************************************************************
 *     Copyright (c) 2007 Broadcom Corporation
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
 * BMedia library, Probe of AAC audio with ADTS encapsulation
 * 
 * Revision History:
 *
 * $brcm_Log: $
 * 
 *******************************************************************************/
#ifndef _BAAC_ADTS_PROBE_H__
#define _BAAC_ADTS_PROBE_H__

#include "bmedia_probe_es.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
#define B_AAC_ADTS_N_FRAMES	3
#define B_AAC_ADTS_SYNC	0xFFF
#define B_AAC_ADTS_SYNC_LEN	2

extern const bmedia_probe_es_desc baac_adts_probe;

size_t b_aac_adts_probe_parse_header(batom_cursor *cursor, bmedia_probe_audio *info, uint32_t sync_word);

#ifdef __cplusplus
}
#endif


#endif /* _BAAC_ADTS_PROBE_H__ */

