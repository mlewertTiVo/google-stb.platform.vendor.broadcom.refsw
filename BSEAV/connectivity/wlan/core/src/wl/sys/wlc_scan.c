/*
 * Common (OS-independent) portion of
 * Broadcom 802.11abg Networking Device Driver
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



#include <wlc_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <bcmendian.h>
#include <proto/802.1d.h>
#include <proto/802.11.h>
#include <proto/802.11e.h>
#ifdef	BCMCCX
#include <proto/802.11_ccx.h>
#endif	/* BCMCCX */
#include <proto/wpa.h>
#include <proto/vlan.h>
#include <sbconfig.h>
#include <pcicfg.h>
#include <bcmsrom.h>
#include <wlioctl.h>
#include <epivers.h>
#ifdef BCMCCX
#include <bcmcrypto/ccx.h>
#endif /* BCMCCX */
#include <d11.h>
#include <wlc_rate.h>
#include <wlc_pub.h>
#include <wlc_bsscfg.h>
#include <wlc_channel.h>
#include <wlc_scandb.h>
#include <wlc.h>
#include <wlc_hw.h>
#include <wlc_tx.h>
#include <wlc_phy_hal.h>
#include <phy_utils_api.h>
#include <phy_misc_api.h>
#include <wlc_event.h>
#include <wl_export.h>
#include <wlc_rm.h>
#ifdef BCMCCX
#include <wlc_ccx.h>
#endif	/* BCMCCX */
#include <wlc_ap.h>
#include <wlc_assoc.h>
#include <wlc_scan_priv.h>
#include <wlc_scan.h>
#ifdef WLP2P
#include <wlc_p2p.h>
#endif
#include <wlc_11h.h>
#include <wlc_11d.h>
#include <wlc_dfs.h>
#ifdef ANQPO
#include <wl_anqpo.h>
#endif
#include <wlc_hw_priv.h>
#ifdef WL11K
#include <wlc_rrm.h>
#endif /* WL11K */
#include <wlc_obss.h>
#include <wlc_objregistry.h>
#ifdef WLRSDB
#include <wlc_rsdb.h>
#endif /* WLRSDB */
#ifdef WL_EXCESS_PMWAKE
#include <wlc_pm.h>
#endif /* WL_EXCESS_PMWAKE */
#ifdef WLPFN
#include <wl_pfn.h>
#endif
#include <wlc_bmac.h>
#include <wlc_lq.h>
#ifdef WLSCAN_PS
#include <wlc_stf.h>
#endif
#ifdef WLMESH
#include <wlc_mesh.h>
#include <wlc_mesh_peer.h>
#endif
#include <wlc_utils.h>
#include <wlc_msch.h>
#include <wlc_event_utils.h>
/* TODO: remove wlc_scan_utils.h dependency */
#include <wlc_scan_utils.h>
#include <wlc_dump.h>
#include <wlc_iocv.h>
#include <wlc_chanctxt.h>
#include <wlc_pm.h>
#include <wlc_dfs.h>
#ifdef WL_OCE
#include <wlc_oce.h>
#endif

#if defined(BCMDBG) || defined(WLMSG_INFORM)
#define	WL_INFORM_SCAN(args)                                                                    \
	do {                                                                            \
		if ((wl_msg_level & WL_INFORM_VAL) || (wl_msg_level2 & WL_SCAN_VAL))    \
		WL_PRINT(args);                                     \
	} while (0)
#undef WL_INFORM_ON
#define WL_INFORM_ON() ((wl_msg_level & WL_INFORM_VAL) || (wl_msg_level2 & WL_SCAN_VAL))
#else
#define        WL_INFORM_SCAN(args)
#endif /* BCMDBG */


#ifdef WLMCNX
#include <wlc_mcnx.h>
#endif /* WLMCNX */

/* scan times in milliseconds */
#define WLC_SCAN_MIN_PROBE_TIME	10	/* minimum useful time for an active probe */
#define WLC_SCAN_HOME_TIME	45	/* time for home channel processing */
#define WLC_SCAN_ASSOC_TIME 20 /* time to listen on a channel for probe resp while associated */

#ifdef BCMQT_CPU
#define WLC_SCAN_UNASSOC_TIME 400	/* qt is slow */
#else
#define WLC_SCAN_UNASSOC_TIME 40	/* listen on a channel for prb rsp while unassociated */
#endif
#define WLC_SCAN_NPROBES	2	/* do this many probes on each channel for an active scan */

/* scan_pass state values */

/* Enables the iovars */
#define WLC_SCAN_IOVARS

#if defined(BRINGUP_BUILD)
#undef WLC_SCAN_IOVARS
#endif
#ifdef WLOLPC
#include <wlc_olpc_engine.h>
#endif /* WLOLPC */

static void wlc_scan_watchdog(void *hdl);

static void wlc_scan_channels(scan_info_t *scan_info, chanspec_t *chanspec_list,
	int *pchannel_num, int channel_max, chanspec_t chanspec_start, int channel_type,
	int band);

static void wlc_scantimer(void *arg);

static int scanmac_get(scan_info_t *scan_info, void *params, uint p_len, void *arg, int len);
static int scanmac_set(scan_info_t *scan_info, void *arg, int len);

static
int _wlc_scan(
	wlc_scan_info_t *wlc_scan_info,
	int bss_type,
	const struct ether_addr* bssid,
	int nssid,
	wlc_ssid_t *ssid,
	int scan_type,
	int nprobes,
	int active_time,
	int passive_time,
	int home_time,
	const chanspec_t* chanspec_list, int channel_num, chanspec_t chanspec_start,
	bool save_prb,
	scancb_fn_t fn, void* arg,
	int away_channels_limit,
	bool extdscan,
	bool suppress_ssid,
	bool include_cache,
	uint scan_flags,
	wlc_bsscfg_t *cfg,
	actcb_fn_t act_cb, void *act_cb_arg, int bandinfo, chanspec_t band_chanspec_start,
	struct ether_addr *sa_override);

#ifdef WLRSDB
static void wlc_parallel_scan_cb(void *arg, int status, wlc_bsscfg_t *cfg);
static void wlc_scan_free_chanspec_list(scan_info_t *scan_info);
static int wlc_scan_split_channels_per_band(scan_info_t *scan_info,
	const chanspec_t* chanspec_list, int chan_num, chanspec_t** chanspec_set0,
	chanspec_t** chanspec_set1, int *channel_set0_num, int *channel_set1_num, int active_time,
	int passive_time, int scan_type);
static void wlc_scan_get_parallel_passive_setting(scan_info_t *scan_info,
	bool *parallel_passive, bool *passive_channels_first);
static void wlc_scan_handle_req_end_for_pscan(wlc_info_t *wlc);
static bool wlc_scan_check_disable_pscan_wlc(scan_info_t *scan_info);
static void wlc_scan_disable_idle_pscan_wlc(scan_info_t *scan_info);
static void wlc_scan_enable_idle_pscan_wlc(wlc_scan_info_t *wlc_scan_info);
#endif /* WLRSDB */

#ifdef WLC_SCAN_IOVARS
static int wlc_scan_doiovar(void *hdl, uint32 actionid,
	void *params, uint p_len, void *arg,
	uint len, uint val_size, struct wlc_if *wlcif);
#endif /* WLC_SCAN_IOVARS */

static void wlc_scan_callback(scan_info_t *scan_info, uint status);
static int wlc_scan_apply_scanresults(scan_info_t *scan_info, int status);

static uint wlc_scan_prohibited_channels(scan_info_t *scan_info,
	chanspec_t *chanspec_list, int channel_max);
static void wlc_scan_do_pass(scan_info_t *scan_info, chanspec_t chanspec);
static void wlc_scan_sendprobe(scan_info_t *scan_info);
static void wlc_scan_ssidlist_free(scan_info_t *scan_info);
static int wlc_scan_chnsw_clbk(void* handler_ctxt, wlc_msch_cb_info_t *cb_info);
static int _wlc_scan_schd_channels(wlc_scan_info_t *wlc_scan_info);
static int _wlc_scan_schd_channels_complete(wlc_scan_info_t *wlc_scan_info,
	void *cb, bool FIXED_SLOT);
#ifdef WLRSDB
static uint32 wlc_scan_get_current_home_time(scan_info_t *scan_info);
#endif

#if defined(BCMDBG) || defined(WLMSG_INFORM)
static void wlc_scan_print_ssids(wlc_ssid_t *ssid, int nssid);
#endif
#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static int wlc_scan_dump(scan_info_t *si, struct bcmstrbuf *b);
#endif

#ifdef WL11N
static void wlc_ht_obss_scan_update(scan_info_t *scan_info, int status);
#else /* WL11N */
#define wlc_ht_obss_scan_update(a, b) do {} while (0)
#endif /* WL11N */

#ifdef WLSCANCACHE
static void wlc_scan_merge_cache(scan_info_t *scan_info, uint current_timestamp,
                                 const struct ether_addr *BSSID, int nssid, const wlc_ssid_t *SSID,
                                 int BSS_type, const chanspec_t *chanspec_list, uint chanspec_num,
                                 wlc_bss_list_t *bss_list);
static void wlc_scan_fill_cache(scan_info_t *scan_info, uint current_timestamp);
static void wlc_scan_build_cache_list(void *arg1, void *arg2, uint timestamp,
                                      struct ether_addr *BSSID, wlc_ssid_t *SSID,
                                      int BSS_type, chanspec_t chanspec, void *data, uint datalen);
static void wlc_scan_cache_result(scan_info_t *scan_info);
#else
#define wlc_scan_fill_cache(si, ts)	do {} while (0)
#define wlc_scan_merge_cache(si, ts, BSSID, nssid, SSID, BSS_type, c_list, c_num, bss_list)
#define wlc_scan_cache_result(si)
#endif

#ifdef WLSCAN_PS
static int wlc_scan_ps_config_cores(scan_info_t *scan_info, bool flag);
#endif

static void wlc_scan_bss_deinit(void *ctx, wlc_bsscfg_t *cfg);
static bool wlc_scan_usage_scan(wlc_scan_info_t *scan);

static void wlc_scan_excursion_end(scan_info_t *scan_info);
#ifdef WLSCAN_SUMEVT
static void wlc_scan_summary_chan_stats(scan_info_t *scan_info,
	chanspec_t chanspec, bool ishome);
static void wlc_scan_send_evntlog_scan_summary(scan_info_t *scan_info,
	wl_scan_summary_t *scan_sum);
#endif /* WLSCAN_SUMEVT */

#define CHANNEL_PASSIVE_DWELLTIME(s) ((s)->passive_time)
#define CHANNEL_ACTIVE_DWELLTIME(s) ((s)->active_time)
#define SCAN_INVALID_DWELL_TIME		0
#define SCAN_PROBE_TXRATE(scan_info)	0

#define SCAN_PASSIVE_5G_CHANSPEC(s_info)	\
	((s_info->channel_idx < s_info->channel_num) &&	\
	CHSPEC_IS5G(s_info->chanspec_list[s_info->channel_idx]) && \
		wlc_quiet_chanspec(s_info->wlc->cmi, \
		s_info->chanspec_list[s_info->channel_idx]))

#ifdef WLRSDB
#define SCAN_CURRENT_5G_PASSIVE(s_info)		\
	(s_info->scan_cmn->flag & SCAN_CMN_FLAG_5G_5G_PASSIVE)

typedef int (*scan_sched_complete_cb)(wlc_scan_info_t *wlc_scan_info,
	void *cb, bool FIXED_SLOT);
#endif /* WLRSDB */

#ifdef WLC_SCAN_IOVARS
/* IOVar table */

/* Parameter IDs, for use only internally to wlc -- in the wlc_iovars
 * table and by the wlc_doiovar() function.  No ordering is imposed:
 * the table is keyed by name, and the function uses a switch.
 */
enum {
	IOV_PASSIVE = 1,
	IOV_SCAN_ASSOC_TIME,
	IOV_SCAN_UNASSOC_TIME,
	IOV_SCAN_PASSIVE_TIME,
	IOV_SCAN_HOME_TIME,
	IOV_SCAN_NPROBES,
	IOV_SCAN_EXTENDED,
	IOV_SCAN_NOPSACK,
	IOV_SCANCACHE,
	IOV_SCANCACHE_TIMEOUT,
	IOV_SCANCACHE_CLEAR,
	IOV_SCAN_FORCE_ACTIVE,	/* force passive to active conversion in radar/restricted channel */
	IOV_SCAN_ASSOC_TIME_DEFAULT,
	IOV_SCAN_UNASSOC_TIME_DEFAULT,
	IOV_SCAN_PASSIVE_TIME_DEFAULT,
	IOV_SCAN_HOME_TIME_DEFAULT,
	IOV_SCAN_DBG,
	IOV_SCAN_TEST,
	IOV_SCAN_HOME_AWAY_TIME,
	IOV_SCAN_RSDB_PARALLEL_SCAN,
	IOV_SCAN_RX_PWRSAVE,	/* reduce rx chains for pwr optimization */
	IOV_SCAN_TX_PWRSAVE,	/* reduce tx chains for pwr optimization */
	IOV_SCAN_PWRSAVE,	/* turn on/off bith tx and rx for single core scanning  */
	IOV_SCANMAC,
	IOV_SCAN_SUPPRESS,	/* scan suppress IOVAR */
	IOV_SCAN_IN_PROGRESS,
	IOV_LAST 		/* In case of a need to check max ID number */
};

