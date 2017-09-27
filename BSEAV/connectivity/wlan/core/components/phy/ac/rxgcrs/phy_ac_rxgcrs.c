/*
 * ACPHY Rx Gain Control and Carrier Sense module implementation
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

#include <phy_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <phy_dbg.h>
#include <phy_mem.h>
#include "phy_type_rxgcrs.h"
#include <phy_utils_var.h>
#include <phy_ac.h>
#include <phy_ac_rxgcrs.h>
#include <phy_ac_btcx.h>
#include <phy_ac_fcbs.h>
#include <phy_ac_nap.h>
#include <phy_ac_noise.h>
#include <phy_ac_rssi.h>
#include <phy_rxgcrs_api.h>
#include <phy_noise_api.h>
#include <phy_misc_api.h>

/* ************************ */
/* Modules used by this module */
/* ************************ */
#include <wlc_radioreg_20691.h>
#include <wlc_radioreg_20693.h>
#include <wlc_radioreg_20694.h>
#include <wlc_radioreg_20695.h>
#include <wlc_radioreg_20696.h>
#include <wlc_phy_radio.h>
#include <wlc_phyreg_ac.h>

#include <phy_utils_reg.h>
#include <phy_ac_info.h>
#include <phy_ac_nap.h>
#include <phy_stf.h>

#include <hndpmu.h>
#include <bcmdevs.h>
#include <phy_rstr.h>

/* Rx Gain Control params */
typedef struct {
	int8  lna12_gain_2g_tbl[2][N_LNA12_GAINS];
	int8  lna12_gain_5g_tbl[2][N_LNA12_GAINS];
	uint8 lna12_gainbits_2g_tbl[2][N_LNA12_GAINS];
	uint8 lna12_gainbits_5g_tbl[2][N_LNA12_GAINS];
	uint8 lna1Rout_2g_tbl[N_LNA12_GAINS];
	uint8 lna1Rout_5g_tbl[N_LNA12_GAINS];
	uint8 lna2_gm_ind_2g_tbl[N_LNA12_GAINS];
	uint8 lna2_gm_ind_5g_tbl[N_LNA12_GAINS];
	int8 gainlimit_tbl[RXGAIN_CONF_ELEMENTS][MAX_RX_GAINS_PER_ELEM];
	int8 tia_gain_tbl[N_TIA_GAINS];
	int8 tia_gainbits_tbl[N_TIA_GAINS];
	int8 biq01_gain_tbl[2][N_BIQ01_GAINS];
	int8 biq01_gainbits_tbl[2][N_BIQ01_GAINS];
	int8 fast_agc_clip_gains[4];
	int16 maxgain[ACPHY_MAX_RX_GAIN_STAGES];
	uint8 lna1_min_indx;
	uint8 lna1_max_indx;
	uint8 lna1_byp_indx;
	uint8 lna2_min_indx;
	uint8 lna2_max_indx;
	uint8 lna2_2g_min_indx;
	uint8 lna2_5g_min_indx;
	uint8 lna2_2g_max_indx;
	uint8 lna2_5g_max_indx;
	uint8 farrow_shift;
	uint8 tia_max_indx;
	uint8 max_analog_gain;
	int8 lna1_byp_gain_2g;
	int8 lna1_byp_gain_5g;
	uint8 lna1_byp_rout_2g;
	uint8 lna1_byp_rout_5g;
	int8 lna1_sat_2g;
	int8 lna1_sat_5g;
	uint8 high_sen_adjust_2g;
	uint8 high_sen_adjust_5g;

	/* SSAGC Specific */
	int8 ssagc_clip_gains[SSAGC_CLIP1_GAINS_CNT];
	uint8 ssagc_clip2_tbl[SSAGC_CLIP2_TBL_IDX_CNT];
	uint8 ssagc_rssi_thresh_2g[SSAGC_N_RSSI_THRESH];
	uint8 ssagc_rssi_thresh_5g[SSAGC_N_RSSI_THRESH];
	uint8 ssagc_clipcnt_thresh_2g[SSAGC_N_CLIPCNT_THRESH];
	uint8 ssagc_clipcnt_thresh_5g[SSAGC_N_CLIPCNT_THRESH];
	bool ssagc_lna1byp_flag[SSAGC_CLIP1_GAINS_CNT];
	bool ssagc_en;

	/* MclipAGC */
	bool mclip_agc_en;

	/* SSAGC related */
	uint8 ssagc_lna_byp_gain_thresh;

	/* Low NF ELNA mode */
	int8 fast_agc_lonf_clip_gains[4];
	uint8 lna2_lonf_force_indx_2g;
	uint8 lna2_lonf_force_indx_5g;
	uint8 lna1_lonf_max_indx;
	uint8 max_analog_gain_lonf;
	uint8 high_sen_adjust_lonf_2g;
	uint8 high_sen_adjust_lonf_5g;

	int8 lna1_sat_2g_elna;
	int8 lna1_sat_5g_elna;
} phy_ac_rxg_params_t;

/* Rx Gain Control params table ID's */
typedef enum {
	LNA12_GAIN_TBL_2G,
	LNA12_GAIN_TBL_5G,
	LNA12_GAIN_BITS_TBL_2G,
	LNA12_GAIN_BITS_TBL_5G,
	TIA_GAIN_TBL,
	TIA_GAIN_BITS_TBL,
	BIQ01_GAIN_TBL,
	BIQ01_GAIN_BITS_TBL,
	GAIN_LIMIT_TBL,
	LNA1_ROUTMAP_TBL_2G,
	LNA1_ROUTMAP_TBL_5G,
	LNA1_GAINMAP_TBL_2G,
	LNA1_GAINMAP_TBL_5G,
	LNA2_GAINMAP_TBL_2G,
	LNA2_GAINMAP_TBL_5G,
	MAX_GAIN_TBL,
	FAST_AGC_CLIP_GAIN_TBL,
	SSAGC_CLIP_GAIN_TBL,
	SSAGC_LNA1BYP_TBL,
	SSAGC_CLIP2_TBL,
	SSAGC_CLIPCNT_THRESH_TBL_2G,
	SSAGC_RSSI_THRESH_TBL_2G,
	SSAGC_CLIPCNT_THRESH_TBL_5G,
	SSAGC_RSSI_THRESH_TBL_5G,
	FAST_AGC_LONF_CLIP_GAIN_TBL
} phy_ac_rxg_param_tbl_t;

/* module private states */
struct phy_ac_rxgcrs_info {
	phy_info_t *pi;
	phy_ac_info_t *aci;
	phy_rxgcrs_info_t *cmn_info;
	acphy_desense_values_t *lte_desense, *bt_desense;
	acphy_desense_values_t	*curr_desense, *zero_desense, *total_desense;
	acphy_fem_rxgains_t *fem_rxgains; /* Array of size PHY_CORE_MAX */
	acphy_rxgainctrl_t *rxgainctrl_params; /* Array of size PHY_CORE_MAX */
	phy_ac_rxg_params_t *rxg_params;
	int16	rxgainctrl_maxout_gains[ACPHY_MAX_RX_GAIN_STAGES];
	uint16	clip1_th, edcrs_en;
	uint16	initGain_codeA;
	uint16	initGain_codeB;
	uint16	deaf_count;
	uint8	phy_crs_th_from_crs_cal;
	uint8	rxgainctrl_stage_len[ACPHY_MAX_RX_GAIN_STAGES];
	int8	phy_noise_cache_crsmin[PHY_SIZE_NOISE_CACHE_ARRAY][PHY_CORE_MAX];
	int8	rx_elna_bypass_gain_th[PHY_CORE_MAX];
	int8	phy_noise_in_crs_min[PHY_CORE_MAX];	/* noise power in dB for all cores */
	int8	phy_noise_pwr_array[PHY_SIZE_NOISE_ARRAY][PHY_CORE_MAX];
	int8	phy_noise_counter;	/* Dummy variable for noise averaging */
	uint8	phy_debug_crscal_counter;
	uint8	phy_debug_crscal_channel;
	/* Asymmetric AWGN noise jammer fix */
	uint8	srom_asymmetricjammermod;
	int8	lna2_complete_gaintbl[12];
	uint8	crsmincal_run;
	/* Parameter to enable subband_cust in 43012A0 */
	int8	enable_subband_cust;
	uint8	w1clipmod;
	bool	thresh_noise_cal; /* enable/disable additional entries in noise cal table */
	bool	crsmincal_enable;		/* Flag for enabling auto crsminpower cal */
	bool	force_crsmincal;
	bool	mdgain_trtx_allowed;
	/* lesi */
	int8 lesi;
	bool lesi_mode;
	bool tia_idx_max_eq_init;
	/* Flag for enabling auto lesiscale cal */
	bool lesiscalecal_enable;
	uint8 lonf_elna_mode; /* Flag for enabling LOW NF ELNA mode */
};

static const char BCMATTACHDATA(rstr_subband_cust)[]            = "subband_cust";
static const char BCMATTACHDATA(rstr_ssagc_en)[]                = "ssagc_en";
static const char BCMATTACHDATA(rstr_rxgains2gelnagainaD)[]     = "rxgains2gelnagaina%d";
static const char BCMATTACHDATA(rstr_rxgains2gtrelnabypaD)[]    = "rxgains2gtrelnabypa%d";
static const char BCMATTACHDATA(rstr_rxgains2gtrisoaD)[]        = "rxgains2gtrisoa%d";
static const char BCMATTACHDATA(rstr_rxgains5gelnagainaD)[]     = "rxgains5gelnagaina%d";
static const char BCMATTACHDATA(rstr_rxgains5gtrelnabypaD)[]    = "rxgains5gtrelnabypa%d";
static const char BCMATTACHDATA(rstr_rxgains5gtrisoaD)[]        = "rxgains5gtrisoa%d";
static const char BCMATTACHDATA(rstr_rxgains5gmelnagainaD)[]    = "rxgains5gmelnagaina%d";
static const char BCMATTACHDATA(rstr_rxgains5gmtrelnabypaD)[]   = "rxgains5gmtrelnabypa%d";
static const char BCMATTACHDATA(rstr_rxgains5gmtrisoaD)[]       = "rxgains5gmtrisoa%d";
static const char BCMATTACHDATA(rstr_rxgains5ghelnagainaD)[]    = "rxgains5ghelnagaina%d";
static const char BCMATTACHDATA(rstr_rxgains5ghtrelnabypaD)[]   = "rxgains5ghtrelnabypa%d";
static const char BCMATTACHDATA(rstr_rxgains5ghtrisoaD)[]       = "rxgains5ghtrisoa%d";
static const char BCMATTACHDATA(rstr_lonf_elna_mode)[] 			= "lonf_elna_mode";

static const uint8 tiny4365_g_lna_rout_map[] = {9, 9, 9, 9, 6, 0};
static const uint8 tiny4365_g_lna_gain_map[] = {2, 3, 4, 5, 5, 5};
static const uint8 tiny4365_a_lna_rout_map[] = {9, 9, 9, 9, 6, 0};
static const uint8 tiny4365_a_lna_gain_map[] = {4, 5, 6, 7, 7, 7};
static const uint8 ac_tia_gain_tiny_4365[] = {10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 37, 37, 37};
static const uint8 ac_tia_gainbits_tiny_4365[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9};

/* local functions */
static void phy_ac_rxgcrs_std_params_attach(phy_ac_rxgcrs_info_t *info);
static int phy_ac_rxgcrs_init(phy_type_rxgcrs_ctx_t *ctx);
static void phy_ac_rxgcrs_set_locale(phy_type_rxgcrs_ctx_t *ctx, uint8 region_group);
static void phy_ac_upd_lna1_bypass(phy_info_t *pi, uint8 core);
static int8 wlc_phy_rxgainctrl_encode_pktgain_acphy(phy_info_t *pi, uint8 core, int8 gain_dB,
                                     bool trloss, bool lna1byp, uint8 *gidx);
static void BCMATTACHFN(wlc_phy_srom_read_gainctrl_acphy)(phy_ac_rxgcrs_info_t *rxgcrs_info);
static int BCMATTACHFN(phy_ac_populate_rxg_params)(phy_ac_rxgcrs_info_t *rxgcrs_info);
static void phy_ac_single_shot_agc(phy_info_t *pi, bool init, bool band_change, bool bw_change);
static void phy_ac_ssagc_rxgainctrl(phy_info_t *pi, uint8 core);
static void phy_ac_populate_ssagc_clipgain_tables(phy_info_t *pi, uint32 *rssi_clip_gains,
		uint8 core);
static uint32 phy_ac_ssgac_pack_gains(phy_info_t *pi, uint8 *gain_idx, bool trtx, bool lna1byp,
		bool trrx);
static void phy_ac_ssagc_control_config(phy_info_t *pi);
static void phy_ac_set_ssagc_RSSI_detectors(phy_info_t *pi, uint8 core, uint8 *thresholds);
static void phy_ac_set_ssagc_clipcnt_thresh(phy_info_t *pi, uint8 core, uint8 *thresholds);
static void phy_ac_ssagc_rssi_setup(phy_info_t *pi, uint8 core, bool init, bool band_change,
		bool bw_change);
static void phy_ac_fastagc_config(phy_info_t *pi);
static void phy_ac_ssagc_config(phy_info_t *pi, bool init, bool band_change,
		bool bw_change);
static void* BCMRAMFN(phy_ac_get_rxg_param_tbl)(phy_info_t *pi, phy_ac_rxg_param_tbl_t tbl_id,
		uint8 ind_2d_tbl);
static void wlc_phy_adjust_ed_thres_acphy(phy_type_rxgcrs_ctx_t * ctx, int32 *assert_thresh_dbm,
	bool set_threshold);
#if defined(BCMINTPHYDBG) || defined(WLTEST) || defined(DBG_PHY_IOV) || \
	defined(WFD_PHY_LL_DEBUG)
static int wlc_phy_noisecal_run_acphy(phy_type_rxgcrs_ctx_t *ctx, void *a, bool set);
#endif /* BCMINTPHYDBG || WLTEST || DBG_PHY_IOV || WFD_PHY_LL_DEBUG */
static rxgain_ovrd_t rxgainindx_cmd_ovrd[PHY_CORE_MAX];

static void wlc_phy_mclip_agc(phy_info_t *pi, bool init, bool band_change, bool bw_change);
static void wlc_phy_mclip_agc_rssi_setup(phy_info_t *pi, bool init, bool band_change,
		bool bw_change,	int8 mclip_rssi[][10]);
static void wlc_phy_mclip_agc_rxgainctrl(phy_info_t *pi, int8 mclip_rssi[][10]);
static void wlc_phy_mclip_agc_clipcnt_thresh(phy_info_t *pi, uint8 core,
		const uint8 thresh_list[]);
static uint8 wlc_phy_mclip_agc_calc_lna_bypass_rssi(phy_info_t *pi, int8 *mclip_rssi,
		int8 sens_lna1Byp_lo, int8 sens_hi);
static uint32 wlc_phy_mclip_agc_pack_gains(phy_info_t *pi, uint8 *gain_indx, uint8 trtx,
		uint8 lna1byp);
#if defined(RXDESENS_EN)
static int phy_ac_rxgcrs_get_rxdesens(phy_type_rxgcrs_ctx_t *ctx, int32 *ret_int_ptr);
static int phy_ac_rxgcrs_set_rxdesens(phy_type_rxgcrs_ctx_t *ctx, int32 int_val);
#endif /* defined(RXDESENS_EN) */
static int acphy_get_rxgain_index(phy_type_rxgcrs_ctx_t *ctx, int32 *index);
static int acphy_set_rxgain_index(phy_type_rxgcrs_ctx_t *ctx, int32 index);
static void wlc_phy_edcrs_thresh_acphy(phy_info_t *pi);
static uint16 phy_ac_rxgcrs_sel_classifier(phy_type_rxgcrs_ctx_t *ctx, uint16 val);
static bool phy_ac_rxgcrs_wd(phy_wd_ctx_t *ctx);
static void
phy_ac_rxgcrs_stay_in_carriersearch(phy_type_rxgcrs_ctx_t *ctx, bool enable);
#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static void phy_ac_rxgcrs_phydump_chanest(phy_type_rxgcrs_ctx_t *ctx,
	struct bcmstrbuf *b);
#endif /* defined(BCMDBG) || defined(BCMDBG_DUMP) */
#ifdef WLTEST
static void
phy_ac_rxgcrs_get_chanest(phy_type_rxgcrs_ctx_t *ctx, uint16 fftk, uint16 idx,
	int16 *ch_re, int16 *ch_im);
#endif /* WLTEST */
#if defined(DBG_BCN_LOSS)
static int
phy_ac_rxgcrs_dump_phycal_rx_min(phy_type_rxgcrs_ctx_t *ctx, struct bcmstrbuf *b);
#endif /* DBG_BCN_LOSS */

extern char *nvram_get(const char *name);

/* register phy type specific implementation */
phy_ac_rxgcrs_info_t *
BCMATTACHFN(phy_ac_rxgcrs_register_impl)(phy_info_t *pi, phy_ac_info_t *aci,
	phy_rxgcrs_info_t *cmn_info)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info;
	phy_type_rxgcrs_fns_t fns;

	PHY_TRACE(("%s\n", __FUNCTION__));

	/* allocate all storage together */
	if ((rxgcrs_info = phy_malloc(pi, sizeof(phy_ac_rxgcrs_info_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->lte_desense = phy_malloc(pi, sizeof(acphy_desense_values_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc lte_desense failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->bt_desense = phy_malloc(pi, sizeof(acphy_desense_values_t))) == NULL) {
		PHY_ERROR(("%s: bt_desense malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->curr_desense = phy_malloc(pi, sizeof(acphy_desense_values_t))) == NULL) {
		PHY_ERROR(("%s: curr_desense malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->zero_desense = phy_malloc(pi, sizeof(acphy_desense_values_t))) == NULL) {
		PHY_ERROR(("%s: zero_desense malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->total_desense = phy_malloc(pi, sizeof(acphy_desense_values_t))) == NULL) {
		PHY_ERROR(("%s: total_desense malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->fem_rxgains =
		phy_malloc(pi, sizeof(acphy_fem_rxgains_t) * PHY_CORE_MAX)) == NULL) {
		PHY_ERROR(("%s: fem_rxgains malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((rxgcrs_info->rxgainctrl_params =
		phy_malloc(pi, sizeof(acphy_rxgainctrl_t) * PHY_CORE_MAX)) == NULL) {
		PHY_ERROR(("%s: rxgainctrl_params malloc failed\n", __FUNCTION__));
		goto fail;
	}

	rxgcrs_info->pi = pi;
	rxgcrs_info->aci = aci;
	rxgcrs_info->cmn_info = cmn_info;

	/* register watchdog fn */
	if (phy_wd_add_fn(pi->wdi, phy_ac_rxgcrs_wd, rxgcrs_info,
		PHY_WD_PRD_1TICK, PHY_WD_1TICK_RXGCRS,
		PHY_WD_FLAG_NONE) != BCME_OK) {
		PHY_ERROR(("%s: phy_wd_add_fn failed\n", __FUNCTION__));
		goto fail;
	}

	/* register PHY type specific implementation */
	bzero(&fns, sizeof(fns));
	fns.ctx = rxgcrs_info;
	fns.init_rxgcrs = phy_ac_rxgcrs_init;
	fns.set_locale = phy_ac_rxgcrs_set_locale;
	fns.adjust_ed_thres = wlc_phy_adjust_ed_thres_acphy;
#if defined(RXDESENS_EN)
	fns.get_rxdesens = phy_ac_rxgcrs_get_rxdesens;
	fns.set_rxdesens = phy_ac_rxgcrs_set_rxdesens;
#endif /* defined(RXDESENS_EN) */
	fns.get_rxgainindex = acphy_get_rxgain_index;
	fns.set_rxgainindex = acphy_set_rxgain_index;
#if defined(BCMINTPHYDBG) || defined(WLTEST) || defined(DBG_PHY_IOV) || \
	defined(WFD_PHY_LL_DEBUG)
	fns.forcecal_noise = wlc_phy_noisecal_run_acphy;
#endif /* BCMINTPHYDBG || WLTEST || DBG_PHY_IOV || WFD_PHY_LL_DEBUG */
	fns.sel_classifier = phy_ac_rxgcrs_sel_classifier;
	fns.stay_in_carriersearch = phy_ac_rxgcrs_stay_in_carriersearch;
#if defined(BCMDBG) || defined(BCMDBG_DUMP)
	fns.phydump_chanest = phy_ac_rxgcrs_phydump_chanest;
#endif /* defined(BCMDBG) || defined(BCMDBG_DUMP) */
#ifdef WLTEST
	fns.get_chanest = phy_ac_rxgcrs_get_chanest;
#endif /* WLTEST */
#if defined(DBG_BCN_LOSS)
	fns.phydump_phycal_rxmin = phy_ac_rxgcrs_dump_phycal_rx_min;
#endif /* DBG_BCN_LOSS */

	/* Read rxgainctrl srom entries - elna gain, trloss */
	wlc_phy_srom_read_gainctrl_acphy(rxgcrs_info);

	if (phy_ac_populate_rxg_params(rxgcrs_info) != BCME_OK) {
		goto fail;
	}

	if (ACPHY_HWACI_HWTBL_MITIGATION(pi) &&
	    (ACMAJORREV_33(pi->pubpi->phy_rev) || ACMAJORREV_37(pi->pubpi->phy_rev))) {
		phy_ac_noise_hwaci_switching_regs_tbls_list_init(pi);
	}

	phy_ac_rxgcrs_std_params_attach(rxgcrs_info);

	if (phy_rxgcrs_register_impl(cmn_info, &fns) != BCME_OK) {
		PHY_ERROR(("%s: phy_rxgcrs_register_impl failed\n", __FUNCTION__));
		goto fail;
	}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
		/* Read lesimode from NVRAM for 2G (Aux slice) */
		if (wlapi_si_coreunit(pi->sh->physhim) == DUALMAC_AUX) {
			rxgcrs_info->lesi = (uint8)PHY_GETINTVAR_ARRAY_SLICE(pi,
				rstr_lesi_en, 0);
		}
		 /* Read lesimode from NVRAM for 5G (Main slice) */
		if (wlapi_si_coreunit(pi->sh->physhim) == DUALMAC_MAIN) {
			rxgcrs_info->lesi = (uint8)PHY_GETINTVAR_ARRAY_SLICE(pi,
				rstr_lesi_en, 1);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
	if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
		/* Need to initialize the *_idx_* values so gainlimit tbls are filled correctly */
		rxgcrs_info->zero_desense->lna1_idx_min = 0;
		rxgcrs_info->zero_desense->lna1_idx_max = 5;
		rxgcrs_info->zero_desense->lna2_idx_min = 3;
		rxgcrs_info->zero_desense->lna2_idx_max = 3;
		rxgcrs_info->zero_desense->mix_idx_min = 0;
		rxgcrs_info->zero_desense->mix_idx_max = 15;
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
	rxgcrs_info->enable_subband_cust = (uint8)PHY_GETINTVAR_DEFAULT(pi,
		rstr_subband_cust, 0);

	return rxgcrs_info;

	/* error handling */
fail:
	phy_ac_rxgcrs_unregister_impl(rxgcrs_info);
	return NULL;
}

void
BCMATTACHFN(phy_ac_rxgcrs_unregister_impl)(phy_ac_rxgcrs_info_t *rxgcrs_info)
{
	phy_info_t *pi;

	PHY_TRACE(("%s\n", __FUNCTION__));

	if (rxgcrs_info == NULL)
		return;

	pi = rxgcrs_info->pi;

	/* unregister from common */
	phy_rxgcrs_unregister_impl(rxgcrs_info->cmn_info);

	if (rxgcrs_info->rxg_params) {
		phy_mfree(pi, rxgcrs_info->rxg_params, sizeof(phy_ac_rxg_params_t));
	}

	if (rxgcrs_info->rxgainctrl_params) {
		phy_mfree(pi, rxgcrs_info->rxgainctrl_params,
		sizeof(acphy_rxgainctrl_t) * PHY_CORE_MAX);
	}

	if (rxgcrs_info->fem_rxgains) {
		phy_mfree(pi, rxgcrs_info->fem_rxgains, sizeof(acphy_fem_rxgains_t) * PHY_CORE_MAX);
	}

	if (rxgcrs_info->total_desense) {
		phy_mfree(pi, rxgcrs_info->total_desense, sizeof(acphy_desense_values_t));
	}
	if (rxgcrs_info->zero_desense) {
		phy_mfree(pi, rxgcrs_info->zero_desense, sizeof(acphy_desense_values_t));
	}
	if (rxgcrs_info->curr_desense) {
		phy_mfree(pi, rxgcrs_info->curr_desense, sizeof(acphy_desense_values_t));
	}
	if (rxgcrs_info->bt_desense) {
		phy_mfree(pi, rxgcrs_info->bt_desense, sizeof(acphy_desense_values_t));
	}
	if (rxgcrs_info->lte_desense) {
		phy_mfree(pi, rxgcrs_info->lte_desense, sizeof(acphy_desense_values_t));
	}
	phy_mfree(pi, rxgcrs_info, sizeof(phy_ac_rxgcrs_info_t));
}

/* inter-module data API */
bool
phy_ac_rxgcrs_get_md_trtx(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	return rxgcrsi->mdgain_trtx_allowed;
}

uint16
phy_ac_rxgcrs_get_deaf_count(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	return rxgcrsi->deaf_count;
}

static int
WLBANDINITFN(phy_ac_rxgcrs_init)(phy_type_rxgcrs_ctx_t *ctx)
{
	phy_ac_rxgcrs_info_t *rxgcrsi = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrsi->pi;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	uint8 region_group = wlc_phy_get_locale(pi->rxgcrsi);
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	if (pi->phy_init_por) {
		rxgcrsi->mdgain_trtx_allowed = FALSE;
		pi->interf->curr_home_channel = CHSPEC_CHANNEL(pi->radio_chanspec);
	}

	/* Sets Assert and Deassert thresholds for all 20MHz subbands for EDCRS */
	/* edcrs phyreg is shadowed.
	 * For shadowed reg/table, initialed values have to be put on both sets.
	 */
#ifdef ENABLE_FCBS
		if (IS_FCBS(pi)) {
			for (chanidx = 0; chanidx < MAX_FCBS_CHANS; chanidx++) {
				wlc_phy_prefcbsinit_acphy(pi, chanidx);
				wlc_phy_edcrs_thresh_acphy(pi);
			}
			wlc_phy_prefcbsinit_acphy(pi, 0);
		} else {
			wlc_phy_edcrs_thresh_acphy(pi);
		}
#else
		wlc_phy_edcrs_thresh_acphy(pi);
#endif

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if ((region_group ==  REGION_EU) && (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		ACMAJORREV_33(pi->pubpi->phy_rev)))
	    wlc_phy_set_srom_eu_edthresh_acphy(pi);
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	return BCME_OK;
}

static void
BCMATTACHFN(wlc_phy_srom_read_gainctrl_acphy)(phy_ac_rxgcrs_info_t *rxgcrs_info)
{
	phy_info_t *pi = rxgcrs_info->pi;
	uint8 core, srom_rx, ant;
	char srom_name[30];
	phy_info_acphy_t *pi_ac;
	uint8 raw_elna, raw_trloss, raw_bypass;
	uint8 i;
#ifndef BOARD_FLAGS3
	uint32 bfl3; /* boardflags3 */
#endif

	pi_ac = rxgcrs_info->aci;
	BCM_REFERENCE(pi_ac);

#ifndef BOARD_FLAGS
	BF_ELNA_2G(pi_ac) = (BOARDFLAGS(GENERIC_PHY_INFO(pi)->boardflags) &
	                              BFL_SROM11_EXTLNA) != 0;
	BF_ELNA_5G(pi_ac) = (BOARDFLAGS(GENERIC_PHY_INFO(pi)->boardflags) &
	                              BFL_SROM11_EXTLNA_5GHz) != 0;
#endif /* BOARD_FLAGS */

	FOREACH_CORE(pi, core) {
		ant = phy_get_rsdbbrd_corenum(pi, core);
		pi_ac->sromi->femrx_2g[ant].elna = 0;
		pi_ac->sromi->femrx_2g[ant].trloss = 0;
		pi_ac->sromi->femrx_2g[ant].elna_bypass_tr = 0;

		/*  -------  2G -------  */
		if (BF_ELNA_2G(pi_ac)) {
			snprintf(srom_name, sizeof(srom_name),  rstr_rxgains2gelnagainaD, ant);
			if (PHY_GETVAR(pi, srom_name) != NULL) {
				srom_rx = (uint8)PHY_GETINTVAR(pi, srom_name);
				pi_ac->sromi->femrx_2g[ant].elna = (2 * srom_rx) + 6;
			}

			snprintf(srom_name, sizeof(srom_name),  rstr_rxgains2gtrelnabypaD, ant);
			if (PHY_GETVAR(pi, srom_name) != NULL) {
				pi_ac->sromi->femrx_2g[ant].elna_bypass_tr =
				        (uint8)PHY_GETINTVAR(pi, srom_name);
			}
		}

		snprintf(srom_name, sizeof(srom_name),  rstr_rxgains2gtrisoaD, ant);
		if (PHY_GETVAR(pi, srom_name) != NULL) {
			srom_rx = (uint8)PHY_GETINTVAR(pi, srom_name);
			pi_ac->sromi->femrx_2g[ant].trloss = (2 * srom_rx) + 8;
		}

		if (PHY_BAND5G_ENAB(pi)) {
			pi_ac->sromi->femrx_5g[ant].elna = 0;
			pi_ac->sromi->femrx_5g[ant].trloss = 0;
			pi_ac->sromi->femrx_5g[ant].elna_bypass_tr = 0;

			pi_ac->sromi->femrx_5gm[ant].elna = 0;
			pi_ac->sromi->femrx_5gm[ant].trloss = 0;
			pi_ac->sromi->femrx_5gm[ant].elna_bypass_tr = 0;

			pi_ac->sromi->femrx_5gh[ant].elna = 0;
			pi_ac->sromi->femrx_5gh[ant].trloss = 0;
			pi_ac->sromi->femrx_5gh[ant].elna_bypass_tr = 0;

			/*  -------  5G -------  */
			if (BF_ELNA_5G(pi_ac)) {
				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5gelnagainaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL) {
					srom_rx = (uint8)PHY_GETINTVAR(pi, srom_name);
					pi_ac->sromi->femrx_5g[ant].elna = (2 * srom_rx) + 6;
				}

				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5gtrelnabypaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL) {
					pi_ac->sromi->femrx_5g[ant].elna_bypass_tr =
					        (uint8)PHY_GETINTVAR(pi, srom_name);
				}
			}

			snprintf(srom_name, sizeof(srom_name), rstr_rxgains5gtrisoaD, ant);
			if (PHY_GETVAR(pi, srom_name) != NULL) {
				srom_rx = (uint8)PHY_GETINTVAR(pi, srom_name);
				pi_ac->sromi->femrx_5g[ant].trloss = (2 * srom_rx) + 8;
			}

			/*  -------  5G (mid) -------  */
			raw_elna = 0; raw_trloss = 0; raw_bypass = 0;
			if (BF_ELNA_5G(pi_ac)) {
				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5gmelnagainaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL)
					raw_elna = (uint8)PHY_GETINTVAR(pi, srom_name);

				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5gmtrelnabypaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL)
					raw_bypass = (uint8)PHY_GETINTVAR(pi, srom_name);
			}
			snprintf(srom_name, sizeof(srom_name), rstr_rxgains5gmtrisoaD, ant);
			if (PHY_GETVAR(pi, srom_name) != NULL)
				raw_trloss = (uint8)PHY_GETINTVAR(pi, srom_name);

			if (((raw_elna == 0) && (raw_trloss == 0) && (raw_bypass == 0)) ||
			    ((raw_elna == 7) && (raw_trloss == 0xf) && (raw_bypass == 1))) {
				/* No entry in SROM, use generic 5g ones */
				pi_ac->sromi->femrx_5gm[ant].elna =
						pi_ac->sromi->femrx_5g[ant].elna;
				pi_ac->sromi->femrx_5gm[ant].elna_bypass_tr =
				        pi_ac->sromi->femrx_5g[ant].elna_bypass_tr;
				pi_ac->sromi->femrx_5gm[ant].trloss =
						pi_ac->sromi->femrx_5g[ant].trloss;
			} else {
				if (BF_ELNA_5G(pi_ac)) {
					pi_ac->sromi->femrx_5gm[ant].elna = (2 * raw_elna) + 6;
					pi_ac->sromi->femrx_5gm[ant].elna_bypass_tr = raw_bypass;
				}
				pi_ac->sromi->femrx_5gm[ant].trloss = (2 * raw_trloss) + 8;
			}

			/*  -------  5G (high) -------  */
			raw_elna = 0; raw_trloss = 0; raw_bypass = 0;
			if (BF_ELNA_5G(pi_ac)) {
				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5ghelnagainaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL)
					raw_elna = (uint8)PHY_GETINTVAR(pi, srom_name);

				snprintf(srom_name, sizeof(srom_name),
					rstr_rxgains5ghtrelnabypaD, ant);
				if (PHY_GETVAR(pi, srom_name) != NULL)
					raw_bypass = (uint8)PHY_GETINTVAR(pi, srom_name);
			}
			snprintf(srom_name, sizeof(srom_name), rstr_rxgains5ghtrisoaD, ant);
			if (PHY_GETVAR(pi, srom_name) != NULL)
				raw_trloss = (uint8)PHY_GETINTVAR(pi, srom_name);

			if (((raw_elna == 0) && (raw_trloss == 0) && (raw_bypass == 0)) ||
			    ((raw_elna == 7) && (raw_trloss == 0xf) && (raw_bypass == 1))) {
				/* No entry in SROM, use generic 5g ones */
				pi_ac->sromi->femrx_5gh[ant].elna =
						pi_ac->sromi->femrx_5gm[ant].elna;
				pi_ac->sromi->femrx_5gh[ant].elna_bypass_tr =
				        pi_ac->sromi->femrx_5gm[ant].elna_bypass_tr;
				pi_ac->sromi->femrx_5gh[ant].trloss =
						pi_ac->sromi->femrx_5gm[ant].trloss;
			} else {
				if (BF_ELNA_5G(pi_ac)) {
					pi_ac->sromi->femrx_5gh[ant].elna = (2 * raw_elna) + 6;
					pi_ac->sromi->femrx_5gh[ant].elna_bypass_tr = raw_bypass;
				}
				pi_ac->sromi->femrx_5gh[ant].trloss = (2 * raw_trloss) + 8;
			}
		}
	}

	pi_ac->sromi->dot11b_opts = (uint32)PHY_GETINTVAR_DEFAULT(pi, rstr_dot11b_opts, 1);

	if (PHY_GETVAR(pi, rstr_tiny_maxrxgain) != NULL) {
	  for (i = 0; i < 3; i++) {
	    pi_ac->sromi->tiny_maxrxgain[i] =
	    (uint8) PHY_GETINTVAR_ARRAY_DEFAULT(pi, rstr_tiny_maxrxgain, i, 0);
	  }
	} else {
	    for (i = 0; i < 3; i++) {
	      pi_ac->sromi->tiny_maxrxgain[i] = 0;
	    }
	}

	pi_ac->sromi->gainctrlsph = (bool)PHY_GETINTVAR(pi, rstr_gainctrlsph);

#ifndef BOARD_FLAGS3
	if ((PHY_GETVAR_SLICE(pi, rstr_boardflags3)) != NULL) {
		bfl3 = (uint32)PHY_GETINTVAR_SLICE(pi, rstr_boardflags3);
		BF3_LTECOEX_GAINTBL_EN(pi_ac) = (bfl3 & BFL3_LTECOEX_GAINTBL_EN) >>
			BFL3_LTECOEX_GAINTBL_EN_SHIFT;
	} else {
		BF3_LTECOEX_GAINTBL_EN(pi_ac) = 0;
	}
#endif /* BOARD_FLAGS3 */

	rxgcrs_info->w1clipmod = (uint8)PHY_GETINTVAR_DEFAULT(pi, rstr_w1clipmod, 0);
	rxgcrs_info->srom_asymmetricjammermod = (uint8)PHY_GETINTVAR_DEFAULT_SLICE
		(pi, rstr_asymmetricjammermod, 0);
	rxgcrs_info->thresh_noise_cal = (bool)PHY_GETINTVAR_DEFAULT(pi, rstr_thresh_noise_cal, 1);

	/* Low noise figure ELNA mode control */
	rxgcrs_info->lonf_elna_mode = (uint8)PHY_GETINTVAR_DEFAULT(pi, rstr_lonf_elna_mode, 0x0);
}

/* watchdog callback */
static bool
phy_ac_rxgcrs_wd(phy_wd_ctx_t *ctx)
{
	phy_ac_rxgcrs_info_t *rxgcrsi = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrsi->pi;

	/* Enabling crsmin_cal from watchdog. crsmin phyregs are only updated if the
	 * (current noise power - prev value from cache) is above certain threshold
	 */
	if (rxgcrsi->crsmincal_enable || (rxgcrsi->lesiscalecal_enable &&
		(ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev) ||
			ACMAJORREV_40(pi->pubpi->phy_rev)))) {
		phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
	}
	return TRUE;
}

static int
BCMATTACHFN(phy_ac_populate_rxg_params)(phy_ac_rxgcrs_info_t *rxgcrs_info)
{
	phy_ac_rxg_params_t *rxg_params;
	phy_info_t *pi;
	uint8 i;
	uint8 *lna1_rout_map_2g, *lna1_rout_map_5g;
	uint8 *lna1_gain_map_2g, *lna1_gain_map_5g;

	pi = rxgcrs_info->pi;

	if ((rxg_params = phy_malloc(pi, sizeof(phy_ac_rxg_params_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if (IS_28NM_RADIO(pi)) {
		/* Copy LNA12 gain params to structure element */
		for (i = 0; i < 2; i++) {
			memcpy(rxg_params->lna12_gain_2g_tbl[i],
					phy_ac_get_rxg_param_tbl(pi, LNA12_GAIN_TBL_2G, i),
					sizeof(int8) * N_LNA12_GAINS);
			memcpy(rxg_params->lna12_gainbits_2g_tbl[i],
					phy_ac_get_rxg_param_tbl(pi, LNA12_GAIN_BITS_TBL_2G, i),
					sizeof(int8) * N_LNA12_GAINS);

			/* If A band supported */
			if (PHY_BAND5G_ENAB(pi)) {
				memcpy(rxg_params->lna12_gain_5g_tbl[i],
						phy_ac_get_rxg_param_tbl(pi, LNA12_GAIN_TBL_5G, i),
						sizeof(int8) * N_LNA12_GAINS);
				memcpy(rxg_params->lna12_gainbits_5g_tbl[i],
					phy_ac_get_rxg_param_tbl(pi, LNA12_GAIN_BITS_TBL_5G, i),
					sizeof(int8) * N_LNA12_GAINS);
			}
		}

		/* Copy gain limit table */
		for (i = 0; i < RXGAIN_CONF_ELEMENTS; i++) {
			memcpy(rxg_params->gainlimit_tbl[i],
					phy_ac_get_rxg_param_tbl(pi, GAIN_LIMIT_TBL, i),
					sizeof(int8) * MAX_RX_GAINS_PER_ELEM);
		}

		/* LNA1 Rout table (2G) */
		lna1_rout_map_2g = phy_ac_get_rxg_param_tbl(pi, LNA1_ROUTMAP_TBL_2G, 0);
		lna1_gain_map_2g = phy_ac_get_rxg_param_tbl(pi, LNA1_GAINMAP_TBL_2G, 0);
		for (i = 0; i < N_LNA12_GAINS; i++) {
			rxg_params->lna1Rout_2g_tbl[i] = (lna1_rout_map_2g[i] << 3) |
					lna1_gain_map_2g[i];
		}

		/* LNA1 Rout table (5G), if A band supported */
		if (PHY_BAND5G_ENAB(pi)) {
			lna1_rout_map_5g = phy_ac_get_rxg_param_tbl(pi, LNA1_ROUTMAP_TBL_5G, 0);
			lna1_gain_map_5g = phy_ac_get_rxg_param_tbl(pi, LNA1_GAINMAP_TBL_5G, 0);

			for (i = 0; i < N_LNA12_GAINS; i++) {
				rxg_params->lna1Rout_5g_tbl[i] = (lna1_rout_map_5g[i] << 3) |
					lna1_gain_map_5g[i];
			}
		}

		/* TIA Gain and GainBits table */
		memcpy(rxg_params->tia_gain_tbl,
				phy_ac_get_rxg_param_tbl(pi, TIA_GAIN_TBL, 0),
				sizeof(int8) * N_TIA_GAINS);
		memcpy(rxg_params->tia_gainbits_tbl,
				phy_ac_get_rxg_param_tbl(pi, TIA_GAIN_BITS_TBL, 0),
				sizeof(int8) * N_TIA_GAINS);

		/* Copy BIQ01 gain params to structure element */
		for (i = 0; i < 2; i++) {
			memcpy(rxg_params->biq01_gain_tbl[i],
				phy_ac_get_rxg_param_tbl(pi, BIQ01_GAIN_TBL, i),
				sizeof(int8) * N_BIQ01_GAINS);
			memcpy(rxg_params->biq01_gainbits_tbl[i],
				phy_ac_get_rxg_param_tbl(pi, BIQ01_GAIN_BITS_TBL, i),
				sizeof(int8) * N_BIQ01_GAINS);
		}

	}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		/* LNA2 Gm table */
		memcpy(rxg_params->lna2_gm_ind_2g_tbl,
				phy_ac_get_rxg_param_tbl(pi, LNA2_GAINMAP_TBL_2G, 0),
				sizeof(int8) * N_LNA12_GAINS);

		if (PHY_BAND5G_ENAB(pi)) {
			memcpy(rxg_params->lna2_gm_ind_5g_tbl,
				phy_ac_get_rxg_param_tbl(pi, LNA2_GAINMAP_TBL_5G, 0),
				sizeof(int8) * N_LNA12_GAINS);
		}

		/* Max gain table */
		memcpy(rxg_params->maxgain, phy_ac_get_rxg_param_tbl(pi, MAX_GAIN_TBL, 0),
				sizeof(int16) * ACPHY_MAX_RX_GAIN_STAGES);

		/* Fast AGC clip gains */
		memcpy(rxg_params->fast_agc_clip_gains,
				phy_ac_get_rxg_param_tbl(pi, FAST_AGC_CLIP_GAIN_TBL, 0),
				sizeof(int8) * 4);

		rxg_params->lna1_min_indx = 0;
		rxg_params->lna1_max_indx = ACPHY_MAX_LNA1_IDX;
		rxg_params->lna2_min_indx = 1;
		rxg_params->lna2_max_indx = ACPHY_20695_MAX_LNA2_IDX;
		rxg_params->lna2_2g_min_indx = 1;
		rxg_params->lna2_5g_min_indx = 2;
		rxg_params->lna2_2g_max_indx = ACPHY_20695_MAX_LNA2_IDX;
		rxg_params->lna2_5g_max_indx = 3;
		rxg_params->lna1_byp_indx = 6;
		rxg_params->farrow_shift = 2;
		rxg_params->tia_max_indx = 9;
		rxg_params->max_analog_gain = MAX_ANALOG_RX_GAIN_28NM_ULP;
		rxg_params->lna1_byp_gain_2g = LNA1_BYPASS_GAIN_2G_20695;
		rxg_params->lna1_byp_gain_5g = LNA1_BYPASS_GAIN_5G_20695;
		rxg_params->lna1_byp_rout_2g = LNA1_BYPASS_ROUT_2G_20695;
		rxg_params->lna1_byp_rout_5g = LNA1_BYPASS_ROUT_5G_20695;
		rxg_params->lna1_sat_2g = -12;
		rxg_params->lna1_sat_5g = -12;
		rxg_params->lna1_sat_2g_elna = -12;
		rxg_params->lna1_sat_5g_elna = -8;
		rxg_params->high_sen_adjust_2g = 14;
		rxg_params->high_sen_adjust_5g = 15;

		/* Single shot specific configs */
		memcpy(rxg_params->ssagc_clip_gains,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_CLIP_GAIN_TBL, 0),
				SSAGC_CLIP1_GAINS_CNT * sizeof(int8));
		memcpy(rxg_params->ssagc_lna1byp_flag,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_LNA1BYP_TBL, 0),
				SSAGC_CLIP1_GAINS_CNT * sizeof(bool));
		memcpy(rxg_params->ssagc_clip2_tbl,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_CLIP2_TBL, 0),
				SSAGC_CLIP2_TBL_IDX_CNT * sizeof(uint8));
		memcpy(rxg_params->ssagc_rssi_thresh_2g,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_RSSI_THRESH_TBL_2G, 0),
				SSAGC_N_RSSI_THRESH * sizeof(uint8));
		memcpy(rxg_params->ssagc_clipcnt_thresh_2g,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_CLIPCNT_THRESH_TBL_2G, 0),
				SSAGC_N_CLIPCNT_THRESH * sizeof(uint8));

		if (PHY_BAND5G_ENAB(pi)) {
			memcpy(rxg_params->ssagc_rssi_thresh_5g,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_RSSI_THRESH_TBL_5G, 0),
				SSAGC_N_RSSI_THRESH * sizeof(uint8));
			memcpy(rxg_params->ssagc_clipcnt_thresh_5g,
				phy_ac_get_rxg_param_tbl(pi, SSAGC_CLIPCNT_THRESH_TBL_5G, 0),
				SSAGC_N_CLIPCNT_THRESH * sizeof(uint8));
		}

		rxg_params->ssagc_lna_byp_gain_thresh = 15;

		/* Low noise figure ELNA mode */
		memcpy(rxg_params->fast_agc_lonf_clip_gains,
				phy_ac_get_rxg_param_tbl(pi, FAST_AGC_LONF_CLIP_GAIN_TBL, 0),
				sizeof(int8) * 4);
		rxg_params->lna2_lonf_force_indx_2g = 1;
		rxg_params->lna2_lonf_force_indx_5g = 1;
		rxg_params->lna1_lonf_max_indx = 5;
		rxg_params->max_analog_gain_lonf = MAX_ANALOG_RX_GAIN_28NM_ULP_LONF;
		rxg_params->high_sen_adjust_lonf_2g = 14;
		rxg_params->high_sen_adjust_lonf_5g = 14;
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
	if (IS_28NM_RADIO(pi)) {
		/*
		if (CHSPEC_IS2G(pi->radio_chanspec) ? BF_ELNA_2G(pi->u.pi_acphy)
				: BF_ELNA_5G(pi->u.pi_acphy))
			rxg_params->lna1_max_indx = 5;
		else
			rxg_params->lna1_max_indx = 5;
		*/
		rxg_params->lna1_max_indx = 5;

		rxg_params->lna1_byp_gain_2g = -12;
		rxg_params->lna1_byp_gain_5g = -5;
		rxg_params->lna1_byp_rout_2g = 10;
		rxg_params->lna1_byp_rout_5g = 8;
		rxg_params->lna1_byp_indx = 6;

		rxg_params->lna1_min_indx = 0;
		rxg_params->lna2_min_indx = 3;
		rxg_params->lna2_max_indx = 3;
		rxg_params->lna2_2g_min_indx = 3;
		rxg_params->lna2_2g_max_indx = 3;
		rxg_params->lna2_5g_min_indx = 3;
		rxg_params->lna2_5g_max_indx = 3;
		/* mclip agc is default to on for 28nm radio's */
		rxg_params->mclip_agc_en = (bool)PHY_GETINTVAR_DEFAULT(pi, "mclip_agc_en", 1);
	}

	/* SSAGC enable control */
	rxg_params->ssagc_en = (bool)PHY_GETINTVAR_DEFAULT(pi, rstr_ssagc_en, 0);

	/* setup ptr */
	rxgcrs_info->rxg_params = rxg_params;

	return (BCME_OK);

fail:
	if (rxg_params != NULL) {
		phy_mfree(pi, rxg_params, sizeof(phy_ac_rxg_params_t));
	}

	return (BCME_NOMEM);
}

static void
BCMATTACHFN(phy_ac_rxgcrs_std_params_attach)(phy_ac_rxgcrs_info_t *ri)
{
	uint8 gain_len[] = {2, 6, 7, 10, 8, 8, 11}; /* elna, lna1, lna2, mix, bq0, bq1, dvga */
	uint8 i, core_num;
	phy_info_t *pi = ri->pi;
	char *val = NULL;
	uint lesidisab = 0;

	if (TINY_RADIO(pi)) {
		gain_len[3] = 12; /* tia */
		gain_len[5] = 3;  /* farrow */
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		ACMAJORREV_33(pi->pubpi->phy_rev)) {
		gain_len[TIA_ID] = 13;
		gain_len[BQ1_ID] = 3;
		gain_len[DVGA_ID] = 15;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	if (IS_28NM_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			gain_len[2] = 4; /* lna2 */
			gain_len[3] = 12; /* tia */
			gain_len[4] = 6; /* BIQ0 */
			gain_len[5] = 6; /* BIQ1 - Not present */
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
		{
			gain_len[2] = 4;
			gain_len[3] = 16;
			gain_len[4] = 6;
			gain_len[5] = 3; /* BIQ1 - Not present */
		}
	}
	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
		ri->rxgainctrl_stage_len[i] = gain_len[i];

	ri->crsmincal_enable = TRUE;
	ri->force_crsmincal  = FALSE;
	ri->crsmincal_run = 0;
	ri->phy_crs_th_from_crs_cal = ACPHY_CRSMIN_DEFAULT;
	/* default clip1_th & edcrs_en */
	ri->clip1_th = 0x404e;
	ri->edcrs_en = 0xfff;
	bzero((uint8 *)ri->phy_noise_pwr_array, sizeof(ri->phy_noise_pwr_array));
	bzero((uint8 *)ri->phy_noise_in_crs_min, sizeof(ri->phy_noise_in_crs_min));
	ri->phy_debug_crscal_counter = 0;
	ri->phy_debug_crscal_channel = 0;
	ri->phy_noise_counter = 0;
	ri->lesiscalecal_enable =  TRUE;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0))
	if (ACMAJORREV_0(pi->pubpi->phy_rev)) {
		uint8 subband_num;
		for (subband_num = 0; subband_num <= 4; subband_num++)
			FOREACH_CORE(pi, core_num)
				ri->phy_noise_cache_crsmin[subband_num][core_num] = -30;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0)) */
	{
		FOREACH_CORE(pi, core_num) {
			ri->phy_noise_cache_crsmin[0][core_num] = -30;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_4)))
			if (ACMAJORREV_2(pi->pubpi->phy_rev) ||
			(!(BF_ELNA_5G(ri->aci)) && ACMAJORREV_4(pi->pubpi->phy_rev))) {
				ri->phy_noise_cache_crsmin[1][core_num] = -32;
				ri->phy_noise_cache_crsmin[2][core_num] = -32;
				ri->phy_noise_cache_crsmin[3][core_num] = -31;
				ri->phy_noise_cache_crsmin[4][core_num] = -31;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_4))) */
			{
				ri->phy_noise_cache_crsmin[1][core_num] = -28;
				ri->phy_noise_cache_crsmin[2][core_num] = -28;
				ri->phy_noise_cache_crsmin[3][core_num] = -26;
				ri->phy_noise_cache_crsmin[4][core_num] = -25;
			}
		}
	}
	/* lesi */
	ri->lesi = 0;
	ri->tia_idx_max_eq_init = FALSE;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
	if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
		ri->lesi = TRUE;
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */

	/* gainctrl/tia (tiny) related */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		ri->tia_idx_max_eq_init = TRUE;
		if (ACMAJORREV_33(pi->pubpi->phy_rev)) {
			/* chippkg bit-2: LESI 0: Enable ; 1: Disable */
			if ((pi->sh->chippkg & 0x4) == 0)
				ri->lesi = 1;
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	if ((val = nvram_get("lesidisab")) != NULL)
		lesidisab = bcm_atoi(val);
	if (lesidisab) {
		ri->lesi = 0;
		ri->lesi_mode = 1;
	}

}

static void *
BCMATTACHFN(phy_ac_get_rxg_param_tbl)(phy_info_t *pi, phy_ac_rxg_param_tbl_t tbl_id,
uint8 ind_2d_tbl)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		switch (tbl_id) {
			case LNA12_GAIN_TBL_2G:
				return (void *)lna12_gain_tbl_2g_maj36[ind_2d_tbl];

			case LNA12_GAIN_TBL_5G:
				return (void *)lna12_gain_tbl_5g_maj36[ind_2d_tbl];

			case LNA12_GAIN_BITS_TBL_2G:
				return (void *)lna12_gainbits_tbl_2g_maj36[ind_2d_tbl];

			case LNA12_GAIN_BITS_TBL_5G:
				return (void *)lna12_gainbits_tbl_5g_maj36[ind_2d_tbl];

			case TIA_GAIN_TBL:
				return (void *)tia_gain_tbl_maj36;

			case TIA_GAIN_BITS_TBL:
				return (void *)tia_gainbits_tbl_maj36;

			case BIQ01_GAIN_TBL:
				return (void *)biq01_gain_tbl_maj36[ind_2d_tbl];

			case BIQ01_GAIN_BITS_TBL:
				return (void *)biq01_gainbits_tbl_maj36[ind_2d_tbl];

			case GAIN_LIMIT_TBL:
				return (void *)gainlimit_tbl_maj36[ind_2d_tbl];

			case LNA1_ROUTMAP_TBL_2G:
				return (void *)lna1_rout_map_2g_maj36;

			case LNA1_ROUTMAP_TBL_5G:
				return (void *)lna1_rout_map_5g_maj36;

			case LNA1_GAINMAP_TBL_2G:
				return (void *)lna1_gain_map_2g_maj36;

			case LNA1_GAINMAP_TBL_5G:
				return (void *)lna1_gain_map_5g_maj36;

			case LNA2_GAINMAP_TBL_2G:
				return (void *)lna2_gain_map_2g_maj36;

			case LNA2_GAINMAP_TBL_5G:
				return (void *)lna2_gain_map_5g_maj36;

			case MAX_GAIN_TBL:
				return (void *)maxgain_maj36;

			case FAST_AGC_CLIP_GAIN_TBL:
				return (void *)fast_agc_clip_gains_maj36;

			case SSAGC_CLIP_GAIN_TBL:
				return (void *)ssagc_clip_gains_maj36;

			case SSAGC_LNA1BYP_TBL:
				return (void *)ssagc_lna1byp_maj36;

			case SSAGC_CLIP2_TBL:
				return (void *)ssagc_clip2_tbl_maj36;

			case SSAGC_CLIPCNT_THRESH_TBL_2G:
				return (void *)ssagc_clipcnt_thresh_2g_maj36;

			case SSAGC_RSSI_THRESH_TBL_2G:
				return (void *)ssagc_rssi_thresh_2g_maj36;

			case SSAGC_CLIPCNT_THRESH_TBL_5G:
				return (void *)ssagc_clipcnt_thresh_5g_maj36;

			case SSAGC_RSSI_THRESH_TBL_5G:
				return (void *)ssagc_rssi_thresh_5g_maj36;

			case FAST_AGC_LONF_CLIP_GAIN_TBL:
				return (void *)fast_agc_lonf_clip_gains_maj36;

			default:
				return NULL;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	if (IS_28NM_RADIO(pi)) {
		switch (tbl_id) {
			case LNA12_GAIN_TBL_2G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna12_gain_tbl_2g_maj40_min1[ind_2d_tbl];
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna12_gain_tbl_2g_maj40[ind_2d_tbl];

			case LNA12_GAIN_TBL_5G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna12_gain_tbl_5g_maj40_min1[ind_2d_tbl];
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna12_gain_tbl_5g_maj40[ind_2d_tbl];

			case LNA12_GAIN_BITS_TBL_2G:
				return (void *)lna12_gainbits_tbl_2g_maj40[ind_2d_tbl];

			case LNA12_GAIN_BITS_TBL_5G:
				return (void *)lna12_gainbits_tbl_5g_maj40[ind_2d_tbl];

			case TIA_GAIN_TBL:
				return (void *)tia_gain_tbl_maj40;

			case TIA_GAIN_BITS_TBL:
				return (void *)tia_gainbits_tbl_maj40;

			case BIQ01_GAIN_TBL:
				return (void *)biq01_gain_tbl_maj40[ind_2d_tbl];

			case BIQ01_GAIN_BITS_TBL:
				return (void *)biq01_gainbits_tbl_maj40[ind_2d_tbl];

			case GAIN_LIMIT_TBL:
				return (void *)gainlimit_tbl_maj40[ind_2d_tbl];

			case LNA1_ROUTMAP_TBL_2G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna1_rout_map_2g_maj40_min1;
				else if (ACMAJORREV_40(pi->pubpi->phy_rev))
					return (void *)lna1_rout_map_2g_maj40;
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna1_rout_map_2g_maj37;

			case LNA1_ROUTMAP_TBL_5G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna1_rout_map_5g_maj40_min1;
				else if (ACMAJORREV_40(pi->pubpi->phy_rev))
					return (void *)lna1_rout_map_5g_maj40;
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna1_rout_map_5g_maj37;

			case LNA1_GAINMAP_TBL_2G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna1_gain_map_2g_maj40_min1;
				else if (ACMAJORREV_40(pi->pubpi->phy_rev))
					return (void *)lna1_gain_map_2g_maj40;
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna1_gain_map_2g_maj37;

			case LNA1_GAINMAP_TBL_5G:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
				if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1)
					return (void *)lna1_gain_map_5g_maj40_min1;
				else if (ACMAJORREV_40(pi->pubpi->phy_rev))
					return (void *)lna1_gain_map_5g_maj40;
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
					return (void *)lna1_gain_map_5g_maj37;

			default:
				return NULL;
		}
	} else {
		return NULL;
	}
}

void
phy_ac_rxgcrs_set_desense(phy_ac_rxgcrs_info_t *rxgcrs_info, acphy_desense_values_t *desense,
	phy_ac_desense_type_t type)
{
	acphy_desense_values_t *local_desense;
	PHY_TRACE(("%s\n", __FUNCTION__));
	switch (type) {
		case LTE_DESENSE:
			local_desense = rxgcrs_info->lte_desense;
			break;
		case BT_DESENSE:
			local_desense = rxgcrs_info->bt_desense;
			break;
		case CURR_DESENSE:
			local_desense = rxgcrs_info->curr_desense;
			break;
		case ZERO_DESENSE:
			local_desense = rxgcrs_info->zero_desense;
			break;
		case TOTAL_DESENSE:
			local_desense = rxgcrs_info->total_desense;
			break;
		default:
			PHY_ERROR(("Unsupported desense type\n"));
			return;
	}
	memcpy(local_desense, desense, sizeof(acphy_desense_values_t));
}

acphy_desense_values_t
phy_ac_rxgcrs_get_desense(phy_ac_rxgcrs_info_t *rxgcrs_info, phy_ac_desense_type_t type)
{
	acphy_desense_values_t local_desense, *prxgcrs_desense = NULL;
	PHY_TRACE(("%s\n", __FUNCTION__));
	switch (type) {
		case CURR_DESENSE:
			prxgcrs_desense = rxgcrs_info->curr_desense;
			break;
		case ZERO_DESENSE:
			prxgcrs_desense = rxgcrs_info->zero_desense;
			break;
		case TOTAL_DESENSE:
			prxgcrs_desense = rxgcrs_info->total_desense;
			break;
		default:
			PHY_ERROR(("Unsupported desense type\n"));
	}
	if (prxgcrs_desense != NULL) {
		memcpy(&local_desense, prxgcrs_desense, sizeof(acphy_desense_values_t));
	} else {
		bzero(&local_desense, sizeof(acphy_desense_values_t));
	}
	return local_desense;
}

#if defined(BCMDBG) || defined(BCMDBG_DUMP) || defined(WLTEST)
void
phy_ac_rxgcrs_cal_dump(phy_ac_rxgcrs_info_t *rxgcrsi, struct bcmstrbuf *b)
{
	uint8 core;
	phy_stf_data_t *stf_shdata = phy_stf_get_data(rxgcrsi->pi->stfi);

	BCM_REFERENCE(stf_shdata);

	bcm_bprintf(b, "crs_min_pwr cal:\n");
	if (rxgcrsi->crsmincal_run != 1) {
		bcm_bprintf(b, "  ACI desense is on:  crs_min_pwr cal DID NOT run\n");
	} else {
		bcm_bprintf(b, "   crsmin_cal ran %d times for channel %d:\n",
			rxgcrsi->phy_debug_crscal_counter,
			rxgcrsi->phy_debug_crscal_channel);
	}
	bcm_bprintf(b, "   Noise power used for setting crs_min thresholds : ");
	FOREACH_ACTV_CORE(rxgcrsi->pi, stf_shdata->phyrxchain, core) {
		bcm_bprintf(b, "Core-%d : %d, ", core, rxgcrsi->phy_noise_in_crs_min[core]);
	}
}
#endif /* defined(BCMDBG) || defined(BCMDBG_DUMP) || defined(WLTEST) */

static void
WLBANDINITFN(wlc_phy_edcrs_thresh_acphy)(phy_info_t *pi)
{
	uint16 assert, deassert;
	uint8 core;
	phy_info_acphy_t *pi_ac;
	pi_ac = pi->u.pi_acphy;

	/* -68, -74 (based on formula), though higher on chip */
	assert = 940;  deassert = 812;

	pi_ac->sromi->ed_thresh_default = ((((assert - 832)*30103)) - 48000000)/640000;
	if (pi_ac->sromi->ed_thresh2g) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (TINY_RADIO(pi) && (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev))) {
			assert = (640000*(pi_ac->sromi->ed_thresh2g + 6 + 75) + 25045696)/30103;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			assert = (640000*(pi_ac->sromi->ed_thresh2g + 75) + 25045696)/30103;
		}
		deassert = (640000*(pi_ac->sromi->ed_thresh2g + 69) + 25045696)/30103;
	}

	if (ACREV_GE(pi->pubpi->phy_rev, 32)) {
		/* Retain reset values for 43012A0 */
		if (!(ACMAJORREV_36(pi->pubpi->phy_rev))) {
			FOREACH_CORE(pi, core) {
				/* Set the EDCRS Assert Threshold to -71dBm */
				WRITE_PHYREGCE(pi, ed_crs20LAssertThresh0, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20LAssertThresh1, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20UAssertThresh0, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20UAssertThresh1, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1AssertThresh0, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1AssertThresh1, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20Usub1AssertThresh0, core, assert);
				WRITE_PHYREGCE(pi, ed_crs20Usub1AssertThresh1, core, assert);

				/* Set the EDCRS De-assert Threshold to -77dBm */
				WRITE_PHYREGCE(pi, ed_crs20LDeassertThresh0, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20LDeassertThresh1, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20UDeassertThresh0, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20UDeassertThresh1, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1DeassertThresh0, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1DeassertThresh1, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20Usub1DeassertThresh0, core, deassert);
				WRITE_PHYREGCE(pi, ed_crs20Usub1DeassertThresh1, core, deassert);
			}
		}
	} else {
		/* Set the EDCRS Assert Threshold to -71dBm */
		WRITE_PHYREG(pi, ed_crs20LAssertThresh0, assert);
		WRITE_PHYREG(pi, ed_crs20LAssertThresh1, assert);
		WRITE_PHYREG(pi, ed_crs20UAssertThresh0, assert);
		WRITE_PHYREG(pi, ed_crs20UAssertThresh1, assert);
		WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh0, assert);
		WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh1, assert);
		WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh0, assert);
		WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh1, assert);

		/* Set the EDCRS De-assert Threshold to -77dBm */
		WRITE_PHYREG(pi, ed_crs20LDeassertThresh0, deassert);
		WRITE_PHYREG(pi, ed_crs20LDeassertThresh1, deassert);
		WRITE_PHYREG(pi, ed_crs20UDeassertThresh0, deassert);
		WRITE_PHYREG(pi, ed_crs20UDeassertThresh1, deassert);
		WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh0, deassert);
		WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh1, deassert);
		WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh0, deassert);
		WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh1, deassert);
	}
}


/* ********************************************* */
/*				Internal Definitions					*/
/* ********************************************* */
static const uint8 ac_lna1_2g[]       = {0xf6, 0xff, 0x6, 0xc, 0x12, 0x19};
static const uint8 ac_lna1_2g_tiny[]	= {0xfa, 0x00, 0x6, 0xc, 0x12, 0x18};
static const uint8 ac_tiny_g_lna_rout_map[] = {9, 9, 9, 9, 6, 0};
static const uint8 ac_tiny_g_lna_gain_map[] = {2, 3, 4, 5, 5, 5};
static const uint8 ac_tiny_a_lna_rout_map[] = {11, 11, 11, 11, 7, 0};
static const uint8 ac_tiny_a_lna_gain_map[] = {4, 5, 6, 7, 7, 7};
static const uint8 ac_4365_a_lna_rout_map[] = {9, 9, 9, 9, 6, 0};
static const uint8 ac_lna1_2g_ltecoex[]       = {0xf6, 0xff, 0x6, 0xc, 0x12, 0x16};
static const uint8 ac_lna1_5g_tiny[]	= {0xfb, 0x00, 0x6, 0xc, 0x12, 0x18};
static const uint8 ac_lna1_5g_4365[]	= {0xfe, 0x03, 0x9, 0x10, 0x14, 0x18};
static const uint8 ac_lna1_5g[] = {0xf9, 0xfe, 0x4, 0xa, 0x10, 0x17};
static const uint8 ac_lna1_2g_43352_ilna[]    = {0xff, 0xff, 0x6, 0xc, 0x12, 0x19};
static const uint8 ac_lna2_2g_ltecoex[] = {0xf4, 0xf8, 0xfc, 0xff, 0xff, 0x5, 0x9};
static const uint8 ac_lna2_2g_43352_ilna[] = {0xfa, 0xfa, 0xfe, 0x01, 0x4, 0x7, 0xb};
static const uint8 ac_lna2_5g[] = {0xf5, 0xf8, 0xfb, 0xfe, 0x2, 0x5, 0x9};
static const uint8 ac_lna2_2g_gm2[] = {0xf4, 0xf8, 0xfc, 0xff, 0x2, 0x5, 0x9};
static const uint8 ac_lna2_2g_gm3[] = {0xf6, 0xfa, 0xfe, 0x01, 0x4, 0x7, 0xb};
static const uint8 ac_lna2_tiny[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
static const uint8 ac_lna2_tiny_4349[] = {0xEE, 0xf4, 0xfa, 0x0, 0x0, 0x0, 0x0};
static const uint8 ac_lna2_tiny_4365_2g[] = {0xf4, 0xfa, 0, 6, 6, 6, 6};
static const uint8 ac_lna2_tiny_4365_5g[] = {0xf1, 0xf7, 0xfd, 3, 3, 3, 3};
static const uint8 ac_lna2_tiny_ilna_dcc_comp[] = {0, 20, 16, 12, 8, 4, 0};
static const uint8 ac_lna1_rout_delta_2g[] = {0, 1, 2, 3, 5, 7, 8, 11, 13, 15, 18, 20};
static const uint8 ac_lna1_rout_delta_5g[] = {10, 7, 4, 2, 0};

#ifndef WLC_DISABLE_ACI
static void wlc_phy_desense_mf_high_thresh_acphy(phy_info_t *pi, bool on);
static void wlc_phy_desense_print_phyregs_acphy(phy_info_t *pi, const char str[]);
#endif

static int8 wlc_phy_rxgainctrl_calc_low_sens_acphy(phy_info_t *pi, int8 clipgain,
	bool trtx, bool lna1byp, uint8 core);
static void wlc_phy_set_analog_rxgain(phy_info_t *pi, uint8 clipgain,
	uint8 *gain_idx, bool trtx, uint8 core);
static int8 wlc_phy_rxgainctrl_calc_high_sens_acphy(phy_info_t *pi, int8 clipgain,
	bool trtx, bool lna1byp, uint8 core);
static uint8 wlc_phy_rxgainctrl_set_init_clip_gain_acphy(phy_info_t *pi, uint8 clipgain,
	int8 gain_dB, bool trtx, bool lna1byp, uint8 core);
static void wlc_phy_rxgainctrl_nbclip_acphy(phy_info_t *pi, uint8 core,
	int8 rxpwr_dBm);
static void wlc_phy_rxgainctrl_w1clip_acphy(phy_info_t *pi, uint8 core,
	int8 rxpwr_dBm);
static void wlc_phy_rxgainctrl_w1clip_acphy_28nm_ulp(phy_info_t *pi, uint8 core,
	int8 rxpwr_dBm);
#ifndef WLC_DISABLE_ACI
static void wlc_phy_set_crs_min_pwr_higain_acphy(phy_info_t *pi, uint8 thresh);
#endif
static void wlc_phy_limit_rxgaintbl_acphy(uint8 gaintbl[], uint8 gainbitstbl[], uint8 sz,
	const uint8 default_gaintbl[], uint8 min_idx, uint8 max_idx);
static void wlc_phy_rxgainctrl_nbclip_acphy_tiny(phy_info_t *pi, uint8 core, int8 rxpwr_dBm);
static void wlc_phy_rxgainctrl_nbclip_acphy_28nm_ulp(phy_info_t *pi, uint8 core, int8 rxpwr_dBm);
#define ACPHY_NUM_NB_THRESH 8
#define ACPHY_NUM_NB_THRESH_TINY 9
#define ACPHY_NUM_W1_THRESH 12

#define NB_LOW 0
#define NB_MID 1
#define NB_HIGH 2
#ifndef WLC_DISABLE_ACI

/*
Default - High MF thresholds are used only if pktgain < 81dB.
To always use high mf thresholds, change this to 98dBs
*/
static void
wlc_phy_desense_mf_high_thresh_acphy(phy_info_t *pi, bool on)
{
	uint16 val;
	uint8 core;

	if (on) {
		val = 0x5f62;
	} else {
		val = (TINY_RADIO(pi) || ACMAJORREV_37(pi->pubpi->phy_rev) ||
		       ACMAJORREV_40(pi->pubpi->phy_rev)) ? 0x454b : 0x4e51;
	}
	if (ACREV_GE(pi->pubpi->phy_rev, 32)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev)) {
			/* Use default values for now */
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			FOREACH_CORE(pi, core) {
				WRITE_PHYREGCE(pi, crshighlowpowThresholdl, core, val);
				WRITE_PHYREGCE(pi, crshighlowpowThresholdu, core, val);
				WRITE_PHYREGCE(pi, crshighlowpowThresholdlSub1, core, val);
				WRITE_PHYREGCE(pi, crshighlowpowThresholduSub1, core, val);
			}
		}
	} else {
		WRITE_PHYREG(pi, crshighlowpowThresholdl, val);
		WRITE_PHYREG(pi, crshighlowpowThresholdu, val);
		WRITE_PHYREG(pi, crshighlowpowThresholdlSub1, val);
		WRITE_PHYREG(pi, crshighlowpowThresholduSub1, val);
	}
}

static void
wlc_phy_desense_print_phyregs_acphy(phy_info_t *pi, const char str[])
{
}

#endif /* #ifndef WLC_DISABLE_ACI */

void wlc_phy_get_rxgain_acphy(phy_info_t *pi, rxgain_t rxgain[], int16 *tot_gain,
                              uint8 force_gain_type)
{
	uint8  core, bw_idx, ant, core_freq_segment_map;
	uint16 code_A, code_B, gain_tblid, stall_val;
	int8   gain_dvga, gain_bq0, gain_bq1, gain_lna1, gain_lna2, gain_mix, tr_loss = 0;
	int8   elna_gain[PHY_CORE_MAX], lna1_gain_corr[PHY_CORE_MAX];
	bool   elna_present, tr_tx;
	int8   subband_idx;
	bool lna1byp;
	uint8 bands[NUM_CHANS_IN_CHAN_BONDING];
	/* Mclip gain */
	bool mclip_agc_en;
	uint32 mclip_gaincode;
	int8   mclip_gainidx;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	lna1byp = CHSPEC_IS2G(pi->radio_chanspec) ?
		((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
		& BFL2_LNA1BYPFORTR2G) != 0) :
		((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
		& BFL2_LNA1BYPFORTR5G) != 0);

	bzero(elna_gain, sizeof(elna_gain));
	(void)memset(lna1_gain_corr, 0, sizeof(lna1_gain_corr));

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		elna_present = BF_ELNA_2G(pi_ac);
		bw_idx = (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	} else {
		elna_present = BF_ELNA_5G(pi_ac);
		bw_idx = (CHSPEC_IS80(pi->radio_chanspec) ||
				PHY_AS_80P80(pi, pi->radio_chanspec)) ? 2 :
				(CHSPEC_IS160(pi->radio_chanspec)) ? 3 :
		        (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	}
	/* 43602 China Spur WAR Gain Boosting for PAD */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
	if (CHSPEC_IS5G(pi->radio_chanspec) && ACMAJORREV_5(pi->pubpi->phy_rev) &&
		(pi->sromi->dBpad)) {
		/* Boost Core 2 radio gain */
		MOD_RADIO_REGC(pi, TXMIX5G_CFG1, 2, gainboost, 0x9);
		MOD_RADIO_REGC(pi, PGA5G_CFG1, 2, gainboost, 0x7);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */

	mclip_agc_en = (bool)PHY_GETINTVAR_DEFAULT(pi, "mclip_agc_en", 1);

	FOREACH_CORE(pi, core) {
		if (force_gain_type == 4) {
			if (mclip_agc_en) {
				/* Use mclip gaincode with elna off */
				/* For REV40, use lna index of 5 (same as in initgain) */
				/* To avoid lna gain step delta affecting tr offset measurement */
				/* {0 5 3 0 0 2 0} tr = 1 */
				mclip_gaincode = 0x410075;
				/* construct code_A,B to reuse existing code and for consistency */
				code_A  =  (mclip_gaincode >> 1)&0x7ff;
				code_B  =  ((mclip_gaincode >> 23)&0x1)<<1 |
					((!mclip_gaincode & 0x1))<<2 | (mclip_gaincode & 0x1)<<3 |
					((mclip_gaincode >> 12)&0x7)<<4 |
					((mclip_gaincode >> 15)&0x7)<<8 |
					((mclip_gaincode >> 18)&0xf)<<12;
			}
			else {
				code_A  =  READ_PHYREGC(pi, cliploGainCodeA, core);
				code_B   = READ_PHYREGC(pi, cliploGainCodeB, core);
			}
		} else if (force_gain_type == 3) {
			if (mclip_agc_en) {
				/* Use mclip gaincode with lna index 3 */
				/* REV40: in case lna1 offset delta between idx 5 and 3 */
				/* Use force_gain_type 3 to calibation and compensate the delta */
				/* {0 3 3 0 0 2 0} tr = 0 */
				mclip_gaincode = 0x41006c;
				/* construct code_A,B to reuse existing code and for consistency */
				code_A  =  (mclip_gaincode >> 1)&0x7ff;
				code_B  =  ((mclip_gaincode >> 23)&0x1)<<1 |
					((!mclip_gaincode & 0x1))<<2 | (mclip_gaincode & 0x1)<<3 |
					((mclip_gaincode >> 12)&0x7)<<4 |
					((mclip_gaincode >> 15)&0x7)<<8 |
					((mclip_gaincode >> 18)&0xf)<<12;
			} else {
				code_A  =  READ_PHYREGC(pi, clipmdGainCodeA, core);
				code_B   = READ_PHYREGC(pi, clipmdGainCodeB, core);
			}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
			if (CHSPEC_IS5G(pi->radio_chanspec) && ACMAJORREV_4(pi->pubpi->phy_rev)) {
				int8   LNA1_GAINSTEP_ERR[CH_5G_4BAND] = {1, 2, 2, 2}; /* 5g:l,ml,mu,h */
				core_freq_segment_map = phy_ac_chanmgr_get_data
					(pi_ac->chanmgri)->core_freq_mapping[core];
				subband_idx = wlc_phy_rssi_get_chan_freq_range_acphy(pi,
						core_freq_segment_map, core);
				lna1_gain_corr[core] = LNA1_GAINSTEP_ERR[subband_idx];
			}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		} else if (force_gain_type == 2) {
			if (mclip_agc_en) {
				/* 2G and 5G has different lineup; Use similar gain */
				mclip_gainidx = CHSPEC_IS2G(pi->radio_chanspec) ? 1 : 3;
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RSSICLIPGAIN0+core*32, 1,
				                         (1+mclip_gainidx), 32,
				                         &mclip_gaincode);
				/* construct code_A,B to reuse existing code and for consistency */
				code_A  =  (mclip_gaincode >> 1)&0x7ff;
				code_B  =  ((mclip_gaincode >> 23)&0x1)<<1 |
					((!mclip_gaincode & 0x1))<<2 | (mclip_gaincode & 0x1)<<3 |
					((mclip_gaincode >> 12)&0x7)<<4 |
					((mclip_gaincode >> 15)&0x7)<<8 |
					((mclip_gaincode >> 18)&0xf)<<12;
			} else {
				code_A  =  READ_PHYREGC(pi, clipHiGainCodeA, core);
				code_B   = READ_PHYREGC(pi, clipHiGainCodeB, core);
			}
		} else if (force_gain_type == 1) {
			/* Change limited to 4350, Olympic program
			 * When we issue iqest with -i 1 option, INIT gain is applied.
			 * But because of Interference_code, the INIT gain can change
			 * So, for 4350, we have forced the fixed init gain by hardcoding it.
			 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
			if (ACMAJORREV_2(pi->pubpi->phy_rev) && ACMINORREV_1(pi)) {
				code_A	=  pi_ac->rxgcrsi->initGain_codeA;
				code_B	 = pi_ac->rxgcrsi->initGain_codeB;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */
			{
				code_A  =  READ_PHYREGC(pi, InitGainCodeA, core);
				code_B   = READ_PHYREGC(pi, InitGainCodeB, core);
			}
		} else if (force_gain_type == 7) {
			code_A	=  pi_ac->rxgcrsi->initGain_codeA;
			code_B	 = pi_ac->rxgcrsi->initGain_codeB;
		} else if (force_gain_type == 8) {
			code_A	=  pi_ac->rxgcrsi->initGain_codeA;
			code_B	 = pi_ac->rxgcrsi->initGain_codeB;
		} else if (force_gain_type == 9) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
			if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
				/* Bump gain by 6dB for better estimation below -82dBm */
				code_A  =  0x4aa;
				code_B   = 0x1204;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
			{
				code_A  =  0x16a;
				code_B   = 0x554;
			}
		} else if (force_gain_type == 6) {
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_tx_pu, 0);
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_rx_pu, 0);
			MOD_PHYREGCE(pi, RfctrlIntc, core, override_tr_sw, 0);
			continue;
		} else {
			return;
		}

		rxgain[core].lna1 = (code_A >> 1) & 0x7;
		rxgain[core].lna2 = (code_A >> 4) & 0x7;
		rxgain[core].mix  = (code_A >> 7) & 0xf;
		rxgain[core].lpf0 = (code_B >> 4) & 0x7;
		rxgain[core].lpf1 = (code_B >> 8) & 0x7;
		rxgain[core].dvga = (code_B >> 12) & 0xf;

		if (core == 0) {
			gain_tblid =  ACPHY_TBL_ID_GAIN0;
		} else if (core == 1) {
			gain_tblid =  ACPHY_TBL_ID_GAIN1;
		} else {
			gain_tblid =  ACPHY_TBL_ID_GAIN2;
		}

		stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
		ACPHY_DISABLE_STALL(pi);
		/* ELNA */
		if (elna_present == 1) {
			wlc_phy_table_read_acphy(pi, gain_tblid, 1, (0x0 + (code_A & 0x1)),
			                         8, &elna_gain[core]);
		}

		/* lna1, lna2 and mixer */
		wlc_phy_table_read_acphy(pi, gain_tblid, 1, (0x8 + rxgain[core].lna1), 8,
				&gain_lna1);
		gain_lna1 -= lna1_gain_corr[core];
		gain_lna2 = pi_ac->rxgcrsi->lna2_complete_gaintbl[rxgain[core].lna2];
		wlc_phy_table_read_acphy(pi, gain_tblid, 1, (0x20 + rxgain[core].mix), 8,
		                         &gain_mix);
		ACPHY_ENABLE_STALL(pi, stall_val);

		gain_bq0 = 3 * rxgain[core].lpf0;
		/* For 4347, lpf1 is not used. lpf1 index is still set to 2 */
		/* But lpf1 gain should be set to 0 regardless of index   */
		if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
			gain_bq1 = 0;
		}
		else {
			gain_bq1 = 3 * rxgain[core].lpf1;
		}
		gain_dvga = 3 * rxgain[core].dvga;

		if (force_gain_type == 4) {
			if (mclip_agc_en) {
				tr_tx = mclip_gaincode & 0x1;
			} else {
				tr_tx = READ_PHYREGFLD(pi, Core0cliploGainCodeB, clip1loTrTxIndex);
			}
			if (lna1byp) {
					tr_loss =  (READ_PHYREG(pi, TRLossValue) & 0x7f);
					gain_lna1 = READ_PHYREGFLD(pi, Core0_lna1BypVals,
							lna1BypGain0);
			}
			if (tr_tx) {
					MOD_PHYREGCE(pi, RfctrlIntc, core,
							tr_sw_tx_pu, 1);
					MOD_PHYREGCE(pi, RfctrlIntc, core,
							tr_sw_rx_pu, 0);
					MOD_PHYREGCE(pi, RfctrlIntc, core,
							override_tr_sw, 1);
					if (core == 0) {
						tr_loss = READ_PHYREGFLD(pi, Core0_TRLossValue,
							freqGainTLoss0);
					} else if (core == 1) {
						tr_loss = READ_PHYREGFLD(pi, Core1_TRLossValue,
							freqGainTLoss1);
					} else if (core == 2) {
						tr_loss = READ_PHYREGFLD(pi, Core2_TRLossValue,
							freqGainTLoss2);
					} else {
						tr_loss = READ_PHYREGFLD(pi, Core3_TRLossValue,
							freqGainTLoss3);
					}
			}
		 } else if ((force_gain_type == 3) &&
		            (mclip_agc_en || (pi_ac->rxgcrsi->mdgain_trtx_allowed))) {
			if (mclip_agc_en) {
				tr_tx = mclip_gaincode & 0x1;
			} else {
				tr_tx = READ_PHYREGFLD(pi, Core0clipmdGainCodeB, clip1mdTrTxIndex);
			}
			if (tr_tx) {
					MOD_PHYREGCE(pi, RfctrlIntc, core,
						tr_sw_tx_pu, 1);
					MOD_PHYREGCE(pi, RfctrlIntc, core,
						tr_sw_rx_pu, 0);
					MOD_PHYREGCE(pi, RfctrlIntc, core,
						override_tr_sw, 1);
					if (core == 0) {
						tr_loss = READ_PHYREGFLD(pi, Core0_TRLossValue,
							freqGainTLoss0);
					} else if (core == 1) {
						tr_loss = READ_PHYREGFLD(pi, Core1_TRLossValue,
							freqGainTLoss1);
					} else if (core == 2) {
						tr_loss = READ_PHYREGFLD(pi, Core2_TRLossValue,
							freqGainTLoss2);
					} else {
						tr_loss = READ_PHYREGFLD(pi, Core3_TRLossValue,
							freqGainTLoss3);
					}
			} else {
					tr_loss =  READ_PHYREG(pi, TRLossValue) & 0x7f;
			}
		} else {
			tr_loss =  READ_PHYREG(pi, TRLossValue) & 0x7f;
		}
		/* Total gain: */
		tot_gain[core] = elna_gain[core] + gain_lna1 + gain_lna2 + gain_mix + gain_bq0
		        +  gain_bq1 - tr_loss + gain_dvga;
		/* core_freq_segment_map is only required for 80P80 mode.
		For other modes, it is ignored
		*/
		core_freq_segment_map =
			phy_ac_chanmgr_get_data(pi_ac->chanmgri)->core_freq_mapping[core];

		/* adjust total gain based on common rssi correction factor: */
		ant = phy_get_rsdbbrd_corenum(pi, core);
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			tot_gain[core] -=
			        pi_ac->sromi->rssioffset.rssi_corr_normal[ant][bw_idx];
		} else {
			if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
				phy_ac_chanmgr_get_chan_freq_range_80p80(pi, 0, bands);
				subband_idx = (core <= 1) ? (bands[0] - 1) : (bands[1] - 1);
			} else {
				subband_idx = phy_ac_chanmgr_get_chan_freq_range(pi,
					pi->radio_chanspec, core_freq_segment_map)-1;
			}
			tot_gain[core] -=
			        pi_ac->sromi->rssioffset.rssi_corr_normal_5g[ant][subband_idx]
			        [bw_idx];
		}
		PHY_RXIQ(("In %s: | Mode = %d | Code_A = %X | Code_B = %X |"
			  "\n", __FUNCTION__, force_gain_type, code_A, code_B));
	}
}

static int8
wlc_phy_rxgainctrl_calc_low_sens_acphy(phy_info_t *pi, int8 clipgain, bool trtx, bool lna1byp,
	uint8 core)
{
	int sens, sens_bw_c9[] = {-66, -63, -60};   /* c9s1 1% sensitivity for 20/40/80 mhz */
	int sens_bw_c11[]= {-63, -60, -57};   /* c11s1_ldpc 1% sensitivity for 20/40/80 mhz */
	uint8 low_sen_adjust[] = {25, 22, 19};   /* low_end_sens = -clip_gain - low_sen_adjust */
	uint8 bw_idx, elna_idx, trloss, elna_bypass_tr;
	int8 elna, detection, demod;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	acphy_desense_values_t *desense = pi_ac->rxgcrsi->total_desense;

	ASSERT(core < PHY_CORE_MAX);
	bw_idx = CHSPEC_BW_LE20(pi->radio_chanspec) ? 0 : CHSPEC_IS40(pi->radio_chanspec) ? 1 : 2;
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev) ||
	    ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_25_40(pi->pubpi->phy_rev))
		sens =  sens_bw_c11[bw_idx];
	else
		sens =  sens_bw_c9[bw_idx];

	if (lna1byp)
		sens = sens + 7;

	elna_idx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initExtLnaIndex);
	elna = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][elna_idx];
	trloss = pi_ac->rxgcrsi->fem_rxgains[core].trloss;
	elna_bypass_tr = pi_ac->rxgcrsi->fem_rxgains[core].elna_bypass_tr;

	if (TINY_RADIO(pi))
		detection = -6 - (clipgain + low_sen_adjust[bw_idx]);
	else
		detection = 0 - (clipgain + low_sen_adjust[bw_idx]);

	demod = trtx ? (sens + trloss - (elna_bypass_tr * elna)) : sens;
	demod += desense->nf_hit_lna12;

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		uint8 idx, max_lna2;
		int8 extra_loss = 0;
		/* sens is worse if lower lna2 gains are used */
		if (trtx) {
			idx = pi_ac->rxgcrsi->rxgainctrl_stage_len[LNA2_ID] - 1;
			max_lna2 =
				pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[LNA2_ID][idx];
			extra_loss = 3 * (3 - max_lna2);
			demod += MAX(0, extra_loss);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	return MAX(detection, demod);
}

int
acphy_get_rxgain_index(phy_type_rxgcrs_ctx_t *ctx, int32 *index)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;
	rxgain_t rxgain[PHY_CORE_MAX];
	int16 gain_db[PHY_CORE_MAX];
	uint8 rssi_rev = phy_ac_rssi_get_data(pi->u.pi_acphy->rssii)->rssi_cal_rev;
	int i;
	bool forced;
	uint32 ind;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	if (!(CHIPID(pi->sh->chip) == BCM4345_CHIP_ID || CHIPID(pi->sh->chip) == BCM4350_CHIP_ID))
		return BCME_UNSUPPORTED;

	*index = 0;

	/* find gain */
	FOREACH_CORE(pi, i) {
		forced = READ_PHYREGFLDCE(pi, RfctrlOverrideGains, i, rxgain);
		if (i == 0) {

			if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
				/* prevent any rssi cal */
				phy_ac_rssi_set_cal_rev(pi->u.pi_acphy->rssii, TRUE);

				/* when forced, init gain will contain current gain. In normal
				 * operation we use GainInfo. These correspond to added gain modes
				 * 11 and 10 respectively
				 */

				/* init gain or pkt */

				wlc_phy_get_rxgain_acphy(pi, rxgain, gain_db, forced ? 11 : 10);
				phy_ac_rssi_set_cal_rev(pi->u.pi_acphy->rssii, rssi_rev);
			} else { /* ACHWACIREV(pi) */
				wlc_phy_get_rxgain_acphy(pi, rxgain, gain_db, 10);
			}
		}
		/* Broadloss of 2 dB is applied. Common to both 4345 and 4350 */
		if (rxgain[i].trtx == 0)
			gain_db[i] += 2;

		/* compute index with round */
		ind = ((gain_db[i] + 20)*85+255)/256;

		if (ind >= 64)
			return BCME_ERROR;

		if (ACPHY_ENABLE_FCBS_HWACI(pi)) {

		  /* if ACI mode, index reference from 64 */
		  if (rxgcrs_info->curr_desense->ofdm_desense ||
		      rxgcrs_info->curr_desense->bphy_desense ||
		      rxgcrs_info->curr_desense->lna1_tbl_desense)
		    ind += 64;
		} else if (ACHWACIREV(pi)) {
			if (phy_ac_noise_get_data(pi_ac->noisei)->hw_aci_status)
				ind += 64;
		}

		*index |= ind<<(8*i);
		if (ACHWACIREV(pi) && forced) {
			*index = phy_ac_noise_get_data(pi_ac->noisei)->gain_idx_forced;
		}
	}
	PHY_INFORM(("GIDX_OVR: Core0 | phyreg 0x722: %x | 0x730: %x |"
		"0x731: %x |0x734: %x | \n",
		READ_PHYREG(pi, RfctrlOverrideGains0),
		READ_PHYREG(pi, RfctrlCoreRXGAIN10),
		READ_PHYREG(pi, RfctrlCoreRXGAIN20),
		READ_PHYREG(pi, RfctrlCoreLpfGain0)));
	PHY_INFORM(("GIDX_OVR: Core1 | phyreg 0x922: %x | 0x930: %x |"
		"0x931: %x |0x934: %x | \n",
		READ_PHYREG(pi, RfctrlOverrideGains1),
		READ_PHYREG(pi, RfctrlCoreRXGAIN11),
		READ_PHYREG(pi, RfctrlCoreRXGAIN21),
		READ_PHYREG(pi, RfctrlCoreLpfGain1)));

	return BCME_OK;
}

int
acphy_set_rxgain_index(phy_type_rxgcrs_ctx_t *ctx, int32 index)
{
	phy_ac_rxgcrs_info_t *ri = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = ri->pi;
	uint8 core;
	int gain_step;
	int8 dB;
	int32 rb_index = 0;
	uint8 gain_idx[ACPHY_MAX_RX_GAIN_STAGES] = {0, 0, 0, 0, 0, 0, 0};
	bool trtx;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	rxgain_t rxgain[PHY_CORE_MAX];
	bool blocker_mode, forced;
	bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(pi_ac)
		: BF_ELNA_5G(pi_ac);
	rxgain_ovrd_t rxgain_ovrd[PHY_CORE_MAX];
	uint8 ind = 0; /* per core gain index */
	bool suspend;

	if (!(CHIPID(pi->sh->chip) == BCM4345_CHIP_ID || CHIPID(pi->sh->chip) == BCM4350_CHIP_ID))
		return BCME_UNSUPPORTED;

	phy_ac_noise_set_gainidx(pi_ac->noisei, (uint16) index);

	/* find gain */
	FOREACH_CORE(pi, core) {

		ind = index >> (8*core);

		forced = READ_PHYREGFLDCE(pi, RfctrlOverrideGains, core, rxgain);

		/* map from index to gain */
		blocker_mode = ind > 63;
		gain_step = ind & 0x3f;

		/* return an error in out of bounds
		(rely on encode gain for actualy max/min checking)
		*/
		gain_step = MIN(MAX(gain_step, 0), 38);

		/* round ? may need to add bias for unique unique mapping */
		dB = gain_step * 3 - 20;

		if (ACPHY_ENABLE_FCBS_HWACI(pi)) {

			ri->curr_desense->clipgain_desense[0] = 0;
			ri->curr_desense->clipgain_desense[1] = 0;
			ri->curr_desense->clipgain_desense[2] = 0;
			ri->curr_desense->clipgain_desense[3] = 0;

			if ((blocker_mode == 0) || (ind == 255)) {
				ri->curr_desense->ofdm_desense = 0;
				ri->curr_desense->bphy_desense = 0;
				ri->curr_desense->lna1_tbl_desense = 0;
			} else {
				ri->curr_desense->ofdm_desense = HWACI_OFDM_DESENSE;
				ri->curr_desense->bphy_desense = HWACI_BPHY_DESENSE;
				ri->curr_desense->lna1_tbl_desense = HWACI_LNA1_DESENSE;
				ri->curr_desense->clipgain_desense[0] = HWACI_CLIP_INIT_DESENSE;
				ri->curr_desense->clipgain_desense[1] = HWACI_CLIP_HIGH_DESENSE;
				ri->curr_desense->clipgain_desense[2] = HWACI_CLIP_MED_DESENSE;
				ri->curr_desense->clipgain_desense[3] = HWACI_CLIP_LO_DESENSE;
			}

			/* this will update AGC settings for ALL cores, even though
			further below we only copy those of interest. For multicore
			devices this will have to be handled more carefully...
			*/
			wlc_phy_rxgainctrl_set_gaintbls_acphy_tiny(pi, core, ACPHY_TBL_ID_GAIN0,
			ACPHY_TBL_ID_GAINBITS0);
			wlc_phy_rxgainctrl_gainctrl_acphy_tiny(pi, 0);
#ifndef WLC_DISABLE_ACI
		} else if (ACHWACIREV(pi)) {
			int state;
			if (blocker_mode == 0) {
				state = 0;
			} else if (ind == 255) {
				state = -1;
			} else {
				state = 1;
			}
			wlc_phy_hwaci_override_acphy(pi, state);
#endif /* WLC_DISABLE_ACI */
		}

		if (ind == 255) {
			/* index 255 - release any gain override */
			WRITE_PHYREGCE(pi, RfctrlOverrideGains, core,
			rxgainindx_cmd_ovrd[core].rfctrlovrd);
			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN1, core,
			rxgainindx_cmd_ovrd[core].rxgain);
			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN2, core,
			rxgainindx_cmd_ovrd[core].rxgain2);
			WRITE_PHYREGCE(pi, RfctrlCoreLpfGain, core,
			rxgainindx_cmd_ovrd[core].lpfgain);
			MOD_PHYREGCE(pi, RfctrlIntc, core, override_tr_sw, 0);
#ifndef WLC_DISABLE_ACI
			if (ACHWACIREV(pi)) {
				MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_tx_pu, 0);
				MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_rx_pu, 0);
				MOD_PHYREGCE(pi, RfctrlIntc, core, override_tr_sw, 0);
				wlc_phy_hwaci_override_acphy(pi, -1);
			}
#endif /* WLC_DISABLE_ACI */
			continue;
		}

		if (dB < ri->rx_elna_bypass_gain_th[core])
			trtx = elna_present;
		else
			trtx = 0;

		if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
			(void)wlc_phy_rxgainctrl_encode_gain_acphy(pi, core, dB, trtx,
				FALSE, INIT_GAIN, gain_idx);
			/* write to gain code A , also allows us to use get_rxgain */
			wlc_phy_set_analog_rxgain(pi, 0, gain_idx, trtx, core);

			acphy_get_rxgain_index(pi, &rb_index); /* debug only */
		} else if (ACHWACIREV(pi)) {
			(void)wlc_phy_rxgainctrl_encode_pktgain_acphy(pi, core, dB, trtx,
				FALSE, gain_idx);
		}

		/* also write to rfctrl (not strictly necessary if we are
		enforcing stay-in-carrier search
		*/
		rxgain[core].lna1 = gain_idx[1];
		rxgain[core].lna2 = gain_idx[2];
		rxgain[core].mix  = gain_idx[3];
		rxgain[core].lpf0 = gain_idx[4];
		rxgain[core].lpf1 = gain_idx[5];
		rxgain[core].dvga = gain_idx[6];
		PHY_INFORM(("GIDX_vals core%d: index %d:%d | dB: %d ,%d |elna %d lna1 %d lna2 %d "
			"mix %d biq0 %d biq1 %d dvga %d trtx %d EBYP_th %d | BKR %d\n",
			core, ind, ((rb_index>>(core*8))&0xff), dB,
			wlc_phy_rxgainctrl_encode_pktgain_acphy(pi, core, dB, trtx,
			FALSE, gain_idx),
			gain_idx[0], gain_idx[1], gain_idx[2], gain_idx[3], gain_idx[4],
			gain_idx[5], gain_idx[6], trtx, ri->rx_elna_bypass_gain_th[core],
			blocker_mode));

		if (forced == 0) {
			/* gain is currently unforced - save override state for to restore
			at a later date if required
			*/
			rxgainindx_cmd_ovrd[core].rfctrlovrd =
			READ_PHYREGCE(pi, RfctrlOverrideGains, core);
			rxgainindx_cmd_ovrd[core].rxgain =
			READ_PHYREGCE(pi, RfctrlCoreRXGAIN1, core);
			rxgainindx_cmd_ovrd[core].rxgain2 =
			READ_PHYREGCE(pi, RfctrlCoreRXGAIN2, core);
			rxgainindx_cmd_ovrd[core].lpfgain =
			READ_PHYREGCE(pi, RfctrlCoreLpfGain, core);
		}

		/* override tr/tx */
		if (trtx) {
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_tx_pu, trtx);
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_rx_pu, 0);
			MOD_PHYREGCE(pi, RfctrlIntc, core, override_tr_sw, 1);
		} else {
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_tx_pu, 0);
			MOD_PHYREGCE(pi, RfctrlIntc, core, tr_sw_rx_pu, 0);
			MOD_PHYREGCE(pi, RfctrlIntc, core, override_tr_sw, 0);
		}
	}
	/* override all core gains
	 * Note, with this implementation we must have all cores overriden,
	 * because we are using rfctrl_overriderxgain.
	 */
	if (ind != 255)
		wlc_phy_rfctrl_override_rxgain_acphy(pi, 0, rxgain, rxgain_ovrd);

	suspend = !(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC);
	if (!suspend)
		wlapi_suspend_mac_and_wait(pi->sh->physhim);
	/* flush init gain changes */
	wlc_phy_resetcca_acphy(pi);
	if (!suspend)
		wlapi_enable_mac(pi->sh->physhim);

	return BCME_OK;

}

static int8
wlc_phy_rxgainctrl_encode_pktgain_acphy(phy_info_t *pi, uint8 core, int8 gain_dB,
                                     bool trloss, bool lna1byp, uint8 *gidx)
{
	int16 gain_needed, tr;
	int8 i, k;
	int8 *gaintbl_this_stage, gain_this_stage = 0;
	int16 gain_applied = 0;
	uint8 *gainbitstbl_this_stage;
	uint8 gaintbl_len;
	int16 st_gain = 24, max_bq0_idx = 7, max_bq1_idx = 0;
	int16 gain_lim_off[4] = {0, 8, 16, 32};
	int8 gain_lim_entries[12];
	int16 bq0_index = 0, bq1_index = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	int8   elna_gain[PHY_CORE_MAX];
	bool   elna_present;
	/* BQ0 stage number. At which stage, remanining gain is rounded */

	PHY_TRACE(("%s: TARGET %d\n", __FUNCTION__, gain_dB));

	ASSERT(core < PHY_CORE_MAX);

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		elna_present = BF_ELNA_2G(pi_ac);
	} else {
		elna_present = BF_ELNA_5G(pi_ac);
	}

	if (elna_present == 1) {
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			elna_gain[core] = pi_ac->sromi->femrx_2g[core].elna;
		} else if (CHSPEC_CHANNEL(pi->radio_chanspec) < 100) {
			elna_gain[core] = pi_ac->sromi->femrx_5g[core].elna;
		} else {
			elna_gain[core] = pi_ac->sromi->femrx_5gh[core].elna;
		}
	} else {
		elna_gain[core] = 0;
	}

	if (trloss) {
		/* Values: elna_gain = 12, trloss = 18 */
		tr =  elna_gain[core] - pi_ac->rxgcrsi->fem_rxgains[core].trloss;
	} else {
		tr = elna_gain[core];
	}
	gain_needed = gain_dB;

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {

		gaintbl_this_stage = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[i];
		gainbitstbl_this_stage = pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[i];
		gaintbl_len = pi_ac->rxgcrsi->rxgainctrl_stage_len[i];

		if ((i == 1) || (i == 2) || (i == 3)) {
			/* For LNA1/2, read the gaindB, gainBits and gainLim tables. */
			wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_GAIN0, gaintbl_len,
				gain_lim_off[i], 8, gaintbl_this_stage);
			wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_GAINBITS0, gaintbl_len,
				gain_lim_off[i], 8, gainbitstbl_this_stage);
			wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_GAINLIMIT, gaintbl_len,
				gain_lim_off[i], 8, gain_lim_entries);
		}
		if (i == 0) {
			/* ELNA */
			/* If elna not bypassed tr == 12, else tr == 12 - 18 */
			gain_this_stage = (int8)tr;
		} else if ((i == 1) || (i == 2) || (i == 3)) {
			/* LNA1, LNA2 and Mix-Tia */
			for (k = gaintbl_len - 1; k >= 0; k--) {
					if ((gain_needed >= gain_lim_entries[k]) || (k == 0)) {
						gain_this_stage = gaintbl_this_stage[k];
					} else {
						continue;
					}
				gain_this_stage = gaintbl_this_stage[k];
				gidx[i] = gainbitstbl_this_stage[k];
				break;
			}
		} else if (i == 4) {
			/* Biquad 0 */
			/* Logic for choosing BQ0 gain.
			 * Step1: Is remaining gain >= 24, if yes, BQ0 will give 24 dB gain
			 * If no, is remaining gain >= 24/2, if yes, BQ0 will give 12 dB gain
			 * If no, is remaining gain >= 24/2/2, if yes, BQ0 will give 6 dB gain
			 * If no, is remaining gain >= 24/2/2/2, if yes, BQ0 will give 3 dB gain
			 */
			while ((gain_needed < st_gain) && (st_gain != 0)) {
				st_gain = st_gain / 2;
			}
			bq0_index = st_gain / 3;
			bq0_index = MIN(max_bq0_idx, bq0_index);
			bq0_index = bq0_index > 0 ? bq0_index : 0;
			gain_this_stage = bq0_index * 3;
			gidx[i] = (int8)bq0_index;

		} else if (i == 5) {
			/* Biquad 1 */
			/* Logic behind choosing biquad gain indices -
			 * First biquad 0's index is arrived at. Then, remaning gain from Biquad 1.
			 */
			max_bq1_idx = (READ_PHYREG(pi, Core0_BiQuad_MaxGain) - bq0_index * 3) / 3;
			bq1_index = gain_needed / 3;
			bq1_index = MIN(bq1_index, max_bq1_idx);

			bq1_index = bq1_index > 0 ? bq1_index : 0;
			gain_this_stage = bq1_index * 3;
			gidx[i] = (int8)bq1_index;

		} else if (i == 6) {
			/* DVGA */
			if (gain_needed >= 3) {
				gidx[i] = (int8)(gain_needed / 3);
				gain_this_stage =  gidx[i] * 3;
			} else {
				gidx[i] = 0;
				gain_this_stage =  0;
			}
		} else {
			gain_this_stage = 0;
		}

		gain_applied += gain_this_stage;
		gain_needed = gain_needed - gain_this_stage;
	}
	PHY_INFORM(("gain_applied = %d, tr = %d \n",
		gain_applied, tr));

	return (int8)gain_applied;
}

static void
wlc_phy_set_analog_rxgain(phy_info_t *pi, uint8 clipgain, uint8 *gain_idx, bool trtx, uint8 core)
{
	uint8 lna1, lna2, mix, bq0, bq1, tx, rx, dvga;
	uint16 gaincodeA, gaincodeB, final_gain;
	bool lna1byp;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	ASSERT(core < PHY_CORE_MAX);

	lna1 = gain_idx[1];
	lna2 = gain_idx[2];
	mix = gain_idx[3];
	bq0 = (TINY_RADIO(pi)) ? 0 : gain_idx[4];
	bq1 = gain_idx[5];
	dvga = (TINY_RADIO(pi) || IS_28NM_RADIO(pi)) ? gain_idx[6] : 0;

	lna1byp = pi_ac->rxgcrsi->fem_rxgains[core].lna1byp;
	if (lna1byp || !trtx) {
		tx = 0; rx = 1;
	} else {
		tx = 1; rx = 0;
	}

	gaincodeA = ((mix << 7) | (lna2 << 4) | (lna1 << 1));
	gaincodeB = (dvga<<12) | (bq1 << 8) | (bq0 << 4) | (tx << 3) | (rx << 2);

	if (clipgain == 0) {
		WRITE_PHYREGC(pi, InitGainCodeA, core, gaincodeA);
		WRITE_PHYREGC(pi, InitGainCodeB, core, gaincodeB);
		final_gain = ((bq1 << 13) | (bq0 << 10) | (mix << 6) | (lna2 << 3) | (lna1 << 0));
		if (core == 3)
		    wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQ, 1, 0x509, 16,
		                          &final_gain);
		else
		    wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQ, 1, (0xf9 + core), 16,
		                          &final_gain);

		if (TINY_RADIO(pi) || IS_28NM_RADIO(pi)) {
			uint8 gmrout;
			uint16 rfseq_init_aux;
			uint8 offset = ACPHY_LNAROUT_BAND_OFFSET(pi,
				pi->radio_chanspec)	+ lna1 +
				ACPHY_LNAROUT_CORE_RD_OFST(pi, core);

			wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, offset, 8, &gmrout);

			if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev))
				rfseq_init_aux = dvga;
			else
				rfseq_init_aux = (((0xf & (gmrout >> 3)) << 4) | dvga);

			if (core == 3)
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQ, 1, 0x506, 16,
					&rfseq_init_aux);
			else
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQ, 1, (0xf6 + core),
					16, &rfseq_init_aux);

			/* elna index always zero, not required */
		}
	} else if (clipgain == 1) {
		WRITE_PHYREGC(pi, clipHiGainCodeA, core, gaincodeA);
		WRITE_PHYREGC(pi, clipHiGainCodeB, core, gaincodeB);
	} else if (clipgain == 2) {
		WRITE_PHYREGC(pi, clipmdGainCodeA, core, gaincodeA);
		WRITE_PHYREGC(pi, clipmdGainCodeB, core, gaincodeB);
	} else if (clipgain == 3) {
		WRITE_PHYREGC(pi, cliploGainCodeA, core, gaincodeA);
		WRITE_PHYREGC(pi, cliploGainCodeB, core, gaincodeB);
	} else if (clipgain == 4) {
		WRITE_PHYREGC(pi, clip2GainCodeA, core, gaincodeA);
		WRITE_PHYREGC(pi, clip2GainCodeB, core, gaincodeB);
	}

	if (lna1byp) {
		MOD_PHYREGC(pi, cliploGainCodeB, core, clip1loLna1Byp, 1);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			WRITE_PHYREGC(pi, _lna1BypVals, core, 0xfa1);
			MOD_PHYREG(pi, AfePuCtrl, lna1_pd_during_byp, 1);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

	} else {
		MOD_PHYREGC(pi, cliploGainCodeB, core, clip1loLna1Byp, 0);
	}

}

static int8
wlc_phy_rxgainctrl_calc_high_sens_acphy(phy_info_t *pi, int8 clipgain, bool trtx, bool lna1byp,
	uint8 core)
{
	uint8 high_sen_adjust = 23;  /* high_end_sens = high_sen_adjust - clip_gain */
	uint8 elna_idx, trloss;
	int8 elna, saturation, clipped, lna1_sat;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	if (TINY_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			lna1_sat = CHSPEC_IS2G(pi->radio_chanspec) ? -12 : -10;
			high_sen_adjust = 20;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				lna1_sat = -10;
				high_sen_adjust = 17;
			} else {
				lna1_sat = -2;
				high_sen_adjust = 19;
			}
		}
		if (lna1byp)
			lna1_sat = CHSPEC_IS2G(pi->radio_chanspec) ? (lna1_sat + 10):(lna1_sat + 7);
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

		bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(pi_ac)
	                                                      : BF_ELNA_5G(pi_ac);

		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			lna1_sat = elna_present ? rxg_params->lna1_sat_2g_elna :
					rxg_params->lna1_sat_2g;
			high_sen_adjust = rxg_params->high_sen_adjust_2g;
		} else {
			lna1_sat = elna_present ? rxg_params->lna1_sat_5g_elna :
					rxg_params->lna1_sat_5g;
			high_sen_adjust = rxg_params->high_sen_adjust_5g;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
		lna1_sat = -16;
		high_sen_adjust = 23;
	}

	ASSERT(core < PHY_CORE_MAX);
	elna_idx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initExtLnaIndex);
	elna = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][elna_idx];
	trloss = pi_ac->rxgcrsi->fem_rxgains[core].trloss;

	/* c9 needs lna1 input to be below -lna1_sat */
	saturation = trtx ? (trloss + lna1_sat - elna) : 0 + lna1_sat - elna;
	clipped = high_sen_adjust - clipgain;

	return MIN(saturation, clipped);
}

/* Wrapper to call encode_gain & set init/clip gains */
static uint8
wlc_phy_rxgainctrl_set_init_clip_gain_acphy(phy_info_t *pi, uint8 clipgain, int8 gain_dB,
	bool trtx, bool lna1byp, uint8 core)
{
	uint8 gain_idx[ACPHY_MAX_RX_GAIN_STAGES];
	uint8 gain_applied;

	gain_applied = wlc_phy_rxgainctrl_encode_gain_acphy(pi, core, gain_dB, trtx, lna1byp,
		clipgain, gain_idx);
	wlc_phy_set_analog_rxgain(pi, clipgain, gain_idx, trtx, core);

	return gain_applied;
}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
static void
phy_ac_rxgcrs_max_anagain_to_initgain_lesi(phy_ac_rxgcrs_info_t *info, uint8 i, uint8 core,
	uint8 gidx)
{
	uint8 l, offset = 0, gainbits[15];
	int8 gains[15];
	uint16 gainbits_tblid, gains_tblid;
	phy_info_t *pi = info->pi;
	uint8 gainbit_tbl_entry_size = sizeof(info->rxgainctrl_params[0].gainbitstbl[0][0]);
	uint8 gain_tbl_entry_size = sizeof(info->rxgainctrl_params[0].gaintbl[0][0]);

	if (i == 1) {
		offset = 8;
	} else if (i == 2) {
		offset = 16;
	} else if (i == 3) {
		offset = 32;
	} else {
		ASSERT(0);
	}

	/* gainBits */
	for (l = 0; l < info->rxgainctrl_stage_len[i]; l++)
		gainbits[l] = (l > gidx) ? gidx : info->rxgainctrl_params[core].gainbitstbl[i][l];
	gainbits_tblid = (core == 0) ? ACPHY_TBL_ID_GAINBITS0 : (core == 1) ?
	        ACPHY_TBL_ID_GAINBITS1 : (core == 2) ?
	        ACPHY_TBL_ID_GAINBITS2 : ACPHY_TBL_ID_GAINBITS3;
	wlc_phy_table_write_acphy(
		pi, gainbits_tblid, info->rxgainctrl_stage_len[i],
		offset, ACPHY_GAINBITS_TBL_WIDTH, gainbits);
	memcpy(info->rxgainctrl_params[core].gainbitstbl[i], gainbits,
	       gainbit_tbl_entry_size * info->rxgainctrl_stage_len[i]);

	/* gaintbl */
	for (l = 0; l < info->rxgainctrl_stage_len[i]; l++)
		gains[l] = (l > gidx) ?
		        info->rxgainctrl_params[core].gaintbl[i][gidx] :
		        info->rxgainctrl_params[core].gaintbl[i][l];
	gains_tblid = (core == 0) ? ACPHY_TBL_ID_GAIN0 : (core == 1) ?
	        ACPHY_TBL_ID_GAIN1 : (core == 2) ?
	        ACPHY_TBL_ID_GAIN2 : ACPHY_TBL_ID_GAIN3;
	wlc_phy_table_write_acphy(
		pi, gains_tblid, info->rxgainctrl_stage_len[i],
		offset, ACPHY_GAINDB_TBL_WIDTH, gains);
	memcpy(info->rxgainctrl_params[core].gaintbl[i], gains,
	       gain_tbl_entry_size * info->rxgainctrl_stage_len[i]);

}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

uint8
wlc_phy_rxgainctrl_encode_gain_acphy(phy_info_t *pi, uint8 core, int8 gain_dB,
	bool trloss, bool lna1byp, uint8 clipgain, uint8 *gidx)
{
	int16 min_gains[ACPHY_MAX_RX_GAIN_STAGES], max_gains[ACPHY_MAX_RX_GAIN_STAGES];
	int8 k, maxgain_this_stage;
	int16 sum_min_gains, gain_needed, tr = 0;
	uint8 i, j;
	int8 *gaintbl_this_stage, gain_this_stage = 0;
	int16 total_gain = 0;
	int16 gain_applied = 0;
	uint8 *gainbitstbl_this_stage;
	uint8 gaintbl_len, lowest_idx;
	int8 lna1mingain, lna1maxgain;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxgcrs_info_t *info = pi->u.pi_acphy->rxgcrsi;
	acphy_rxgainctrl_t gainctrl_params;
	uint8 rnd_stage;
	int8 lna1_byp_gain = 0;
	uint8 lna1_byp_ind = 0;
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	phy_ac_rxg_params_t *rxg_params = info->rxg_params;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

	PHY_TRACE(("%s: TARGET %d\n", __FUNCTION__, gain_dB));
	ASSERT(core < PHY_CORE_MAX);

	if (TINY_RADIO(pi) || ACMAJORREV_36(pi->pubpi->phy_rev)) {
		rnd_stage = ACPHY_MAX_RX_GAIN_STAGES - 4;
	} else if (IS_28NM_RADIO(pi)) {
		rnd_stage = ACPHY_MAX_RX_GAIN_STAGES - 2;
	} else {
		rnd_stage = ACPHY_MAX_RX_GAIN_STAGES - 3;
	}
	memcpy(&gainctrl_params, &pi_ac->rxgcrsi->rxgainctrl_params[core],
		sizeof(acphy_rxgainctrl_t));

	/* Over-write tia table to all same if using tia_high_mode */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if ((ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) &&
		(clipgain == 0)) {
		int8 idx = 7;
		for (j = 0; j < info->rxgainctrl_stage_len[TIA_ID]; j++) {
			gainctrl_params.gainbitstbl[TIA_ID][j] = idx;
			gainctrl_params.gaintbl[TIA_ID][j] =
				info->rxgainctrl_params[core].gaintbl[TIA_ID][idx];
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	if (lna1byp) {
		if (IS_28NM_RADIO(pi)) {
			for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
				max_gains[i] = info->rxgainctrl_maxout_gains[i];
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			lna1_byp_gain = (READ_PHYREGC(pi, _lna1BypVals, core) &
				ACPHY_Core0_lna1BypVals_lna1BypGain0_MASK(pi->pubpi->phy_rev))
				>> ACPHY_Core0_lna1BypVals_lna1BypGain0_SHIFT(pi->pubpi->phy_rev);
			for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
				max_gains[i] = info->rxgainctrl_maxout_gains[i];
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			tr = info->fem_rxgains[core].trloss;
			lna1mingain = info->rxgainctrl_params[core].gaintbl[1][0];
			lna1maxgain = info->rxgainctrl_params[core].gaintbl[1][5];
			tr = tr - (lna1maxgain - lna1mingain);
			PHY_INFORM(("lna1mingain=%d ,lna1maxgain=%d, new tr=%d \n",
				lna1mingain, lna1maxgain, tr));
			for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
				max_gains[i] = info->rxgainctrl_maxout_gains[i] + tr;
		}
	} else if (trloss) {
		tr =  info->fem_rxgains[core].trloss;
		for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
			max_gains[i] = info->rxgainctrl_maxout_gains[i] +
			        info->fem_rxgains[core].trloss;
	} else {
		for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
			max_gains[i] = info->rxgainctrl_maxout_gains[i];
	}
	gain_needed = gain_dB + tr;
	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			min_gains[i] = gainctrl_params.gaintbl[i][0];
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			min_gains[i] = info->rxgainctrl_params[core].gaintbl[i][0];
		}
		if (IS_28NM_RADIO(pi)) {
			if (i == 3) {
				if (clipgain == INIT_GAIN) {
					min_gains[i] = 28;
				}
			}
		}
	}

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {
		if (i == rnd_stage) {
			if ((gain_needed % 3) == 2)
				++gain_needed;
			if ((!TINY_RADIO(pi) && !ACMAJORREV_36(pi->pubpi->phy_rev))&&
					(gain_needed > 30))
				gain_needed = 30;
		}
		sum_min_gains = 0;

		for (j = i + 1; j < ACPHY_MAX_RX_GAIN_STAGES; j++) {
			if (TINY_RADIO(pi) && i < 5 && j >= 5)
				break;

			sum_min_gains += min_gains[j];
		}

		maxgain_this_stage = gain_needed - sum_min_gains;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			gaintbl_this_stage = gainctrl_params.gaintbl[i];
			gainbitstbl_this_stage = gainctrl_params.gainbitstbl[i];
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			gaintbl_this_stage = info->rxgainctrl_params[core].gaintbl[i];
			gainbitstbl_this_stage = info->rxgainctrl_params[core].gainbitstbl[i];
		}
		gaintbl_len = info->rxgainctrl_stage_len[i];

		if (lna1byp && (i == 1)) {
			gaintbl_len = 1;
			if (!ACMAJORREV_32(pi->pubpi->phy_rev) &&
				!ACMAJORREV_33(pi->pubpi->phy_rev)) {
				lna1_byp_gain = READ_PHYREGFLD(pi, Core0_lna1BypVals, lna1BypGain0);
				lna1_byp_ind = READ_PHYREGFLD(pi, Core0_lna1BypVals, lna1BypIndex0);
			}
		}

		/* Limit TIA to max tia index programmed in case of SSAGC */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (rxg_params->ssagc_en) {
			if ((i == 3) && (clipgain == SSAGC_CLIP_GAIN)) {
				if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
					gaintbl_len = 6;
				}
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

		if (IS_28NM_RADIO(pi) && !ACMAJORREV_36(pi->pubpi->phy_rev)) {
			if (i == 3) {
				if (clipgain == INIT_GAIN) {
					gidx[i] = 15;
					gain_this_stage =  28;
					gain_applied += gain_this_stage;
					gain_needed = gain_needed - gain_this_stage;
					gaintbl_len = 0;
					/* TIA in INIT_GAIN is special mode, bypass calc below */
				} else {
					gaintbl_len = 7;
				}
			}
		}
		/* Limit Mixer Gain to 2 in 5G band */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev) && !CHSPEC_IS2G(pi->radio_chanspec)) {
			if (i == 2) {
				gaintbl_len = 3;
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

		for (k = gaintbl_len - 1; k >= 0; k--) {
			if (lna1byp && (i == 1)) {
				gain_this_stage = lna1_byp_gain;
			} else {
				gain_this_stage = gaintbl_this_stage[k];
			}
			total_gain = gain_this_stage + gain_applied;
			lowest_idx = 0;

			if (gainbitstbl_this_stage[k] == gainbitstbl_this_stage[0])
				lowest_idx = 1;

			if ((lowest_idx == 1) || (lna1byp && (i == 1)) ||
			    ((gain_this_stage <= maxgain_this_stage) && (total_gain
			                                                 <= max_gains[i]))) {
				if (lna1byp && (i == 1)) {
					gidx[i] = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
						ACMAJORREV_33(pi->pubpi->phy_rev)) ?
						6 : lna1_byp_ind;
				} else {
					gidx[i] = gainbitstbl_this_stage[k];
				}
				gain_applied += gain_this_stage;
				gain_needed = gain_needed - gain_this_stage;
				break;
			}
		}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			/* If we want to fix max_tia_gain = initgain (for lesi) */
			if ((clipgain == 0) && (i > 0) && (i <= 3) && info->tia_idx_max_eq_init) {
				phy_ac_rxgcrs_max_anagain_to_initgain_lesi(info, i,
					core, gidx[i]);
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	}
	PHY_INFORM(("gain_applied = %d, tr = %d \n", gain_applied, tr));

	return (gain_applied - tr);
}

static void
wlc_phy_rxgainctrl_nbclip_acphy(phy_info_t *pi, uint8 core, int8 rxpwr_dBm)
{
	/* Multuply all pwrs by 10 to avoid floating point math */
	int rxpwrdBm_60mv, pwr;
	int pwr_60mv[] = {-40, -40, -40};     /* 20, 40, 80 */
	uint8 nb_thresh[] = {0, 35, 60, 80, 95, 120, 140, 156}; /* nb_thresh*10 to avoid float */
	const char *reg_name[ACPHY_NUM_NB_THRESH] = {"low", "low", "mid", "mid", "mid",
					       "mid", "high", "high"};
	uint8 mux_sel[] = {0, 0, 1, 1, 1, 1, 2, 2};
	uint8 reg_val[] = {1, 0, 1, 2, 0, 3, 1, 0};
	uint8 nb, i;
	int nb_thresh_bq[ACPHY_NUM_NB_THRESH];
	int v1, v2, vdiff1, vdiff2;
	uint8 idx[ACPHY_MAX_RX_GAIN_STAGES];
	uint16 initgain_codeA;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	ASSERT(core < PHY_CORE_MAX);
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	rxpwrdBm_60mv = (CHSPEC_IS80(pi->radio_chanspec) ||
		PHY_AS_80P80(pi, pi->radio_chanspec)) ? pwr_60mv[2] :
		CHSPEC_IS160(pi->radio_chanspec) ? 0 :
		(CHSPEC_IS40(pi->radio_chanspec)) ? pwr_60mv[1] : pwr_60mv[0];

	for (i = 0; i < ACPHY_NUM_NB_THRESH; i++) {
		nb_thresh_bq[i] = rxpwrdBm_60mv + nb_thresh[i];
	}

	/* Get the INITgain code */
	initgain_codeA = READ_PHYREGC(pi, InitGainCodeA, core);

	idx[0] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initExtLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initExtLnaIndex);
	idx[1] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initLnaIndex);
	idx[2] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initlna2Index)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initlna2Index);
	idx[3] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initmixergainIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initmixergainIndex);
	idx[4] = READ_PHYREGFLDC(pi, InitGainCodeB, core, InitBiQ0Index);
	idx[5] = READ_PHYREGFLDC(pi, InitGainCodeB, core, InitBiQ1Index);
	idx[6] = READ_PHYREGFLDC(pi, InitGainCodeB, core, initvgagainIndex);

	pwr = rxpwr_dBm;
	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES - 2; i++)
		pwr += pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[i][idx[i]];
	if (pi_ac->rxgcrsi->curr_desense->elna_bypass == 1)
		pwr = pwr - pi_ac->rxgcrsi->fem_rxgains[core].trloss;
	pwr = pwr * 10;

	nb = 0;
	if (pwr < nb_thresh_bq[0]) {
		nb = 0;
	} else if (pwr > nb_thresh_bq[ACPHY_NUM_NB_THRESH - 1]) {
		nb = ACPHY_NUM_NB_THRESH - 1;

		/* Reduce the bq0 gain, if can't achieve nbclip with highest nbclip thresh */
		if ((pwr - nb_thresh_bq[ACPHY_NUM_NB_THRESH - 1]) > 20) {
			if ((idx[4] > 0) && (idx[5] < 7)) {
				MOD_PHYREGC(pi, InitGainCodeB, core, InitBiQ0Index, idx[4] - 1);
				MOD_PHYREGC(pi, InitGainCodeB, core, InitBiQ1Index, idx[5] + 1);
			}
		}
	} else {
		for (i = 0; i < ACPHY_NUM_NB_THRESH - 1; i++) {
			v1 = nb_thresh_bq[i];
			v2 = nb_thresh_bq[i + 1];
			if ((pwr >= v1) && (pwr <= v2)) {
				vdiff1 = pwr > v1 ? (pwr - v1) : (v1 - pwr);
				vdiff2 = pwr > v2 ? (pwr - v2) : (v2 - pwr);

				if (vdiff1 < vdiff2)
					nb = i;
				else
					nb = i+1;
				break;
			}
		}
	}

	MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcNbClipMuxSel, mux_sel[nb]);

	if (strcmp(reg_name[nb], "low") == 0) {
		MOD_RADIO_REGC(pi, NBRSSI_CONFG, core, nbrssi_Refctrl_low, reg_val[nb]);
	} else if (strcmp(reg_name[nb], "mid") == 0) {
		MOD_RADIO_REGC(pi, NBRSSI_CONFG, core, nbrssi_Refctrl_mid, reg_val[nb]);
	} else {
		MOD_RADIO_REGC(pi, NBRSSI_CONFG, core, nbrssi_Refctrl_high, reg_val[nb]);
	}
}
static void
wlc_phy_rxgainctrl_w1clip_acphy_28nm_ulp(phy_info_t *pi, uint8 core, int8 rxpwr_dBm)
{
	/* Multuply all pwrs by 10 to avoid floating point math */

	int lna1_rxpwrdBm_lo4;
	uint8 *w1_hi;

	uint8 w1_delta_low[] = {0, 12, 23, 37, 45, 56, 63, 72, 78, 86, 93, 97};
	uint8 w1_delta_mid[] = {0, 17, 32, 45, 56, 67, 76, 84, 92, 99, 106, 113};
	uint8 w1_delta_hi2g[] = {0, 18, 33, 47, 59, 71, 81, 90, 98, 106, 111, 107};
	uint8 w1_delta_hi5g[] = {0, 18, 33, 47, 59, 71, 81, 90, 98, 106, 111, 107};
	/* 5g hi values are to be tuned later */
	int w1_thresh_low[ACPHY_NUM_W1_THRESH], w1_thresh_mid[ACPHY_NUM_W1_THRESH];
	int w1_thresh_high[ACPHY_NUM_W1_THRESH];
	int *w1_thresh;
	uint8 i, w1_muxsel, w1;
	uint8 elna, lna1_idx;
	int v1, v2, vdiff1, vdiff2, pwr, lna1_diff;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;
	int8 lna1_gain_idx5;

	ASSERT(core < PHY_CORE_MAX);
	w1_hi = CHSPEC_IS2G(pi->radio_chanspec) ? &w1_delta_hi2g[0] : &w1_delta_hi5g[0];

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		lna1_rxpwrdBm_lo4 = -380;
		lna1_gain_idx5 = rxg_params->lna12_gain_2g_tbl[0][5];
	} else {
		lna1_rxpwrdBm_lo4 = -340;
		lna1_gain_idx5 = rxg_params->lna12_gain_5g_tbl[0][5];
	}

	/* mid is 5dB higher than low, and high is 6dB higher than mid */
	for (i = 0; i < ACPHY_NUM_W1_THRESH; i++) {
		w1_thresh_low[i] = lna1_rxpwrdBm_lo4 + w1_delta_low[i];
		w1_thresh_mid[i] = 50 + lna1_rxpwrdBm_lo4 + w1_delta_mid[i];
		w1_thresh_high[i] = 110 + lna1_rxpwrdBm_lo4 + w1_hi[i];
	}

	elna = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][0];
	lna1_idx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initLnaIndex);
	lna1_diff = lna1_gain_idx5 -
			pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[1][lna1_idx];

	pwr = rxpwr_dBm + elna - lna1_diff;
	if (pi_ac->rxgcrsi->curr_desense->elna_bypass == 1) {
		pwr = pwr - pi_ac->rxgcrsi->fem_rxgains[core].trloss;
	}
	pwr = pwr * 10;

	if (pwr <= w1_thresh_low[0]) {
		w1 = 0;
		w1_muxsel = 0;
	} else if (pwr >= w1_thresh_high[ACPHY_NUM_W1_THRESH - 1]) {
		w1 = 11;
		w1_muxsel = 2;
	} else {
		if (pwr > w1_thresh_mid[ACPHY_NUM_W1_THRESH - 1]) {
			w1_thresh = w1_thresh_high;
			w1_muxsel = 2;
		} else if (pwr < w1_thresh_mid[0]) {
			w1_thresh = w1_thresh_low;
			w1_muxsel = 0;
		} else {
			w1_thresh = w1_thresh_mid;
			w1_muxsel = 1;
		}

		for (w1 = 0; w1 < ACPHY_NUM_W1_THRESH - 1; w1++) {
			v1 = w1_thresh[w1];
			v2 = w1_thresh[w1 + 1];
			if ((pwr >= v1) && (pwr <= v2)) {
				vdiff1 = pwr > v1 ? (pwr - v1) : (v1 - pwr);
				vdiff2 = pwr > v2 ? (pwr - v2) : (v2 - pwr);

				if (vdiff2 <= vdiff1)
					w1 = w1 + 1;

				break;
			}
		}
	}

	MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW1ClipMuxSel, w1_muxsel);
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		MOD_RADIO_REG_28NM(pi, RF, LNA2G_REG3, core, rx2g_wrssi1_threshold, w1+4);
	} else {
		MOD_RADIO_REG_28NM(pi, RF, RX5G_WRSSI1, core, rx5g_wrssi_threshold, w1+4);
	}
}

static void
wlc_phy_rxgainctrl_w1clip_acphy(phy_info_t *pi, uint8 core, int8 rxpwr_dBm)
{
	/* Multuply all pwrs by 10 to avoid floating point math */

	int lna1_rxpwrdBm_lo4;
	int lna1_pwrs_w1clip[] = {-340, -340, -340};   /* 20, 40, 80 */
	uint8 *w1_hi, w1_delta[] = {0, 19, 35, 49, 60, 70, 80, 88, 95, 102, 109, 115};
	uint8 w1_delta_hi2g[] = {0, 19, 35, 49, 60, 70, 80, 92, 105, 120, 130, 140};
	uint8 w1_delta_hi5g[] = {0, 19, 35, 49, 60, 70, 80, 96, 113, 130, 155, 180};
	int w1_thresh_low[ACPHY_NUM_W1_THRESH], w1_thresh_mid[ACPHY_NUM_W1_THRESH];
	int w1_thresh_high[ACPHY_NUM_W1_THRESH];
	int *w1_thresh;
	uint8 i, w1_muxsel, w1;
	uint8 elna, lna1_idx;
	int v1, v2, vdiff1, vdiff2, pwr, lna1_diff;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	ASSERT(core < PHY_CORE_MAX);
	w1_hi = CHSPEC_IS2G(pi->radio_chanspec) ? &w1_delta_hi2g[0] : &w1_delta_hi5g[0];

	if (TINY_RADIO(pi)) {
		if (CHSPEC_IS2G(pi->radio_chanspec))
			lna1_rxpwrdBm_lo4 = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev)) ? -360 : -370;
		else
			lna1_rxpwrdBm_lo4 = -310;
	} else
		lna1_rxpwrdBm_lo4 = (CHSPEC_IS80(pi->radio_chanspec) ||
				PHY_AS_80P80(pi, pi->radio_chanspec)) ? lna1_pwrs_w1clip[2] :
				CHSPEC_IS160(pi->radio_chanspec) ? 0 :
				(CHSPEC_IS40(pi->radio_chanspec)) ?
				lna1_pwrs_w1clip[1] : lna1_pwrs_w1clip[0];

	/* mid is 6dB higher than low, and high is 6dB higher than mid */
	for (i = 0; i < ACPHY_NUM_W1_THRESH; i++) {
		w1_thresh_low[i] = lna1_rxpwrdBm_lo4 + w1_delta[i];
		w1_thresh_mid[i] = 60 + w1_thresh_low[i];
		w1_thresh_high[i] = 120 + lna1_rxpwrdBm_lo4 + w1_hi[i];
	}

	elna = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][0];

	lna1_idx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initLnaIndex);

	if (TINY_RADIO(pi)) {
		lna1_diff = 24 - pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[1][lna1_idx];
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
	if (CHSPEC_IS2G(pi->radio_chanspec) &&
	    (ACMAJORREV_2(pi->pubpi->phy_rev) && ACMINORREV_3(pi))) {
		lna1_diff = 25 - pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[1][lna1_idx];
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */
	{
		lna1_diff = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[1][5] -
		            pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[1][lna1_idx];
	}

	pwr = rxpwr_dBm + elna - lna1_diff;
	if (pi_ac->rxgcrsi->curr_desense->elna_bypass == 1)
		pwr = pwr - pi_ac->rxgcrsi->fem_rxgains[core].trloss;
	pwr = pwr * 10;

	if (pwr <= w1_thresh_low[0]) {
		w1 = 0;
		w1_muxsel = 0;
	} else if (pwr >= w1_thresh_high[ACPHY_NUM_W1_THRESH - 1]) {
		w1 = 11;
		w1_muxsel = 2;
	} else {
		if (pwr > w1_thresh_mid[ACPHY_NUM_W1_THRESH - 1]) {
			w1_thresh = w1_thresh_high;
			w1_muxsel = 2;
		} else if (pwr < w1_thresh_mid[0]) {
			w1_thresh = w1_thresh_low;
			w1_muxsel = 0;
		} else {
			w1_thresh = w1_thresh_mid;
			w1_muxsel = 1;
		}

		for (w1 = 0; w1 < ACPHY_NUM_W1_THRESH - 1; w1++) {
			v1 = w1_thresh[w1];
			v2 = w1_thresh[w1 + 1];
			if ((pwr >= v1) && (pwr <= v2)) {
				vdiff1 = pwr > v1 ? (pwr - v1) : (v1 - pwr);
				vdiff2 = pwr > v2 ? (pwr - v2) : (v2 - pwr);

				if (vdiff2 <= vdiff1)
					w1 = w1 + 1;

				break;
			}
		}
	}

	if (TINY_RADIO(pi)) {
		MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW1ClipMuxSel, w1_muxsel);

		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			MOD_RADIO_REG_TINY(pi, LNA2G_RSSI1, core, lna2g_dig_wrssi1_threshold, w1+4);
		} else {
			MOD_RADIO_REG_TINY(pi, LNA5G_RSSI1, core, lna5g_dig_wrssi1_threshold, w1+4);
		}
	} else {
		/* the w1 thresh array is wrt w1 code = 4 */
		/*	MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcW1ClipCntTh, w1 + 4); */
		MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW1ClipMuxSel, w1_muxsel);
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			MOD_RADIO_REGC(pi, LNA2G_RSSI, core, dig_wrssi1_threshold, w1 + 4);
		} else {
			MOD_RADIO_REGC(pi, LNA5G_RSSI, core, dig_wrssi1_threshold, w1 + 4);
		}
	}
}

#ifndef WLC_DISABLE_ACI
static void
wlc_phy_set_crs_min_pwr_higain_acphy(phy_info_t *pi, uint8 thresh)
{
	MOD_PHYREG(pi, crsminpoweru0, crsminpower1, thresh);
	MOD_PHYREG(pi, crsmfminpoweru0, crsmfminpower1, thresh);
	if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
		MOD_PHYREG(pi, crsminpowerl0, crsminpower1, thresh);
		MOD_PHYREG(pi, crsmfminpowerl0, crsmfminpower1, thresh);
		MOD_PHYREG(pi, crsminpoweruSub10, crsminpower1, thresh);
		MOD_PHYREG(pi, crsmfminpoweruSub10, crsmfminpower1,  thresh);
		MOD_PHYREG(pi, crsminpowerlSub10, crsminpower1, thresh);
		MOD_PHYREG(pi, crsmfminpowerlSub10, crsmfminpower1,  thresh);
	}
}
#endif /* !WLC_DISABLE_ACI */

static void
wlc_phy_limit_rxgaintbl_acphy(uint8 gaintbl[], uint8 gainbitstbl[], uint8 sz,
	const uint8 default_gaintbl[], uint8 min_idx, uint8 max_idx)
{
	uint8 i;

	for (i = 0; i < sz; i++) {
		if (i < min_idx) {
			gaintbl[i] = default_gaintbl[min_idx];
			gainbitstbl[i] = min_idx;
		} else if (i > max_idx) {
			gaintbl[i] = default_gaintbl[max_idx];
			gainbitstbl[i] = max_idx;
		} else {
			gaintbl[i] = default_gaintbl[i];
			gainbitstbl[i] = i;
		}
	}
}

static void
wlc_phy_rxgainctrl_nbclip_acphy_28nm_ulp(phy_info_t *pi, uint8 core, int8 rxpwr_dBm)
{
	/* Multuply all pwrs by 10 to avoid floating point math */
	int16 rxpwrdBm_bw, pwr;
	int16 pwr_dBm[] = {-40, -40, -40};	/* 20, 40, 80 */
	int16 nb_thresh[] = { 0, 20, 40, 60, 80, 100, 120, 140, 160}; /* nb_thresh*10 avoid float */
	int8 reg_id[ACPHY_NUM_NB_THRESH_TINY] = { NB_LOW, NB_LOW, NB_LOW, NB_MID, NB_MID, NB_HIGH,
		NB_HIGH, NB_HIGH, NB_HIGH };
	uint8 mux_sel[] = {0, 0, 0, 1, 1, 2, 2, 2, 2};
	uint8 reg_val[] = {0, 2, 4, 2, 4, 1, 3, 5, 7};
	uint8 nb, i;
	int v1, v2, vdiff1, vdiff2;
	int8 idx[ACPHY_MAX_RX_GAIN_STAGES] = {-1, -1, -1, -1, -1, -1, -1};
	uint16 initgain_codeA;
	bool use_w3_detector = FALSE; /* To chhose betwen NB(T2) and W3(T1) detectors */
	int16 t2_delta_dB = 0; /* the power delta between T2 and T1 detectors */
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	int16 nb_thresh_end;

	ASSERT(core < PHY_CORE_MAX);
	ASSERT(IS_28NM_RADIO(pi));

	rxpwrdBm_bw = (CHSPEC_IS80(pi->radio_chanspec)) ? pwr_dBm[2] :
		(CHSPEC_IS40(pi->radio_chanspec)) ? pwr_dBm[1] : pwr_dBm[0];

	PHY_TRACE(("%s: adc pwr %d \n", __FUNCTION__, rxpwrdBm_bw));

	nb_thresh_end = nb_thresh[ACPHY_NUM_NB_THRESH_TINY - 1] + rxpwrdBm_bw;

	/* Get the INITgain code */
	initgain_codeA = READ_PHYREGC(pi, InitGainCodeA, core);

	idx[0] = (initgain_codeA &
			ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initExtLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initExtLnaIndex);
	idx[1] = (initgain_codeA &
			ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initLnaIndex);
	idx[2] = (initgain_codeA &
			ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initlna2Index)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initlna2Index);
	idx[3] = (initgain_codeA &
			ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initmixergainIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initmixergainIndex);

	pwr = rxpwr_dBm;

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES - 2; i++)
		if (idx[i] >= 0)
			pwr += pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[i][idx[i]];

	if (pi_ac->rxgcrsi->curr_desense->elna_bypass == 1)
		pwr -= pi_ac->rxgcrsi->fem_rxgains[core].trloss;

	pwr *= 10;

	/* Use T1 detector bank if T2 bank is not covering pwr */
	if (pwr > nb_thresh_end) {
		use_w3_detector = TRUE;

		if (CHSPEC_IS80(pi->radio_chanspec)) {
			if (idx[3] >= 8) {
				t2_delta_dB = 75;
			} else if (idx[3] >= 4) {
				t2_delta_dB = 60;
			} else {
				t2_delta_dB = 0;
			}
		} else if (CHSPEC_IS40(pi->radio_chanspec)) {
			if (idx[3] >= 8) {
				t2_delta_dB = 75;
			} else if (idx[3] >= 5) {
				t2_delta_dB = 60;
			} else {
				t2_delta_dB = 0;
			}
		} else {
			if (idx[3] >= 5) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
				if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
					/* 12.0 * 10 to avoid float */
					t2_delta_dB = 120;
				} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
				{
					t2_delta_dB = 60;
				}
			} else {
				t2_delta_dB = 0;
			}
		}
	}

	use_w3_detector = use_w3_detector && (t2_delta_dB > 0);

	for (i = 0; i < ACPHY_NUM_NB_THRESH_TINY; i++) {
		nb_thresh[i] += (rxpwrdBm_bw + t2_delta_dB);
	}

	if (pwr < nb_thresh[0]) {
		nb = 0;
	} else if (pwr > nb_thresh[ACPHY_NUM_NB_THRESH_TINY - 1]) {
		nb = ACPHY_NUM_NB_THRESH_TINY - 1;
	} else {
		nb = 0;
		for (i = 0; i < ACPHY_NUM_NB_THRESH_TINY - 1; i++) {
			v1 = nb_thresh[i];
			v2 = nb_thresh[i + 1];
			if ((pwr >= v1) && (pwr <= v2)) {
				vdiff1 = pwr > v1 ? (pwr - v1) : (v1 - pwr);
				vdiff2 = pwr > v2 ? (pwr - v2) : (v2 - pwr);

				nb = (vdiff1 < vdiff2) ? i : i + 1;
				break;
			}
		}
	}

	MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcNbClipMuxSel, mux_sel[nb]);

	if (use_w3_detector) {
		MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW3ClipMuxSel, 1);

		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG1, 0,
					tia_nbrssi1_threshold0, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG1, 0,
					tia_nbrssi1_threshold1, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG1, 0,
					tia_nbrssi1_threshold2, 0)
		ACPHY_REG_LIST_EXECUTE(pi);

		if (reg_id[nb] == NB_LOW) {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core,
					tia_nbrssi1_threshold0, reg_val[nb]);
		} else if (reg_id[nb] == NB_MID) {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core,
					tia_nbrssi1_threshold1, reg_val[nb]);
		} else {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core,
					tia_nbrssi1_threshold2, reg_val[nb]);
		}
	} else {
		MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW3ClipMuxSel, 0);

		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG2, 0,
					tia_nbrssi2_threshold0, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG2, 0,
					tia_nbrssi2_threshold1, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_NBRSSI_REG2, 0,
					tia_nbrssi2_threshold2, 0)
		ACPHY_REG_LIST_EXECUTE(pi);

		if (reg_id[nb] == NB_LOW) {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core,
					tia_nbrssi2_threshold0, reg_val[nb]);
		} else if (reg_id[nb] == NB_MID) {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core,
					tia_nbrssi2_threshold1, reg_val[nb]);
		} else {
			MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core,
					tia_nbrssi2_threshold2, reg_val[nb]);
		}
	}
}
static void
wlc_phy_rxgainctrl_nbclip_acphy_tiny(phy_info_t *pi, uint8 core, int8 rxpwr_dBm)
{
	/* Multuply all pwrs by 10 to avoid floating point math */
	int16 rxpwrdBm_bw, pwr;
	int16 pwr_dBm[] = {-40, -40, -40};	/* 20, 40, 80 */
	int16 nb_thresh[] = { 0, 20, 40, 60, 80, 100, 120, 140, 160}; /* nb_thresh*10 avoid float */
	const char * const reg_name[ACPHY_NUM_NB_THRESH_TINY] = {"low", "low", "low", "mid", "mid",
	                                                    "high", "high", "high", "high"};
	uint8 mux_sel[] = {0, 0, 0, 1, 1, 2, 2, 2, 2};
	uint8 reg_val[] = {0, 2, 4, 2, 4, 1, 3, 5, 7};
	uint8 nb, i;
	int v1, v2, vdiff1, vdiff2;
	int8 idx[ACPHY_MAX_RX_GAIN_STAGES] = {-1, -1, -1, -1, -1, -1, -1};
	uint16 initgain_codeA;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	bool use_w3_detector = FALSE; /* To chhose betwen NB(T2) and W3(T1) detectors */
	int16 t2_delta_dB = 0; /* the power delta between T2 and T1 detectors */
	int16 nb_thresh_end;

	ASSERT(core < PHY_CORE_MAX);
	ASSERT(TINY_RADIO(pi));

	rxpwrdBm_bw = (CHSPEC_IS80(pi->radio_chanspec) ||
			PHY_AS_80P80(pi, pi->radio_chanspec)) ? pwr_dBm[2] :
			(CHSPEC_IS160(pi->radio_chanspec)) ? 0 :
			(CHSPEC_IS40(pi->radio_chanspec)) ? pwr_dBm[1] : pwr_dBm[0];

	PHY_TRACE(("%s: adc pwr %d \n", __FUNCTION__, rxpwrdBm_bw));

	nb_thresh_end = nb_thresh[ACPHY_NUM_NB_THRESH_TINY - 1] + rxpwrdBm_bw;

	/* Get the INITgain code */
	initgain_codeA = READ_PHYREGC(pi, InitGainCodeA, core);

	idx[0] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initExtLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initExtLnaIndex);
	idx[1] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initLnaIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initLnaIndex);
	if (!ACMAJORREV_32(pi->pubpi->phy_rev) && !ACMAJORREV_33(pi->pubpi->phy_rev)) {
		idx[2] = (initgain_codeA &
			ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initlna2Index)) >>
			ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initlna2Index);
	}
	idx[3] = (initgain_codeA &
		ACPHY_REG_FIELD_MASK(pi, InitGainCodeA, core, initmixergainIndex)) >>
		ACPHY_REG_FIELD_SHIFT(pi, InitGainCodeA, core, initmixergainIndex);

	pwr = rxpwr_dBm;

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES - 2; i++) {
		if (idx[i] >= 0) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				int16 tmp_pwr;
				tmp_pwr = pi_ac->rxgcrsi->rxgainctrl_params
								[core].gaintbl[i][idx[i]];
				pwr += tmp_pwr;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
				pwr += pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[i][idx[i]];
		}
	}

	if (pi_ac->rxgcrsi->curr_desense->elna_bypass == 1)
		pwr -= pi_ac->rxgcrsi->fem_rxgains[core].trloss;

	pwr *= 10;

	/* Use T1 detector bank if T2 bank is not covering pwr, and in 5G only (for the */
	/* time being). As it can be seen for T1 vs. T2 delta to be > 0 TIA index >4 is */
	/* needed, typically for the current 2G gain line up TIA index = 4 (BIQ 2 is off), */
	/* and T1 doesn't help. To make this work for 2G, it is needed to decresse LNA */
	/* index which can lower sensitivity. */
	if (pwr > nb_thresh_end) {
	  use_w3_detector = TRUE;

	  if (CHSPEC_IS80(pi->radio_chanspec) ||
	    PHY_AS_80P80(pi, pi->radio_chanspec)) {
	    if (idx[3] >= 8)
	      t2_delta_dB = 75;
	    else if (idx[3] >= 4)
	      t2_delta_dB = 60;
	    else
	      t2_delta_dB = 0;
	  } else if (CHSPEC_IS160(pi->radio_chanspec)) {
	      ASSERT(0);
	  } else if (CHSPEC_IS40(pi->radio_chanspec)) {
	    if (idx[3] >= 8)
	      t2_delta_dB = 75;
	    else if (idx[3] >= 5)
	      t2_delta_dB = 60;
	    else
	      t2_delta_dB = 0;
	  } else {
	    if (idx[3] >= 5)
	      t2_delta_dB = 60;
	    else
	      t2_delta_dB = 0;
	  }

	}

	use_w3_detector = use_w3_detector && (t2_delta_dB > 0);
	for (i = 0; i < ACPHY_NUM_NB_THRESH_TINY; i++) {
	  nb_thresh[i] += (rxpwrdBm_bw + t2_delta_dB);
	}

	if (pwr < nb_thresh[0]) {
		nb = 0;
	} else if (pwr > nb_thresh[ACPHY_NUM_NB_THRESH_TINY - 1]) {
		nb = ACPHY_NUM_NB_THRESH_TINY - 1;
	} else {
		nb = 0;
		for (i = 0; i < ACPHY_NUM_NB_THRESH_TINY - 1; i++) {
			v1 = nb_thresh[i];
			v2 = nb_thresh[i + 1];
			if ((pwr >= v1) && (pwr <= v2)) {
				vdiff1 = pwr > v1 ? (pwr - v1) : (v1 - pwr);
				vdiff2 = pwr > v2 ? (pwr - v2) : (v2 - pwr);

				nb = (vdiff1 < vdiff2) ? i : i + 1;
				break;
			}
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, wrssi3_ref_low_sel, 0);
		MOD_RADIO_REG_TINY(pi, TIA_CFG12, core, wrssi3_ref_mid_sel, 0);
		MOD_RADIO_REG_TINY(pi, TIA_CFG12, core, wrssi3_ref_high_sel, 0);
		MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_low_sel, 0);
		MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_mid_sel, 0);
		MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_high_sel, 0);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

	MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcNbClipMuxSel, mux_sel[nb]);
	if (use_w3_detector) {
	  MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW3ClipMuxSel, 1);
	  if (strcmp(reg_name[nb], "low") == 0) {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, wrssi3_ref_low_sel, reg_val[nb]);
	  } else if (strcmp(reg_name[nb], "mid") == 0) {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG12, core, wrssi3_ref_mid_sel, reg_val[nb]);
	  } else {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG12, core, wrssi3_ref_high_sel, reg_val[nb]);
	  }
	} else {
	  MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcW3ClipMuxSel, 0);
	  if (strcmp(reg_name[nb], "low") == 0) {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_low_sel, reg_val[nb]);
	  } else if (strcmp(reg_name[nb], "mid") == 0) {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_mid_sel, reg_val[nb]);
	  } else {
	    MOD_RADIO_REG_TINY(pi, TIA_CFG13, core, nbrssi_ref_high_sel, reg_val[nb]);
	  }
	}
}


/* ********************************************* */
/*				External Definitions					*/
/* ********************************************* */
/**********  DESENSE : ACI, NOISE, BT (start)  ******** */

void
wlc_phy_rxgainctrl_gainctrl_acphy_tiny(phy_info_t *pi, uint8 init_desense)
{
	/* at present this is just a place holder for
	 * 'static' ELNA configuration. Eventually both TCL and
	 * driver should be changes to follow the auto-calc
	 * routine used in wlc_phy_rxgainctrl_gainctrl_acphy
	 */
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxgcrs_info_t *rxgcrsi = pi_ac->rxgcrsi;
	bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(pi_ac)
	                                                      : BF_ELNA_5G(pi_ac);
	uint8 max_analog_gain;
	int16 maxgain[ACPHY_MAX_RX_GAIN_STAGES] = {0, 0, 0, 0, -100, 125, 125};
	uint8 i, core;
	uint8 stall_val;
	bool init_trtx, hi_trtx, md_trtx, lo_trtx, lna1byp, clip2_trtx;
	int8 md_low_end, hi_high_end, lo_low_end, md_high_end;
	int8 clip2_gain;
	int8 hi_gain1, mid_gain1, lo_gain1;
	int8 nbclip_pwrdBm, w1clip_pwrdBm;
	int8 clip_gain[] = {61, 41, 19, 1};
	uint16 LPFgain, RFgain, LNARout, lna1idx, lna2idx, lna1_addr, LNAgm;
	uint8 lna1Rout, lna2Rout;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	bool lo_lna1byp = FALSE;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	rxgcrsi->mdgain_trtx_allowed = FALSE;

	MOD_PHYREG(pi, RxSdFeConfig6, rx_farrow_rshift_0, 0);

	max_analog_gain = READ_PHYREGFLD(pi, Core0HpFBw, maxAnalogGaindb);

	/* fill in gain limits for analog stages */
	maxgain[0] = maxgain[1] = maxgain[2] = maxgain[3] = max_analog_gain;

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
		rxgcrsi->rxgainctrl_maxout_gains[i] = maxgain[i];

	/* Keep track of it (used in interf_mitigation) */
	rxgcrsi->curr_desense->elna_bypass = wlc_phy_get_max_lna_index_acphy(pi, ELNA_ID);
	init_trtx = elna_present & rxgcrsi->curr_desense->elna_bypass;
	hi_trtx = elna_present & rxgcrsi->curr_desense->elna_bypass;
	md_trtx = elna_present & (rxgcrsi->mdgain_trtx_allowed |
		rxgcrsi->curr_desense->elna_bypass);
	lo_trtx = elna_present;
	clip2_trtx = md_trtx;

	if (ACPHY_LO_NF_MODE_ELNA_TINY(pi)) {
		clip_gain[0] = ACPHY_INIT_GAIN_TINY;
		clip_gain[1] = 42;
		clip_gain[2] = 25;
		clip_gain[3] = 15;
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		ACMAJORREV_33(pi->pubpi->phy_rev)) {
		clip_gain[0] = CHSPEC_IS2G(pi->radio_chanspec) ?
		        ACPHY_INIT_GAIN_4365_2G : ACPHY_INIT_GAIN_4365_5G;
#ifdef WL11ULB
		if (CHSPEC_IS10(pi->radio_chanspec) || CHSPEC_IS5(pi->radio_chanspec))
			clip_gain[0] += 3;
		PHY_CAL(("---%s: ulb: init_gain = %d\n", __FUNCTION__, clip_gain[0]));
#endif /* WL11ULB */
		clip_gain[1] = CHSPEC_IS2G(pi->radio_chanspec) ? 47 : 44;
		clip_gain[2] = 31;
		clip_gain[3] = 21;

		clip2_trtx = lo_trtx;
		if (!elna_present) {
			lo_lna1byp = TRUE;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	{
		if (!elna_present || md_trtx) {
			clip_gain[2] = 19;
			clip_gain[3] = 1;
		} else {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				clip_gain[2] = 24;
				clip_gain[3] = 10;
			} else {
				clip_gain[2] = 24;
				clip_gain[3] = 15;
			}
		}
	}

	if (ACPHY_ENABLE_FCBS_HWACI(pi))
		/* Apply desense */
		for (i = 0; i < 4; i++)
			clip_gain[i] -= rxgcrsi->curr_desense->clipgain_desense[i];

	/* with elna if md_gain uses TR != T, then LO_gain needs to be higher */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))
		clip2_gain = clip2_trtx ? clip_gain[3] : (clip_gain[2] + clip_gain[3]) >> 1;
	else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		clip2_gain = md_trtx ? clip_gain[3] : (clip_gain[2] + clip_gain[3]) >> 1;

	stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
	if (stall_val == 0)
		ACPHY_DISABLE_STALL(pi);

	FOREACH_CORE(pi, core) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			bool md_lna1byp = FALSE;
			bool lo_lna1byp_core;

			lna1byp = rxgcrsi->fem_rxgains[core].lna1byp;
			lo_lna1byp_core = lo_lna1byp | lna1byp;

			/* 0,1,2,3 for Init, hi, md and lo gains respectively */
			wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0, clip_gain[0] -
			                                            init_desense, init_trtx,
			                                            FALSE, core);
			hi_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 1, clip_gain[1],
			                                                       hi_trtx, FALSE,
			                                                       core);
			mid_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 2, clip_gain[2],
			                                                        md_trtx, md_lna1byp,
			                                                        core);
			wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 4, clip2_gain, clip2_trtx,
			                                            FALSE, core);
			lo_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 3, clip_gain[3],
			                                                       lo_trtx,
			                                                       lo_lna1byp_core,
			                                                       core);

			/* NB_CLIP */
			md_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, mid_gain1, md_trtx,
			                                                    md_lna1byp, core);
			hi_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, hi_gain1, hi_trtx,
			                                                      FALSE, core);
			lo_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, lo_gain1, lo_trtx,
			                                                    lo_lna1byp_core, core);
			md_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, mid_gain1,
				md_trtx, md_lna1byp, core);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			/* not supported on Tiny yet */
			lna1byp = rxgcrsi->fem_rxgains[core].lna1byp;

			/* 0,1,2,3 for Init, hi, md and lo gains respectively */
			wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0, clip_gain[0], init_trtx,
			                                            FALSE, core);

			hi_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 1, clip_gain[1],
			                                                       hi_trtx, FALSE,
			                                                       core);

			mid_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 2, clip_gain[2],
			                                                        md_trtx, FALSE,
			                                                        core);

			wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 4, clip2_gain, md_trtx,
			                                            FALSE, core);

			lo_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 3, clip_gain[3],
			                                                       lo_trtx, lna1byp,
			                                                       core);

			/* NB_CLIP */
			md_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, mid_gain1, md_trtx,
					0, core);
			hi_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, hi_gain1, hi_trtx,
					0, core);
			lo_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, lo_gain1, lo_trtx,
					0, core);
			md_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, mid_gain1,
					md_trtx, 0, core);
		}
		if (ROUTER_4349(pi) && CHSPEC_IS5G(pi->radio_chanspec) &&
			!BF_ELNA_5G(pi->u.pi_acphy)) {

			/* http://confluence.broadcom.com/display/WLAN/53573-Driver-GainRangeCurves
			 * JIRAs: SWWLAN-87252: Fix for PER Hump at ~-35dBm in 5G 20 20in40 20in80
			 * Issue: High-Gain is having PER hump at ~-35dBm
			 * Fix: Moving the NbClip Thsh in such a way that Mid gain will be applied
			 * in this region
			 */
			hi_high_end = -33;
		}

		w1clip_pwrdBm = (lo_low_end + md_high_end) >> 1;

		/* -1 times pwr to avoid rounding off error */
		if (CHSPEC_IS20(pi->radio_chanspec)) {
			/*
			 * use mid-point, as late clip2 is causing minor humps,
			 * move nb/w1 clip to a lower pwr
			 */
			nbclip_pwrdBm = (md_low_end + hi_high_end) >> 1;
		} else {
			/*
			 * (0.4*md/lo + 0.6*hi/md) fixed point. On fading channels,
			 * low-end sensitivity degrades, keep more margin there.
			 */
			nbclip_pwrdBm = (((-2 * md_low_end) + (-3 * hi_high_end)) * 13) >> 6;
			nbclip_pwrdBm = -nbclip_pwrdBm;
		}

		wlc_phy_rxgainctrl_nbclip_acphy_tiny(pi, core, nbclip_pwrdBm);
		wlc_phy_rxgainctrl_w1clip_acphy(pi, core, w1clip_pwrdBm);

		/* save threshold for gain index functions */
		if (md_trtx)
			/* assume 12 dB backoff */
		  rxgcrsi->rx_elna_bypass_gain_th[core] = clip_gain[0] + (-13 - nbclip_pwrdBm/10);
		else if (lo_trtx)
		  rxgcrsi->rx_elna_bypass_gain_th[core] = clip_gain[0] + (-13 - w1clip_pwrdBm/10);
		else
		  rxgcrsi->rx_elna_bypass_gain_th[core] = -100;
	}
	ACPHY_ENABLE_STALL(pi, stall_val);

	/* Saving the values of Init gain to be used
	 * in "wlc_phy_get_rxgain_acphy" function (used in rxiqest)
	 */
	rxgcrsi->initGain_codeA = READ_PHYREGC(pi, InitGainCodeA, 0);
	rxgcrsi->initGain_codeB = READ_PHYREGC(pi, InitGainCodeB, 0);

	if ((phy_ac_noise_get_data(pi_ac->noisei)->hw_aci_status == FALSE) &&
		!ACMAJORREV_32(pi->pubpi->phy_rev) && !ACMAJORREV_33(pi->pubpi->phy_rev)) {
		LPFgain  = (((rxgcrsi->initGain_codeB) & 0x700) >> 0x5) |
			(((rxgcrsi->initGain_codeB) & 0x70) >> 0x4);
		RFgain = rxgcrsi->initGain_codeA >> 1;
		lna1idx = RFgain & 0x7;
		lna2idx = (RFgain >> 3) & 0x7;

		lna1_addr = lna1idx + ACPHY_LNAROUT_BAND_OFFSET(pi, pi->radio_chanspec);
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, lna1_addr, 8, &lna1Rout);
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, (16 + lna2idx), 8, &lna2Rout);

		LNARout = (((lna2Rout >> 3) & 0xf) << 4) | ((lna1Rout >> 3) & 0xf);
		LNAgm = TINY_RADIO(pi) ? (lna1Rout & 0x7) : lna1idx;
		/* add DVGA [13:10] and LNA1 settings [2:0] */
		RFgain &= 0x3F8;  /*  mask off LNA1 index */
		RFgain |= (((rxgcrsi->initGain_codeB) & 0xf000)>>2)  | LNAgm;
		wlapi_bmac_write_shm(pi->sh->physhim, M_RXGAIN_HI(pi), RFgain);
		wlapi_bmac_write_shm(pi->sh->physhim, M_LPFGAIN_HI(pi), LPFgain);
		wlapi_bmac_write_shm(pi->sh->physhim, M_LNA_ROUT(pi), LNARout);
	}
}

void
wlc_phy_rxgainctrl_gainctrl_acphy_28nm_ulp(phy_info_t *pi)
{
	/* at present this is just a place holder for
	 * 'static' ELNA configuration. Eventually both TCL and
	 * driver should be changes to follow the auto-calc
	 * routine used in wlc_phy_rxgainctrl_gainctrl_acphy
	 */
	phy_ac_rxgcrs_info_t *ri = pi->u.pi_acphy->rxgcrsi;
	phy_ac_rxg_params_t *rxg_params = ri->rxg_params;
	bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(ri->aci)
	                                                      : BF_ELNA_5G(ri->aci);
	uint8 max_analog_gain;
	uint8 i, core;
	bool init_trtx, hi_trtx, md_trtx, lo_trtx, lna1byp;
	int8 md_low_end, hi_high_end, lo_low_end, md_high_end;
	int8 clip2_gain;
	int8 hi_gain1, mid_gain1, lo_gain1;
	int8 nbclip_pwrdBm, w1clip_pwrdBm;
	int8 clip_gain[4];

	ri->mdgain_trtx_allowed = FALSE;

	/* Copy clip gains from params */
	if (ACPHY_LO_NF_MODE_ELNA_28NM(pi)) {
		memcpy(clip_gain, rxg_params->fast_agc_lonf_clip_gains, ARRAYSIZE(clip_gain));
	} else {
		memcpy(clip_gain, rxg_params->fast_agc_clip_gains, ARRAYSIZE(clip_gain));
	}

	MOD_PHYREG(pi, RxSdFeConfig6, rx_farrow_rshift_0, 0);

	max_analog_gain = READ_PHYREGFLD(pi, Core0HpFBw, maxAnalogGaindb);
	/* fill in gain limits for analog stages */
	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {
		if (rxg_params->maxgain[i] == 0) {
			ri->rxgainctrl_maxout_gains[i] = max_analog_gain;
		} else {
			ri->rxgainctrl_maxout_gains[i] = rxg_params->maxgain[i];
		}
	}

	/* Keep track of it (used in interf_mitigation) */
	ri->curr_desense->elna_bypass = wlc_phy_get_max_lna_index_acphy(pi, ELNA_ID);
	init_trtx = elna_present & ri->curr_desense->elna_bypass;
	hi_trtx = elna_present & ri->curr_desense->elna_bypass;
	md_trtx = elna_present & (ri->mdgain_trtx_allowed | ri->curr_desense->elna_bypass);
	lo_trtx = elna_present;

	/* This needs to be checkde */
	if (!elna_present || md_trtx) {
	 /* Add relevant changes later */
	} else {
	 /* Add relevant changes later */
	}

	if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
		/* Apply desense */
		for (i = 0; i < 4; i++) {
			clip_gain[i] -= ri->curr_desense->clipgain_desense[i];
		}
	}

	/* with elna if md_gain uses TR != T, then LO_gain needs to be higher */
	clip2_gain = md_trtx ? clip_gain[3] : (clip_gain[2] + clip_gain[3]) >> 1;

	FOREACH_CORE(pi, core) {
		lna1byp = ri->fem_rxgains[core].lna1byp;

		/* 0,1,2,3 for Init, hi, md and lo gains respectively */
		wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, INIT_GAIN, clip_gain[0], init_trtx,
		                                            FALSE, core);

		hi_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, HIGH_GAIN, clip_gain[1],
		                                                       hi_trtx, FALSE, core);

		mid_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, MID_GAIN, clip_gain[2],
		                                                        md_trtx, FALSE, core);

		wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, CLIP2_GAIN, clip2_gain, md_trtx,
		                                            FALSE, core);

		lo_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, LOW_GAIN, clip_gain[3],
		                                                       lo_trtx, lna1byp, core);

		/* NB_CLIP */
		md_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, mid_gain1, md_trtx, 0,
				core);
		hi_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, hi_gain1, hi_trtx, 0,
				core);
		lo_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, lo_gain1, lo_trtx, 0,
				core);
		md_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, mid_gain1, md_trtx, 0,
				core);

		w1clip_pwrdBm = (lo_low_end + md_high_end) >> 1;
		/* -1 times pwr to avoid rounding off error */
		if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
			/*
			 * use mid-point, as late clip2 is causing minor humps,
			 * move nb/w1 clip to a lower pwr
			 */
			nbclip_pwrdBm = (md_low_end + hi_high_end) >> 1;
		} else {
			/*
			 * (0.4*md/lo + 0.6*hi/md) fixed point. On fading channels,
			 * low-end sensitivity degrades, keep more margin there.
			 */
			nbclip_pwrdBm = (((-2 * md_low_end) + (-3 * hi_high_end)) * 13) >> 6;
			nbclip_pwrdBm = -nbclip_pwrdBm;
		}

		wlc_phy_rxgainctrl_nbclip_acphy_28nm_ulp(pi, core, nbclip_pwrdBm);
		wlc_phy_rxgainctrl_w1clip_acphy_28nm_ulp(pi, core, w1clip_pwrdBm);
	}

	/* Saving the values of Init gain to be used
	 * in "wlc_phy_get_rxgain_acphy" function (used in rxiqest)
	 */
	ri->initGain_codeA = READ_PHYREGC(pi, InitGainCodeA, 0);
	ri->initGain_codeB = READ_PHYREGC(pi, InitGainCodeB, 0);
}

uint8
wlc_phy_get_max_lna_index_acphy(phy_info_t *pi, uint8 lna)
{
	uint8 max_idx, desense_state;
	acphy_desense_values_t *desense = NULL;
	uint8 elna_bypass, lna1_backoff, lna2_backoff;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	desense = pi_ac->rxgcrsi->total_desense;
	elna_bypass = desense->elna_bypass;
	lna1_backoff = desense->lna1_tbl_desense;
	lna2_backoff = desense->lna2_tbl_desense;

	desense_state = phy_ac_noise_get_desense_state(pi->u.pi_acphy->noisei);

	/* Find default max_idx */
	if (lna == ELNA_ID) {
		max_idx = elna_bypass;       /* elna */
	} else if (lna == LNA1_ID) {               /* lna1 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
		if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
			max_idx = desense_state != 0 ? desense->lna1_idx_max : 5;
			max_idx = MAX(0, max_idx - lna1_backoff);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
		if (IS_28NM_RADIO(pi)) {
			max_idx = ACPHY_LO_NF_MODE_ELNA_28NM(pi) ? rxg_params->lna1_lonf_max_indx :
					rxg_params->lna1_max_indx;
			max_idx = MAX(0, max_idx  - lna1_backoff);
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			max_idx = desense_state != 0 ? desense->lna1_idx_max:
				ACPHY_4365_MAX_LNA1_IDX;
			max_idx = MAX(0, max_idx - lna1_backoff);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			max_idx = MAX(0, ACPHY_MAX_LNA1_IDX  - lna1_backoff);
		}
	} else {                             /* lna2 */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (BF_ELNA_2G(pi_ac) && (elna_bypass == 0)) {
				uint8 core = 0;
				if (pi_ac->sromi->femrx_2g[core].elna > 10) {
					max_idx = ACPHY_ELNA2G_MAX_LNA2_IDX;
				} else if (pi_ac->sromi->femrx_2g[core].elna > 8) {
					max_idx = ACPHY_ELNA2G_MAX_LNA2_IDX_L;
				} else {
					max_idx = ACPHY_ILNA2G_MAX_LNA2_IDX;
				}
			} else {
				max_idx = ACPHY_ILNA2G_MAX_LNA2_IDX;
			}
		} else {
			max_idx = (BF_ELNA_5G(pi_ac) && (elna_bypass == 0)) ?
			        ACPHY_ELNA5G_MAX_LNA2_IDX : ACPHY_ILNA5G_MAX_LNA2_IDX;
		}

		/* Fix max idx for 4349 to 3 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			max_idx = ACPHY_20693_MAX_LNA2_IDX;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		if (IS_28NM_RADIO(pi)) {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				max_idx = ACPHY_LO_NF_MODE_ELNA_28NM(pi) ?
					rxg_params->lna2_lonf_force_indx_2g :
					rxg_params->lna2_2g_max_indx;
			} else {
				max_idx = ACPHY_LO_NF_MODE_ELNA_28NM(pi) ?
					rxg_params->lna2_lonf_force_indx_5g :
					rxg_params->lna2_5g_max_indx;
			}
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			/* Fix max lna2 idx for 4365 to 1 */
			max_idx = desense_state != 0 ? desense->lna2_idx_max:
				ACPHY_4365_MAX_LNA2_IDX;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
		}

		max_idx = MAX(0, max_idx - lna2_backoff);
	}

	return max_idx;
}

#ifndef WLC_DISABLE_ACI

/*********** Desense (geneal) ********** */
/* IMP NOTE: make sure whatever regs are changed here are either:
1. reset back to default below OR
2. Updated in gainctrl()
*/
void
wlc_phy_desense_apply_acphy(phy_info_t *pi, bool apply_desense)
{
	/* reset:
	   1 --> clear aci settings (the ones that gainctrl does not clear)
	   0 --> apply aci_noise_bt mitigation
	*/
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	acphy_desense_values_t *desense, *curr_desense;
	uint8 ofdm_desense, bphy_desense, initgain_desense;
	uint8 crsmin_thresh, crsmin_init;
	int8 crsmin_high;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	uint16 digigainlimit;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	uint8 bphy_minshiftbits[] = {0x77, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04,
	     0x04, 0x04, 0x04};
	uint16 bphy_peakenergy[]  = {0x10, 0x60, 0x10, 0x4c, 0x60, 0x30, 0x40, 0x40, 0x38, 0x2e,
	     0x40, 0x34, 0x40};
	uint8 bphy_initgain_backoff[] = {0, 0, 0, 0, 0, 0, 0, 3, 6, 9, 9, 12, 12};
	uint8 max_bphy_shiftbits = sizeof(bphy_minshiftbits) / sizeof(uint8);

	uint8 max_initgain_desense = 12, max_anlg_desense = 9 ;   /* only desnese bq0 */
	bool  ofdm_desense_extra_halfdB = 0;

	uint8 core, bphy_idx = 0;
	bool call_gainctrl = FALSE;

	desense = pi_ac->rxgcrsi->total_desense;
	curr_desense = pi_ac->rxgcrsi->curr_desense;

	wlapi_suspend_mac_and_wait(pi->sh->physhim);

	if (!apply_desense && !desense->forced) {
		/* when channel is changed, and the current channel is in mitigatation, then
		   we need to restore the values. wlc_phy_rxgainctrl_gainctrl_acphy() takes
		   care of all the gainctrl part, but we need to still restore back bphy regs
		*/
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev)) {
			wlc_phy_set_crs_min_pwr_higain_acphy(pi, ACPHY_CRSMIN_GAINHI);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			wlc_phy_set_crs_min_pwr_higain_acphy(pi, ACPHY_CRSMIN_DEFAULT);
		}
		wlc_phy_desense_mf_high_thresh_acphy(pi, FALSE);
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (IS_28NM_RADIO(pi)) {
				WRITE_PHYREG(pi, DigiGainLimit0, 0xC477);
			} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) {
				digigainlimit = READ_PHYREG(pi, DigiGainLimit0) & 0x8000;
				WRITE_PHYREG(pi, DigiGainLimit0, digigainlimit | 0x4477);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

			if (!ACMAJORREV_3(pi->pubpi->phy_rev)) {
				WRITE_PHYREG(pi, DigiGainLimit0, 0x4477);
			}
			WRITE_PHYREG(pi, PeakEnergyL, 0x10);
		}

		wlc_phy_desense_print_phyregs_acphy(pi, "restore");
		/* turn on LESI */
		if (pi_ac->rxgcrsi->lesi == 1) {
			phy_ac_rxgcrs_lesi(pi_ac->rxgcrsi, TRUE, 0);
		}
		/* channal update reset for default */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			wlc_phy_mlua_adjust_acphy(pi, FALSE);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	} else {
		/* Get total desense based on aci & bt & lte */
		wlc_phy_desense_calc_total_acphy(pi_ac->rxgcrsi);

		bphy_desense = MIN(ACPHY_ACI_MAX_DESENSE_BPHY_DB, desense->bphy_desense);
		ofdm_desense = MIN(ACPHY_ACI_MAX_DESENSE_OFDM_DB, desense->ofdm_desense);

		ofdm_desense_extra_halfdB = desense->ofdm_desense_extra_halfdB;

		/* channal update set to interference mode */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ofdm_desense > 0) {
		  if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		    wlc_phy_mlua_adjust_acphy(pi, TRUE);
		  }
		} else {
		  if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		    wlc_phy_mlua_adjust_acphy(pi, FALSE);
		  }
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

		/* Update total desense */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_36)))
		if (ACMAJORREV_4(pi->pubpi->phy_rev) || ACMAJORREV_36(pi->pubpi->phy_rev)) {
			desense->ofdm_desense = ofdm_desense;
			desense->bphy_desense = bphy_desense;
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_36))) */

		/* Initgain can change due to bphy desense */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if ((ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) &&
		    (curr_desense->bphy_desense != bphy_desense))
			call_gainctrl = TRUE;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

		/* Update current desense */
		curr_desense->ofdm_desense = ofdm_desense;
		curr_desense->bphy_desense = bphy_desense;
		curr_desense->ofdm_desense_extra_halfdB = ofdm_desense_extra_halfdB;

		/* if any ofdm desense is needed, first start using higher
		   mf thresholds (1dB sens loss)
		*/
		wlc_phy_desense_mf_high_thresh_acphy(pi, (ofdm_desense > 0));

		if (pi_ac->rxgcrsi->lesi) {
			/*
				First ACPHY_ACI_MAX_LESI_DESENSE_DB-1 of desense is done using lesi_crs
				ACPHY_ACI_MAX_LESI_DESENSE_DB+ dB is done by turning off lesi
				remainder is done using initgain/crsmin (with lesi off)
			*/
			if(ofdm_desense < ACPHY_ACI_MAX_LESI_DESENSE_DB) {
				phy_ac_rxgcrs_lesi(pi_ac->rxgcrsi, TRUE, (2*ofdm_desense) + ofdm_desense_extra_halfdB);
				ofdm_desense = 0; ofdm_desense_extra_halfdB = 0;
			} else {
				phy_ac_rxgcrs_lesi(pi_ac->rxgcrsi, FALSE, 0);
				ofdm_desense -= ACPHY_ACI_MAX_LESI_DESENSE_DB;
			}
		} else {
			/* 1st dB is for mf_high thresh */
			if (ofdm_desense > 0)
				ofdm_desense --;
		}

		/* Distribute desense between INITgain & crsmin(ofdm) & digigain(bphy) */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			/* round to 2, as bphy desnese table is in 2dB steps */
			bphy_idx = MIN((bphy_desense + 1) >> 1, max_bphy_shiftbits - 1);
			initgain_desense = bphy_initgain_backoff[bphy_idx];
		} else {
			initgain_desense = 0;
		}

#ifdef BCMLTECOEX
		if (phy_ac_btcx_get_data(pi_ac->btcxi)->ltecx_mode == 1)
			initgain_desense = ofdm_desense;
		else
#endif
		initgain_desense = MIN(initgain_desense, max_initgain_desense);
		desense->analog_gain_desense_ofdm = MAX(desense->analog_gain_desense_ofdm,
			MIN(max_anlg_desense, ofdm_desense));


		if (ACPHY_HWACI_WITH_DESENSE_ENG(pi) && CHSPEC_IS2G(pi->radio_chanspec)) {
			desense->clipgain_desense[0] = MAX(desense->clipgain_desense[0],
				initgain_desense);
			if (curr_desense->clipgain_desense[0] != desense->clipgain_desense[0])
			{
				curr_desense->clipgain_desense[0] =
					desense->clipgain_desense[0];
				call_gainctrl = TRUE;
			}
		}

		/* OFDM Desense */
		/* With init gain, max crsmin desense = 30dB, after which ADC will start clipping */
		/* With HI gain, crsmin desense = new_sens - old_sens
		   = (-96 + desense) - (0 - (higain + 27))
		   = -69 + desense + higain
		*/

		/*  In TINY, clipping happens close to -60 dBm. SO Max desense of crsmin_init
		 *  should be increased from 30 to 35 dB.
		 *
		 *  crsmin_high is changed to take care of any change in default HI or INIT gain.
		*/
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			crsmin_init = MIN(33, MAX(0, ofdm_desense - desense->clipgain_desense[0]));
			crsmin_high = MAX(0, ofdm_desense + ACPHY_HI_GAIN_28NM_ULP -
				ACPHY_INIT_GAIN_28NM_ULP - desense->clipgain_desense[1]);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_32))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			uint8 init_gain;

			init_gain = CHSPEC_IS2G(pi->radio_chanspec) ?
					ACPHY_INIT_GAIN_4365_2G : ACPHY_INIT_GAIN_4365_5G;
			crsmin_init = MIN(35, MAX(0, ofdm_desense - desense->clipgain_desense[0]));
			crsmin_high = ofdm_desense + ACPHY_HI_GAIN_TINY -
				init_gain - desense->clipgain_desense[1];
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_32)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			crsmin_init = MIN(35, MAX(0, ofdm_desense - desense->clipgain_desense[0]));
			crsmin_high = MAX(0, ofdm_desense + ACPHY_HI_GAIN_TINY -
				ACPHY_INIT_GAIN_TINY - desense->clipgain_desense[1]);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
			crsmin_init = ofdm_desense - desense->clipgain_desense[0];
			crsmin_high = ofdm_desense + ACPHY_HI_GAIN - 69;
		} else {
			crsmin_init = MAX(0, ofdm_desense - initgain_desense);
			crsmin_high = ofdm_desense + ACPHY_HI_GAIN - 69;
		}
		PHY_ACI(("aci_mode1, desense, init = %d, bphy_idx = %d, crsmin = {%d %d}\n",
		         initgain_desense, bphy_idx, crsmin_init, crsmin_high));

		if ((curr_desense->elna_bypass != desense->elna_bypass) ||
		    (curr_desense->lna1_gainlmt_desense != desense->lna1_gainlmt_desense) ||
		    (curr_desense->lna1_tbl_desense != desense->lna1_tbl_desense) ||
		    (curr_desense->lna1_idx_min != desense->lna1_idx_min) ||
		    (curr_desense->lna1_idx_max != desense->lna1_idx_max) ||
		    (curr_desense->lna1_tbl_desense != desense->lna1_tbl_desense) ||
		    (curr_desense->lna2_tbl_desense != desense->lna2_tbl_desense) ||
		    (curr_desense->lna2_gainlmt_desense != desense->lna2_gainlmt_desense) ||
		    (curr_desense->mixer_setting_desense != desense->mixer_setting_desense) ||
		    (curr_desense->analog_gain_desense_ofdm != desense->analog_gain_desense_ofdm) ||
		    (curr_desense->analog_gain_desense_bphy != desense->analog_gain_desense_bphy)) {
			// TODO: Fix condition above, while ACI shadow init curr_dense == desense
			PHY_ACI(("%s: lna1_tbl_desense: current=%d, wanted=%d\n", __FUNCTION__,
					curr_desense->lna1_tbl_desense, desense->lna1_tbl_desense));
			PHY_ACI(("%s: lna1_idx_max: current=%d, wanted=%d\n", __FUNCTION__,
					curr_desense->lna1_idx_max, desense->lna1_idx_max));
			if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
				/* Update the current structure to hold the new desense values */
				memcpy(curr_desense, desense, sizeof(acphy_desense_values_t));
				wlc_phy_rxgainctrl_set_gaintbls_acphy(pi, TRUE, TRUE, TRUE);
			} else {
				wlc_phy_upd_lna1_lna2_gains_acphy(pi);
			}
			call_gainctrl = TRUE;
		}

		/* if lna1/lna2 gaintable has changed, call gainctrl as it effects all clip gains */
		if (call_gainctrl) {
			if (TINY_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
				if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev))
					wlc_phy_rxgainctrl_gainctrl_acphy_tiny(pi,
						initgain_desense);
				else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
					wlc_phy_rxgainctrl_gainctrl_acphy_tiny(pi, 0);
			} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				wlc_phy_rxgainctrl_gainctrl_acphy_28nm_ulp(pi);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			if (IS_28NM_RADIO(pi)) {
				if (pi_ac->rxgcrsi->rxg_params->mclip_agc_en) {
					wlc_phy_mclip_agc(pi, TRUE, TRUE, TRUE);
				} else {
					PHY_ERROR(("%s: Fast AGC needed for ACMAJORREV_25_40\n",
						__FUNCTION__));
					return;
				}
			} else {
				wlc_phy_rxgainctrl_gainctrl_acphy(pi);
			}
		}

		if (!TINY_RADIO(pi) && !IS_28NM_RADIO(pi)) {
			/* Update INITgain */
			FOREACH_CORE(pi, core) {
				wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0,
				                                            ACPHY_INIT_GAIN
				                                            - initgain_desense,
				                                            desense->elna_bypass,
				                                            FALSE, core);
			}
		}

		/* adjust crsmin threshold, 8 ticks increase gives 3dB rejection */
		crsmin_thresh = ACPHY_CRSMIN_DEFAULT +
						((44 * (2*crsmin_init + ofdm_desense_extra_halfdB)) >> 5);
#ifdef BCMLTECOEX
		if (phy_ac_btcx_get_data(pi_ac->btcxi)->ltecx_mode == 1)
			wlc_phy_set_crs_min_pwr_acphy(pi,
				MAX(crsmin_thresh, ACPHY_CRSMIN_DEFAULT),
					0, 0, 0, 0);
		else
#endif
		if ((ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) &&
			(wlapi_bmac_btc_mode_get(pi->sh->physhim) == 7)) {
			crsmin_thresh = MAX(0, pi->u.pi_acphy->rxgcrsi->phy_crs_th_from_crs_cal +
				((44 * (2*crsmin_init + ofdm_desense_extra_halfdB)) >> 5));  /* init gain */
			wlc_phy_set_crs_min_pwr_acphy(pi, crsmin_thresh,
				0, 0, 0, 0);
		} else {
			wlc_phy_set_crs_min_pwr_acphy(pi, MAX(crsmin_thresh,
				pi->u.pi_acphy->rxgcrsi->phy_crs_th_from_crs_cal),
				0, 0, 0, 0);
		}

		/* crs high_gain */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev)||
			ACMAJORREV_33(pi->pubpi->phy_rev)) {
			crsmin_thresh = MAX(ACPHY_CRSMIN_GAINHI,
				ACPHY_CRSMIN_DEFAULT + ((88 * crsmin_high) >> 5));
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			crsmin_thresh = ACPHY_CRSMIN_DEFAULT
					+ MAX(0, ((88 * crsmin_high) >> 5));
		}
		wlc_phy_set_crs_min_pwr_higain_acphy(pi, crsmin_thresh);

		/* bphy desense */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (IS_28NM_RADIO(pi)) {
				WRITE_PHYREG(pi, DigiGainLimit0,
					0xC400 | bphy_minshiftbits[bphy_idx]);
			} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				digigainlimit = READ_PHYREG(pi, DigiGainLimit0) & 0x8000;
				WRITE_PHYREG(pi, DigiGainLimit0,
					digigainlimit | 0x4400 | bphy_minshiftbits[bphy_idx]);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				WRITE_PHYREG(pi,  DigiGainLimit0,
					0x4400 | bphy_minshiftbits[bphy_idx]);
			}
			WRITE_PHYREG(pi, PeakEnergyL, bphy_peakenergy[bphy_idx]);
		}
		wlc_phy_desense_print_phyregs_acphy(pi, "apply");
	}

	/* Inform rate contorl to slow down is mitigation is on */
	wlc_phy_aci_updsts_acphy(pi);

	wlapi_enable_mac(pi->sh->physhim);
}

void
wlc_phy_desense_calc_total_acphy(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	phy_info_t *pi = rxgcrsi->pi;

#ifdef BCMLTECOEX
	acphy_desense_values_t *bt, *aci, *lte;
#else
	acphy_desense_values_t *bt, *aci;
#endif
	uint8 i;

	acphy_desense_values_t *total = rxgcrsi->total_desense;

	if ((aci = phy_ac_noise_get_desense(rxgcrsi->aci->noisei)) == NULL) {
		aci = rxgcrsi->zero_desense;
	}

	/* if desense is forced, then skip without calculation and just use the forced value */
	if (total->forced)
		return;

#ifdef BCMLTECOEX
	if ((phy_ac_btcx_get_data(rxgcrsi->aci->btcxi)->btc_mode == 0 &&
		phy_ac_btcx_get_data(rxgcrsi->aci->btcxi)->ltecx_mode == 0) ||
		wlc_phy_is_scan_chan_acphy(pi) || CHSPEC_IS5G(pi->radio_chanspec) ||
		ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		/* only consider aci desense */
		memcpy(total, aci, sizeof(acphy_desense_values_t));
	} else {
		/* Merge BT & ACI & LTE desense, take max */
		bt  = rxgcrsi->bt_desense;
		lte = rxgcrsi->lte_desense;
		total->ofdm_desense = MAX(MAX(aci->ofdm_desense, bt->ofdm_desense),
				lte->ofdm_desense);
		total->ofdm_desense_extra_halfdB = aci->ofdm_desense_extra_halfdB;
		total->bphy_desense = MAX(MAX(aci->bphy_desense, bt->bphy_desense),
			lte->bphy_desense);
		total->lna1_tbl_desense = MAX(MAX(aci->lna1_tbl_desense, bt->lna1_tbl_desense),
			lte->lna1_tbl_desense);
		total->analog_gain_desense_ofdm = MAX(MAX(aci->analog_gain_desense_ofdm,
			bt->analog_gain_desense_ofdm), lte->analog_gain_desense_ofdm);
		total->analog_gain_desense_bphy = MAX(MAX(aci->analog_gain_desense_bphy,
			bt->analog_gain_desense_bphy), lte->analog_gain_desense_bphy);
		total->lna2_tbl_desense = MAX(MAX(aci->lna2_tbl_desense, bt->lna2_tbl_desense),
			lte->lna2_tbl_desense);
		total->lna1_gainlmt_desense = MAX(MAX(aci->lna1_gainlmt_desense,
			bt->lna1_gainlmt_desense), lte->lna1_gainlmt_desense);
		total->lna2_gainlmt_desense = MAX(MAX(aci->lna2_gainlmt_desense,
			bt->lna2_gainlmt_desense), lte->lna2_gainlmt_desense);
		total->elna_bypass = MAX(MAX(aci->elna_bypass, bt->elna_bypass),
			lte->elna_bypass);
		total->mixer_setting_desense = MAX(MAX(aci->mixer_setting_desense,
			bt->mixer_setting_desense), lte->mixer_setting_desense);
		total->nf_hit_lna12 =  MAX(MAX(aci->nf_hit_lna12, bt->nf_hit_lna12),
			lte->nf_hit_lna12);
		total->on = aci->on | bt->on | lte->on;
		for (i = 0; i < 4; i++)
			total->clipgain_desense[i] = MAX(MAX(aci->clipgain_desense[i],
				bt->clipgain_desense[i]), lte->clipgain_desense[i]);
	}
#else
	if ((phy_ac_btcx_get_data(rxgcrsi->aci->btcxi)->btc_mode == 0) ||
		wlc_phy_is_scan_chan_acphy(pi) || CHSPEC_IS5G(pi->radio_chanspec) ||
		ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		/* only consider aci desense */
		memcpy(total, aci, sizeof(acphy_desense_values_t));
	} else {
		/* Merge BT & ACI desense, take max */
		bt  = rxgcrsi->bt_desense;
		total->ofdm_desense = MAX(aci->ofdm_desense, bt->ofdm_desense);
		total->ofdm_desense_extra_halfdB = aci->ofdm_desense_extra_halfdB;

		total->bphy_desense = MAX(aci->bphy_desense, bt->bphy_desense);
		total->analog_gain_desense_ofdm = MAX(aci->analog_gain_desense_ofdm,
			bt->analog_gain_desense_ofdm);
		total->analog_gain_desense_bphy = MAX(aci->analog_gain_desense_bphy,
			bt->analog_gain_desense_bphy);
		total->lna1_tbl_desense = MAX(aci->lna1_tbl_desense, bt->lna1_tbl_desense);
		total->lna2_tbl_desense = MAX(aci->lna2_tbl_desense, bt->lna2_tbl_desense);
		total->lna1_gainlmt_desense =
		  MAX(aci->lna1_gainlmt_desense, bt->lna1_gainlmt_desense);
		total->lna2_gainlmt_desense =
		  MAX(aci->lna2_gainlmt_desense, bt->lna2_gainlmt_desense);
		total->mixer_setting_desense =
		  MAX(aci->mixer_setting_desense, bt->mixer_setting_desense);
		total->elna_bypass = MAX(aci->elna_bypass, bt->elna_bypass);
		total->nf_hit_lna12 =  MAX(aci->nf_hit_lna12, bt->nf_hit_lna12);
		total->on = aci->on | bt->on;
		for (i = 0; i < 4; i++)
			total->clipgain_desense[i] = MAX(aci->clipgain_desense[i],
				bt->clipgain_desense[i]);
	}
#endif /* BCMLTECOEX */

	/* uCode detected high pwr RSSI. Time to save ilna1 */
	if (phy_ac_hirssi_set(pi) == TRUE)
		total->elna_bypass = TRUE;

}
#endif /* #ifndef WLC_DISABLE_ACI */

void
wlc_phy_rxgainctrl_gainctrl_acphy(phy_info_t *pi)
{
	bool elna_present;
	bool init_trtx, hi_trtx, md_trtx, lo_trtx, lna1byp;
	uint8 init_gain = ACPHY_INIT_GAIN, hi_gain = ACPHY_HI_GAIN;
	uint8 mid_gain = 35, lo_gain, clip2_gain;
	uint8 hi_gain1, mid_gain1, lo_gain1;
	/* 1% PER point used for all the PER numbers */

	/* For bACI/ACI: max output pwrs {elna, lna1, lna2, mix, bq0, bq1, dvga} */
	uint8 maxgain_2g[] = {43, 43, 43, 52, 52, 100, 0};
	uint8 maxgain_5g[] = {47, 47, 47, 52, 52, 100, 0};

	uint8 i, core, elna_idx;
	int8 md_low_end, hi_high_end, lo_low_end, md_high_end, max_himd_hi_end;
	int8 nbclip_pwrdBm, w1clip_pwrdBm;
	phy_ac_rxgcrs_info_t *ri = pi->u.pi_acphy->rxgcrsi;
	bool elnabyp_en = phy_ac_hirssi_get(pi);
	uint16 LPFgain, RFgain, LNARout, lna1idx, lna2idx;
	uint8 lna1Rout, lna2Rout;

	ri->mdgain_trtx_allowed = TRUE;
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
			ri->rxgainctrl_maxout_gains[i] = maxgain_2g[i];
		elna_present = BF_ELNA_2G(ri->aci);
	} else {
		for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
			ri->rxgainctrl_maxout_gains[i] = maxgain_5g[i];
		elna_present = BF_ELNA_5G(ri->aci);
	}

	/* Keep track of it (used in interf_mitigation) */
	ri->curr_desense->elna_bypass = wlc_phy_get_max_lna_index_acphy(pi, ELNA_ID);
	init_trtx = elna_present & ri->curr_desense->elna_bypass;
	hi_trtx = elna_present & ri->curr_desense->elna_bypass;
	md_trtx = elna_present & (ri->mdgain_trtx_allowed | ri->curr_desense->elna_bypass);
	lo_trtx = TRUE;

	/* with elna if md_gain uses TR != T, then LO_gain needs to be higher */
	if (elnabyp_en)
		lo_gain = ((!elna_present) || md_trtx) ? 15 : 30;
	else
		lo_gain = ((!elna_present) || md_trtx) ? 20 : 30;
	clip2_gain = md_trtx ? lo_gain : (mid_gain + lo_gain) >> 1;

	FOREACH_CORE(pi, core) {
		lna1byp = ri->fem_rxgains[core].lna1byp;
		if (lna1byp) {
			mid_gain = 35;
			lo_gain = 15;
			PHY_INFORM((" iPa chip, set lo_gain = 15 \n"));
			hi_gain = 48;
		}
		elna_idx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initExtLnaIndex);
		max_himd_hi_end =
			- 16 - ri->rxgainctrl_params[core].gaintbl[0][elna_idx];
		if (ri->curr_desense->elna_bypass == 1)
			max_himd_hi_end += ri->fem_rxgains[core].trloss;

		/* 0,1,2,3 for Init, hi, md and lo gains respectively */
		wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0, init_gain, init_trtx,
			FALSE, core);
		hi_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 1, hi_gain,
			hi_trtx, FALSE, core);
		mid_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 2, mid_gain,
			md_trtx, FALSE, core);
		wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 4, clip2_gain, md_trtx,
			FALSE, core);
		lo_gain1 = wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 3, lo_gain,
			lo_trtx, lna1byp, core);

		/* NB_CLIP */
		md_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, mid_gain1, md_trtx, 0,
				core);
		hi_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, hi_gain1, hi_trtx, 0,
				core);
		lo_low_end = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, lo_gain1, lo_trtx, 0,
				core);
		md_high_end = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, mid_gain1, md_trtx, 0,
				core);

		/* -1 times pwr to avoid rounding off error */
		if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
			/* use mid-point, as late clip2 is causing minor humps,
			   move nb/w1 clip to a lower pwr
			*/
			nbclip_pwrdBm = (md_low_end + hi_high_end) >> 1;
		} else {
			/* (0.4*md/lo + 0.6*hi/md) fixed point. On fading channels,
			   low-end sensitivity degrades, keep more margin there.
			*/
			nbclip_pwrdBm = (((-2*md_low_end)+(-3*hi_high_end)) * 13) >> 6;
			nbclip_pwrdBm *= -1;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_5)))
			if (CHSPEC_IS80(pi->radio_chanspec) &&
			    ((ACMAJORREV_2(pi->pubpi->phy_rev)) ||
			     ACMAJORREV_5(pi->pubpi->phy_rev))) {
				nbclip_pwrdBm -= 4;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_5))) */
			if (CHSPEC_IS80(pi->radio_chanspec) &&
			           ((RADIOID(pi->pubpi->radioid) == BCM2069_ID && PHY_ILNA(pi)) &&
			           (RADIOREV(pi->pubpi->radiorev) == 0x2C))) {
				/* to cure the 20ll PER hump issue for 43569a2/43570a2
				 * http://confluence.broadcom.com/x/NC-UEg
				 */
				nbclip_pwrdBm -= 6;
			}
		}

		if (CHSPEC_BW_LE20(pi->radio_chanspec) ||
		    (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi) && !PHY_ILNA(pi))) {
			if (elnabyp_en) {
				w1clip_pwrdBm = (((-2*lo_low_end) + (-3*md_high_end)) * 13) >> 6;
				w1clip_pwrdBm *= -1;
			} else {
				w1clip_pwrdBm = (lo_low_end + md_high_end) >> 1;
			}
		} else {
			w1clip_pwrdBm = (((-2*lo_low_end) + (-3*md_high_end)) * 13) >> 6;
			w1clip_pwrdBm *= -1;
		}
		if ((ACMAJORREV_2(pi->pubpi->phy_rev) && !ACMINORREV_3(pi) &&
			!ACMINORREV_5(pi) && !PHY_ILNA(pi)) &&
			!(CHSPEC_IS2G(pi->radio_chanspec) && ri->w1clipmod)) {
			/* Do not increase w1clip to prevent LNA1 clamping for 2G */
			w1clip_pwrdBm += 3;
		}

		wlc_phy_rxgainctrl_nbclip_acphy(pi, core, nbclip_pwrdBm);
		wlc_phy_rxgainctrl_w1clip_acphy(pi, core, w1clip_pwrdBm);

		/* 2G/5G VHT20 hump in eLNA, WAR for 43162 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
		if (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi) &&
		     !PHY_ILNA(pi) && CHSPEC_IS20(pi->radio_chanspec) && PHY_XTAL_IS40M(pi)) {
			if (CHSPEC_IS5G(pi->radio_chanspec))
				nbclip_pwrdBm = ((md_low_end + hi_high_end) >> 1) - 5;
			else
				nbclip_pwrdBm = ((md_low_end + hi_high_end) >> 1) - 4;

			wlc_phy_rxgainctrl_nbclip_acphy(pi, core, nbclip_pwrdBm);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	}
	/* After comming out of this loop, gain ctrl is done. So, values of Init gain in the phyregs
	 * are the correct/default ones. These should be the ones that should be used in
	 * "wlc_phy_get_rxgain_acphy" function (used in rxiqest). So, we have to save the default
	 * Init gain phyregs in some variables. We assume, that both core's init gains will be same
	 * and thus save only core 0's init gain and this should be fine for single core chips too.
	 */
	ri->initGain_codeA = READ_PHYREGC(pi, InitGainCodeA, 0);
	ri->initGain_codeB = READ_PHYREGC(pi, InitGainCodeB, 0);
	LPFgain  = (((ri->initGain_codeB) & 0x700) >> 0x5) |
		(((ri->initGain_codeB) & 0x70) >> 0x4);
	RFgain = ((ri->initGain_codeA) >> 1) & 0x3FF;
	lna1idx = RFgain & 0x7;
	lna2idx = (RFgain >> 3) & 0x7;
	wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, lna1idx, 8, &lna1Rout);
	wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, (16 + lna2idx), 8, &lna2Rout);
	LNARout = ((lna2Rout & 0xF0) | (lna1Rout >>4)) & 0xFF;
	wlapi_bmac_write_shm(pi->sh->physhim, M_RXGAIN_HI(pi), RFgain);
	wlapi_bmac_write_shm(pi->sh->physhim, M_LPFGAIN_HI(pi), LPFgain);
	wlapi_bmac_write_shm(pi->sh->physhim, M_LNA_ROUT(pi), LNARout);
}

void
wlc_phy_upd_lna1_lna2_gains_acphy(phy_info_t *pi)
{
	wlc_phy_upd_lna1_lna2_gainlimittbls_acphy(pi, 1);
	wlc_phy_upd_lna1_lna2_gainlimittbls_acphy(pi, 2);
	wlc_phy_upd_lna1_lna2_gaintbls_acphy(pi, 1);
	wlc_phy_upd_lna1_lna2_gaintbls_acphy(pi, 2);
}

void
wlc_phy_upd_lna1_lna2_gaintbls_acphy(phy_info_t *pi, uint8 lna12)
{
	uint8 offset, sz, core, desense_state;
	uint8 gaintbl[10];
	uint8 gainbitstbl[10];
	uint8 max_idx, min_idx;
	const uint8 *default_gaintbl = NULL;
	uint16 gain_tblid, gainbits_tblid;
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;
	acphy_desense_values_t *desense = rxgcrsi->total_desense;
	phy_ac_rxg_params_t *rxg_params = rxgcrsi->rxg_params;
	uint8 lna1rout_tbl[6], lna1rout_offset;
	uint8 lna1, lna1_idx, lna1_rout;
	uint8 stall_val;
	uint8 i;

	if ((lna12 < LNA1_ID) || (lna12 > LNA2_ID)) return;

	stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
	ACPHY_DISABLE_STALL(pi);

	sz = rxgcrsi->rxgainctrl_stage_len[lna12];

	desense_state = phy_ac_noise_get_desense_state(pi->u.pi_acphy->noisei);

	if (lna12 == LNA1_ID) {          /* lna1 */
		offset = 8;
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (IS_28NM_RADIO(pi)) {
				default_gaintbl = (uint8 *)(rxg_params->lna12_gain_2g_tbl[0]);
			} else	if (!TINY_RADIO(pi)) {
				if (PHY_ILNA(pi)) { /* iLNA only chip */
					default_gaintbl = ac_lna1_2g_43352_ilna;
				} else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
					if (BF3_LTECOEX_GAINTBL_EN(rxgcrsi->aci) == 1 &&
					    ACMAJORREV_1(pi->pubpi->phy_rev)) {
						default_gaintbl = ac_lna1_2g_ltecoex;
					} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
					{
						default_gaintbl = ac_lna1_2g;
					}
				}
			} else {
				default_gaintbl = ac_lna1_2g_tiny;
			}
		} else if (IS_28NM_RADIO(pi)) {
			default_gaintbl = (uint8 *)(rxg_params->lna12_gain_5g_tbl[0]);
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			default_gaintbl = ac_lna1_5g_4365;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			default_gaintbl = (TINY_RADIO(pi)) ? ac_lna1_5g_tiny : ac_lna1_5g;
		}
	} else {  /* lna2 */
		offset = 16;
		if (TINY_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				default_gaintbl = CHSPEC_IS2G(pi->radio_chanspec) ?
				        ac_lna2_tiny_4365_2g : ac_lna2_tiny_4365_5g;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
			if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
				default_gaintbl = ac_lna2_tiny_4349;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
			if (ACMAJORREV_3(pi->pubpi->phy_rev) &&
				ACREV_GE(pi->pubpi->phy_rev, 11) &&
				PHY_ILNA(pi)) {
				default_gaintbl = ac_lna2_tiny_ilna_dcc_comp;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */
			{
				default_gaintbl = ac_lna2_tiny;
			}
		} else if (IS_28NM_RADIO(pi)) {
			default_gaintbl = (uint8 *)(CHSPEC_IS2G(pi->radio_chanspec) ?
					rxg_params->lna12_gain_2g_tbl[1] :
					rxg_params->lna12_gain_5g_tbl[1]);
		} else if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (PHY_ILNA(pi)) { /* iLNA only chip */
				default_gaintbl = ac_lna2_2g_43352_ilna;
			} else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
				if (BF3_LTECOEX_GAINTBL_EN(rxgcrsi->aci) == 1 &&
				    ACMAJORREV_1(pi->pubpi->phy_rev)) {
					default_gaintbl = ac_lna2_2g_ltecoex;
				} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
				{
					default_gaintbl = (desense->elna_bypass)
						? ac_lna2_2g_gm3 : ac_lna2_2g_gm2;
				}
			}
		} else {
			default_gaintbl = ac_lna2_5g;
		}
		memcpy(rxgcrsi->lna2_complete_gaintbl, default_gaintbl, sizeof(uint8)*sz);
	}

	max_idx = wlc_phy_get_max_lna_index_acphy(pi, lna12);
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		if (desense_state != 0)
			min_idx = (lna12 == LNA1_ID) ? desense->lna1_idx_min: desense->lna2_idx_min;
		else
			min_idx = (lna12 == LNA1_ID) ? 0 : max_idx;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	if (IS_28NM_RADIO(pi)) {
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			min_idx = (lna12 == LNA1_ID) ? rxg_params->lna1_min_indx :
				(ACPHY_LO_NF_MODE_ELNA_28NM(pi) ?
					rxg_params->lna2_lonf_force_indx_2g :
					rxg_params->lna2_2g_min_indx);
		} else {
			min_idx = (lna12 == LNA1_ID) ? rxg_params->lna1_min_indx :
				(ACPHY_LO_NF_MODE_ELNA_28NM(pi) ?
					rxg_params->lna2_lonf_force_indx_5g :
					rxg_params->lna2_5g_min_indx);
		}
	} else {
		min_idx = ACMAJORREV_4(pi->pubpi->phy_rev) ? max_idx
			: ((pi->pubpi->phy_rev == 0) ? 1 : 0);
	}
	PHY_ACI(("%s: Limit gaintable lna=%d, min_idx=%d, max_idx=%d\n",
	         __FUNCTION__, lna12, min_idx, max_idx));
	wlc_phy_limit_rxgaintbl_acphy(gaintbl, gainbitstbl, sz, default_gaintbl, min_idx, max_idx);

	/* Update rxgcrsi->curr_desense (used in interf_mitigation) */
	if (lna12 == LNA1_ID) {
		rxgcrsi->curr_desense->lna1_tbl_desense = desense->lna1_tbl_desense;
	} else {
		rxgcrsi->curr_desense->lna2_tbl_desense = desense->lna2_tbl_desense;
	}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (BF3_LTECOEX_GAINTBL_EN(rxgcrsi->aci) == 1 && ACMAJORREV_1(pi->pubpi->phy_rev)) {
		uint8 lna1Rout = 0x25;
		uint8 lna2Rout = 0x44;

		/* for now changing rout of lna1 and lna2 to achieve */
		/* lower gain of 3dB for the init gain index only */
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, 5, 8, &lna1Rout);
		MOD_RADIO_REG(pi, RFX, OVR6, ovr_lna2g_lna1_Rout, 0);
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, 20, 8, &lna2Rout);
		MOD_RADIO_REG(pi, RFX, OVR6, ovr_lna2g_lna2_Rout, 0);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	if (TINY_RADIO(pi)) {
		uint8 x;
		uint8 lnarout_val;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_4(pi->pubpi->phy_rev) || ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev)) {
			uint8  lnarout_val2G, lnarout_val5G, lna2rout_val;
			FOREACH_CORE(pi, core) {
				for (x = 0; x < 6; x++) {
					lnarout_val2G = (ac_tiny_g_lna_rout_map[x] << 3) |
						ac_tiny_g_lna_gain_map[x];
					if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
						ACMAJORREV_33(pi->pubpi->phy_rev))
						lnarout_val5G = (ac_4365_a_lna_rout_map[x] << 3) |
						        ac_tiny_a_lna_gain_map[x];
					else
						lnarout_val5G = (ac_tiny_a_lna_rout_map[x] << 3) |
							ac_tiny_a_lna_gain_map[x];
					lnarout_val = CHSPEC_IS5G(pi->radio_chanspec) ?
						lnarout_val5G : lnarout_val2G;
					wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1,
						(ACPHY_LNAROUT_BAND_OFFSET(pi,
						pi->radio_chanspec)	+ x +
						ACPHY_LNAROUT_CORE_WRT_OFST(pi->pubpi->phy_rev,
						core)),	8, &lnarout_val);
					if (!ACMAJORREV_32(pi->pubpi->phy_rev) &&
						!ACMAJORREV_33(pi->pubpi->phy_rev)) {
						lna2rout_val =  wlc_phy_get_max_lna_index_acphy(pi,
							LNA2_ID);
						wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT,
							1, (x + 16 +
							ACPHY_LNAROUT_CORE_WRT_OFST(
							pi->pubpi->phy_rev, core)), 8,
							&lna2rout_val);
					}
				}
				MOD_PHYREGCE(pi, RfctrlCoreRXGAIN1, core, rxrf_lna2_gain,
					wlc_phy_get_max_lna_index_acphy(pi, LNA2_ID));
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			/* 2G index is 0->5 */
			for (x = 0; x < 6; x++) {
				lnarout_val = (ac_tiny_g_lna_rout_map[x] << 3) |
					ac_tiny_g_lna_gain_map[x];
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, x, 8,
					&lnarout_val);
			}

			/* 5G index is 8->13 */
			for (x = 0; x < 6; x++) {
				lnarout_val =  (ac_tiny_a_lna_rout_map[x] << 3) |
					ac_tiny_a_lna_gain_map[x];
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, 8 + x, 8,
				                          &lnarout_val);
			}
		}
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		uint8 idx;
		uint8 *lna2_gm_ind = CHSPEC_IS2G(pi->radio_chanspec) ?
				rxg_params->lna2_gm_ind_2g_tbl : rxg_params->lna2_gm_ind_5g_tbl;

		/* 2G index is 0->5 */
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 6, 0, 8,
				rxg_params->lna1Rout_2g_tbl);

		/* 5G index is 8->13 */
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 6, 8, 8,
				rxg_params->lna1Rout_5g_tbl);

		for (i = 0; i < 7; i++) {
			/* LNA2 gm indexes 16->22 */
			idx = ACPHY_LO_NF_MODE_ELNA_28NM(pi) ?
				(CHSPEC_IS2G(pi->radio_chanspec) ?
				rxg_params->lna2_lonf_force_indx_2g :
				rxg_params->lna2_lonf_force_indx_5g) : i;
			wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 7, (16 + i), 8,
					&lna2_gm_ind[idx]);
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
	}

	/* Update gaintbl */
	FOREACH_CORE(pi, core) {
		if (core == 0) {
			gain_tblid =  ACPHY_TBL_ID_GAIN0;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS0;
		} else if (core == 1) {
			gain_tblid =  ACPHY_TBL_ID_GAIN1;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS1;
		} else if (core == 2) {
			gain_tblid =  ACPHY_TBL_ID_GAIN2;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS2;
		} else {
			gain_tblid =  ACPHY_TBL_ID_GAIN3;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS3;
		}

		if (!TINY_RADIO(pi) && !IS_28NM_RADIO(pi)) {
			lna1rout_offset = core * 24;
			if (CHSPEC_IS5G(pi->radio_chanspec))
				lna1rout_offset += 8;

			if (lna12 == LNA1_ID) {
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT, 6,
				                         lna1rout_offset, 8, lna1rout_tbl);
				for (i = 0; i < 6; i++) {
					lna1 = lna1rout_tbl[gainbitstbl[i]];
					lna1_idx = lna1 & 0x7;
					lna1_rout = (lna1 >> 3) & 0xf;
					gaintbl[i] = CHSPEC_IS2G(pi->radio_chanspec) ?
					        default_gaintbl[lna1_idx] -
					        ac_lna1_rout_delta_2g[lna1_rout]:
					        default_gaintbl[lna1_idx] -
					        ac_lna1_rout_delta_5g[lna1_rout];
				}
			}
		}

		memcpy(rxgcrsi->rxgainctrl_params[core].gaintbl[lna12],
			gaintbl, sizeof(uint8)*sz);
		wlc_phy_table_write_acphy(pi, gain_tblid, sz, offset, 8, gaintbl);
		memcpy(rxgcrsi->rxgainctrl_params[core].gainbitstbl[lna12], gainbitstbl,
			sizeof(uint8)*sz);
		wlc_phy_table_write_acphy(pi, gainbits_tblid, sz, offset, 8, gainbitstbl);
	}

	ACPHY_ENABLE_STALL(pi, stall_val);
}

void
wlc_phy_upd_lna1_lna2_gainlimittbls_acphy(phy_info_t *pi, uint8 lna12)
{
	uint8 i, sz, max_idx, core, lna2_limit_size, lna1_rout = 0;
	uint8 lna1rout_tbl[N_LNA12_GAINS];
	uint8 max_idx_pktgain_limit, max_idx_non_init_lna1rout_tbl, lna1rout_offset;
	uint8 lna1_tbl[] = {11, 12, 14, 32, 36, 40};
	uint8 lna2_tbl[] = {0, 0, 0, 3, 3, 3, 3};
	uint8 tiny_lna2_tbl[] = {127, 127, 127, 127};
	uint8 tiny_tia_tbl[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127};
	uint8 *tiny_gainlmt_lna2, *tiny_gainlmt_tia;
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;
	acphy_desense_values_t *desense = rxgcrsi->total_desense;
	uint16 gainlimitid;

	if (!TINY_RADIO(pi) && !IS_28NM_RADIO(pi)) {
		sz = (lna12 == LNA1_ID) ? 6 : 7;

		/* Limit based on desense mitigation mode */
		if (lna12 == LNA1_ID) {
			max_idx = MAX(0, (sz - 1) - desense->lna1_gainlmt_desense);
			rxgcrsi->curr_desense->lna1_gainlmt_desense = desense->lna1_gainlmt_desense;

			max_idx_pktgain_limit = max_idx;
			/* if rout desense is active, we have to change the non-init gain entries of
			 * lna1_rout tbl and modify pkt gain limit tbl to not use init-gain instead
			 * of just limiting LNA1 gain in the pkt gain limit tbls
			 */
			max_idx_non_init_lna1rout_tbl = (desense->lna1rout_gainlmt_desense > 0) ?
			        max_idx : (sz - 2);

			/* LNA1 gain in pktgain limit tbl has to be capped
			   only to not use init gain
			*/
			max_idx_pktgain_limit = (desense->lna1rout_gainlmt_desense > 0) ?
			        (sz - 2) : max_idx;
			lna1_rout = CHSPEC_IS2G(pi->radio_chanspec) ?
			        0 + desense->lna1rout_gainlmt_desense :
			        4 - desense->lna1rout_gainlmt_desense;
			for (i = 0; i <= sz - 2; i++) {
				lna1rout_tbl[i] = (lna1_rout << 3) |
				        MAX(0, max_idx_non_init_lna1rout_tbl - (sz - 2 - i));
			}

			if (!(ACREV_IS(pi->pubpi->phy_rev, 0) || TINY_RADIO(pi))) {
				/* WRITE the lna1 rout table (only first 5 entries) */
				FOREACH_CORE(pi, core) {
					lna1rout_offset = core * 24;
					if (CHSPEC_IS5G(pi->radio_chanspec))
						lna1rout_offset += 8;
					wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, sz-1,
					                          lna1rout_offset, 8, lna1rout_tbl);
				}
			}
		} else {
			max_idx_pktgain_limit = MAX(0, (sz - 1) - desense->lna2_gainlmt_desense);
			rxgcrsi->curr_desense->lna2_gainlmt_desense = desense->lna2_gainlmt_desense;
		}

		/* Write 0x7f to entries not to be used */
		for (i = (max_idx_pktgain_limit + 1); i < sz; i++) {
			if (lna12 == LNA1_ID) {
				lna1_tbl[i] = 0x7f;
			} else {
				lna2_tbl[i] = 0x7f;
			}
		}

		if (lna12 == LNA1_ID) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
			if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, sz,
					8, 8,  lna1_tbl);
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT1, sz,
					8, 8,  lna1_tbl);
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT2, sz,
					8, 8,  lna1_tbl);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25))
			if (ACMAJORREV_25(pi->pubpi->phy_rev)) {
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, sz,
					8, 8,  lna1_tbl);
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT1, sz,
					8, 8,  lna1_tbl);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25)) */
			{
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT, sz,
					8, 8, lna1_tbl);
			}
			/* 4335C0: This is for DSSS_CCK packet gain limit */
			if (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi))
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT, sz, 72, 8,
				                          lna1_tbl);
		}
		else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
			if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, sz,
					16, 8, lna2_tbl);
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT1, sz,
					16, 8, lna2_tbl);
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT2, sz,
					16, 8, lna2_tbl);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
			{
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT, sz,
					16, 8, lna2_tbl);
			}
			/* 4335C0: This is for DSSS_CCK packet gain limit */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_2)))
			if (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi))
				wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT, sz,
					80, 8, lna2_tbl);
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_2))) */
		}
	} else {
		tiny_gainlmt_lna2 = tiny_lna2_tbl;
		tiny_gainlmt_tia = tiny_tia_tbl;

		FOREACH_CORE(pi, core) {
			if (ACMAJORREV_4(pi->pubpi->phy_rev) || IS_28NM_RADIO(pi)) {
				if (core == 0)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT0;
				else if (core == 1)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT1;
				else if (core == 2)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT2;
				else /* (core == 3) */
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT3;

				/* get max lna2 indx */
				lna2_limit_size =
					wlc_phy_get_max_lna_index_acphy(pi, LNA2_ID) + 1;
			} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				uint8 tia_gainlmt_4365[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127};
				uint8 lna2_gainlmt_4365[] = {0, 0, 0, 0, 0, 0, 0};
				/* Use different for 4365 */
				tiny_gainlmt_lna2 = lna2_gainlmt_4365;
				tiny_gainlmt_tia = tia_gainlmt_4365;
				if (core == 0)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT0;
				else if (core == 1)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT1;
				else if (core == 2)
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT2;
				else /* (core == 3) */
					gainlimitid = ACPHY_TBL_ID_GAINLIMIT3;
				/* get max lna2 indx */
				lna2_limit_size = wlc_phy_get_max_lna_index_acphy(pi, LNA2_ID) + 1;
				lna2_limit_size = 7;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				gainlimitid = ACPHY_TBL_ID_GAINLIMIT;
				lna2_limit_size = 2;
			}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				phy_ac_rxg_params_t *rxg_params = rxgcrsi->rxg_params;
				if (lna12 == LNA1_ID) {
					wlc_phy_table_write_acphy(pi, gainlimitid, 8,
						8, 8, rxg_params->gainlimit_tbl[LNA1_TBL_IND]);
					wlc_phy_table_write_acphy(pi, gainlimitid, 8,
						8+64, 8, rxg_params->gainlimit_tbl[LNA1_TBL_IND]);
				} else {
					wlc_phy_table_write_acphy(pi, gainlimitid, 8,
						16, 8, rxg_params->gainlimit_tbl[LNA2_TBL_IND]);
					wlc_phy_table_write_acphy(pi, gainlimitid, 8,
						16+64, 8, rxg_params->gainlimit_tbl[LNA2_TBL_IND]);
					wlc_phy_table_write_acphy(pi, gainlimitid,
						12, 32, 8, rxg_params->gainlimit_tbl[TIA_TBL_IND]);
					wlc_phy_table_write_acphy(pi, gainlimitid, 12,
						32+64, 8, rxg_params->gainlimit_tbl[TIA_TBL_IND]);
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			if (IS_28NM_RADIO(pi)) {
				wlc_phy_upd_lna1_lna2_gainlimittbls_28nm(pi, lna12,
					gainlimitid, core);
			} else {
				wlc_phy_table_write_acphy(pi, gainlimitid, lna2_limit_size,
					16, 8, tiny_gainlmt_lna2);
				wlc_phy_table_write_acphy(pi, gainlimitid, lna2_limit_size,
					16+64, 8, tiny_gainlmt_lna2);
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
				if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) {
					wlc_phy_table_write_acphy(pi, gainlimitid,
						13, 32, 8, tiny_gainlmt_tia);
					wlc_phy_table_write_acphy(pi, gainlimitid,
						13, 32+64, 8, tiny_gainlmt_tia);
				} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
				{
					wlc_phy_table_write_acphy(pi, gainlimitid,
						12, 32, 8, tiny_gainlmt_tia);
					wlc_phy_table_write_acphy(pi, gainlimitid,
						12, 32+64, 8, tiny_gainlmt_tia);
				}
			}
		}
	}
}

void
wlc_phy_upd_lna1_lna2_gainlimittbls_28nm(phy_info_t *pi, uint8 lna12, uint16 gainlimitid,
	uint8 core)
{
	uint8 i, sz, max_idx, lna1_rout = 0, lna1_gain;
	uint8 lna1rout_tbl[N_LNA12_GAINS];
	uint8 max_idx_pktgain_limit, max_idx_non_init_lna1rout_tbl, lna1rout_offset;
	uint8 lna1_tbl[] = {11, 12, 14, 32, 36, 40};
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;
	acphy_desense_values_t *desense = rxgcrsi->total_desense;
	phy_ac_rxg_params_t *rxg_params = rxgcrsi->rxg_params;

	if (lna12 == LNA1_ID) {
		/* Size for LNA1 gain table */
		sz =  6;

		/* Copy default gain limit table */
		memcpy(lna1_tbl, rxg_params->gainlimit_tbl[LNA1_TBL_IND],
			sizeof(uint8)*sz);
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			memcpy(lna1rout_tbl, rxg_params->lna1Rout_2g_tbl,
				sizeof(uint8)*sz);
		} else {
			memcpy(lna1rout_tbl, rxg_params->lna1Rout_5g_tbl,
				sizeof(uint8)*sz);
		}

		// Limit based on desense mitigation mode
		// Two ways to limit the gain, either through lna1_idx_max or lna1_gainlmt_desense
		max_idx = MAX(0, (sz - 1) - desense->lna1_gainlmt_desense);
		rxgcrsi->curr_desense->lna1_gainlmt_desense =
			desense->lna1_gainlmt_desense;

		if (max_idx > desense->lna1_idx_max) {
			max_idx = desense->lna1_idx_max;
		}
		rxgcrsi->curr_desense->lna1_idx_max =
			desense->lna1_idx_max;

		/* if rout desense is active, we have to change the non-init
		 * gain entries of lna1_rout tbl and modify pkt gain limit
		 * tbl to not use init-gain instead of just limiting LNA1
		 * gain in the pkt gain limit tbls
		 */
		max_idx_non_init_lna1rout_tbl =
			(desense->lna1rout_gainlmt_desense > 0) ?
			max_idx : (sz - 2);

		/* LNA1 gain in pktgain limit tbl has to be capped
		   only to not use init gain
		*/
		max_idx_pktgain_limit =
			(desense->lna1rout_gainlmt_desense > 0) ?
			(sz - 2) : max_idx;

		for (i = 0; i < max_idx_non_init_lna1rout_tbl; i++) {
			lna1_rout = wlc_phy_get_lna_gain_rout(pi, i,
					GET_LNA_ROUT);
			lna1_gain = wlc_phy_get_lna_gain_rout(pi, i,
					GET_LNA_GAINCODE);
			lna1_gain -= desense->lna1rout_gainlmt_desense;
			lna1rout_tbl[i] = (lna1_rout << 3) | lna1_gain;
		}

		/* WRITE the lna1 rout table (only first 5 entries) */
		lna1rout_offset = core * 24;
		if (CHSPEC_IS5G(pi->radio_chanspec))
			lna1rout_offset += 8;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT,
			sz, lna1rout_offset, 8, lna1rout_tbl);

		/* Write 0x7f to entries not to be used */
		for (i = (max_idx_pktgain_limit + 1); i < sz; i++) {
			lna1_tbl[i] = 0x7f;
		}

		wlc_phy_table_write_acphy(pi, gainlimitid, 6, 8, 8,
			lna1_tbl);
		wlc_phy_table_write_acphy(pi, gainlimitid, 6, 8+64, 8,
			lna1_tbl);
	} else {
		wlc_phy_table_write_acphy(pi, gainlimitid, 4, 16, 8,
			rxg_params->gainlimit_tbl[LNA2_TBL_IND]);
		wlc_phy_table_write_acphy(pi, gainlimitid, 4, 16+64, 8,
			rxg_params->gainlimit_tbl[LNA2_TBL_IND]);
	}
}

void
wlc_phy_upd_lna1_bypass_acphy(phy_info_t *pi, uint8 core)
{
	uint8 idx = 6, rout, rout_gain, rout_offset, rout_tbl;
	uint16 lna1byp_val, gainbits_tblid;

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		ACMAJORREV_33(pi->pubpi->phy_rev)) {
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			rout = 9; rout_gain = -6;
			rout_offset = 0 + (core * 24) + idx;
		} else {
			rout = 0; rout_gain = 1;
			rout_offset = 8 + (core * 24) + idx;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	{
		rout = 0; rout_gain = 0; rout_offset = 0;
		ASSERT(0); /* Not sure what should go in here */
	}

	lna1byp_val = (1 << ACPHY_Core0_lna1BypVals_lna1BypEn0_SHIFT(pi->pubpi.phy_rev)) |
	        (idx << ACPHY_Core0_lna1BypVals_lna1BypIndex0_SHIFT(pi->pubpi.phy_rev)) |
	        (rout_gain << ACPHY_Core0_lna1BypVals_lna1BypGain0_SHIFT(pi->pubpi.phy_rev));

	WRITE_PHYREGC(pi, _lna1BypVals, core, lna1byp_val);
	MOD_PHYREG(pi, AfePuCtrl, lna1_pd_during_byp, 1);

	/*  Set the Rout to be used for index 6 */
	rout_tbl = (rout << 3) + idx;
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, rout_offset, 8, &rout_tbl);

	gainbits_tblid = (core == 0) ? ACPHY_TBL_ID_GAINBITS0 : (core == 1) ?
	        ACPHY_TBL_ID_GAINBITS1 : (core == 2) ? ACPHY_TBL_ID_GAINBITS2 :
	        ACPHY_TBL_ID_GAINBITS3;
	wlc_phy_table_write_acphy(pi, gainbits_tblid, 1, 6, 8, &idx);
}

void wlc_phy_rfctrl_override_rxgain_acphy(phy_info_t *pi, uint8 restore,
                                           rxgain_t rxgain[], rxgain_ovrd_t rxgain_ovrd[])
{
	uint8 core, lna1_Rout, lna2_Rout;
	uint16 reg_rxgain, reg_rxgain2, reg_lpfgain;
	uint8 stall_val;
	uint8 lna1_gm;
	uint8 offset;
	bool suspend;
	uint8 lna1byp = 0;

	if (restore == 1) {
		/* restore the stored values */
		FOREACH_CORE(pi, core) {
			WRITE_PHYREGCE(pi, RfctrlOverrideGains, core, rxgain_ovrd[core].rfctrlovrd);
			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN1, core, rxgain_ovrd[core].rxgain);
			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN2, core, rxgain_ovrd[core].rxgain2);
			WRITE_PHYREGCE(pi, RfctrlCoreLpfGain, core, rxgain_ovrd[core].lpfgain);
			PHY_INFORM(("%s, Restoring RfctrlOverride(rxgain) values\n", __FUNCTION__));
		}
	} else {
		suspend = !(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC);
		if (!suspend) {
			wlapi_suspend_mac_and_wait(pi->sh->physhim);
			phy_utils_phyreg_enter(pi);
		}
		stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
		ACPHY_DISABLE_STALL(pi);
		FOREACH_CORE(pi, core) {
			/* Save the original values */
			rxgain_ovrd[core].rfctrlovrd = READ_PHYREGCE(pi, RfctrlOverrideGains, core);
			rxgain_ovrd[core].rxgain = READ_PHYREGCE(pi, RfctrlCoreRXGAIN1, core);
			rxgain_ovrd[core].rxgain2 = READ_PHYREGCE(pi, RfctrlCoreRXGAIN2, core);
			rxgain_ovrd[core].lpfgain = READ_PHYREGCE(pi, RfctrlCoreLpfGain, core);

			offset = (TINY_RADIO(pi) || IS_28NM_RADIO(pi)) ?
				(rxgain[core].lna1 +
				ACPHY_LNAROUT_BAND_OFFSET(pi, pi->radio_chanspec)) :
				(5 + ACPHY_LNAROUT_BAND_OFFSET(pi, pi->radio_chanspec));

			wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT,
				1, offset + ACPHY_LNAROUT_CORE_RD_OFST(pi, core),
				8, &lna1_Rout);
			/* For rev40 lna2 not used but table has entries for lna2 */
			if (IS_28NM_RADIO(pi)) {
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT,
					1, 16 + ACPHY_LNAROUT_CORE_RD_OFST(pi, core),
					8, &lna2_Rout);
			} else {
				/* LnaRoutLUT WAR for 4349A2 */
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_LNAROUT,
					1, 22 + ACPHY_LNAROUT_CORE_RD_OFST(pi, core),
					8, &lna2_Rout);
			}

			/* LNA1 bypss check */
			if (IS_28NM_RADIO(pi)) {
				lna1byp = (rxgain[core].lna1 == LNA1_BYPASS_INDEX) ? 1 : 0;
				PHY_INFORM(("%s, LNA1 bypass mode override\n", __FUNCTION__));
			}

			/* Write the rxgain override registers */
			lna1_gm = (TINY_RADIO(pi) || IS_28NM_RADIO(pi))
				? (lna1_Rout & 0x7) : rxgain[core].lna1;

			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN1, core,
			              (lna1byp << 14) | (rxgain[core].dvga << 10) |
			              (rxgain[core].mix << 6) | (rxgain[core].lna2 << 3) | lna1_gm);
			WRITE_PHYREGCE(pi, RfctrlCoreRXGAIN2, core,
			              (((lna2_Rout >> 3) & 0xf) << 4 | ((lna1_Rout >> 3) & 0xf)));
			WRITE_PHYREGCE(pi, RfctrlCoreLpfGain, core,
			              (rxgain[core].lpf1 << 3) | rxgain[core].lpf0);

			MOD_PHYREGCE(pi, RfctrlOverrideGains, core, rxgain, 1);
			MOD_PHYREGCE(pi, RfctrlOverrideGains, core, lpf_bq1_gain, 1);
			MOD_PHYREGCE(pi, RfctrlOverrideGains, core, lpf_bq2_gain, 1);

			reg_rxgain = READ_PHYREGCE(pi, RfctrlCoreRXGAIN1, core);
			reg_rxgain2 = READ_PHYREGCE(pi, RfctrlCoreRXGAIN2, core);
			reg_lpfgain = READ_PHYREGCE(pi, RfctrlCoreLpfGain, core);
			PHY_INFORM(("%s, core %d. rxgain_ovrd = 0x%x, lpf_ovrd = 0x%x\n",
			            __FUNCTION__, core, reg_rxgain, reg_lpfgain));
			PHY_INFORM(("%s, core %d. rxgain_rout_ovrd = 0x%x\n",
			            __FUNCTION__, core, reg_rxgain2));
			BCM_REFERENCE(reg_rxgain);
			BCM_REFERENCE(reg_rxgain2);
			BCM_REFERENCE(reg_lpfgain);
		}
		ACPHY_ENABLE_STALL(pi, stall_val);
		if (!suspend) {
			phy_utils_phyreg_exit(pi);
			wlapi_enable_mac(pi->sh->physhim);
		}
	}
}

uint8 wlc_phy_calc_extra_init_gain_acphy(phy_info_t *pi, uint8 extra_gain_3dB,
	rxgain_t rxgain[])
{
	uint16 init_gain_code[4];
	uint8 core, MAX_DVGA, MAX_LPF, MAX_MIX;
	uint8 dvga, mix, lpf0, lpf1;
	uint8 dvga_inc, lpf0_inc, lpf1_inc;
	uint8 max_inc, gain_ticks = extra_gain_3dB;

	MAX_DVGA = 4; MAX_LPF = 10; MAX_MIX = 4;
	wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RFSEQ, 3, 0xf9, 16, &init_gain_code);

	/* Find if the requested gain increase is possible */
	FOREACH_CORE(pi, core) {
		dvga = 0;
		mix = (init_gain_code[core] >> 6) & 0xf;
		lpf0 = (init_gain_code[core] >> 10) & 0x7;
		lpf1 = (init_gain_code[core] >> 13) & 0x7;
		max_inc = MAX(0, MAX_DVGA - dvga) + MAX(0, MAX_LPF - lpf0 - lpf1) +
		        MAX(0, MAX_MIX - mix);
		gain_ticks = MIN(gain_ticks, max_inc);
	}
	if (gain_ticks != extra_gain_3dB) {
		PHY_INFORM(("%s: Unable to find enough extra gain. Using extra_gain = %d\n",
		            __FUNCTION__, 3 * gain_ticks));
	}
		/* Do nothing if no gain increase is required/possible */
	if (gain_ticks == 0) {
		return gain_ticks;
	}
	/* Find the mix, lpf0, lpf1 gains required for extra INITgain */
	FOREACH_CORE(pi, core) {
		uint8 gain_inc = gain_ticks;
		dvga = 0;
		mix = (init_gain_code[core] >> 6) & 0xf;
		lpf0 = (init_gain_code[core] >> 10) & 0x7;
		lpf1 = (init_gain_code[core] >> 13) & 0x7;
		dvga_inc = MIN((uint8) MAX(0, MAX_DVGA - dvga), gain_inc);
		dvga += dvga_inc;
		gain_inc -= dvga_inc;
		lpf1_inc = MIN((uint8) MAX(0, MAX_LPF - lpf1 - lpf0), gain_inc);
		lpf1 += lpf1_inc;
		gain_inc -= lpf1_inc;
		lpf0_inc = MIN((uint8) MAX(0, MAX_LPF - lpf1 - lpf0), gain_inc);
		lpf0 += lpf0_inc;
		gain_inc -= lpf0_inc;
		mix += MIN((uint8) MAX(0, MAX_MIX - mix), gain_inc);
		rxgain[core].lna1 = init_gain_code[core] & 0x7;
		rxgain[core].lna2 = (init_gain_code[core] >> 3) & 0x7;
		rxgain[core].mix  = mix;
		rxgain[core].lpf0 = lpf0;
		rxgain[core].lpf1 = lpf1;
		rxgain[core].dvga = dvga;
	}
	return gain_ticks;
}

/* ************************************* */
/*		Carrier Sense related definitions		*/
/* ************************************* */
static void wlc_phy_ofdm_crs_acphy(phy_info_t *pi, bool enable);
static void wlc_phy_clip_det_acphy(phy_info_t *pi, bool enable);

static void
wlc_phy_ofdm_crs_acphy(phy_info_t *pi, bool enable)
{
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));
	/* MAC should be suspended before calling this function */
	ASSERT((R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC) == 0);
	if (enable) {
		if (ACREV_GE(pi->pubpi->phy_rev, 32)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				uint8 core;

				FOREACH_CORE(pi, core) {
				MOD_PHYREGCE(pi, crsControlu, core, totEnable, 1);
				MOD_PHYREGCE(pi, crsControll, core, totEnable, 1);
				MOD_PHYREGCE(pi, crsControluSub1, core, totEnable, 1);
				MOD_PHYREGCE(pi, crsControllSub1, core, totEnable, 1);
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				MOD_PHYREG(pi, crsControlu0, totEnable, 1);
				if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControll0, totEnable, 1)
						MOD_PHYREG_ENTRY(pi, crsControllSub10, totEnable, 1)
						MOD_PHYREG_ENTRY(pi, crsControluSub10, totEnable, 1)
					ACPHY_REG_LIST_EXECUTE(pi);
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 2) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu1, totEnable, 1)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
						ACPHY_REG_LIST_START
							MOD_PHYREG_ENTRY(pi, crsControll1,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControllSub11,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControluSub11,
								totEnable, 1)
						ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 3) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu2, totEnable, 1)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
						ACPHY_REG_LIST_START
							MOD_PHYREG_ENTRY(pi, crsControll2,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControllSub12,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControluSub12,
								totEnable, 1)
						ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 4) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu3, totEnable, 1)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
						ACPHY_REG_LIST_START
							MOD_PHYREG_ENTRY(pi, crsControll3,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControllSub13,
								totEnable, 1)
							MOD_PHYREG_ENTRY(pi, crsControluSub13,
								totEnable, 1)
						ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
			}
		} else {
			ACPHY_REG_LIST_START
				MOD_PHYREG_ENTRY(pi, crsControlu, totEnable, 1)
				MOD_PHYREG_ENTRY(pi, crsControll, totEnable, 1)
				MOD_PHYREG_ENTRY(pi, crsControluSub1, totEnable, 1)
				MOD_PHYREG_ENTRY(pi, crsControllSub1, totEnable, 1)
			ACPHY_REG_LIST_EXECUTE(pi);
		}
	} else {
		if (ACREV_GE(pi->pubpi->phy_rev, 32)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				uint8 core;

				FOREACH_CORE(pi, core) {
				MOD_PHYREGCE(pi, crsControlu, core, totEnable, 0);
				MOD_PHYREGCE(pi, crsControll, core, totEnable, 0);
				MOD_PHYREGCE(pi, crsControluSub1, core, totEnable, 0);
				MOD_PHYREGCE(pi, crsControllSub1, core, totEnable, 0);
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				MOD_PHYREG(pi, crsControlu0, totEnable, 0);
				if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
				ACPHY_REG_LIST_START
					MOD_PHYREG_ENTRY(pi, crsControll0, totEnable, 0)
					MOD_PHYREG_ENTRY(pi, crsControllSub10, totEnable, 0)
					MOD_PHYREG_ENTRY(pi, crsControluSub10, totEnable, 0)
				ACPHY_REG_LIST_EXECUTE(pi);
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 2) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu1, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControll1, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControllSub11, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControluSub11, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 3) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu2, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControll2, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControllSub12, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControluSub12, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
				if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 4) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControlu3, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					ACPHY_REG_LIST_START
						MOD_PHYREG_ENTRY(pi, crsControll3, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControllSub13, totEnable, 0)
						MOD_PHYREG_ENTRY(pi, crsControluSub13, totEnable, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
					}
				}
			}
		} else {
			ACPHY_REG_LIST_START
				MOD_PHYREG_ENTRY(pi, crsControlu, totEnable, 0)
				MOD_PHYREG_ENTRY(pi, crsControll, totEnable, 0)
				MOD_PHYREG_ENTRY(pi, crsControluSub1, totEnable, 0)
				MOD_PHYREG_ENTRY(pi, crsControllSub1, totEnable, 0)
			ACPHY_REG_LIST_EXECUTE(pi);
		}
	}
}

static void
wlc_phy_clip_det_acphy(phy_info_t *pi, bool enable)
{
	uint8 core;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;

	/* Make clip detection difficult (impossible?) */
	/* don't change this loop to active core loop, gives 100% per, why? */
	FOREACH_CORE(pi, core) {
		if (ACREV_IS(pi->pubpi->phy_rev, 0)) {
			if (enable) {
				WRITE_PHYREGC(pi, Clip1Threshold, core,
					pi_ac->rxgcrsi->clip1_th);
			} else {
				WRITE_PHYREGC(pi, Clip1Threshold, core, 0xffff);
			}
		} else {
			if (enable) {
				phy_utils_and_phyreg(pi, ACPHYREGC(pi, computeGainInfo, core),
					(uint16)~ACPHY_REG_FIELD_MASK(pi, computeGainInfo, core,
					disableClip1detect));
			} else {
				phy_utils_or_phyreg(pi, ACPHYREGC(pi, computeGainInfo, core),
					ACPHY_REG_FIELD_MASK(pi, computeGainInfo, core,
					disableClip1detect));
			}
		}
	}

}

void wlc_phy_force_crsmin_acphy(phy_info_t *pi, void *p)
{
	int8 *set_crs_thr = p;

	/* Prepare Mac and Phregs */
	wlapi_suspend_mac_and_wait(pi->sh->physhim);
	phy_utils_phyreg_enter(pi);

	if (set_crs_thr[0] == -1) {
		bool save_phynoise_disable;
		/* Auto crsmin power mode */
		PHY_CAL(("Setting auto crsmin power mode\n"));
		/* save and enable the noisecal schedule state */
		save_phynoise_disable = phy_noise_sched_get((wlc_phy_t*)pi);
		phy_noise_sched_set((wlc_phy_t *)pi, PHY_FORCEMEASURE_MODE, FALSE);
		phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
		/* restore the noisecal schedule state */
		phy_noise_sched_set((wlc_phy_t *)pi, PHY_FORCEMEASURE_MODE,
			save_phynoise_disable);
		pi->u.pi_acphy->rxgcrsi->crsmincal_enable = TRUE;
	} else if (set_crs_thr[0] == 0) {
		/* Default crsmin value */
		PHY_CAL(("Setting default crsmin: %d\n", ACPHY_CRSMIN_DEFAULT));
		wlc_phy_set_crs_min_pwr_acphy(pi, ACPHY_CRSMIN_DEFAULT, 0, 0, 0, 0);
		pi->u.pi_acphy->rxgcrsi->crsmincal_enable = FALSE;
	} else {
		/* Set the crsmin power value to be 'set_crs_thr' */
		PHY_CAL(("Setting crsmin: %d %d %d %d\n",
			set_crs_thr[0], set_crs_thr[1], set_crs_thr[2], set_crs_thr[3]));
		wlc_phy_set_crs_min_pwr_acphy(pi, set_crs_thr[0], 0, set_crs_thr[1],
			set_crs_thr[2], set_crs_thr[3]);
		pi->u.pi_acphy->rxgcrsi->crsmincal_enable = FALSE;
	}

	/* Enable mac */
	phy_utils_phyreg_exit(pi);
	wlapi_enable_mac(pi->sh->physhim);
}

void phy_ac_rxgcrs_force_lesiscale(phy_ac_rxgcrs_info_t *rxgcrsi, void *p)
{
	phy_info_t *pi = rxgcrsi->pi;
	int8  *set_lesi_scale = p;
	/* Local copy of phyrxchains & EnTx bits before overwrite */
	int8 zerodBs[] = {64, 64, 64, 64};

	/* Prepare Mac and Phregs */
	wlapi_suspend_mac_and_wait(pi->sh->physhim);
	phy_utils_phyreg_enter(pi);

	if (set_lesi_scale[0] == -1) {
		/* Auto crsmin power mode */
		PHY_CAL(("Setting auto crsmin power mode\n"));
		phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
		rxgcrsi->lesiscalecal_enable = TRUE;
	} else if (set_lesi_scale[0] == 0) {
		/* Default crsmin value */
		PHY_CAL(("Setting default crsmin: %d\n", ACPHY_LESISCALE_DEFAULT));
		phy_ac_rxgcrs_set_lesiscale(rxgcrsi, zerodBs);
		rxgcrsi->lesiscalecal_enable = FALSE;
	} else {
		/* Set the crsmin power value to be 'set_crs_thr' */
		printf("Setting LESI scale: %d %d %d %d\n",
			set_lesi_scale[0], set_lesi_scale[1], set_lesi_scale[2], set_lesi_scale[3]);
		phy_ac_rxgcrs_set_lesiscale(rxgcrsi, set_lesi_scale);
		rxgcrsi->lesiscalecal_enable = FALSE;
	}

	/* Enable mac */
	phy_utils_phyreg_exit(pi);
	wlapi_enable_mac(pi->sh->physhim);
}

void
wlc_phy_crs_min_pwr_cal_acphy(phy_info_t *pi, uint8 crsmin_cal_mode)
{
	int8   cmplx_pwr_dbm[PHY_CORE_MAX], cnt, tmp_diff, cache_up_th = 2;
	int8   thresh_20[] = {39, 42, 45, 48, 51, 53, 54, 57, 60, 63,
		66, 68, 70, 72, 75, 78, 80, 83, 86, 90, 92, 94}; /* idx 0 --> -36 dBm */
	int8   thresh_40[] = {41, 44, 46, 48, 50, 52, 54, 56, 58, 60,
		63, 66, 69, 71, 74, 76, 79, 82, 86, 89, 92, 94}; /* idx 0 --> -34 dBm */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	int8   thresh_20_ulp[] = {40, 43, 46, 49, 52, 55, 56, 59,
			62, 65, 67, 69, 72, 75, 77, 79, 82, 84, 87,
			90, 92, 94, 97, 99, 101, 104, 106, 109, 112,
			114, 116, 119, 121, 123, 125, 127, 129}; /* idx 0 --> -37 dBm */
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

	int8   thresh_80[] = {41, 44, 46, 48, 50, 52, 55, 57, 60, 63,
		65, 68, 70, 72, 74, 77, 80, 84, 87, 90, 92, 94}; /* idx 0 --> -30 dBm */
#ifdef WL11ULB
	int8   thresh_ULB[] = {33, 36, 39, 42, 45, 48, 51, 53, 54, 57,
			60, 63, 66, 68, 70, 72, 75, 78, 80, 83, 86, 90}; /* idx 0 --> -38 dBm */
#endif /* WL11ULB */
	int8   scale_lesi[] = {64, 57, 50, 45, 40, 35, 32, 28, 25, 22,
		20, 18, 16, 14, 12, 11, 10,  9,  8,  7,  6,  5,  5};

	uint8  thresh_sz = 15;	/* i.e. not the full size of the array */
	uint8  thresh_sz_lesi = 18;	/* i.e. not the full size of the array */
	uint8  i;
	uint8  fp[PHY_CORE_NUM_4];
	int8  lesi_scale[PHY_CORE_MAX] = {0};
	uint8  chan_freq_range, run_cal = 0, abs_diff_cache_curr = 0, core_freq_segment_map;
	int16  acum_noise_pwr;

	uint8 dvga;
	uint8  min_of_all_cores = 54, max_fp = 255;
	struct Tidxlist {
		int8 idx;
		int8 core;
	} idxlists[PHY_CORE_MAX];

	struct Tidxlist_lesi {
		int8 idx;
		int8 core;
	} idxlists_lesi[PHY_CORE_MAX];

	uint core_count = 0;

	bool suspend;
	int8 weakest_rssi;
	int8 offset_1 = 0, offset_2 = 0, offset_3 = 0;
	uint8 fp_min_of_all_cores = 255;
	phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;

	weakest_rssi = phy_ac_noise_get_weakest_rssi(rxgcrsi->aci->noisei);

	bzero((uint8 *)cmplx_pwr_dbm, sizeof(cmplx_pwr_dbm));
	bzero((uint8 *)fp, sizeof(fp));
	bzero(idxlists_lesi, sizeof(idxlists_lesi));

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		/* Get weakest rssi, and find maximum crsmin allowed based on rssi */
		if (weakest_rssi != 0) {
			uint8 max_desense;
			max_desense = MAX(0, weakest_rssi + 87);
			// 8 ticks is 3dBs. 8/3 = 43/16
			max_fp = MIN(max_fp, ACPHY_CRSMIN_DEFAULT + ((max_desense*43)>>4));
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	suspend = !(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC);

	/* Increase crsmin by 1.5dB for 4360/43602. Helps with Zero-pkt-loss (less fall triggers) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0))
	if (ACMAJORREV_0(pi->pubpi->phy_rev) || ACMAJORREV_5(pi->pubpi->phy_rev)) {
		for (i = 0; i < thresh_sz; i++) {
			thresh_20[i] += 4;
			thresh_40[i] += 4;
			thresh_80[i] += 4;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
	if (ACMAJORREV_3(pi->pubpi->phy_rev) && rxgcrsi != NULL &&
		rxgcrsi->thresh_noise_cal) {
		thresh_sz = 20;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(pi->pubpi->phy_rev) && rxgcrsi != NULL &&
		rxgcrsi->thresh_noise_cal) {
		thresh_sz = sizeof(thresh_20); /* use the full size of the table */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
		/* Increase by 9 for 2G */
			for (i = 3; i < thresh_sz; i++) {
			/* Increase crsmin by 3 dB for 4349 when
			* inband blocker >= -30dBm
			*/
				thresh_20[i] += 9;
			}
		} else {
			/* Increase by 6 for 5G */
			for (i = 12; i < thresh_sz; i++) {
			/* Increase crsmin by 2dB for 4349 when inband blocker >= -24dBm */
				thresh_20[i] += 6;
				thresh_40[i] += 6;
				thresh_80[i] += 6;
			}
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
	if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
		thresh_sz = sizeof(thresh_20);
	}


	/* Initialize */
	FOREACH_CORE(pi, i) {
		fp[i] = 0x36;
	}

	/* since we are touching phy regs mac has to be suspended */
	if (!suspend) {
		wlapi_suspend_mac_and_wait(pi->sh->physhim);
		phy_utils_phyreg_enter(pi);
	}

	/* upate the noise pwr array with most recent noise pwr */
	if (crsmin_cal_mode == PHY_CRS_RUN_AUTO) {
		FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, i)  {
			rxgcrsi->phy_noise_pwr_array[rxgcrsi->phy_noise_counter][i] =
			phy_ac_noise_get_data(pi->u.pi_acphy->noisei)->phy_noise_all_core[i]+1;
		}
		rxgcrsi->phy_noise_counter++;
		rxgcrsi->phy_noise_counter = rxgcrsi->phy_noise_counter%(PHY_SIZE_NOISE_ARRAY);
	}

	FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, i)  {

		/* core_freq_segment_map is only required for 80P80 mode.
		For other modes, it is ignored
		*/
		core_freq_segment_map =
			phy_ac_chanmgr_get_data(pi->u.pi_acphy->chanmgri)->core_freq_mapping[i];

		/* check the freq range of the current channel */
		/* 2G, 5GL, 5GM, 5GH, 5GX */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_33))
		if (ACMAJORREV_33(pi->pubpi->phy_rev) && PHY_AS_80P80(pi, pi->radio_chanspec)) {
			uint8 chans[NUM_CHANS_IN_CHAN_BONDING] = {0, 0};

			phy_ac_chanmgr_get_chan_freq_range_80p80(pi, pi->radio_chanspec, chans);
			chan_freq_range = chans[0];
			ASSERT(chan_freq_range < PHY_SIZE_NOISE_CACHE_ARRAY);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_33)) */
		{
			chan_freq_range = phy_ac_chanmgr_get_chan_freq_range(pi, 0,
				core_freq_segment_map);
		}
		if (crsmin_cal_mode == PHY_CRS_RUN_AUTO) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				int8   noises_core[PHY_SIZE_NOISE_ARRAY];
				/* noises_core would be change in get_avg_noisepwr, so be careful */
				for (cnt = 0; cnt < PHY_SIZE_NOISE_ARRAY; cnt++) {
					noises_core[cnt] =
						rxgcrsi->phy_noise_pwr_array[cnt][i];
				}

				/* Get avg noise value discarding few max values */
				cmplx_pwr_dbm[i] =
				        phy_ac_rxgcrs_get_avg_noisepwr(noises_core);

				if (cmplx_pwr_dbm[i] == 0) {
					goto exit;
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				/* Enter here only from ucode noise interrupt */
				acum_noise_pwr = 0;
				/* average of the most recent noise pwrs */
				for (cnt = 0; cnt < PHY_SIZE_NOISE_ARRAY; cnt++) {
					if (rxgcrsi->phy_noise_pwr_array[cnt][i] != 0)
					{
						acum_noise_pwr +=
							rxgcrsi->phy_noise_pwr_array[cnt][i];
					} else {
						break;
					}
				}

				if (cnt != 0) {
					cmplx_pwr_dbm[i] = acum_noise_pwr/cnt;
				} else {
					goto exit;
				}
			}

			/* cache_update threshold: 2 4335 and 4350, and 3 for 4360 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0))
			if (ACMAJORREV_0(pi->pubpi->phy_rev))
				cache_up_th = 3;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0)) */

			/* update noisecal_cache with valid noise power */
			/* check if the new noise pwr reading is same as cache */
			tmp_diff = 0;
			if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
				tmp_diff = rxgcrsi->phy_noise_cache_crsmin
				        [chan_freq_range][i] - cmplx_pwr_dbm[i];
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				tmp_diff = (rxgcrsi->phy_noise_cache_crsmin
				            [chan_freq_range][i] + 3) - cmplx_pwr_dbm[i];
			} else {
				tmp_diff = (rxgcrsi->phy_noise_cache_crsmin
				            [chan_freq_range][i] + 7) - cmplx_pwr_dbm[i];
			}
			abs_diff_cache_curr = tmp_diff < 0 ? (0 - tmp_diff) : tmp_diff;

			/* run crscal with the current noise pwr if the call comes from phy_cals */
			if (abs_diff_cache_curr >= cache_up_th || rxgcrsi->force_crsmincal) {
				run_cal++;
				if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
					rxgcrsi->phy_noise_cache_crsmin
					[chan_freq_range][i] = cmplx_pwr_dbm[i];
				} else if (CHSPEC_IS40(pi->radio_chanspec)) {
					rxgcrsi->phy_noise_cache_crsmin
					[chan_freq_range][i] = cmplx_pwr_dbm[i] - 3;
				} else {
					rxgcrsi->phy_noise_cache_crsmin
					[chan_freq_range][i] = cmplx_pwr_dbm[i] - 7;
				}
			}
		} else {
			/* Enter here only from chan_change */

			/* During chan_change, read back the noise pwr from cache */
			if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
				cmplx_pwr_dbm[i] = rxgcrsi->phy_noise_cache_crsmin
				        [chan_freq_range][i];
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				cmplx_pwr_dbm[i] = rxgcrsi->phy_noise_cache_crsmin
				        [chan_freq_range][i] + 3;
			} else {
				cmplx_pwr_dbm[i] = rxgcrsi->phy_noise_cache_crsmin
				        [chan_freq_range][i] + 7;
			}
		}

		dvga = READ_PHYREGFLDC(pi, InitGainCodeB, i, initvgagainIndex);
		/* get the index number for crs_th table */
		if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				idxlists[core_count].idx = cmplx_pwr_dbm[i] + 37;
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			{
				idxlists[core_count].idx = cmplx_pwr_dbm[i] + 36;
				if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				    ACMAJORREV_33(pi->pubpi->phy_rev) ||
				    ACMAJORREV_37(pi->pubpi->phy_rev) ||
				    ACMAJORREV_40(pi->pubpi->phy_rev)) {
					idxlists_lesi[core_count].idx =
						cmplx_pwr_dbm[i] - 3*dvga + 40;
				}
			}
		} else if (CHSPEC_IS40(pi->radio_chanspec)) {
			idxlists[core_count].idx = cmplx_pwr_dbm[i] + 34;
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			    ACMAJORREV_33(pi->pubpi->phy_rev) ||
			    ACMAJORREV_37(pi->pubpi->phy_rev) ||
			    ACMAJORREV_40(pi->pubpi->phy_rev))
				idxlists_lesi[core_count].idx = cmplx_pwr_dbm[i] - 3*dvga + 37;
		} else {
			idxlists[core_count].idx = cmplx_pwr_dbm[i] + 30;
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			    ACMAJORREV_33(pi->pubpi->phy_rev) ||
			    ACMAJORREV_37(pi->pubpi->phy_rev) ||
			    ACMAJORREV_40(pi->pubpi->phy_rev))
				idxlists_lesi[core_count].idx = cmplx_pwr_dbm[i] - 3*dvga + 34;
		}

#ifdef WL11ULB
		if (CHSPEC_IS10(pi->radio_chanspec) || CHSPEC_IS5(pi->radio_chanspec) ||
			CHSPEC_IS2P5(pi->radio_chanspec)) {
			idxlists[core_count].idx = cmplx_pwr_dbm[i] + 38;
		}
#endif /* WL11ULB */

		PHY_CAL(("%s: cmplx_pwr (%d) =======  %d\n", __FUNCTION__, i, cmplx_pwr_dbm[i]));
		/* out of bound */
		if ((idxlists[core_count].idx < 0) ||
		    (idxlists[core_count].idx > (thresh_sz - 1))) {
			idxlists[core_count].idx = idxlists[core_count].idx < 0 ?
			        0 : (thresh_sz - 1);
		}

		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		    ACMAJORREV_33(pi->pubpi->phy_rev) ||
		    ACMAJORREV_37(pi->pubpi->phy_rev) ||
		    ACMAJORREV_40(pi->pubpi->phy_rev)) {
			if ((idxlists_lesi[core_count].idx < 0) ||
			    (idxlists[core_count].idx > (thresh_sz_lesi - 1))) {
				idxlists_lesi[core_count].idx = idxlists_lesi[core_count].idx < 0 ?
				        0 : (thresh_sz_lesi - 1);
			}
		}

		idxlists[core_count].core = i;
		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		    ACMAJORREV_33(pi->pubpi->phy_rev) ||
		    ACMAJORREV_37(pi->pubpi->phy_rev) ||
		    ACMAJORREV_40(pi->pubpi->phy_rev))
			idxlists_lesi[core_count].core = i;

		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		    ACMAJORREV_33(pi->pubpi->phy_rev) ||
		    ACMAJORREV_37(pi->pubpi->phy_rev) ||
		    ACMAJORREV_40(pi->pubpi->phy_rev)) {
			if (ISSIM_ENAB(pi->sh->sih)) {
				fp[i] = fp[0];
			} else {
				if (CHSPEC_IS20(pi->radio_chanspec)) {
					fp[i] = thresh_20[idxlists[core_count].idx];
				} else if (CHSPEC_IS40(pi->radio_chanspec)) {
					fp[i] = thresh_40[idxlists[core_count].idx];
				} else if (CHSPEC_IS80(pi->radio_chanspec) ||
					PHY_AS_80P80(pi, pi->radio_chanspec)) {
					fp[i] = thresh_80[idxlists[core_count].idx];
				} else if (CHSPEC_IS160(pi->radio_chanspec)) {
					ASSERT(0);
				}
			}
#ifdef WL11ULB
			if (CHSPEC_IS10(pi->radio_chanspec) || CHSPEC_IS5(pi->radio_chanspec) ||
				CHSPEC_IS2P5(pi->radio_chanspec)) {
				fp[i] = thresh_ULB[idxlists[core_count].idx];
			}
#endif /* WL11ULB */
			/* Limit fp based on weakest rssi */
			fp[i] = MIN(max_fp, fp[i]);

			if (i == 0)
				min_of_all_cores = fp[0];
			else
				min_of_all_cores = MIN(min_of_all_cores, fp[i]);

			lesi_scale[i] = scale_lesi[idxlists_lesi[core_count].idx];
		} else {
			if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
				if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
					fp[idxlists[core_count].core] =
						thresh_20_ulp[idxlists[core_count].idx];
				} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
				{
					fp[idxlists[core_count].core] = thresh_20
						[idxlists[core_count].idx];
				}
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				fp[idxlists[core_count].core] = thresh_40[idxlists[core_count].idx];
			} else if (CHSPEC_IS80(pi->radio_chanspec)) {
				fp[idxlists[core_count].core] = thresh_80[idxlists[core_count].idx];
			}

			if (idxlists[core_count].core == 1) {
				offset_1 = fp[1] - fp[0];
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
				if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
					int8  nvar_adj, freqweight, noise_delta;

				/* If the offset is > 6dB assume degenerate RX senario */
					if (ABS(offset_1) > 16) {
						fp[0] = MAX(fp[0], fp[1]);
						offset_1 = 0;
					 }
					if (stf_shdata->phyrxchain != 3) {
						if (ABS(offset_1) > 16) {
							fp[0] = MAX(fp[0], fp[1]);
							offset_1 = 0;
						}
					}
					if ((rxgcrsi->srom_asymmetricjammermod)) {
						noise_delta = cmplx_pwr_dbm[1] - cmplx_pwr_dbm[0];
						freqweight = 0;
						if (ABS(noise_delta) > 3) {
							freqweight = MAX(MIN(32 +
								noise_delta*4/3, 0x3d), 0x3);
							nvar_adj = MAX(MIN((16*ABS(noise_delta)/5) +
									0xe, 0x80), 0xe);
							MOD_PHYREG(pi, FreqGainBypass, bypass, 1);
							MOD_PHYREG(pi, FreqGainBypass, bypassValue,
									freqweight);
							MOD_PHYREG(pi, FSTRMetricTh,
								lowPwr_min_metric_th, 0x1c);
							if (noise_delta > 0) {
								MOD_PHYREG(pi, nvcfg0,
									noisevar_nf_radio_qdb_core1,
									nvar_adj);
							} else {
								MOD_PHYREG(pi, nvcfg0,
									noisevar_nf_radio_qdb_core0,
									nvar_adj);
							}
						} else {
							MOD_PHYREG(pi, FreqGainBypass, bypass, 0);
							MOD_PHYREG(pi, FSTRMetricTh,
								lowPwr_min_metric_th, 0x20);
							MOD_PHYREG(pi, nvcfg0,
								noisevar_nf_radio_qdb_core1, 0xE);
							MOD_PHYREG(pi, nvcfg0,
								noisevar_nf_radio_qdb_core0, 0xE);
						}
					}
				}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
			} else if (idxlists[core_count].core == 2) {
				offset_2 = fp[2] - fp[0];
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
				if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
				/* If the offset is > 6dB assume degenerate RX senario */
					if (ABS(offset_2) > 16) {
						fp[0] = MAX(fp[0], fp[2]);
						offset_1 = 0;
						offset_2 = 0;
					}
				}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
			}
		}

		++core_count;
	}

	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev) ||
	    ACMAJORREV_37(pi->pubpi->phy_rev)) {
		offset_1 = fp[1] - min_of_all_cores;
		offset_2 = fp[2] - min_of_all_cores;
		offset_3 = fp[3] - min_of_all_cores;
		fp[0] = min_of_all_cores;
	}
	FOREACH_CORE(pi, i) {
			fp_min_of_all_cores = MIN(fp_min_of_all_cores, fp[i]);
	}
#ifdef WL_NAP
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	/* Call napping ED threshold cal */
	if (PHY_NAP_ENAB(pi->sh->physhim)) {
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			phy_ac_nap_ed_thresh_cal(pi, cmplx_pwr_dbm);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
#endif /* WL_NAP */

	/* check if current noise pwr is different from the one in cache */
	if ((run_cal == 0) && (crsmin_cal_mode == PHY_CRS_RUN_AUTO)) {
		goto exit;
	}

	/* Below flag is set from phy_cals only */
	if (crsmin_cal_mode == PHY_CRS_RUN_AUTO)
		rxgcrsi->force_crsmincal = FALSE;


	rxgcrsi->crsmincal_run = 1;


	/* if noise desense is on, then the below variable will be used for comparison */
	rxgcrsi->phy_crs_th_from_crs_cal = MAX(MAX(fp[0], fp[1]), fp[2]);

	if (((!ACPHY_ENABLE_FCBS_HWACI(pi) || ACPHY_HWACI_WITH_DESENSE_ENG(pi)) &&
		pi->phywatchdog_override) || ((pi->phywatchdog_override) &&
		(IS_4364_1x1(pi) || IS_4364_3x3(pi)))) {
		if (!ACHWACIREV(pi)) {
			/* if desense is forced, then reset the variable below to default */
			if (rxgcrsi->total_desense->forced) {
				rxgcrsi->phy_crs_th_from_crs_cal = ACPHY_CRSMIN_DEFAULT;
				rxgcrsi->crsmincal_run = 2;
				goto exit;
			}
		}

		/* Don't update the crsmin registers if any desense(aci/bt) is on */
		if (rxgcrsi->total_desense->on) {
			rxgcrsi->crsmincal_run = 2;
			goto exit;
		}
	}

	/* Debug: keep count of all calls to crsmin_cal  */
	/* Debug: store the channel info  */
	/* Debug: store the noise pwr used for updating crs thresholds */
	rxgcrsi->phy_debug_crscal_counter++;
	rxgcrsi->phy_debug_crscal_channel = CHSPEC_CHANNEL(pi->radio_chanspec);
	FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, i)  {
		/* Debug info: for dumping the noise pwr used in crsmin_cal */
		rxgcrsi->phy_noise_in_crs_min[i] = cmplx_pwr_dbm[i];
	}

	/* call for updating the crsmin thresholds */
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev) ||
	    ACMAJORREV_37(pi->pubpi->phy_rev)) {
		if (rxgcrsi->crsmincal_enable)
			wlc_phy_set_crs_min_pwr_acphy(pi, fp[0], 0, offset_1, offset_2,
				offset_3);
		if (rxgcrsi->lesiscalecal_enable)
			phy_ac_rxgcrs_set_lesiscale(rxgcrsi, lesi_scale);
	} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
		int8 offset_0 = 0;
		offset_0 = fp[0] - fp_min_of_all_cores;
		offset_1 = fp[1] - fp_min_of_all_cores;
		wlc_phy_set_crs_min_pwr_acphy(pi, fp_min_of_all_cores, offset_0, offset_1,
			offset_2, offset_3);
		if (rxgcrsi->lesiscalecal_enable)
			phy_ac_rxgcrs_set_lesiscale(rxgcrsi, lesi_scale);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
	{
		wlc_phy_set_crs_min_pwr_acphy(pi, fp[0], 0, offset_1, offset_2,
			offset_3);
	}

exit:
	/* resume mac */
	if (!suspend) {
		phy_utils_phyreg_exit(pi);
		wlapi_enable_mac(pi->sh->physhim);
	}

#ifdef WL_PSMX
	/* trigger MU resound due to the resetcca triggered by updating rxchains */
	if (D11REV_IS(pi->sh->corerev, 64)) {
		OR_REG(pi->sh->osh, &pi->regs->maccommand_x, MCMDX_SND);
	}
#endif /* WL_PSMX */
}

int8
phy_ac_rxgcrs_get_avg_noisepwr(int8 noises[])
{
	int16 accum = 0;
	int8 min_val, avg_val = 0;
	uint8 i, j, loop, min_idx, cnt = 0;

	/* Take only lowest half noise vals, as higher values could be over pkt */
	loop = PHY_SIZE_NOISE_ARRAY >> 1;
	for (j = 0; j < loop; j++) {
		min_val = 100;  min_idx = PHY_SIZE_NOISE_ARRAY;
		for (i = 0; i < PHY_SIZE_NOISE_ARRAY; i++) {
			if ((noises[i] != 0) && (noises[i] < min_val)) {
				min_val = noises[i];
				min_idx = i;
			}
		}

		if (min_idx < PHY_SIZE_NOISE_ARRAY) {
			accum += noises[min_idx];
			noises[min_idx] = 0;
			cnt++;
		} else {
			break;    // no more valid values
		}
	}

	/* Average it */
	if (cnt > 0) avg_val = accum / cnt;

	return avg_val;
}

void
wlc_phy_set_crs_min_pwr_acphy(phy_info_t *pi, uint8 ac_th, int8 offset_0, int8 offset_1,
int8 offset_2, int8 offset_3)
{
	uint8 core;
	uint8 mfcrs_1bit = 0; /* match filter carrier sense 1 bit mode */
	uint8 mf_th = ac_th;
	int8  mf_off1 = offset_1;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;

	if (!ACMAJORREV_32(pi->pubpi->phy_rev) && !ACMAJORREV_33(pi->pubpi->phy_rev))
		PHY_TRACE(("%s: AC %d\n", __FUNCTION__, ac_th));

	/* 1-bit MF: 4335A0, 4335B0, 4350 */
	/* 6-bit MF: 4335C0 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(pi->pubpi->phy_rev) || ACMAJORREV_2(pi->pubpi->phy_rev)) {
		mfcrs_1bit = 1;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	{
		mfcrs_1bit = 0;
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	/* did not see any positive pwr consumption impact of 1 bit mode, so increase sensitivity */
	if (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi)) {
		mfcrs_1bit = 0;
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
	if (ACMAJORREV_2(pi->pubpi->phy_rev) && !ACMINORREV_0(pi)) {
		mfcrs_1bit = 0; /* >= v2.1 */
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */

	if (ac_th == 0) {
		if (mfcrs_1bit == 1) {
			if (CHSPEC_IS20(pi->radio_chanspec)) {
				mf_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 60 :
					pi_ac->paramsi->mfcrs_th_bw20;
				ac_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 58 :
					pi_ac->paramsi->accrs_th_bw20;
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				mf_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 60 :
					pi_ac->paramsi->mfcrs_th_bw40;
				ac_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 58 :
					pi_ac->paramsi->accrs_th_bw40;
			} else if (CHSPEC_IS80(pi->radio_chanspec) ||
				PHY_AS_80P80(pi, pi->radio_chanspec)) {
				mf_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 67 :
					pi_ac->paramsi->mfcrs_th_bw80;
				ac_th = (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) ? 60 :
					pi_ac->paramsi->accrs_th_bw80;
			} else if (CHSPEC_IS160(pi->radio_chanspec)) {
				ASSERT(0);
			}
		} else {
			mf_th = ACPHY_CRSMIN_DEFAULT;
			ac_th = ACPHY_CRSMIN_DEFAULT;
		}
		pi->u.pi_acphy->rxgcrsi->phy_crs_th_from_crs_cal = ac_th;
	} else {
		if (mfcrs_1bit == 1) {
			if (CHSPEC_IS20(pi->radio_chanspec)) {
				mf_th = ((ac_th*101)/100) + 2;
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				mf_th = ((ac_th*109)/100) - 2;
			} else if (CHSPEC_IS80(pi->radio_chanspec) ||
				PHY_AS_80P80(pi, pi->radio_chanspec)) {
				mf_th = ((ac_th*105)/100) + 2;
			} else if (CHSPEC_IS160(pi->radio_chanspec)) {
				ASSERT(0);
			}
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
	/* Adjust offset values for 1-bit MF */
	/* Not needed for 4335 and 4360, will be needed for 4350, disabled for now */

	if (ACMAJORREV_2(pi->pubpi->phy_rev)) {
		uint8 phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
		BCM_REFERENCE(phyrxchain);
		FOREACH_ACTV_CORE(pi, phyrxchain, core) {
			if (core == 1) {
				if (CHSPEC_IS5G(pi->radio_chanspec)) {
					if (CHSPEC_IS20(pi->radio_chanspec))
					{
						mf_off1 = ((((ac_th+offset_1)*101)/100) + 2)-mf_th;
					} else if (CHSPEC_IS40(pi->radio_chanspec)) {
						mf_off1 = ((((ac_th+offset_1)*109)/100) - 2)-mf_th;
					} else if (CHSPEC_IS80(pi->radio_chanspec)) {
						mf_off1 = ((((ac_th+offset_1)*105)/100) + 2)-mf_th;
					}
				}
			}
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */

#ifdef WL11ULB
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		if (CHSPEC_IS10(pi->radio_chanspec)) {
			ac_th -= 0;
			mf_th -= 0;
		} else if (CHSPEC_IS5(pi->radio_chanspec)) {
			ac_th -= 8;
			mf_th -= 8;
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
#endif /* WL11ULB */

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))
		PHY_CAL(("%s: (AC_th, MF_th) = (%d, %d)\n", __FUNCTION__, ac_th, mf_th));
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

	FOREACH_CORE(pi, core) {

		switch (core) {
			case 0:
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
				if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
				/* Clip_high gain crsmin threshold should not be as high
				* as init gain crsmin threshold. Lowering clip_high gain
				* crsmin threshold helps removing humps around -60dBm
				* Changed desense value from 24 tp 10.
				*/
#ifndef WLC_DISABLE_ACI
					wlc_phy_set_crs_min_pwr_higain_acphy(pi, ac_th-10);
#endif
					/* Desensing AC-CRS by 16 ticks helps lowering error floors
					* for 5G 40MHz/80MHz channels
					*/
					if (CHSPEC_IS5G(pi->radio_chanspec) &&
						!CHSPEC_BW_LE20(pi->radio_chanspec)) {
						ac_th += 16;
					}
				}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

				MOD_PHYREG(pi, crsminpoweru0, crsminpower0, ac_th);
				MOD_PHYREG(pi, crsmfminpoweru0, crsmfminpower0, mf_th);
				MOD_PHYREG(pi, crsminpoweroffset0, crsminpowerOffsetu, offset_0);
				MOD_PHYREG(pi, crsmfminpoweroffset0, crsmfminpowerOffsetu,
						offset_0);

				if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					MOD_PHYREG(pi, crsminpowerl0, crsminpower0, ac_th);
					MOD_PHYREG(pi, crsmfminpowerl0, crsmfminpower0, mf_th);
					MOD_PHYREG(pi, crsminpoweruSub10, crsminpower0, ac_th);
					MOD_PHYREG(pi, crsmfminpoweruSub10, crsmfminpower0,  mf_th);
					MOD_PHYREG(pi, crsminpowerlSub10, crsminpower0, ac_th);
					MOD_PHYREG(pi, crsmfminpowerlSub10, crsmfminpower0,  mf_th);

					MOD_PHYREG(pi, crsminpoweroffset0,
						crsminpowerOffsetl, offset_0);
					MOD_PHYREG(pi, crsmfminpoweroffset0,
						crsmfminpowerOffsetl, offset_0);
					MOD_PHYREG(pi, crsminpoweroffsetSub10,
						crsminpowerOffsetlSub1, offset_0);
					MOD_PHYREG(pi, crsminpoweroffsetSub10,
						crsminpowerOffsetuSub1, offset_0);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub10,
							crsmfminpowerOffsetlSub1, offset_0);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub10,
							crsmfminpowerOffsetuSub1, offset_0);
				}
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
				if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev)) {
					/* Force the offsets for core-0 */
					/* Core 0 */
					MOD_PHYREG(pi, crsminpoweroffset0,
					           crsminpowerOffsetu, 0);
					MOD_PHYREG(pi, crsminpoweroffset0,
					           crsminpowerOffsetl, 0);
					MOD_PHYREG(pi, crsmfminpoweroffset0,
					           crsmfminpowerOffsetu, 0);
					MOD_PHYREG(pi, crsmfminpoweroffset0,
					           crsmfminpowerOffsetl, 0);
					MOD_PHYREG(pi, crsminpoweroffsetSub10,
					           crsminpowerOffsetlSub1, 0);
					MOD_PHYREG(pi, crsminpoweroffsetSub10,
					           crsminpowerOffsetuSub1, 0);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub10,
					           crsmfminpowerOffsetlSub1, 0);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub10,
					           crsmfminpowerOffsetuSub1, 0);
				}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
				break;
			case 1:
				/* Force the offsets for core-1 */
				/* Core 1 */
				MOD_PHYREG(pi, crsminpoweroffset1,
				    crsminpowerOffsetu, offset_1);
				MOD_PHYREG(pi, crsmfminpoweroffset1,
				    crsmfminpowerOffsetu, mf_off1);
				if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					MOD_PHYREG(pi, crsminpoweroffset1,
						crsminpowerOffsetl, offset_1);
					MOD_PHYREG(pi, crsmfminpoweroffset1,
						crsmfminpowerOffsetl, mf_off1);
					MOD_PHYREG(pi, crsminpoweroffsetSub11,
						crsminpowerOffsetlSub1, offset_1);
					MOD_PHYREG(pi, crsminpoweroffsetSub11,
						crsminpowerOffsetuSub1, offset_1);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub11,
						crsmfminpowerOffsetlSub1, mf_off1);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub11,
						crsmfminpowerOffsetuSub1, mf_off1);
				}
				break;
			case 2:
				/* Force the offsets for core-2 */
				/* Core 2 */
				MOD_PHYREG(pi, crsminpoweroffset2,
					crsminpowerOffsetu, offset_2);
				MOD_PHYREG(pi, crsmfminpoweroffset2,
					crsmfminpowerOffsetu, offset_2);
				if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
					MOD_PHYREG(pi, crsminpoweroffset2,
						crsminpowerOffsetl, offset_2);
					MOD_PHYREG(pi, crsmfminpoweroffset2,
						crsmfminpowerOffsetl, offset_2);
					MOD_PHYREG(pi, crsminpoweroffsetSub12,
						crsminpowerOffsetlSub1, offset_2);
					MOD_PHYREG(pi, crsminpoweroffsetSub12,
						crsminpowerOffsetuSub1, offset_2);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub12,
						crsmfminpowerOffsetlSub1, offset_2);
					MOD_PHYREG(pi, crsmfminpoweroffsetSub12,
						crsmfminpowerOffsetuSub1, offset_2);
				}
				break;
			case 3:
				/* Force the offsets for core-3 */
				/* Core 3 */
				MOD_PHYREG(pi, crsminpoweroffset3,
					crsminpowerOffsetu, offset_3);
				MOD_PHYREG(pi, crsminpoweroffset3,
					crsminpowerOffsetl, offset_3);
				MOD_PHYREG(pi, crsmfminpoweroffset3,
					crsmfminpowerOffsetu, offset_3);
				MOD_PHYREG(pi, crsmfminpoweroffset3,
					crsmfminpowerOffsetl, offset_3);
				MOD_PHYREG(pi, crsminpoweroffsetSub13,
					crsminpowerOffsetlSub1, offset_3);
				MOD_PHYREG(pi, crsminpoweroffsetSub13,
					crsminpowerOffsetuSub1, offset_3);
				MOD_PHYREG(pi, crsmfminpoweroffsetSub13,
					crsmfminpowerOffsetlSub1, offset_3);
				MOD_PHYREG(pi, crsmfminpoweroffsetSub13,
					crsmfminpowerOffsetuSub1, offset_3);
				break;
		default:
			break;
		}
	}
}

void
phy_ac_rxgcrs_set_lesiscale(phy_ac_rxgcrs_info_t *rxgcrsi, int8 *lesi_scale)
{
	uint8 core;
	phy_info_t *pi = rxgcrsi->pi;
	if (rxgcrsi->lesi > 0) {
		uint8 phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
		BCM_REFERENCE(phyrxchain);
		FOREACH_ACTV_CORE(pi, phyrxchain, core) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_32))
			if (ACMAJORREV_32(pi->pubpi->phy_rev)) {
				MOD_PHYREGCE(pi, lesiPathControl0, core,
					inpScalingFactor, lesi_scale[core]);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_32)) */
			{
				MOD_PHYREGCE(pi, lesiInputScaling0_, core,
					inpScalingFactor_0, lesi_scale[core]);
				MOD_PHYREGCE(pi, lesiInputScaling1_, core,
					inpScalingFactor_1, lesi_scale[core]);
				MOD_PHYREGCE(pi, lesiInputScaling2_, core,
					inpScalingFactor_2, lesi_scale[core]);
				MOD_PHYREGCE(pi, lesiInputScaling3_, core,
					inpScalingFactor_3, lesi_scale[core]);
			}
		}
	}
}

static void
phy_ac_rxgcrs_stay_in_carriersearch(phy_type_rxgcrs_ctx_t *ctx, bool enable)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint8 class_mask;

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	/* MAC should be suspended before calling this function */
	ASSERT((R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC) == 0);
	if (enable) {
		if (pi_ac->rxgcrsi->deaf_count == 0) {
			phy_rxgcrs_sel_classifier(pi, 4);
			wlc_phy_ofdm_crs_acphy(pi, FALSE);
			wlc_phy_clip_det_acphy(pi, FALSE);
			WRITE_PHYREG(pi, ed_crsEn, 0);
			if (!ACMAJORREV_32(pi->pubpi->phy_rev) &&
				!ACMAJORREV_33(pi->pubpi->phy_rev)) {
				wlc_phy_resetcca_acphy(pi);
			}
		}

		pi_ac->rxgcrsi->deaf_count++;
#ifdef WL_MUPKTENG
		if (!(D11REV_IS(pi->sh->corerev, 64) && wlapi_is_mutx_pkteng_on(pi->sh->physhim)))
#endif
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			wlc_phy_resetcca_acphy(pi);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	} else {
	  ASSERT(pi_ac->rxgcrsi->deaf_count > 0);

		pi_ac->rxgcrsi->deaf_count--;
		if (pi_ac->rxgcrsi->deaf_count == 0) {
			class_mask = CHSPEC_IS2G(pi->radio_chanspec) ? 7 : 6;   /* No bphy in 5g */
			phy_rxgcrs_sel_classifier(pi, class_mask);
			wlc_phy_ofdm_crs_acphy(pi, TRUE);
			wlc_phy_clip_det_acphy(pi, TRUE);
			WRITE_PHYREG(pi, ed_crsEn, pi_ac->rxgcrsi->edcrs_en);
		}
	}
}

#if defined(BCMDBG) || defined(BCMDBG_DUMP)
void wlc_phy_force_gainlevel_acphy(phy_info_t *pi, int16 int_val)
{
	uint8 core;
	bool suspend = FALSE;
	uint8 phyrxchain;

	BCM_REFERENCE(phyrxchain);

	/* Supsend MAC */
	wlc_phy_conditional_suspend(pi, &suspend);

	phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
	FOREACH_ACTV_CORE(pi, phyrxchain, core) {
		/* disable clip2 */
		MOD_PHYREGC(pi, computeGainInfo, core, disableClip2detect, 1);
		WRITE_PHYREGC(pi, Clip2Threshold, core, 0xffff);
		printf("wlc_phy_force_gainlevel_acphy (%d) : ", int_val);
		switch (int_val) {
		case 0:
			printf("initgain -- adc never clips.\n");
			if (ACREV_IS(pi->pubpi->phy_rev, 0)) {
				WRITE_PHYREGC(pi, Clip1Threshold, core, 0xffff);
			} else {
				/* In 4349 to force INIT gain in core-1, we have to disable
				 * Clip1Detect in core-0, writing 1 to core-1
				 * Disableclip1detect register has no effect
				 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
				if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
					MOD_PHYREGC(pi, computeGainInfo, 0, disableClip1detect, 1);
					WRITE_PHYREGC(pi, Clip1Threshold, 0, 0xffff);
					WRITE_PHYREGC(pi, Adcclip, 0, 0xff);
				} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
				{
					MOD_PHYREGC(pi, computeGainInfo, core,
							disableClip1detect, 1);
					WRITE_PHYREGC(pi, Clip1Threshold, core, 0xffff);
					WRITE_PHYREGC(pi, Adcclip, core, 0xff);
				}
			}
			break;
		case 1:
			printf("clip hi -- adc always clips, nb never clips.\n");
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcNbClipCntTh, 0xff);
			break;
		case 2:
			printf("clip md -- adc/nb always clips, w1 never clips.\n");
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcNbClipCntTh, 0);
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcW1ClipCntTh, 0xff);
			break;
		case 3:
			printf("clip lo -- adc/nb/w1 always clips.\n");
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcNbClipCntTh, 0);
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcW1ClipCntTh, 0);
			break;
		case 4:
			printf("adc clip.\n");
			WRITE_PHYREGC(pi, clipHiGainCodeA, core, 0x0);
			WRITE_PHYREGC(pi, clipHiGainCodeB, core, 0x8);
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcNbClipCntTh, 0xff);
			break;
		case 5:
			printf("nb clip.\n");
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				WRITE_PHYREGC(pi, clipmdGainCodeA, core, 0x4aa);
				WRITE_PHYREGC(pi, clip2GainCodeA, core, 0x4aa);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			{
				WRITE_PHYREGC(pi, clipmdGainCodeA, core, 0xfffe);
				WRITE_PHYREGC(pi, clip2GainCodeA, core, 0xfffe);
			}
			WRITE_PHYREGC(pi, clipmdGainCodeB, core, 0x554);
			MOD_PHYREGC(pi, FastAgcClipCntTh, core, fastAgcW1ClipCntTh, 0xff);
			WRITE_PHYREGC(pi, clip2GainCodeB, core, 0x554);
			break;
		case 6:
			printf("w1 clip.\n");
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				WRITE_PHYREGC(pi, cliploGainCodeA, core, 0x4aa);
				WRITE_PHYREGC(pi, clip2GainCodeA, core, 0x4aa);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			{
				WRITE_PHYREGC(pi, cliploGainCodeA, core, 0xfffe);
				WRITE_PHYREGC(pi, clip2GainCodeA, core, 0xfffe);
			}
			WRITE_PHYREGC(pi, cliploGainCodeB, core, 0x554);
			MOD_PHYREGC(pi, RssiClipMuxSel, core, fastAgcNbClipMuxSel, 0);
			WRITE_PHYREGC(pi, clip2GainCodeB, core, 0x554);
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
			if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
				MOD_RADIO_REGC(pi, NBRSSI_CONFG, core, nbrssi_Refctrl_low, 1);
			}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
			break;
		}
	}
	printf("wlc_phy_force_gainlevel_acphy (%d)\n", int_val);

	/* Resume MAC */
	wlc_phy_conditional_resume(pi, &suspend);
}
#endif /* defined(BCMDBG) || defined(BCMDBG_DUMP) */

uint8 wlc_phy_get_lna_gain_rout(phy_info_t *pi, uint8 idx, acphy_lna_gain_rout_t type)
{
	uint8 ret_val;
	phy_ac_rxg_params_t *rxg_params = pi->u.pi_acphy->rxgcrsi->rxg_params;

	if (IS_28NM_RADIO(pi)) {
		if (type == GET_LNA_GAINCODE) {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				ret_val = (rxg_params->lna1Rout_2g_tbl[idx] & 0x7);
			} else {
				ret_val = (rxg_params->lna1Rout_5g_tbl[idx] & 0x7);
			}
		} else {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				ret_val = (rxg_params->lna1Rout_2g_tbl[idx] >> 3);
			} else {
				ret_val = (rxg_params->lna1Rout_5g_tbl[idx] >> 3);
			}
		}
	} else {
		if (type == GET_LNA_GAINCODE) {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				ret_val = ac_tiny_g_lna_gain_map[idx];
			} else {
				ret_val = ac_tiny_a_lna_gain_map[idx];
			}
		} else {
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				ret_val = ac_tiny_g_lna_rout_map[idx];
			} else {
				ret_val = ac_tiny_a_lna_rout_map[idx];
			}
		}
	}
	return (ret_val);
}

/* This function tells you locale info, e.g EU, so that correct edcrs setting could be done */
static void
phy_ac_rxgcrs_set_locale(phy_type_rxgcrs_ctx_t *ctx, uint8 region_group)
{
	/* USAGE: if (region_group == LOCALE_EU) */
	/* Nothing for now - just a template */
}

void wlc_phy_set_srom_eu_edthresh_acphy(phy_info_t *pi)
{
	int32 eu_edthresh;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	eu_edthresh = CHSPEC_IS2G(pi->radio_chanspec) ?
	        pi->srom_eu_edthresh2g : pi->srom_eu_edthresh5g;
	if (eu_edthresh < -10) /* 0 & 0xff(-1) are invalid values */
		wlc_phy_adjust_ed_thres(pi, &eu_edthresh, TRUE);
	else
		wlc_phy_adjust_ed_thres(pi, &pi_ac->sromi->ed_thresh_default, TRUE);
}

static void
wlc_phy_rxgainctrl_set_gaintbls_acphy_2069(phy_info_t *pi,
	uint8 core, uint16 gain_tblid, uint16 gainbits_tblid)
{
	uint16 gmsz;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	acphy_desense_values_t *desense = pi_ac->rxgcrsi->total_desense;

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		uint8 mixbits_2g[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
		uint8 mix_2g_43352_ilna[] = {0x9, 0x9, 0x9, 0x9, 0x9,
			0x9, 0x9, 0x9, 0x9, 0x9};
		uint8 mix_2g[]  = {0x3, 0x3, 0x3, 0x3, 0x3,
			0x3, 0x3, 0x3, 0x3, 0x3};
		if (PHY_ILNA(pi)) {
			/* Use lna2_gm_sz = 3 (for ACI), mix/tia_gm_sz = 2 */
			ACPHYREG_BCAST(pi, RfctrlCoreLowPwr0, 0x2c);
			ACPHYREG_BCAST(pi, RfctrlOverrideLowPwrCfg0, 0xc);

			wlc_phy_table_write_acphy(pi, gain_tblid, 10, 32, 8, mix_2g_43352_ilna);
			wlc_phy_table_write_acphy(pi, gainbits_tblid, 10, 32, 8, mixbits_2g);

			/* copying values into gaintbl arrays
				to avoid reading from table
			*/
			memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[3],
				mix_2g_43352_ilna,
				sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[3]);
		} else {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
			if ((RADIOID_IS(pi->pubpi->radioid, BCM2069_ID) &&
			     RADIOMAJORREV(pi) == 2) ||
			    (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID) &&
			     RADIOMAJORREV(pi) == 0 &&
			     (RADIOMINORREV(pi) == 16 || RADIOMINORREV(pi) == 17))) {
				/* Use lna2_gm_sz = 2 (for ACI), mix/tia_gm_sz = 1 */
				gmsz = desense->elna_bypass ? 0x2c : 0x28;
				wlc_phy_table_write_acphy(pi, gain_tblid, 10, 32, 8,
					mix_2g_43352_ilna);
				/* copying values into gaintbl arrays
				to avoid reading from table
				*/
				memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[3],
					mix_2g_43352_ilna,
					sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[3]);
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
			{
				/* Use lna2_gm_sz = 2 (for ACI), mix/tia_gm_sz = 1 */
				gmsz = desense->elna_bypass ? 0x1c : 0x18;
				wlc_phy_table_write_acphy(pi, gain_tblid, 10, 32, 8, mix_2g);
				memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[3], mix_2g,
					sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[3]);
			}
			ACPHYREG_BCAST(pi, RfctrlCoreLowPwr0, gmsz);
			ACPHYREG_BCAST(pi, RfctrlOverrideLowPwrCfg0, 0xc);
			wlc_phy_table_write_acphy(pi, gainbits_tblid, 10, 32, 8, mixbits_2g);
			/* copying values into gaintbl arrays to avoid reading from table */
			memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[3], mixbits_2g,
				sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[3]);
		}
	} else {
		/* 5g settings */
		uint8 mix5g_elna[]  = {0x7, 0x7, 0x7, 0x7, 0x7,
		                       0x7, 0x7, 0x7, 0x7, 0x7};
		uint8 mixbits5g_elna[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
		uint8 mix5g_ilna[]  = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
		uint8 mixbits5g_ilna[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
		uint8 *mix_5g;
		uint8 *mixbits_5g;

		/* Mixer tables based on elna/ilna */
		if (BF_ELNA_5G(pi_ac)) {
			mix_5g = mix5g_elna;
			mixbits_5g = mixbits5g_elna;
		} else {
			mix_5g = mix5g_ilna;
			mixbits_5g = mixbits5g_ilna;
		}

		/* Use lna2_gm_sz = 3, mix/tia_gm_sz = 2 */
		ACPHYREG_BCAST(pi, RfctrlCoreLowPwr0, 0x2c);
		ACPHYREG_BCAST(pi, RfctrlOverrideLowPwrCfg0, 0xc);

		wlc_phy_table_write_acphy(pi, gain_tblid, 10, 32, 8, mix_5g);
		wlc_phy_table_write_acphy(pi, gainbits_tblid, 10, 32, 8, mixbits_5g);

		/* copying values into gaintbl arrays to avoid reading from table */
		memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[3], mix_5g,
		       sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[3]);
	}
}

void
phy_ac_rxgcrs_upd_mix_gains(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	phy_info_t *pi = rxgcrsi->pi;
	acphy_desense_values_t *desense = NULL;
	uint8 core, idx, desense_state;
	int8 tia_mix_gain[13];
	uint8 tia_mix_gainbits[13];
	uint8 tia_mix_gainlimits[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7f};
	uint8 gain_tbl_entry_size =
		sizeof(rxgcrsi->rxgainctrl_params[0].gaintbl[0][0]);
	uint8 gainbit_tbl_entry_size =
		sizeof(rxgcrsi->rxgainctrl_params[0].gainbitstbl[0][0]);
	uint16 gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0;
	uint16 gain_tblid = ACPHY_TBL_ID_GAIN0;
	uint16 gainbits_tblid = ACPHY_TBL_ID_GAINBITS0;
	const uint8 *default_gain_tbl = ac_tia_gain_tiny_4365;
	const uint8 *default_gainbits_tbl = ac_tia_gainbits_tiny_4365;
	uint8 stall_val;

	if ((desense = phy_ac_noise_get_desense(rxgcrsi->aci->noisei)) == NULL) {
		desense_state = 0;
	} else {
		desense_state = phy_ac_noise_get_desense_state(rxgcrsi->aci->noisei);
	}

	/* disable stalls for table write */
	stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
	ACPHY_DISABLE_STALL(pi);

	if (desense_state != 0) {
		for (idx = 0; idx < rxgcrsi->rxgainctrl_stage_len[TIA_ID]; idx++) {
			tia_mix_gainbits[idx] = MIN(desense->mix_idx_max,
					MAX(idx, desense->mix_idx_min));
			tia_mix_gain[idx] = default_gain_tbl[tia_mix_gainbits[idx]];
		}
	} else {
		memcpy(tia_mix_gain, default_gain_tbl,
				sizeof(uint8) * rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
		memcpy(tia_mix_gainbits, default_gainbits_tbl,
				sizeof(uint8) * rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
	}

	for (core = 0; core < PHY_CORE_MAX; core++) {
		gain_tblid = ACPHY_TBL_ID_GAIN0 + core * 32;
		gainbits_tblid = ACPHY_TBL_ID_GAINBITS0 + core * 32;
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0 + core * 32;

		wlc_phy_set_agc_gaintbls_acphy(
			pi,
			gain_tblid, tia_mix_gain,
			gainbits_tblid, tia_mix_gainbits,
			gainlimit_tblid, tia_mix_gainlimits, tia_mix_gainlimits,
			TIA_OFFSET,
			rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
		memcpy(rxgcrsi->rxgainctrl_params[core].gaintbl[TIA_ID],
		       tia_mix_gain,
		       gain_tbl_entry_size * rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
		memcpy(rxgcrsi->rxgainctrl_params[core].gainbitstbl[TIA_ID],
		       tia_mix_gainbits,
		       gainbit_tbl_entry_size * rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
	}

	ACPHY_ENABLE_STALL(pi, stall_val);
}

void
wlc_phy_rxgainctrl_set_gaintbls_acphy_wave2(
	phy_info_t *pi,
	uint8      core,
	uint16     gain_tblid,
	uint16     gainbits_tblid)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	bool elna_present = CHSPEC_IS2G(pi->radio_chanspec) ?
		BF_ELNA_2G(pi_ac) : BF_ELNA_5G(pi_ac);
	int8 elna = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][0];
	int8 elna_gain[] = {0, 0};
	uint8 elna_gainbits[] = {0, 0};
	uint8 elna_gainlimits[] = {0, 0};

	uint8 lna2rout[] = {0, 1, 2, 3, 3, 3, 3};
	int8 tia_mix_gain[] = {10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 37, 37, 37};
	uint8 tia_mix_gainbits[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9};
	uint8 tia_mix_gainlimits[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7f};

	int8 dvga1_gain[] = {-12, -6, 0};
	uint8 dvga1_gainbits[] = {0, 1, 2};
	uint8 dvga1_gainlimits[] = {0, 0, 0};
	int8 dvga_gain[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42};
	uint8 dvga_gainbits[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	uint8 gain_tbl_entry_size = sizeof(pi_ac->rxgcrsi->rxgainctrl_params[0].gaintbl[0][0]);
	uint8 gainbit_tbl_entry_size =
		sizeof(pi_ac->rxgcrsi->rxgainctrl_params[0].gainbitstbl[0][0]);
	uint16 gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT;

	if (core == 0)
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0;
	else if (core == 1)
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT1;
	else if (core == 2)
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT2;
	else
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT3;

	/* ELNA */
	elna_gain[0] = elna_present ? elna : 0;
	elna_gain[1] = elna_present ? elna : 0;
	wlc_phy_set_agc_gaintbls_acphy(
		pi,
		gain_tblid, elna_gain,
		gainbits_tblid, elna_gainbits,
		gainlimit_tblid, elna_gainlimits, elna_gainlimits,
		ELNA_OFFSET,
		pi_ac->rxgcrsi->rxgainctrl_stage_len[ELNA_ID]);
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[ELNA_ID],
	       elna_gain,
	       gain_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[ELNA_ID]);
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[ELNA_ID],
	       elna_gainbits,
	       gainbit_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[ELNA_ID]);

	/* LNA2 ROUT (there is no ROUT for lna2, but index is important) */
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 7,
	                          (ACPHY_LNAROUT_CORE_WRT_OFST(pi->pubpi.phy_rev,
	                                                       core) + 16), 8, &lna2rout);
	/* LNA1 & LNA2 */
	wlc_phy_upd_lna1_lna2_gains_acphy(pi);

	/* TIA+MIX */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		phy_ac_rxgcrs_upd_mix_gains(pi_ac->rxgcrsi);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	{
		wlc_phy_set_agc_gaintbls_acphy(
			pi,
			gain_tblid, tia_mix_gain,
			gainbits_tblid, tia_mix_gainbits,
			gainlimit_tblid, tia_mix_gainlimits, tia_mix_gainlimits,
			TIA_OFFSET,
			pi_ac->rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
		memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[TIA_ID],
		       tia_mix_gain,
		       gain_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
		memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[TIA_ID],
		       tia_mix_gainbits,
		       gainbit_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[TIA_ID]);
	}
	/* BQ0 */
	memset(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[BQ0_ID],
	       0,
	       gain_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[BQ0_ID]);
	memset(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[BQ0_ID],
	       0,
	       gainbit_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[BQ0_ID]);
	/* BQ1/DVGA1 */
	wlc_phy_set_agc_gaintbls_acphy(
		pi,
		gain_tblid, dvga1_gain,
		gainbits_tblid, dvga1_gainbits,
		gainlimit_tblid, dvga1_gainlimits, dvga1_gainlimits,
		BQ1_OFFSET,
		pi_ac->rxgcrsi->rxgainctrl_stage_len[BQ1_ID]);
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[BQ1_ID],
	       dvga1_gain,
	       gain_tbl_entry_size *  pi_ac->rxgcrsi->rxgainctrl_stage_len[BQ1_ID]);
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[BQ1_ID],
	       dvga1_gainbits,
	       gainbit_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[BQ1_ID]);
	/* DVGA */
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[DVGA_ID],
	       dvga_gain,
	       gain_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[DVGA_ID]);
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[DVGA_ID],
	       dvga_gainbits,
	       gainbit_tbl_entry_size * pi_ac->rxgcrsi->rxgainctrl_stage_len[DVGA_ID]);

	wlc_phy_upd_lna1_bypass_acphy(pi, core);
}

void
wlc_phy_rxgainctrl_set_gaintbls_acphy_tiny(phy_info_t *pi,
	uint8 core, uint16 gain_tblid, uint16 gainbits_tblid)
{
	phy_ac_rxgcrs_info_t *ri = pi->u.pi_acphy->rxgcrsi;
	acphy_desense_values_t *desense = ri->total_desense;
	uint8 farrow_shift = 2;

	/* TIA */
	/* 2g & 5g settings */
	uint8 tia[]     = {10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43};
	uint8 tiabits[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11};
	int8  lna1_gain_g[] = { -6, 0, 6, 12, 18, 24};
	int8  lna1_gain_a[] = { -5, 0, 6, 12, 18, 24};
	int8  *lna1_gain = (CHSPEC_IS2G(pi->radio_chanspec) || !ACMAJORREV_4(pi->pubpi->phy_rev))
		? lna1_gain_g : lna1_gain_a;
	uint8 lna1_gainbits[] = {0, 1, 2, 3, 4, 5};
	int8 elna_gain = ri->rxgainctrl_params[core].gaintbl[0][0];
	uint8 i;
	bool elna_present = CHSPEC_IS2G(pi->radio_chanspec) ? BF_ELNA_2G(ri->aci)
	                                                    : BF_ELNA_5G(ri->aci);
	uint8 lna1_min_indx; /* default ilna parameters */
	uint8 lna1_max_indx;
	uint8 tia_max_indx;
	int8 lna1_max_gain = lna1_gain[5];
	/* eventually we may move into lna_present clause below */
	const uint8 unused = GAINLIMIT_MAX_OUT;
	const uint8 used = 0;
	uint16 gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		gainlimit_tblid = (core) ? ACPHY_TBL_ID_GAINLIMIT1 : ACPHY_TBL_ID_GAINLIMIT0;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		if (core == 0)
			gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0;
		else if (core == 1)
			gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT1;
		else if (core == 2)
			gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT2;
		else
			gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT3;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	{
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT;
	}

	wlc_phy_table_write_acphy(pi, gainbits_tblid, 1, DVGA1_OFFSET + 3,
		ACPHY_GAINBITS_TBL_WIDTH, &farrow_shift);

	if (desense->mixer_setting_desense > 0) {
			tia_max_indx = 11;
			tia_max_indx -= ri->curr_desense->mixer_setting_desense;

			/* limit tia */
			for (i = 0; i < 12; i++) {
				if (i >= (tia_max_indx+1)) {
					tia[i]	 = tia[tia_max_indx];
					tiabits[i] = tia_max_indx;
			/* below, input value 1 is to write one table entry at a time */
				wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, TIA_OFFSET + i,
					ACPHY_GAINLMT_TBL_WIDTH, &unused);
				} else {
			/* below, input value 1 is to write one table entry at a time */
				wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, TIA_OFFSET + i,
					ACPHY_GAINLMT_TBL_WIDTH, &used);
				}
			}
		}

	wlc_phy_table_write_acphy(pi, gain_tblid, ri->rxgainctrl_stage_len[TIA_ID],
		TIA_OFFSET,	ACPHY_GAINDB_TBL_WIDTH, tia);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, ri->rxgainctrl_stage_len[TIA_ID],
		TIA_OFFSET,	ACPHY_GAINBITS_TBL_WIDTH, tiabits);

	/* copying values into gaintbl arrays to avoid reading from table */
	memcpy(ri->rxgainctrl_params[core].gaintbl[TIA_ID], tia,
		sizeof(ri->rxgainctrl_params[core].gaintbl[0][0])
		* ri->rxgainctrl_stage_len[TIA_ID]);

	memcpy(ri->rxgainctrl_params[core].gainbitstbl[TIA_ID], tiabits,
		sizeof(ri->rxgainctrl_params[core].gainbitstbl[0][0])
		* ri->rxgainctrl_stage_len[TIA_ID]);

	/* backoff LNA1 for ELNA/iLNA(ACI) operation */
	if (elna_present) {
		if (ACPHY_LO_NF_MODE_ELNA_TINY(pi))
			lna1_min_indx = 5;
		else
			lna1_min_indx = 4;
		if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
			if (ri->curr_desense->lna1_tbl_desense < lna1_min_indx)
				lna1_min_indx -= ri->curr_desense->lna1_tbl_desense;
			else
				lna1_min_indx = 0;
		}

		lna1_max_indx = 5;

		/* find maximum gain */
		for (i = lna1_min_indx;
					(i <= lna1_max_indx) &&
					(elna_gain + lna1_gain[i] < lna1_max_gain);
					i++)
			;
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		if ((desense->lna1_tbl_desense > 0) && (ACMAJORREV_4(pi->pubpi->phy_rev))) {
			lna1_max_indx = lna1_min_indx;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		{
			lna1_max_indx = (i > lna1_max_indx) ? lna1_max_indx : i;
		}
	} else {
		/* iLNA LNA1 configuration
		 * eg. set lna1_max_lna=4 to reduce LNA1 gain by 6 dB.
		 * The algorithm will allocate more gain to tia.
		 * Further changes in gain distribution can be tuned by setting max_analog_gain.
		 */
		lna1_min_indx = 0; /* unused */
		lna1_max_indx = 5;
	}

	/* modify encode gain to obey gainlimit */

	lna1_max_gain = lna1_gain[lna1_max_indx];

	/* limit LNA1 appropriately */
	for (i = 0; i < 6; i++) {
		if (i >= (lna1_max_indx + 1)) {
			lna1_gain[i]     = lna1_gain[lna1_max_indx];
			lna1_gainbits[i] = lna1_max_indx;
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, LNA1_OFFSET + i,
				ACPHY_GAINLMT_TBL_WIDTH, &unused);
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, (LNA1_OFFSET +
			GAINLMT_TBL_BAND_OFFSET + i), ACPHY_GAINLMT_TBL_WIDTH, &unused);
		} else {
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, LNA1_OFFSET + i,
				ACPHY_GAINLMT_TBL_WIDTH, &used);
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, (LNA1_OFFSET +
			GAINLMT_TBL_BAND_OFFSET + i), ACPHY_GAINLMT_TBL_WIDTH, &used);
		}
	}

	/* need this for encode gain to work correctly */
	wlc_phy_table_write_acphy(pi, gain_tblid, ri->rxgainctrl_stage_len[LNA1_ID],
		LNA1_OFFSET, ACPHY_GAINDB_TBL_WIDTH, lna1_gain);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, ri->rxgainctrl_stage_len[LNA1_ID],
		LNA1_OFFSET, ACPHY_GAINBITS_TBL_WIDTH, lna1_gainbits);

	/* copying values into gaintbl arrays to avoid reading from hw */
	memcpy(ri->rxgainctrl_params[core].gaintbl[LNA1_ID], lna1_gain,
		sizeof(ri->rxgainctrl_params[core].gaintbl[0][0])
		* ri->rxgainctrl_stage_len[LNA1_ID]);

	memcpy(ri->rxgainctrl_params[core].gainbitstbl[LNA1_ID], lna1_gainbits,
		sizeof(ri->rxgainctrl_params[core].gainbitstbl[0][0])
		* ri->rxgainctrl_stage_len[LNA1_ID]);

	/* write max analog gain */
	if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
		uint8 max_analog_gain;

		max_analog_gain = (MAX_ANALOG_RX_GAIN_TINY >=
			ri->curr_desense->analog_gain_desense_ofdm) ? (MAX_ANALOG_RX_GAIN_TINY -
			ri->curr_desense->analog_gain_desense_ofdm) : 0;

		MOD_PHYREGC(pi, HpFBw, core, maxAnalogGaindb, max_analog_gain);
		max_analog_gain = (MAX_ANALOG_RX_GAIN_TINY >=
			ri->curr_desense->analog_gain_desense_bphy) ? (MAX_ANALOG_RX_GAIN_TINY -
			ri->curr_desense->analog_gain_desense_bphy) : 0;
		MOD_PHYREGC(pi, DSSScckPktGain, core, dsss_cck_maxAnalogGaindb, max_analog_gain);
	} else {
		MOD_PHYREGC(pi, HpFBw, core, maxAnalogGaindb, MAX_ANALOG_RX_GAIN_TINY);
		MOD_PHYREGC(pi, DSSScckPktGain, core, dsss_cck_maxAnalogGaindb,
		            MAX_ANALOG_RX_GAIN_TINY);
	}
}

void
wlc_phy_rxgainctrl_set_gaintbls_acphy_28nm_ulp(phy_info_t *pi,
	uint8 core, uint16 gain_tblid, uint16 gainbits_tblid)
{
	phy_ac_rxgcrs_info_t *ri = pi->u.pi_acphy->rxgcrsi;
	acphy_desense_values_t *desense = ri->total_desense;
	phy_ac_rxg_params_t *rxg_params = ri->rxg_params;

	/* TIA */
	/* 2g & 5g settings */
	uint8 tia[N_TIA_GAINS];
	uint8 tiabits[N_TIA_GAINS];
	int8  lna1_gain[N_LNA12_GAINS];
	uint8 lna1_gainbits[N_LNA12_GAINS];
	uint8 lna1_gain_limit[N_LNA12_GAINS];
	int8 elna_gain = ri->rxgainctrl_params[core].gaintbl[0][0];
	uint8 i;
	bool elna_present = CHSPEC_IS2G(pi->radio_chanspec) ? BF_ELNA_2G(ri->aci)
	                                                    : BF_ELNA_5G(ri->aci);
	uint8 lna1_min_indx; /* default ilna parameters */
	uint8 lna1_max_indx;
	uint8 tia_max_indx;
	int8 lna1_max_gain;
	uint8 lna1_byp_bit = rxg_params->lna1_byp_indx;
	uint8 max_analog_gain;

	/* eventually we may move into lna_present clause below */
	const uint8 unused = GAINLIMIT_MAX_OUT;
	const uint8 used = 0;
	uint16 gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0;
	wlc_phy_table_write_acphy(pi, gainbits_tblid, 1, DVGA1_OFFSET + 3,
		ACPHY_GAINBITS_TBL_WIDTH, &rxg_params->farrow_shift);

	/* Copy gains from params */
	memcpy(tia, rxg_params->tia_gain_tbl, sizeof(int8) * N_TIA_GAINS);
	memcpy(tiabits, rxg_params->tia_gainbits_tbl, sizeof(int8) * N_TIA_GAINS);

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		memcpy(lna1_gain, rxg_params->lna12_gain_2g_tbl[0], sizeof(int8) * N_LNA12_GAINS);
		memcpy(lna1_gainbits, rxg_params->lna12_gainbits_2g_tbl[0],
				sizeof(int8) * N_LNA12_GAINS);
	} else {
		memcpy(lna1_gain, rxg_params->lna12_gain_5g_tbl[0], sizeof(int8) * N_LNA12_GAINS);
		memcpy(lna1_gainbits, rxg_params->lna12_gainbits_5g_tbl[0],
				sizeof(int8) * N_LNA12_GAINS);
	}

	memcpy(lna1_gain_limit, rxg_params->gainlimit_tbl[LNA1_TBL_IND],
			ARRAYSIZE(lna1_gain_limit));

	lna1_max_gain = lna1_gain[rxg_params->lna1_max_indx];

	if (desense->mixer_setting_desense > 0) {
		tia_max_indx = rxg_params->tia_max_indx;
		tia_max_indx -= ri->curr_desense->mixer_setting_desense;

		/* limit tia */
		for (i = 0; i < N_TIA_GAINS; i++) {
			if (i >= (tia_max_indx+1)) {
				tia[i]	 = tia[tia_max_indx];
				tiabits[i] = tia_max_indx;
				/* below, input value 1 is to write one table entry at a time */
				wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, TIA_OFFSET + i,
						ACPHY_GAINLMT_TBL_WIDTH, &unused);
			} else {
				/* below, input value 1 is to write one table entry at a time */
				wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, TIA_OFFSET + i,
						ACPHY_GAINLMT_TBL_WIDTH, &used);
			}
		}
	}

	wlc_phy_table_write_acphy(pi, gain_tblid, ri->rxgainctrl_stage_len[TIA_ID],
		TIA_OFFSET, ACPHY_GAINDB_TBL_WIDTH, tia);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, ri->rxgainctrl_stage_len[TIA_ID],
		TIA_OFFSET, ACPHY_GAINBITS_TBL_WIDTH, tiabits);
	/* copying values into gaintbl arrays to avoid reading from table */
	memcpy(ri->rxgainctrl_params[core].gaintbl[TIA_ID], tia,
		sizeof(ri->rxgainctrl_params[core].gaintbl[0][0])
		* ri->rxgainctrl_stage_len[TIA_ID]);

	memcpy(ri->rxgainctrl_params[core].gainbitstbl[TIA_ID], tiabits,
		sizeof(ri->rxgainctrl_params[core].gainbitstbl[0][0])
		* ri->rxgainctrl_stage_len[TIA_ID]);

	/* backoff LNA1 for ELNA/iLNA(ACI) operation */
	if (elna_present) {
		lna1_min_indx = rxg_params->lna1_min_indx;
		if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
			if (ri->curr_desense->lna1_tbl_desense < lna1_min_indx)
				lna1_min_indx -= ri->curr_desense->lna1_tbl_desense;
			else
				lna1_min_indx = rxg_params->lna1_min_indx;
		}

		if (ACPHY_LO_NF_MODE_ELNA_28NM(pi)) {
			/* Do not back off LNA1, hence set min as max */
			lna1_min_indx = rxg_params->lna1_lonf_max_indx;
		}

		lna1_max_indx = rxg_params->lna1_max_indx;

		/* find maximum gain */
		for (i = lna1_min_indx; (i <= lna1_max_indx) &&
				(elna_gain + lna1_gain[i] < lna1_max_gain); i++);

			lna1_max_indx = (i > lna1_max_indx) ? lna1_max_indx : i;
	} else {
		/* iLNA LNA1 configuration
		 * eg. set lna1_max_lna=4 to reduce LNA1 gain by 6 dB.
		 * The algorithm will allocate more gain to tia.
		 * Further changes in gain distribution can be tuned by setting max_analog_gain.
		 */
		lna1_min_indx = rxg_params->lna1_min_indx; /* unused */
		lna1_max_indx = rxg_params->lna1_max_indx;
	}

	/* modify encode gain to obey gainlimit */
	lna1_max_gain = lna1_gain[lna1_max_indx];

	/* limit LNA1 appropriately */
	for (i = 0; i < 6; i++) {
		if (i >= (lna1_max_indx + 1)) {
			lna1_gain[i]     = lna1_gain[lna1_max_indx];
			lna1_gainbits[i] = lna1_max_indx;
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, LNA1_OFFSET + i,
				ACPHY_GAINLMT_TBL_WIDTH, &unused);
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, (LNA1_OFFSET +
			GAINLMT_TBL_BAND_OFFSET + i), ACPHY_GAINLMT_TBL_WIDTH, &unused);
		} else {
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, LNA1_OFFSET + i,
				ACPHY_GAINLMT_TBL_WIDTH, &(lna1_gain_limit[i]));
			/* below, input value 1 is to write one table entry at a time */
			wlc_phy_table_write_acphy(pi, gainlimit_tblid, 1, (LNA1_OFFSET +
			GAINLMT_TBL_BAND_OFFSET + i), ACPHY_GAINLMT_TBL_WIDTH,
			 &(lna1_gain_limit[i]));
		}
	}

	/* need this for encode gain to work correctly */
	wlc_phy_table_write_acphy(pi, gain_tblid, ri->rxgainctrl_stage_len[LNA1_ID],
		LNA1_OFFSET, ACPHY_GAINDB_TBL_WIDTH, lna1_gain);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, ri->rxgainctrl_stage_len[LNA1_ID],
		LNA1_OFFSET, ACPHY_GAINBITS_TBL_WIDTH, lna1_gainbits);

	/* Put LNA1 bypass index in gainbits */
	wlc_phy_table_write_acphy(pi, gainbits_tblid, 1,
		(LNA1_OFFSET + 6), ACPHY_GAINBITS_TBL_WIDTH, &lna1_byp_bit);

	/* copying values into gaintbl arrays to avoid reading from hw */
	memcpy(ri->rxgainctrl_params[core].gaintbl[LNA1_ID], lna1_gain,
		sizeof(ri->rxgainctrl_params[core].gaintbl[0][0])
		* ri->rxgainctrl_stage_len[LNA1_ID]);

	memcpy(ri->rxgainctrl_params[core].gainbitstbl[LNA1_ID], lna1_gainbits,
		sizeof(ri->rxgainctrl_params[core].gainbitstbl[0][0])
		* ri->rxgainctrl_stage_len[LNA1_ID]);

	/* write max analog gain */
	if (ACPHY_ENABLE_FCBS_HWACI(pi)) {
		max_analog_gain = (ACPHY_LO_NF_MODE_ELNA_28NM(pi)) ?
			rxg_params->max_analog_gain_lonf : rxg_params->max_analog_gain;
		max_analog_gain = (max_analog_gain >=
			ri->curr_desense->analog_gain_desense_ofdm) ?
			(max_analog_gain -
			ri->curr_desense->analog_gain_desense_ofdm) : 0;
		MOD_PHYREGC(pi, HpFBw, core, maxAnalogGaindb, max_analog_gain);

		max_analog_gain = (ACPHY_LO_NF_MODE_ELNA_28NM(pi)) ?
			rxg_params->max_analog_gain_lonf : rxg_params->max_analog_gain;
		max_analog_gain = (max_analog_gain >=
			ri->curr_desense->analog_gain_desense_bphy) ?
			(max_analog_gain -
			ri->curr_desense->analog_gain_desense_bphy) : 0;
		MOD_PHYREGC(pi, DSSScckPktGain, core, dsss_cck_maxAnalogGaindb, max_analog_gain);
	} else {
		max_analog_gain = (ACPHY_LO_NF_MODE_ELNA_28NM(pi)) ?
			rxg_params->max_analog_gain_lonf : rxg_params->max_analog_gain;
		MOD_PHYREGC(pi, HpFBw, core, maxAnalogGaindb, max_analog_gain);
		MOD_PHYREGC(pi, DSSScckPktGain, core, dsss_cck_maxAnalogGaindb,
		            max_analog_gain);
	}

	/* LNA1 bypass related config */
	if (ri->fem_rxgains[core].lna1byp) {
		phy_ac_upd_lna1_bypass(pi, core);
	}
}

void
wlc_phy_rxgainctrl_set_gaintbls_acphy(phy_info_t *pi,
	bool init, bool band_change, bool bw_change)
{
	uint8 elna[2], ant;

	/* lna1 GainLimit */
	uint8 stall_val, core, i;
	uint16 save_forclks, gain_tblid, gainbits_tblid;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	bool suspend;

	suspend = !(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC);
	if (!suspend) {
		wlapi_suspend_mac_and_wait(pi->sh->physhim);
		phy_utils_phyreg_enter(pi);
	}
	/* Disable stall before writing tables */
	stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
	ACPHY_DISABLE_STALL(pi);

	/* If receiver is in active demod, it will NOT update the Gain tables */
	save_forclks = READ_PHYREG(pi, fineRxclockgatecontrol);
	MOD_PHYREG(pi, fineRxclockgatecontrol, forcegaingatedClksOn, 1);

	/* LNA1/2 (always do this, as the previous channel could have been in ACI mitigation) */
	wlc_phy_upd_lna1_lna2_gains_acphy(pi);

	FOREACH_CORE(pi, core) {
		if (core == 0) {
			gain_tblid =  ACPHY_TBL_ID_GAIN0;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS0;
		} else if (core == 1) {
			gain_tblid =  ACPHY_TBL_ID_GAIN1;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS1;
		} else if (core == 2) {
			gain_tblid =  ACPHY_TBL_ID_GAIN2;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS2;
		} else {
			gain_tblid =  ACPHY_TBL_ID_GAIN3;
			gainbits_tblid =  ACPHY_TBL_ID_GAINBITS3;
		}

		/* FEM - elna, trloss (from srom) */
		ant = phy_get_rsdbbrd_corenum(pi, core);
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			elna[0] = elna[1] = pi_ac->sromi->femrx_2g[ant].elna;
			pi_ac->rxgcrsi->fem_rxgains[core].elna = elna[0];
			pi_ac->rxgcrsi->fem_rxgains[core].trloss =
				pi_ac->sromi->femrx_2g[ant].trloss;
			pi_ac->rxgcrsi->fem_rxgains[core].elna_bypass_tr =
			        pi_ac->sromi->femrx_2g[ant].elna_bypass_tr;
			pi_ac->rxgcrsi->fem_rxgains[core].lna1byp =
				((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
				& BFL2_LNA1BYPFORTR2G) != 0);
		} else if (CHSPEC_CHANNEL(pi->radio_chanspec) < 100) {
			elna[0] = elna[1] = pi_ac->sromi->femrx_5g[ant].elna;
			pi_ac->rxgcrsi->fem_rxgains[core].elna = elna[0];
			pi_ac->rxgcrsi->fem_rxgains[core].trloss =
				pi_ac->sromi->femrx_5g[ant].trloss;
			pi_ac->rxgcrsi->fem_rxgains[core].elna_bypass_tr =
			        pi_ac->sromi->femrx_5g[ant].elna_bypass_tr;
			pi_ac->rxgcrsi->fem_rxgains[core].lna1byp =
				((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
				& BFL2_LNA1BYPFORTR5G) != 0);
		} else {
			elna[0] = elna[1] = pi_ac->sromi->femrx_5gh[ant].elna;
			pi_ac->rxgcrsi->fem_rxgains[core].elna = elna[0];
			pi_ac->rxgcrsi->fem_rxgains[core].trloss =
				pi_ac->sromi->femrx_5gh[ant].trloss;
			pi_ac->rxgcrsi->fem_rxgains[core].elna_bypass_tr =
			        pi_ac->sromi->femrx_5gh[ant].elna_bypass_tr;
			pi_ac->rxgcrsi->fem_rxgains[core].lna1byp =
				((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
				& BFL2_LNA1BYPFORTR5G) != 0);
		}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			uint8 num_rssi_cal_gi;
			num_rssi_cal_gi = CHSPEC_IS2G(pi->radio_chanspec)?
				pi_ac->sromi->num_rssi_cal_gi_2g:
				pi_ac->sromi->num_rssi_cal_gi_5g;
			if (num_rssi_cal_gi == 3) {
				/* Adjust gaindB table based on GI 3 RSSI CAL results */
				/* Apply adj only for 3 point calibation (GI 1/4/3) */
				wlc_phy_rxgain_adj_forrssi_acphy(pi, core, 3);
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */

		/* RSSI elna on/off delta adjusted through trinT register */
		wlc_phy_set_trloss_reg_acphy(pi, core);
		wlc_phy_table_write_acphy(pi, gain_tblid, 2, 0, 8, elna);

		memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0],
		       elna, sizeof(uint8)*pi_ac->rxgcrsi->rxgainctrl_stage_len[0]);

		/* MIX, LPF / TIA */
		if (TINY_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				wlc_phy_rxgainctrl_set_gaintbls_acphy_wave2(
					pi, core, gain_tblid, gainbits_tblid);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				wlc_phy_rxgainctrl_set_gaintbls_acphy_tiny(
					pi, core, gain_tblid, gainbits_tblid);
			}
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			wlc_phy_rxgainctrl_set_gaintbls_acphy_28nm_ulp(pi, core, gain_tblid,
				gainbits_tblid);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
		if (IS_28NM_RADIO(pi)) {
			if (init||band_change) {
				wlc_phy_rxgainctrl_set_gaintbls_acphy_28nm(pi, core, gain_tblid,
					gainbits_tblid);
			}
		} else if (init || band_change) {
			wlc_phy_rxgainctrl_set_gaintbls_acphy_2069(pi, core, gain_tblid,
				gainbits_tblid);
		}

		if (init) {
			/* Store gainctrl info (to be used for Auto-Gainctrl)
			 * lna1,2 taken care in wlc_phy_upd_lna1_lna2_gaintbls_acphy()
			 */
			wlc_phy_table_read_acphy(pi, gainbits_tblid, 1, 0, 8,
				pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[0]);
			wlc_phy_table_read_acphy(pi, gainbits_tblid, 10, 32, 8,
			    pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[3]);
			wlc_phy_table_read_acphy(pi, gain_tblid, 8, 96, 8,
			    pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[4]);
			wlc_phy_table_read_acphy(pi, gain_tblid, 8, 112, 8,
			    pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[5]);
			wlc_phy_table_read_acphy(pi, gainbits_tblid, 8, 96, 8,
			    pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[4]);
			wlc_phy_table_read_acphy(pi, gainbits_tblid, 8, 112, 8,
			    pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[5]);

			if (TINY_RADIO(pi) || IS_28NM_RADIO(pi)) {
				/* initialise DVGA table */
				for (i = 0; i < pi_ac->rxgcrsi->rxgainctrl_stage_len[6]; i++) {
					pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[6][i] =
						3 * i;
					pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[6][i] =
						i;
				}
			}
		}
	}

	/* Restore */
	WRITE_PHYREG(pi, fineRxclockgatecontrol, save_forclks);
	ACPHY_ENABLE_STALL(pi, stall_val);
	if (!suspend) {
		phy_utils_phyreg_exit(pi);
		wlapi_enable_mac(pi->sh->physhim);
	}
}

void
wlc_phy_set_trloss_reg_acphy(phy_info_t *pi, int8 core)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 boardloss = 2, trt, trr, ant, core_freq_segment_map;
	int8 rssi_tr_offset = 0;
	int8  bw_idx, subband_idx;
	uint8 bands[NUM_CHANS_IN_CHAN_BONDING];
	uint16 intc, mask = ACPHY_RfctrlIntc0_override_tr_sw_MASK(rev) |
	        ACPHY_RfctrlIntc0_tr_sw_tx_pu_MASK(rev) | ACPHY_RfctrlIntc0_tr_sw_rx_pu_MASK(rev);
	bool suspend = FALSE;
	bool rssi_cal_rev = phy_ac_rssi_get_data(pi_ac->rssii)->rssi_cal_rev;
	wlc_phy_conditional_suspend(pi, &suspend);

	bw_idx = (CHSPEC_IS80(pi->radio_chanspec) || PHY_AS_80P80(pi, pi->radio_chanspec)) ? 2 :
			(CHSPEC_IS160(pi->radio_chanspec)) ? 3 :
	        (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	core_freq_segment_map = phy_ac_chanmgr_get_data(pi_ac->chanmgri)->core_freq_mapping[core];
	ant = phy_get_rsdbbrd_corenum(pi, core);

	if (rssi_cal_rev == FALSE) {
		if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
			phy_ac_chanmgr_get_chan_freq_range_80p80(pi, pi->radio_chanspec, bands);
			subband_idx = (core <= 1) ? (bands[0] - 1) : (bands[1] - 1);
		} else {
			subband_idx = phy_ac_chanmgr_get_chan_freq_range(pi,
			  pi->radio_chanspec, core_freq_segment_map)-1;
		}
	} else {
		subband_idx = wlc_phy_rssi_get_chan_freq_range_acphy(pi, core_freq_segment_map,
			core);
	}
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
	  if (rssi_cal_rev == FALSE) {
	    rssi_tr_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_ON][bw_idx];
	  } else {
	    rssi_tr_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  }
	} else {
	  if (rssi_cal_rev == FALSE) {
	    rssi_tr_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  } else {
	    rssi_tr_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  }
	}
	if (rssi_cal_rev == TRUE) {
	  /* With new scheme, rssi gain delta's are in qdB steps */
	  rssi_tr_offset = (rssi_tr_offset + 2) >> 2;
	}

	/* adjust TRLoss with rssi cal param */
	trr = boardloss;
	trt = pi_ac->rxgcrsi->fem_rxgains[core].trloss + rssi_tr_offset;

	/* Not sure why all 4335 boards don't need boardloss = 2 (chip default) */
	if (ACMAJORREV_0(pi->pubpi->phy_rev) ||
	    ACMAJORREV_2(pi->pubpi->phy_rev) ||
	    ACMAJORREV_5(pi->pubpi->phy_rev) ||
	    ACMAJORREV_40(pi->pubpi->phy_rev) ||
	    (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi) && PHY_ILNA(pi)))
		trt += boardloss;

	/* Clear up RfctrlIntc override (as uCode might have set it for hirssi_elnabypass */
	if (PHY_SW_HIRSSI_UCODE_CAP(pi)) {
		intc = phy_utils_read_phyreg(pi, ACPHYREGCE(pi, RfctrlIntc, core)) & (~mask);
		phy_utils_write_phyreg(pi, ACPHYREGCE(pi, RfctrlIntc, core), intc);
	}

	switch (core) {
	case 0 :
		if (ACREV_IS(pi->pubpi->phy_rev, 0)) {
			/* adjust TRLoss with rssi cal param */
			MOD_PHYREG(pi, TRLossValue, freqGainTLoss, trt);
			MOD_PHYREG(pi, TRLossValue, freqGainRLoss, trr);
		} else {
			MOD_PHYREG(pi, Core0_TRLossValue, freqGainTLoss0, trt);
			MOD_PHYREG(pi, Core0_TRLossValue, freqGainRLoss0, trr);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
			/* for 4347, the hwrssi uses TRloss from the common reg */
			if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
				MOD_PHYREG(pi, TRLossValue, freqGainTLoss, trt);
				MOD_PHYREG(pi, TRLossValue, freqGainRLoss, trr);
			}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		}
		break;
	case 1:
		if (PHYCORENUM(pi->pubpi->phy_corenum) > 1 &&
		    ACREV_GT(pi->pubpi->phy_rev, 0)) {
			/* adjust TRLoss with rssi cal param */
			MOD_PHYREG(pi, Core1_TRLossValue, freqGainTLoss1, trt);
			MOD_PHYREG(pi, Core1_TRLossValue, freqGainRLoss1, trr);
		}
		break;
	case 2:
		if (PHYCORENUM(pi->pubpi->phy_corenum) > 2 &&
		    ACREV_GT(pi->pubpi->phy_rev, 0)) {
			/* adjust TRLoss with rssi cal param */
			MOD_PHYREG(pi, Core2_TRLossValue, freqGainTLoss2, trt);
			MOD_PHYREG(pi, Core2_TRLossValue, freqGainRLoss2, trr);
		}
		break;
	case 3:
		if (PHYCORENUM(pi->pubpi->phy_corenum) > 3 &&
		    ACREV_GT(pi->pubpi->phy_rev, 0)) {
			/* adjust TRLoss with rssi cal param */
			MOD_PHYREG(pi, Core3_TRLossValue, freqGainTLoss3, trt);
			MOD_PHYREG(pi, Core3_TRLossValue, freqGainRLoss3, trr);
		}
		break;
	default:
		break;
	}
	wlc_phy_conditional_resume(pi, &suspend);
}

void
wlc_phy_set_lna1byp_reg_acphy(phy_info_t *pi, int8 core)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	int8 lna1_gain_offset = 0, core_freq_segment_map, ant;
	int8  bw_idx, subband_idx;
	bool suspend = FALSE;
	uint8 bands[NUM_CHANS_IN_CHAN_BONDING];
	bool rssi_cal_rev = phy_ac_rssi_get_data(pi_ac->rssii)->rssi_cal_rev;
	wlc_phy_conditional_suspend(pi, &suspend);

	bw_idx = (CHSPEC_IS80(pi->radio_chanspec)) ? 2 :
	        (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	core_freq_segment_map = phy_ac_chanmgr_get_data(pi_ac->chanmgri)->core_freq_mapping[core];
	ant = phy_get_rsdbbrd_corenum(pi, core);

	if (rssi_cal_rev == FALSE) {
		if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
			phy_ac_chanmgr_get_chan_freq_range_80p80(pi, pi->radio_chanspec, bands);
			subband_idx = (core <= 1) ? (bands[0] - 1) : (bands[1] - 1);
		} else {
			subband_idx = phy_ac_chanmgr_get_chan_freq_range(pi,
			  pi->radio_chanspec, core_freq_segment_map)-1;
		}
	} else {
		subband_idx = wlc_phy_rssi_get_chan_freq_range_acphy(pi, core_freq_segment_map,
			core);
	}
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
	  if (rssi_cal_rev == FALSE) {
	    lna1_gain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_ON][bw_idx];
	  } else {
	    lna1_gain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  }
	} else {
	  if (rssi_cal_rev == FALSE) {
	    lna1_gain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g[ant]
	        [ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  } else {
	    lna1_gain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_OFF][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	  }
	}
	if (rssi_cal_rev == TRUE) {
	  /* With new scheme, rssi gain delta's are in qdB steps */
	  lna1_gain_offset = (lna1_gain_offset + 2) >> 2;
	}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		int8 gain_lna1_byp;
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			gain_lna1_byp = LNA1_BYPASS_GAIN_2G_20695 - lna1_gain_offset;
		} else {
			gain_lna1_byp = LNA1_BYPASS_GAIN_5G_20695 - lna1_gain_offset;
		}
		MOD_PHYREG(pi, Core0_lna1BypVals, lna1BypGain0, gain_lna1_byp);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

	wlc_phy_conditional_resume(pi, &suspend);
}

void
wlc_phy_rxgain_adj_forrssi_acphy(phy_info_t *pi, int8 core, uint8 force_gain_type)
{
	/* REV40 force_gain_type 3 lna1 gain step offset adjustment */
	/* Assumes force_gain_type 3 uses lna1 of 3 */
	/* Only support rssi_cal_rev == TRUE */

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 ant, core_freq_segment_map, midgain_index, lna1_index;
	int8  bw_idx, subband_idx, gain_lna1, rssi_midgain_offset;
	uint16 gain_tblid;
	uint8 rssi_rev = phy_ac_rssi_get_data(pi_ac->rssii)->rssi_cal_rev;

	if (force_gain_type == 3 && (rssi_rev == TRUE)) {
		midgain_index = ACPHY_GAIN_DELTA_GainIndex_3;
		/* lna1 index 3 used for force_gain_type 3 to calibrate lna1 gain step delta */
		lna1_index = 3;
	} else {
		/* Fix me. Only adjust for force_gain_type 3 for now */
		/* Add more type as needed */
		return;
	}

	bw_idx = (CHSPEC_IS80(pi->radio_chanspec)) ? 2 :
	        (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	core_freq_segment_map = phy_ac_chanmgr_get_data(pi_ac->chanmgri)->core_freq_mapping[core];
	ant = phy_get_rsdbbrd_corenum(pi, core);

	subband_idx = wlc_phy_rssi_get_chan_freq_range_acphy(pi,
	                                                     core_freq_segment_map, core);
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
	    rssi_midgain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][midgain_index][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_2g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	} else {
	    rssi_midgain_offset =
	      pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][midgain_index][bw_idx][subband_idx]-
	        pi_ac->sromi->rssioffset.rssi_corr_gain_delta_5g_sub
	        [ant][ACPHY_GAIN_DELTA_ELNA_ON][bw_idx][subband_idx];
	}

	if (core == 0) {
			gain_tblid =  ACPHY_TBL_ID_GAIN0;
		} else if (core == 1) {
			gain_tblid =  ACPHY_TBL_ID_GAIN1;
		} else {
			gain_tblid =  ACPHY_TBL_ID_GAIN2;
		}
	wlc_phy_table_read_acphy(pi, gain_tblid, 1, (0x8 + lna1_index), 8,
	                         &gain_lna1);

	rssi_midgain_offset = (rssi_midgain_offset + 2) >> 2;
	/* Limit the range of adjustment */
	rssi_midgain_offset = MAX(MIN(rssi_midgain_offset, ACPHY_MIDGAIN_OFFSET_LIMIT),
	                          -ACPHY_MIDGAIN_OFFSET_LIMIT);

	/* Update the gaindB table for the index */
	gain_lna1 -= rssi_midgain_offset;
	wlc_phy_table_write_acphy(pi, gain_tblid, 1, (0x8 + lna1_index), 8,
	                          &gain_lna1);
}

void
wlc_phy_set_agc_gaintbls_acphy(
	phy_info_t *pi,
	uint32 gain_tblid,
	const void *gain,
	uint32 gainbits_tblid,
	const void *gainbits,
	uint32 gainlimits_tblid,
	const void *gainlimits_ofdm,
	const void *gainlimits_cck,
	uint32 offset,
	uint32 len)
{
	wlc_phy_table_write_acphy(
		pi, gain_tblid, len, offset,
		ACPHY_GAINDB_TBL_WIDTH, gain);
	wlc_phy_table_write_acphy(
		pi, gainbits_tblid, len, offset,
		ACPHY_GAINBITS_TBL_WIDTH, gainbits);
	wlc_phy_table_write_acphy(
		pi, gainlimits_tblid, len, offset,
		ACPHY_GAINLMT_TBL_WIDTH, gainlimits_ofdm);
	wlc_phy_table_write_acphy(
		pi, gainlimits_tblid, len, offset + GAINLMT_TBL_BAND_OFFSET,
		ACPHY_GAINLMT_TBL_WIDTH, gainlimits_cck);
}

void
phy_ac_get_fem_rxgains(phy_info_t *pi, int8 *rx_gains)
{
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;

	rx_gains[0] = rxgcrsi->fem_rxgains[0].elna;
	rx_gains[1] = rxgcrsi->fem_rxgains[0].trloss;
	rx_gains[2] = rxgcrsi->fem_rxgains[0].elna_bypass_tr;
}

void phy_ac_get_rxgains_ctrl(phy_info_t *pi, int8 *rx_gains, int8 *input_param)
{
	phy_ac_rxgcrs_info_t *rxgcrsi = pi->u.pi_acphy->rxgcrsi;

	rx_gains[0]  = rxgcrsi->rxgainctrl_params[0].gaintbl[1][input_param[0]];
	rx_gains[1]  = rxgcrsi->rxgainctrl_params[0].gaintbl[2][input_param[1]];
	rx_gains[2] = rxgcrsi->rxgainctrl_params[0].gaintbl[3][input_param[2]];
	rx_gains[3] = rxgcrsi->rxgainctrl_params[0].gaintbl[4][input_param[3]];
	rx_gains[4] = rxgcrsi->rxgainctrl_params[0].gaintbl[5][input_param[4]];
}

void phy_ac_subband_cust_28nm_ulp(phy_info_t *pi)
{
	int8 lna1_2g_gain[] = {-1, 4, 9, 16, 22, 28};
	int8 lna2_2g_gain[] = {-8, -8, -2, -2};
	int8 lna1_5g_gain[] = {-5, 1, 7, 14, 19, 25};
	int8 lna2_5g_gain[] = {-8, -8, -2, -2};
	uint8 tia_gain[] = {15, 18, 21, 24, 27, 30, 30, 30, 30, 33, 33, 33, 33, 33, 33, 33};
	uint8 biq_gain[] = {0, 3, 6, 9, 12, 15};

	uint8 lna1_bits[] = {0, 1, 2, 3, 4, 5, 6};
	uint8 lna2_bits[] = {1, 1, 2, 2};
	uint8 tia_bits[] = {0, 1, 2, 3, 4, 5, 5, 5, 5, 9, 9, 9, 9, 9, 9, 9};
	uint8 biq_bits[] = {0, 1, 2, 3, 4, 5};

	uint8 ext_lna_lmt[] = {0, 0};
	uint8 lna1_lmt[] = {0, 0, 0, 0, 0, 0};
	uint8 lna2_lmt[] = {0, 0, 0, 0};
	uint8 tia_lmt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	uint8 ofdm_cck_offset[] = {0, 64};
	uint8 i;

	/* [2:0] -> Gm index, [6:3] -> Rout index */
	uint8 lna1_2g_rout_lut[] = {9, 9, 9, 9, 6, 0};
	uint8 lna1_5g_rout_lut[] = {9, 9, 9, 9, 6, 0};
	uint8 lna1_2g_gm_lut[] = {2, 3, 4, 5, 5, 5};
	uint8 lna1_5g_gm_lut[] = {4, 5, 6, 7, 7, 7};
	uint8 lna2_cfg_lut[] = {0, 1, 2, 3, 4, 5, 6};
	uint8 lna1_2g_cfg_lut[] = {0, 0, 0, 0, 0, 0};
	uint8 lna1_5g_cfg_lut[] = {0, 0, 0, 0, 0, 0};

	/* Gain codes
	   [extlna, lna1, lna2, tia, biq0, biq1(na), dvga]
	*/
	uint8 initgain_code_2g[] = {0, 5, 1, 9, 3, 0, 1}; /* 65dB */
	uint8 initgain_code_5g[] = {0, 5, 1, 9, 3, 0, 2}; /* 65dB */
	uint8 clip_hi_code_2g[] = {0, 5, 1, 3, 0, 0, 0}; /* 44dB */
	uint8 clip_hi_code_5g[] = {0, 5, 1, 3, 1, 0, 0}; /* 44dB */
	uint8 clip_md_code_2g[] = {0, 3, 1, 1, 0, 0, 0}; /* 26dB */
	uint8 clip_md_code_5g[] = {0, 4, 1, 0, 0, 0, 0}; /* 26dB */
	uint8 clip_lo_code_2g[] = {0, 0, 1, 0, 0, 0, 0}; /* 6dB */
	uint8 clip_lo_code_5g[] = {0, 0, 1, 1, 0, 0, 0}; /* 6dB */
	uint8 clip_lo_lna1byp_code_2g[] = {0, 6, 1, 3, 0, 0, 0}; /* 6dB */
	uint8 clip_lo_lna1byp_code_5g[] = {0, 6, 1, 3, 0, 0, 0}; /* 6dB */
	uint8 clip2_code_2g[] = {0, 0, 1, 1, 0, 0, 0}; /* 10dB */
	uint8 clip2_code_5g[] = {0, 0, 1, 3, 0, 0, 0}; /* 10dB */

	uint8 core = 0;
	bool lna1byp;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	/* Gain dB Table */
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAIN0, 6, LNA1_OFFSET, ACPHY_GAINDB_TBL_WIDTH,
		(CHSPEC_IS2G(pi->radio_chanspec) ? lna1_2g_gain : lna1_5g_gain));
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAIN0, 4, LNA2_OFFSET, ACPHY_GAINDB_TBL_WIDTH,
		(CHSPEC_IS2G(pi->radio_chanspec) ? lna2_2g_gain : lna2_5g_gain));
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAIN0, 16, TIA_OFFSET,
		ACPHY_GAINDB_TBL_WIDTH, tia_gain);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAIN0, 6, BIQ0_OFFSET,
		ACPHY_GAINDB_TBL_WIDTH, biq_gain);

	/* Gain bits Table */
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINBITS0, 7, LNA1_OFFSET,
		ACPHY_GAINBITS_TBL_WIDTH, lna1_bits);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINBITS0, 4, LNA2_OFFSET,
		ACPHY_GAINBITS_TBL_WIDTH, lna2_bits);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINBITS0, 16, TIA_OFFSET,
		ACPHY_GAINBITS_TBL_WIDTH, tia_bits);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINBITS0, 6, BIQ0_OFFSET,
		ACPHY_GAINBITS_TBL_WIDTH, biq_bits);

	/* Gain limit Table for OFDM and CCK
	   Offsets : 0 to 63 are for OFDM gain limit
	   Offsets : 64 to end are for CCK gain limit
	*/
	for (i = 0; i < 2; i++) {
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, 2,
				(EXT_LNA_OFFSET + ofdm_cck_offset[i]),
				ACPHY_GAINLMT_TBL_WIDTH, ext_lna_lmt);
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, 6,
				(LNA1_OFFSET + ofdm_cck_offset[i]),
				ACPHY_GAINLMT_TBL_WIDTH, lna1_lmt);
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, 4,
				(LNA2_OFFSET + ofdm_cck_offset[i]),
				ACPHY_GAINLMT_TBL_WIDTH, lna2_lmt);
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_GAINLIMIT0, 16,
				(TIA_OFFSET + ofdm_cck_offset[i]),
				ACPHY_GAINLMT_TBL_WIDTH, tia_lmt);
	}

	/* LNARout Table
	  [2:0] -> gm ind
	  [6:3] -> rout ind
	*/
	for (i = 0; i < ARRAYSIZE(lna1_2g_cfg_lut); i++) {
		lna1_2g_cfg_lut[i] = (lna1_2g_rout_lut[i] << 3) | lna1_2g_gm_lut[i];
		lna1_5g_cfg_lut[i] = (lna1_5g_rout_lut[i] << 3) | lna1_5g_gm_lut[i];
	}

	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 6, 0, 8, lna1_2g_cfg_lut);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 6, 8, 8, lna1_5g_cfg_lut);
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 7, 16, 8, lna2_cfg_lut);

	/* Gain codes (init, clip high, clip mid, clip low, clip2) */
	wlc_phy_set_analog_rxgain(pi, INIT_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
			initgain_code_2g : initgain_code_5g), 0, core);
	wlc_phy_set_analog_rxgain(pi, HIGH_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
			clip_hi_code_2g : clip_hi_code_5g), 0, core);
	wlc_phy_set_analog_rxgain(pi, MID_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
			clip_md_code_2g : clip_md_code_5g), 0, core);
	wlc_phy_set_analog_rxgain(pi, CLIP2_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
			clip2_code_2g : clip2_code_5g), 0, core);

	/* LNA1 byapss for clip LOW gain */
	lna1byp = CHSPEC_IS2G(pi->radio_chanspec) ?
			((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
			& BFL2_LNA1BYPFORTR2G) != 0) :
			((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
			& BFL2_LNA1BYPFORTR5G) != 0);

	if (lna1byp) {
		phy_ac_upd_lna1_bypass(pi, core);
		pi_ac->rxgcrsi->fem_rxgains[core].lna1byp = lna1byp;

		wlc_phy_set_analog_rxgain(pi, LOW_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
				clip_lo_lna1byp_code_2g : clip_lo_lna1byp_code_5g), 0, core);
	} else {
		wlc_phy_set_analog_rxgain(pi, LOW_GAIN, (CHSPEC_IS2G(pi->radio_chanspec) ?
				clip_lo_code_2g : clip_lo_code_5g), 0, core);
	}

	MOD_PHYREG(pi, clip_detect_normpwr_var_mux, mClpAgcEn, 0);
}

/* Additional settings to use LNA1 bypass instead of T/R */
static void
phy_ac_upd_lna1_bypass(phy_info_t *pi, uint8 core)
{
	uint8 rout_2g = 0;
	int8  gain_2g = 0;
	uint8 rout_5g = 0;
	int8  gain_5g = 0;
	uint8 idx = 6;	/* Rout table index to be used when in bypass */
	uint8 rout;
	int8 gain;
	uint16 byp_val;
	uint8 rout_val;
	uint32 rout_offset;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	if (IS_28NM_RADIO(pi)) {
		/* 43012 bypass settings */
		gain_2g = rxg_params->lna1_byp_gain_2g;
		gain_5g = rxg_params->lna1_byp_gain_5g;
		rout_2g = rxg_params->lna1_byp_rout_2g;
		rout_5g = rxg_params->lna1_byp_rout_5g;
		idx = rxg_params->lna1_byp_indx;
	}

	/* Make sure the AGC knows the gain when LNA1 is in bypass */
	gain = CHSPEC_IS2G(pi->radio_chanspec) ? gain_2g: gain_5g;
	byp_val = (0xff & gain) << ACPHY_Core0_lna1BypVals_lna1BypGain0_SHIFT(pi->pubpi->phy_rev);

	/* Gain/Rout table index to be used when in bypass */
	byp_val |= idx << ACPHY_Core0_lna1BypVals_lna1BypIndex0_SHIFT(pi->pubpi->phy_rev);

	/* Have the AGC use the gain set in lna1BypGain */
	byp_val |= 1 << ACPHY_Core0_lna1BypVals_lna1BypEn0_SHIFT(pi->pubpi->phy_rev);

	WRITE_PHYREGC(pi, _lna1BypVals, core, byp_val);

	/* Set the Rout to be used for index 6 */
	rout = CHSPEC_IS2G(pi->radio_chanspec) ? rout_2g: rout_5g;
	rout_val = (rout << 3) + idx;
	if (IS_28NM_RADIO(pi)) {
		rout_offset = (core * 24) + idx + (CHSPEC_IS2G(pi->radio_chanspec) ? 0 : 8);
	} else {
		rout_offset = (core * 24) + idx + (CHSPEC_IS2G(pi->radio_chanspec) ? 0 : 16);
	}
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUT, 1, rout_offset, 8, &rout_val);

	/* Power down LNA1 when bypassed */
	MOD_PHYREG(pi, AfePuCtrl, lna1_pd_during_byp, 1);

	/* Only for chips with a dedicated BT-LNA, no shared LNA */
	ASSERT(READ_PHYREGFLD(pi, SlnaControl, SlnaEn) == 0);

	/* WAR for LNA1 bypass sticky on Core 0, CRDOT11ACPHY-1161 */
	MOD_PHYREG(pi, SlnaControl, SlnaCore, 3);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		wlc_phy_set_lna1byp_reg_acphy(pi, core);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
}

static void
phy_ac_single_shot_agc(phy_info_t *pi, bool init, bool band_change, bool bw_change)
{
	uint8 core;
	uint8 gain_idx[ACPHY_MAX_RX_GAIN_STAGES];
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;
	int8 init_gain = rxg_params->fast_agc_clip_gains[0];

	if (init == TRUE) {
		/* SSAGC control config */
		phy_ac_ssagc_control_config(pi);
	}

	if ((init == TRUE) || (band_change == TRUE) || (bw_change == TRUE)) {
		FOREACH_CORE(pi, core) {
			phy_ac_ssagc_rssi_setup(pi, core, init, band_change, bw_change);
			phy_ac_ssagc_rxgainctrl(pi, core);

			/* Configure init gain */
			wlc_phy_rxgainctrl_encode_gain_acphy(pi, core, init_gain, 0, 0,
					INIT_GAIN, gain_idx);
			wlc_phy_set_analog_rxgain(pi, INIT_GAIN, gain_idx, 0, core);
		}
	}
}

static void
phy_ac_ssagc_rxgainctrl(phy_info_t *pi, uint8 core)
{
	uint8 max_analog_gain, i;
	bool trtx, trrx, lna1byp_en, lna1byp;
	uint32 rssi_clip_gains[SSAGC_CLIP1_GAINS_CNT];
	uint8 gain_idx[ACPHY_MAX_RX_GAIN_STAGES];
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;
	bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(pi_ac)
	                                                      : BF_ELNA_5G(pi_ac);

	/* Max analog gain configuration */
	max_analog_gain = rxg_params->max_analog_gain;

	/* fill in gain limits for analog stages */
	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {
		if (rxg_params->maxgain[i] == 0) {
			pi_ac->rxgcrsi->rxgainctrl_maxout_gains[i] = max_analog_gain;
		} else {
			pi_ac->rxgcrsi->rxgainctrl_maxout_gains[i] = rxg_params->maxgain[i];
		}
	}

	lna1byp_en = CHSPEC_IS2G(pi->radio_chanspec) ?
			((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
			& BFL2_LNA1BYPFORTR2G) != 0) :
			((BOARDFLAGS2(GENERIC_PHY_INFO(pi)->boardflags2)
			& BFL2_LNA1BYPFORTR5G) != 0);
	pi_ac->rxgcrsi->fem_rxgains[core].lna1byp = lna1byp_en;

	if (lna1byp_en == TRUE) {
		phy_ac_upd_lna1_bypass(pi, core);
	}

	/* Auto distribute gain */
	for (i = 0; i < SSAGC_CLIP1_GAINS_CNT; i++) {
		lna1byp = FALSE; trtx = FALSE; trrx = TRUE;

		if (rxg_params->ssagc_clip_gains[i] < rxg_params->ssagc_lna_byp_gain_thresh) {
			if (elna_present) {
				lna1byp = FALSE; trtx = TRUE; trrx = FALSE;
			} else if (lna1byp_en) {
				lna1byp = TRUE; trtx = FALSE; trrx = TRUE;
			}
		}

		wlc_phy_rxgainctrl_encode_gain_acphy(pi, core, rxg_params->ssagc_clip_gains[i],
				trtx, lna1byp, SSAGC_CLIP_GAIN, gain_idx);
		rssi_clip_gains[i] = phy_ac_ssgac_pack_gains(pi, gain_idx, trtx, lna1byp, trrx);
	}

	/* Write clip gain table */
	phy_ac_populate_ssagc_clipgain_tables(pi, rssi_clip_gains, core);
}

static void
phy_ac_populate_ssagc_clipgain_tables(phy_info_t *pi, uint32 *rssi_clip_gains, uint8 core)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	/* ClipGain table */
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RSSICLIPGAIN0, SSAGC_CLIP1_GAINS_CNT, 0, 32,
			rssi_clip_gains);

	/* Clip2Idx table */
	wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RSSICLIP2GAIN0,
			SSAGC_CLIP2_TBL_IDX_CNT, 0, 8, rxg_params->ssagc_clip2_tbl);
}

static uint32
phy_ac_ssgac_pack_gains(phy_info_t *pi, uint8 *gain_idx, bool trtx, bool lna1byp, bool trrx)
{
	uint8 elna = gain_idx[0];
	uint8 lna1 = gain_idx[1];
	uint8 lna2 = gain_idx[2];
	uint8 tia = gain_idx[3];
	uint8 bq0 = gain_idx[4];
	uint8 bq1 = gain_idx[5];
	uint8 dvga = gain_idx[6];

	uint32 pack_gain_idx = (lna1byp << 23) | (trrx << 22) | (dvga << 18) | (bq1 << 15) |
			(bq0 << 12) | (tia << 8) | (lna2 << 5) | (lna1 << 2) | (elna << 1) | trtx;
	return pack_gain_idx;
}

static void
phy_ac_ssagc_control_config(phy_info_t *pi)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		/* Set clip gain defer time to 1 us */
		ACPHY_REG_LIST_START
			MOD_PHYREG_ENTRY(pi, defer_setClip1_CtrLen, defer_setclip1gain_len, 56)
			MOD_PHYREG_ENTRY(pi, defer_setClip2_CtrLen, defer_setclip2gain_len, 40)
			MOD_PHYREG_ENTRY(pi, defer_carrier_search_ctr,
					defer_carrier_search_ctr, 112)

			/* Enable pre-clip */
			MOD_PHYREG_ENTRY(pi, ADC_PreClip_Enable, adc_preclip1_enable, 1)
			MOD_PHYREG_ENTRY(pi, ADC_PreClip_Enable, adc_preclip2_enable, 0)

			/* Configure pre-clip thresholds */
			/* Default threshold / 4 */
			MOD_PHYREG_ENTRY(pi, Core0PreClip1Threshold, PreClip1Threshold, 2062)
			MOD_PHYREG_ENTRY(pi, Core0PreClip2Threshold, PreClip2Threshold, 2596)

			/* Configure pre-clip counters */
			MOD_PHYREG_ENTRY(pi, ADC_PreClip1_CtrLen, adc_preclip1_len, 36)
			MOD_PHYREG_ENTRY(pi, ADC_PreClip2_CtrLen, adc_preclip2_len, 16)

			/* Disable clip2 */
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl1, singleShotAgcClip2En, 0)
			MOD_PHYREG_ENTRY(pi, Core0computeGainInfo, disableClip2detect, 1)
			WRITE_PHYREG_ENTRY(pi, Core0Clip2Threshold, 0xffff)

			/* Configure SSAGC control */
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, singleShotAgcEn, 1)
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, singleShotClipTblOffset, 0)
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, singleShotPktGainElement, 0x40)

			/*
			   2 : full packet gain is applied if clip didn't happen, otherwise only
			   the selected elements(based on singleShotPktGainElement) are changed
			   1 : full packet gain is applied like in the fast agc mode.
			   0 : full packet gain if clip didn't happen otherwise it is ignored
			*/
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, singleShotPktGainMode, 2)

			/* Fix for reducing Badplcp */
			MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, disable_SingleShotAgcEn_finestr, 1)
		ACPHY_REG_LIST_EXECUTE(pi);

		if (ACMINORREV_0(pi)) {
			/* Fix for reducing Badplcp */
			MOD_PHYREG(pi, fineRxclockgatecontrol, forcerxbe1x1gatedClkson, 1);
		} else {
			uint16 spare_reg;

			/* Spare reg to enable Badplcp fix : SpareReg(15) */
			spare_reg = READ_PHYREG(pi, SpareReg) | (1 << 15);
			WRITE_PHYREG(pi, SpareReg, spare_reg);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
}

static void
phy_ac_ssagc_rssi_setup(phy_info_t *pi, uint8 core, bool init, bool band_change, bool bw_change)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	if (init == TRUE) {
		/* Configure the RSSI detector analog thrshold */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			phy_ac_set_ssagc_RSSI_detectors(pi, core,
					rxg_params->ssagc_rssi_thresh_2g);
		} else {
			phy_ac_set_ssagc_RSSI_detectors(pi, core,
					rxg_params->ssagc_rssi_thresh_5g);
		}
	}

	if (bw_change == TRUE) {
		/* Configure hi and low digital thresholds */
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			phy_ac_set_ssagc_clipcnt_thresh(pi, core,
					rxg_params->ssagc_clipcnt_thresh_2g);
		} else {
			phy_ac_set_ssagc_clipcnt_thresh(pi, core,
					rxg_params->ssagc_clipcnt_thresh_5g);
		}
	}
}

static void
phy_ac_set_ssagc_RSSI_detectors(phy_info_t *pi, uint8 core, uint8 *thresholds)
{
	uint8 w1 = thresholds[0];
	uint8 w3_low = thresholds[1];
	uint8 w3_mid = thresholds[2];
	uint8 w3_hi = thresholds[3];
	uint8 nb_low = thresholds[4];
	uint8 nb_mid = thresholds[5];
	uint8 nb_hi = thresholds[6];

	/* 7271 should use phy_ac_set_20696_RSSI_detectors below */
	ASSERT(!RADIOID_IS(pi->pubpi->radioid, BCM20696_ID));

	/* Configure W1 threshold */
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			MOD_RADIO_REG_20694(pi, RF, LNA2G_REG3, core,
					lna2g_dig_wrssi1_threshold, w1);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		{
			MOD_RADIO_REG_28NM(pi, RF, LNA2G_REG3, core, rx2g_wrssi1_threshold, w1);
		}
	} else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			MOD_RADIO_REG_20694(pi, RF, RX5G_WRSSI1, core, rx5g_wrssi_threshold, w1);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		{
			MOD_RADIO_REG_28NM(pi, RF, RX5G_WRSSI1, core, rx5g_wrssi_threshold, w1);
		}
	}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold2, w3_hi);
		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold1, w3_mid);
		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold0, w3_low);

		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold2, nb_hi);
		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold1, nb_mid);
		MOD_RADIO_REG_20694(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold0, nb_low);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
	{
		/* Configure W3 thresholds */
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold2, w3_hi);
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold1, w3_mid);
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold0, w3_low);

		/* Configure NB thresholds */
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold2, nb_hi);
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold1, nb_mid);
		MOD_RADIO_REG_28NM(pi, RF, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold0, nb_low);
	}
}

/* Same functionality as phy_ac_set_ssagc_RSSI_detectors above but for 7271 */
static void
phy_ac_set_20696_RSSI_detectors(phy_info_t *pi, uint8 core, uint8 *thresholds)
{
	uint8 w1 = thresholds[0];
	uint8 w3_low = thresholds[1];
	uint8 w3_mid = thresholds[2];
	uint8 w3_hi = thresholds[3];
	uint8 nb_low = thresholds[4];
	uint8 nb_mid = thresholds[5];
	uint8 nb_hi = thresholds[6];

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20696_ID));

	/* Configure W1 threshold */
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		MOD_RADIO_REG_20696(pi, LNA2G_REG3, core, lna2g_dig_wrssi1_threshold, w1);
	} else {
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI1, core, rx5g_wrssi_threshold, w1);
	}

	/* Configure W3 thresholds */
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold_high, w3_hi);
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold_mid, w3_mid);
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG1, core, tia_nbrssi1_threshold_low, w3_low);

	/* Configure NB thresholds */
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold_high, nb_hi);
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold_mid, nb_mid);
	MOD_RADIO_REG_20696(pi, TIA_NBRSSI_REG2, core, tia_nbrssi2_threshold_low, nb_low);
}

static void
phy_ac_set_ssagc_clipcnt_thresh(phy_info_t *pi, uint8 core, uint8 *thresholds)
{
	MOD_PHYREGC(pi, SsAgcW1ClipCntTh, core, ssAgcW1ClipCntThLo, thresholds[0]);
	MOD_PHYREGC(pi, SsAgcW1ClipCntTh, core, ssAgcW1ClipCntThHi, thresholds[1]);
	MOD_PHYREGC(pi, SsAgcW2ClipCntTh, core, ssAgcW2ClipCntThLo, thresholds[2]);
	MOD_PHYREGC(pi, SsAgcW2ClipCntTh, core, ssAgcW2ClipCntThHi, thresholds[3]);
	MOD_PHYREGC(pi, SsAgcNbClipCntTh1, core, ssAgcNbClipCntThLo, thresholds[4]);
	MOD_PHYREGC(pi, SsAgcNbClipCntTh1, core, ssAgcNbClipCntThHi, thresholds[5]);
}

static void
phy_ac_fastagc_config(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	/* Disable SSAGC flag */
	rxg_params->ssagc_en = FALSE;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		wlc_phy_rxgainctrl_gainctrl_acphy_28nm_ulp(pi);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

	/* Configure registers related to Fast AGC */
	ACPHY_REG_LIST_START
		MOD_PHYREG_ENTRY(pi, defer_setClip1_CtrLen, defer_setclip1gain_len, 20)
		MOD_PHYREG_ENTRY(pi, defer_setClip2_CtrLen, defer_setclip2gain_len, 16)
		MOD_PHYREG_ENTRY(pi, defer_carrier_search_ctr,
				defer_carrier_search_ctr, 112)

		/* Disable pre-clip */
		MOD_PHYREG_ENTRY(pi, ADC_PreClip_Enable, adc_preclip1_enable, 0)
		MOD_PHYREG_ENTRY(pi, ADC_PreClip_Enable, adc_preclip2_enable, 0)

		/* Disable SSAGC */
		MOD_PHYREG_ENTRY(pi, singleShotAgcCtrl, singleShotAgcEn, 0)
	ACPHY_REG_LIST_EXECUTE(pi);
}

static void
phy_ac_ssagc_config(phy_info_t *pi, bool init, bool band_change, bool bw_change)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	/* Enable SSAGC flag */
	rxg_params->ssagc_en = TRUE;

	phy_ac_single_shot_agc(pi, init, band_change, bw_change);
}

void
phy_ac_agc_config(phy_info_t *pi, uint8 agc_type)
{
	if (agc_type == FAST_AGC) {
		phy_ac_fastagc_config(pi);
	} else if (agc_type == SINGLE_SHOT_AGC) {
		phy_ac_ssagc_config(pi, TRUE, TRUE, TRUE);
	} else {
		PHY_ERROR(("%s: Invalid AGC type %d failed\n", __FUNCTION__, agc_type));
		ASSERT(0);
	}
}

void
chanspec_setup_rxgcrs(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;
#ifndef WLC_DISABLE_ACI
	uint16 orig_reg_vals;
	uint8 stall_val = 0;
	aci_reg_list_entry *reg_list_ptr;
	aci_tbl_list_entry *tbl_list_ptr;
	uint8 p_phytbl7_8_buf[119];
	uint32 p_phytbl24_28_buf[22];
#endif
	int8 desense_state;
	int8 desense_state_init;

	/* JIRA(CRDOT11ACPHY-143) - Turn off receiver during channel change */
	pi_ac->rxgcrsi->deaf_count = 0;
	phy_ac_rxgcrs_stay_in_carriersearch(pi_ac->rxgcrsi, TRUE);

	bzero((uint8 *)pi_ac->rxgcrsi->phy_noise_in_crs_min,
	      sizeof(pi_ac->rxgcrsi->phy_noise_in_crs_min));
	bzero((uint8 *)pi_ac->rxgcrsi->phy_noise_pwr_array,
	      sizeof(pi_ac->rxgcrsi->phy_noise_pwr_array));

	/* Debug parameters: printed by 'wl dump phycal' */
	pi_ac->rxgcrsi->phy_debug_crscal_counter = 0;
	pi_ac->rxgcrsi->phy_noise_counter = 0;

	/* set the crsmin_th from cache at chan_change */
	wlc_phy_crs_min_pwr_cal_acphy(pi, PHY_CRS_SET_FROM_CACHE);

	if (ACPHY_HWACI_HWTBL_MITIGATION(pi)) {
		desense_state_init = 1;
	} else {
		desense_state_init = 0;
	}

	/* Rx gainctrl (if not QT) */
	if (ISSIM_ENAB(pi->sh->sih)) {
		if (!ACMAJORREV_36(pi->pubpi->phy_rev)) {
			return;
		}
	}
	/*
	 For initial bringup need to call subband cust
	 Flag is read from 'subband_cust' NVRAM param
	*/
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		if (pi_ac->rxgcrsi->enable_subband_cust == 1) {
			phy_ac_subband_cust_28nm_ulp(pi);
			return;
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

#ifndef WLC_DISABLE_ACI
	/* Merge ACI & BT params into one */
	if (!ACPHY_ENABLE_FCBS_HWACI(pi) || ACPHY_HWACI_WITH_DESENSE_ENG(pi)) {
		wlc_phy_desense_calc_total_acphy(pi_ac->rxgcrsi);
	}
#endif /* !WLC_DISABLE_ACI */

	wlc_phy_rxgainctrl_set_gaintbls_acphy(pi,
		CCT_INIT(pi_ac), CCT_BAND_CHG(pi_ac), CCT_BW_CHG(pi_ac));

#ifndef WLC_DISABLE_ACI
	if (ACPHY_ENABLE_FCBS_HWACI(pi) && (CCT_INIT(pi_ac) || CCT_BAND_CHG(pi_ac)))
		wlc_phy_hwaci_mitigation_enable_acphy(pi,
			((pi->sh->interference_mode & ACPHY_HWACI_MITIGATION) > 0)
#ifdef WL_ACI_FCBS
			? HWACI_AUTO_FCBS : 0,
#else
			? HWACI_AUTO_SW : 0,
#endif
			TRUE);
#endif /* WLC_DISABLE_ACI */
	for (desense_state = desense_state_init; desense_state >= 0; desense_state--) {
		if (ACPHY_HWACI_HWTBL_MITIGATION(pi)) {
			phy_ac_noise_hwaci_mitigation(pi->u.pi_acphy->noisei, desense_state);
		}

		if (TINY_RADIO(pi)) {
			wlc_phy_rxgainctrl_gainctrl_acphy_tiny(pi, 0);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
			if (ACMAJORREV_3(pi->pubpi->phy_rev) && ACREV_GE(pi->pubpi->phy_rev, 11))
				wlc_phy_enable_lna_dcc_comp_20691(pi, 0);
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			if (rxg_params->ssagc_en || (PHY_NAP_ENAB(pi->sh->physhim) &&
					phy_ac_nap_is_enabled(pi_ac->napi))) {
				/* Enable SSAGC when Nap is enabled or SSAGC is enabled */
				rxg_params->ssagc_en = TRUE;

				/* Single Shot AGC */
				phy_ac_single_shot_agc(pi, CCT_INIT(pi_ac), CCT_BAND_CHG(pi_ac),
						CCT_BW_CHG(pi_ac));
			} else {
				/* Fast AGC */
				wlc_phy_rxgainctrl_gainctrl_acphy_28nm_ulp(pi);
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
		if (IS_28NM_RADIO(pi)) {
			if (rxg_params->mclip_agc_en) {
				wlc_phy_mclip_agc(pi, CCT_INIT(pi_ac), CCT_BAND_CHG(pi_ac),
					CCT_BW_CHG(pi_ac));
			} else {
				PHY_ERROR(("%s: Fast AGC needed for 28nm Radios \n", __FUNCTION__));
				return;
			}
		} else {
			/* Set INIT, Clip gains, clip thresh (srom based) */
			wlc_phy_rxgainctrl_gainctrl_acphy(pi);
		}

#ifndef WLC_DISABLE_ACI
		if (!ACPHY_ENABLE_FCBS_HWACI(pi) || ACPHY_HWACI_WITH_DESENSE_ENG(pi)) {
			/*
			 * Desense on top of default gainctrl, if desense on
			 * (otherwise restore defaults)
			 */
			// wlc_phy_desense_apply_acphy(pi, pi_ac->rxgcrsi->total_desense->on);
			wlc_phy_desense_apply_acphy(pi, TRUE);
		}
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev) ||
		    ACMAJORREV_37(pi->pubpi->phy_rev)) {
			PHY_ACI(("%s: Check if we have to initialize the HW-ACI shadow\n",
			         __FUNCTION__));
			if (ACPHY_HWACI_HWTBL_MITIGATION(pi) && desense_state == 1) {
				PHY_ACI(("Copy regs + gain table to ACI shadow gain tables\n"));
				stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
				wlapi_suspend_mac_and_wait(pi->sh->physhim);
				ACPHY_DISABLE_STALL(pi);

				for (reg_list_ptr =
					phy_ac_noise_get_data(pi_ac->noisei)->hwaci_phyreg_list;
					reg_list_ptr->regaddr != 0xFFFF; reg_list_ptr++) {
					orig_reg_vals =
						phy_utils_read_phyreg(pi, reg_list_ptr->regaddr);
					phy_utils_write_phyreg(pi, reg_list_ptr->regaddraci,
						orig_reg_vals);
				}

				for (tbl_list_ptr =
					phy_ac_noise_get_data(pi_ac->noisei)->hwaci_phytbl_list;
					tbl_list_ptr->tblidaci != 0xFFFF; tbl_list_ptr++) {
					switch (tbl_list_ptr->tbl_width) {
						case 7:
						case 8:
							wlc_phy_table_read_acphy(pi,
								tbl_list_ptr->tblid,
								tbl_list_ptr->tbl_len,
								tbl_list_ptr->start_offset,
								8, p_phytbl7_8_buf);
							wlc_phy_table_write_acphy(pi,
								tbl_list_ptr->tblidaci,
								tbl_list_ptr->tbl_len,
								tbl_list_ptr->start_offset,
								8, p_phytbl7_8_buf);
							break;
						case 24:
						case 28:
							wlc_phy_table_read_acphy(pi,
								tbl_list_ptr->tblid,
								tbl_list_ptr->tbl_len,
								tbl_list_ptr->start_offset,
								32, p_phytbl24_28_buf);
							wlc_phy_table_write_acphy(pi,
								tbl_list_ptr->tblidaci,
								tbl_list_ptr->tbl_len,
								tbl_list_ptr->start_offset,
								32, p_phytbl24_28_buf);
							break;
					}
				}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
				if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
					uint8 i, core, offset;
					// LNARout settings for ACI Mitigation
					// 2G Backoff 12dB
					const uint8 lna1_rout_map_2g_maj37_aci[N_LNA12_GAINS] = {  9,  9,  9,  9,  8, 10};
					const uint8 lna1_gain_map_2g_maj37_aci[N_LNA12_GAINS] = {  0,  1,  2,  3,  4,  5};
					// 5G Backoff 12dB
					const uint8 lna1_rout_map_5g_maj37_aci[N_LNA12_GAINS] = { 11, 11, 11, 11, 11, 11};
					const uint8 lna1_gain_map_5g_maj37_aci[N_LNA12_GAINS] = {  2,  3,  4,  5,  6,  7};

					// Setup registers to reduce PER humps when ACI present
					FOREACH_CORE(pi, core) {
						// Values found from experimentation: see Notebook 7271B0_058, _061 etc.
						if (CHSPEC_IS5G(pi->radio_chanspec)) {
							WRITE_PHYREGC(pi, Clip1Threshold, core, 0x1E00);
							WRITE_PHYREGC(pi, Clip2Threshold, core, 0x2018);
						} else {
							WRITE_PHYREGC(pi, Clip1Threshold, core, 0x1C00);
							WRITE_PHYREGC(pi, Clip2Threshold, core, 0x2018);
						}
						WRITE_PHYREGC(pi, smallsigThreshold, core, 0x0010);
					}

					FOREACH_CORE(pi, core) {
					    // 2G
					    for (i = 0; i < N_LNA12_GAINS; i++) {
					        p_phytbl7_8_buf[i] = (lna1_rout_map_2g_maj37_aci[i] << 3) |
					                lna1_gain_map_2g_maj37_aci[i];
					    }
					    /* 2G index is 0->5 for core0 */
					    offset = 0 + ACPHY_LNAROUT_CORE_WRT_OFST(pi, core);
					    wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUTLUTACI,
					            N_LNA12_GAINS, offset, 8, p_phytbl7_8_buf);
					}
					FOREACH_CORE(pi, core) {
					    // 5G
					    for (i = 0; i < N_LNA12_GAINS; i++) {
					        p_phytbl7_8_buf[i] = (lna1_rout_map_5g_maj37_aci[i] << 3) |
					                lna1_gain_map_5g_maj37_aci[i];
					    }
					    /* 5G index is 8->13 for core0 */
					    offset = 8 + ACPHY_LNAROUT_CORE_WRT_OFST(pi, core);
					    wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_LNAROUTLUTACI,
					            N_LNA12_GAINS, offset, 8, p_phytbl7_8_buf);
					}
				}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
				ACPHY_ENABLE_STALL(pi, stall_val);
				wlapi_enable_mac(pi->sh->physhim);
			} else {
				PHY_ACI(("Do NOT copy to HW-ACI shadow\n"));
			}
		}
#endif /* !WLC_DISABLE_ACI */
	}
	if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
		/* force a reset2rx seq to let gain changes take effects */
		wlc_phy_force_rfseq_acphy(pi, ACPHY_RFSEQ_RESET2RX);
	}

}

void
wlc_phy_rxgainctrl_set_gaintbls_acphy_28nm(phy_info_t *pi,
	uint8 core, uint16 gain_tblid, uint16 gainbits_tblid)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	//acphy_desense_values_t *desense = pi_ac->rxgcrsi->total_desense;
	phy_ac_rxg_params_t *rxg_params = pi_ac->rxgcrsi->rxg_params;

	/* TIA */
	/* 2g & 5g settings */
	uint8 tia[16];
	uint8 tiabits[16];
	uint8 tialimit[16];
	uint8 i;
	//bool elna_present = CHSPEC_IS2G(pi->radio_chanspec) ? BF_ELNA_2G(pi_ac)
	//                                                    : BF_ELNA_5G(pi_ac);
	uint8 elna_gainbits[] = {0, 0};
	uint8 elna_gainlimits[] = {0, 0};
	uint8 lna1_byp_bit = 6;

	uint16 gainlimit_tblid;
	if (core == 0) {
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT0;
	} else if (core == 1) {
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT1;
	} else if (core == 2) {
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT2;
	} else if (core == 3) {
		gainlimit_tblid = ACPHY_TBL_ID_GAINLIMIT3;
	} else {
		ASSERT(0);
		gainlimit_tblid = -1;
	}
	/* ELNA setting */
	wlc_phy_table_write_acphy(pi, gainbits_tblid, pi_ac->rxgcrsi->rxgainctrl_stage_len[ELNA_ID],
		EXT_LNA_OFFSET, ACPHY_GAINDB_TBL_WIDTH, elna_gainbits);
	wlc_phy_table_write_acphy(pi, gainlimit_tblid,
		pi_ac->rxgcrsi->rxgainctrl_stage_len[ELNA_ID],
		EXT_LNA_OFFSET, ACPHY_GAINDB_TBL_WIDTH, elna_gainlimits);

	/* LNA1 settting already taken care of by upd_lna1_lna2_gains_acphy */

	/* Put LNA1 bypass index in gainbits */
	wlc_phy_table_write_acphy(pi, gainbits_tblid, 1,
		(LNA1_OFFSET + 6), ACPHY_GAINBITS_TBL_WIDTH, &lna1_byp_bit);
	/* LNA1 bypass related config */
	//if (pi_ac->rxgcrsi->fem_rxgains[core].lna1byp) {
	phy_ac_upd_lna1_bypass(pi, core);
	//}
	/* TIA setting */
	memcpy(tia, rxg_params->tia_gain_tbl, sizeof(int8) * N_TIA_GAINS);
	memcpy(tiabits, rxg_params->tia_gainbits_tbl, sizeof(int8) * N_TIA_GAINS);
	memcpy(tialimit, rxg_params->gainlimit_tbl[TIA_TBL_IND], sizeof(int8) * N_TIA_GAINS);
	for (i = 0; i < 4; i++) {
		tia[N_TIA_GAINS+i] = rxg_params->tia_gain_tbl[N_TIA_GAINS-1];
		tiabits[N_TIA_GAINS+i] = rxg_params->tia_gainbits_tbl[N_TIA_GAINS-1];
		tialimit[N_TIA_GAINS+i] = rxg_params->gainlimit_tbl[TIA_TBL_IND][N_TIA_GAINS-1];
	}

	/* program tia table */
	wlc_phy_table_write_acphy(pi, gain_tblid, 16, TIA_OFFSET, ACPHY_GAINDB_TBL_WIDTH, tia);

	wlc_phy_table_write_acphy(pi, gainbits_tblid, 16, TIA_OFFSET, ACPHY_GAINBITS_TBL_WIDTH,
		tiabits);

	wlc_phy_table_write_acphy(pi, gainlimit_tblid, 16, TIA_OFFSET, ACPHY_GAINLMT_TBL_WIDTH,
		tialimit);
	wlc_phy_table_write_acphy(pi, gainlimit_tblid, 16, 64+TIA_OFFSET, ACPHY_GAINLMT_TBL_WIDTH,
		tialimit);

	/* copying values into gaintbl arrays to avoid reading from table */
	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[TIA_ID], tia,
		sizeof(pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[0][0])
		* N_TIA_GAINS);

	memcpy(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[TIA_ID], tiabits,
		sizeof(pi_ac->rxgcrsi->rxgainctrl_params[core].gainbitstbl[0][0])
		* N_TIA_GAINS);

	/* BIQ0 setting */
	wlc_phy_table_write_acphy(pi, gain_tblid, N_BIQ01_GAINS, BIQ0_OFFSET,
		ACPHY_GAINDB_TBL_WIDTH, rxg_params->biq01_gain_tbl[0]);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, N_BIQ01_GAINS, BIQ0_OFFSET,
		ACPHY_GAINBITS_TBL_WIDTH, rxg_params->biq01_gainbits_tbl[0]);
	wlc_phy_table_write_acphy(pi, gainlimit_tblid, N_BIQ01_GAINS, 48,
		ACPHY_GAINLMT_TBL_WIDTH, rxg_params->gainlimit_tbl[BIQ0_TBL_IND]);
	wlc_phy_table_write_acphy(pi, gainlimit_tblid, N_BIQ01_GAINS, 64+48,
		ACPHY_GAINLMT_TBL_WIDTH, rxg_params->gainlimit_tbl[BIQ0_TBL_IND]);


	/* BIQ1 setting */
	wlc_phy_table_write_acphy(pi, gain_tblid, 3, DVGA1_OFFSET, ACPHY_GAINDB_TBL_WIDTH,
		rxg_params->biq01_gain_tbl[1]);
	wlc_phy_table_write_acphy(pi, gainbits_tblid, 3, DVGA1_OFFSET, ACPHY_GAINBITS_TBL_WIDTH,
		rxg_params->biq01_gainbits_tbl[1]);

}

static void wlc_phy_mclip_agc(phy_info_t *pi, bool init, bool band_change, bool bw_change)
{
#ifndef BOARD_FLAGS
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
#endif
	phy_ac_rxgcrs_info_t *info = pi->u.pi_acphy->rxgcrsi;
	uint8 core, i;
	int8 mclip_rssi[PHY_CORE_MAX][10];
	uint8 init_gain;
	bool trtx;
	bool elna_present = (CHSPEC_IS2G(pi->radio_chanspec)) ? BF_ELNA_2G(pi_ac)
	                                                    : BF_ELNA_5G(pi_ac);
	info->curr_desense->elna_bypass = wlc_phy_get_max_lna_index_acphy(pi, ELNA_ID);

	if (init) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25))
		if (ACMAJORREV_25(pi->pubpi->phy_rev)) {
			FOREACH_CORE(pi, core) {
				/* disable clip2 when mclip agc enabled for 4347TC2 */
				MOD_PHYREGC(pi, computeGainInfo, core, disableClip2detect, 1);
				WRITE_PHYREGC(pi, Clip2Threshold, core, 0xffff);
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25)) */

		if (!ACMAJORREV_37(pi->pubpi->phy_rev)) {
			FOREACH_CORE(pi, core) {
				/* set proper clip detector mux connection */
				WRITE_PHYREGC(pi, _RSSIMuxSel2, core,  0x06db);
			}
		}

		// Enable Mclip
		MOD_PHYREG(pi, clip_detect_normpwr_var_mux, mClpAgcSdwTblEn, 1);
		MOD_PHYREG(pi, clip_detect_normpwr_var_mux, mClpAgcEn, 1);

		// 7271, 4347 and 4357
		if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
			// Disable Clip3 for mclip AGC as it was not decreasing
			// the gain in earlier PHY's, see CRDOT11ACPHY-2670
			MOD_PHYREG(pi, ADC_PreClip_Enable, small_sig_gain_indx_decr, 0);
		}
	}

	if (init || band_change || bw_change) {
		/* program init gain */
		FOREACH_CORE(pi, core) {
			for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES-1; i++)
				if (!ACMAJORREV_40(pi->pubpi->phy_rev)) {
					info->rxgainctrl_maxout_gains[i] = 67;
				} else {
				/* For 4347 maxout_gain is kept at 62dB, like Tcl.
				* Higher than this would lead to a difference from
				* the analog-portion of pktgain (62 dB max)
				* This leads to performance degradation with
				* LESI-5G enabled. This setting is also valid
				* for LESI-5G disabled
				* RB: http://wlan-rb.sj.broadcom.com/r/94904
				*/
				info->rxgainctrl_maxout_gains[i] = 62;
				}
			info->rxgainctrl_maxout_gains[ACPHY_MAX_RX_GAIN_STAGES-1] = 100;

			trtx = elna_present & info->curr_desense->elna_bypass;
			init_gain = ACPHY_INIT_GAIN_28NM;
			init_gain -= info->curr_desense->clipgain_desense[0];
			for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++) {
				info->rxgainctrl_maxout_gains[i] -=
					info->curr_desense->clipgain_desense[0];
			}

			wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0, init_gain,
				trtx, 0, core);
		}
		wlc_phy_mclip_agc_rssi_setup(pi, init, band_change, bw_change, mclip_rssi);
		wlc_phy_mclip_agc_rxgainctrl(pi, mclip_rssi);
	}
}

static void wlc_phy_mclip_agc_rssi_setup(phy_info_t *pi, bool init, bool band_change,
	bool bw_change, int8 mclip_rssi[][10])
{

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 core;
	uint8 analog_threshold[7] = {8, 4, 4, 4, 2, 2, 2};
	uint8 ana_thld_2g_r40[7] = {14, 4, 4, 4, 2, 2, 2};
	const uint8 clipcnt_thresh_bw80[9] = {80, 80, 80, 120, 120, 120, 120, 120, 120};
	const uint8 clipcnt_thresh_bw40[9] = {40, 40, 40,  60,  60,  60,  60,  60,  60};
	const uint8 clipcnt_thresh_bw20[9] = {20, 20, 20,  30,  30,  30,  30,  30,  30};
	int8 adclo_ref = 0, nblo_ref = 0, w1md_ref = 0;
	uint8 elna_indx, lna1_indx, tia_indx, lpf_indx, trtx;
	int8 elna_gain, lna1_gain, tia_gain, lpf_gain, adc_gain, nb_gain, w1_gain;

	FOREACH_CORE(pi, core) {
		/* each core is programed the same threshold. can be different if necessary */
		if (init || band_change || bw_change) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
			if (ACMAJORREV_40(pi->pubpi->phy_rev) && HW_ACMINORREV(pi) == 1) {
				uint8 ana_thld_5g_40_1[7] = {10, 4, 4, 4, 2, 2, 2};
				if (CHSPEC_IS2G(pi->radio_chanspec) && (BF_ELNA_2G(pi_ac))) {
					phy_ac_set_ssagc_RSSI_detectors(pi, core, ana_thld_2g_r40);
					adclo_ref = -12;
					nblo_ref = -3;
					w1md_ref = 4;
				} else {
					phy_ac_set_ssagc_RSSI_detectors(pi, core, ana_thld_5g_40_1);
					adclo_ref = -12;
					nblo_ref = -2;
					w1md_ref = 4;
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25_40))
			if (ACMAJORREV_25_40(pi->pubpi->phy_rev)) {
				if (CHSPEC_IS2G(pi->radio_chanspec) && (BF_ELNA_2G(pi_ac))) {
					phy_ac_set_ssagc_RSSI_detectors(pi, core, ana_thld_2g_r40);
					adclo_ref = -10;
					nblo_ref = -8;
					w1md_ref = -2;
				} else {
					phy_ac_set_ssagc_RSSI_detectors(pi, core, analog_threshold);
					nblo_ref = -6;
					adclo_ref = -10;
					w1md_ref = 0;
				}
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_25_40)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID)) {
				if (CHSPEC_IS2G(pi->radio_chanspec) && (BF_ELNA_2G(pi_ac))) {
					phy_ac_set_20696_RSSI_detectors(pi, core, ana_thld_2g_r40);
					adclo_ref = -10;
					nblo_ref = -8;
					w1md_ref = -2;
				} else {
					phy_ac_set_20696_RSSI_detectors(pi, core, analog_threshold);
					adclo_ref = -10;
					nblo_ref = -6;
					w1md_ref = 0;
				}
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */
			{
			}
		}

		if (init || band_change || bw_change) {
			if (CHSPEC_IS80(pi->radio_chanspec)) {
				wlc_phy_mclip_agc_clipcnt_thresh(pi, core, clipcnt_thresh_bw80);
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				wlc_phy_mclip_agc_clipcnt_thresh(pi, core, clipcnt_thresh_bw40);
			} else {
				wlc_phy_mclip_agc_clipcnt_thresh(pi, core, clipcnt_thresh_bw20);
			}
		}
		elna_indx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initExtLnaIndex);
		lna1_indx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initLnaIndex);
		tia_indx = READ_PHYREGFLDC(pi, InitGainCodeA, core, initmixergainIndex);
		lpf_indx = READ_PHYREGFLDC(pi, InitGainCodeB, core, InitBiQ0Index);
		trtx = READ_PHYREGFLDC(pi, InitGainCodeB, core, InitHiTrTxIndex);
		if (tia_indx > 11) tia_indx = 11;

		elna_gain = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[ELNA_ID][elna_indx];
		lna1_gain = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[LNA1_ID][lna1_indx];
		tia_gain = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[TIA_ID][tia_indx];
		lpf_gain = pi_ac->rxgcrsi->rxgainctrl_params[core].gaintbl[BQ0_ID][lpf_indx];
		w1_gain = elna_gain + lna1_gain;
		w1_gain -= (trtx) ? pi_ac->rxgcrsi->fem_rxgains[core].trloss : 0;
		nb_gain = w1_gain + tia_gain;
		adc_gain = nb_gain + lpf_gain;

		if (IS_28NM_RADIO(pi)) {
			mclip_rssi[core][0] = adclo_ref - adc_gain;
			mclip_rssi[core][1] = nblo_ref - nb_gain;
			mclip_rssi[core][2] = mclip_rssi[core][1] + 6;
			mclip_rssi[core][3] = mclip_rssi[core][2] + 6;
			mclip_rssi[core][4] = mclip_rssi[core][3] + 1;
			mclip_rssi[core][5] = mclip_rssi[core][4] + 6;
			mclip_rssi[core][6] = mclip_rssi[core][5] + 6;
			mclip_rssi[core][8] = w1md_ref - w1_gain;
			mclip_rssi[core][7] = mclip_rssi[core][8] - 6;
			mclip_rssi[core][9] = mclip_rssi[core][8] + 6;
		}
	}
}


static void wlc_phy_mclip_agc_rxgainctrl(phy_info_t *pi, int8 mclip_rssi[][10])
{

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	int8 maxgain[] = {100, 100, 100, 100, 100, 100, 100};
	int8 margin_low_sens = 15, margin_high_sens = 12;
	int8 det_adjust_list[] = {24, 22, 20};
	int8 high_sens_adjust_list_2g[] = {14, 14, 14};
	int8 high_sens_adjust_list_2g_rev40[] = {10, 10, 10};
	int8 high_sens_adjust_list_5g[] = {6, 8, 9};
	int8 *high_sens_adjust_list;
	int8 high_adjust, det_adjust;
	bool elna_present = CHSPEC_IS2G(pi->radio_chanspec) ? BF_ELNA_2G(pi_ac)
	                                                    : BF_ELNA_5G(pi_ac);
	uint8 i;
	uint8 bw_indx = CHSPEC_IS20(pi->radio_chanspec) ? 0 :
			CHSPEC_IS40(pi->radio_chanspec) ? 1 : 2;

	int8 sens_hi, sens_lna1Byp_lo, sens_lna1Byp_hi, sens_elnaByp_lo = -1;
	int8 lna1Byp_rssi_idx, lna1Byp_gain_idx;
	int8 elnaByp_rssi_idx = -1, elnaByp_gain_idx;
	int8 sens_high, sens_low;
	uint8 gain_indx[7];
	uint32 mclip_gaincode[10];
	uint8 core;
	int8 rssi, rssi_low, rssi_high, clipgain, min_gain, max_gain, trtx, lna1byp;
	uint8 mag;

	mag = READ_PHYREGFLD(pi, Core0HpFBw, maxAnalogGaindb);
	maxgain[0] = maxgain[1] = maxgain[3] = maxgain[4] = mag;
	maxgain[2] = -100;

	for (i = 0; i < ACPHY_MAX_RX_GAIN_STAGES; i++)
		pi_ac->rxgcrsi->rxgainctrl_maxout_gains[i] = maxgain[i];

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		high_sens_adjust_list =
		    (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) ?
			high_sens_adjust_list_2g_rev40: high_sens_adjust_list_2g;
	} else {
		high_sens_adjust_list = high_sens_adjust_list_5g;
	}
	det_adjust = det_adjust_list[bw_indx];
	high_adjust = high_sens_adjust_list[bw_indx];

	MOD_PHYREG(pi, clip_detect_normpwr_var_mux, mClpAgcSdwTblSel, 1);

	FOREACH_CORE(pi, core) {
		sens_hi = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, 20, 0, 0, core);
		sens_lna1Byp_lo = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, 60, 0, 1, core);
		sens_lna1Byp_hi = wlc_phy_rxgainctrl_calc_high_sens_acphy(pi, 20, 0, 1, core);
		lna1Byp_rssi_idx = wlc_phy_mclip_agc_calc_lna_bypass_rssi(pi, mclip_rssi[core],
			sens_lna1Byp_lo, sens_hi);
		if (ACMAJORREV_37(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)) {
			lna1Byp_gain_idx = 100;	// Do not use LNA1 bypass
		} else {
			lna1Byp_gain_idx = lna1Byp_rssi_idx + 1;
		}
		elnaByp_gain_idx = 100;

		if (elna_present) {
			// Elna Bypass mode
			if (pi_ac->rxgcrsi->curr_desense->elna_bypass) {
				elnaByp_gain_idx = 0;
			}
			else {
				sens_elnaByp_lo = wlc_phy_rxgainctrl_calc_low_sens_acphy(pi, 60,
					1, 0, core);
				elnaByp_rssi_idx = wlc_phy_mclip_agc_calc_lna_bypass_rssi(pi,
					mclip_rssi[core], sens_elnaByp_lo, sens_lna1Byp_hi);
				elnaByp_gain_idx = elnaByp_rssi_idx + 1;
			}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
			if (ACMAJORREV_40(pi->pubpi->phy_rev))
				elnaByp_gain_idx = elnaByp_gain_idx + 2;
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		}
		PHY_INFORM(("sens_hi %d sens_lna1Byp_lo %d sens_lna1Byp_hi %d, lna1Byp_rssi_idx %d,"
			"sens_elnaByp_lo %d elnaByp_rssi_idx %d\n",
			sens_hi, sens_lna1Byp_lo, sens_lna1Byp_hi, lna1Byp_rssi_idx,
			sens_elnaByp_lo, elnaByp_rssi_idx));

		for (i = 0; i <= 10; i++) {
			if (i == 0) {
				rssi = mclip_rssi[core][0];
				sens_high = rssi + margin_high_sens;
				clipgain = high_adjust - sens_high;
				clipgain = MIN(clipgain, ACPHY_INIT_GAIN_28NM);
			} else if (i == 1) {
				rssi = mclip_rssi[core][1];
				sens_high = rssi + margin_high_sens;
				clipgain = high_adjust - sens_high;
			} else if (i == 10) {
				rssi = mclip_rssi[core][9];
				sens_low = rssi - margin_low_sens;
				clipgain = 0 - sens_low - det_adjust;
			} else {
				rssi_low = mclip_rssi[core][i-1];
				rssi_high = mclip_rssi[core][i];
				sens_low = rssi_low - margin_low_sens;
				sens_high = rssi_high + margin_high_sens;
				min_gain = 0 - sens_low - det_adjust;
				max_gain = high_adjust - sens_high;
				clipgain = (min_gain + max_gain)>>1;
			}

			if (elna_present) {
				trtx = (i >= elnaByp_gain_idx) ? 1 : 0;
				lna1byp = (i >= lna1Byp_gain_idx) && (trtx == 0);
			} else {
				trtx = 0;
				lna1byp = 0;
			}
			if (i == 0) {
				/* Program init gain */
				wlc_phy_rxgainctrl_set_init_clip_gain_acphy(pi, 0, clipgain,
					trtx, 0, core);
			} else {
				wlc_phy_rxgainctrl_encode_gain_acphy(pi, core, clipgain, trtx,
					lna1byp, 7, gain_indx);
				mclip_gaincode[i-1] = wlc_phy_mclip_agc_pack_gains(pi, gain_indx,
					trtx, lna1byp);
			}
			PHY_INFORM(("mclip_agc: clip%d code 0x%08x\n", i, mclip_gaincode[i]));
		}
		/* program clip gain */
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RSSICLIPGAIN0+core*32, 10, 1, 32,
			mclip_gaincode);
	}

	MOD_PHYREG(pi, clip_detect_normpwr_var_mux, mClpAgcSdwTblSel, 1);

}


static void wlc_phy_mclip_agc_clipcnt_thresh(phy_info_t *pi, uint8 core, const uint8 thresh_list[])
{
	/* W1 HI/MD/LW */
	MOD_PHYREGC(pi, mClpAgcW1ClipCntTh,  core, mClpAgcW1ClipCntThHi, thresh_list[0]);
	MOD_PHYREGC(pi, mClpAgcMDClipCntTh2, core, mClpAgcW1ClipCntThMd, thresh_list[1]);
	MOD_PHYREGC(pi, mClpAgcW1ClipCntTh,  core, mClpAgcW1ClipCntThLo, thresh_list[2]);
	/* W3 HI/MD/LW */
	MOD_PHYREGC(pi, mClpAgcW3ClipCntTh,  core, mClpAgcW3ClipCntThHi, thresh_list[3]);
	MOD_PHYREGC(pi, mClpAgcMDClipCntTh1, core, mClpAgcW3ClipCntThMd, thresh_list[4]);
	MOD_PHYREGC(pi, mClpAgcW3ClipCntTh,  core, mClpAgcW3ClipCntThLo, thresh_list[5]);
	/* NB HI/MD/LW */
	MOD_PHYREGC(pi, mClpAgcNbClipCntTh,  core, mClpAgcNbClipCntThHi, thresh_list[6]);
	MOD_PHYREGC(pi, mClpAgcMDClipCntTh1, core, mClpAgcNbClipCntThMd, thresh_list[7]);
	MOD_PHYREGC(pi, mClpAgcNbClipCntTh,  core, mClpAgcNbClipCntThLo, thresh_list[8]);

}

static uint8
wlc_phy_mclip_agc_calc_lna_bypass_rssi(phy_info_t *pi, int8 *mclip_rssi, int8 lo, int8 hi)
{
	int8 target_rssi, rssi_diff, min_rssi_diff = 30, min_rssi_idx = 0, i;
	target_rssi = (lo+hi)>>1;

	for (i = 0; i < 10; i++) {
		rssi_diff = ABS(mclip_rssi[i] - target_rssi);
		if (rssi_diff < min_rssi_diff) {
			min_rssi_diff = rssi_diff;
			min_rssi_idx = i;
		}
	}
	return min_rssi_idx;
}

static uint32
wlc_phy_mclip_agc_pack_gains(phy_info_t *pi, uint8 *gain_indx, uint8 trtx, uint8 lna1byp)
{
	uint32 gaincode;
	gaincode = ((lna1byp&0x1)<<23) + (1<<22) +
		((gain_indx[6]&0xF)<<18) +
		((gain_indx[5]&0x7)<<15) +
		((gain_indx[4]&0x7)<<12) +
		((gain_indx[3]&0xF)<< 8) +
		((gain_indx[2]&0x7)<< 5) +
		((gain_indx[1]&0x7)<< 2) +
		((gain_indx[0]&0x1)<< 1) + (trtx&0x1);

	return gaincode;

}

static void wlc_phy_adjust_ed_thres_acphy(phy_type_rxgcrs_ctx_t *ctx, int32 *assert_thresh_dbm,
	bool set_threshold)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t * pi = rxgcrs_info->pi;
	bool suspend = FALSE;
	int32 assert_local_dBm;

	/* Set the EDCRS Assert and De-assert Threshold
	The de-assert threshold is set to 6dB lower then the assert threshold
	Accurate Formula:64*log2(round((10.^((THRESHOLD_dBm +65-30)./10).*50).*(2^9./0.4).^2))
	Simplified Accurate Formula: 64*(THRESHOLD_dBm + 75)/(10*log10(2)) + 832;
	Implemented Approximate Formula: 640000*(THRESHOLD_dBm + 75)/30103 + 832;
	*/
	int32 assert_thres_val, de_assert_thresh_val;
	uint16 th_u16;
	uint8 core;

	/* Suspend MAC */
	wlc_phy_conditional_suspend(pi, &suspend);

	if (set_threshold == TRUE) {
		/* TINY maps 0.2 to adc code 512, whereas non-TINY 0.4, so there is a 6dB
		   difference in formula, and ed logic in chip does not take care of it
		*/
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (TINY_RADIO(pi) &&
			(ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))) {
			assert_local_dBm = *assert_thresh_dbm + 6;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			assert_local_dBm = *assert_thresh_dbm;
		}
		assert_thres_val = (640000*(assert_local_dBm + 75) + 25045696)/30103;
		de_assert_thresh_val = (640000*(assert_local_dBm + 69) + 25045696)/30103;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			/* Set the EDCRS Assert Threshold for core0 */
			WRITE_PHYREG(pi, ed_crs20LAssertThresh00, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20LAssertThresh10, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh00, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh10, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh00, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh10, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh00, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh10, (uint16)assert_thres_val);

			/* Set the EDCRS De-assert Threshold for core0 */
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh00, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh10, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh00, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh10, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh00,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh10,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh00,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh10,
				(uint16)de_assert_thresh_val);

			/* Set the EDCRS Assert Threshold for core1 */
			WRITE_PHYREG(pi, ed_crs20LAssertThresh01, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20LAssertThresh11, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh01, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh11, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh01, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh11, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh01, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh11, (uint16)assert_thres_val);

			/* Set the EDCRS De-assert Threshold for core1 */
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh01, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh11, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh01, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh11, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh01,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh11,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh01,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh11,
				(uint16)de_assert_thresh_val);

			/* Set the EDCRS Assert Threshold for core2 */
			WRITE_PHYREG(pi, ed_crs20LAssertThresh02, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20LAssertThresh12, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh02, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh12, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh02, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh12, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh02, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh12, (uint16)assert_thres_val);

			/* Set the EDCRS De-assert Threshold for core2 */
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh02, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh12, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh02, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh12, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh02,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh12,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh02,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh12,
				(uint16)de_assert_thresh_val);

			/* Set the EDCRS Assert Threshold for core3 */
			WRITE_PHYREG(pi, ed_crs20LAssertThresh03, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20LAssertThresh13, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh03, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh13, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh03, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh13, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh03, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh13, (uint16)assert_thres_val);

			/* Set the EDCRS De-assert Threshold for core3 */
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh03, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh13, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh03, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh13, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh03,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh13,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh03,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh13,
				(uint16)de_assert_thresh_val);

		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		if (ACREV_GE(pi->pubpi->phy_rev, 34) &&
			(!(ACMAJORREV_36(pi->pubpi->phy_rev)))) {
			FOREACH_CORE(pi, core) {
				/* Set the EDCRS Assert Threshold for core0 */
				th_u16 = (uint16)assert_thres_val;
				WRITE_PHYREGCE(pi, ed_crs20LAssertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20LAssertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20UAssertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20UAssertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1AssertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1AssertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Usub1AssertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Usub1AssertThresh1, core, th_u16);

				/* Set the EDCRS De-assert Threshold for core0 */
				th_u16 = (uint16)de_assert_thresh_val;
				WRITE_PHYREGCE(pi, ed_crs20LDeassertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20LDeassertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20UDeassertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20UDeassertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1DeassertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Lsub1DeassertThresh1, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Usub1DeassertThresh0, core, th_u16);
				WRITE_PHYREGCE(pi, ed_crs20Usub1DeassertThresh1, core, th_u16);
			}
		} else {
			/* Set the EDCRS Assert Threshold */
			WRITE_PHYREG(pi, ed_crs20LAssertThresh0, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20LAssertThresh1, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh0, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20UAssertThresh1, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh0, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1AssertThresh1, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh0, (uint16)assert_thres_val);
			WRITE_PHYREG(pi, ed_crs20Usub1AssertThresh1, (uint16)assert_thres_val);

			/* Set the EDCRS De-assert Threshold */
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh0, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20LDeassertThresh1, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh0, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20UDeassertThresh1, (uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh0,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Lsub1DeassertThresh1,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh0,
				(uint16)de_assert_thresh_val);
			WRITE_PHYREG(pi, ed_crs20Usub1DeassertThresh1,
				(uint16)de_assert_thresh_val);
		}
	} else {
		if (ACREV_GE(pi->pubpi->phy_rev, 32)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
			if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
				assert_thres_val = READ_PHYREG(pi, ed_crs20LAssertThresh0);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
			{
				assert_thres_val = READ_PHYREG(pi, ed_crs20LAssertThresh00);
			}
		} else {
			assert_thres_val = READ_PHYREG(pi, ed_crs20LAssertThresh0);
		}
		assert_local_dBm = ((((assert_thres_val - 832)*30103)) - 48000000)/640000;

		/* TINY maps 0.2 to adc code 512, whereas non-TINY 0.4, so there is a 6dB
		   difference in formula, and ed logic in chip does not take care of it
		*/
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (TINY_RADIO(pi) &&
			(ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))) {
			*assert_thresh_dbm = assert_local_dBm - 6;
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			*assert_thresh_dbm = assert_local_dBm;
		}

	}

	/* Resume MAC */
	wlc_phy_conditional_resume(pi, &suspend);
}

#if defined(BCMINTPHYDBG) || defined(WLTEST) || defined(DBG_PHY_IOV) || \
	defined(WFD_PHY_LL_DEBUG)
/*
 * This function has been implemented to get over a problem seen in the multi-DUT test scenario.
 * In this scenarios because of the Tx Cal going on in the other neighbor DUT the Noise
 * calibration in this DUT would get high noise numbers. To avoid this problem this IOVAR can
 * be used to trigger a noise cal at a DUT under known conditions (other DUTs are not doing
 * any calibration). The algo in the PHY averages the last 4 values received from the noise cal
 * and uses them to populate the value in the crsminpoweru0 register. In order to refresh the
 * value in this register to a good value the post processing function (phy_noise_sample_intr)
 * is triggered 4 times after doing a single cal at the PSM, so as to flush out the stale readings.
 */
static int
wlc_phy_noisecal_run_acphy(phy_type_rxgcrs_ctx_t *ctx, void *a, bool set)
{
	phy_ac_rxgcrs_info_t *rxgcrsi = (phy_ac_rxgcrs_info_t *) ctx;
	phy_info_t *pi = rxgcrsi->pi;
	int status = 0, i;
	bool phywatchdog_override;
	bool force_crsmincal;
	bool trigger_crsmin_cal;

	/* Backup the watchdog's flag. */
	phywatchdog_override = pi->phywatchdog_override;
	pi->phywatchdog_override = 0;

	/* Suspend the PSM */
	wlapi_suspend_mac_and_wait(pi->sh->physhim);

	/* Ensure no Noise cal in progress */
	status = R_REG(pi->sh->osh, &pi->regs->maccommand);
	if (status & MCMD_BG_NOISE) {
		ASSERT(0);
		PHY_ERROR(("Fatal error. Aborting, PSM not done with previous noise cal.\n"));
		wlapi_enable_mac(pi->sh->physhim);
		return BCME_ERROR;
	}

	/* Start noise measurements. */
	OR_REG(pi->sh->osh, &pi->regs->maccommand, MCMD_BG_NOISE);

	/* Let the PSM run to perform Noise Cal */
	wlapi_enable_mac(pi->sh->physhim);

	SPINWAIT(R_REG(pi->sh->osh, &pi->regs->maccommand) & MCMD_BG_NOISE,
		ACPHY_SPINWAIT_NOISE_CAL_STATUS);
	if (R_REG(pi->sh->osh, &pi->regs->maccommand) & MCMD_BG_NOISE) {
		PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR : Noise cal failed \n", __FUNCTION__));
		PHY_FATAL_ERROR(pi, PHY_RC_NOISE_CAL_FAILED);
	}

	/* Backup CRS Min cal params */
	force_crsmincal = rxgcrsi->force_crsmincal;
	trigger_crsmin_cal = phy_ac_noise_get_data(pi->u.pi_acphy->noisei)->trigger_crsmin_cal;

	/* Calling it PHY_SIZE_NOISE_ARRAY times so as to take a fresh reading */
	for (i = 0; i < PHY_SIZE_NOISE_ARRAY; i++) {
		/* To set the phy reg to new cal value set the CRS Min cal params */
		rxgcrsi->force_crsmincal = TRUE;
		phy_ac_noise_set_trigger_crsmin_cal(pi->u.pi_acphy->noisei, TRUE);
		phy_noise_sample_intr((wlc_phy_t *)pi);
	}

	/* Restore the watchdog's flag. */
	pi->phywatchdog_override = phywatchdog_override;
	phy_ac_noise_set_trigger_crsmin_cal(pi->u.pi_acphy->noisei, trigger_crsmin_cal);
	rxgcrsi->force_crsmincal = force_crsmincal;

	return BCME_OK;
}
#endif /* BCMINTPHYDBG || WLTEST || DBG_PHY_IOV || WFD_PHY_LL_DEBUG */

#ifdef WL11ULB
void
phy_ac_rxgcrs_ulb_cal(phy_ac_rxgcrs_info_t *rxgcrsi, uint8 enULB)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	phy_info_t *pi = rxgcrsi->pi;
	uint8 phase_id = pi->cal_info->cal_phase_id;
	if (PHY_ULB_ENAB(pi)) {
		if (ACMAJORREV_36(pi->pubpi->phy_rev) && (enULB == 1)) {
			/* Enable ULB mode */
			wlc_phy_ulb_mode(pi, CHSPEC_IS10(pi->radio_chanspec) ? PMU_ULB_BW_10MHZ :
					CHSPEC_IS5(pi->radio_chanspec) ? PMU_ULB_BW_5MHZ
					:  PMU_ULB_BW_2P5MHZ);

			if ((phase_id == MPHASE_CAL_STATE_IDLE) ||
					(phase_id == ACPHY_CAL_PHASE_RXCAL)) {
				/* Do CRS min pwr cal for ULB mode */
				if ((rxgcrsi->crsmincal_enable)) {
					PHY_CAL(("%s : crsminpwr cal\n", __FUNCTION__));
					rxgcrsi->force_crsmincal = TRUE;
					phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
				}
			}
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
}
#endif /* WL11ULB */

void
phy_ac_rxgcrs_cal(phy_ac_rxgcrs_info_t *rxgcrsi, uint8 enULB)
{
	phy_info_t *pi = rxgcrsi->pi;
	if ((pi->cal_info->cal_phase_id == MPHASE_CAL_STATE_IDLE) ||
		(pi->cal_info->cal_phase_id == ACPHY_CAL_PHASE_RXCAL)) {
		/* Disable CRS min pwr cal in ULB mode
		 * For ULB mode, do crs cal at the end of the function
		 */
		if ((rxgcrsi->crsmincal_enable ||
			rxgcrsi->lesiscalecal_enable) && (enULB == 0)) {

			bool save_phynoise_disable;

			rxgcrsi->force_crsmincal = TRUE;
			PHY_CAL(("%s : crsminpwr cal\n", __FUNCTION__));
			rxgcrsi->force_crsmincal = TRUE;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
			if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
				ACMAJORREV_33(pi->pubpi->phy_rev)) {
				phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
			} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
			{
				/* save and enable the noisecal schedule state  */
				save_phynoise_disable = phy_noise_sched_get((wlc_phy_t *)pi);
				phy_noise_sched_set((wlc_phy_t *)pi, PHY_CAL_MODE, FALSE);
				phy_noise_sample_request_crsmincal((wlc_phy_t*)pi);
				 /* restore the noisecal schedule state */
				phy_noise_sched_set((wlc_phy_t *)pi, PHY_CAL_MODE,
					save_phynoise_disable);
			}
		}
	}
}

#if defined(RXDESENS_EN)
static int
phy_ac_rxgcrs_get_rxdesens(phy_type_rxgcrs_ctx_t *ctx, int32 *ret_int_ptr)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	if (!ACPHY_ENABLE_FCBS_HWACI(rxgcrs_info->pi) && rxgcrs_info->total_desense.forced) {
		*ret_int_ptr = (int32)rxgcrs_info->total_desense.ofdm_desense;
		return BCME_OK;
	} else {
		err = BCME_UNSUPPORTED;
	}
}

static int
phy_ac_rxgcrs_set_rxdesens(phy_type_rxgcrs_ctx_t *ctx, int32 int_val)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;
	int err = BCME_OK;
	if (!(ACPHY_ENABLE_FCBS_HWACI(pi)) {
		wlapi_suspend_mac_and_wait(pi->sh->physhim);
		if (int_val <= 0) {
			/* disable phy_rxdesens and restore
			 * default interference mode
			 */
			rxgcrs_info->total_desense.forced = FALSE;
			pi->sh->interference_mode_override = FALSE;
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				pi->sh->interference_mode = pi->sh->interference_mode_2G;
			} else {
				pi->sh->interference_mode = pi->sh->interference_mode_5G;
			}

#ifndef WLC_DISABLE_ACI
			/* turn off interference mode
			 * before entering another mode
			 */
			if (pi->sh->interference_mode != INTERFERE_NONE) {
				phy_noise_set_mode(pi, INTERFERE_NONE, TRUE);
			}
			if (!phy_noise_set_mode(pi, pi->sh->interference_mode, TRUE)) {
				err = BCME_BADOPTION;
			}
#endif /* !defined(WLC_DISABLE_ACI) */

			/* restore crsmincal automode, and force crsmincal */
			int8 negative = -1;
			wlc_phy_force_crsmin_acphy(pi, &negative);
			phy_ac_rxgcrs_force_lesiscale(rxgcrs_info, &negative);

		} else {
			/* enable phy_rxdesens and disable interference mode
			* through override mode
			*/
			pi->sh->interference_mode_override = TRUE;
			pi->sh->interference_mode_2G_override = INTERFERE_NONE;
			pi->sh->interference_mode_5G_override = INTERFERE_NONE;
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				pi->sh->interference_mode = pi->sh->interference_mode_2G_override;
			} else {
				pi->sh->interference_mode = pi->sh->interference_mode_5G_override;
			}
			phy_noise_set_mode(pi, INTERFERE_NONE, TRUE);

			/* disable crsmincal */
			rxgcrs_info->crsmincal_enable = FALSE;
			rxgcrs_info->lesiscalecal_enable = FALSE;
			/* apply desense */
			rxgcrs_info->total_desense.forced = TRUE;
			rxgcrs_info->total_desense.ofdm_desense = (uint8)int_val;
			rxgcrs_info->total_desense.bphy_desense = (uint8)int_val;
			wlc_phy_desense_apply_acphy(pi, TRUE);

		}
		wlapi_enable_mac(pi->sh->physhim);
	} else {
		err = BCME_UNSUPPORTED;
	}
	return err;
}
#endif /* defined(RXDESENS_EN) */

void
phy_ac_rxgcrs_lesi(phy_ac_rxgcrs_info_t *rxgcrsi, bool on, uint8 delta_halfdB)
{
	phy_info_t *pi = rxgcrsi->pi;
	uint8 delta = (delta_halfdB << 3);   // 16 ticks = 1dB

	if (rxgcrsi->lesi > 0) {
		if (on) {
			MOD_PHYREG(pi, lesi_control, lesiFstrEn, 1);
			MOD_PHYREG(pi, lesi_control, lesiCrsEn, 1);
			if (!ACMAJORREV_37(pi->pubpi->phy_rev) &&
			    !ACMAJORREV_40(pi->pubpi->phy_rev)) {
				MOD_PHYREG(pi, lesiFstrControl0, winmf_1stPeak_Scl, 0x8);
				MOD_PHYREG(pi, lesiCrsTypRxPowerPerCore, PowerLevelPerCore,
				       CHSPEC_IS20(pi->radio_chanspec) ?
				       0x15b : CHSPEC_IS40(pi->radio_chanspec) ? 0x228 : 0x308);
				MOD_PHYREG(pi, lesiCrsHighRxPowerPerCore, PowerLevelPerCore,
				       CHSPEC_IS20(pi->radio_chanspec) ?
				       0x76e : CHSPEC_IS40(pi->radio_chanspec) ? 0x9c1 : 0xAA2);
				MOD_PHYREG(pi, lesiCrsMinRxPowerPerCore, PowerLevelPerCore,
				       CHSPEC_IS20(pi->radio_chanspec) ?
				       0x5 : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x4);
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_1, crsDetTh1_1Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x41 : CHSPEC_IS40(pi->radio_chanspec) ? 0x2d : 0x23));
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_1, crsDetTh1_2Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x30 : CHSPEC_IS40(pi->radio_chanspec) ? 0x20 : 0x1b));
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_2, crsDetTh1_3Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x28 : CHSPEC_IS40(pi->radio_chanspec) ? 0x1b : 0x17));
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_2, crsDetTh1_4Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x23 : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x14));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_1, crsDetTh1_1Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x41 : CHSPEC_IS40(pi->radio_chanspec) ? 0x2d : 0x23));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_1, crsDetTh1_2Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x30 : CHSPEC_IS40(pi->radio_chanspec) ? 0x20 : 0x1b));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_2, crsDetTh1_3Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x28 : CHSPEC_IS40(pi->radio_chanspec) ? 0x1b : 0x17));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_2, crsDetTh1_4Core, delta +
						(CHSPEC_IS20(pi->radio_chanspec) ?
						0x23 : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x14));

				MOD_PHYREG(pi, lesiFstrControl3, cCrsFftInpAdj,
				       CHSPEC_IS20(pi->radio_chanspec) ?
				       0x0 : CHSPEC_IS40(pi->radio_chanspec) ? 0x1 : 0x3);
				MOD_PHYREG(pi, lesiFstrClassifyThreshold0, MaxScaleHighValue, 0x5a);
			}
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_33))
			if (ACMAJORREV_33(pi->pubpi->phy_rev)) {
				uint8 lsb, msb;
				uint16 val = 0;
				/* the default value caused the degradation on SGI for C1s4 */
				MOD_PHYREG(pi, lesiFstrControl5, lesi_sgi_hw_adj, 0x13);
				/* Increase the 20 primary crs detection thresold
				* which cause the PER floor
				*/
				msb = 0x33 + delta; lsb = 0x44 + delta;		   /* 0x3344 + delta */
				val = (msb << 8) | lsb;
				WRITE_PHYREG(pi, lesiCrs20P1stDetThreshold_1, val);
				WRITE_PHYREG(pi, lesiCrs20P2ndDetThreshold_1, val);

				msb = 0x26 + delta; lsb = 0x2a + delta;		  /* 0x262a + delta */
				val = (msb << 8) | lsb;
				WRITE_PHYREG(pi, lesiCrs20P1stDetThreshold_2, val);
				WRITE_PHYREG(pi, lesiCrs20P2ndDetThreshold_2, val);
				/* PER hump at low power regime siwthcing point at 2G band,
				* the WAR is to set fstrSwitchEn
				*/
				if (CHSPEC_IS2G(pi->radio_chanspec)) {
				  MOD_PHYREG(pi, lesiFstrControl4, fstrSwitchEn, 1);
				  MOD_PHYREG(pi, lesiFstrControl4, selLesiCstr, 0);
				}
			}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_33)) */

			if (ACMAJORREV_37(pi->pubpi->phy_rev) ||
			    ACMAJORREV_40(pi->pubpi->phy_rev)) {
			/* WAR for RX PER spike when LESI enabled */
				MOD_PHYREG(pi, lesiFstrControl4, fstrSwitchEn, 1);
				MOD_PHYREG(pi, lesiFstrControl4, selLesiCstr, 0);
				MOD_PHYREG(pi, lesiFstrModeSwitchLoPower, fstrSwitchPwrLo1c,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0xec : CHSPEC_IS40(pi->radio_chanspec) ? 0x19d : 0x13f);
				MOD_PHYREG(pi, lesiFstrModeSwitchHiPower, fstrSwitchPwrHi1c,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x239 : CHSPEC_IS40(pi->radio_chanspec) ? 0x33d : 0x211);

				MOD_PHYREG(pi, lesiFstrControl4, lCrsFftOp1Adj,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x1 : CHSPEC_IS40(pi->radio_chanspec) ? 0x2 : 0x4);
				MOD_PHYREG(pi, lesiFstrControl4, delayCmbCfo,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x0 : CHSPEC_IS40(pi->radio_chanspec) ? 0x1 : 0x1);

				MOD_PHYREG(pi, lesiFstrControl0, winmf_1stPeak_Scl, 0x4);

				WRITE_PHYREG(pi, lesiCrsTypRxPowerPerCore,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x15b : CHSPEC_IS40(pi->radio_chanspec) ? 0x228 : 0x184);
				WRITE_PHYREG(pi, lesiCrsHighRxPowerPerCore,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x76e : CHSPEC_IS40(pi->radio_chanspec) ? 0x9c1 : 0x551);
				WRITE_PHYREG(pi, lesiCrsMinRxPowerPerCore,
					CHSPEC_IS20(pi->radio_chanspec) ?
					0x5 : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x2c);

				WRITE_PHYREG(pi, LowPowerRegimeControlUpperThreshold0, 0x4e);
				WRITE_PHYREG(pi, LowPowerRegimeControlLowerThreshold0, 0x48);
				WRITE_PHYREG(pi, LowPowerRegimeControlUpperThreshold1, 0x4e);
				WRITE_PHYREG(pi, LowPowerRegimeControlLowerThreshold1, 0x48);
				/* Increase the LESI Crs1st/Crs2nd Detection Thres to help
				* reduce the PER floor while not degrading the sensitivity
				 */
				WRITE_PHYREG(pi, lesiCrs1stDetThreshold_1, (delta << 8 | delta) +	// Add to both 1 Core and 2 Cores
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x3344 : CHSPEC_IS40(pi->radio_chanspec) ? 0x2633 : 0x1e26));
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_2, crsDetTh1_3Core, delta +
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x2a : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x11));
				MOD_PHYREG(pi, lesiCrs1stDetThreshold_2, crsDetTh1_4Core, delta +
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x26 : CHSPEC_IS40(pi->radio_chanspec) ? 0x15 : 0xe));

				WRITE_PHYREG(pi, lesiCrs2ndDetThreshold_1, (delta << 8 | delta) +	// Add to both 1 Core and 2 Cores
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x3344 : CHSPEC_IS40(pi->radio_chanspec) ? 0x2633 : 0x1e26));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_2, crsDetTh1_3Core, delta +
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x2a : CHSPEC_IS40(pi->radio_chanspec) ? 0x18 : 0x11));
				MOD_PHYREG(pi, lesiCrs2ndDetThreshold_2, crsDetTh1_4Core, delta +
					(CHSPEC_IS20(pi->radio_chanspec) ?
					0x26 : CHSPEC_IS40(pi->radio_chanspec) ? 0x15 : 0xe));

				MOD_PHYREG(pi, lesiHTPktGain, LesiHTAgcPktgainWait, 0xc);
				MOD_PHYREG(pi, lesiHTPktGain, LesiHTAGCStrtAdjst, 0);

				MOD_PHYREG(pi, lesiFstrClassifyThreshold0, MaxScaleHighValue, 0x40);
				MOD_PHYREG(pi, lesiFstrClassifyThreshold0, MaxScaleLowValue, 0x3);

				MOD_PHYREG(pi, lesiFstrControl5, lesi_sgi_hw_adj, 0x13);
				MOD_PHYREG(pi, lesiFstrControl3, lesi_sgi_adj, 0xa);

			    /* Increase the 20P crs detection thresold for lower PER floor */
			    WRITE_PHYREG(pi, lesiCrs20P1stDetThreshold_1, 0x3344 + (delta << 8 | delta));
			    WRITE_PHYREG(pi, lesiCrs20P2ndDetThreshold_1, 0x3344 + (delta << 8 | delta));
			    WRITE_PHYREG(pi, lesiCrs20P1stDetThreshold_2, 0x262a + (delta << 8 | delta));
			    WRITE_PHYREG(pi, lesiCrs20P2ndDetThreshold_2, 0x262a + (delta << 8 | delta));
				/* CRDOT11ACPHY-2555:
				* lesi and bphy_pre_det set in aux block bphy pkt
				*/
				if (wlapi_si_coreunit(pi->sh->physhim) == DUALMAC_AUX) {
					MOD_PHYREG(pi, CRSMiscellaneousParam, bphy_pre_det_en, 0);
				}
			}
		} else {
			MOD_PHYREG(pi, lesi_control, lesiFstrEn, 0);
			MOD_PHYREG(pi, lesi_control, lesiCrsEn, 0);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
			if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
				if (wlapi_si_coreunit(pi->sh->physhim) == DUALMAC_AUX &&
						CHSPEC_IS2G(pi->radio_chanspec))
					MOD_PHYREG(pi, CRSMiscellaneousParam, bphy_pre_det_en, 1);
			}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		}
	}

	if (rxgcrsi->lesi > 0 && PHY_AS_80P80(pi, pi->radio_chanspec)) {
		  MOD_PHYREG(pi, lesi_control, lesiFstrEn, 0);
		  MOD_PHYREG(pi, lesi_control, lesiCrsEn, 0);
	}
}

int
phy_ac_rxgcrs_iovar_set_lesi(phy_ac_rxgcrs_info_t *rxgcrsi, int32 set_val)
{
	phy_info_t *pi = rxgcrsi->pi;

	if (rxgcrsi->lesi_mode) {
		return BCME_UNSUPPORTED;
	}

	MOD_PHYREG(pi, lesi_control, lesiFstrEn, (uint16) set_val);
	MOD_PHYREG(pi, lesi_control, lesiCrsEn, (uint16) set_val);

	/* chippkg bit-2: LESI supported, 0: TRUE ; 1: FALSE */
	if (ACMAJORREV_37(pi->pubpi->phy_rev) || (ACMAJORREV_33(pi->pubpi->phy_rev) && (pi->sh->chippkg & 0x4) == 0)) {
		rxgcrsi->lesi = (int8) set_val;
	}

	if (set_val > 0) {
		phy_ac_rxgcrs_lesi(rxgcrsi, TRUE, 0);
	} else {
		phy_ac_rxgcrs_lesi(rxgcrsi, FALSE, 0);
	}
	return BCME_OK;
}

int
phy_ac_rxgcrs_iovar_get_lesi(phy_ac_rxgcrs_info_t *rxgcrsi, int32 *ret_val)
{
	phy_info_t *pi = rxgcrsi->pi;
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
		*ret_val = rxgcrsi->lesi;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
	{
		*ret_val = (int32) (READ_PHYREG(pi, lesi_control) & 0x1);
	}
	return BCME_OK;
}

void phy_ac_rxgcrs_clean_noise_array(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	uint8 indx, i;
	uint8 phyrxchain = phy_stf_get_data(rxgcrsi->pi->stfi)->phyrxchain;

	BCM_REFERENCE(phyrxchain);

	FOREACH_ACTV_CORE(rxgcrsi->pi, phyrxchain, i) {
		for (indx = 0; indx < PHY_SIZE_NOISE_ARRAY; indx++) {
			rxgcrsi->phy_noise_pwr_array[indx][i] = 0;
		}
	}
	rxgcrsi->phy_noise_counter = 0;
}

bool phy_ac_rxgcrs_get_lesi(phy_ac_rxgcrs_info_t *rxgcrsi)
{
	char *val = NULL;
	uint lesidisab = 0;

	if (rxgcrsi == NULL) {
		if ((val = nvram_get("lesidisab")) != NULL) {
			lesidisab = bcm_atoi(val);
		}
		return (lesidisab == FALSE);
	}
	else
		return rxgcrsi->lesi;
}

static uint16
phy_ac_rxgcrs_sel_classifier(phy_type_rxgcrs_ctx_t *ctx, uint16 val)
{
	phy_info_t *pi = ((phy_ac_rxgcrs_info_t *) ctx)->pi;
	uint16 curr_ctl, new_ctl;
	bool suspend;
	uint16 mask = ACPHY_ClassifierCtrl_classifierSel_MASK(pi->pubpi->phy_rev);

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (!pi->sh->clk)
		return BCME_NOCLK;

	suspend = !(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC);
	if (!suspend) {
		wlapi_suspend_mac_and_wait(pi->sh->physhim);
	}
	/* Turn on/off classification (bphy, ofdm, and wait_ed), mask and
	 * val are bit fields, bit 0: bphy, bit 1: ofdm, bit 2: wait_ed;
	 * for types corresponding to bits set in mask, apply on/off state
	 * from bits set in val.
	 */
	curr_ctl = READ_PHYREG(pi, ClassifierCtrl);

	new_ctl = (curr_ctl & (~mask)) | (val & mask);

	WRITE_PHYREG(pi, ClassifierCtrl, new_ctl);

	if (!suspend) {
		wlapi_enable_mac(pi->sh->physhim);
	}
	return new_ctl;
}

#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static void
phy_ac_rxgcrs_phydump_chanest(phy_type_rxgcrs_ctx_t *ctx, struct bcmstrbuf *b)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;
	uint16 num_rx, num_sts, num_tones, start_tone;
	uint16 k, r, t, fftk;
	uint32 ch;
	uint16 ch_re_ma, ch_im_ma;
	uint8  ch_re_si, ch_im_si;
	int16  ch_re, ch_im;
	int8   ch_exp;
	uint8  dump_tones;

	num_rx = (uint8)PHYCORENUM(pi->pubpi->phy_corenum);
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_4)))
	if (ACMAJORREV_2(pi->pubpi->phy_rev) ||
		ACMAJORREV_4(pi->pubpi->phy_rev)) {
		/* number of spatial streams supported */
		num_sts = 2;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_4))) */
	{
		num_sts = 4;
	}
	if (CHSPEC_IS40(pi->radio_chanspec)) {
		num_tones = 128;
#ifdef CHSPEC_IS80
	} else if (CHSPEC_IS80(pi->radio_chanspec)) {
		num_tones = 256;
#endif /* CHSPEC_IS80 */
	} else {
		num_tones = 64;
	}

	bcm_bprintf(b, "num_tones=%d\n", num_tones);

	/* Dump only 16 sub-carriers at a time */
	dump_tones = 16;
	/* Reset the dump counter */
	if (pi->phy_chanest_dump_ctr > (num_tones/dump_tones - 1))
		pi->phy_chanest_dump_ctr = 0;

	start_tone = pi->phy_chanest_dump_ctr * dump_tones;
	pi->phy_chanest_dump_ctr++;

	for (r = 0; r < num_rx; r++) {
		bcm_bprintf(b, "rx=%d\n", r);
		for (t = 0; t < num_sts; t++) {
			bcm_bprintf(b, "sts=%d\n", t);
			for (k = start_tone; k < (start_tone + dump_tones); k++) {
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_CHANEST(r), 1,
				                         t*256 + k, 32, &ch);

				if (ACMAJORREV_0(pi->pubpi->phy_rev) ||
					ACMAJORREV_1(pi->pubpi->phy_rev) ||
					(ACMAJORREV_2(pi->pubpi->phy_rev) &&
					SW_ACMINORREV_2(pi->pubpi->phy_rev)) ||
					ACMAJORREV_5(pi->pubpi->phy_rev) ||
					ACMAJORREV_32(pi->pubpi->phy_rev) ||
					ACMAJORREV_33(pi->pubpi->phy_rev) ||
					ACMAJORREV_37(pi->pubpi->phy_rev)) {
					/* Q11 FLP (12,12,6) */
					ch_re_ma  = ((ch >> 18) & 0x7ff);
					ch_re_si  = ((ch >> 29) & 0x001);
					ch_im_ma  = ((ch >>  6) & 0x7ff);
					ch_im_si  = ((ch >> 17) & 0x001);
					ch_exp    = ((int8)((ch << 2) & 0xfc)) >> 2;
					ch_re = (ch_re_si > 0) ? -ch_re_ma : ch_re_ma;
					ch_im = (ch_im_si > 0) ? -ch_im_ma : ch_im_ma;
				} else if (ACMAJORREV_2(pi->pubpi->phy_rev) ||
						ACMAJORREV_4(pi->pubpi->phy_rev) ||
						ACMAJORREV_40(pi->pubpi->phy_rev)) {
					/* Q8 FLP (9,9,5) */
					ch_re_ma  = ((ch >> 14) & 0xff);
					ch_re_si  = ((ch >> 22) & 0x01);
					ch_im_ma  = ((ch >>  5) & 0xff);
					ch_im_si  = ((ch >> 13) & 0x01);
					ch_exp    = ((int8)((ch << 3) & 0xf8)) >> 3;
					ch_re = (ch_re_si > 0) ? -ch_re_ma : ch_re_ma;
					ch_im = (ch_im_si > 0) ? -ch_im_ma : ch_im_ma;
				} else {
					/* Q13 FXP (14,14) */
					ch_re_ma  = ((ch >> 14) & 0x3fff);
					ch_im_ma  = ((ch >>  0) & 0x3fff);
					ch_exp = 0;
					ch_re = ((int16)((ch_re_ma << 2) & 0xfffc)) >> 2;
					ch_im = ((int16)((ch_im_ma << 2) & 0xfffc)) >> 2;
				}
				fftk = ((k < num_tones/2) ? (k + num_tones/2) : (k - num_tones/2));

				bcm_bprintf(b, "chan(%d,%d,%d)=(%d+i*%d)*2^%d;\n",
				            r+1, t+1, fftk+1, ch_re, ch_im, ch_exp);
			}
		}
	}
}
#endif /* defined(BCMDBG) || defined(BCMDBG_DUMP) */

#ifdef WLTEST
static void
phy_ac_rxgcrs_get_chanest(phy_type_rxgcrs_ctx_t *ctx, uint16 fftk, uint16 idx,
	int16 *ch_re, int16 *ch_im)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;
	uint32 ch, tbl_id;
	uint16 ch_re_ma, ch_im_ma;
	uint8  ch_re_si, ch_im_si;

	tbl_id = ACPHY_TBL_ID_CHANEST(idx);
	wlc_phy_table_read_acphy(pi, tbl_id, 1, fftk, 32, &ch);
	if (ACMAJORREV_0(pi->pubpi->phy_rev) || ACMAJORREV_5(pi->pubpi->phy_rev) ||
	    ACMAJORREV_32(pi->pubpi->phy_rev) ||
	    ACMAJORREV_33(pi->pubpi->phy_rev) ||
	    ACMAJORREV_37(pi->pubpi->phy_rev)) {
		ch_re_ma  = ((ch >> 18) & 0x7ff);
		ch_re_si  = ((ch >> 29) & 0x001);
		ch_im_ma  = ((ch >>  6) & 0x7ff);
		ch_im_si  = ((ch >> 17) & 0x001);
	} else {
		ch_re_ma  = ((ch >> 14) & 0xff);
		ch_re_si  = ((ch >> 22) & 0x01);
		ch_im_ma  = ((ch >>  5) & 0xff);
		ch_im_si  = ((ch >> 13) & 0x01);
	}
	*ch_re = (ch_re_si > 0) ? -ch_re_ma : ch_re_ma;
	*ch_im = (ch_im_si > 0) ? -ch_im_ma : ch_im_ma;
}
#endif /* WLTEST */

#if defined(DBG_BCN_LOSS)
static int
phy_ac_rxgcrs_dump_phycal_rx_min(phy_type_rxgcrs_ctx_t *ctx, struct bcmstrbuf *b)
{
	phy_ac_rxgcrs_info_t *rxgcrs_info = (phy_ac_rxgcrs_info_t *)ctx;
	phy_info_t *pi = rxgcrs_info->pi;

	PHY_ERROR(("wl%d: %s: AC Phy not yet supported\n", pi->sh->unit, __FUNCTION__));
	return BCME_UNSUPPORTED;
}
#endif /* DBG_BCN_LOSS */
