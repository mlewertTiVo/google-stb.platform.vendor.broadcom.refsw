/*
 * 802.11h CCA stats module header file
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 *
 * <<Broadcom-WL-IPTag/Proprietary:>>
 *
 * $Id$
*/

/**
 * Clear Channel Assessment is an 802.11 std term.
 * It assists in channel (re)selection and interference mitigation.
 */


#ifndef _wlc_cca_h_
#define _wlc_cca_h_

#define CCA_CHANNELS_NUM 39

#ifdef ISID_STATS
/* This is specially for dongle builds so not changing the value*/
#define CCA_POOL_MAX		450	/* bigger pool for interference samples */
#else
/* 
 * For non dongle builds, a random number was hard coded here as CCA POOL size. 
 * instead this should have been number of channels * number of seconds of history per channel.
 * The random number chosen earlier was less than the corrected number hence
 * cca pool was running out of space causing debug prints of cca_alloc_failures. 
 * Now, the cca watch dog will recycle cca pool as expected. 
 */
#define CCA_POOL_MAX		MAX_CCA_SECS*CCA_CHANNELS_NUM
#endif
#define CCA_FREE_BUF		0xffff

typedef uint16 cca_idx_t;
typedef struct {
	chanspec_t chanspec;
	cca_idx_t  secs[MAX_CCA_SECS];
} cca_congest_channel_t;

typedef struct {
	uint32 duration;	/* millisecs spent sampling this channel */
	uint32 congest_ibss;	/* millisecs in our bss (presumably this traffic will */
				/*  move if cur bss moves channels) */
	uint32 congest_obss;	/* traffic not in our bss */
	uint32 interference;	/* millisecs detecting a non 802.11 interferer. */
	uint32 timestamp;	/* second timestamp */
#ifdef ISID_STATS
	uint32 crsglitch;	/* crs glitchs */
	uint32 badplcp;		/* num bad plcp */
	uint32 bphy_crsglitch;	/* bphy  crs glitchs */
	uint32 bphy_badplcp;		/* num bphy bad plcp */
#endif /* ISID_STATS */
	uint32 congest_rx;
	uint32 congest_tx;
} wlc_congest_t;

typedef struct {
	chanspec_t chanspec;	/* Which channel? */
	uint8 num_secs;		/* How many secs worth of data */
	wlc_congest_t  secs[1];	/* Data */
} wlc_congest_channel_req_t;

extern void cca_stats_upd(wlc_info_t *wlc, int calculate);
extern void cca_stats_tsf_upd(wlc_info_t *wlc);
extern cca_info_t *wlc_cca_attach(wlc_info_t *wlc);
extern void wlc_cca_detach(cca_info_t *cca);
extern int cca_query_stats(wlc_info_t *wlc, chanspec_t chanspec, int nsecs,
	wlc_congest_channel_req_t *stats_results, int buflen);
extern chanspec_t wlc_cca_get_chanspec(wlc_info_t *wlc, int index);
extern int cca_send_event(wlc_info_t *wlc, bool forced);

extern bool wlc_cca_chan_qual_event_update(wlc_info_t *wlc, uint8 id, int v);

#endif /* _wlc_cca_h_ */