/* AP IO Vars */
static const bcm_iovar_t wlc_scan_iovars[] = {
	{"passive", IOV_PASSIVE,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
#ifdef STA
	{"scan_assoc_time", IOV_SCAN_ASSOC_TIME,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
	{"scan_unassoc_time", IOV_SCAN_UNASSOC_TIME,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
#endif /* STA */
	{"scan_passive_time", IOV_SCAN_PASSIVE_TIME,
	(IOVF_WHL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
	/* unlike the other scan times, home_time can be zero */
	{"scan_home_time", IOV_SCAN_HOME_TIME,
	(IOVF_WHL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
#ifdef STA
	{"scan_nprobes", IOV_SCAN_NPROBES,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_INT8, 0
	},
	{"scan_force_active", IOV_SCAN_FORCE_ACTIVE,
	0, 0, IOVT_BOOL, 0
	},
#ifdef WLSCANCACHE
	{"scancache", IOV_SCANCACHE,
	(IOVF_OPEN_ALLOW), 0, IOVT_BOOL, 0
	},
	{"scancache_timeout", IOV_SCANCACHE_TIMEOUT,
	(IOVF_OPEN_ALLOW), 0, IOVT_INT32, 0
	},
	{"scancache_clear", IOV_SCANCACHE_CLEAR,
	(IOVF_OPEN_ALLOW), 0, IOVT_VOID, 0
	},
#endif /* WLSCANCACHE */
#endif /* STA */
#ifdef STA
	{"scan_assoc_time_default", IOV_SCAN_ASSOC_TIME_DEFAULT,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
	{"scan_unassoc_time_default", IOV_SCAN_UNASSOC_TIME_DEFAULT,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
#endif /* STA */
	{"scan_passive_time_default", IOV_SCAN_PASSIVE_TIME_DEFAULT,
	(IOVF_NTRL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
	/* unlike the other scan times, home_time can be zero */
	{"scan_home_time_default", IOV_SCAN_HOME_TIME_DEFAULT,
	(IOVF_WHL|IOVF_OPEN_ALLOW), 0, IOVT_UINT16, 0
	},
#ifdef BCMDBG
	{"scan_dbg", IOV_SCAN_DBG, 0, 0, IOVT_UINT8, 0},
	{"scan_test", IOV_SCAN_TEST, 0, 0, IOVT_UINT8, 0},
#endif
#ifdef STA
	{"scan_home_away_time", IOV_SCAN_HOME_AWAY_TIME, (IOVF_WHL), 0, IOVT_UINT16, 0},
#endif /* STA */
#ifdef WLRSDB
	{"scan_parallel", IOV_SCAN_RSDB_PARALLEL_SCAN, 0, 0, IOVT_BOOL, 0},
#endif
#ifdef WLSCAN_PS
#if defined(BCMDBG) || defined(WLTEST)
	/* debug iovar to enable power optimization in rx */
	{"scan_rx_ps", IOV_SCAN_RX_PWRSAVE, (IOVF_WHL), 0, IOVT_UINT8, 0},
	/* debug iovar to enable power optimization in tx */
	{"scan_tx_ps", IOV_SCAN_TX_PWRSAVE, (IOVF_WHL), 0, IOVT_UINT8, 0},
#endif /* defined(BCMDBG) || defined(WLTEST) */
	/* single core scanning to reduce power consumption */
	{"scan_ps", IOV_SCAN_PWRSAVE, (IOVF_WHL), 0, IOVT_UINT8, 0},
#endif /* WLSCAN_PS */
	/* configurable scan MAC */
	{"scanmac", IOV_SCANMAC, 0, 0, IOVT_BUFFER, OFFSETOF(wl_scanmac_t, data)},
	{"scansuppress", IOV_SCAN_SUPPRESS, (IOVF_RSDB_SET), 0, IOVT_BOOL, 0},
	{"scan_in_progress", IOV_SCAN_IN_PROGRESS, 0, 0, IOVT_BOOL, 0},
	{NULL, 0, 0, 0, 0, 0}
};
#endif /* WLC_SCAN_IOVARS */

/* debug timer used in scan module */
/* #define DEBUG_SCAN_TIMER */
#ifdef DEBUG_SCAN_TIMER
static void
wlc_scan_add_timer_dbg(scan_info_t *scan, uint to, bool prd, const char *fname, int line)
{
	WL_SCAN(("wl%d: %s(%d): wl_add_timer: timeout %u tsf %u\n",
		scan->unit, fname, line, to, SCAN_GET_TSF_TIMERLOW(scan)));
	wl_add_timer(scan->wlc->wl, scan->timer, to, prd);
}

static bool
wlc_scan_del_timer_dbg(scan_info_t *scan, const char *fname, int line)
{
	WL_SCAN(("wl%d: %s(%d): wl_del_timer: tsf %u\n",
		scan->unit, func, line, SCAN_GET_TSF_TIMERLOW(scan)));
	return wl_del_timer(scan->wlc->wl, scan->timer);
}
#define WLC_SCAN_ADD_TIMER(scan, to, prd) \
	      wlc_scan_add_timer_dbg(scan, to, prd, __FUNCTION__, __LINE__)
#define WLC_SCAN_DEL_TIMER(wlc, scan) \
	      wlc_scan_del_timer_dbg(scan, __FUNCTION__, __LINE__)
#define WLC_SCAN_ADD_TEST_TIMER(scan, to, prd) \
	wl_add_timer((scan)->wlc->wl, (scan)->test_timer, (to), (prd))
#else /* DEBUG_SCAN_TIMER */
#define WLC_SCAN_ADD_TIMER(scan, to, prd) wl_add_timer((scan)->wlc->wl, (scan)->timer, (to), (prd))
#define WLC_SCAN_DEL_TIMER(scan) wl_del_timer((scan)->wlc->wl, (scan)->timer)
#define WLC_SCAN_ADD_TEST_TIMER(scan, to, prd) \
	wl_add_timer((scan)->wlc->wl, (scan)->test_timer, (to), (prd))
#endif /* DEBUG_SCAN_TIMER */
#define WLC_SCAN_FREE_TIMER(scan)	wl_free_timer((scan)->wlc->wl, (scan)->timer)

#define WLC_SCAN_LENGTH_ERR	"%s: Length verification failed len: %d sm->len: %d\n"

#ifdef BCMDBG
#define SCAN_DBG_ENT	0x1
#define WL_SCAN_ENT(scan, x)	do {					\
		if (WL_SCAN_ON() && ((scan)->debug & SCAN_DBG_ENT))	\
			printf x;					\
	} while (0)
#else /* !BCMDBG */
#define WL_SCAN_ENT(scan, x)
#endif /* !BCMDBG */

#ifdef BCMDBG
/* some test cases */
#define SCAN_TEST_NONE	0
#define SCAN_TEST_ABORT_PSPEND	1	/* abort after sending PM1 indication */
#define SCAN_TEST_ABORT_PSPEND_AND_SCAN	2	/* abort after sending PM1 indication and scan */
#define SCAN_TEST_ABORT_WSUSPEND	3	/* abort after tx suspend */
#define SCAN_TEST_ABORT_WSUSPEND_AND_SCAN	4	/* abort after suspend and scan */
#define SCAN_TEST_ABORT_ENTRY	5	/* abort right away after the scan request */
#endif

/* guard time */
#define WLC_SCAN_PSPEND_GUARD_TIME	15
#define WLC_SCAN_WSUSPEND_GUARD_TIME	5

/* This includes the auto generated ROM IOCTL/IOVAR patch handler C source file (if auto patching is
 * enabled). It must be included after the prototypes and declarations above (since the generated
 * source file may reference private constants, types, variables, and functions).
 */
#include <wlc_patch.h>

wlc_scan_info_t*
BCMATTACHFN(wlc_scan_attach)(wlc_info_t *wlc, void *wl, osl_t *osh, uint unit)
{
	scan_info_t *scan_info;
	wlc_iov_disp_fn_t iovar_fn = NULL;
	const bcm_iovar_t *iovars = NULL;
	int	err = 0;

	uint	scan_info_size = (uint)sizeof(scan_info_t);
	uint	ssid_offs, chan_offs;

	ssid_offs = scan_info_size = ROUNDUP(scan_info_size, sizeof(uint32));
	scan_info_size += sizeof(wlc_ssid_t) * WLC_SCAN_NSSID_PREALLOC;
	chan_offs = scan_info_size = ROUNDUP(scan_info_size, sizeof(uint32));

	scan_info = (scan_info_t *)MALLOCZ(osh, scan_info_size);
	if (scan_info == NULL) {
		WL_ERROR((WLC_MALLOC_ERR, WLCWLUNIT(wlc), __FUNCTION__, (int)scan_info_size,
			MALLOCED(osh)));
		return NULL;
	}

	scan_info->scan_pub = (struct wlc_scan_info *)MALLOCZ(osh, sizeof(struct wlc_scan_info));
	if (scan_info->scan_pub == NULL) {
		WL_ERROR((WLC_MALLOC_ERR, WLCWLUNIT(wlc), __FUNCTION__,
			(int)sizeof(struct wlc_scan_info), MALLOCED(osh)));
		MFREE(osh, scan_info, scan_info_size);
		return NULL;
	}
	scan_info->scan_pub->scan_priv = (void *)scan_info;

	/* OBJECT REGISTRY: check if shared scan_cmn_info &
	 * wlc_scan_cmn_info  has value already stored
	 */
	scan_info->scan_cmn = (scan_cmn_info_t*)
		obj_registry_get(wlc->objr, OBJR_SCANPRIV_CMN);

	if (scan_info->scan_cmn == NULL) {
		if ((scan_info->scan_cmn =  (scan_cmn_info_t*) MALLOCZ(osh,
			sizeof(scan_cmn_info_t))) == NULL) {
			WL_ERROR((WLC_MALLOC_ERR, WLCWLUNIT(wlc), __FUNCTION__,
				(int)sizeof(scan_cmn_info_t), MALLOCED(osh)));
			goto error;
		}
		/* OBJECT REGISTRY: We are the first instance, store value for key */
		obj_registry_set(wlc->objr, OBJR_SCANPRIV_CMN, scan_info->scan_cmn);
	}
	BCM_REFERENCE(obj_registry_ref(wlc->objr, OBJR_SCANPRIV_CMN));

	scan_info->scan_pub->wlc_scan_cmn = (struct wlc_scan_cmn_info*)
		obj_registry_get(wlc->objr, OBJR_SCANPUBLIC_CMN);

	if (scan_info->scan_pub->wlc_scan_cmn == NULL) {
		if ((scan_info->scan_pub->wlc_scan_cmn =  (struct wlc_scan_cmn_info*)
			MALLOCZ(osh, sizeof(struct wlc_scan_cmn_info))) == NULL) {
			WL_ERROR(("wl%d: %s: wlc_scan_cmn_info alloc falied\n",
				unit, __FUNCTION__));
			goto error;
		}
		/* OBJECT REGISTRY: We are the first instance, store value for key */
		obj_registry_set(wlc->objr, OBJR_SCANPUBLIC_CMN,
		scan_info->scan_pub->wlc_scan_cmn);
	}
	BCM_REFERENCE(obj_registry_ref(wlc->objr, OBJR_SCANPUBLIC_CMN));

#if defined(WLSCANCACHE) && !defined(WLSCANCACHE_DISABLED)
	scan_info->sdb = (wlc_scandb_t*)
		obj_registry_get(wlc->objr, OBJR_SCANDB_CMN);

	if (scan_info->sdb == NULL) {
		scan_info->sdb = wlc_scandb_create(osh, unit,
			wlc->pub->tunables->max_scancache_results);
		if (scan_info->sdb == NULL) {
			WL_ERROR(("wl%d: %s: wlc_create_scandb failed\n",
				unit, __FUNCTION__));
			goto error;
		}
	}
	/* OBJECT REGISTRY: We are the first instance, store value for key */
	obj_registry_set(wlc->objr, OBJR_SCANDB_CMN, scan_info->sdb);

	wlc->pub->cmn->_scancache_support = TRUE;
	scan_info->scan_pub->wlc_scan_cmn->_scancache = FALSE; /* disabled by default */
#endif /* WLSCANCACHE && !WLSCANCACHE_DISABLED */

	scan_info->scan_cmn->memsize = scan_info_size;
	scan_info->wlc = wlc;
	scan_info->osh = osh;
	scan_info->unit = unit;
	scan_info->channel_idx = -1;
	scan_info->scan_pub->in_progress = FALSE;

	scan_info->scan_cmn->defaults.assoc_time = WLC_SCAN_ASSOC_TIME;
	scan_info->scan_cmn->defaults.unassoc_time = WLC_SCAN_UNASSOC_TIME;
	scan_info->scan_cmn->defaults.home_time = WLC_SCAN_HOME_TIME;
	scan_info->scan_cmn->defaults.passive_time = WLC_SCAN_PASSIVE_TIME;
	scan_info->scan_cmn->defaults.nprobes = WLC_SCAN_NPROBES;
	scan_info->scan_cmn->defaults.passive = FALSE;
	scan_info->scan_cmn->home_away_time = WLC_SCAN_AWAY_LIMIT;

	scan_info->timer = wl_init_timer((struct wl_info *)wl,
	                                 wlc_scantimer, scan_info, "scantimer");
	if (scan_info->timer == NULL) {
		WL_ERROR(("wl%d: %s: wl_init_timer for scan timer failed\n", unit, __FUNCTION__));
		goto error;
	}

	SCAN_SET_WATCHDOG_FN(wlc_scan_watchdog);

#ifdef WLC_SCAN_IOVARS
	iovar_fn = wlc_scan_doiovar;
	iovars = wlc_scan_iovars;
#endif /* WLC_SCAN_IOVARS */

	scan_info->ssid_prealloc = (wlc_ssid_t*)((uintptr)scan_info + ssid_offs);
	scan_info->nssid_prealloc = WLC_SCAN_NSSID_PREALLOC;
	scan_info->ssid_list = scan_info->ssid_prealloc;

	BCM_REFERENCE(chan_offs);

	if (PWRSTATS_ENAB(wlc->pub)) {
		scan_info->scan_stats =
			(wl_pwr_scan_stats_t*)MALLOC(osh, sizeof(wl_pwr_scan_stats_t));
		if (scan_info->scan_stats == NULL) {
			WL_ERROR(("wl%d: %s: failure allocating power stats\n",
				unit, __FUNCTION__));
			goto error;
		}
		bzero((char*)scan_info->scan_stats, sizeof(wl_pwr_scan_stats_t));
	}
	err = wlc_module_register(wlc->pub, iovars, "scan", scan_info, iovar_fn,
		wlc_scan_watchdog, NULL, wlc_scan_down);
	if (err) {
		WL_ERROR(("wl%d: %s: wlc_module_register err=%d\n",
		          unit, __FUNCTION__, err));
		goto error;
	}

	err = wlc_module_add_ioctl_fn(wlc->pub, (void *)scan_info->scan_pub,
	                              wlc_scan_ioctl, 0, NULL);
	if (err) {
		WL_ERROR(("wl%d: %s: wlc_module_add_ioctl_fn err=%d\n",
		          unit, __FUNCTION__, err));
		goto error;
	}

	/* register bsscfg deinit callback through cubby registration */
	if ((err = wlc_bsscfg_cubby_reserve(wlc, 0, NULL, wlc_scan_bss_deinit, NULL,
			scan_info)) < 0) {
		WL_ERROR(("wl%d: %s: wlc_bsscfg_cubby_reserve err=%d\n",
		          unit, __FUNCTION__, err));
		goto error;
	}

#if defined(BCMDBG) || defined(BCMDBG_DUMP)
	wlc_dump_register(wlc->pub, "scan", (dump_fn_t)wlc_scan_dump, (void *)scan_info);
#ifdef WLSCANCACHE
	if (SCANCACHE_SUPPORT(scan_info->wlc))
		wlc_dump_register(wlc->pub, "scancache", wlc_scandb_dump, scan_info->sdb);
#endif /* WLSCANCACHE */
#endif /* BCMDBG || BCMDBG_DUMP */

#ifdef BCMDBG
	/* scan_info->debug = SCAN_DBG_ENT; */
#endif
	scan_info->pspend_guard_time = WLC_SCAN_PSPEND_GUARD_TIME;
	scan_info->wsuspend_guard_time = WLC_SCAN_WSUSPEND_GUARD_TIME;

#ifdef WLSCAN_PS
	scan_info->scan_ps_txchain = 0;
	scan_info->scan_ps_rxchain = 0;
	/* disable scan power optimization by default */
#if defined(BCMDBG) || defined(WLTEST)
	scan_info->scan_rx_pwrsave = FALSE;
	scan_info->scan_tx_pwrsave = FALSE;
#endif /* defined(BCMDBG) || defined(WLTEST) */
	scan_info->scan_cmn->scan_pwrsave_enable = FALSE;
	scan_info->scan_cmn->scan_mimo_override = FALSE;
#endif /* WLSCAN_PS */

#if defined(WLSCAN_PS) && defined(WL_STF_ARBITRATOR)
	if (WLSCAN_PS_ENAB(wlc->pub) && WLC_STF_ARB_ENAB(wlc->pub)) {
		/* Register with STF Aribitrator module for configuring chains */
		scan_info->stf_scan_req = wlc_stf_nss_request_register(wlc,
			WLC_STF_ARBITRATOR_REQ_ID_SCAN,
			WLC_STF_ARBITRATOR_REQ_PRIO_SCAN,
			NULL, NULL);
		if (!scan_info->stf_scan_req)
			goto error;
	}
#endif /* WLSCAN_PS && WL_STF_ARBITRATOR */

#if defined(WLRSDB) && !defined(RSDB_PARALLEL_SCAN_DISABLED)
	if (RSDB_ENAB(wlc->pub)) {
		scan_info->scan_cmn->rsdb_parallel_scan = TRUE;
	}
#endif /* WLRSDB && !RSDB_PARALLEL_SCAN_DISABLED */
#if defined(WLSCAN_SUMEVT) && !defined(WLSCAN_SUMEVT_DISABLED)
	wlc->pub->cmn->_scan_sumevt = TRUE;
#endif /* (WLSCAN_SUMEVT) && !defined(WLSCAN_SUMEVT_DISABLED) */
#ifdef WLSCAN_SUMEVT
	if (SCAN_SUMEVT_ENAB(wlc->pub)) {
		if ((scan_info->scan_sum_chan_info =
			MALLOCZ(osh, (sizeof(wl_scan_summary_t)))) == NULL) {
			WL_ERROR(("wl%d: %s: failure allocating scan_sum_chan_info\n",
				unit, __FUNCTION__));
			goto error;
		}

		if (scan_info->scan_cmn->scn_summary == NULL) {
			if ((scan_info->scan_cmn->scn_summary =
				MALLOCZ(osh, (sizeof(wl_scan_summary_t)))) == NULL) {
				WL_ERROR(("wl%d: %s: failure allocating scn_summary\n",
				unit, __FUNCTION__));
				goto error;
			}
		}
	}
#endif /* WLSCAN_SUMEVT */

	return scan_info->scan_pub;

error:
	if (scan_info) {
		if (scan_info->timer != NULL)
			WLC_SCAN_FREE_TIMER(scan_info);
		if (scan_info->sdb) {
			wlc_scandb_free(scan_info->sdb);
			scan_info->sdb = NULL;
		}
#ifdef WLSCAN_SUMEVT
		if (SCAN_SUMEVT_ENAB(wlc->pub)) {
			if (scan_info->scan_sum_chan_info != NULL)
				MFREE(scan_info->osh, scan_info->scan_sum_chan_info,
					sizeof(wl_scan_summary_t));
			if (scan_info->scan_cmn->scn_summary != NULL) {
				MFREE(scan_info->osh, scan_info->scan_cmn->scn_summary,
					sizeof(wl_scan_summary_t));
				scan_info->scan_cmn->scn_summary = NULL;
			}
		}
#endif
		if (scan_info->scan_pub)
			MFREE(osh, scan_info->scan_pub, sizeof(struct wlc_scan_info));
		if (PWRSTATS_ENAB(wlc->pub) && scan_info->scan_stats) {
			MFREE(osh, scan_info->scan_stats, sizeof(wl_pwr_scan_stats_t));
		}
		MFREE(osh, scan_info, scan_info_size);
	}

	return NULL;
}

static void
wlc_scan_ssidlist_free(scan_info_t *scan_info)
{
	if (scan_info->ssid_list != scan_info->ssid_prealloc) {
		MFREE(scan_info->osh, scan_info->ssid_list,
		      scan_info->nssid * sizeof(wlc_ssid_t));
		scan_info->ssid_list = scan_info->ssid_prealloc;
		scan_info->nssid = scan_info->nssid_prealloc;
	}
}

int
BCMUNINITFN(wlc_scan_down)(void *hdl)
{
#ifndef BCMNODOWN
	scan_info_t *scan_info = (scan_info_t *)hdl;
	int callbacks = 0;

	if (!WLC_SCAN_DEL_TIMER(scan_info))
		callbacks ++;

#ifdef WLRSDB
	if (RSDB_ENAB((SCAN_WLC(scan_info))->pub)) {
		/*
		* Avoid this call to disable scan if 2G WLC is being brought up/down on purpose
		*/
		if (scan_info->scan_pub->flag & SCAN_FLAG_PASSIVE_PSCAN_BLOCK) {
			return callbacks;
		}
	}
#endif /* WLRSDB */

	scan_info->state = WLC_SCAN_STATE_START;
	scan_info->channel_idx = -1;
	scan_info->scan_pub->in_progress = FALSE;
	wlc_phy_hold_upd(SCAN_GET_PI_PTR(scan_info), PHY_HOLD_FOR_SCAN, FALSE);

	wlc_scan_ssidlist_free(scan_info);

	return callbacks;
#else
	return 0;
#endif /* BCMNODOWN */
}

void
BCMATTACHFN(wlc_scan_detach)(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info;
	if (!wlc_scan_info)
		return;

	scan_info = (scan_info_t *)wlc_scan_info->scan_priv;

	if (scan_info) {
		int memsize = scan_info->scan_cmn->memsize;
		wlc_info_t * wlc = scan_info->wlc;

		if (scan_info->timer) {
			WLC_SCAN_FREE_TIMER(scan_info);
			scan_info->timer = NULL;
		}
#ifdef WLSCAN_SUMEVT
		if (SCAN_SUMEVT_ENAB(wlc->pub)) {
			MFREE(scan_info->osh, scan_info->scan_sum_chan_info,
				sizeof(wl_scan_summary_t));
			if (scan_info->scan_cmn->scn_summary != NULL) {
				MFREE(scan_info->osh, scan_info->scan_cmn->scn_summary,
					sizeof(wl_scan_summary_t));
				scan_info->scan_cmn->scn_summary = NULL;
			}
		}
#endif

		wlc_module_unregister(scan_info->wlc->pub, "scan", scan_info);

		(void)wlc_module_remove_ioctl_fn(scan_info->wlc->pub, (void *)scan_info->scan_pub);

		ASSERT(scan_info->ssid_list == scan_info->ssid_prealloc);
		if (scan_info->ssid_list != scan_info->ssid_prealloc) {
			WL_ERROR(("wl%d: %s: ssid_list not set to prealloc\n",
				scan_info->unit, __FUNCTION__));
		}
		if (obj_registry_unref(wlc->objr, OBJR_SCANPRIV_CMN) == 0) {
			obj_registry_set(wlc->objr, OBJR_SCANPRIV_CMN, NULL);
			MFREE(wlc->osh, scan_info->scan_cmn, sizeof(scan_cmn_info_t));
		}
#ifdef WLSCANCACHE
		if (SCANCACHE_SUPPORT(scan_info->wlc) &&
			(obj_registry_unref(wlc->objr, OBJR_SCANDB_CMN) == 0)) {
			obj_registry_set(wlc->objr, OBJR_SCANDB_CMN, NULL);
			wlc_scandb_free(scan_info->sdb);
		}
#endif /* WLSCANCACHE */
		if (obj_registry_unref(wlc->objr, OBJR_SCANPUBLIC_CMN) == 0) {
			obj_registry_set(wlc->objr, OBJR_SCANPUBLIC_CMN, NULL);
			MFREE(wlc->osh, wlc_scan_info->wlc_scan_cmn, sizeof(wlc_scan_cmn_t));
		}

#if defined(WLSCAN_PS) && defined(WL_STF_ARBITRATOR)
	if (WLSCAN_PS_ENAB(wlc->pub) && WLC_STF_ARB_ENAB(wlc->pub)) {
			if (scan_info->stf_scan_req &&
				wlc_stf_nss_request_unregister(scan_info->wlc,
				scan_info->stf_scan_req))
			WL_STF_ARBITRATOR_ERROR(("ARBI: Error Unregister bsscfg's scan req\n"));
			scan_info->stf_scan_req = NULL;
		}
#endif /* WLSCAN_PS && WL_STF_ARBITRATOR */

		MFREE(scan_info->osh, wlc_scan_info, sizeof(struct wlc_scan_info));
		MFREE(scan_info->osh, scan_info, memsize);
	}
}

#if defined(BCMDBG) || defined(WLMSG_INFORM)
static void
wlc_scan_print_ssids(wlc_ssid_t *ssid, int nssid)
{
	char ssidbuf[SSID_FMT_BUF_LEN];
	int linelen = 0;
	int len;
	int i;

	for (i = 0; i < nssid; i++) {
		len = wlc_format_ssid(ssidbuf, ssid[i].SSID, ssid[i].SSID_len);
		/* keep the line length under 80 cols */
		if (linelen + (len + 2) > 80) {
			printf("\n");
			linelen = 0;
		}
		printf("\"%s\" ", ssidbuf);
		linelen += len + 3;
	}
	printf("\n");
}
#endif /* BCMDBG || WLMSG_INFORM */

bool
wlc_scan_in_scan_chanspec_list(wlc_scan_info_t *wlc_scan_info, chanspec_t chanspec)
{
	scan_info_t *scan_info = (scan_info_t *) wlc_scan_info->scan_priv;
	int i;
	uint8 chan;

	/* scan chanspec list not setup, return no match */
	if (scan_info->channel_idx == -1) {
		WL_INFORM_SCAN(("%s: Scan chanspec list NOT setup, NO match\n", __FUNCTION__));
		return FALSE;
	}

	/* if strict channel match report is not needed, return match */
	if (wlc_scan_info->state & SCAN_STATE_OFFCHAN)
		return TRUE;

	chan = wf_chspec_ctlchan(chanspec);
	for (i = 0; i < scan_info->channel_num; i++) {
		if (wf_chspec_ctlchan(scan_info->chanspec_list[i]) == chan) {
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef WLSCAN_SUMEVT
static void
wlc_scan_summary_chan_stats(scan_info_t *scan_info, chanspec_t chanspec, bool ishome)
{
	wl_scan_summary_t *scan_chan_info = scan_info->scan_sum_chan_info;
	ASSERT(scan_chan_info != NULL);
	if (ishome) {
		scan_chan_info->scan_flags |= HOME_CHAN;
	} else {
		scan_info->scan_cmn->chan_offset++;
		if (!wlc_scan_quiet_chanspec(scan_info, chanspec) ||
			(scan_info->scan_pub->state & SCAN_STATE_RADAR_CLEAR)) {
			scan_chan_info->scan_flags |= ACTIVE_SCAN_SCN_SUM;
		}
	}
	scan_chan_info->scan_flags |= SCAN_SUM_CHAN_INFO;
	scan_chan_info->u.scan_chan_info.start_time = OSL_SYSUPTIME();
	scan_chan_info->u.scan_chan_info.chanspec = chanspec;
	if (scan_info->unit == MAC_CORE_UNIT_0) {
		scan_chan_info->scan_flags |= SCAN_SUM_WLC_CORE0;
	} else if (scan_info->unit == MAC_CORE_UNIT_1) {
		scan_chan_info->scan_flags  |= SCAN_SUM_WLC_CORE1;
	}
}

static void
wlc_scan_send_evntlog_scan_summary(scan_info_t *scan_info, wl_scan_summary_t *scan_sum)
{
	scan_sum->len = sizeof(*scan_sum) - MAX(sizeof(wl_scan_channel_info_t),
		sizeof(wl_scan_summary_info_t));
	if (!(scan_sum->scan_flags & SCAN_SUM_CHAN_INFO)) {
		/* Update the actual scanned channel num including
		* passive channels that were converted to active scans
		*/
		scan_sum->u.scan_sum_info.total_chan_num =
			scan_info->scan_cmn->chan_offset;
		scan_info->scan_cmn->chan_offset = 0;
		scan_sum->len += OFFSETOF(wl_scan_summary_info_t, ssid);
		scan_sum->u.scan_sum_info.scan_end_time = OSL_SYSUPTIME();
	} else {
		scan_sum->len += OFFSETOF(wl_scan_channel_info_t, probe_count);
		scan_sum->u.scan_chan_info.end_time = OSL_SYSUPTIME();
	}
	scan_sum->version = SCAN_SUMMARY_VERSION;
	scan_sum->sync_id = scan_info->scan_cmn->scan_sync_id;
#if defined(EVENT_LOG_COMPILE)
	if (EVENT_LOG_IS_ON(EVENT_LOG_TAG_SCAN_SUMMARY)) {
		EVENT_LOG_BUFFER(EVENT_LOG_TAG_SCAN_SUMMARY,
			(void*)(scan_sum),
			sizeof(wl_scan_summary_t));
	}
#endif /* EVENT_LOG_COMPILE */
	memset(scan_sum, 0, sizeof(wl_scan_summary_t));
}
#endif /* WLSCAN_SUMEVT */


#ifdef WLRSDB
static void
wlc_scan_handle_req_end_for_pscan(wlc_info_t *wlc)
{
	wlc_info_t *oth_wlc = wlc_rsdb_get_other_wlc(wlc);
	wlc_scan_info_t *wlc_scan_info = wlc->scan;
	wlc_scan_info_t *oth_wlc_scan_info = oth_wlc->scan;
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	scan_info_t *oth_scan_info = (scan_info_t *)oth_wlc_scan_info->scan_priv;

	/*
	* while in passive pscan phase, wait for both wlcs to complete
	* their previously requested schedules before scheduling the next request.
	*/
	if (!(oth_wlc_scan_info->flag &
		SCAN_FLAG_PASSIVE_PSCAN_REQ_ENDED)) {
		wlc_scan_info->flag |=
			SCAN_FLAG_PASSIVE_PSCAN_REQ_ENDED;
	}
	else {
		/*
		* other wlc finished processing REQ_END. Bump ahead channel scan
		* sequence and proceed with setting scantimer.
		*/
		wlc_scan_info->flag &= ~SCAN_FLAG_PASSIVE_PSCAN_REQ_ENDED;
		oth_wlc_scan_info->flag &= ~SCAN_FLAG_PASSIVE_PSCAN_REQ_ENDED;
		scan_info->channel_idx++;
		oth_scan_info->channel_idx++;

		/*
		* compare channel_idx with channel_num as we may have
		* completed scanning all channels and decide next state
		*/

		if (scan_info->channel_idx < scan_info->channel_num) {
			scan_info->state = WLC_SCAN_STATE_PARTIAL;
		} else {
			/* all channels for this wlc scanned */
			scan_info->state = WLC_SCAN_STATE_COMPLETE;
		}

		if (oth_scan_info->channel_idx < oth_scan_info->channel_num) {
			oth_scan_info->state = WLC_SCAN_STATE_PARTIAL;
		} else {
			/* all channels for this wlc scanned */
			oth_scan_info->state = WLC_SCAN_STATE_COMPLETE;
		}
		/* run scantimer to conclude next action */
		WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
		WLC_SCAN_ADD_TIMER(oth_scan_info, 0, 0);

		/*
		* if any one of the wlcs has finished scanning passive
		* channels OR Any one wlc has finished scanning all
		* channels, passive parallel scan cannot happen anymore.
		* clear passive p-scan flag.
		*/
		if (!(SCAN_PASSIVE_5G_CHANSPEC(scan_info)) ||
			!(SCAN_PASSIVE_5G_CHANSPEC(oth_scan_info))) {
			scan_info->scan_cmn->flag &=
				~(SCAN_CMN_FLAG_5G_5G_PASSIVE);
		}
	}
}

static bool
wlc_scan_check_disable_pscan_wlc(scan_info_t *scan_info)
{
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	bool ret = FALSE;

	if (RSDB_ENAB(wlc->pub) && SCAN_CURRENT_5G_PASSIVE(scan_info)) {
		wlc_info_t *wlc_2g = NULL;
		wlc_info_t *wlc_5g = NULL;

		/*
		* if this is unassociated 2GWLC and 5GWLC is associated
		* and currently 2GWLC is scanning passive channel parallely
		* with 5GWLC, then return TRUE to bring down this WLC when
		* it's idle (5G WLC one home channel)
		*/
		wlc_rsdb_get_wlcs(scan_info->wlc, &wlc_2g, &wlc_5g);

		if ((wlc == wlc_2g) && wlc_5g->pub->associated &&
			!wlc->pub->associated) {
			ret = TRUE;
		}
	}
	return ret;
}

static void
wlc_scan_disable_idle_pscan_wlc(scan_info_t *scan_info)
{
	if (!wlc_scan_check_disable_pscan_wlc(scan_info)) {
		/* disabling idle unassociuated wlc not supported */
		return;
	}
	else {
		wlc_info_t *wlc = SCAN_WLC(scan_info);

		if (scan_info->scan_pub->flag & SCAN_FLAG_PASSIVE_PSCAN_BLOCK) {
			wlc->mpc_scan = FALSE;
			scan_info->scan_pub->in_progress = FALSE;
			wlc->mpc_delay_off = 0;

			wlc_scan_radio_mpc_upd(scan_info);
			wlc_scan_radio_upd(scan_info);

			scan_info->scan_pub->flag &= ~SCAN_FLAG_PASSIVE_PSCAN_BLOCK;
			wlc->mpc_scan = TRUE;
			scan_info->scan_pub->in_progress = TRUE;
			scan_info->scan_pub->flag |= SCAN_FLAG_PASSIVE_PSCAN_WAKE_PENDING;
		}
	}
}

static void
wlc_scan_enable_idle_pscan_wlc(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;

	wlc_scan_info->flag |= SCAN_FLAG_PASSIVE_PSCAN_BLOCK;
	wlc_scan_radio_mpc_upd(scan_info);
	wlc_scan_info->flag &= ~SCAN_FLAG_PASSIVE_PSCAN_BLOCK;
}

static int
BCMRAMFN(wlc_scan_is_parallel_enab)(scan_info_t *scan)
{
	return (RSDB_ENAB((scan)->wlc->pub) && scan->scan_cmn->rsdb_parallel_scan);
}

/* return true if any of the scan is in progress. */
int
wlc_scan_anyscan_in_progress(wlc_scan_info_t *scan)
{
	int idx;
	scan_info_t *scan_info = (scan_info_t *)scan->scan_priv;
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	wlc_info_t *wlc_iter;

	FOREACH_WLC(wlc->cmn, idx, wlc_iter) {
		if (wlc_iter->scan && wlc_iter->scan->in_progress)
			return TRUE;
#if defined(WLDFS) && (defined(RSDB_DFS_SCAN) || defined(BGDFS))
		if ((WLRSDBDFS_ENAB(wlc->pub) || BGDFS_ENAB(wlc->pub)) &&
			wlc_dfs_scan_in_progress(wlc->dfs)) {
			return TRUE;
		}
#endif /* WLDFS && (RSDB_DFS_SCAN || BGDFS) */
	}
	return FALSE;
}

/* Return TRUE when duplicate channel found in list
*/
static bool
wlc_scan_duplicate_channels_found(const chanspec_t* chanspec_list, int chan_num)
{
	bool ret = FALSE;
	uint8 channel;
	uint8 channel_num_index[MAXCHANNEL];
	int32 i;

	memset(channel_num_index, 0, MAXCHANNEL);

	for (i = 0; i < chan_num; i++) {
		channel = CHSPEC_CHANNEL(chanspec_list[i]);
		channel_num_index[channel-1]++;
		if (channel_num_index[channel-1] > 1) {
			ret = TRUE;
			break;
		}
	}
	return ret;
}

/* returns TRUE if scan is active scan looking at the scan_type value
 * and default value.
 */
static int
wlc_scan_is_active_scan(scan_info_t *scan_info, int scan_type)
{
	if (scan_type != DOT11_SCANTYPE_PASSIVE || scan_type != DOT11_SCANTYPE_ACTIVE) {
		/* If scan_type comes as -1 or any invalid value use from defaults. */
		scan_type = scan_info->scan_cmn->defaults.passive ?
			DOT11_SCANTYPE_PASSIVE : DOT11_SCANTYPE_ACTIVE;
	}
	return (scan_type == DOT11_SCANTYPE_ACTIVE);
}

static void
wlc_scan_get_parallel_passive_setting(scan_info_t *scan_info,
	bool *parallel_passive, bool *passive_channels_first)
{
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	UNUSED_PARAMETER(wlc);

	/*
	* setting parallel passive 5G false, this function will be excluded from ROM
	* and final setting will be done based on board settings.
	*/
	*parallel_passive = FALSE;
	*passive_channels_first = FALSE;
}

static int
wlc_scan_split_channels_per_band(scan_info_t *scan_info, const chanspec_t* chanspec_list,
	int chan_num, chanspec_t** chanspec_set0, chanspec_t** chanspec_set1,
	int *channel_set0_num, int *channel_set1_num, int active_time,
	int passive_time, int scan_type)
{
	int i, j, k;
	int passive_2g = 0, passive_5g = 0;
	scan_cmn_info_t *scan_cmn = scan_info->scan_cmn;
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	int num_passive_chan = 0;
	bool parallel_passive = TRUE;
	bool passive_channels_first = TRUE;
	int num_passive_chan_5g = 0;
	int num_passive_chan_2g = 0;
	bool is_rsdb_mode = WLC_RSDB_DUAL_MAC_MODE(WLC_RSDB_CURR_MODE(wlc));

	wlc_info_t *other_wlc = wlc_rsdb_get_other_wlc(wlc);
	scan_cmn->chanspec_list_size = (sizeof(chanspec_t) * chan_num);

	wlc_scan_get_parallel_passive_setting(scan_info, &parallel_passive,
		&passive_channels_first);

	if (WLC_SCAN_SIB_ISSET(wlc, BAND_5G_INDEX) &&
		!AWDL_ENAB(SCAN_WLC(scan_info)->pub)) {
		parallel_passive = TRUE;
		if (wlc->pub->associated && other_wlc->pub->associated) {
			/* When set to TRUE, all the passive channels will be scanned first
			 * if 5G-5G parallel passive scan is set.
			 */
			passive_channels_first = TRUE;
		}
	}
	/*
	* If we are in RSDB mode but only 2G wlc is associated, disable 5G-5G parallel
	* passive scan
	*/
	if (is_rsdb_mode) {
		wlc_info_t *wlc_2g = NULL;
		wlc_info_t *wlc_5g = NULL;
		wlc_rsdb_get_wlcs(wlc, &wlc_2g, &wlc_5g);
		/* if 5G wlc is unassociated, but 2G wlc is associated,
		* skip parallel scan
		*/
		if (!wlc_5g->pub->associated &&
			wlc_2g->pub->associated) {
			parallel_passive = FALSE;
			passive_channels_first = FALSE;
		}
	}

	ASSERT(scan_cmn->chanspeclist == NULL);
	/* Allocate new chanspec list for both 2G and 5G channels. */
	scan_cmn->chanspeclist = MALLOCZ(wlc->osh, scan_cmn->chanspec_list_size);

	if (scan_cmn->chanspeclist == NULL) {
		WL_ERROR(("wl%d Unable to allocate chanspec list for parallel scan\n",
			SCAN_UNIT(scan_info)));
		return BCME_NOMEM;
	}

	*chanspec_set1 = *chanspec_set0 = NULL;
	*channel_set0_num = *channel_set1_num = 0;

	/* If same channel is repeated, disable 5g-5g passive parallel scan. */
	if (wlc_scan_duplicate_channels_found(chanspec_list, chan_num)) {
		parallel_passive = FALSE;
	}

	if (wlc_scan_is_active_scan(scan_info, scan_type)) {
		scan_type = DOT11_SCANTYPE_ACTIVE;
	} else {
		scan_type = DOT11_SCANTYPE_PASSIVE;
	}

	if (passive_time < 0)
		passive_time = scan_info->scan_cmn->defaults.passive_time;

	if (active_time < 0) {
		if (wlc_rsdb_any_wlc_associated(scan_info->wlc)) {
			active_time = scan_info->scan_cmn->defaults.assoc_time;
		} else {
			active_time = scan_info->scan_cmn->defaults.unassoc_time;
		}
	}
	if (scan_type  == DOT11_SCANTYPE_PASSIVE) {
		/* All channels are passive for passive scan */
		if (chan_num > 1) {
			*channel_set0_num = chan_num/2;
			*channel_set1_num = (chan_num + 1) / 2;
		} else {
			/* Only one channel to deal with. */
			if (CHSPEC_IS2G(chanspec_list[0])) {
				*channel_set0_num = chan_num;
			} else {
				*channel_set1_num = chan_num;
			}
		}

	} else {
		/* Findout number of 2G & 5G non-passive channels in chanspec_list  */
		for (i = 0; i < chan_num; i++) {
			if (!wf_chspec_malformed(chanspec_list[i]) &&
				(wlc_scan_valid_chanspec_db(scan_info, chanspec_list[i]) ||
				(scan_info->scan_pub->state & SCAN_STATE_PROHIBIT))) {
				if (wlc_scan_quiet_chanspec(scan_info, chanspec_list[i])) {
					if (CHSPEC_IS2G(chanspec_list[i]))
						num_passive_chan_2g++;
					else
						num_passive_chan_5g++;
				} else {
					if (CHSPEC_IS2G(chanspec_list[i]))
						(*channel_set0_num)++;
					else
						(*channel_set1_num)++;
				}
			}
		}

		num_passive_chan = num_passive_chan_2g + num_passive_chan_5g;
		ASSERT((*channel_set0_num + *channel_set1_num + num_passive_chan) <= chan_num);

		if (passive_channels_first == FALSE && *channel_set0_num < *channel_set1_num) {
			parallel_passive = FALSE;
			WL_INFORM(("Disable Parallel passive Scan. Number of active channels in 2G"
				" is less than Number of active channels in 5G.\n"));
		}

		/* parallel passive is possible only when more than 1 passive channel exist. */
		if (parallel_passive && num_passive_chan > 1) {
			int passive_channels_5g = num_passive_chan_5g;
			int extra_passive_5g = 0;
			int active_time_5g = *channel_set1_num * active_time;
			int total_time_2g = *channel_set0_num * active_time +
				num_passive_chan_2g * passive_time;

			/* if 5G active scan time is much less than total 2G scan time, we can scan
			 * more number of passive channels.
			 */
			if (active_time_5g < total_time_2g) {
				extra_passive_5g = (total_time_2g - active_time_5g)/passive_time;
				if (extra_passive_5g > num_passive_chan_5g)
					extra_passive_5g = num_passive_chan_5g;

			}

			/* Add the 2G passive channels to 2G chanspec_list */
			*channel_set0_num += num_passive_chan_2g;
			passive_2g = num_passive_chan_2g;


			passive_channels_5g -= extra_passive_5g;
			*channel_set0_num += passive_channels_5g/2;
			*channel_set1_num += (extra_passive_5g + (passive_channels_5g + 1) / 2);

			/* Update total passive channels */
			passive_2g += passive_channels_5g/2;
			passive_5g = extra_passive_5g + (passive_channels_5g + 1) / 2;

			if (passive_channels_5g > 1) {
				/* When number passive channels to scan are greater than 2 we
				* actually do a 5G-5G passive scanning provided 2G chanset is less
				* sa compared to 5G. Hence we have to disable
				* conversion of passive to active scan on a RADAR channel even
				* when we find an AP beaconing to avoid 5G-5G TX/RX.
				*/
				scan_info->scan_cmn->flag |= SCAN_CMN_FLAG_NO_RADAR_CLEAR;
				scan_info->scan_cmn->flag |= SCAN_CMN_FLAG_5G_5G_PASSIVE;
			} else {
				/* 5G-5G passive is not done due to chanset timings. */
				parallel_passive = FALSE;
			}
		} else {
			*channel_set1_num += num_passive_chan_5g;
			*channel_set0_num += num_passive_chan_2g;
			passive_2g = num_passive_chan_2g;
			passive_5g = num_passive_chan_5g;
		}
	}

	if (*channel_set0_num)
		*chanspec_set0 = scan_cmn->chanspeclist;

	if (*channel_set1_num)
		*chanspec_set1 = scan_cmn->chanspeclist + *channel_set0_num;

	if (scan_type  == DOT11_SCANTYPE_PASSIVE) {
		/* By now, *channel_set0_num + *channel_set1_num + num_passive_chan = chan_num
		 * and just split the channels between the two list without caring
		 * quiet_chanspec. Try to keep 2G channels and 5G channels together
		 * to avoid large channel switch time.
		 */
		for (i = 0, j = 0, k = 0; i < chan_num; i++) {
			if (!wf_chspec_malformed(chanspec_list[i]) &&
				(wlc_scan_valid_chanspec_db(scan_info, chanspec_list[i]) ||
				(scan_info->scan_pub->state & SCAN_STATE_PROHIBIT))) {
				if (CHSPEC_IS2G(chanspec_list[i]))
					(*chanspec_set0)[j++] = chanspec_list[i];
				else
					(*chanspec_set1)[k++] = chanspec_list[i];

				/* If we reach limit for one of the lists, break */
				if (parallel_passive &&
					(j >= *channel_set0_num || k >= *channel_set1_num)) {
					i++;
					break;
				}
			}
		}
		if (parallel_passive) {
			/* Populate the list which can be filled with remaining channels */
			while (j < *channel_set0_num && i < chan_num) {
				(*chanspec_set0)[j++] = chanspec_list[i++];
			}
			while (k < *channel_set1_num && i < chan_num) {
				(*chanspec_set1)[k++] = chanspec_list[i++];
			}
		} else {
				*channel_set0_num = j;
				*channel_set1_num = k;
		}
	} else {

		/* if 5G-5G passive scanning is set and both wlc's are associated, then
		 * scan both 5G-5G channel set using wlc-0 & 1 first and then move the active
		 * channels. This is to avoid same band tx/rx when one core is associated in 5G
		 */
		/* The below loop enumerates active channel set. so set j,k so that active
		 * channels start after passive channels.
		 */
		if (parallel_passive && num_passive_chan && passive_channels_first) {
			/* set0 is 2G and set1 is for 5G channels */
			j = passive_2g;
			k = passive_5g;
		} else {
			j = 0;
			k = 0;
		}
		for (i = 0; i < chan_num; i++) {
			if (!wf_chspec_malformed(chanspec_list[i]) &&
				(wlc_scan_valid_chanspec_db(scan_info, chanspec_list[i]) ||
				(scan_info->scan_pub->state & SCAN_STATE_PROHIBIT)) &&
				!wlc_scan_quiet_chanspec(scan_info, chanspec_list[i])) {
				if (CHSPEC_IS2G(chanspec_list[i])) {
					(*chanspec_set0)[j++] = chanspec_list[i];
				} else {
					(*chanspec_set1)[k++] = chanspec_list[i];
				}
			}
		}

		if (parallel_passive && num_passive_chan && passive_channels_first) {
			/* set0 is 2G and set1 is for 5G channels */
			j = 0;
			k = 0;
		}
		/* Note that j and k are used below without resetting except when
		 * passive parallel first is set.
		 */
		if (num_passive_chan) {
			for (i = 0; i < chan_num; i++) {
				if (!wf_chspec_malformed(chanspec_list[i]) &&
					(wlc_scan_valid_chanspec_db(scan_info, chanspec_list[i]) ||
					(scan_info->scan_pub->state & SCAN_STATE_PROHIBIT)) &&
					wlc_scan_quiet_chanspec(scan_info, chanspec_list[i])) {

					if (CHSPEC_IS2G(chanspec_list[i]) && passive_2g-- > 0) {
						(*chanspec_set0)[j++] = chanspec_list[i];
					} else if (parallel_passive &&
						passive_2g-- > 0) {
						(*chanspec_set0)[j++] = chanspec_list[i];

					} else {
						(*chanspec_set1)[k++] = chanspec_list[i];
					}
				}
			}
		}
	}

	return BCME_OK;
}

static
void wlc_scan_free_chanspec_list(scan_info_t *scan_info)
{
	if (scan_info->scan_cmn->chanspeclist) {
		MFREE(SCAN_WLC(scan_info)->osh, scan_info->scan_cmn->chanspeclist,
		scan_info->scan_cmn->chanspec_list_size);
		scan_info->scan_cmn->chanspeclist = NULL;
		scan_info->scan_cmn->chanspec_list_size = 0;
	}
}

static
void wlc_parallel_scan_cb(void *arg, int status, wlc_bsscfg_t *cfg)
{
	wlc_info_t *scanned_wlc = (wlc_info_t*) arg;
	wlc_info_t *scan_request_wlc = cfg->wlc;
	scan_info_t *scan_info = (scan_info_t *)scanned_wlc->scan->scan_priv;
	int status2 = scan_info->scan_cmn->first_scanresult_status;
	int final_status = WLC_E_STATUS_INVALID;
	UNUSED_PARAMETER(scan_request_wlc);

	WL_SCAN(("wl%d.%d %s Scanned in wlc:%d, requested wlc:%d # of CBs %d\n",
		scanned_wlc->pub->unit, cfg->_idx, __FUNCTION__, scanned_wlc->pub->unit,
		scan_request_wlc->pub->unit, scan_info->scan_cmn->num_of_cbs));

	if (scan_info->scan_pub->state & SCAN_STATE_SUPPRESS) {
		scanned_wlc->mpc_scan = FALSE;
		wlc_scan_radio_mpc_upd(scan_info);
	}

	if (scan_info->state == WLC_SCAN_STATE_ABORT) {
		if (scan_info->scan_cmn->num_of_cbs > 0) {
			wlc_info_t *otherwlc;
			/* first scan aborted, abort the other and wait for it to finish */
			scan_info->scan_cmn->first_scanresult_status = status;
			otherwlc = wlc_rsdb_get_other_wlc(scanned_wlc);
			wlc_scan_abort(otherwlc->scan, status);
			return;
		} else {
			wlc_scan_free_chanspec_list(scan_info);
			 (*scan_info->scan_cmn->cb)(scan_info->scan_cmn->cb_arg, status, cfg);
			return;
		}
	}
	if (scan_info->scan_cmn->num_of_cbs > 0) {
		/* Wait for all scan to complete. */
		scan_info->scan_cmn->first_scanresult_status = status;
		return;
	}
	ASSERT(scan_info->scan_cmn->num_of_cbs == 0);
	/* single bands scan will have one scan complete callback. In such cases
	 * first status(status2) will be invalid.
	 */
	if (status2 == WLC_E_STATUS_INVALID || status == status2) {
		final_status = status;
	} else {
		/* We have different scan status from different wlc's...
		 * Send a single scan status based on these status codes.
		Order of increasing priority:
		 WLC_E_STATUS_NOCHANS
		 WLC_E_STATUS_SUCCESS
		 WLC_E_STATUS_PARTIAL
		 WLC_E_STATUS_SUPPRESS
		 Below ABORT status cases handled above:
		 WLC_E_STATUS_NEWASSOC
		 WLC_E_STATUS_CCXFASTRM
		 WLC_E_STATUS_11HQUIET
		 WLC_E_STATUS_CS_ABORT
		 WLC_E_STATUS_ABORT
		*/

		ASSERT(status != WLC_E_STATUS_ABORT);
		ASSERT(status2 != WLC_E_STATUS_ABORT);

		switch (status) {
			case WLC_E_STATUS_NOCHANS:
				final_status = status2;
				break;
			case WLC_E_STATUS_SUPPRESS:
				if (status2 == WLC_E_STATUS_SUCCESS ||
					status2 == WLC_E_STATUS_PARTIAL ||
					status2 == WLC_E_STATUS_NOCHANS)
					final_status = status;
				break;
			case WLC_E_STATUS_SUCCESS:
				if (status2 == WLC_E_STATUS_NOCHANS)
					final_status = status;
				else
					final_status = status2;
				break;
			case WLC_E_STATUS_PARTIAL:
				if (status2 == WLC_E_STATUS_SUPPRESS)
					final_status = status2;
				else
					final_status = status;
				break;
			default:
				/* No Other valid status in scanning apart from abort.
				 * If new status got introduced, handle it in
				 * new switch case.
				 */
				/* Use the last status */
				final_status = status;
				WL_SCAN_ERROR(("Err. Parallel scan"
				" status's are not matching\n"));
				ASSERT(0);
		}
	}

	scan_info->scan_cmn->flag &= ~(SCAN_CMN_FLAG_5G_5G_PASSIVE);

	wlc_scan_free_chanspec_list(scan_info);
	(*scan_info->scan_cmn->cb)(scan_info->scan_cmn->cb_arg,
		final_status, cfg);
}
#endif /* WLRSDB */

/* Caution: when passing a non-primary bsscfg to this function the caller
 * must make sure to abort the scan before freeing the bsscfg!
 */
int
wlc_scan(
	wlc_scan_info_t *wlc_scan_info,
	int bss_type,
	const struct ether_addr* bssid,
	int nssid,
	wlc_ssid_t *ssid,
	int scan_type,
	int nprobes,
	int active_time,
	int passive_time,
	int home_time,
	const chanspec_t* chanspec_list,
	int channel_num,
	chanspec_t chanspec_start,
	bool save_prb,
	scancb_fn_t fn, void* arg,
	int away_channels_limit,
	bool extdscan,
	bool suppress_ssid,
	bool include_cache,
	uint scan_flags,
	wlc_bsscfg_t *cfg,
	uint8 usage,
	actcb_fn_t act_cb, void *act_cb_arg,
	struct ether_addr *sa_override)
{

	scan_info_t *scan_info = (scan_info_t *) wlc_scan_info->scan_priv;
	uint ret_val = BCME_OK;
#ifdef WLRSDB
	wlc_info_t *wlc_2g, *wlc_5g;
	uint ret_val0 = BCME_OK, ret_val1 = BCME_OK;
	int parallel_scan_disabled = FALSE;
#if defined(BCMDBG) || defined(WLMSG_SCAN)
	char chanbuf[CHANSPEC_STR_LEN];
#endif /* defined(BCMDBG) || defined(WLMSG_SCAN) */
#endif /* WLRSDB */

	wlc_scan_info->wlc_scan_cmn->usage = (uint8)usage;

	WL_INFORM(("wl%d %s bss_type=%d nssid=%d nprobes=%d active_time=%d channel_num=%d"
		" save_prb=%d fn=%p away_channels_limit=%d extdscan=%d suppress_ssid=%d "
		"scan_flags=0x%x usage=%d passive_time=%d\n", SCAN_UNIT(scan_info), __FUNCTION__,
		bss_type, nssid, nprobes, active_time, channel_num, save_prb, fn,
		away_channels_limit, extdscan, suppress_ssid, scan_flags, usage, passive_time));

	/* channel list validation */
	if (channel_num > MAXCHANNEL) {
		WL_SCAN_ERROR(("wl%d: %s: wlc_scan bad param channel_num %d greater"
		" than max %d\n", scan_info->unit, __FUNCTION__,
		channel_num, MAXCHANNEL));
		channel_num = 0;
	}

	if (channel_num > 0 && chanspec_list == NULL) {
		WL_SCAN_ERROR(("wl%d: %s: wlc_scan bad param channel_list was NULL"
			" with channel_num = %d\n",
			scan_info->unit, __FUNCTION__, channel_num));
		channel_num = 0;
	}
#ifdef STA
		if (!IS_EXTSTA_ENAB(scan_info) && !IS_AS_IN_PROGRESS(scan_info) &&
			ANY_SCAN_IN_PROGRESS(wlc_scan_info)) {
			/* Abort the if any non-assoc scan is already in progress. */
			wlc_scan_abort(wlc_scan_info, WLC_E_STATUS_ABORT);
		}
#endif /* STA */

#ifdef WLRSDB

	/* If act_cb is provided, disable parallel scan. This is required as the caller may not
	 * handle action CB coming on other wlc. Enable it only if WL_SCANFLAGS_FORCE_PARALLEL
	 * is defined.
	 */
	 if (!(scan_flags & WL_SCANFLAGS_FORCE_PARALLEL) && act_cb)
		parallel_scan_disabled = TRUE;

	/* Disable parallel scanning in case of band locked scenarios */
	if (SCAN_WLC(scan_info)->bandlocked)
		parallel_scan_disabled = TRUE;

	/* RSDB is enabled on this chip... trigger parallel scan
	 * if parallel scan is not disabled.
	 */
	if (RSDB_PARALLEL_SCAN_ON(scan_info) && !parallel_scan_disabled) {
		chanspec_t *chanspec_list5g = NULL;
		chanspec_t *chanspec_list2g = NULL;
		scan_info_t *scan_info_2g = NULL;
		scan_info_t *scan_info_5g = NULL;
		bool both_wlc_scan = FALSE;

		chanspec_t chanspec_start_2g = 0, chanspec_start_5g = 0;
		int channel_num2g = 0, channel_num5g = 0;
		int err;

		cfg = SCAN_USER(scan_info, cfg);

		WL_SCAN(("wl%d.%d:%s RSDB PARALLEL SCANNING.\n",
			cfg->wlc->pub->unit, cfg->_idx, __FUNCTION__));

		if (channel_num <= 0) {
			scan_info->passive_time =
				scan_info->scan_cmn->defaults.passive_time;

			if (passive_time >= 0)
				scan_info->passive_time = (uint16)passive_time;

			/* Generate the list of channels from CLM */
			wlc_scan_default_channels(wlc_scan_info,
				wf_chspec_ctlchspec(SCAN_HOME_CHANNEL(scan_info)),
				WLC_BAND_ALL, scan_info->chanspec_list,
				&scan_info->channel_num);

			chanspec_list = scan_info->chanspec_list;
			channel_num = scan_info->channel_num;
		}

		err = wlc_scan_split_channels_per_band(scan_info,
			chanspec_list, channel_num, &chanspec_list2g,
			&chanspec_list5g, &channel_num2g, &channel_num5g, active_time,
			passive_time, scan_type);

		if (err != BCME_OK) {
			wlc_scan_free_chanspec_list(scan_info);
			return err;
		}

		/* Return if no chanset enumeration. */
		if (!(channel_num2g || channel_num5g)) {
			wlc_scan_free_chanspec_list(scan_info);
			return BCME_EPERM;
		}
		else if (channel_num2g && channel_num5g) {
			both_wlc_scan = TRUE;
		}

		chanspec_start_2g = chanspec_list2g[0];
		chanspec_start_5g = chanspec_list5g[0];

		/* Setup valid scan callbacks. */
		scan_info->scan_cmn->cb = fn;
		scan_info->scan_cmn->cb_arg = arg;
		scan_info->scan_cmn->num_of_cbs = 0;
		scan_info->scan_cmn->first_scanresult_status = WLC_E_STATUS_INVALID;
		/* scan_info->scan_cmn->cfg = SCAN_USER(scan_info, cfg); */

		wlc_rsdb_get_wlcs(cfg->wlc, &wlc_2g, &wlc_5g);

		/* Becuase of core 0 and core 1 up
		* current chanspec (wlc->chanspec) could have changed
		* so update the wlc_2g and wlc_5g based on the current chanspec (wlc->chanspec)
		*/
		wlc_rsdb_get_wlcs(cfg->wlc, &wlc_2g, &wlc_5g);

		if (both_wlc_scan) {
			/* need to wake up both wlcs together before scheduling first request */
			scan_info_2g = (scan_info_t *)wlc_2g->scan->scan_priv;
			scan_info_5g = (scan_info_t *)wlc_5g->scan->scan_priv;
			wlc_2g->mpc_scan = TRUE;
			wlc_scan_radio_mpc_upd(scan_info_2g);
			wlc_5g->mpc_scan = TRUE;
			wlc_scan_radio_mpc_upd(scan_info_5g);
		}

		/* 2G band scan */
		if (channel_num2g) {

			if (!wf_chspec_valid(chanspec_start_2g)) {
				chanspec_start_2g =
					wf_chspec_ctlchspec(wlc_2g->home_chanspec);
			}
			WL_SCAN(("Scanning on wl%d.%d, Start chanspec:%s (2G), total:%d\n",
				wlc_2g->pub->unit, cfg->_idx,
				wf_chspec_ntoa_ex(chanspec_start_2g, chanbuf),
				channel_num2g));

			/* Get the control chanspecs of channel. */
			chanspec_start_2g = wf_chspec_ctlchspec(chanspec_start_2g);

			ret_val0 = _wlc_scan(wlc_2g->scan, bss_type, bssid, nssid, ssid,
				scan_type, nprobes, active_time, passive_time, home_time,
				chanspec_list2g, channel_num2g, chanspec_start,
				save_prb, wlc_parallel_scan_cb, wlc_2g, away_channels_limit,
				extdscan, suppress_ssid, include_cache, scan_flags,
				cfg, act_cb, act_cb_arg, WLC_BAND_2G, chanspec_start_2g,
				sa_override);

			if (ret_val0 == BCME_OK) {
				scan_info->scan_cmn->num_of_cbs++;
			} else {
				if (both_wlc_scan) {
					/* mpc update for 5G wlc also as 2g wlc scan failed */
					wlc_5g->mpc_scan = FALSE;
					wlc_scan_radio_mpc_upd(scan_info_5g);
					wlc_scan_radio_upd(scan_info_5g);
				}
				wlc_scan_free_chanspec_list(scan_info);
				return ret_val0;
			}
		}

		/* 5G band scan */
		if (channel_num5g) {
			if (!wf_chspec_valid(chanspec_start_5g)) {
				chanspec_start_5g =
					wf_chspec_ctlchspec(wlc_5g->home_chanspec);
			}

			WL_SCAN(("Scanning on wl%d.%d Start chanspec:%s (5G) total:%d\n",
				wlc_5g->pub->unit, cfg->_idx,
				wf_chspec_ntoa_ex(chanspec_start_5g, chanbuf),
				channel_num5g));
			/* Get the control chanspecs of channel. */
			chanspec_start_5g = wf_chspec_ctlchspec(chanspec_start_5g);

			ret_val1 = _wlc_scan(wlc_5g->scan, bss_type, bssid, nssid, ssid,
				scan_type, nprobes, active_time, passive_time, home_time,
				chanspec_list5g, channel_num5g, chanspec_start,
				save_prb, wlc_parallel_scan_cb, wlc_5g, away_channels_limit,
				extdscan, suppress_ssid, include_cache, scan_flags,
				cfg, act_cb, act_cb_arg, WLC_BAND_5G,
				chanspec_start_5g, sa_override);

			if (ret_val1 == BCME_OK) {
				scan_info->scan_cmn->num_of_cbs++;
			}
			else {
				/*
				* if 5G WLC scan failed it's mpc update will happen in _wlc_scan()
				* no need to explicitly do anything here
				*/
			}
		}

		if (scan_info->scan_cmn->num_of_cbs)
			ret_val = BCME_OK;
		else {
			wlc_scan_free_chanspec_list(scan_info);
			return BCME_EPERM;
		}
	} else
#endif /* WLRSDB */
	{
		ret_val = _wlc_scan(wlc_scan_info, bss_type, bssid, nssid, ssid,
			scan_type, nprobes, active_time, passive_time, home_time,
			chanspec_list, channel_num, chanspec_start,
			save_prb, fn, arg, away_channels_limit, extdscan, suppress_ssid,
			include_cache, scan_flags, cfg, act_cb, act_cb_arg, WLC_BAND_ALL,
			wf_chspec_ctlchspec(SCAN_HOME_CHANNEL(scan_info)), sa_override);
	}
#ifdef WLSCAN_SUMEVT
		if (ret_val == BCME_OK && SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
			if (RSDB_PARALLEL_SCAN_ON(scan_info)) {
				scan_info->scan_cmn->scn_summary->scan_flags |=
					PARALLEL_SCAN;
				if (WLC_SCAN_SIB_ISSET(SCAN_WLC(scan_info), BAND_5G_INDEX))
					scan_info->scan_cmn->scn_summary->scan_flags |=
						BAND5G_SIB_ENAB;
				if (WLC_SCAN_SIB_ISSET(SCAN_WLC(scan_info), BAND_2G_INDEX))
					scan_info->scan_cmn->scn_summary->scan_flags |=
						BAND2G_SIB_ENAB;
			}
			scan_info->scan_cmn->scan_sync_id++;
			scan_info->scan_cmn->scn_summary->u.scan_sum_info.scan_start_time =
					OSL_SYSUPTIME();
		}
#endif /* WLSCAN_SUMEVT */
	return ret_val;
}

int _wlc_scan(
	wlc_scan_info_t *wlc_scan_info,
	int bss_type,
	const struct ether_addr* bssid,
	int nssid,
	wlc_ssid_t *ssid,
	int scan_type,
	int nprobes,
	int active_time,
	int passive_time,
	int home_time,
	const chanspec_t* chanspec_list, int channel_num, chanspec_t chanspec_start,
	bool save_prb,
	scancb_fn_t fn, void* arg,
	int away_channels_limit,
	bool extdscan,
	bool suppress_ssid,
	bool include_cache,
	uint scan_flags,
	wlc_bsscfg_t *cfg,
	actcb_fn_t act_cb, void *act_cb_arg, int bandinfo, chanspec_t band_chanspec_start,
	struct ether_addr *sa_override)
{
	scan_info_t *scan_info = (scan_info_t *) wlc_scan_info->scan_priv;
	bool scan_in_progress;
	int i, num;
	wlc_info_t *wlc = scan_info->wlc;

#if defined(BCMDBG) || defined(WLMSG_INFORM)
	char *ssidbuf;
	char eabuf[ETHER_ADDR_STR_LEN];
	char chanbuf[CHANSPEC_STR_LEN];
#endif
#ifdef WLMSG_ROAM
	char SSIDbuf[DOT11_MAX_SSID_LEN+1];
#endif
	int ret = BCME_OK;
	BCM_REFERENCE(wlc);
	BCM_REFERENCE(home_time);
	BCM_REFERENCE(away_channels_limit);
	BCM_REFERENCE(suppress_ssid);
	ASSERT(nssid);
	ASSERT(ssid != NULL);
	ASSERT(bss_type == DOT11_BSSTYPE_INFRASTRUCTURE ||
	       bss_type == DOT11_BSSTYPE_INDEPENDENT ||
	       bss_type == DOT11_BSSTYPE_MESH ||
	       bss_type == DOT11_BSSTYPE_ANY);

	WL_SCAN(("wl%d: %s: scan request at %u\n", scan_info->unit, __FUNCTION__,
		wl_scan_hnd_time()));
#if defined(BCMDBG) || defined(WLMSG_INFORM)
	ssidbuf = (char *) MALLOCZ(scan_info->osh, SSID_FMT_BUF_LEN);
	if (ssidbuf == NULL) {
		WL_ERROR((WLC_MALLOC_ERR, scan_info->unit, __FUNCTION__, (int)SSID_FMT_BUF_LEN,
			MALLOCED(scan_info->osh)));
		return BCME_NOMEM;
	}

	if (nssid == 1) {
		wlc_format_ssid(ssidbuf, ssid->SSID, ssid->SSID_len);
		WL_INFORM_SCAN(("wl%d: %s: scanning for SSID \"%s\"\n", scan_info->unit,
			__FUNCTION__, ssidbuf));
	} else {
		WL_INFORM_SCAN(("wl%d: %s: scanning for SSIDs:\n", scan_info->unit, __FUNCTION__));
		if (WL_INFORM_ON())
			wlc_scan_print_ssids(ssid, nssid);
	}
	WL_INFORM_SCAN(("wl%d: %s: scanning for BSSID \"%s\"\n", scan_info->unit, __FUNCTION__,
	           (bcm_ether_ntoa(bssid, eabuf), eabuf)));
	MFREE(scan_info->osh, (void *)ssidbuf, SSID_FMT_BUF_LEN);
#endif /* BCMDBG || WLMSG_INFORM */

	/* enforce valid argument */
	scan_info->ssid_wildcard_enabled = 0;
	for (i = 0; i < nssid; i++) {
		if (ssid[i].SSID_len > DOT11_MAX_SSID_LEN) {
			WL_SCAN(("wl%d: %s: invalid SSID len %d, capping\n",
			          scan_info->unit, __FUNCTION__, ssid[i].SSID_len));
			ssid[i].SSID_len = DOT11_MAX_SSID_LEN;
		}
		if (ssid[i].SSID_len == 0)
			scan_info->ssid_wildcard_enabled = 1;
	}

	scan_in_progress = SCAN_IN_PROGRESS(wlc_scan_info);

	/* clear or set optional params to default */
	/* keep persistent scan suppress flag */
	wlc_scan_info->state &= SCAN_STATE_SUPPRESS;
	scan_info->nprobes = scan_info->scan_cmn->defaults.nprobes;
	if (IS_ASSOCIATED(scan_info)) {
		scan_info->active_time = scan_info->scan_cmn->defaults.assoc_time;
	} else {
		scan_info->active_time = scan_info->scan_cmn->defaults.unassoc_time;
	}
	scan_info->passive_time = scan_info->scan_cmn->defaults.passive_time;
	if (scan_info->scan_cmn->defaults.passive)
		wlc_scan_info->state |= SCAN_STATE_PASSIVE;

	if (scan_type == DOT11_SCANTYPE_ACTIVE) {
		wlc_scan_info->state &= ~SCAN_STATE_PASSIVE;
	} else if (scan_type == DOT11_SCANTYPE_PASSIVE) {
		wlc_scan_info->state |= SCAN_STATE_PASSIVE;
	}
	/* passive scan always has nprobes to 1 */
	if (wlc_scan_info->state & SCAN_STATE_PASSIVE) {
		scan_info->nprobes = 1;
	}
	if (active_time > 0)
		scan_info->active_time = (uint16)active_time;

	if (passive_time >= 0)
		scan_info->passive_time = (uint16)passive_time;

	if (nprobes > 0 && ((wlc_scan_info->state & SCAN_STATE_PASSIVE) == 0))
		scan_info->nprobes = (uint8)nprobes;
	if (save_prb)
		wlc_scan_info->state |= SCAN_STATE_SAVE_PRB;
	if (include_cache && SCANCACHE_ENAB(wlc_scan_info))
		wlc_scan_info->state |= SCAN_STATE_INCLUDE_CACHE;

	if (scan_flags & WL_SCANFLAGS_HOTSPOT) {
#ifdef ANQPO
		if (SCAN_ANQPO_ENAB(scan_info)) {
			wl_scan_anqpo_scan_start(scan_info);
			wlc_scan_info->wlc_scan_cmn->is_hotspot_scan = TRUE;
		}
#endif
	} else {
		wlc_scan_info->wlc_scan_cmn->is_hotspot_scan = FALSE;
	}
#ifdef STA
#ifdef WL11D
	/* If we're doing autocountry, clear country info accumulation */
	if (IS_AUTOCOUNTRY_ENAB(scan_info))
		wlc_scan_11d_scan_start(scan_info);
#endif /* WL11D */
#endif /* STA */

#ifdef WL_PROXDETECT
	if (PROXD_ENAB(wlc->pub)) {
		wlc_scan_info->flag &= ~SCAN_FLAG_SWITCH_CHAN;
		if (scan_flags & WL_SCANFLAGS_SWTCHAN) {
			wlc_scan_info->flag |= SCAN_FLAG_SWITCH_CHAN;
		}
	}
#endif

	WL_SCAN(("wl%d: %s: wlc_scan params: nprobes %d dwell active/passive %dms/%dms"
		" flags %d\n",
		scan_info->unit, __FUNCTION__, scan_info->nprobes, scan_info->active_time,
		scan_info->passive_time, wlc_scan_info->state));

	/* In case of ULB Mode with parallel scan disabled, we need scan_info->bss_cfg to
	 * point to correct bsscfg so that min_bw value configured by user can be picked
	 * for scanning
	 */
	scan_info->bsscfg = SCAN_USER(scan_info, cfg);

	if (!wlc_scan_info->iscan_cont) {
		wlc_scan_default_channels(wlc_scan_info, band_chanspec_start, bandinfo,
		scan_info->chanspec_list, &scan_info->channel_num);
	}

	if (scan_flags & WL_SCANFLAGS_PROHIBITED) {
		scan_info->scan_pub->state |= SCAN_STATE_PROHIBIT;
		num = wlc_scan_prohibited_channels(scan_info,
			&scan_info->chanspec_list[scan_info->channel_num],
			(MAXCHANNEL - scan_info->channel_num));
		scan_info->channel_num += num;
	} else
		scan_info->scan_pub->state &= ~SCAN_STATE_PROHIBIT;

	if (scan_flags & WL_SCANFLAGS_OFFCHAN)
		scan_info->scan_pub->state |= SCAN_STATE_OFFCHAN;
	else
		scan_info->scan_pub->state &= ~SCAN_STATE_OFFCHAN;

	if (IS_SIM_ENAB(scan_info)) {
		/* QT hack: abort scan since full scan may take forever */
		scan_info->channel_num = 1;
	}

	/* set required and optional params */
	/* If IBSS Lock Out feature is turned on, set the scan type to BSS only */
	wlc_scan_info->wlc_scan_cmn->bss_type =
		(IS_IBSS_ALLOWED(scan_info) == FALSE)?DOT11_BSSTYPE_INFRASTRUCTURE:bss_type;
	bcopy((const char*)bssid, (char*)&wlc_scan_info->bssid, ETHER_ADDR_LEN);

	/* allocate memory for ssid list, using prealloc if sufficient */
	ASSERT(scan_info->ssid_list == scan_info->ssid_prealloc);
	if (scan_info->ssid_list != scan_info->ssid_prealloc) {
		WL_SCAN(("wl%d: %s: ssid_list not set to prealloc\n",
		          scan_info->unit, __FUNCTION__));
	}
	if (nssid > scan_info->nssid_prealloc) {
		scan_info->ssid_list = MALLOCZ(scan_info->osh,
		                              nssid * sizeof(wlc_ssid_t));
		/* failed, cap at prealloc (best effort) */
		if (scan_info->ssid_list == NULL) {
			WL_ERROR((WLC_MALLOC_ERR, scan_info->unit, __FUNCTION__,
				(int)(nssid * sizeof(wlc_ssid_t)),
				MALLOCED(scan_info->osh)));
			nssid = scan_info->nssid_prealloc;
			scan_info->ssid_list = scan_info->ssid_prealloc;
		}
	}
	/* Now ssid_list is the right size for [current] nssid count */

	bcopy(ssid, scan_info->ssid_list, (sizeof(wlc_ssid_t) * nssid));
	scan_info->nssid = nssid;

#ifdef STA
	/* Update the mpc state to bring up the core */
	wlc->mpc_scan = TRUE;
	wlc_scan_radio_mpc_upd(scan_info);
#endif /* STA */

	if (!wlc->pub->up) {
		/* when the driver is going down, mpc can't pull up the core */
		WL_ERROR(("wl%d: %s, can not scan while driver is down\n",
			wlc->pub->unit, __FUNCTION__));
		ret = BCME_NOTUP;
		goto end;
	}

#ifdef WLP2P
	if (IS_P2P_ENAB(scan_info)) {
		scan_info->ssid_wildcard_enabled = FALSE;
		for (i = 0; i < nssid; i ++) {
			if (scan_info->ssid_list[i].SSID_len == 0)
				wlc_p2p_fixup_SSID(wlc->p2p, cfg,
					&scan_info->ssid_list[i]);
			if (scan_info->ssid_list[i].SSID_len == 0)
				scan_info->ssid_wildcard_enabled = TRUE;
		}
	}
#endif

	/* channel list validation */
	if (channel_num > MAXCHANNEL) {
		WL_SCAN(("wl%d: %s: wlc_scan bad param channel_num %d greater than max %d\n",
			scan_info->unit, __FUNCTION__, channel_num, MAXCHANNEL));
		channel_num = 0;
	}
	if (channel_num > 0 && chanspec_list == NULL) {
		WL_SCAN(("wl%d: %s: wlc_scan bad param channel_list was NULL with channel_num ="
			" %d\n",
			scan_info->unit, __FUNCTION__, channel_num));
		channel_num = 0;
	}
	for (i = 0; i < channel_num; i++) {
		if ((scan_flags & WL_SCANFLAGS_PROHIBITED) ||
			WLC_CNTRY_DEFAULT_ENAB(wlc)) {
			if (wf_chspec_valid(chanspec_list[i]) == FALSE) {
				channel_num = 0;
			}
		}
		else if (!wlc_scan_valid_chanspec_db(scan_info, chanspec_list[i])) {
			channel_num = 0;
		}
	}

	if (channel_num > 0) {
		for (i = 0; i < channel_num; i++)
			scan_info->chanspec_list[i] = chanspec_list[i];
		scan_info->channel_num = channel_num;
	}
#ifdef BCMCCX
	else if (BSS_CCX_ENAB(SCAN_WLC(scan_info), SCAN_USER(scan_info, cfg))) {
		/* no fast roam if the roam channel list is invalid */
		wlc_ccx_set_fast_roam(wlc->ccx, SCAN_USER(scan_info, cfg), FALSE);
	}
#endif
#ifdef WLMSG_ROAM
	bcopy(&ssid->SSID, SSIDbuf, ssid->SSID_len);
	SSIDbuf[ssid->SSID_len] = 0;
	WL_ROAM(("SCAN for '%s' %d SSID(s) %d channels\n", SSIDbuf, nssid, channel_num));
#endif /* WLMSG_ROAM */

#ifdef BCMDBG
	if (WL_INFORM_ON()) {
		char chan_list_buf[128];
		struct bcmstrbuf b;

		bcm_binit(&b, chan_list_buf, sizeof(chan_list_buf));

		for (i = 0; i < scan_info->channel_num; i++) {
			bcm_bprintf(&b, " %s",
				wf_chspec_ntoa_ex(scan_info->chanspec_list[i], chanbuf));

			if ((i % 8) == 7 || (i + 1) == scan_info->channel_num) {
				WL_INFORM_SCAN(("wl%d: wlc_scan: scan channels %s\n",
					scan_info->unit, chan_list_buf));
				bcm_binit(&b, chan_list_buf, sizeof(chan_list_buf));
			}
		}
	}
#endif /* BCMDBG */

	if ((wlc_scan_info->state & SCAN_STATE_SUPPRESS) || (!scan_info->channel_num)) {
		int status;

		WL_INFORM_SCAN(("wl%d: %s: scan->state %d scan->channel_num %d\n",
			scan_info->unit, __FUNCTION__,
			wlc_scan_info->state, scan_info->channel_num));

		if (wlc_scan_info->state & SCAN_STATE_SUPPRESS)
			status = WLC_E_STATUS_SUPPRESS;
		else
			status = WLC_E_STATUS_NOCHANS;

		if (scan_in_progress)
			wlc_scan_abort(wlc_scan_info, status);

		/* no scan now, but free any earlier leftovers */
		wlc_scan_bss_list_free(scan_info);

		if (fn != NULL)
			(fn)(arg, status, SCAN_USER(scan_info, cfg));

		wlc_scan_ssidlist_free(scan_info);
		ret = BCME_EPERM;
		goto end;
	}

	if (!sa_override || ETHER_ISNULLADDR(sa_override)) {
		sa_override = &scan_info->bsscfg->cur_etheraddr;
	}
	memcpy(&scan_info->sa_override, sa_override, sizeof(scan_info->sa_override));

	scan_info->cb = fn;
	scan_info->cb_arg = arg;

	scan_info->act_cb = act_cb;
	scan_info->act_cb_arg = act_cb_arg;

	scan_info->extdscan = extdscan;

#ifdef EXT_STA
	/* Save flag: suppress ssid in probes during connection attempt
	 * following hibernation
	 */
	if (IS_EXTSTA_ENAB(scan_info))
		scan_info->suppress_ssid = suppress_ssid;
#endif /* EXT_STA */

	/* extd scan for nssids one ssid per each pass..  */
	scan_info->npasses = (scan_info->extdscan && nssid) ? nssid : scan_info->nprobes;
	scan_info->channel_idx = 0;
	if (chanspec_start != 0) {
		for (i = 0; i < scan_info->channel_num; i++) {
			if (scan_info->chanspec_list[i] == chanspec_start) {
				scan_info->channel_idx = i;
				WL_INFORM_SCAN(("starting new iscan on chanspec %s\n",
				           wf_chspec_ntoa_ex(chanspec_start, chanbuf)));
				break;
			}
		}
	}
	wlc_scan_info->in_progress = TRUE;
#ifdef WLOLPC
	/* if on olpc chan notify - if needed, terminate active cal; go off channel */
	if (OLPC_ENAB(wlc)) {
		wlc_olpc_eng_hdl_chan_update(wlc->olpc_info);
	}
#endif /* WLOLPC */
	wlc_phy_hold_upd(SCAN_GET_PI_PTR(scan_info), PHY_HOLD_FOR_SCAN, TRUE);

#if defined(STA)
	wlc_scan_info->wlc_scan_cmn->scan_start_time = OSL_SYSUPTIME();
#endif /* STA */

	/* ...and free any leftover responses from before */
	wlc_scan_bss_list_free(scan_info);


	/* keep core awake to receive solicited probe responses, SCAN_IN_PROGRESS is TRUE */
	ASSERT(SCAN_STAY_AWAKE(scan_info));
	wlc_scan_set_wake_ctrl(scan_info);

#if !defined(WLOFFLD) && !defined(SCANOL) && defined(WLDFS)
	/* If we are switching away from radar home_chanspec
	 * because STA scans (normal/Join/Roam) with
	 * atleast one local 11H AP in radar channel,
	 * turn of radar_detect.
	 * NOTE: Implied that upstream AP assures this radar
	 * channel is clear.
	 */
	if (WLDFS_ENAB(wlc->pub) && WL11H_AP_ENAB(wlc) && scan_info->bsscfg != NULL &&
			BSSCFG_AP(scan_info->bsscfg) && scan_info->bsscfg->up &&
			WLC_APSTA_ON_RADAR_CHANNEL(wlc) &&
		wlc_radar_chanspec(wlc->cmi, wlc->home_chanspec) && wlc->dfs != NULL) {
		WL_REGULATORY(("wl%d: %s Moving from home channel dfs OFF\n",
			wlc->pub->unit, __FUNCTION__));
		wlc_set_dfs_cacstate(wlc->dfs, OFF, scan_info->bsscfg);
	}
#endif /* !defined(WLOFFLD) && !defined(SCANOL)) && defined(WLDFS) */

#ifdef WLSCAN_PS
	if (WLSCAN_PS_ENAB(wlc->pub)) {
		/* Support for MIMO force depends on underlying WLSCAN_PS code */
		if (scan_flags & WL_SCANFLAGS_MIMO)
			scan_info->scan_cmn->scan_mimo_override = TRUE;
		else
			scan_info->scan_cmn->scan_mimo_override = FALSE;
	}
#endif /* WLSCAN_PS */

	if ((ret = _wlc_scan_schd_channels(wlc_scan_info)) != BCME_OK) {
		/* call wlc_scantimer to get the scan state machine going */
		/* DUALBAND - Don't call wlc_scantimer() directly from DPC... */
		/* send out AF as soon as possible to aid reliability of GON */
		wlc_scan_abort(wlc_scan_info, WLC_SCAN_STATE_ABORT);
		goto end;
	}

#ifdef AWDL_FAMILY
	if ((cfg == wlc->cfg) &&
		(cfg->assoc->type == AS_ROAM)) {
		WL_ASSOC(("%s(): AS_ROAM start...\n", __FUNCTION__));
		wlc_bss_mac_event(wlc, cfg, WLC_E_ROAM_START, NULL, 0,
		cfg->roam->reason, ON, 0, 0);
	}
#endif
#ifdef WLRSDB
	if (RSDB_ENAB(wlc->pub)) {
		if (ret == BCME_OK) {
			wlc_scan_disable_idle_pscan_wlc(scan_info);
		}
	}
#endif
	/* if a scan is in progress, allow the next callback to restart the scan state machine */
	return ret;

end:
#ifdef STA
	/* Update the mpc state  to bring the core down */
	wlc->mpc_scan = FALSE;
	wlc_scan_radio_mpc_upd(scan_info);
	wlc_scan_radio_upd(scan_info);
#endif /* STA */
	return ret;
}

static uint32
_wlc_get_next_chan_scan_time(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_info_t *wlc = scan_info->wlc;
	chanspec_t next_chanspec;
	uint32 channel_dwelltime;
	uint8 passes;

	if (scan_info->channel_idx < scan_info->channel_num) {
		next_chanspec = scan_info->chanspec_list[scan_info->channel_idx];
	} else {
		return SCAN_INVALID_DWELL_TIME;
	}

	if (scan_info->scan_pub->state & SCAN_STATE_PASSIVE) {
		return CHANNEL_PASSIVE_DWELLTIME(scan_info);
	} else {
		if (wlc_quiet_chanspec(wlc->cmi, next_chanspec) ||
		    !wlc_valid_chanspec_db(wlc->cmi, next_chanspec)) {
			return CHANNEL_PASSIVE_DWELLTIME(scan_info);
		}

		channel_dwelltime = CHANNEL_ACTIVE_DWELLTIME(scan_info);
		/* Return remaining dwell time considering pass, nprobes and home time */
		if (scan_info->pass > 1) {
			passes = (!wlc_scan_valid_chanspec_db(scan_info, next_chanspec)) ? 1
				: scan_info->npasses;
			/* Cap remaining dwell to active/home time in case calculation goes wrong */
			return (MIN(channel_dwelltime,
				((channel_dwelltime/passes) * (passes - scan_info->pass + 1))));
		}
		return channel_dwelltime;
	}
}

#ifdef WLRSDB
static uint32
wlc_scan_get_current_home_time(scan_info_t * scan_info)
{
	wlc_info_t *wlc = scan_info->wlc;
	int idx;
	wlc_cmn_info_t *wlc_cmn = wlc->cmn;
	wlc_info_t *wlc_iter;
	uint32 max_time = 0;

	FOREACH_WLC(wlc_cmn, idx, wlc_iter) {
		if (!AP_ACTIVE(wlc_iter)) {
			continue;
		}
		else {
			int ap_idx;
			wlc_bsscfg_t *ap_cfg;
			FOREACH_UP_AP(wlc_iter, ap_idx, ap_cfg) {
				uint32 bcn_per =  (ap_cfg->current_bss->beacon_period <<
					MSCH_TIME_UNIT_1024US);
				if (max_time < bcn_per)
					max_time = bcn_per;
			}
		}
	}
	if (max_time == 0) {
		/* no APs associated on either core */
		max_time = DEFAULT_HOME_TIME * 1000;
	}
	else {
		/* extra 5ms to guarantee that for AP case, beacon is TXd on home channel */
		max_time = MAX((max_time + 5*1000), DEFAULT_HOME_TIME * 1000);
	}
	return max_time;
}
#endif /* WLRSDB */

static int
_wlc_scan_schd_channels_complete(wlc_scan_info_t *wlc_scan_info,
	void *cb, bool FIXED_SLOT)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	uint32 dwell_time = _wlc_get_next_chan_scan_time(wlc_scan_info);
	uint32 chn_cnt = 1;
	int chn_start = scan_info->channel_idx;
	wlc_msch_req_param_t req_param;
	int err;

	memset(&req_param, 0, sizeof(req_param));

	if (!FIXED_SLOT) {
		/*
		* this will be reached in non-RSDB builds or
		* when we don't want to do 5G-5G passive parallel scan
		*/

		/*
		* We've got a channel and a target dwell time. So, get the other channels with the
		* same dwell time.
		*/
		while (++scan_info->channel_idx < scan_info->channel_num) {
			if (_wlc_get_next_chan_scan_time(wlc_scan_info) != dwell_time) {
				break;
			}
			chn_cnt++;
		}
		scan_info->cur_scan_chanspec = 0;
		req_param.duration = MS_TO_USEC(dwell_time);
		req_param.req_type = MSCH_RT_START_FLEX;
		req_param.priority = MSCH_RP_CONNECTION;
		if ((err = wlc_msch_timeslot_register(wlc->msch_info,
			&scan_info->chanspec_list[chn_start],
			chn_cnt, wlc_scan_chnsw_clbk, wlc_scan_info, &req_param,
			&scan_info->msch_req_hdl)) != BCME_OK) {
			WL_ERROR(("wl%d: %s: MSCH timeslot register failed, err %d\n",
				scan_info->unit, __FUNCTION__, err));
			return err;
		}
	}
#ifdef WLRSDB
	else {
		if (RSDB_ENAB(wlc->pub)) {
			/*
			* this will be reached when 5G-5G
			* passive parallel scan need to be done in RSDB builds.
			*/
			scan_sched_complete_cb CB = NULL;
			wlc_info_t *other_wlc = (wlc_info_t *)wlc_rsdb_get_other_wlc(wlc);
			uint64 start_time = msch_future_time(wlc->msch_info,
				wlc_scan_get_current_home_time(scan_info));

			if (cb != NULL) {
				CB = (scan_sched_complete_cb)cb;
			}

			ASSERT(SCAN_CURRENT_5G_PASSIVE(scan_info));
			ASSERT(SCAN_PASSIVE_5G_CHANSPEC(scan_info));

			/* resgister fixed slot for both wlcs now */
			if (CB != NULL) {
				/* complete "other_wlc" scan MSCH registration first */
				if ((err = CB(other_wlc->scan, NULL, TRUE)) != BCME_OK) {
					return err;
				}
			}

			/* complete "this" wlc now */
			scan_info->cur_scan_chanspec = 0;
			req_param.duration = MS_TO_USEC(dwell_time);
			req_param.req_type = MSCH_RT_BOTH_FIXED;
			req_param.priority = MSCH_DEFAULT_PRIO;
			req_param.start_time_l = (uint32)(start_time);
			req_param.start_time_h = (uint32) (start_time >> 32);

			if ((err = wlc_msch_timeslot_register(wlc->msch_info,
				&scan_info->chanspec_list[chn_start],
				chn_cnt, wlc_scan_chnsw_clbk, wlc_scan_info, &req_param,
				&scan_info->msch_req_hdl)) != BCME_OK) {
				WL_ERROR(("wl%d: %s: MSCH timeslot register failed,"
					" err %d\n", scan_info->unit, __FUNCTION__, err));

				return err;
			}
		}
	}
#endif /* WLRSDB */
	return BCME_OK;
}

static int
_wlc_scan_schd_channels(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	uint32 dwell_time = _wlc_get_next_chan_scan_time(wlc_scan_info);
#ifdef WLRSDB
	wlc_info_t *wlc = SCAN_WLC(scan_info);
#endif

	/*
	* We need to account for invalide dwell times of 0, which could be returned if
	* we're doing an active scan and the caller has specified passive dwell time of 0
	* to avoid scanning quiet channels. In that case, we must reject the current channel
	* and see if we can find another channel that can be scanned.
	*/
	if (dwell_time == SCAN_INVALID_DWELL_TIME) {
		while (++scan_info->channel_idx < scan_info->channel_num) {
			dwell_time = _wlc_get_next_chan_scan_time(wlc_scan_info);
			if (dwell_time != SCAN_INVALID_DWELL_TIME) {
				break;
			}
		}

		if (scan_info->channel_idx >= scan_info->channel_num) {
			return BCME_SCANREJECT;
		}
	}

#ifdef WLRSDB
	if (RSDB_ENAB(wlc->pub) && SCAN_CURRENT_5G_PASSIVE(scan_info)) {
		wlc_info_t *wlc_2g = NULL;
		wlc_info_t *wlc_5g = NULL;
		bool trigger_passive_scan_other_wlc = FALSE;

		wlc_rsdb_get_wlcs(wlc, &wlc_2g, &wlc_5g);

		if (SCAN_PASSIVE_5G_CHANSPEC(scan_info)) {
			/*
			* trying to scan passive channel, check allowed for this wlc or not
			*/
			if (wlc != wlc_5g) {
				if (!wlc_5g->pub->associated) {
					/* if 5GWLC is not associated, proceed */
					goto resume;
				}
				else {
					/*
					* let 5GWLC drive scan request on passive channel
					* for 2GWLC. Mark scan_info with PASSIVE_PSCAN_BLOCK
					*/
					wlc_scan_info->flag |= SCAN_FLAG_PASSIVE_PSCAN_BLOCK;
					return BCME_OK;
				}
			}
			else {
				if (wlc->pub->associated) {
					/*
					* 5GWLC is associated and scanning.
					* Needs to drive 2GWLC for 5G passive scan if applicable
					*/
					scan_info_t *wlc_2g_sinfo = wlc_2g->scan->scan_priv;
					if (SCAN_PASSIVE_5G_CHANSPEC(wlc_2g_sinfo)) {
						/*
						* there is a pending request for passive 5G scan on
						* 2GWLC, trigger it
						*/
						trigger_passive_scan_other_wlc = TRUE;
					}
				}
				/* else 5GWLC unassociated, nothing to do */
			}
		}
		else if ((wlc == wlc_2g)) {
			/*
			* passive scan phase over, henceforth only parallel scan
			* bring up 2G core if it was brought down before
			*/
			wlc_scan_enable_idle_pscan_wlc(wlc_scan_info);
		}
resume:
		if (trigger_passive_scan_other_wlc) {
			return _wlc_scan_schd_channels_complete(wlc_scan_info,
				_wlc_scan_schd_channels_complete, TRUE);
		}
	}
#endif /* WLRSDB */

	return _wlc_scan_schd_channels_complete(wlc_scan_info, NULL, FALSE);
}

/** Called back by the Multichannel (msch) scheduler */
static int
wlc_scan_chnsw_clbk(void* handler_ctxt, wlc_msch_cb_info_t *cb_info)
{
	wlc_scan_info_t *wlc_scan_info = (wlc_scan_info_t *)handler_ctxt;
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_info_t *wlc = SCAN_WLC(scan_info);
	uint32 type = cb_info->type;
	uint32  dummy_tsf_h, start_tsf;
	char chanbuf[CHANSPEC_STR_LEN];
	BCM_REFERENCE(chanbuf);
	if (scan_info->msch_req_hdl == NULL) {
		/* The requeset is been cancelled, ignore the Clbk */
		return BCME_OK;
	}

	do {
		if (type & MSCH_CT_PRE_ONCHAN) {
#ifdef WLRSDB
			if (RSDB_ENAB((SCAN_WLC(scan_info))->pub)) {
				if (wlc_scan_info->flag &
					SCAN_FLAG_PASSIVE_PSCAN_WAKE_PENDING) {
					wlc_scan_info->flag &=
						~SCAN_FLAG_PASSIVE_PSCAN_WAKE_PENDING;
					wlc_scan_enable_idle_pscan_wlc(wlc_scan_info);
				}
			}
#endif
			break;
		}

		if (type & MSCH_CT_ON_CHAN) {
			wlc_txqueue_start(wlc, NULL, cb_info->chanspec, NULL);
		}

		if (type & MSCH_CT_SLOT_START) {
			wlc_msch_onchan_info_t *onchan =
				(wlc_msch_onchan_info_t *)cb_info->type_specific;
			scan_info->cur_scan_chanspec = cb_info->chanspec;
#ifdef STA
			/* TODO: remove this backward dependency */
			wlc_scan_utils_set_chanspec(wlc, scan_info->cur_scan_chanspec);
#endif
			/* delete any pending timer */
			WLC_SCAN_DEL_TIMER(scan_info);

			/* disable CFP & TSF update */
			wlc_mhf(SCAN_WLC(scan_info), MHF2, MHF2_SKIP_CFP_UPDATE,
				MHF2_SKIP_CFP_UPDATE, WLC_BAND_ALL);
			wlc_scan_skip_adjtsf(scan_info, TRUE, NULL, WLC_SKIP_ADJTSF_SCAN,
				WLC_BAND_ALL);

#ifdef WLSCAN_SUMEVT
			if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
				if (scan_info->scan_sum_chan_info->scan_flags &
					SCAN_SUM_CHAN_INFO) {
					wlc_scan_send_evntlog_scan_summary(
						scan_info,
						scan_info->scan_sum_chan_info);
				}
			}
#endif /* WLSCAN_SUMEVT */

			SCAN_BCN_PROMISC(scan_info) =
				wlc_scan_usage_scan(scan_info->scan_pub);
			wlc_scan_mac_bcn_promisc(scan_info);

			SCAN_READ_TSF(scan_info, &start_tsf, &dummy_tsf_h);
			scan_info->start_tsf = start_tsf;

			if (wlc_scan_quiet_chanspec(scan_info, scan_info->cur_scan_chanspec) ||
				!wlc_scan_valid_chanspec_db(scan_info,
				scan_info->cur_scan_chanspec) ||
				(wlc_scan_info->state & SCAN_STATE_PASSIVE)) {
				if(onchan){
					/* PASSIVE SCAN */
					scan_info->state = WLC_SCAN_STATE_LISTEN;
					scan_info->timeslot_id = onchan->timeslot_id;

					WL_SCAN(("wl%d: passive dwell time %d ms, chanspec %s,"
						" tsf %u\n", scan_info->unit,
						CHANNEL_PASSIVE_DWELLTIME(scan_info),
						wf_chspec_ntoa_ex(scan_info->cur_scan_chanspec,
						chanbuf), start_tsf));

					return BCME_OK;
				}
				else{
					WL_ERROR(("%s: Dereferencing NULL pointer onchan (which is a copy of cb_info->type_specific)\n", __FUNCTION__));
					ASSERT(onchan);
					return BCME_ERROR;
				}
			}

			scan_info->state = WLC_SCAN_STATE_START;
			WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
			break;
		}

		if (type & MSCH_CT_SLOT_END) {
			if (scan_info->state != WLC_SCAN_STATE_SEND_PROBE &&
				scan_info->state != WLC_SCAN_STATE_LISTEN) {
				WL_SCAN(("wl%d: %s: wrong scan state %d \n",
					scan_info->unit, __FUNCTION__, scan_info->state));
			}

			scan_info->state = WLC_SCAN_STATE_CHN_SCAN_DONE;
			scan_info->timeslot_id = 0;

			/* scan passes complete for the current channel */
			WL_SCAN(("wl%d: %s: %sscanned channel %d, total responses %d, tsf %u\n",
			         scan_info->unit, __FUNCTION__,
			         ((wlc_scan_quiet_chanspec(scan_info, cb_info->chanspec) &&
			           !(wlc_scan_info->state & SCAN_STATE_RADAR_CLEAR)) ?
			          "passively ":""),
			        CHSPEC_CHANNEL(cb_info->chanspec),
				SCAN_RESULT_MEB(scan_info, count),
			        SCAN_GET_TSF_TIMERLOW(scan_info)));

			/* reset the radar clear flag since we will be leaving the channel */
			wlc_scan_info->state &= ~SCAN_STATE_RADAR_CLEAR;
#ifdef WLSCAN_SUMEVT
			if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
				if (scan_info->scan_sum_chan_info->scan_flags &
					SCAN_SUM_CHAN_INFO) {
					wlc_scan_send_evntlog_scan_summary(scan_info,
						scan_info->scan_sum_chan_info);
				}
			}
#endif /* WLSCAN_SUMEVT */


			/* resume normal tx queue operation */
			wlc_scan_excursion_end(scan_info);
#ifdef WLSCAN_SUMEVT
			if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {

				/* Home channel specific statistics part of scan
				* summary to be populated
				*/
				wlc_scan_summary_chan_stats(scan_info,
					SCAN_HOME_CHANNEL(scan_info), TRUE);
			}
#endif /* WLSCAN_SUMEVT */
		}

		if (type & MSCH_CT_REQ_END) {
			/* delete any pending timer */
			WLC_SCAN_DEL_TIMER(scan_info);

			/* clear msch request handle */
			scan_info->msch_req_hdl = NULL;

			if (scan_info->channel_idx < scan_info->channel_num) {
#ifdef WLRSDB
				if (RSDB_ENAB(wlc->pub) &&
					SCAN_CURRENT_5G_PASSIVE(scan_info)) {
					wlc_info_t *wlc_2g = NULL;
					wlc_info_t *wlc_5g = NULL;
					wlc_rsdb_get_wlcs(wlc, &wlc_2g, &wlc_5g);
					/*
					* interleaved parallel passive scan kicks in only when
					* 5G wlc is associated.
					*/
					if (wlc_5g->pub->associated &&
						SCAN_PASSIVE_5G_CHANSPEC(scan_info)) {
						wlc_scan_handle_req_end_for_pscan(wlc);
						return BCME_OK;
					}
				}
#endif /* WLRSDB */
				scan_info->state = WLC_SCAN_STATE_PARTIAL;
				WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
			}
			else {
				scan_info->state = WLC_SCAN_STATE_COMPLETE;
				WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
			}
			break;
		}
	} while (0);

	return BCME_OK;
}

void
wlc_scan_timer_update(wlc_scan_info_t *wlc_scan_info, uint32 ms)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;

	WLC_SCAN_DEL_TIMER(scan_info);
	WLC_SCAN_ADD_TIMER(scan_info, ms, 0);
}

/* return number of channels in current scan */
int
wlc_scan_chnum(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_priv = wlc_scan_info->scan_priv;
#ifdef WLRSDB
	if (RSDB_PARALLEL_SCAN_ON(scan_priv)) {
		int idx;
		wlc_info_t *wlc;
		int tot_channels = 0;

		FOREACH_WLC(SCAN_WLC(scan_priv)->cmn, idx, wlc) {
			wlc_scan_info = wlc->scan;
			if (SCAN_IN_PROGRESS(wlc_scan_info)) {
				tot_channels +=
					((scan_info_t *)wlc_scan_info->scan_priv)->channel_num;
			}
		}
		return tot_channels;
	} else
#endif /* WLRSDB */

	return scan_priv->channel_num;
}

#if defined(STA)
uint32
wlc_curr_roam_scan_time(wlc_info_t *wlc)
{
	uint32 curr_roam_scan_time = 0;
	/* Calculate awake time due to active roam scan */
	if (wlc_scan_inprog(wlc) && (AS_IN_PROGRESS(wlc))) {
		wlc_bsscfg_t *cfg = AS_IN_PROGRESS_CFG(wlc);
		wlc_assoc_t *as = cfg->assoc;
		if (as->type == AS_ROAM)
			curr_roam_scan_time = wlc_get_curr_scan_time(wlc);
	}
	return curr_roam_scan_time;
}

uint32
wlc_get_curr_scan_time(wlc_info_t *wlc)
{
	scan_info_t *scan_info = wlc->scan->scan_priv;
	wlc_scan_info_t *scan_pub = scan_info->scan_pub;
	if (scan_pub->in_progress)
		return (OSL_SYSUPTIME() - scan_pub->wlc_scan_cmn->scan_start_time);
	return 0;
}

static void
wlc_scan_time_upd(wlc_info_t *wlc, scan_info_t *scan_info)
{
	wlc_scan_info_t *scan_pub = scan_info->scan_pub;
	uint32 scan_dur;

	scan_dur = wlc_get_curr_scan_time(wlc);

	/* Record scan end */
	scan_pub->wlc_scan_cmn->scan_stop_time = OSL_SYSUPTIME();

	/* For power stats, accumulate duration in the appropriate bucket */
	if (PWRSTATS_ENAB(wlc->pub)) {
		scan_data_t *scan_data = NULL;

		/* Accumlate this scan in the appropriate pwr_stats bucket */
		if (FALSE);
#ifdef WLPFN
#ifdef WL_EXCESS_PMWAKE
		else if (WLPFN_ENAB(wlc->pub) && wl_pfn_scan_in_progress(wlc->pfn)) {

			wlc_excess_pm_wake_info_t *epmwake = wlc->excess_pmwake;
			scan_data = &scan_info->scan_stats->pno_scans[0];

			epmwake->pfn_scan_ms += scan_dur;

			if (epmwake->pfn_alert_enable && ((epmwake->pfn_scan_ms -
				epmwake->pfn_alert_thresh_ts) >	epmwake->pfn_alert_thresh)) {
				wlc_generate_pm_alert_event(wlc, PFN_ALERT_THRESH_EXCEEDED,
					NULL, 0);
				/* Disable further events */
				epmwake->pfn_alert_enable = FALSE;
			}
		}
#endif /* WL_EXCESS_PMWAKE */
#endif /* WLPFN */
		else if (AS_IN_PROGRESS(wlc)) {
			wlc_bsscfg_t *cfg = AS_IN_PROGRESS_CFG(wlc);
			wlc_assoc_t *as = cfg->assoc;

			if (as->type == AS_ROAM) {
				scan_data = &scan_info->scan_stats->roam_scans;
#ifdef WL_EXCESS_PMWAKE
				wlc->excess_pmwake->roam_ms += scan_dur;
				wlc_check_roam_alert_thresh(wlc);
#endif /* WL_EXCESS_PMWAKE */
			}
			else
				scan_data = &scan_info->scan_stats->assoc_scans;
		}
		else if (scan_pub->wlc_scan_cmn->usage == SCAN_ENGINE_USAGE_NORM ||
			scan_pub->wlc_scan_cmn->usage == SCAN_ENGINE_USAGE_ESCAN)
			scan_data = &scan_info->scan_stats->user_scans;
		else
			scan_data = &scan_info->scan_stats->other_scans;

		if (scan_data) {
			scan_data->count++;
			/* roam scan in ms */
			if (scan_data == &scan_info->scan_stats->roam_scans)
				scan_data->dur += scan_dur;
			else
				scan_data->dur += (scan_dur * 1000); /* Scale to usec */
		}
	}

	return;
}

int
wlc_pwrstats_get_scan(wlc_scan_info_t *scan, uint8 *destptr, int destlen)
{
	scan_info_t *scani = (scan_info_t *)scan->scan_priv;
	wl_pwr_scan_stats_t *scan_stats = scani->scan_stats;
	uint16 taglen = sizeof(wl_pwr_scan_stats_t);

	/* Make sure there's room for this section */
	if (destlen < (int)ROUNDUP(sizeof(wl_pwr_scan_stats_t), sizeof(uint32)))
		return BCME_BUFTOOSHORT;

	/* Update common structure fields and copy into the destination */
	scan_stats->type = WL_PWRSTATS_TYPE_SCAN;
	scan_stats->len = taglen;
	memcpy(destptr, scan_stats, taglen);

	/* Report use of this segment (plus padding) */
	return (ROUNDUP(taglen, sizeof(uint32)));
}

#endif /* STA */

/* abort the current scan, and return to home channel */
void
wlc_scan_abort(wlc_scan_info_t *wlc_scan_info, int status)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_bsscfg_t *scan_cfg = scan_info->bsscfg;

#ifdef WLRSDB
	if (RSDB_ENAB((SCAN_WLC(scan_info))->pub)) {
		/*
		* Avoid this call to abort scan if 2G WLC is being brought up/down on purpose
		*/
		if (wlc_scan_info->flag & SCAN_FLAG_PASSIVE_PSCAN_BLOCK) {
			return;
		}
	}
#endif

	BCM_REFERENCE(scan_cfg);

#if defined(WLDFS) && (defined(RSDB_DFS_SCAN) || defined(BGDFS))
	if ((WLRSDBDFS_ENAB(scan_info->wlc->pub) || BGDFS_ENAB(scan_info->wlc->pub)) &&
		wlc_dfs_scan_in_progress(scan_info->wlc->dfs)) {
		wlc_dfs_scan_abort(scan_info->wlc->dfs);
	}
#endif /* WLDFS && (RSDB_DFS_SCAN || BGDFS) */

	if (!SCAN_IN_PROGRESS(wlc_scan_info)) {
#ifdef WLRSDB
		/* Check for the other wlc scan in progress. If so, abort the
		 * other wlc scan..
		 */
		 if (RSDB_PARALLEL_SCAN_ON(scan_info)) {
			 wlc_info_t *otherwlc = wlc_rsdb_get_other_wlc(scan_info->wlc);
			 wlc_scan_info = otherwlc->scan;
			 if (SCAN_IN_PROGRESS(wlc_scan_info)) {
				scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
				BCM_REFERENCE(scan_cfg);
			 } else
				return;
		 } else
#endif /* WLRSDB */
		return;
	}

	WL_INFORM_SCAN(("wl%d: %s: aborting scan in progress\n", scan_info->unit, __FUNCTION__));
#ifdef WLRCC
	if ((WLRCC_ENAB(scan_info->wlc->pub)) && scan_cfg &&
		scan_info->bsscfg->roam &&
		(scan_info->bsscfg->roam->n_rcc_channels > 0))
		scan_info->bsscfg->roam->rcc_valid = TRUE;
#endif
	if (SCANCACHE_ENAB(wlc_scan_info) &&
#ifdef WLP2P
	    (scan_cfg && !BSS_P2P_DISC_ENAB(scan_info->wlc, scan_cfg)) &&
#endif
		TRUE) {
		wlc_scan_cache_result(scan_info);
	}

	wlc_scan_bss_list_free(scan_info);
	wlc_scan_terminate(wlc_scan_info, status);

	wlc_ht_obss_scan_update(scan_info, WLC_E_STATUS_ABORT);

#ifdef EXT_STA
	/* Required for Vista */
	if (IS_EXTSTA_ENAB(scan_info))
		wlc_scan_bss_mac_event(scan_info, scan_cfg, WLC_E_SCAN_COMPLETE, NULL,
			WLC_E_STATUS_ABORT, 0, 0, NULL, 0);
#endif /* EXT_STA */
}

void
wlc_scan_abort_ex(wlc_scan_info_t *wlc_scan_info, wlc_bsscfg_t *cfg, int status)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	if (scan_info->bsscfg == cfg)
		wlc_scan_abort(wlc_scan_info, status);
}

/* wlc_scan_terminate is called when user (from intr/dpc or ioctl) requests to
 * terminate the scan. However it may also be invoked from its own call chain
 * from which tx status processing is executed. Use the flag TERMINATE to prevent
 * it from being re-entered.
 */
/* Driver's primeter lock will prevent wlc_scan_terminate from being invoked from
 * intr/dpc or ioctl when a timer callback is running. However it may be invoked
 * from the wlc_scantimer call chain in which tx status processing is executed.
 * So use the flag IN_TMR_CB to prevent wlc_scan_terminate being re-entered.
 */
void
wlc_scan_terminate(wlc_scan_info_t *wlc_scan_info, int status)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_info_t *wlc = scan_info->wlc;
	int err;

	if (!SCAN_IN_PROGRESS(wlc_scan_info))
		return;

	/* ignore if already in terminate/finish process */
	if (wlc_scan_info->state & SCAN_STATE_TERMINATE) {
		WL_SCAN(("wl%d: %s: ignore wlc_scan_terminate request\n",
		         scan_info->unit, __FUNCTION__));
		return;
	}

	/* protect wlc_scan_terminate being recursively called from the
	 * callchain
	 */

	wlc_scan_info->state |= SCAN_STATE_TERMINATE;

	/* defer the termination if called from the timer callback */
	if (wlc_scan_info->state & SCAN_STATE_IN_TMR_CB) {
		WL_SCAN(("wl%d: %s: defer wlc_scan_terminate request\n",
		         scan_info->unit, __FUNCTION__));
		scan_info->status = status;
		return;
	}

	/* abort the current scan, and return to home channel */
	WL_INFORM_SCAN(("wl%d: %s: terminating scan in progress\n", scan_info->unit, __FUNCTION__));

#ifdef WLSCAN_PS
	/* scan is done. now reset the cores */
	if (WLSCAN_PS_ENAB(wlc->pub))
		wlc_scan_ps_config_cores(scan_info, FALSE);
#endif

#if !defined(WLOFFLD) && !defined(SCANOL) && defined(WLDFS)
	/* If we are switching back to radar home_chanspec
	 * because:
	 * 1. STA scans (normal/Join/Roam) aborted with
	 * atleast one local 11H AP in radar channel,
	 * 2. Scan is not join/roam.
	 * turn radar_detect ON.
	 * NOTE: For Join/Roam radar_detect ON is done
	 * at later point in wlc_roam_complete() or
	 * wlc_set_ssid_complete(), when STA succesfully
	 * associates to upstream AP.
	 */
	if (WLDFS_ENAB(scan_info->wlc->pub) && WL11H_AP_ENAB(wlc) && scan_info->bsscfg != NULL &&
			BSSCFG_AP(scan_info->bsscfg) && scan_info->bsscfg->up &&
			WLC_APSTA_ON_RADAR_CHANNEL(wlc) && wlc->dfs != NULL &&
#ifdef STA
			!AS_IN_PROGRESS(wlc) &&
#endif
			wlc_radar_chanspec(wlc->cmi, wlc->home_chanspec)) {
		WL_REGULATORY(("wl%d: %s Join/scan aborted back"
			"to home channel dfs ON\n",
			wlc->pub->unit, __FUNCTION__));
		wlc_set_dfs_cacstate(wlc->dfs, ON, scan_info->bsscfg);
	}
#endif /* !defined(WLOFFLD) && !defined(SCANOL)) && defined(WLDFS) */

	/* clear scan ready flag */
	wlc_scan_info->state &= ~SCAN_STATE_READY;

	/* Cancel the MSCH request */
	if (scan_info->msch_req_hdl) {
		if ((err = wlc_msch_timeslot_unregister(wlc->msch_info,
			&scan_info->msch_req_hdl)) != BCME_OK) {
			WL_SCAN_ERROR(("wl%d: %s: MSCH timeslot unregister failed, err %d\n",
			         scan_info->unit, __FUNCTION__, err));
		}

		/* resume normal tx queue operation */
		wlc_scan_excursion_end(scan_info);

		scan_info->msch_req_hdl = NULL;
	}

	scan_info->state = WLC_SCAN_STATE_ABORT;
	scan_info->channel_idx = -1;
#if defined(STA)
	wlc_scan_time_upd(wlc, scan_info);
#endif /* STA */
	wlc_scan_info->in_progress = FALSE;

#ifdef WLBTCPROF
	wlc_btcx_set_btc_profile_param(wlc, wlc->home_chanspec, TRUE);
#endif

	wlc_phy_hold_upd(SCAN_GET_PI_PTR(scan_info), PHY_HOLD_FOR_SCAN, FALSE);
#ifdef WLOLPC
	/* notify scan just terminated - if needed, kick off new cal */
	if (OLPC_ENAB(wlc)) {
		wlc_olpc_eng_hdl_chan_update(wlc->olpc_info);
	}
#endif /* WLOLPC */

	wlc_scan_ssidlist_free(scan_info);

#ifdef STA
	wlc_scan_set_wake_ctrl(scan_info);
	WL_MPC(("wl%d: %s: SCAN_IN_PROGRESS==FALSE, update mpc\n", scan_info->unit, __FUNCTION__));
	wlc->mpc_scan = FALSE;
	wlc_scan_radio_mpc_upd(scan_info);
#endif /* STA */

	/* abort PM indication process */
	wlc_scan_info->state &= ~SCAN_STATE_PSPEND;
	/* abort tx suspend delay process */
	wlc_scan_info->state &= ~SCAN_STATE_DLY_WSUSPEND;
	/* abort TX FIFO suspend process */
	wlc_scan_info->state &= ~SCAN_STATE_WSUSPEND;
	/* delete a future timer explictly */
	WLC_SCAN_DEL_TIMER(scan_info);
#ifdef STA
#ifdef WL11D
	/* If we are in 802.11D mode and we are still waiting to find a
	 * valid Country IE, take this opportunity to parse scan results,
	 * if necessary, and try to apply it.
	 */
	if (IS_AUTOCOUNTRY_ENAB(scan_info))
		wlc_scan_11d_scan_complete(scan_info, status);
#endif /* WL11D */
	wlc_scan_callback(scan_info, status);
#endif /* STA */

	scan_info->state = WLC_SCAN_STATE_START;

	wlc_scan_info->state &= ~SCAN_STATE_TERMINATE;
}

static void
wlc_scantimer(void *arg)
{
	scan_info_t *scan_info = (scan_info_t *)arg;
	wlc_scan_info_t	*wlc_scan_info = scan_info->scan_pub;
	wlc_info_t *wlc = scan_info->wlc;
	int status = WLC_E_STATUS_SUCCESS;
	/* char chanbuf[CHANSPEC_STR_LEN]; */

	UNUSED_PARAMETER(wlc);

	WL_SCAN_ENT(scan_info, ("wl%d: %s: enter, state 0x%x tsf %u\n",
	                        scan_info->unit, __FUNCTION__,
	                        wlc_scan_info->state,
	                        SCAN_GET_TSF_TIMERLOW(scan_info)));

	wlc_scan_info->state |= SCAN_STATE_IN_TMR_CB;

	if (SCAN_DEVICEREMOVED(scan_info)) {
		WL_ERROR(("wl%d: %s: dead chip\n", scan_info->unit, __FUNCTION__));
		if (SCAN_IN_PROGRESS(wlc_scan_info)) {
			wlc_scan_bss_list_free(scan_info);
			wlc_scan_callback(scan_info, WLC_E_STATUS_ABORT);

		}
		SCAN_WL_DOWN(scan_info);
		goto exit;
	}

	if (scan_info->state == WLC_SCAN_STATE_PARTIAL) {
		if (_wlc_scan_schd_channels(wlc_scan_info) == BCME_OK) {
#ifdef WLRSDB
			if (RSDB_ENAB(wlc->pub)) {
				wlc_scan_disable_idle_pscan_wlc(scan_info);
			}
#endif
			goto exit;
		}

		WL_SCAN_ERROR(("wl%d: %s: Error, failed to scan %d channels\n",
			scan_info->unit, __FUNCTION__,
			(scan_info->channel_num - scan_info->channel_idx)));

		scan_info->state = WLC_SCAN_STATE_COMPLETE;
	}

	if (scan_info->state == WLC_SCAN_STATE_ABORT) {
		WL_SCAN_ENT(scan_info, ("wl%d: %s: move state to START\n",
		                        scan_info->unit, __FUNCTION__));
		scan_info->state = WLC_SCAN_STATE_START;
		goto exit;
	}

	if (scan_info->state == WLC_SCAN_STATE_START) {
		scan_info->state = WLC_SCAN_STATE_SEND_PROBE;
		scan_info->pass = scan_info->npasses;
#ifdef WL_OCE
		if (OCE_ENAB(wlc->pub)) {
			if (wlc_oce_is_oce_environment(wlc->oce))
				scan_info->defer_probe = TRUE;
			else
				scan_info->defer_probe = FALSE;
		}
#endif
	}

	if (scan_info->state == WLC_SCAN_STATE_SEND_PROBE) {
		if (scan_info->pass > 0) {
#ifdef WL_OCE
			if (OCE_ENAB(wlc->pub) && scan_info->defer_probe == TRUE) {
				scan_info->defer_probe = FALSE;
				uint8 def_to = wlc_oce_get_probe_defer_time(wlc->oce);
				WLC_SCAN_ADD_TIMER(scan_info, def_to, 0);
				goto exit;
			}
#endif /* WL_OCE */
#ifdef WLSCAN_PS
			/* scan started, switch to one tx/rx core */
			if (WLSCAN_PS_ENAB(wlc->pub))
				wlc_scan_ps_config_cores(scan_info, TRUE);
#endif /* WLSCAN_PS */
			wlc_scan_do_pass(scan_info, scan_info->cur_scan_chanspec);
			scan_info->pass--;
#ifdef WL_OCE
		if (OCE_ENAB(wlc->pub))
			scan_info->defer_probe = TRUE;
#endif
			goto exit;
		} else {
			if (scan_info->timeslot_id) {
				if (BCME_OK != wlc_msch_timeslot_cancel(wlc->msch_info,
					scan_info->msch_req_hdl, scan_info->timeslot_id))
					goto exit;

				scan_info->timeslot_id = 0;

				/* reset radar clear flag since we are leaving this channel */
				wlc_scan_info->state &= ~SCAN_STATE_RADAR_CLEAR;

				/* resume normal tx queue operation */
				wlc_scan_excursion_end(scan_info);

				if (scan_info->chanspec_list[scan_info->channel_idx - 1] !=
					scan_info->cur_scan_chanspec) {
					/* scan complete for current channel */
					scan_info->state = WLC_SCAN_STATE_CHN_SCAN_DONE;
				} else {
					/* scan complete for current request */
					/* clear msch request handle */
					scan_info->msch_req_hdl = NULL;

					if (scan_info->channel_idx < scan_info->channel_num) {
						scan_info->state = WLC_SCAN_STATE_PARTIAL;
						WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
					}
					else {
						scan_info->state = WLC_SCAN_STATE_COMPLETE;
						WLC_SCAN_ADD_TIMER(scan_info, 0, 0);
					}
					goto exit;
				}
			}
		}
	}

	if (scan_info->state != WLC_SCAN_STATE_COMPLETE) {
		goto exit;
	}

	scan_info->state = WLC_SCAN_STATE_START;
	wlc_scan_info->state |= SCAN_STATE_TERMINATE;

#if defined(BCMDBG) || defined(WLMSG_INFORM)
	if (scan_info->nssid == 1) {
		char ssidbuf[SSID_FMT_BUF_LEN];
		wlc_ssid_t *ssid = scan_info->ssid_list;

		if (WL_INFORM_ON())
			wlc_format_ssid(ssidbuf, ssid->SSID, ssid->SSID_len);
		WL_INFORM_SCAN(("wl%d: %s: %s scan done, %d total responses for SSID \"%s\"\n",
		           scan_info->unit, __FUNCTION__,
		           (wlc_scan_info->state & SCAN_STATE_PASSIVE) ? "Passive" : "Active",
		           SCAN_RESULT_MEB(scan_info, count), ssidbuf));
	} else {
		WL_INFORM_SCAN(("wl%d: %s: %s scan done, %d total responses for SSIDs:\n",
		           scan_info->unit, __FUNCTION__,
		           (wlc_scan_info->state & SCAN_STATE_PASSIVE) ? "Passive" : "Active",
		           SCAN_RESULT_MEB(scan_info, count)));
		if (WL_INFORM_ON())
			wlc_scan_print_ssids(scan_info->ssid_list, scan_info->nssid);
	}
#endif /* BCMDBG || WLMSG_INFORM */

#ifdef WLSCAN_PS
	/* scan is done, revert core mask */
	if (WLSCAN_PS_ENAB(wlc->pub))
		wlc_scan_ps_config_cores(scan_info, FALSE);
#endif

	wlc_scan_info->state &= ~SCAN_STATE_READY;
	scan_info->channel_idx = -1;
#if defined(STA)
	wlc_scan_time_upd(wlc, scan_info);
#endif /* STA */
	wlc_scan_info->in_progress = FALSE;
#ifdef WLBTCPROF
	wlc_btcx_set_btc_profile_param(wlc, wlc->home_chanspec, TRUE);
#endif
	wlc_phy_hold_upd(SCAN_GET_PI_PTR(scan_info), PHY_HOLD_FOR_SCAN, FALSE);
#ifdef WLOLPC
	/* notify scan just terminated - if needed, kick off new cal */
	if (OLPC_ENAB(wlc)) {
		wlc_olpc_eng_hdl_chan_update(wlc->olpc_info);
	}
#endif /* WLOLPC */
#ifdef WLMESH
	/* prepare peering list */
	if (WLMESH_ENAB(wlc->pub) && BSSCFG_MESH(scan_info->bsscfg))
		wlc_mpeer_parse_scan_info(wlc->mesh_info, scan_info->bsscfg,
			SCAN_RESULT_PTR(scan_info));
#endif /* WLMESH */

	wlc_scan_ssidlist_free(scan_info);

	/* allow core to sleep again (no more solicited probe responses) */
	wlc_scan_set_wake_ctrl(scan_info);

#ifdef WLCQ
	/* resume any channel quality measurement */
	wlc_lq_channel_qa_sample_req(wlc);
#endif /* WLCQ */

#ifdef STA
	/* disable radio for non-association scan.
	 * Association scan will continue with JOIN process and
	 * end up at MACEVENT: WLC_E_SET_SSID
	 */
	WL_MPC(("wl%d: scan done, SCAN_IN_PROGRESS==FALSE, update mpc\n", scan_info->unit));
	wlc->mpc_scan = FALSE;
	wlc_scan_radio_mpc_upd(scan_info);
	wlc_scan_radio_upd(scan_info);	/* Bring down the radio immediately */
#endif /* STA */
	wlc_scan_callback(scan_info, status);

	wlc_scan_info->state &= ~SCAN_STATE_TERMINATE;

exit:
	wlc_scan_info->state &= ~SCAN_STATE_IN_TMR_CB;

	/* You can't read hardware registers, tsf in this case, if you don't have
	 * clocks enabled. e.g. you are down. The exit path of this function will
	 * result in a down'ed driver if we are just completing a scan and MPC is
	 * enabled and meets the conditions to down the driver (typically no association).
	 * While typically MPC down is deferred for a time delay, the call of the
	 * wlc_scan_mpc_upd() in the scan completion path will force an immediate down
	 * before we get to the exit of this function.  For this reason, we condition
	 * the read of the tsf timer in the debugging output on the presence of
	 * hardware clocks.
	 */
	WL_SCAN_ENT(scan_info, ("wl%d: %s: exit, state 0x%x tsf %u\n",
	                        scan_info->unit, __FUNCTION__,
	                        wlc_scan_info->state,
	                        SCAN_GET_TSF_TIMERLOW(scan_info)));
}

static void
wlc_scan_act(scan_info_t *si, uint dwell)
{
	wlc_info_t *wlc = si->wlc;
#ifdef BCMDBG
	uint saved = dwell;
#endif
	/* real scan request */
	if (si->act_cb == NULL) {
		wlc_scan_sendprobe(si);
		goto set_timer;
	}

	/* other requests using the scan engine */
	(si->act_cb)(wlc, si->act_cb_arg, &dwell);

#ifdef BCMDBG
	if (dwell != saved) {
		WL_SCAN(("wl%d: %s: adjusting dwell time from %u to %u ms\n",
		         si->unit, __FUNCTION__, saved, dwell));
	}
#endif

set_timer:
	WLC_SCAN_ADD_TIMER(si, dwell, 0);
}

void
wlc_scan_radar_clear(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t	*scan_info = (scan_info_t *) wlc_scan_info->scan_priv;
	uint32		channel_dwelltime;
	uint32 cur_l, cur_h;
	uint32 elapsed_time, remaining_time, active_time;
	char chanbuf[CHANSPEC_STR_LEN];
	BCM_REFERENCE(chanbuf);

	/* if we are not on a radar quiet channel,
	 * or a passive scan was requested,
	 * or we already processed the radar clear signal,
	 * or it is not a prohibited channel,
	 * then do nothing
	 */
	if ((wlc_scan_valid_chanspec_db(scan_info, SCAN_BAND_PI_RADIO_CHANSPEC(scan_info)) &&
	     !wlc_scan_quiet_chanspec(scan_info, SCAN_BAND_PI_RADIO_CHANSPEC(scan_info))) ||
	    (wlc_scan_info->state & (SCAN_STATE_PASSIVE | SCAN_STATE_RADAR_CLEAR)))
		return;

	/* if we are not in the channel scan portion of the scan, do nothing */
	if (scan_info->state != WLC_SCAN_STATE_LISTEN)
		return;

	/* if there is not enough time remaining for a probe,
	 * do nothing unless explicitly enabled
	 */
	if (scan_info->scan_cmn->force_active == FALSE) {
		SCAN_READ_TSF(scan_info, &cur_l, &cur_h);

		elapsed_time = (cur_l - scan_info->start_tsf) / 1000;

		channel_dwelltime = CHANNEL_PASSIVE_DWELLTIME(scan_info);

		if (elapsed_time > channel_dwelltime)
			remaining_time = 0;
		else
			remaining_time = channel_dwelltime - elapsed_time;

		if (remaining_time < WLC_SCAN_MIN_PROBE_TIME)
			return;

		if (remaining_time >= CHANNEL_ACTIVE_DWELLTIME(scan_info)) {
			active_time = CHANNEL_ACTIVE_DWELLTIME(scan_info)/scan_info->npasses;
			/* if remaining time is MSCH_CANCELSLOT_PREPARE or more than
			 * active dwell time, cancel current time slot to save
			 * after probing for active dwell time. Otherwise do not canel.
			 */
			channel_dwelltime = CHANNEL_ACTIVE_DWELLTIME(scan_info);
			if (remaining_time < channel_dwelltime + MSCH_CANCELSLOT_PREPARE)
				scan_info->timeslot_id = 0;
		} else {
			active_time = remaining_time;
			scan_info->timeslot_id = 0;
		}
	}
	else {
		active_time = CHANNEL_ACTIVE_DWELLTIME(scan_info);
		scan_info->timeslot_id = 0;
	}

	scan_info->pass = scan_info->npasses;
	if (scan_info->pass > 0) {
		/* everything is ok to switch to an active scan */
		wlc_scan_info->state |= SCAN_STATE_RADAR_CLEAR;
		scan_info->state = WLC_SCAN_STATE_SEND_PROBE;

		WLC_SCAN_DEL_TIMER(scan_info);

		SCAN_TO_MUTE(scan_info, OFF, 0);
#ifdef WLSCAN_SUMEVT
		/* Populate channel specific statistics for radar channels
		 * that undergo active scanning
		 */
		if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
			if (scan_info->scan_sum_chan_info->scan_flags &
				SCAN_SUM_CHAN_INFO) {
				wlc_scan_send_evntlog_scan_summary(scan_info,
					scan_info->scan_sum_chan_info);
			}
			wlc_scan_summary_chan_stats(scan_info,
				SCAN_WLC(scan_info)->chanspec, FALSE);
		}
#endif /* WLSCAN_SUMEVT */

		wlc_scan_act(scan_info, active_time);
		scan_info->pass--;

		WL_REGULATORY(("wl%d: wlc_scan_radar_clear: rcvd beacon on radar chanspec %s,"
			" converting to active scan, %d ms left\n", scan_info->unit,
			wf_chspec_ntoa_ex(SCAN_BAND_PI_RADIO_CHANSPEC(scan_info), chanbuf),
			active_time));
	}
}

#ifdef BCMDBG
static void
print_valid_channel_error(scan_info_t *scan_info, chanspec_t chspec)
{
	uint8 channel = CHSPEC_CHANNEL(chspec);
	uint bandunit = CHSPEC_WLCBANDUNIT(chspec);
	char chanbuf[CHANSPEC_STR_LEN];
	wlc_info_t *wlc = scan_info->wlc;
	BCM_REFERENCE(chanbuf);

	WL_PRINT(("chspec=%s\n", wf_chspec_ntoa_ex(chspec, chanbuf)));

	if (CHANNEL_BANDUNIT(wlc, CHSPEC_CHANNEL(chspec)) != bandunit) {
			WL_PRINT(("CHANNEL_BANDUNIT(wlc, CHSPEC_CHANNEL(chspec))=%x\n",
			CHANNEL_BANDUNIT(wlc, CHSPEC_CHANNEL(chspec))));
		return;
	}

	/* Check a 20Mhz channel -- always assumed to be dual-band */
	if (CHSPEC_IS20(chspec)) {
		if (!VALID_CHANNEL20_DB(wlc, chspec)) {
			WL_PRINT(("VALID_CHANNEL20_DB = %d\n",
			VALID_CHANNEL20_DB(wlc, chspec)));
		} else {
			WL_PRINT(("%s: no error found\n", __FUNCTION__));
		}
		return;
	} else if (CHSPEC_IS40(chspec)) {
		/* Check a 40Mhz channel */
		if (!wlc->pub->phy_bw40_capable) {
			WL_PRINT(("phy not bw40 capable\n"));
			return;
		}

		if (!VALID_40CHANSPEC_IN_BAND(wlc, CHSPEC_WLCBANDUNIT(chspec))) {
			WL_PRINT(("!VALID_40CHANSPEC_IN_BAND(%p, %d)\n",
				OSL_OBFUSCATE_BUF(wlc), chspec));
			return;
		}
		if (!VALID_CHANNEL20_DB(wlc, LOWER_20_SB(channel)) ||
			!VALID_CHANNEL20_DB(wlc, UPPER_20_SB(channel))) {
			WL_PRINT(("dual bands not both valid = [%x, %x]\n",
				LOWER_20_SB(channel), UPPER_20_SB(channel)));
			return;
		}

		/* check that the lower sideband allows an upper sideband */
			WL_PRINT(("%s: lower sideband not allow upper one"
					"OR error not found\n",	__FUNCTION__));

	} else if (CHSPEC_IS80(chspec)) {
		/* Check a 80MHz channel - only 5G band supports 80MHz */

		chanspec_t chspec40;

		/* Only 5G supports 80MHz
		 * Check the chanspec band with BAND_5G() instead of the more straightforward
		 * CHSPEC_IS5G() since BAND_5G() is conditionally compiled on BAND5G support. This
		 * check will turn into a constant check when compiling without BAND5G support.
		 */
		if (!BAND_5G(CHSPEC2WLC_BAND(chspec))) {
			WL_PRINT(("band not 5g for 80MHz\n"));
			return;
		}

		/* Make sure that the phy is 80MHz capable and that
		 * we are configured for 80MHz on the band
		 */
		if (!wlc->pub->phy_bw80_capable ||
		    !WL_BW_CAP_80MHZ(SCAN_GET_BANDSTATE(scan_info, BAND_5G_INDEX)->bw_cap)) {
			WL_PRINT(("!phy_bw80_capable (%x) || !mimo_cap_80 (%x)\n",
				!wlc->pub->phy_bw80_capable,
				!WL_BW_CAP_80MHZ(SCAN_GET_BANDSTATE(scan_info,
				BAND_5G_INDEX)->bw_cap)));
			return;
		}
		/* Check that the 80MHz center channel is a defined channel */
		/* Make sure both 40 MHz side channels are valid
		 * Create a chanspec for each 40MHz side side band and check
		 */
		chspec40 = (chanspec_t)((channel - CH_20MHZ_APART) |
			WL_CHANSPEC_CTL_SB_L |
			WL_CHANSPEC_BW_40 |
			WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));

			return;
		}
		chspec40 = (chanspec_t)((channel + CH_20MHZ_APART) |
			WL_CHANSPEC_CTL_SB_L |
			WL_CHANSPEC_BW_40 |
			WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));
			return;
		}
		WL_PRINT(("%s: err not found or 80MHz has no"
				"channel %d\n", __FUNCTION__, channel));
		return;
	}
	else if (CHSPEC_IS8080(chspec) || CHSPEC_IS160(chspec)) {
		chanspec_t chspec40;

		/* Only 5G supports 80+80/160 MHz
		 * Check the chanspec band with BAND_5G() instead of the more straightforward
		 * CHSPEC_IS5G() since BAND_5G() is conditionally compiled on BAND5G support. This
		 * check will turn into a constant check when compiling without BAND5G support.
		 */
		if (!BAND_5G(CHSPEC2WLC_BAND(chspec))) {
			WL_PRINT(("band not 5g for 80+80/160 MHz\n"));
			return;
		}

		/* Make sure that the phy is 80MHz capable and that
		 * we are configured for 80MHz on the band
		 */
		if (!wlc->pub->phy_bw8080_capable || !wlc->pub->phy_bw160_capable ||
		    !WL_BW_CAP_160MHZ(SCAN_GET_BANDSTATE(scan_info, BAND_5G_INDEX)->bw_cap)) {
			WL_PRINT(("!phy_bw8080_capable (%x) || !phy_bw8080_capable (%x) ||"
				"!mimo_cap_160 (%x)\n",
				!wlc->pub->phy_bw8080_capable,
				!wlc->pub->phy_bw160_capable,
				!WL_BW_CAP_160MHZ(SCAN_GET_BANDSTATE(scan_info,
				BAND_5G_INDEX)->bw_cap)));
			return;
		}

		/* Check whether primary 80 channel is valid */
		channel = wf_chspec_primary80_channel(chspec);
		chspec40 = (chanspec_t)((channel - CH_20MHZ_APART) |
			WL_CHANSPEC_CTL_SB_L |
			WL_CHANSPEC_BW_40 |
			WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));

			return;
		}
		chspec40 = (chanspec_t)((channel + CH_20MHZ_APART) |
			WL_CHANSPEC_CTL_SB_L |
			WL_CHANSPEC_BW_40 |
			WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));
			return;
		}

		/* Check whether secondary 80 channel is valid */
		channel = wf_chspec_secondary80_channel(chspec);
		chspec40 = (chanspec_t)((channel - CH_20MHZ_APART) |
		WL_CHANSPEC_CTL_SB_L |
		WL_CHANSPEC_BW_40 |
		WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));
			return;
		}
		chspec40 = (chanspec_t)((channel + CH_20MHZ_APART) |
			WL_CHANSPEC_CTL_SB_L |
			WL_CHANSPEC_BW_40 |
			WL_CHANSPEC_BAND_5G);

		if (!wlc_scan_valid_chanspec_db(scan_info, chspec40)) {
			WL_PRINT(("wl%d: %s: 80MHz: chanspec %0X -> chspec40 %0X "
					"failed valid check\n",
					scan_info->unit, __FUNCTION__, chspec, chspec40));
			return;
		}
	}
}
#endif /* BCMDBG */

/*
 * Returns default channels for this locale in band 'band'
 */
void
wlc_scan_default_channels(wlc_scan_info_t *wlc_scan_info, chanspec_t chanspec_start,
int band, chanspec_t *chanspec_list, int *channel_count)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	int num;

#ifdef BCMDBG
	if (!wlc_scan_valid_chanspec_db(scan_info, chanspec_start)) {
		WL_SCAN_ERROR(("wlc_scan_valid_chanspec_db(%p, %x)==FALSE\n",
			OSL_OBFUSCATE_BUF(SCAN_CMIPTR(scan_info)), chanspec_start));
		print_valid_channel_error(scan_info, chanspec_start);
	}
#endif /* BCMDBG */
	ASSERT(wlc_scan_valid_chanspec_db(scan_info, chanspec_start));

	/* enumerate all the active (non-quiet) channels first */
	wlc_scan_channels(scan_info, chanspec_list, &num, MAXCHANNEL,
		chanspec_start, CHAN_TYPE_CHATTY, band);
	*channel_count = num;

	/* if scan_info->passive_time = 0, skip the passive channels */
	if (!scan_info->passive_time)
		return;

	/* enumerate all the passive (quiet) channels second */
	wlc_scan_channels(scan_info, &chanspec_list[num], &num,
		(MAXCHANNEL - *channel_count), chanspec_start, CHAN_TYPE_QUIET, band);
	*channel_count += num;
}

/*
 * Scan channels are always 2.5/5/10/20MHZ, so return the valid set of 2.5/5/10/2020MHZ channels
 * for this locale. This function will return the channels available in the band of argument
 * 'band' band can be WLC_BAND_ALL WLC_BAND_2G or WLC_BAND_5G
 */
static void
wlc_scan_channels(scan_info_t *scan_info, chanspec_t *chanspec_list,
	int *pchannel_num, int channel_max, chanspec_t chanspec_start, int channel_type,
	int band)
{
	uint bandunit;
	uint channel;
	chanspec_t chanspec;
	int num = 0;
	uint i;

#ifdef SLAVE_RADAR
	wlc_info_t *wlc = scan_info->wlc;
#endif
	/* chanspec start should be for a 2.5/5/10/20MHZ channel */
	ASSERT(CHSPEC_ISLE20(chanspec_start));
	bandunit = CHSPEC_WLCBANDUNIT(chanspec_start);
	for (i = 0; i < SCAN_NBANDS(scan_info); i++) {
		channel = CHSPEC_CHANNEL(chanspec_start);
		chanspec = CH20MHZ_CHSPEC(channel);

#ifdef WL_OCE
		if (OCE_ENAB(scan_info->wlc->pub) && bandunit == BAND_2G_INDEX) {
			/* populate chanspec list with OCE channels first */
			if (channel_type != CHAN_TYPE_QUIET)
				num += wlc_oce_get_pref_channels(chanspec_list + num);
		}
#endif /* WL_OCE */

		while (num < channel_max) {
			if (SCAN_VALID_CHANNEL20_IN_BAND(scan_info, bandunit,
				channel) &&
			    ((channel_type == CHAN_TYPE_CHATTY &&
				!wlc_scan_quiet_chanspec(scan_info, chanspec)) ||
			     (channel_type == CHAN_TYPE_QUIET &&
				wlc_scan_quiet_chanspec(scan_info, chanspec) &&
#ifdef SLAVE_RADAR
				/*
				 * If radar was detected on this chanspec and Non Occupancy
				 * period is not yet over, then exclude this chanspec from
				 * scan.
				 */
				wlc_dfs_valid_ap_chanspec(wlc, chanspec) &&
#endif
				TRUE))) {
#ifdef WL_OCE
				/* skip adding OCE channels to the list since its already done */
				if (!(OCE_ENAB(scan_info->wlc->pub) &&
					bandunit == BAND_2G_INDEX &&
					wlc_oce_is_pref_channel(chanspec)))
#endif /* WL_OCE */
					chanspec_list[num++] = chanspec;
			}

			channel = (channel + 1) % MAXCHANNEL;
			chanspec = CH20MHZ_CHSPEC(channel);
			if (wf_chspec_ctlchan(chanspec) == wf_chspec_ctlchan(chanspec_start))
				break;
		}

		/* only find channels for one band */
		if (!SCAN_IS_MBAND_UNLOCKED(scan_info))
			break;
		if (band == WLC_BAND_ALL) {
			/* prepare to find the other band's channels */
			bandunit = ((bandunit == 1) ? 0 : 1);
			chanspec_start = CH20MHZ_CHSPEC(0);
		} else
			/* We are done with current band. */
			break;
	}

	*pchannel_num = num;
}

static uint
wlc_scan_prohibited_channels(scan_info_t *scan_info, chanspec_t *chanspec_list,
	int channel_max)
{
	WLC_BAND_T *band;
	uint channel, maxchannel, i, j;
	chanvec_t sup_chanvec, chanvec;
	int num = 0;

	band = SCAN_GET_CUR_BAND(scan_info);
	for (i = 0; i < SCAN_NBANDS(scan_info); i++) {
		const char *acdef = wlc_scan_11d_get_autocountry_default(scan_info);

		bzero(&sup_chanvec, sizeof(chanvec_t));
		/* Get the list of all the channels in autocountry_default
		 * and supported by phy
		 */
		phy_utils_chanspec_band_validch(
			(phy_info_t *)SCAN_GET_PI_BANDUNIT(scan_info, band->bandunit),
			band->bandtype, &sup_chanvec);
		if (!wlc_scan_get_chanvec(scan_info, acdef, band->bandtype, &chanvec))
			return 0;

		for (j = 0; j < sizeof(chanvec_t); j++)
			sup_chanvec.vec[j] &= chanvec.vec[j];

		maxchannel = BAND_2G(band->bandtype) ? (CH_MAX_2G_CHANNEL + 1) : MAXCHANNEL;
		for (channel = 0; channel < maxchannel; channel++) {
			if (isset(sup_chanvec.vec, channel) &&
				!SCAN_VALID_CHANNEL20_IN_BAND(scan_info, band->bandunit,
					channel)) {
				chanspec_list[num++] = CH20MHZ_CHSPEC(channel);
				if (num >= channel_max)
					return num;
			}
		}
		band = SCAN_GET_BANDSTATE(scan_info, SCAN_OTHERBANDUNIT(scan_info));
	}

	return num;
}

#ifdef CNTRY_DEFAULT
int
wlc_scan_prohibited_channels_get(wlc_scan_info_t *scan, chanspec_t *chanspec_list,
	int channel_max)
{
	scan_info_t *scan_info = (scan_info_t *)scan->scan_priv;
	return (int)wlc_scan_prohibited_channels(scan_info, chanspec_list, channel_max);
}
#endif /* CNTRY_DEFAULT */

bool
wlc_scan_inprog(wlc_info_t *wlc_info)
{
	return SCAN_IN_PROGRESS(wlc_info->scan);
}

static int
wlc_scan_apply_scanresults(scan_info_t *scan_info, int status)
{
#ifdef STA
	int idx;
	wlc_bsscfg_t *cfg;
#endif /* STA */
	wlc_bsscfg_t *scan_cfg;
	wlc_scan_info_t *wlc_scan_info;
	wlc_info_t *wlc = scan_info->wlc;
	BCM_REFERENCE(status);

	(void)wlc_scan_info;

	/* Store for later use */
	scan_cfg = scan_info->bsscfg;

	UNUSED_PARAMETER(wlc);
	UNUSED_PARAMETER(scan_cfg);

#ifdef WLRSDB
	/* Move scan results to scan request wlc in case of parallel scanning. */
	if (RSDB_PARALLEL_SCAN_ON(scan_info)) {
		scan_info->scan_cmn->num_of_cbs--;
		WL_SCAN(("wl%d.%d:%s Num of CBs pending:%d\n", wlc->pub->unit,
		         scan_cfg->_idx, __FUNCTION__, scan_info->scan_cmn->num_of_cbs));
		/* TODO: remove this backward dependency */
		wlc_scan_utils_scan_complete(wlc, scan_cfg);
		if (scan_info->scan_cmn->num_of_cbs != 0) {
			return BCME_BUSY;
		}
		/* Need to pickup the wlc & wlc_scan_info where the scan request
		 * is given to use scan results below.
		 */
		scan_info = (scan_info_t*)wlc->scan->scan_priv;
	}
#endif /* WLRSDB */

	wlc_scan_info = scan_info->scan_pub;

#ifdef STA
#ifdef WL11D
	/* If we are in 802.11D mode and we are still waiting to find a
	 * valid Country IE, then take this opportunity to parse these
	 * scan results for one.
	 */
	if (IS_AUTOCOUNTRY_ENAB(scan_info))
		wlc_scan_11d_scan_complete(scan_info, WLC_E_STATUS_SUCCESS);
#endif /* WL11D */
#endif /* STA */

	wlc_ht_obss_scan_update(scan_info, WLC_E_STATUS_SUCCESS);

	/* Don't fill the cache with results from a P2P discovery scan since these entries
	 * are short-lived. Also, a P2P association cannot use Scan cache
	 */
	if (SCANCACHE_ENAB(wlc_scan_info) &&
#ifdef WLP2P
	    !BSS_P2P_DISC_ENAB(wlc, scan_cfg) &&
#endif
	    TRUE) {
		wlc_scan_cache_result(scan_info);
	}

#ifdef STA
	/* if this was a broadcast scan across all channels,
	 * update the roam cache, if possible
	 */
	if (ETHER_ISBCAST(&wlc_scan_info->bssid) &&
	    wlc_scan_info->wlc_scan_cmn->bss_type == DOT11_BSSTYPE_ANY) {
		SCAN_FOREACH_AS_STA(scan_info, idx, cfg) {
			wlc_roam_t *roam = cfg->roam;
			if (roam && roam->roam_scan_piggyback &&
			    roam->active && !roam->fullscan_count) {
				WL_ASSOC(("wl%d: %s: Building roam cache with"
				          " scan results from broadcast scan\n",
				          scan_info->unit, __FUNCTION__));
				/* this counts as a full scan */
				roam->fullscan_count = 1;
				/* update the roam cache */
				wlc_build_roam_cache(cfg, SCAN_RESULT_PTR(scan_info));
			}
		}
	}
#endif /* STA */


	return BCME_OK;
}

static void
wlc_scan_callback(scan_info_t *scan_info, uint status)
{
	scancb_fn_t cb = scan_info->cb;
	void *cb_arg = scan_info->cb_arg;
	wlc_bsscfg_t *cfg = scan_info->bsscfg;
	wlc_info_t *wlc = scan_info->wlc;

	int scan_completed;
#ifdef WLPFN
	bool is_pfn_in_progress;
	/* Check the state before cb is called as the state
	 * changes after cb
	 */
	is_pfn_in_progress = WLPFN_ENAB(wlc->pub) &&
	                     wl_pfn_scan_in_progress(wlc->pfn);
#endif /* WLPFN */

	scan_completed = wlc_scan_apply_scanresults(scan_info, status);

	scan_info->bsscfg = NULL;
	scan_info->cb = NULL;
	scan_info->cb_arg = NULL;
#ifdef WLSCAN_SUMEVT
	if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
		if (scan_completed == BCME_OK) {
			if (status == WLC_E_STATUS_ABORT) {
				scan_info->scan_cmn->scn_summary->scan_flags |=
					SCAN_ABORT;
				memset(scan_info->scan_sum_chan_info,
					0, sizeof(wl_scan_summary_t));
			}
			wlc_scan_send_evntlog_scan_summary(scan_info,
				scan_info->scan_cmn->scn_summary);
		}
	}
#endif /* WLSCAN_SUMEVT */

	/* Registered Scan callback function should take care of
	 * sending a BSS event to the interface attached to it.
	 */
	if (cb != NULL) {
		/* RSDB scan assoc callback may re-alloc cfg during move
		* So we store and restore bsscfg from idx
		*/
		int idx = WLC_BSSCFG_IDX(cfg);
		(cb)(cb_arg, status, cfg);
		cfg = wlc->bsscfg[idx];
	}
	else if (scan_completed == BCME_OK) {
		/* Post a BSS event if an interface is attached to it */
		wlc_scan_bss_mac_event(scan_info, cfg, WLC_E_SCAN_COMPLETE, NULL,
		status, 0, 0, NULL, 0);
	}

	SCAN_RESTORE_BSSCFG(scan_info, cfg);

	/* reset scan engine usage */
	if (scan_completed == BCME_OK) {
#ifdef WLRSDB
		if (RSDB_PARALLEL_SCAN_ON(scan_info))
			scan_info = (scan_info_t *)cfg->wlc->scan->scan_priv;
#endif
		scan_info->scan_pub->wlc_scan_cmn->usage = SCAN_ENGINE_USAGE_NORM;
		/* Free the BSS's in the scan_results. Use the scan info where
		 * scan request is given.
		 */
		wlc_scan_bss_list_free(scan_info);

#ifdef ANQPO
		if (SCAN_ANQPO_ENAB(scan_info) &&
			scan_info->scan_pub->wlc_scan_cmn->is_hotspot_scan) {
			wl_scan_anqpo_scan_stop(scan_info);
		}
#endif /* ANQPO */
	}
#ifdef WLPFN
	if (WLPFN_ENAB(wlc->pub) && !is_pfn_in_progress)
		wl_notify_pfn(wlc);
#endif /* WLPFN */

	/* allow scanmac to be updated */
	wlc_scanmac_update(scan_info->scan_pub);
}

chanspec_t
wlc_scan_get_current_chanspec(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;

	if (scan_info->channel_idx >= scan_info->channel_num)
		return scan_info->chanspec_list[scan_info->channel_num - 1];

	return scan_info->chanspec_list[scan_info->channel_idx];
}

int
wlc_scan_ioctl(void *ctx, uint cmd, void *arg, uint len, struct wlc_if *wlcif)
{
	wlc_scan_info_t *wlc_scan_info = ctx;
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	wlc_info_t *wlc = scan_info->wlc;
	int bcmerror = 0;
	int val = 0, *pval;
	bool bool_val;

	/* default argument is generic integer */
	pval = (int *) arg;
	/* This will prevent the misaligned access */
	if (pval && (uint32)len >= sizeof(val))
		bcopy(pval, &val, sizeof(val));

	/* bool conversion to avoid duplication below */
	bool_val = (val != 0);
	BCM_REFERENCE(bool_val);

	switch (cmd) {
#ifdef STA
	case WLC_SET_PASSIVE_SCAN:
		scan_info->scan_cmn->defaults.passive = (bool_val ? 1 : 0);
		break;

	case WLC_GET_PASSIVE_SCAN:
		ASSERT(pval != NULL);
		if (pval != NULL)
			*pval = scan_info->scan_cmn->defaults.passive;
		else
			bcmerror = BCME_BADARG;
		break;

	case WLC_GET_SCANSUPPRESS:
		 ASSERT(arg != NULL);
		 bcmerror = wlc_iovar_op(wlc, "scansuppress", NULL, 0, arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCANSUPPRESS:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scansuppress", NULL, 0, arg, len, IOV_SET, wlcif);
		break;

	case WLC_GET_SCAN_CHANNEL_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_assoc_time", NULL, 0,
			arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCAN_CHANNEL_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_assoc_time", NULL, 0,
			arg, len, IOV_SET, wlcif);
		break;

	case WLC_GET_SCAN_UNASSOC_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_unassoc_time", NULL, 0,
			arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCAN_UNASSOC_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_unassoc_time", NULL, 0,
			arg, len, IOV_SET, wlcif);
		break;
#endif /* STA */

	case WLC_GET_SCAN_PASSIVE_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_passive_time", NULL, 0,
			arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCAN_PASSIVE_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_passive_time", NULL, 0,
			arg, len, IOV_SET, wlcif);
		break;

	case WLC_GET_SCAN_HOME_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_home_time", NULL, 0,
			arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCAN_HOME_TIME:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_home_time", NULL, 0,
			arg, len, IOV_SET, wlcif);
		break;

	case WLC_GET_SCAN_NPROBES:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_nprobes", NULL, 0,
			arg, len, IOV_GET, wlcif);
		break;

	case WLC_SET_SCAN_NPROBES:
		ASSERT(arg != NULL);
		bcmerror = wlc_iovar_op(wlc, "scan_nprobes", NULL, 0,
			arg, len, IOV_SET, wlcif);
		break;

	default:
		bcmerror = BCME_UNSUPPORTED;
		break;
	}
	return bcmerror;
}

#ifdef BCMDBG
/* test case support - requires wl UP (wl mpc 0; wl up) */
static void
wlc_scan_test_done(void *arg, int status, wlc_bsscfg_t *cfg)
{
	scan_info_t *scan_info = (scan_info_t *)arg;
	wlc_info_t *wlc = scan_info->wlc;

	BCM_REFERENCE(status);
	BCM_REFERENCE(cfg);

	scan_info->test = SCAN_TEST_NONE;

	if (scan_info->test_timer != NULL) {
		wl_del_timer(wlc->wl, scan_info->test_timer);
		wl_free_timer(wlc->wl, scan_info->test_timer);
		scan_info->test_timer = NULL;
	}
}

static void
wlc_scan_test_timer(void *arg)
{
	scan_info_t *scan_info = (scan_info_t *)arg;
	wlc_scan_info_t	*wlc_scan_info = scan_info->scan_pub;
	wlc_ssid_t ssid;
	int err;

	ssid.SSID_len = 0;

	switch (scan_info->test) {
	case SCAN_TEST_ABORT_ENTRY:
	case SCAN_TEST_ABORT_PSPEND:
	case SCAN_TEST_ABORT_WSUSPEND:
		wlc_scan_terminate(wlc_scan_info, WLC_E_STATUS_SUCCESS);
		break;
	case SCAN_TEST_ABORT_PSPEND_AND_SCAN:
	case SCAN_TEST_ABORT_WSUSPEND_AND_SCAN:
		wlc_scan_terminate(wlc_scan_info, WLC_E_STATUS_SUCCESS);
		err = wlc_scan(wlc_scan_info, DOT11_BSSTYPE_ANY, &ether_bcast, 1, &ssid,
		               -1, -1, -1, -1, -1,
		               NULL, 0, 0, FALSE,
		               wlc_scan_test_done, scan_info, 0, FALSE, FALSE,
		               SCANCACHE_ENAB(wlc_scan_info),
		               0, NULL, SCAN_ENGINE_USAGE_NORM, NULL, NULL, NULL);
		if (err != BCME_OK) {
			WL_SCAN_ERROR(("%s: wlc_scan failed, err %d\n", __FUNCTION__, err));
		}
		break;
	}
}

static int
wlc_scan_test(scan_info_t *scan_info, uint8 test_case)
{
	wlc_scan_info_t	*wlc_scan_info = scan_info->scan_pub;
	wlc_info_t *wlc = scan_info->wlc;
	wlc_ssid_t ssid;

	WL_PRINT(("%s: test case %d\n", __FUNCTION__, test_case));

	if (scan_info->test != SCAN_TEST_NONE)
		return BCME_BUSY;

	if (test_case == SCAN_TEST_NONE)
		return BCME_OK;

	ssid.SSID_len = 0;

	if (scan_info->test_timer == NULL)
		scan_info->test_timer =
		        wl_init_timer(wlc->wl, wlc_scan_test_timer, scan_info, "testtimer");
	if (scan_info->test_timer == NULL)
		return BCME_NORESOURCE;

	if ((scan_info->test = test_case) == SCAN_TEST_ABORT_ENTRY)
		/* do this out of order because the timer is served FIFO */
		wl_add_timer(wlc->wl, scan_info->test_timer, 0, 0);

	return wlc_scan(wlc_scan_info, DOT11_BSSTYPE_ANY, &ether_bcast, 1, &ssid,
	                -1, -1, -1, -1, -1,
	                NULL, 0, 0, FALSE,
	                wlc_scan_test_done, scan_info, 0, FALSE, FALSE,
	                SCANCACHE_ENAB(wlc_scan_info),
	                0, NULL, SCAN_ENGINE_USAGE_NORM, NULL, NULL, NULL);
}
#endif	/* BCMDBG */

#ifdef WLC_SCAN_IOVARS
static int
wlc_scan_doiovar(void *hdl, uint32 actionid,
	void *params, uint p_len, void *arg, uint len, uint val_size, struct wlc_if *wlcif)
{
	scan_info_t *scan_info = (scan_info_t *)hdl;
	int err = 0;
	int32 int_val = 0;
	bool bool_val = FALSE;
	int32 *ret_int_ptr;
	wlc_info_t *wlc = scan_info->wlc;

	BCM_REFERENCE(wlc);
	BCM_REFERENCE(val_size);
	BCM_REFERENCE(wlcif);

	/* convenience int and bool vals for first 4 bytes of buffer */
	if (p_len >= (int)sizeof(int_val))
		bcopy(params, &int_val, sizeof(int_val));
	bool_val = (int_val != 0) ? TRUE : FALSE;
	BCM_REFERENCE(bool_val);

	/* convenience int ptr for 4-byte gets (requires int aligned arg) */
	ret_int_ptr = (int32 *)arg;

	/* Do the actual parameter implementation */
	switch (actionid) {
	case IOV_GVAL(IOV_PASSIVE):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.passive;
		break;

	case IOV_SVAL(IOV_PASSIVE):
		scan_info->scan_cmn->defaults.passive = (int8)int_val;
		break;

#ifdef STA
	case IOV_GVAL(IOV_SCAN_ASSOC_TIME):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.assoc_time;
		break;

	case IOV_SVAL(IOV_SCAN_ASSOC_TIME):
		scan_info->scan_cmn->defaults.assoc_time = (uint16)int_val;
		break;

	case IOV_GVAL(IOV_SCAN_UNASSOC_TIME):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.unassoc_time;
		break;

	case IOV_SVAL(IOV_SCAN_UNASSOC_TIME):
		scan_info->scan_cmn->defaults.unassoc_time = (uint16)int_val;
		break;
#endif /* STA */

	case IOV_GVAL(IOV_SCAN_PASSIVE_TIME):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.passive_time;
		break;

	case IOV_SVAL(IOV_SCAN_PASSIVE_TIME):
		scan_info->scan_cmn->defaults.passive_time = (uint16)int_val;
		break;

#ifdef STA
	case IOV_GVAL(IOV_SCAN_HOME_TIME):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.home_time;
		break;

	case IOV_SVAL(IOV_SCAN_HOME_TIME):
		scan_info->scan_cmn->defaults.home_time = (uint16)int_val;
		break;

	case IOV_GVAL(IOV_SCAN_NPROBES):
		*ret_int_ptr = (int32)scan_info->scan_cmn->defaults.nprobes;
		break;

	case IOV_SVAL(IOV_SCAN_NPROBES):
		scan_info->scan_cmn->defaults.nprobes = (int8)int_val;
		break;

	case IOV_GVAL(IOV_SCAN_FORCE_ACTIVE):
		*ret_int_ptr = (int32)scan_info->scan_cmn->force_active;
		break;

	case IOV_SVAL(IOV_SCAN_FORCE_ACTIVE):
		scan_info->scan_cmn->force_active = (int_val != 0);
		break;

#ifdef WLSCANCACHE
	case IOV_GVAL(IOV_SCANCACHE):
		*ret_int_ptr = scan_info->scan_pub->wlc_scan_cmn->_scancache;
		break;

	case IOV_SVAL(IOV_SCANCACHE):
		if (SCANCACHE_SUPPORT(wlc)) {
			scan_info->scan_pub->wlc_scan_cmn->_scancache = bool_val;
#ifdef WL11K
			/* Enable Table mode beacon report in RRM cap if scancache enabled */
			if (WL11K_ENAB(wlc->pub)) {
				wlc_bsscfg_t *cfg = wlc_bsscfg_find_by_wlcif(wlc, wlcif);

				ASSERT(cfg != NULL);

				wlc_rrm_update_cap(wlc->rrm_info, cfg);
			}
#endif /* WL11K */
		}
		else
			err = BCME_UNSUPPORTED;
		break;

	case IOV_GVAL(IOV_SCANCACHE_TIMEOUT):
		if (SCANCACHE_ENAB(scan_info->scan_pub))
			*ret_int_ptr = (int32)wlc_scandb_timeout_get(scan_info->sdb);
		else
			err = BCME_UNSUPPORTED;
		break;

	case IOV_SVAL(IOV_SCANCACHE_TIMEOUT):
		if (SCANCACHE_ENAB(scan_info->scan_pub))
			wlc_scandb_timeout_set(scan_info->sdb, (uint)int_val);
		else
			err = BCME_UNSUPPORTED;
		break;

	case IOV_SVAL(IOV_SCANCACHE_CLEAR):
		/* scancache might be disabled while clearing the cache.
		 * So check for scancache_support instead of scancache_enab.
		 */
		if (SCANCACHE_SUPPORT(wlc))
			wlc_scandb_clear(scan_info->sdb);
		else
			err = BCME_UNSUPPORTED;
		break;

#endif /* WLSCANCACHE */

	case IOV_GVAL(IOV_SCAN_ASSOC_TIME_DEFAULT):
		*ret_int_ptr = WLC_SCAN_ASSOC_TIME;
		break;

	case IOV_GVAL(IOV_SCAN_UNASSOC_TIME_DEFAULT):
		*ret_int_ptr = WLC_SCAN_UNASSOC_TIME;
		break;

#endif /* STA */

	case IOV_GVAL(IOV_SCAN_HOME_TIME_DEFAULT):
		*ret_int_ptr = WLC_SCAN_HOME_TIME;
		break;

	case IOV_GVAL(IOV_SCAN_PASSIVE_TIME_DEFAULT):
		*ret_int_ptr = WLC_SCAN_PASSIVE_TIME;
		break;
#ifdef BCMDBG
	case IOV_SVAL(IOV_SCAN_DBG):
		scan_info->debug = (uint8)int_val;
		break;
	case IOV_SVAL(IOV_SCAN_TEST):
		err = wlc_scan_test(scan_info, (uint8)int_val);
		break;
#endif
#ifdef STA
	case IOV_GVAL(IOV_SCAN_HOME_AWAY_TIME):
		*ret_int_ptr = (int32)scan_info->scan_cmn->home_away_time;
		break;
	case IOV_SVAL(IOV_SCAN_HOME_AWAY_TIME):
		if (int_val <= 0)
			err = BCME_BADARG;
		else
			scan_info->scan_cmn->home_away_time = (uint16)int_val;
		break;
#endif /* STA */

#ifdef WLRSDB
	case IOV_GVAL(IOV_SCAN_RSDB_PARALLEL_SCAN):
		if (RSDB_ENAB(wlc->pub))
			*ret_int_ptr  = scan_info->scan_cmn->rsdb_parallel_scan;
		else
			err = BCME_UNSUPPORTED;
		break;
	case IOV_SVAL(IOV_SCAN_RSDB_PARALLEL_SCAN):
		if (WLC_DUALMAC_RSDB(wlc->cmn)) {
		/* scan_parallel shouldn't be turned off for dual mac rsdb chips */
			if (!bool_val)
				err = BCME_BADARG;
		} else if (RSDB_ENAB(wlc->pub)) {
			if (ANY_SCAN_IN_PROGRESS(scan_info->scan_pub)) {
				err = BCME_BUSY;
			} else {
				scan_info->scan_cmn->rsdb_parallel_scan = bool_val;
			}
		} else {
			err = BCME_UNSUPPORTED;
		}
		break;
#endif /* WLRSDB */

#ifdef WLSCAN_PS
#if defined(BCMDBG) || defined(WLTEST)
	case IOV_GVAL(IOV_SCAN_RX_PWRSAVE):
		if (WLSCAN_PS_ENAB(wlc->pub)) {
			*ret_int_ptr = (uint8)scan_info->scan_rx_pwrsave;
		} else {
			err = BCME_UNSUPPORTED;
		}
		break;

	case IOV_SVAL(IOV_SCAN_RX_PWRSAVE):
		if (WLSCAN_PS_ENAB(wlc->pub)) {
			switch (int_val) {
				case 0:
				case 1:
					scan_info->scan_rx_pwrsave = (uint8)int_val;
					break;
				default:
					err = BCME_BADARG;
			}
		} else {
			err = BCME_UNSUPPORTED;
		}
		break;

	case IOV_GVAL(IOV_SCAN_TX_PWRSAVE):
		if (WLSCAN_PS_ENAB(wlc->pub)) {
			*ret_int_ptr = (uint8)scan_info->scan_tx_pwrsave;
		} else {
			err = BCME_UNSUPPORTED;
		}
		break;

	case IOV_SVAL(IOV_SCAN_TX_PWRSAVE):
		if (WLSCAN_PS_ENAB(wlc->pub)) {
			switch (int_val) {
				case 0:
				case 1:
					scan_info->scan_tx_pwrsave = (uint8)int_val;
					break;
				default:
					err = BCME_BADARG;
			}
		} else {
			err = BCME_UNSUPPORTED;
		}
		break;
#endif /* defined(BCMDBG) || defined(WLTEST) */
	case IOV_GVAL(IOV_SCAN_PWRSAVE):
		*ret_int_ptr = (uint8)scan_info->scan_cmn->scan_pwrsave_enable;
		break;
	case IOV_SVAL(IOV_SCAN_PWRSAVE):
		if (int_val < 0 || int_val > 1)
			err = BCME_BADARG;
		else
			scan_info->scan_cmn->scan_pwrsave_enable = (uint8)int_val;
		break;
#endif /* WLSCAN_PS */
	case IOV_GVAL(IOV_SCANMAC):
		err = scanmac_get(scan_info, params, p_len, arg, len);
		break;
	case IOV_SVAL(IOV_SCANMAC):
		err = scanmac_set(scan_info, arg, len);
		break;

	case IOV_SVAL(IOV_SCAN_SUPPRESS):
		if (bool_val)
			scan_info->scan_pub->state |= SCAN_STATE_SUPPRESS;
		else
			scan_info->scan_pub->state &= ~SCAN_STATE_SUPPRESS;
		break;

	case IOV_GVAL(IOV_SCAN_SUPPRESS):
		*ret_int_ptr = scan_info->scan_pub->state & SCAN_STATE_SUPPRESS ? 1 : 0;
		break;

	case IOV_GVAL(IOV_SCAN_IN_PROGRESS):
		*ret_int_ptr = (int32)scan_info->scan_pub->in_progress;
		break;

	default:
		err = BCME_UNSUPPORTED;
		break;
	}

	return err;
}
#endif /* WLC_SCAN_IOVARS */

static void
wlc_scan_sendprobe(scan_info_t *scan_info)
{
	int i;
	wlc_ssid_t *ssid;
	int n;
	wlc_bsscfg_t *cfg = scan_info->bsscfg;
	const struct ether_addr *da = &ether_bcast;
	const struct ether_addr *bssid = &ether_bcast;

	ASSERT(scan_info->pass >= 1);

	if (scan_info->extdscan) {
		ssid = &scan_info->ssid_list[scan_info->pass - 1];
		n = scan_info->nprobes;
	}
	else {
		ssid = scan_info->ssid_list;
		n = scan_info->nssid;
	}

#ifdef EXT_STA
	if (IS_EXTSTA_ENAB(scan_info) && scan_info->suppress_ssid) {
		_wlc_scan_sendprobe(scan_info, cfg, (const uchar *)"", 0, &scan_info->sa_override,
		              da, bssid, SCAN_PROBE_TXRATE(scan_info),
		              NULL, 0);
		return;
	}
#endif /* EXT_STA */

	for (i = 0; i < n; i++) {
#ifdef WLFMC
		wlc_scan_info_t *wlc_scan_info = scan_info->scan_pub;
		/* in case of roaming reassoc, use unicast scans */
		if (WLFMC_ENAB(scan_info->wlc->pub) &&
			!ETHER_ISMULTI(&wlc_scan_info->bssid) &&
			(cfg->assoc->type == AS_ROAM) &&
			(cfg->roam->reason == WLC_E_REASON_INITIAL_ASSOC)) {
			da = &wlc_scan_info->bssid;
			bssid = &wlc_scan_info->bssid;
		}
#endif /* WLFMC */
		_wlc_scan_sendprobe(scan_info, cfg, ssid->SSID, ssid->SSID_len,
			&scan_info->sa_override, da, bssid, SCAN_PROBE_TXRATE(scan_info), NULL, 0);
		if (!scan_info->extdscan)
			ssid++;
	}
}

static bool
wlc_scan_usage_scan(wlc_scan_info_t *scan)
{
	return (NORM_IN_PROGRESS(scan) || ESCAN_IN_PROGRESS(scan));
}

static void
wlc_scan_do_pass(scan_info_t *scan_info, chanspec_t chanspec)
{
	uint32	channel_dwelltime = 0;
	uint32  dummy_tsf_h, start_tsf;
	char chanbuf[CHANSPEC_STR_LEN];
	BCM_REFERENCE(chanbuf);

	SCAN_READ_TSF(scan_info, &start_tsf, &dummy_tsf_h);
	scan_info->start_tsf = start_tsf;

	channel_dwelltime = CHANNEL_ACTIVE_DWELLTIME(scan_info)/scan_info->npasses;

	WL_SCAN(("wl%d: active dwell time %d ms, chanspec %s, tsf %u\n",
		scan_info->unit, channel_dwelltime,
		wf_chspec_ntoa_ex(scan_info->cur_scan_chanspec, chanbuf),
		start_tsf));
#ifdef WLSCAN_SUMEVT
	if (SCAN_SUMEVT_ENAB(SCAN_WLC(scan_info)->pub)) {
	/* channel specific statistics part of scan summary to be populated */
		if (scan_info->pass == 1) {
			wlc_scan_summary_chan_stats(scan_info,
				SCAN_WLC(scan_info)->chanspec, FALSE);
		}
	}
#endif /* WLSCAN_SUMEVT */

	wlc_scan_act(scan_info, channel_dwelltime);

	/* record phy noise for the scan channel */
	if (D11REV_LT(IS_COREREV(scan_info), 40))
		wlc_lq_noise_sample_request(scan_info->wlc, WLC_NOISE_REQUEST_SCAN,
			CHSPEC_CHANNEL(chanspec));
}

bool
wlc_scan_ssid_match(wlc_scan_info_t *scan_pub, bcm_tlv_t *ssid_ie, bool filter)
{
	scan_info_t 	*scan_info = (scan_info_t *)scan_pub->scan_priv;
	wlc_ssid_t 	*ssid;
	int 		i;
	char		*c;

	if ((scan_pub->wlc_scan_cmn->usage == SCAN_ENGINE_USAGE_RM) ||
	    ((scan_info->nssid == 1) && ((scan_info->ssid_list[0]).SSID_len == 0))) {
		return TRUE;
	}

	if (scan_info->ssid_wildcard_enabled)
		return TRUE;

	if (!ssid_ie || ssid_ie->len > DOT11_MAX_SSID_LEN) {
		return FALSE;
	}

	/* filter out beacons which have all spaces or nulls as ssid */
	if (filter) {
		if (ssid_ie->len == 0)
			return FALSE;
		c = (char *)&ssid_ie->data[0];
		for (i = 0; i < ssid_ie->len; i++) {
			if ((*c != 0) && (*c != ' '))
				break;
			c++;
		}
		if (i == ssid_ie->len)
			return FALSE;
	}

	/* do not do ssid matching if we are sending out bcast SSIDs
	 * do the filtering before the scan_complete callback
	 */
#ifdef EXT_STA
	if (IS_EXTSTA_ENAB(scan_info)) {
		if (scan_info->suppress_ssid)
			return TRUE;
	}
#endif /* EXT_STA */

	ssid = scan_info->ssid_list;
	for (i = 0; i < scan_info->nssid; i++) {
		if (SCAN_IS_MATCH_SSID(scan_info, ssid->SSID, ssid_ie->data,
		                      ssid->SSID_len, ssid_ie->len))
			return TRUE;
#ifdef WLP2P
		if (IS_P2P_ENAB(scan_info) &&
		    wlc_p2p_ssid_match(scan_info->wlc->p2p, scan_info->bsscfg,
		                       ssid->SSID, ssid->SSID_len,
		                       ssid_ie->data, ssid_ie->len))
			return TRUE;
#endif
		ssid++;
	}
	return FALSE;
}

#ifdef WL11N
static void
wlc_ht_obss_scan_update(scan_info_t *scan_info, int status)
{
	wlc_info_t *wlc = scan_info->wlc;
	wlc_bsscfg_t *cfg;
	uint8 chanvec[OBSS_CHANVEC_SIZE]; /* bitvec of channels in 2G */
	uint8 chan, i;
	uint8 num_chan = 0;

	(void)wlc;

	WL_TRACE(("wl%d: wlc_ht_obss_scan_update\n", scan_info->unit));

	cfg = scan_info->bsscfg;

	/* checking  for valid fields */
	if (!wlc_obss_scan_fields_valid(wlc->obss, cfg)) {
		return;
	}

	if (!wlc_obss_is_scan_complete(wlc->obss, cfg, (status == WLC_E_STATUS_SUCCESS),
		scan_info->active_time, scan_info->passive_time)) {
		return;
	}

	bzero(chanvec, OBSS_CHANVEC_SIZE);
	for (i = 0; i < scan_info->channel_num; i++) {
		chan = CHSPEC_CHANNEL(scan_info->chanspec_list[i]);
		if (chan <= CH_MAX_2G_CHANNEL) {
			setbit(chanvec, chan);
			num_chan++;
		}
	}

	wlc_obss_scan_update_countdown(wlc->obss, cfg, chanvec, num_chan);

}
#endif /* WL11N */

#ifdef WLSCANCACHE

/* Add the current wlc_scan_info->scan_results to the scancache */
static void
wlc_scan_fill_cache(scan_info_t *scan_info, uint current_timestamp)
{
	uint index;
	wlc_bss_list_t *scan_results;
	wlc_bss_info_t *bi;
	wlc_ssid_t SSID;
	size_t datalen = 0;
	uint8* data = NULL;
	size_t bi_len;
	wlc_bss_info_t *new_bi;

	scan_results = SCAN_RESULT_PTR(scan_info);
	ASSERT(scan_results);

	wlc_scandb_ageout(scan_info->sdb, current_timestamp);

	/* walk the list of scan resutls, adding each to the cache */
	for (index = 0; index < scan_results->count; index++) {
		bi = scan_results->ptrs[index];
		if (bi == NULL) continue;

		bi_len = sizeof(wlc_bss_info_t);
		if (bi->bcn_prb)
			bi_len += bi->bcn_prb_len;

		/* allocate a new buffer if the current one is not big enough */
		if (data == NULL || bi_len > datalen) {
			if (data != NULL)
				MFREE(scan_info->osh, data, datalen);

			datalen = ROUNDUP(bi_len, 64);

			data = MALLOCZ(scan_info->osh, datalen);
			if (data == NULL) {
				WL_ERROR((WLC_MALLOC_ERR, WLCWLUNIT(scan_info->wlc), __FUNCTION__,
					(int)datalen, MALLOCED(scan_info->osh)));
				continue;
			}
		}

		new_bi = (wlc_bss_info_t*)data;

		memcpy(new_bi, bi, sizeof(wlc_bss_info_t));
		if (bi->bcn_prb) {
			new_bi->bcn_prb = (struct dot11_bcn_prb*)(data + sizeof(wlc_bss_info_t));
			memcpy(new_bi->bcn_prb, bi->bcn_prb, bi->bcn_prb_len);
		}
		new_bi->flags |= WLC_BSS_CACHE;

		SSID.SSID_len = bi->SSID_len;
		memcpy(SSID.SSID, bi->SSID, DOT11_MAX_SSID_LEN);

		wlc_scandb_add(scan_info->sdb,
		               &bi->BSSID, &SSID, bi->bss_type, bi->chanspec, current_timestamp,
		               new_bi, bi_len);
	}

	if (data != NULL)
		MFREE(scan_info->osh, data, datalen);
}

/* Return the contents of the scancache in the 'bss_list' param.
 *
 * Return only those scan results that match the criteria specified by the other params:
 *
 * BSSID:	match the provided BSSID exactly unless BSSID is a NULL pointer or FF:FF:FF:FF:FF:FF
 * nssid:	nssid number of ssids in the array pointed to by SSID
 * SSID:	match [one of] the provided SSIDs exactly unless SSID is a NULL pointer,
 *		SSID[0].SSID_len == 0 (broadcast SSID), or nssid = 0 (no SSIDs to match)
 * BSS_type:	match the 802.11 infrastructure type. Should be one of the values:
 *		{DOT11_BSSTYPE_INFRASTRUCTURE, DOT11_BSSTYPE_INDEPENDENT, DOT11_BSSTYPE_ANY}
 * chanspec_list, chanspec_num: if chanspec_num == 0, no channel filtering is done. Otherwise
 *		the chanspec list should contain 20MHz chanspecs. Only BSSs with a matching channel,
 *		or for a 40MHz BSS, with a matching control channel, will be returned.
 */
void
wlc_scan_get_cache(wlc_scan_info_t *scan_pub,
                   const struct ether_addr *BSSID, int nssid, const wlc_ssid_t *SSID,
                   int BSS_type, const chanspec_t *chanspec_list, uint chanspec_num,
                   wlc_bss_list_t *bss_list)
{
	scan_iter_params_t params;
	scan_info_t *scan_info = (scan_info_t *)scan_pub->scan_priv;

	params.merge = FALSE;
	params.bss_list = bss_list;
	params.current_ts = 0;

	memset(bss_list, 0, sizeof(wlc_bss_list_t));

	/* ageout any old entries */
	wlc_scandb_ageout(scan_info->sdb, OSL_SYSUPTIME());

	wlc_scandb_iterate(scan_info->sdb,
	                   BSSID, nssid, SSID, BSS_type, chanspec_list, chanspec_num,
	                   wlc_scan_build_cache_list, scan_info, &params);
}

/* Merge the contents of the scancache with entries already in the 'bss_list' param.
 *
 * Return only those scan results that match the criteria specified by the other params:
 *
 * current_timestamp: timestamp matching the most recent additions to the cache. Entries with
 *		this timestamp will not be added to bss_list.
 * BSSID:	match the provided BSSID exactly unless BSSID is a NULL pointer or FF:FF:FF:FF:FF:FF
 * nssid:	nssid number of ssids in the array pointed to by SSID
 * SSID:	match [one of] the provided SSIDs exactly unless SSID is a NULL pointer,
 *		SSID[0].SSID_len == 0 (broadcast SSID), or nssid = 0 (no SSIDs to match)
 * BSS_type:	match the 802.11 infrastructure type. Should be one of the values:
 *		{DOT11_BSSTYPE_INFRASTRUCTURE, DOT11_BSSTYPE_INDEPENDENT, DOT11_BSSTYPE_ANY}
 * chanspec_list, chanspec_num: if chanspec_num == 0, no channel filtering is done. Otherwise
 *		the chanspec list should contain 20MHz chanspecs. Only BSSs with a matching channel,
 *		or for a 40MHz BSS, with a matching control channel, will be returned.
 */
static void
wlc_scan_merge_cache(scan_info_t *scan_info, uint current_timestamp,
                   const struct ether_addr *BSSID, int nssid, const wlc_ssid_t *SSID,
                   int BSS_type, const chanspec_t *chanspec_list, uint chanspec_num,
                   wlc_bss_list_t *bss_list)
{
	scan_iter_params_t params;

	params.merge = TRUE;
	params.bss_list = bss_list;
	params.current_ts = current_timestamp;

	wlc_scandb_iterate(scan_info->sdb,
	                   BSSID, nssid, SSID, BSS_type, chanspec_list, chanspec_num,
	                   wlc_scan_build_cache_list, scan_info, &params);
}

static void
wlc_scan_build_cache_list(void *arg1, void *arg2, uint timestamp,
                          struct ether_addr *BSSID, wlc_ssid_t *SSID,
                          int BSS_type, chanspec_t chanspec, void *data, uint datalen)
{
	scan_info_t *scan_info = (scan_info_t*)arg1;
	scan_iter_params_t *params = (scan_iter_params_t*)arg2;
	wlc_bss_list_t *bss_list = params->bss_list;
	wlc_bss_info_t *bi;
	wlc_bss_info_t *cache_bi;

	BCM_REFERENCE(chanspec);
	BCM_REFERENCE(BSSID);
	BCM_REFERENCE(SSID);
	BCM_REFERENCE(BSS_type);

	/* skip the most recent batch of results when merging the cache to a bss_list */
	if (params->merge &&
	    params->current_ts == timestamp)
		return;

	if (bss_list->count >= (uint)SCAN_MAXBSS(scan_info))
		return;

	bi = MALLOC(scan_info->osh, sizeof(wlc_bss_info_t));
	if (!bi) {
		WL_ERROR(("wl%d: %s: out of memory, malloced %d bytes\n",
		          scan_info->unit, __FUNCTION__, MALLOCED(scan_info->osh)));
		return;
	}

	ASSERT(data != NULL);
	ASSERT(datalen >= sizeof(wlc_bss_info_t));

	cache_bi = (wlc_bss_info_t*)data;

	memcpy(bi, cache_bi, sizeof(wlc_bss_info_t));
	if (cache_bi->bcn_prb_len) {
		ASSERT(datalen >= sizeof(wlc_bss_info_t) + bi->bcn_prb_len);
		if (!(bi->bcn_prb = MALLOC(scan_info->osh, bi->bcn_prb_len))) {
			WL_ERROR(("wl%d: %s: out of memory, malloced %d bytes\n",
				scan_info->unit, __FUNCTION__, MALLOCED(scan_info->osh)));
			MFREE(scan_info->osh, bi, sizeof(wlc_bss_info_t));
			return;
		}
		/* Source is a flattened out structure but its bcn_prb pointer is not fixed
		 * when the entry was added to scancache db. So find out the new location.
		 */
		cache_bi->bcn_prb = (struct dot11_bcn_prb*)((uchar *) data +
		                                            sizeof(wlc_bss_info_t));

		memcpy(bi->bcn_prb, cache_bi->bcn_prb, bi->bcn_prb_len);
	}

	bss_list->ptrs[bss_list->count++] = bi;

}

static void
wlc_scan_cache_result(scan_info_t *scan_info)
{
	wlc_scan_info_t	*wlc_scan_info = scan_info->scan_pub;
	uint timestamp = OSL_SYSUPTIME();

	/* if we have scan caching enabled, enter these results in the cache */
	wlc_scan_fill_cache(scan_info, timestamp);

	/* filter to just the desired SSID if we did a bcast scan for suppress */
#ifdef EXT_STA
	if (IS_EXTSTA_ENAB(scan_info)) {
		if (scan_info->suppress_ssid) {
			uint i, dst;
			wlc_bss_info_t **list = SCAN_RESULT_MEB(scan_info, ptrs);
			wlc_bss_info_t *bi;
			wlc_ssid_t *ssid = &scan_info->ssid_list[0];
			uint count = SCAN_RESULT_MEB(scan_info, count);

			for (i = 0, dst = 0; i < count; i++) {
				bi = list[i];

				if ((bi->SSID_len == ssid->SSID_len) &&
				    !bcmp(bi->SSID, ssid->SSID, ssid->SSID_len)) {
					list[dst] = list[i];
					dst++;
				} else {
					if (bi->bcn_prb) {
						MFREE(scan_info->osh, bi->bcn_prb, bi->bcn_prb_len);
					}
					MFREE(scan_info->osh, bi, sizeof(wlc_bss_info_t));
					list[i] = NULL;
				}
			}
			SCAN_RESULT_MEB(scan_info, count) = dst;
		}
	}
#endif /* EXT_STA */

	/* Provide the latest results plus cached results if they were requested. */
	if (wlc_scan_info->state & SCAN_STATE_INCLUDE_CACHE) {
		/* Merge cached results with current results */
		wlc_scan_merge_cache(scan_info, timestamp,
		                     &wlc_scan_info->bssid,
		                     scan_info->nssid, &scan_info->ssid_list[0],
		                     wlc_scan_info->wlc_scan_cmn->bss_type,
		                     scan_info->chanspec_list, scan_info->channel_num,
		                     SCAN_RESULT_PTR(scan_info));

		WL_SCAN(("wl%d: %s: Merged scan results with cache, new total %d\n",
		         scan_info->unit, __FUNCTION__, SCAN_RESULT_MEB(scan_info, count)));
	}
}
#endif /* WLSCANCACHE */

static void
wlc_scan_watchdog(void *hdl)
{
#ifdef WLSCANCACHE
	scan_info_t *scan = (scan_info_t *)hdl;

	/* ageout any old entries to free up memory */
	if (SCANCACHE_ENAB(scan->scan_pub))
		wlc_scandb_ageout(scan->sdb, OSL_SYSUPTIME());
#endif
}

wlc_bsscfg_t *
wlc_scan_bsscfg(wlc_scan_info_t *wlc_scan_info)
{
	scan_info_t *scan_info = (scan_info_t *)wlc_scan_info->scan_priv;
	return scan_info->bsscfg;
}

#ifdef WLSCAN_PS
/* This function configures tx & rxcores to save power.
 *  flag: TRUE to set & FALSE to revert config
 */
#ifdef WL_STF_ARBITRATOR
static void
wlc_scan_ps_config_cores_arb(wlc_scan_info_t *scan_pub, bool flag)
{
	scan_info_t *scan_info = (scan_info_t *) scan_pub->scan_priv;
	wlc_info_t *wlc = scan_info->wlc;
	uint8 rxchains = 0, txchains = 0;

	/* Default to full chains */
	txchains = wlc->stf->hw_txchain;
	rxchains = wlc->stf->hw_rxchain;

	/* Scanning is started */
	/* On activate, adjust as specified by scan_ps iovar and mimo_override flag */
	if (!scan_info->scan_cmn->scan_mimo_override && scan_info->scan_cmn->scan_pwrsave_enable) {
		txchains = ONE_CHAIN_CORE0;
		rxchains = ONE_CHAIN_CORE0;
	}
	/* Pass the selected configuration to the STF arbitrator */
	wlc_stf_nss_request_update(wlc, scan_info->stf_scan_req,
		(flag ? WLC_STF_ARBITRATOR_REQ_STATE_RXTX_ACTIVE :
			WLC_STF_ARBITRATOR_REQ_STATE_RXTX_INACTIVE),
		txchains, WLC_BITSCNT(txchains), rxchains, WLC_BITSCNT(rxchains));
}
#endif /* WL_STF_ARBITRATOR */

static int
wlc_scan_ps_config_cores_non_arb(scan_info_t *scan_info, bool flag)
{

	int idx;
	wlc_bsscfg_t *cfg;

	/* bail out if both scan tx & rx pwr opt. are disabled */
	if (!scan_info->scan_tx_pwrsave &&
	    !scan_info->scan_rx_pwrsave) {
		WL_SCAN(("wl%d: %s(%d): tx_ps %d rx_ps\n",
		                scan_info->unit, __FUNCTION__, __LINE__,
		                scan_info->scan_tx_pwrsave,
		                scan_info->scan_rx_pwrsave));
		return BCME_OK;
	}

	/* enable cores only when device is in PM = 1 or 2 mode */
	SCAN_FOREACH_AS_STA(scan_info, idx, cfg) {
		if (cfg->BSS && cfg->pm->PM == PM_OFF) {
			WL_SCAN(("wl%d: %s(%d): pm %d\n",
				scan_info->unit, __FUNCTION__, __LINE__, cfg->pm->PM));

			/* If PM becomes 0 after scan initiated,
			  * we need to reset the cores
			  */
			if (!scan_info->scan_ps_rxchain &&
			    !scan_info->scan_ps_txchain) {
				WL_SCAN(("wl%d: %s(%d): txchain %d rxchain %d\n",
				    scan_info->unit, __FUNCTION__, __LINE__,
				    scan_info->scan_ps_txchain, scan_info->scan_ps_rxchain));
				return BCME_ERROR;
			}
		}
	}

	wlc_suspend_mac_and_wait(scan_info->wlc);

	if (flag) {
		/* Scanning is started */
		if ((
#if defined(BCMDBG) || defined(WLTEST)
			scan_info->scan_tx_pwrsave ||
#endif /* defined(BCMDBG) || defined(WLTEST) */
			scan_info->scan_cmn->scan_pwrsave_enable) && !scan_info->scan_ps_txchain) {
		/* if txchains doesn't match with hw defaults, don't modify chain mask
		  * and also ignore for 1x1. scan pwrsave iovar should be enabled otherwise ignore.
		  */
			if (wlc_stf_txchain_ishwdef(scan_info->wlc) &&
				scan_info->wlc->stf->hw_txchain >= 0x03) {
				wlc_suspend_mac_and_wait(scan_info->wlc);
				/* back up chain configuration */
				scan_info->scan_ps_txchain = scan_info->wlc->stf->txchain;
				wlc_stf_txchain_set(scan_info->wlc, 0x1, FALSE, WLC_TXCHAIN_ID_USR);
				wlc_enable_mac(scan_info->wlc);
			}
		}
		if ((
#if defined(BCMDBG) || defined(WLTEST)
			scan_info->scan_rx_pwrsave ||
#endif /* defined(BCMDBG) || defined(WLTEST) */
			scan_info->scan_cmn->scan_pwrsave_enable) && !scan_info->scan_ps_rxchain) {
		/* if rxchain doesn't match with hw defaults, don't modify chain mask
		  * and also ignore for 1x1. scan pwrsave iovar should be enabled otherwise ignore.
		  */
			if (wlc_stf_rxchain_ishwdef(scan_info->wlc) &&
				scan_info->wlc->stf->hw_rxchain >= 0x03) {
				wlc_suspend_mac_and_wait(scan_info->wlc);
				/* back up chain configuration */
				scan_info->scan_ps_rxchain = scan_info->wlc->stf->rxchain;
				wlc_stf_rxchain_set(scan_info->wlc, 0x1, TRUE);
				wlc_enable_mac(scan_info->wlc);
			}
		}
	} else {
		/* Scanning is ended */
		if (!scan_info->scan_ps_txchain && !scan_info->scan_ps_rxchain) {
			return BCME_OK;
		} else {
			/* when scan_ps_txchain is 0, it mean scan module has not modified chains */
			if (scan_info->scan_ps_txchain) {
				wlc_suspend_mac_and_wait(scan_info->wlc);
				wlc_stf_txchain_set(scan_info->wlc, scan_info->scan_ps_txchain,
				                    FALSE, WLC_TXCHAIN_ID_USR);
				scan_info->scan_ps_txchain = 0;
				wlc_enable_mac(scan_info->wlc);
			}
			if (scan_info->scan_ps_rxchain) {
				wlc_suspend_mac_and_wait(scan_info->wlc);
				wlc_stf_rxchain_set(scan_info->wlc,
				                    scan_info->scan_ps_rxchain, TRUE);
				/* make the chain value to 0 */
				scan_info->scan_ps_rxchain = 0;
				wlc_enable_mac(scan_info->wlc);
			}
		}
	}
	return BCME_OK;
}

static int
wlc_scan_ps_config_cores(scan_info_t *scan_info, bool flag)
{
#ifdef WL_STF_ARBITRATOR
	wlc_info_t *wlc = scan_info->wlc;
	if (WLC_STF_ARB_ENAB(wlc->pub)) {
		wlc_scan_ps_config_cores_arb(scan_info->scan_pub, flag);
		return BCME_OK;
	} else
#endif /* WL_STF_ARBITRATOR */
	{
		return wlc_scan_ps_config_cores_non_arb(scan_info, flag);
	}
}
#endif /* WLSCAN_PS */

#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static int
wlc_scan_dump(scan_info_t *si, struct bcmstrbuf *b)
{
	const bcm_bit_desc_t scan_flags[] = {
		{SCAN_STATE_SUPPRESS, "SUPPRESS"},
		{SCAN_STATE_SAVE_PRB, "SAVE_PRB"},
		{SCAN_STATE_PASSIVE, "PASSIVE"},
		{SCAN_STATE_WSUSPEND, "WSUSPEND"},
		{SCAN_STATE_RADAR_CLEAR, "RADAR_CLEAR"},
		{SCAN_STATE_PSPEND, "PSPEND"},
		{SCAN_STATE_DLY_WSUSPEND, "DLY_WSUSPEND"},
		{SCAN_STATE_READY, "READY"},
		{SCAN_STATE_INCLUDE_CACHE, "INC_CACHE"},
		{SCAN_STATE_PROHIBIT, "PROHIBIT"},
		{SCAN_STATE_IN_TMR_CB, "IN_TMR_CB"},
		{SCAN_STATE_OFFCHAN, "OFFCHAN"},
		{SCAN_STATE_TERMINATE, "TERMINATE"},
		{0, NULL}
	};
	const char *scan_usage[] = {
		"normal",
		"escan",
		"af",
		"rm",
		"excursion"
	};
	char state_str[64];
	char ssidbuf[SSID_FMT_BUF_LEN];
	char eabuf[ETHER_ADDR_STR_LEN];
	const char *bss_type_str;
	uint32 tsf_l, tsf_h;
	struct wlc_scan_info *scan_pub = si->scan_pub;

	wlc_format_ssid(ssidbuf, si->ssid_list[0].SSID, si->ssid_list[0].SSID_len);
	bcm_format_flags(scan_flags, scan_pub->state, state_str, 64);

	bss_type_str = wlc_bsstype_dot11name(scan_pub->wlc_scan_cmn->bss_type);

	bcm_bprintf(b, "in_progress %d SSID \"%s\" type %s BSSID %s state 0x%x [%s] "
	            "usage %u [%s]\n",
	            scan_pub->in_progress, ssidbuf, bss_type_str,
	            bcm_ether_ntoa(&scan_pub->bssid, eabuf),
	            scan_pub->state, state_str,
	            scan_pub->wlc_scan_cmn->usage,
	            scan_pub->wlc_scan_cmn->usage < ARRAYSIZE(scan_usage) ?
	            scan_usage[scan_pub->wlc_scan_cmn->usage] : "unknown");

	bcm_bprintf(b, "extdscan %d\n", si->extdscan);

	if (SCAN_IN_PROGRESS(scan_pub))
		bcm_bprintf(b, "wlc->home_chanspec: %x chanspec_current %x\n",
		            SCAN_HOME_CHANNEL(si), si->chanspec_list[si->channel_idx]);

	bcm_bprintf(b, "suppress_ssid %d\n", si->suppress_ssid);

	if (SCAN_ISUP(si)) {
		SCAN_READ_TSF(si, &tsf_l, &tsf_h);
		bcm_bprintf(b, "start_tsf 0x%08x current tsf 0x%08x\n", si->start_tsf, tsf_l);
	} else {
		bcm_bprintf(b, "start_tsf 0x%08x current tsf <not up>\n", si->start_tsf);
	}

	return 0;
}
#endif /* BCMDBG || BCMDBG_DUMP */

/* scanmac enable */
static int
scanmac_enable(scan_info_t *scan_info, int enable)
{
	int err = BCME_OK;
	wlc_info_t *wlc = scan_info->wlc;
	wlc_info_t *wlc_other = NULL;
	scan_info_t *scan_info_other = NULL;
	wlc_bsscfg_t *bsscfg = NULL;
	bool state = scan_info->scanmac_bsscfg != NULL;

	UNUSED_PARAMETER(wlc_other);
	UNUSED_PARAMETER(scan_info_other);

	if (enable) {
		int idx;
		uint32 flags = WLC_BSSCFG_NOIF | WLC_BSSCFG_NOBCMC;
		wlc_bsscfg_type_t type = {BSSCFG_TYPE_GENERIC, BSSCFG_GENERIC_STA};

		/* scanmac already enabled */
		if (state) {
			WL_SCAN(("wl%d: scanmac already enabled\n", wlc->pub->unit));
			return BCME_OK;
		}

		/* bsscfg with the MAC address exists */
		if (wlc_bsscfg_find_by_hwaddr(wlc, &scan_info->scanmac_config.mac) != NULL) {
			WL_SCAN_ERROR(("wl%d: MAC address is in use\n", wlc->pub->unit));
			return BCME_BUSY;
		}

		/* allocate bsscfg */
		if ((idx = wlc_bsscfg_get_free_idx(wlc)) == -1) {
			WL_ERROR(("wl%d: no free bsscfg\n", wlc->pub->unit));
			return BCME_NORESOURCE;
		}
		else if ((bsscfg = wlc_bsscfg_alloc(wlc, idx, &type, flags,
		                &scan_info->scanmac_config.mac)) == NULL) {
			WL_ERROR(("wl%d: cannot create bsscfg\n", wlc->pub->unit));
			return BCME_NOMEM;
		}
		else if (wlc_bsscfg_init(wlc, bsscfg) != BCME_OK) {
			WL_SCAN_ERROR(("wl%d: cannot init bsscfg\n", wlc->pub->unit));
			err = BCME_ERROR;
			goto free;
		}
		memcpy(&bsscfg->BSSID, &bsscfg->cur_etheraddr, ETHER_ADDR_LEN);
		bsscfg->current_bss->bss_type = DOT11_BSSTYPE_INDEPENDENT;
		scan_info->scanmac_bsscfg = bsscfg;
#ifdef WLRSDB
		/* When RSDB is enabled set the bsscfg for random MAC address in Core 1 as well */
		if (RSDB_ENAB(wlc->pub)) {
			wlc_other = wlc_rsdb_get_other_wlc(wlc);
			scan_info_other = (scan_info_t *)wlc_other->scan->scan_priv;
			scan_info_other->scanmac_bsscfg = bsscfg;
		}
#endif /* WLRSDB */
	}
	else {
		wlc_scan_info_t *wlc_scan_info = wlc->scan;

		/* scanmac not enabled */
		if (!state) {
			WL_SCAN(("wl%d: scanmac is already disabled\n", wlc->pub->unit));
			return BCME_OK;
		}

		bsscfg = scan_info->scanmac_bsscfg;
		ASSERT(bsscfg != NULL);

		wlc_scan_abort(wlc_scan_info, WLC_E_STATUS_ABORT);

	free:
		scan_info->scanmac_bsscfg = NULL;
		memset(&scan_info->scanmac_config, 0, sizeof(scan_info->scanmac_config));
#ifdef WLRSDB
		/* When RSDB is enabled reset the scan mac bsscfg in the AUX core as well */
		if (RSDB_ENAB(wlc->pub)) {
			wlc_other = wlc_rsdb_get_other_wlc(wlc);
			scan_info_other = (scan_info_t *)wlc_other->scan->scan_priv;
			scan_info_other->scanmac_bsscfg = NULL;
			memset(&scan_info_other->scanmac_config, 0,
				sizeof(scan_info->scanmac_config));
#ifdef WLMCNX
			if (MCNX_ENAB(wlc->pub))
				wlc_mcnx_ra_unset(wlc_other->mcnx, bsscfg);
#endif /* WLMCNX */
		}
#endif /* WLRSDB */
		/* free bsscfg + error handling */
		if (bsscfg != NULL)
			wlc_bsscfg_free(wlc, bsscfg);
	}

	return err;
}

/* get random mac */
static void
scanmac_random_mac(scan_info_t *scan_info, struct ether_addr *mac)
{
	wlc_info_t *wlc = scan_info->wlc;

	/* HW random generator only available if core is up */
	if (wlc_getrand(wlc, (uint8 *)mac, ETHER_ADDR_LEN) != BCME_OK) {
		/* random MAC based on OSL_RAND() */
		uint32 rand1 = OSL_RAND();
		uint32 rand2 = OSL_RAND();
		memcpy(&mac->octet[0], &rand1, 2);
		memcpy(&mac->octet[2], &rand2, sizeof(rand2));
	}
}

/* update configured or randomized MAC */
static void
scanmac_update_mac(scan_info_t *scan_info)
{
	wlc_info_t *wlc = scan_info->wlc;
	wlc_info_t *wlc_other = NULL;
	scan_info_t *scan_info_other = NULL;
	wlc_bsscfg_t *bsscfg = scan_info->scanmac_bsscfg;
	wl_scanmac_config_t *scanmac_config = &scan_info->scanmac_config;
	struct ether_addr *mac = &scanmac_config->mac;
	struct ether_addr *mask = &scanmac_config->random_mask;
	struct ether_addr random_mac;

	UNUSED_PARAMETER(wlc_other);
	UNUSED_PARAMETER(scan_info_other);

	/* generate random MAC if random mask is non-zero */
	if (!ETHER_ISNULLADDR(mask)) {
		struct ether_addr *prefix = &scanmac_config->mac;
		int i;
start_random_mac:
		scanmac_random_mac(scan_info, &random_mac);
		for (i = 0; i < ETHER_ADDR_LEN; i++) {
			/* AND the random mask bits */
			random_mac.octet[i] &= mask->octet[i];
			/* OR the MAC prefix */
			random_mac.octet[i] |= prefix->octet[i] & ~mask->octet[i];
		}

		/* randomize again if MAC is not unique accoss bsscfgs */
		if (wlc_bsscfg_find_by_hwaddr(wlc, &random_mac) != NULL) {
			WL_INFORM(("wl%d: scanmac_update_mac: regenerate random MAC\n",
				scan_info->unit));
			goto start_random_mac;
		}

		mac = &random_mac;
		scan_info->is_scanmac_config_updated = TRUE;
	}

	if (scan_info->is_scanmac_config_updated) {
		/* activate new MAC */
		memcpy(&bsscfg->BSSID, mac, ETHER_ADDR_LEN);
		wlc_validate_mac(wlc, bsscfg, mac);
		scan_info->is_scanmac_config_updated = FALSE;
	}
#ifdef WLRSDB
	/* When RSDB is enabled update the scan mac bsscfg and set the RA in mcnx */
	if (RSDB_ENAB(wlc->pub)) {
		wlc_other = wlc_rsdb_get_other_wlc(wlc);
		scan_info_other = (scan_info_t *)wlc_other->scan->scan_priv;
		scan_info_other->scanmac_bsscfg = bsscfg;
		scan_info_other->is_scanmac_config_updated = TRUE;
#ifdef WLMCNX
		if (MCNX_ENAB(wlc->pub))
			wlc_mcnx_ra_set(wlc_other->mcnx, bsscfg);
#endif /* WLMCNX */
	}
#endif /* WLRSDB */

}

/* scanmac config */
static int
scanmac_config(scan_info_t *scan_info, wl_scanmac_config_t *config)
{
	wlc_info_t *wlc = scan_info->wlc;
	wlc_bsscfg_t *bsscfg = scan_info->scanmac_bsscfg;

	if (scan_info->scanmac_bsscfg == NULL) {
		/* scanmac not enabled */
		return BCME_NOTREADY;
	}

	/* don't allow multicast MAC or random mask */
	if (ETHER_ISMULTI(&config->mac) || ETHER_ISMULTI(&config->random_mask)) {
		return BCME_BADADDR;
	}

	/* check if MAC exists */
	if (ETHER_ISNULLADDR(&config->random_mask)) {
		wlc_bsscfg_t *match = wlc_bsscfg_find_by_hwaddr(wlc, &config->mac);
		if (match != NULL && match != bsscfg) {
			WL_SCAN_ERROR(("wl%d: MAC address is in use\n", wlc->pub->unit));
			return BCME_BUSY;
		}
	}

	/* save config */
	memcpy(&scan_info->scanmac_config, config, sizeof(*config));
	scan_info->is_scanmac_config_updated = TRUE;

#ifdef WLRSDB
	/* When RSDB is enabled copy the Random MAC configuration in Core 1 scan info as well */
	if (RSDB_ENAB(wlc->pub)) {
		wlc_info_t *wlc_other = wlc_rsdb_get_other_wlc(wlc);
		scan_info_t *scan_info_other = (scan_info_t *)wlc_other->scan->scan_priv;
		scan_info_other->scanmac_bsscfg = bsscfg;
		memcpy(&scan_info_other->scanmac_config, config, sizeof(*config));
		scan_info_other->is_scanmac_config_updated = TRUE;
	}
#endif /* WLRSDB */

	/* update MAC if scan not in progress */
	if (!SCAN_IN_PROGRESS(scan_info->scan_pub)) {
		scanmac_update_mac(scan_info);
	}

	return BCME_OK;
}

/* scanmac GET iovar */
static int
scanmac_get(scan_info_t *scan_info, void *params, uint p_len, void *arg, int len)
{
	int err = BCME_OK;
	wl_scanmac_t *sm = params;
	wl_scanmac_t *sm_out = arg;

	BCM_REFERENCE(len);

	/* verify length */
	if (p_len < OFFSETOF(wl_scanmac_t, data) ||
		sm->len > p_len - OFFSETOF(wl_scanmac_t, data)) {
		WL_SCAN_ERROR((WLC_SCAN_LENGTH_ERR, __FUNCTION__, p_len, sm->len));
		return BCME_BUFTOOSHORT;
	}

	/* copy subcommand to output */
	sm_out->subcmd_id = sm->subcmd_id;

	/* process subcommand */
	switch (sm->subcmd_id) {
	case WL_SCANMAC_SUBCMD_ENABLE:
	{
		wl_scanmac_enable_t *sm_enable = (wl_scanmac_enable_t *)sm_out->data;
		sm_out->len = sizeof(*sm_enable);
		sm_enable->enable = scan_info->scanmac_bsscfg ? 1 : 0;
		break;
	}
	case WL_SCANMAC_SUBCMD_BSSCFG:
	{
		wl_scanmac_bsscfg_t *sm_bsscfg = (wl_scanmac_bsscfg_t *)sm_out->data;
		if (scan_info->scanmac_bsscfg == NULL) {
			return BCME_ERROR;
		} else {
			sm_out->len = sizeof(*sm_bsscfg);
			sm_bsscfg->bsscfg = WLC_BSSCFG_IDX(scan_info->scanmac_bsscfg);
		}
		break;
	}
	case WL_SCANMAC_SUBCMD_CONFIG:
	{
		wl_scanmac_config_t *sm_config = (wl_scanmac_config_t *)sm_out->data;
		sm_out->len = sizeof(*sm_config);
		memcpy(sm_config, &scan_info->scanmac_config, sizeof(*sm_config));
		break;
	}
	default:
		ASSERT(0);
		err = BCME_UNSUPPORTED;
		break;
	}
	return err;
}

/* scanmac SET iovar */
static int
scanmac_set(scan_info_t *scan_info, void *arg, int len)
{
	int err = BCME_OK;
	wl_scanmac_t *sm = arg;

	/* verify length */
	if (len < (int)OFFSETOF(wl_scanmac_t, data) ||
		sm->len > len - OFFSETOF(wl_scanmac_t, data)) {
		return BCME_BUFTOOSHORT;
	}

	/* process subcommand */
	switch (sm->subcmd_id) {
	case WL_SCANMAC_SUBCMD_ENABLE:
	{
		wl_scanmac_enable_t *sm_enable = (wl_scanmac_enable_t *)sm->data;
		if (sm->len >= sizeof(*sm_enable)) {
			err = scanmac_enable(scan_info, sm_enable->enable);
		} else  {
			err = BCME_BADLEN;
		}
		break;
	}
	case WL_SCANMAC_SUBCMD_BSSCFG:
		err = BCME_BADARG;
		break;
	case WL_SCANMAC_SUBCMD_CONFIG:
	{
		wl_scanmac_config_t *sm_config = (wl_scanmac_config_t *)sm->data;
		if (sm->len >= sizeof(*sm_config)) {
			err = scanmac_config(scan_info, sm_config);
		} else  {
			err = BCME_BADLEN;
		}
		break;
	}
	default:
		ASSERT(0);
		err = BCME_UNSUPPORTED;
		break;
	}
	return err;
}

/* return scanmac bsscfg if macreq enabled, else return NULL */
wlc_bsscfg_t *
wlc_scanmac_get_bsscfg(wlc_scan_info_t *scan, int macreq, wlc_bsscfg_t *cfg)
{
	scan_info_t *scan_info = scan->scan_priv;
	wlc_bsscfg_t *bsscfg = SCAN_USER(scan_info, cfg);

	/* check if scanmac enabled */
	if (scan_info->scanmac_bsscfg != NULL) {
		uint16 sbmap = scan_info->scanmac_config.scan_bitmap;
		bool is_associated = IS_BSS_ASSOCIATED(bsscfg);
		bool is_host_scan = (macreq == WLC_ACTION_SCAN || macreq == WLC_ACTION_ISCAN ||
			macreq == WLC_ACTION_ESCAN);

		/* return scanmac bsscfg if bitmap for macreq is enabled */
		if ((!is_associated && (is_host_scan || macreq == WLC_ACTION_PNOSCAN) &&
			(sbmap & WL_SCANMAC_SCAN_UNASSOC)) ||
			(is_associated &&
			(((macreq == WLC_ACTION_ROAM) && (sbmap & WL_SCANMAC_SCAN_ASSOC_ROAM)) ||
			((macreq == WLC_ACTION_PNOSCAN) && (sbmap & WL_SCANMAC_SCAN_ASSOC_PNO)) ||
			(is_host_scan && (sbmap & WL_SCANMAC_SCAN_ASSOC_HOST))))) {
			return scan_info->scanmac_bsscfg;
		}
	}
	return NULL;
}

/* return scanmac mac if macreq enabled, else return NULL */
struct ether_addr *
wlc_scanmac_get_mac(wlc_scan_info_t *scan, int macreq, wlc_bsscfg_t *bsscfg)
{
	wlc_bsscfg_t *scanmac_bsscfg = wlc_scanmac_get_bsscfg(scan, macreq, bsscfg);

	if (scanmac_bsscfg != NULL) {
		return &scanmac_bsscfg->cur_etheraddr;
	}
	return NULL;
}

/* invoked at scan or GAS complete to allow MAC to be updated */
int
wlc_scanmac_update(wlc_scan_info_t *scan)
{
	scan_info_t *scan_info = scan->scan_priv;

	if (scan_info->scanmac_bsscfg == NULL) {
		/* scanmac not enabled */
		return BCME_NOTREADY;
	}

	scanmac_update_mac(scan_info);

	return BCME_OK;
}

static void
wlc_scan_bss_deinit(void *ctx, wlc_bsscfg_t *cfg)
{
	scan_info_t *scan_info = (scan_info_t *)ctx;
	wlc_info_t *wlc = scan_info->wlc;
	int idx;
	wlc_info_t *my_wlc;
	int scan_cfg = FALSE;

	FOREACH_WLC(wlc->cmn, idx, my_wlc) {
		if (SCAN_IN_PROGRESS(my_wlc->scan) && cfg == wlc_scan_bsscfg(my_wlc->scan)) {
			scan_cfg = TRUE;
			break;
		}
	}
	/* Make sure that any active scan is not associated to this cfg */
	if (scan_cfg) {
		/* ASSERT(bsscfg != wlc_scan_bsscfg(wlc->scan)); */
		WL_SCAN_ERROR(("wl%d.%d: %s: scan still active using cfg %p\n", WLCWLUNIT(wlc),
		          WLC_BSSCFG_IDX(cfg), __FUNCTION__, OSL_OBFUSCATE_BUF(cfg)));
		wlc_scan_abort(wlc->scan, WLC_E_STATUS_ABORT);
	}
}


/* a wrapper to check & end scan excursion & resume primary txq operation */
static void
wlc_scan_excursion_end(scan_info_t *scan_info)
{
	/* enable CFP and TSF update */
	wlc_mhf(SCAN_WLC(scan_info), MHF2, MHF2_SKIP_CFP_UPDATE, 0, WLC_BAND_ALL);
	wlc_scan_skip_adjtsf(scan_info, FALSE, NULL, WLC_SKIP_ADJTSF_SCAN,
		WLC_BAND_ALL);

	/* Restore promisc behavior for beacons and probes */
	SCAN_BCN_PROMISC(scan_info) = FALSE;
	wlc_scan_mac_bcn_promisc(scan_info);

	/* check & resume normal tx queue operation */
	wlc_txqueue_end(scan_info->wlc, NULL, NULL);
}

#ifdef RSDB_APSCAN
/*
 * Checks if chanspeclist contain channels of AP band.
 * return: Num of channels available after truncation of channels in AP bands based on prunetype
 *	-ROAM_PRUNE_APBAND_CHANNELS: Remove the channels in the AP band other than AP's
 *		operating channel
 *	-ROAM_PRUNE_NON_APBAND_CHANNELS: Removes all the channels other than the channels
 *		in the AP band other than the AP's operating channel
 */
static int
wlc_scan_filter_ap_band_channels(scan_info_t * scan, chanspec_t *chspec_list, int num)
{
	wlc_info_t *wlc = SCAN_WLC(scan);
	int cfgidx;
	int chanidx;
	wlc_bsscfg_t *cfg;
	int sync_idx = num;
	bool prune_chanlist = FALSE;
	chanspec_t ap_chspec = 0;
	chanspec_t *ch_list = chspec_list;
	roam_prune_type_t prune_type = ROAM_PRUNE_APBAND_CHANNELS;
	int chnum_to_remove = 0;

	/* it is a roam scan and roam prune active. */
	prune_type = wlc_assoc_get_prune_type(wlc);

	if (prune_type == ROAM_PRUNE_NONE)
		return num;

	/* Running through ALL cfg's irrespective of WLC. */
	FOR_ALL_UP_AP_BSS(wlc, cfgidx, cfg) {
		/* Only infrastructure AP's should be considered */
		if (P2P_GO(wlc, cfg))
			continue;

		ap_chspec = wf_chspec_ctlchspec(cfg->current_bss->chanspec);
		chspec_list = ch_list;
		for (chanidx = 0; chanidx < num; chanidx++) {
			if (prune_type == ROAM_PRUNE_APBAND_CHANNELS) {
				/* Removing the AP's band channels */
				if (CHSPEC_BAND(ap_chspec) == CHSPEC_BAND(*chspec_list) &&
					ap_chspec != *chspec_list) {
					/* set the chanspec to zero to truncate. */
					*chspec_list = 0;
					chnum_to_remove++;
					prune_chanlist = TRUE;
				}
			} else if (prune_type == ROAM_PRUNE_NON_APBAND_CHANNELS) {
				/* Removing the Non AP band channels and AP operating channels */
				if (CHSPEC_BAND(ap_chspec) != CHSPEC_BAND(*chspec_list) ||
					ap_chspec == *chspec_list) {
					/* set the chanspec to zero to truncate. */
					*chspec_list = 0;
					chnum_to_remove++;
					prune_chanlist = TRUE;
				}
			}
			chspec_list++;
		}
	}

	chspec_list = ch_list;
	if (prune_chanlist) {
		chanidx = 0;
		sync_idx = 0;
		while (chanidx < num) {
			if (*chspec_list != 0) {
				*(ch_list + sync_idx) = *chspec_list;
				sync_idx++;
			}
			chspec_list++;
			chanidx++;
		}
	}

	/* return the channel number remaining after truncation */
	return (num - chnum_to_remove);
}

/* Find if prune will applied based on roam->prune_type.
 * input: Roam Chanspec list. (RCC list)
 * num: Number of channels.
 * Return Value:
 * Number of channels remaining after pruning depending on the prune type
 */
int
wlc_scan_filter_channels(wlc_scan_info_t *scan_pub, chanspec_t *chspec_list, int num)
{
	scan_info_t *scan = (scan_info_t*) scan_pub->scan_priv;
	bool pruned_scan;

	/* return the exact number of channels that are received */
	int ret = num;

	if (chspec_list == NULL || !num)
		goto end;

	pruned_scan = (RSDB_APSCAN_ENAB(SCAN_WLC(scan)->pub) &&
			wlc_rsdb_any_aps_active(SCAN_WLC(scan)) &&
			!wlc_rsdb_any_go_active(SCAN_WLC(scan)));

	if (pruned_scan == FALSE)
		goto end;

	ret = wlc_scan_filter_ap_band_channels(scan, chspec_list, num);

end:
	return ret;
}
#endif /* RSDB_APSCAN */
