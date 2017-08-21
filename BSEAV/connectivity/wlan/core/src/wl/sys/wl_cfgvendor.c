/*
 * Linux cfg80211 Vendor Extension Code
 *
 * Copyright (C) 2017, Broadcom. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id$
 */

/*
 * New vendor interface additon to nl80211/cfg80211 to allow vendors
 * to implement proprietary features over the cfg80211 stack.
*/

#include <typedefs.h>
#include <linuxver.h>
#include <osl.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>

#include <bcmutils.h>
#include <bcmwifi_channels.h>
#include <bcmendian.h>
#include <proto/ethernet.h>
#include <proto/802.11.h>
#include <linux/if_arp.h>
#include <asm/uaccess.h>

#if defined(BCMDONGLEHOST)
#include <dngl_stats.h>
#include <dhd.h>
#include <dhd_debug.h>
#include <dhdioctl.h>
#include <wlioctl.h>
#include <wlioctl_utils.h>
#include <dhd_cfg80211.h>
#ifdef PNO_SUPPORT
#include <dhd_pno.h>
#endif /* PNO_SUPPORT */
#ifdef RTT_SUPPORT
#include <dhd_rtt.h>
#endif /* RTT_SUPPORT */
#endif /* defined(BCMDONGLEHOST) */

#include <proto/ethernet.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <linux/etherdevice.h>
#include <linux/wireless.h>
#include <linux/ieee80211.h>
#include <linux/wait.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>

#include <wlioctl.h>
#include <wldev_common.h>
#include <wl_cfg80211.h>
#include <wl_cfgp2p.h>
#ifdef WL_NAN
#include <wl_cfgnan.h>
#endif /* WL_NAN */
#ifdef OEM_ANDROID
#include <wl_android.h>
#endif /* OEM_ANDROID */
#include <wl_cfgvendor.h>
#ifdef PROP_TXSTATUS
#include <dhd_wlfc.h>
#endif
#include <brcm_nl80211.h>

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT)

/*
 * This API is to be used for asynchronous vendor events. This
 * shouldn't be used in response to a vendor command from its
 * do_it handler context (instead wl_cfgvendor_send_cmd_reply should
 * be used).
 */
#if defined(BCMDONGLEHOST) || defined(PLATFORM_INTEGRATED_WIFI)
int wl_cfgvendor_send_async_event(struct wiphy *wiphy,
	struct net_device *dev, int event_id, const void  *data, int len)
{
	u16 kflags;
	struct sk_buff *skb;

	kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;

	/* Alloc the SKB for vendor_event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
	skb = cfg80211_vendor_event_alloc(wiphy, NULL, len, event_id, kflags);
#else
	skb = cfg80211_vendor_event_alloc(wiphy, len, event_id, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
	if (!skb) {
		WL_ERR(("skb alloc failed"));
		return -ENOMEM;
	}

	/* Push the data to the skb */
	nla_put_nohdr(skb, len, data);

	cfg80211_vendor_event(skb, kflags);

	return 0;
}

static int
wl_cfgvendor_send_cmd_reply(struct wiphy *wiphy,
	struct net_device *dev, const void  *data, int len)
{
	struct sk_buff *skb;

	/* Alloc the SKB for vendor_event */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, len);
	if (unlikely(!skb)) {
		WL_ERR(("skb alloc failed"));
		return -ENOMEM;
	}

	/* Push the data to the skb */
	nla_put_nohdr(skb, len, data);

	return cfg80211_vendor_cmd_reply(skb);
}
#endif /* BCMDONGLEHOST || PLATFORM_INTEGRATED_WIFI */

#if defined(BCMDONGLEHOST)
static int
wl_cfgvendor_get_feature_set(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int reply;

	reply = dhd_dev_get_feature_set(bcmcfg_to_prmry_ndev(cfg));

	err =  wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        &reply, sizeof(int));
	if (unlikely(err))
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));

	return err;
}

static int
wl_cfgvendor_get_feature_set_matrix(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct sk_buff *skb;
	int reply;
	int mem_needed, i;

	mem_needed = VENDOR_REPLY_OVERHEAD +
		(ATTRIBUTE_U32_LEN * MAX_FEATURE_SET_CONCURRRENT_GROUPS) + ATTRIBUTE_U32_LEN;

	/* Alloc the SKB for vendor_event */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
	if (unlikely(!skb)) {
		WL_ERR(("skb alloc failed"));
		err = -ENOMEM;
		goto exit;
	}

	nla_put_u32(skb, ANDR_WIFI_ATTRIBUTE_NUM_FEATURE_SET, MAX_FEATURE_SET_CONCURRRENT_GROUPS);
	for (i = 0; i < MAX_FEATURE_SET_CONCURRRENT_GROUPS; i++) {
		reply = dhd_dev_get_feature_set_matrix(bcmcfg_to_prmry_ndev(cfg), i);
		if (reply != WIFI_FEATURE_INVALID) {
			nla_put_u32(skb, ANDR_WIFI_ATTRIBUTE_FEATURE_SET, reply);
		}
	}

	err =  cfg80211_vendor_cmd_reply(skb);

	if (unlikely(err)) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));
	}
exit:
	return err;
}

static int
wl_cfgvendor_set_rand_mac_oui(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int type;
	uint8 random_mac_oui[DOT11_OUI_LEN];

	type = nla_type(data);

	if (type == ANDR_WIFI_ATTRIBUTE_RANDOM_MAC_OUI) {
		memcpy(random_mac_oui, nla_data(data), DOT11_OUI_LEN);

		err = dhd_dev_cfg_rand_mac_oui(bcmcfg_to_prmry_ndev(cfg), random_mac_oui);

		if (unlikely(err))
			WL_ERR(("Bad OUI, could not set:%d \n", err));

	} else {
		err = -1;
	}

	return err;
}
#endif /* defined(BCMDONGLEHOST) */

#ifdef CUSTOM_FORCE_NODFS_FLAG
static int
wl_cfgvendor_set_nodfs_flag(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int type;
	u32 nodfs;

	type = nla_type(data);
	if (type == ANDR_WIFI_ATTRIBUTE_NODFS_SET) {
		nodfs = nla_get_u32(data);
		err = dhd_dev_set_nodfs(bcmcfg_to_prmry_ndev(cfg), nodfs);
	} else {
		err = -1;
	}
	return err;
}
#endif /* CUSTOM_FORCE_NODFS_FLAG */
#ifdef GSCAN_SUPPORT
int
wl_cfgvendor_send_hotlist_event(struct wiphy *wiphy,
	struct net_device *dev, void  *data, int len, wl_vendor_event_t event)
{
	u16 kflags;
	const void *ptr;
	struct sk_buff *skb;
	int malloc_len, total, iter_cnt_to_send, cnt;
	gscan_results_cache_t *cache = (gscan_results_cache_t *)data;

	total = len/sizeof(wifi_gscan_result_t);
	while (total > 0) {
		malloc_len = (total * sizeof(wifi_gscan_result_t)) + VENDOR_DATA_OVERHEAD;
		if (malloc_len > NLMSG_DEFAULT_SIZE) {
			malloc_len = NLMSG_DEFAULT_SIZE;
		}
		iter_cnt_to_send =
		   (malloc_len - VENDOR_DATA_OVERHEAD)/sizeof(wifi_gscan_result_t);
		total = total - iter_cnt_to_send;

		kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;

		/* Alloc the SKB for vendor_event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
		skb = cfg80211_vendor_event_alloc(wiphy, NULL, malloc_len, event, kflags);
#else
		skb = cfg80211_vendor_event_alloc(wiphy, malloc_len, event, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
		if (!skb) {
			WL_ERR(("skb alloc failed"));
			return -ENOMEM;
		}

		while (cache && iter_cnt_to_send) {
			ptr = (const void *) &cache->results[cache->tot_consumed];

			if (iter_cnt_to_send < (cache->tot_count - cache->tot_consumed)) {
				cnt = iter_cnt_to_send;
			} else {
				cnt = (cache->tot_count - cache->tot_consumed);
			}

			iter_cnt_to_send -= cnt;
			cache->tot_consumed += cnt;
			/* Push the data to the skb */
			nla_append(skb, cnt * sizeof(wifi_gscan_result_t), ptr);
			if (cache->tot_consumed == cache->tot_count) {
				cache = cache->next;
			}

		}

		cfg80211_vendor_event(skb, kflags);
	}

	return 0;
}


static int
wl_cfgvendor_gscan_get_capabilities(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pno_gscan_capabilities_t *reply = NULL;
	uint32 reply_len = 0;


	reply = dhd_dev_pno_get_gscan(bcmcfg_to_prmry_ndev(cfg),
	   DHD_PNO_GET_CAPABILITIES, NULL, &reply_len);
	if (!reply) {
		WL_ERR(("Could not get capabilities\n"));
		err = -EINVAL;
		return err;
	}

	err =  wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        reply, reply_len);

	if (unlikely(err)) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));
	}

	kfree(reply);
	return err;
}

static int
wl_cfgvendor_gscan_get_channel_list(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0, type, band;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	uint16 *reply = NULL;
	uint32 reply_len = 0, num_channels, mem_needed;
	struct sk_buff *skb;

	type = nla_type(data);

	if (type == GSCAN_ATTRIBUTE_BAND) {
		band = nla_get_u32(data);
	} else {
		return -EINVAL;
	}

	reply = dhd_dev_pno_get_gscan(bcmcfg_to_prmry_ndev(cfg),
	   DHD_PNO_GET_CHANNEL_LIST, &band, &reply_len);

	if (!reply) {
		WL_ERR(("Could not get channel list\n"));
		err = -EINVAL;
		return err;
	}
	num_channels =  reply_len/ sizeof(uint32);
	mem_needed = reply_len + VENDOR_REPLY_OVERHEAD + (ATTRIBUTE_U32_LEN * 2);

	/* Alloc the SKB for vendor_event */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
	if (unlikely(!skb)) {
		WL_ERR(("skb alloc failed"));
		err = -ENOMEM;
		goto exit;
	}

	nla_put_u32(skb, GSCAN_ATTRIBUTE_NUM_CHANNELS, num_channels);
	nla_put(skb, GSCAN_ATTRIBUTE_CHANNEL_LIST, reply_len, reply);

	err =  cfg80211_vendor_cmd_reply(skb);

	if (unlikely(err)) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));
	}
exit:
	kfree(reply);
	return err;
}

static int
wl_cfgvendor_gscan_get_batch_results(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	gscan_results_cache_t *results, *iter;
	uint32 reply_len, is_done = 1;
	int32 mem_needed, num_results_iter;
	wifi_gscan_result_t *ptr;
	uint16 num_scan_ids, num_results;
	struct sk_buff *skb;
	struct nlattr *scan_hdr, *complete_flag;

	err = dhd_dev_wait_batch_results_complete(bcmcfg_to_prmry_ndev(cfg));
	if (err != BCME_OK)
		return -EBUSY;

	err = dhd_dev_pno_lock_access_batch_results(bcmcfg_to_prmry_ndev(cfg));
	if (err != BCME_OK) {
		WL_ERR(("Can't obtain lock to access batch results %d\n", err));
		return -EBUSY;
	}
	results = dhd_dev_pno_get_gscan(bcmcfg_to_prmry_ndev(cfg),
	             DHD_PNO_GET_BATCH_RESULTS, NULL, &reply_len);

	if (!results) {
		WL_ERR(("No results to send %d\n", err));
		err =  wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		        results, 0);

		if (unlikely(err))
			WL_ERR(("Vendor Command reply failed ret:%d \n", err));
		dhd_dev_pno_unlock_access_batch_results(bcmcfg_to_prmry_ndev(cfg));
		return err;
	}
	num_scan_ids = reply_len & 0xFFFF;
	num_results = (reply_len & 0xFFFF0000) >> 16;
	mem_needed = (num_results * sizeof(wifi_gscan_result_t)) +
	             (num_scan_ids * GSCAN_BATCH_RESULT_HDR_LEN) +
	             VENDOR_REPLY_OVERHEAD + SCAN_RESULTS_COMPLETE_FLAG_LEN;

	if (mem_needed > (int32)NLMSG_DEFAULT_SIZE) {
		mem_needed = (int32)NLMSG_DEFAULT_SIZE;
	}

	WL_TRACE(("is_done %d mem_needed %d max_mem %d\n", is_done, mem_needed,
		(int)NLMSG_DEFAULT_SIZE));
	/* Alloc the SKB for vendor_event */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
	if (unlikely(!skb)) {
		WL_ERR(("skb alloc failed"));
		dhd_dev_pno_unlock_access_batch_results(bcmcfg_to_prmry_ndev(cfg));
		return -ENOMEM;
	}
	iter = results;
	complete_flag = nla_reserve(skb, GSCAN_ATTRIBUTE_SCAN_RESULTS_COMPLETE,
	                    sizeof(is_done));
	mem_needed = mem_needed - (SCAN_RESULTS_COMPLETE_FLAG_LEN + VENDOR_REPLY_OVERHEAD);

	while (iter) {
		num_results_iter =
		    (mem_needed - (int32)GSCAN_BATCH_RESULT_HDR_LEN);
		num_results_iter /= (int32)sizeof(wifi_gscan_result_t);
		if (num_results_iter <= 0 ||
		    ((iter->tot_count - iter->tot_consumed) > num_results_iter)) {
			break;
		}
		scan_hdr = nla_nest_start(skb, GSCAN_ATTRIBUTE_SCAN_RESULTS);
		/* no more room? we are done then (for now) */
		if (scan_hdr == NULL) {
			is_done = 0;
			break;
		}
		nla_put_u32(skb, GSCAN_ATTRIBUTE_SCAN_ID, iter->scan_id);
		nla_put_u8(skb, GSCAN_ATTRIBUTE_SCAN_FLAGS, iter->flag);
		num_results_iter = iter->tot_count - iter->tot_consumed;

		nla_put_u32(skb, GSCAN_ATTRIBUTE_NUM_OF_RESULTS, num_results_iter);
		if (num_results_iter) {
			ptr = &iter->results[iter->tot_consumed];
			iter->tot_consumed += num_results_iter;
			nla_put(skb, GSCAN_ATTRIBUTE_SCAN_RESULTS,
			 num_results_iter * sizeof(wifi_gscan_result_t), ptr);
		}
		nla_nest_end(skb, scan_hdr);
		mem_needed -= GSCAN_BATCH_RESULT_HDR_LEN +
		    (num_results_iter * sizeof(wifi_gscan_result_t));
		iter = iter->next;
	}
	/* Returns TRUE if all result consumed */
	is_done = dhd_dev_gscan_batch_cache_cleanup(bcmcfg_to_prmry_ndev(cfg));
	memcpy(nla_data(complete_flag), &is_done, sizeof(is_done));
	dhd_dev_pno_unlock_access_batch_results(bcmcfg_to_prmry_ndev(cfg));
	return cfg80211_vendor_cmd_reply(skb);
}

static int
wl_cfgvendor_initiate_gscan(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int type, tmp = len;
	int run = 0xFF;
	int flush = 0;
	const struct nlattr *iter;

	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);
		if (type == GSCAN_ATTRIBUTE_ENABLE_FEATURE)
			run = nla_get_u32(iter);
		else if (type == GSCAN_ATTRIBUTE_FLUSH_FEATURE)
			flush = nla_get_u32(iter);
	}

	if (run != 0xFF) {
		err = dhd_dev_pno_run_gscan(bcmcfg_to_prmry_ndev(cfg), run, flush);

		if (unlikely(err)) {
			WL_ERR(("Could not run gscan:%d \n", err));
		}
		return err;
	} else {
		return -EINVAL;
	}


}

static int
wl_cfgvendor_enable_full_scan_result(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int type;
	bool real_time = FALSE;

	type = nla_type(data);

	if (type == GSCAN_ATTRIBUTE_ENABLE_FULL_SCAN_RESULTS) {
		real_time = nla_get_u32(data);

		err = dhd_dev_pno_enable_full_scan_result(bcmcfg_to_prmry_ndev(cfg), real_time);

		if (unlikely(err)) {
			WL_ERR(("Could not run gscan:%d \n", err));
		}

	} else {
		err = -EINVAL;
	}

	return err;
}

static int
wl_cfgvendor_set_scan_cfg_bucket(const struct nlattr *prev,
	gscan_scan_params_t *scan_param, int num)
{
	struct dhd_pno_gscan_channel_bucket  *ch_bucket;
	int k = 0;
	int type, err = 0, rem;
	const struct nlattr *cur, *next;

	nla_for_each_nested(cur, prev, rem) {
		type = nla_type(cur);
		ch_bucket = scan_param->channel_bucket;
		switch (type) {
		case GSCAN_ATTRIBUTE_BUCKET_ID:
			break;
		case GSCAN_ATTRIBUTE_BUCKET_PERIOD:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}

			ch_bucket[num].bucket_freq_multiple =
				nla_get_u32(cur) / MSEC_PER_SEC;
			break;
		case GSCAN_ATTRIBUTE_BUCKET_NUM_CHANNELS:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}
			ch_bucket[num].num_channels = nla_get_u32(cur);
			if (ch_bucket[num].num_channels >
				GSCAN_MAX_CHANNELS_IN_BUCKET) {
				WL_ERR(("channel range:%d,bucket:%d\n",
					ch_bucket[num].num_channels,
					num));
				err = -EINVAL;
				goto exit;
			}
			break;
		case GSCAN_ATTRIBUTE_BUCKET_CHANNELS:
			nla_for_each_nested(next, cur, rem) {
				if (k >= GSCAN_MAX_CHANNELS_IN_BUCKET)
					break;
				if (nla_len(next) != sizeof(uint32)) {
					err = -EINVAL;
					goto exit;
				}
				ch_bucket[num].chan_list[k] = nla_get_u32(next);
				k++;
			}
			break;
		case GSCAN_ATTRIBUTE_BUCKETS_BAND:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}
			ch_bucket[num].band = (uint16)nla_get_u32(cur);
			break;
		case GSCAN_ATTRIBUTE_REPORT_EVENTS:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}
			ch_bucket[num].report_flag = (uint8)nla_get_u32(cur);
			break;
		case GSCAN_ATTRIBUTE_BUCKET_STEP_COUNT:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}
			ch_bucket[num].repeat = (uint16)nla_get_u32(cur);
			break;
		case GSCAN_ATTRIBUTE_BUCKET_MAX_PERIOD:
			if (nla_len(cur) != sizeof(uint32)) {
				err = -EINVAL;
				goto exit;
			}
			ch_bucket[num].bucket_max_multiple =
				nla_get_u32(cur) / MSEC_PER_SEC;
			break;
		default:
			WL_ERR(("unknown attr type:%d\n", type));
			err = -EINVAL;
			goto exit;
		}
	}

exit:
	return err;
}

static int
wl_cfgvendor_set_scan_cfg(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	gscan_scan_params_t *scan_param;
	int j = 0;
	int type, tmp;
	const struct nlattr *iter;

	scan_param = kzalloc(sizeof(gscan_scan_params_t), GFP_KERNEL);
	if (!scan_param) {
		WL_ERR(("Could not set GSCAN scan cfg, mem alloc failure\n"));
		err = -EINVAL;
		return err;

	}

	scan_param->scan_fr = PNO_SCAN_MIN_FW_SEC;
	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);

		if (j >= GSCAN_MAX_CH_BUCKETS) {
			break;
		}

		switch (type) {
			case GSCAN_ATTRIBUTE_BASE_PERIOD:
				if (nla_len(iter) != sizeof(uint32)) {
					err = -EINVAL;
					goto exit;
				}
				scan_param->scan_fr = nla_get_u32(iter) / MSEC_PER_SEC;
				break;
			case GSCAN_ATTRIBUTE_NUM_BUCKETS:
				if (nla_len(iter) != sizeof(uint32)) {
					err = -EINVAL;
					goto exit;
				}
				scan_param->nchannel_buckets = nla_get_u32(iter);
				if (scan_param->nchannel_buckets >=
				    GSCAN_MAX_CH_BUCKETS) {
					WL_ERR(("ncha_buck out of range %d\n",
					scan_param->nchannel_buckets));
					err = -EINVAL;
					goto exit;
				}
				break;
			case GSCAN_ATTRIBUTE_CH_BUCKET_1:
			case GSCAN_ATTRIBUTE_CH_BUCKET_2:
			case GSCAN_ATTRIBUTE_CH_BUCKET_3:
			case GSCAN_ATTRIBUTE_CH_BUCKET_4:
			case GSCAN_ATTRIBUTE_CH_BUCKET_5:
			case GSCAN_ATTRIBUTE_CH_BUCKET_6:
			case GSCAN_ATTRIBUTE_CH_BUCKET_7:
				err = wl_cfgvendor_set_scan_cfg_bucket(iter, scan_param, j);
				if (err < 0) {
					WL_ERR(("set_scan_cfg_buck error:%d\n", err));
					goto exit;
				}
				j++;
				break;
			default:
				WL_ERR(("Unknown type %d\n", type));
				err = -EINVAL;
				goto exit;
		}
	}

	err = dhd_dev_pno_set_cfg_gscan(bcmcfg_to_prmry_ndev(cfg),
	     DHD_PNO_SCAN_CFG_ID, scan_param, FALSE);

	if (err < 0) {
		WL_ERR(("Could not set GSCAN scan cfg\n"));
		err = -EINVAL;
	}

exit:
	kfree(scan_param);
	return err;

}

static int
wl_cfgvendor_hotlist_cfg(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	gscan_hotlist_scan_params_t *hotlist_params;
	int tmp, tmp1, tmp2, type, j = 0, dummy;
	const struct nlattr *outer, *inner = NULL, *iter;
	uint8 flush = 0;
	struct bssid_t *pbssid;

	if (len < sizeof(*hotlist_params) || len >= WLC_IOCTL_MAXLEN) {
		WL_ERR(("buffer length :%d wrong - bail out.\n", len));
		return -EINVAL;
	}

	hotlist_params = kzalloc(sizeof(*hotlist_params)
		+ (sizeof(struct bssid_t) * (PFN_SWC_MAX_NUM_APS - 1)),
		GFP_KERNEL);

	if (!hotlist_params) {
		WL_ERR(("Cannot Malloc memory.\n"));
		return -ENOMEM;
	}

	hotlist_params->lost_ap_window = GSCAN_LOST_AP_WINDOW_DEFAULT;

	nla_for_each_attr(iter, data, len, tmp2) {
		type = nla_type(iter);
		switch (type) {
		case GSCAN_ATTRIBUTE_HOTLIST_BSSIDS:
			pbssid = hotlist_params->bssid;
			nla_for_each_nested(outer, iter, tmp) {
				nla_for_each_nested(inner, outer, tmp1) {
					type = nla_type(inner);

					switch (type) {
					case GSCAN_ATTRIBUTE_BSSID:
						if (nla_len(inner) != sizeof(pbssid[j].macaddr)) {
							WL_ERR(("type:%d length:%d not matching.\n",
								type, nla_len(inner)));
							err = -EINVAL;
							goto exit;
						}
						memcpy(
							&(pbssid[j].macaddr),
							nla_data(inner),
							sizeof(pbssid[j].macaddr));
						break;
					case GSCAN_ATTRIBUTE_RSSI_LOW:
						if (nla_len(inner) != sizeof(uint8)) {
							WL_ERR(("type:%d length:%d not matching.\n",
								type, nla_len(inner)));
							err = -EINVAL;
							goto exit;
						}
						pbssid[j].rssi_reporting_threshold =
							(int8)nla_get_u8(inner);
						break;
					case GSCAN_ATTRIBUTE_RSSI_HIGH:
						if (nla_len(inner) != sizeof(uint8)) {
							WL_ERR(("type:%d length:%d not matching.\n",
								type, nla_len(inner)));
							err = -EINVAL;
							goto exit;
						}
						dummy = (int8)nla_get_u8(inner);
						break;
					default:
						WL_ERR(("ATTR unknown %d\n", type));
						err = -EINVAL;
						goto exit;
					}
				}
				if (++j >= PFN_SWC_MAX_NUM_APS) {
					WL_ERR(("cap hotlist max:%d\n", j));
					break;
				}
			}
			hotlist_params->nbssid = j;
			break;
		case GSCAN_ATTRIBUTE_HOTLIST_FLUSH:
			if (nla_len(iter) != sizeof(uint8)) {
				WL_ERR(("type:%d length:%d not matching.\n",
					type, nla_len(inner)));
				err = -EINVAL;
				goto exit;
			}
			flush = nla_get_u8(iter);
			break;
		case GSCAN_ATTRIBUTE_LOST_AP_SAMPLE_SIZE:
			if (nla_len(iter) != sizeof(uint32)) {
				WL_ERR(("type:%d length:%d not matching.\n",
					type, nla_len(inner)));
				err = -EINVAL;
				goto exit;
			}
			hotlist_params->lost_ap_window = (uint16)nla_get_u32(iter);
			break;
		default:
			WL_ERR(("Unknown type %d\n", type));
			err = -EINVAL;
			goto exit;
		}

	}

	if (dhd_dev_pno_set_cfg_gscan(bcmcfg_to_prmry_ndev(cfg),
	      DHD_PNO_GEOFENCE_SCAN_CFG_ID, hotlist_params, flush) < 0) {
		WL_ERR(("Could not set GSCAN HOTLIST cfg error: %d\n", err));
		err = -EINVAL;
		goto exit;
	}
exit:
	kfree(hotlist_params);
	return err;
}
static int
wl_cfgvendor_set_batch_scan_cfg(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0, tmp, type;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	gscan_batch_params_t batch_param;
	const struct nlattr *iter;

	batch_param.mscan = batch_param.bestn = 0;
	batch_param.buffer_threshold = GSCAN_BATCH_NO_THR_SET;

	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);

		switch (type) {
			case GSCAN_ATTRIBUTE_NUM_AP_PER_SCAN:
				batch_param.bestn = nla_get_u32(iter);
				break;
			case GSCAN_ATTRIBUTE_NUM_SCANS_TO_CACHE:
				batch_param.mscan = nla_get_u32(iter);
				break;
			case GSCAN_ATTRIBUTE_REPORT_THRESHOLD:
				batch_param.buffer_threshold = nla_get_u32(iter);
				break;
			default:
				WL_ERR(("Unknown type %d\n", type));
				break;
		}
	}

	if (dhd_dev_pno_set_cfg_gscan(bcmcfg_to_prmry_ndev(cfg),
	       DHD_PNO_BATCH_SCAN_CFG_ID, &batch_param, 0) < 0) {
		WL_ERR(("Could not set batch cfg\n"));
		err = -EINVAL;
		return err;
	}

	return err;
}

static int
wl_cfgvendor_significant_change_cfg(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	gscan_swc_params_t *significant_params;
	int tmp, tmp1, tmp2, type, j = 0;
	const struct nlattr *outer, *inner, *iter;
	uint8 flush = 0;
	wl_pfn_significant_bssid_t *pbssid;

	significant_params = (gscan_swc_params_t *) kzalloc(len, GFP_KERNEL);
	if (!significant_params) {
		WL_ERR(("Cannot Malloc mem to parse config commands size - %d bytes \n", len));
		return -ENOMEM;
	}


	nla_for_each_attr(iter, data, len, tmp2) {
		type = nla_type(iter);

		switch (type) {
			case GSCAN_ATTRIBUTE_SIGNIFICANT_CHANGE_FLUSH:
			flush = nla_get_u8(iter);
			break;
			case GSCAN_ATTRIBUTE_RSSI_SAMPLE_SIZE:
				significant_params->rssi_window = nla_get_u16(iter);
				break;
			case GSCAN_ATTRIBUTE_LOST_AP_SAMPLE_SIZE:
				significant_params->lost_ap_window = nla_get_u16(iter);
				break;
			case GSCAN_ATTRIBUTE_MIN_BREACHING:
				significant_params->swc_threshold = nla_get_u16(iter);
				break;
			case GSCAN_ATTRIBUTE_SIGNIFICANT_CHANGE_BSSIDS:
				pbssid = significant_params->bssid_elem_list;
				nla_for_each_nested(outer, iter, tmp) {
					nla_for_each_nested(inner, outer, tmp1) {
							switch (nla_type(inner)) {
								case GSCAN_ATTRIBUTE_BSSID:
								memcpy(&(pbssid[j].macaddr),
								     nla_data(inner),
								     ETHER_ADDR_LEN);
								break;
								case GSCAN_ATTRIBUTE_RSSI_HIGH:
								pbssid[j].rssi_high_threshold =
								       (int8) nla_get_u8(inner);
								break;
								case GSCAN_ATTRIBUTE_RSSI_LOW:
								pbssid[j].rssi_low_threshold =
								      (int8) nla_get_u8(inner);
								break;
								default:
									WL_ERR(("ATTR unknown %d\n",
									          type));
									break;
							}
						}
					j++;
				}
				break;
			default:
				WL_ERR(("Unknown type %d\n", type));
				break;
		}
	}
	significant_params->nbssid = j;

	if (dhd_dev_pno_set_cfg_gscan(bcmcfg_to_prmry_ndev(cfg),
	              DHD_PNO_SIGNIFICANT_SCAN_CFG_ID,
	              significant_params, flush) < 0) {
		WL_ERR(("Could not set GSCAN significant cfg\n"));
		err = -EINVAL;
		goto exit;
	}
exit:
	kfree(significant_params);
	return err;
}
#endif /* GSCAN_SUPPORT */

#ifdef RSSI_MONITOR_SUPPORT
static int wl_cfgvendor_set_rssi_monitor(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int err = 0, tmp, type, start = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int8 max_rssi = 0, min_rssi = 0;
	const struct nlattr *iter;

	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);
		switch (type) {
			case RSSI_MONITOR_ATTRIBUTE_MAX_RSSI:
				max_rssi = (int8) nla_get_u32(iter);
				break;
			case RSSI_MONITOR_ATTRIBUTE_MIN_RSSI:
				min_rssi = (int8) nla_get_u32(iter);
				break;
			case RSSI_MONITOR_ATTRIBUTE_START:
				start = nla_get_u32(iter);
		}
	}

	if (dhd_dev_set_rssi_monitor_cfg(bcmcfg_to_prmry_ndev(cfg),
	       start, max_rssi, min_rssi) < 0) {
		WL_ERR(("Could not set rssi monitor cfg\n"));
		err = -EINVAL;
	}
	return err;
}
#endif /* RSSI_MONITOR_SUPPORT */

#ifdef RTT_SUPPORT
void
wl_cfgvendor_rtt_evt(void *ctx, void *rtt_data)
{
	struct wireless_dev *wdev = (struct wireless_dev *)ctx;
	struct wiphy *wiphy;
	struct sk_buff *skb;
	uint32 evt_complete = 0;
	gfp_t kflags;
	rtt_result_t *rtt_result;
	rtt_results_header_t *rtt_header;
	struct list_head *rtt_cache_list;
	struct nlattr *rtt_nl_hdr;
	wiphy = wdev->wiphy;

	WL_DBG(("In\n"));
	/* Push the data to the skb */
	if (!rtt_data) {
		WL_ERR(("rtt_data is NULL\n"));
		return;
	}
	rtt_cache_list = (struct list_head *)rtt_data;
	kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	if (list_empty(rtt_cache_list)) {
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
		skb = cfg80211_vendor_event_alloc(wiphy, NULL, 100,
			GOOGLE_RTT_COMPLETE_EVENT, kflags);
#else
		skb = cfg80211_vendor_event_alloc(wiphy, 100, GOOGLE_RTT_COMPLETE_EVENT, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
		if (!skb) {
			WL_ERR(("skb alloc failed"));
			return;
		}
		evt_complete = 1;
		nla_put_u32(skb, RTT_ATTRIBUTE_RESULTS_COMPLETE, evt_complete);
		cfg80211_vendor_event(skb, kflags);
		return;
	}
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
	list_for_each_entry(rtt_header, rtt_cache_list, list) {
		/* Alloc the SKB for vendor_event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
		skb = cfg80211_vendor_event_alloc(wiphy, NULL, rtt_header->result_tot_len + 100,
			GOOGLE_RTT_COMPLETE_EVENT, kflags);
#else
		skb = cfg80211_vendor_event_alloc(wiphy, rtt_header->result_tot_len + 100,
			GOOGLE_RTT_COMPLETE_EVENT, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
		if (!skb) {
			WL_ERR(("skb alloc failed"));
			return;
		}
		if (list_is_last(&rtt_header->list, rtt_cache_list)) {
			evt_complete = 1;
		}
		nla_put_u32(skb, RTT_ATTRIBUTE_RESULTS_COMPLETE, evt_complete);
		rtt_nl_hdr = nla_nest_start(skb, RTT_ATTRIBUTE_RESULTS_PER_TARGET);
		if (!rtt_nl_hdr) {
			WL_ERR(("rtt_nl_hdr is NULL\n"));
			break;
		}
		nla_put(skb, RTT_ATTRIBUTE_TARGET_MAC, ETHER_ADDR_LEN, &rtt_header->peer_mac);
		nla_put_u32(skb, RTT_ATTRIBUTE_RESULT_CNT, rtt_header->result_cnt);
		list_for_each_entry(rtt_result, &rtt_header->result_list, list) {
			nla_put(skb, RTT_ATTRIBUTE_RESULT,
				rtt_result->report_len, &rtt_result->report);
		}
		nla_nest_end(skb, rtt_nl_hdr);
		cfg80211_vendor_event(skb, kflags);
	}
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
}

static int
wl_cfgvendor_rtt_set_config(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int len) {
	int err = 0, rem, rem1, rem2, type;
	int target_cnt;
	rtt_config_params_t rtt_param;
	rtt_target_info_t* rtt_target = NULL;
	const struct nlattr *iter, *iter1, *iter2;
	int8 eabuf[ETHER_ADDR_STR_LEN];
	int8 chanbuf[CHANSPEC_STR_LEN];
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	rtt_capabilities_t capability;

	WL_DBG(("In\n"));
	err = dhd_dev_rtt_register_noti_callback(wdev->netdev, wdev, wl_cfgvendor_rtt_evt);
	if (err < 0) {
		WL_ERR(("failed to register rtt_noti_callback\n"));
		goto exit;
	}
	err = dhd_dev_rtt_capability(bcmcfg_to_prmry_ndev(cfg), &capability);
	if (err < 0) {
		WL_ERR(("failed to get the capability\n"));
		goto exit;
	}

	memset(&rtt_param, 0, sizeof(rtt_param));
	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
		case RTT_ATTRIBUTE_TARGET_CNT:
			target_cnt = nla_get_u8(iter);
			if (rtt_param.rtt_target_cnt > RTT_MAX_TARGET_CNT) {
				WL_ERR(("exceed max target count : %d\n",
					target_cnt));
				err = BCME_RANGE;
				goto exit;
			}
			rtt_param.rtt_target_cnt = target_cnt;
			rtt_param.target_info = kzalloc(TARGET_INFO_SIZE(target_cnt), GFP_KERNEL);
			if (rtt_param.target_info == NULL) {
				WL_ERR(("failed to allocate target info for (%d)\n", target_cnt));
				err = BCME_NOMEM;
				goto exit;
			}
			break;
		case RTT_ATTRIBUTE_TARGET_INFO:
			rtt_target = rtt_param.target_info;
			nla_for_each_nested(iter1, iter, rem1) {
				nla_for_each_nested(iter2, iter1, rem2) {
					type = nla_type(iter2);
					switch (type) {
					case RTT_ATTRIBUTE_TARGET_MAC:
						memcpy(&rtt_target->addr, nla_data(iter2),
							ETHER_ADDR_LEN);
						break;
					case RTT_ATTRIBUTE_TARGET_TYPE:
						rtt_target->type = nla_get_u8(iter2);
						if (rtt_target->type == RTT_INVALID ||
							(rtt_target->type == RTT_ONE_WAY &&
							!capability.rtt_one_sided_supported)) {
							WL_ERR(("doesn't support RTT type"
								" : %d\n",
								rtt_target->type));
							err = -EINVAL;
							goto exit;
						}
						break;
					case RTT_ATTRIBUTE_TARGET_PEER:
						rtt_target->peer = nla_get_u8(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_CHAN:
						memcpy(&rtt_target->channel, nla_data(iter2),
							sizeof(rtt_target->channel));
						break;
					case RTT_ATTRIBUTE_TARGET_PERIOD:
						rtt_target->burst_period = nla_get_u32(iter2);
						if (rtt_target->burst_period < 32) {
							/* 100ms unit */
							rtt_target->burst_period *= 100;
						} else {
							WL_ERR(("%d value must in (0-31)\n",
								rtt_target->burst_period));
							err = EINVAL;
							goto exit;
						}
						break;
					case RTT_ATTRIBUTE_TARGET_NUM_BURST:
						rtt_target->num_burst = nla_get_u32(iter2);
						if (rtt_target->num_burst > 16) {
							WL_ERR(("%d value must in (0-15)\n",
								rtt_target->num_burst));
							err = -EINVAL;
							goto exit;
						}
						rtt_target->num_burst = BIT(rtt_target->num_burst);
						break;
					case RTT_ATTRIBUTE_TARGET_NUM_FTM_BURST:
						rtt_target->num_frames_per_burst =
						nla_get_u32(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_NUM_RETRY_FTM:
						rtt_target->num_retries_per_ftm =
						nla_get_u32(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_NUM_RETRY_FTMR:
						rtt_target->num_retries_per_ftmr =
						nla_get_u32(iter2);
						if (rtt_target->num_retries_per_ftmr > 3) {
							WL_ERR(("%d value must in (0-3)\n",
								rtt_target->num_retries_per_ftmr));
							err = -EINVAL;
							goto exit;
						}
						break;
					case RTT_ATTRIBUTE_TARGET_LCI:
						rtt_target->LCI_request = nla_get_u8(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_LCR:
						rtt_target->LCI_request = nla_get_u8(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_BURST_DURATION:
						if ((nla_get_u32(iter2) > 1 &&
							nla_get_u32(iter2) < 12)) {
							rtt_target->burst_duration =
							dhd_rtt_idx_to_burst_duration(
								nla_get_u32(iter2));
						} else if (nla_get_u32(iter2) == 15) {
							/* use default value */
							rtt_target->burst_duration = 0;
						} else {
							WL_ERR(("%d value must in (2-11) or 15\n",
								nla_get_u32(iter2)));
							err = -EINVAL;
							goto exit;
						}
						break;
					case RTT_ATTRIBUTE_TARGET_BW:
						rtt_target->bw = nla_get_u8(iter2);
						break;
					case RTT_ATTRIBUTE_TARGET_PREAMBLE:
						rtt_target->preamble = nla_get_u8(iter2);
						break;
					}
				}
				/* convert to chanspec value */
				rtt_target->chanspec =
					dhd_rtt_convert_to_chspec(rtt_target->channel);
				if (rtt_target->chanspec == 0) {
					WL_ERR(("Channel is not valid \n"));
					err = -EINVAL;
					goto exit;
				}
				WL_INFORM(("Target addr %s, Channel : %s for RTT \n",
					bcm_ether_ntoa((const struct ether_addr *)&rtt_target->addr,
					eabuf),
					wf_chspec_ntoa(rtt_target->chanspec, chanbuf)));
				rtt_target++;
			}
			break;
		}
	}
	WL_DBG(("leave :target_cnt : %d\n", rtt_param.rtt_target_cnt));
	if (dhd_dev_rtt_set_cfg(bcmcfg_to_prmry_ndev(cfg), &rtt_param) < 0) {
		WL_ERR(("Could not set RTT configuration\n"));
		err = -EINVAL;
	}
exit:
	/* free the target info list */
	kfree(rtt_param.target_info);
	return err;
}

static int
wl_cfgvendor_rtt_cancel_config(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int len)
{
	int err = 0, rem, type, target_cnt = 0;
	int target_cnt_chk = 0;
	const struct nlattr *iter;
	struct ether_addr *mac_list = NULL, *mac_addr = NULL;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);

	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
		case RTT_ATTRIBUTE_TARGET_CNT:
			if (mac_list != NULL) {
				WL_ERR(("mac_list is not NULL\n"));
				goto exit;
			}
			target_cnt = nla_get_u8(iter);
			if (target_cnt > 0) {
				mac_list = (struct ether_addr *)kzalloc(target_cnt * ETHER_ADDR_LEN,
					GFP_KERNEL);
				if (mac_list == NULL) {
					WL_ERR(("failed to allocate mem for mac list\n"));
					goto exit;
				}
				mac_addr = &mac_list[0];
			} else {
				/* cancel the current whole RTT process */
				goto cancel;
			}
			break;
		case RTT_ATTRIBUTE_TARGET_MAC:
			if (mac_addr) {
				memcpy(mac_addr++, nla_data(iter), ETHER_ADDR_LEN);
				target_cnt_chk++;
				if (target_cnt_chk > target_cnt) {
					WL_ERR(("over target count\n"));
					goto exit;
				}
				break;
			} else {
				WL_ERR(("mac_list is NULL\n"));
				goto exit;
			}
		}
	}
cancel:
	if (dhd_dev_rtt_cancel_cfg(bcmcfg_to_prmry_ndev(cfg), mac_list, target_cnt) < 0) {
		WL_ERR(("Could not cancel RTT configuration\n"));
		err = -EINVAL;
	}

exit:
	if (mac_list) {
		kfree(mac_list);
	}
	return err;
}

static int
wl_cfgvendor_rtt_get_capability(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int len)
{
	int err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	rtt_capabilities_t capability;

	err = dhd_dev_rtt_capability(bcmcfg_to_prmry_ndev(cfg), &capability);
	if (unlikely(err)) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));
		goto exit;
	}
	err =  wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        &capability, sizeof(capability));

	if (unlikely(err)) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));
	}
exit:
	return err;
}

#endif /* RTT_SUPPORT */

#if defined(BCMDONGLEHOST)
static int
wl_cfgvendor_priv_string_handler(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{

	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int ret = 0;
	int ret_len = 0, payload = 0, msglen;
	const struct bcm_nlmsg_hdr *nlioc = data;
	void *buf = NULL, *cur;
	int maxmsglen = PAGE_SIZE - 0x100;
	struct sk_buff *reply;

	WL_ERR(("entry: cmd = %d\n", nlioc->cmd));

	len -= sizeof(struct bcm_nlmsg_hdr);
	ret_len = nlioc->len;
	if (ret_len > 0 || len > 0) {
		if (len > DHD_IOCTL_MAXLEN) {
			WL_ERR(("oversize input buffer %d\n", len));
			len = DHD_IOCTL_MAXLEN;
		}
		if (ret_len > DHD_IOCTL_MAXLEN) {
			WL_ERR(("oversize return buffer %d\n", ret_len));
			ret_len = DHD_IOCTL_MAXLEN;
		}
		payload = max(ret_len, len) + 1;
		buf = vzalloc(payload);
		if (!buf) {
			return -ENOMEM;
		}
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
		memcpy(buf, (void *)((char *)nlioc + nlioc->offset), len);
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
		*((char *)buf + len) = '\0';
	}

	ret = dhd_cfgvendor_priv_string_handler(cfg, wdev, nlioc, buf);
	if (ret) {
		WL_ERR(("dhd_cfgvendor returned error %d", ret));
		vfree(buf);
		return ret;
	}
	cur = buf;
	while (ret_len > 0) {
		msglen = nlioc->len > maxmsglen ? maxmsglen : ret_len;
		ret_len -= msglen;
		payload = msglen + sizeof(msglen);
		reply = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, payload);
		if (!reply) {
			WL_ERR(("Failed to allocate reply msg\n"));
			ret = -ENOMEM;
			break;
		}

		if (nla_put(reply, BCM_NLATTR_DATA, msglen, cur) ||
			nla_put_u16(reply, BCM_NLATTR_LEN, msglen)) {
			kfree_skb(reply);
			ret = -ENOBUFS;
			break;
		}

		ret = cfg80211_vendor_cmd_reply(reply);
		if (ret) {
			WL_ERR(("testmode reply failed:%d\n", ret));
			break;
		}
		cur = (void *)((char *)cur + msglen);
	}

	return ret;
}
#endif /* defined(BCMDONGLEHOST) */

#ifdef OEM_ANDROID
static struct net_device *
wl_cfgvendor_get_ndev(struct bcm_cfg80211 *cfg, struct wireless_dev *wdev,
	const void *data, unsigned long int *out_addr)
{
	char *pos, *pos1;
	char ifname[IFNAMSIZ + 1] = {0};
	struct net_info *iter, *next;
	struct net_device *ndev = NULL;
	*out_addr = (unsigned long int) data; /* point to command str by default */

	/* check whether ifname=<ifname> is provided in the command */
	pos = strstr(data, "ifname=");
	if (pos) {
		pos += strlen("ifname=");
		pos1 = strstr(pos, " ");
		if (!pos1) {
			WL_ERR(("command format error \n"));
			return NULL;
		}
		memcpy(ifname, pos, (pos1 - pos));
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
		for_each_ndev(cfg, iter, next) {
			if (iter->ndev) {
				if (strncmp(iter->ndev->name, ifname,
					strlen(iter->ndev->name)) == 0) {
					/* matching ifname found */
					WL_DBG(("matching interface (%s) found ndev:%p \n",
						iter->ndev->name, iter->ndev));
					*out_addr = (unsigned long int)(pos1 + 1);
					/* Returns the command portion after ifname=<name> */
					return iter->ndev;
				}
			}

		}
#if defined(STRICT_GCC_WARNINGS) && defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
		WL_ERR(("Couldn't find ifname:%s in the netinfo list \n",
			ifname));
		return NULL;
	}

	/* If ifname=<name> arg is not provided, use default ndev */
	ndev = wdev->netdev ? wdev->netdev : bcmcfg_to_prmry_ndev(cfg);
	WL_DBG(("Using default ndev (%s) \n", ndev->name));
	return ndev;
}
#endif /* OEM_ANDROID */

/* Max length for the reply buffer. For BRCM_ATTR_DRIVER_CMD, the reply
 * would be a formatted string and reply buf would be the size of the
 * string.
 */
#define WL_DRIVER_PRIV_CMD_LEN 512
static int
wl_cfgvendor_priv_bcm_handler(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	const struct nlattr *iter;
	int err = 0;
	int data_len = 0, cmd_len = 0, tmp = 0, type = 0;
#if defined(BCMDONGLEHOST)
	struct net_device *ndev = wdev->netdev;
#endif /* defined(BCMDONGLEHOST) */
	char *reply_buf = NULL;
	char *cmd = NULL;
#ifdef OEM_ANDROID
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int bytes_written;
	struct net_device *net = NULL;
	unsigned long int cmd_out = 0;
	u32 reply_len = WL_DRIVER_PRIV_CMD_LEN;
#endif /* OEM_ANDROID */


	WL_DBG(("%s: Enter \n", __func__));

#if defined(BCMDONGLEHOST)
	/* hold wake lock */
	net_os_wake_lock(ndev);
#endif /* defined(BCMDONGLEHOST) */

	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);
		cmd = nla_data(iter);
		cmd_len = nla_len(iter);

		WL_DBG(("%s: type: %d cmd_len:%d cmd_ptr:%p \n", __func__, type, cmd_len, cmd));
		if (!cmd || !cmd_len) {
			WL_ERR(("Invalid cmd data \n"));
			err = -EINVAL;
			goto exit;
		}

#ifdef OEM_ANDROID
		if (type == BRCM_ATTR_DRIVER_CMD) {
			if (cmd_len >= WL_DRIVER_PRIV_CMD_LEN) {
				WL_ERR(("Unexpected command length. Ignore the command\n"));
				err = -EINVAL;
				goto exit;
			}
			net = wl_cfgvendor_get_ndev(cfg, wdev, cmd, &cmd_out);
			if (!cmd_out || !net) {
				err = -ENODEV;
				goto exit;
			}
			cmd = (char *)cmd_out;
			reply_buf = kzalloc(reply_len, GFP_KERNEL);
			if (!reply_buf) {
				WL_ERR(("memory alloc failed for %u \n", cmd_len));
				err = -ENOMEM;
				goto exit;
			}
			memcpy(reply_buf, cmd, cmd_len);
			WL_DBG(("vendor_command: %s len: %u \n", cmd, cmd_len));
			bytes_written = wl_handle_private_cmd(net, reply_buf, reply_len);
			WL_DBG(("bytes_written: %d \n", bytes_written));
			if (bytes_written == 0) {
				snprintf(reply_buf, reply_len, "%s", "OK");
				data_len = strlen("OK");
			} else if (bytes_written > 0) {
				data_len = bytes_written > reply_len ?
					reply_len : bytes_written;
			} else {
				/* -ve return value. Propagate the error back */
				err = bytes_written;
				goto exit;
			}
			break;
		}
#endif /* OEM_ANDROID */
	}

#if defined(OEM_ANDROID) && (defined(BCMDONGLEHOST) || defined(PLATFORM_INTEGRATED_WIFI))
	if ((data_len > 0) && reply_buf) {
		err =  wl_cfgvendor_send_cmd_reply(wiphy, wdev->netdev,
			reply_buf, data_len+1);
		if (unlikely(err))
			WL_ERR(("Vendor Command reply failed ret:%d \n", err));
		else
			WL_DBG(("Vendor Command reply sent successfully!\n"));
	} else {
#endif /* OEM_ANDROID && (BCMDONGLEHOST || PLATFORM_INTEGRATED_WIFI) */
		/* No data to be sent back as reply */
		WL_ERR(("Vendor_cmd: No reply expected. data_len:%u reply_buf %p \n",
			data_len, reply_buf));
#if defined(OEM_ANDROID) && (defined(BCMDONGLEHOST) || defined(PLATFORM_INTEGRATED_WIFI))
	}
#endif /* OEM_ANDROID && (BCMDONGLEHOST || PLATFORM_INTEGRATED_WIFI) */

exit:
#ifdef OEM_ANDROID
	if (reply_buf)
		kfree(reply_buf);
#endif /* OEM_ANDROID */
#if defined(BCMDONGLEHOST)
	net_os_wake_unlock(ndev);
#endif /* defined(BCMDONGLEHOST) */
	return err;
}

#ifdef WL_NAN
static const char *nan_attr_to_str(u16 cmd)
{
	switch (cmd) {
	C2S(NAN_ATTRIBUTE_HEADER)
	C2S(NAN_ATTRIBUTE_HANDLE)
	C2S(NAN_ATTRIBUTE_TRANSAC_ID)
	C2S(NAN_ATTRIBUTE_5G_SUPPORT)
	C2S(NAN_ATTRIBUTE_CLUSTER_LOW)
	C2S(NAN_ATTRIBUTE_CLUSTER_HIGH)
	C2S(NAN_ATTRIBUTE_SID_BEACON)
	C2S(NAN_ATTRIBUTE_SYNC_DISC_5G)
	C2S(NAN_ATTRIBUTE_RSSI_CLOSE)
	C2S(NAN_ATTRIBUTE_RSSI_MIDDLE)
	C2S(NAN_ATTRIBUTE_RSSI_PROXIMITY)
	C2S(NAN_ATTRIBUTE_HOP_COUNT_LIMIT)
	C2S(NAN_ATTRIBUTE_RANDOM_TIME)
	C2S(NAN_ATTRIBUTE_MASTER_PREF)
	C2S(NAN_ATTRIBUTE_PERIODIC_SCAN_INTERVAL)
	C2S(NAN_ATTRIBUTE_PUBLISH_ID)
	C2S(NAN_ATTRIBUTE_TTL)
	C2S(NAN_ATTRIBUTE_PERIOD)
	C2S(NAN_ATTRIBUTE_REPLIED_EVENT_FLAG)
	C2S(NAN_ATTRIBUTE_PUBLISH_TYPE)
	C2S(NAN_ATTRIBUTE_TX_TYPE)
	C2S(NAN_ATTRIBUTE_PUBLISH_COUNT)
	C2S(NAN_ATTRIBUTE_SERVICE_NAME_LEN)
	C2S(NAN_ATTRIBUTE_SERVICE_NAME)
	C2S(NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN)
	C2S(NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO)
	C2S(NAN_ATTRIBUTE_RX_MATCH_FILTER_LEN)
	C2S(NAN_ATTRIBUTE_RX_MATCH_FILTER)
	C2S(NAN_ATTRIBUTE_TX_MATCH_FILTER_LEN)
	C2S(NAN_ATTRIBUTE_TX_MATCH_FILTER)
	C2S(NAN_ATTRIBUTE_SUBSCRIBE_ID)
	C2S(NAN_ATTRIBUTE_SUBSCRIBE_TYPE)
	C2S(NAN_ATTRIBUTE_SERVICERESPONSEFILTER)
	C2S(NAN_ATTRIBUTE_SERVICERESPONSEINCLUDE)
	C2S(NAN_ATTRIBUTE_USESERVICERESPONSEFILTER)
	C2S(NAN_ATTRIBUTE_SSIREQUIREDFORMATCHINDICATION)
	C2S(NAN_ATTRIBUTE_SUBSCRIBE_MATCH)
	C2S(NAN_ATTRIBUTE_SUBSCRIBE_COUNT)
	C2S(NAN_ATTRIBUTE_MAC_ADDR)
	C2S(NAN_ATTRIBUTE_MAC_ADDR_LIST)
	C2S(NAN_ATTRIBUTE_MAC_ADDR_LIST_NUM_ENTRIES)
	C2S(NAN_ATTRIBUTE_PUBLISH_MATCH)
	C2S(NAN_ATTRIBUTE_ENABLE_STATUS)
	C2S(NAN_ATTRIBUTE_JOIN_STATUS)
	C2S(NAN_ATTRIBUTE_ROLE)
	C2S(NAN_ATTRIBUTE_MASTER_RANK)
	C2S(NAN_ATTRIBUTE_ANCHOR_MASTER_RANK)
	C2S(NAN_ATTRIBUTE_CNT_PEND_TXFRM)
	C2S(NAN_ATTRIBUTE_CNT_BCN_TX)
	C2S(NAN_ATTRIBUTE_CNT_BCN_RX)
	C2S(NAN_ATTRIBUTE_CNT_SVC_DISC_TX)
	C2S(NAN_ATTRIBUTE_CNT_SVC_DISC_RX)
	C2S(NAN_ATTRIBUTE_AMBTT)
	C2S(NAN_ATTRIBUTE_CLUSTER_ID)
	C2S(NAN_ATTRIBUTE_INST_ID)
	C2S(NAN_ATTRIBUTE_OUI)
	C2S(NAN_ATTRIBUTE_STATUS)
	C2S(NAN_ATTRIBUTE_DE_EVENT_TYPE)
	C2S(NAN_ATTRIBUTE_MERGE)
	C2S(NAN_ATTRIBUTE_IFACE)
	C2S(NAN_ATTRIBUTE_CHANNEL)
	C2S(NAN_ATTRIBUTE_PEER_ID)
	C2S(NAN_ATTRIBUTE_NDP_ID)
	C2S(NAN_ATTRIBUTE_SECURITY)
	C2S(NAN_ATTRIBUTE_QOS)
	C2S(NAN_ATTRIBUTE_RSP_CODE)
	C2S(NAN_ATTRIBUTE_INST_COUNT)
	C2S(NAN_ATTRIBUTE_PEER_DISC_MAC_ADDR)
	C2S(NAN_ATTRIBUTE_PEER_NDI_MAC_ADDR)
	C2S(NAN_ATTRIBUTE_IF_ADDR)
	C2S(NAN_ATTRIBUTE_WARMUP_TIME)
	C2S(NAN_ATTRIBUTE_RECV_IND_CFG)
	C2S(NAN_ATTRIBUTE_RSSI_THRESHOLD_FLAG)
	C2S(NAN_ATTRIBUTE_CONNMAP)

	default:
		return "NAN_ATTRIBUTE_UNKNOWN";
	}
}

#define WL_NAN_EVENT_MAX_BUF 256
int8 chanbuf[CHANSPEC_STR_LEN];
static int wl_cfgvendor_nan_parse_args(struct wiphy *wiphy,
	const void *buf, int len, nan_cmd_data_t *cmd_data)
{
	int ret = BCME_OK;
	int attr_type;
	int rem = len;
	u16 kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	const struct nlattr *iter;

	NAN_DBG_ENTER();

	nla_for_each_attr(iter, buf, len, rem) {
		attr_type = nla_type(iter);
		WL_DBG(("attr: %s (%u)\n", nan_attr_to_str(attr_type), attr_type));

		switch (attr_type) {
		/* NAN Enable request attributes */
		case NAN_ATTRIBUTE_5G_SUPPORT:
			cmd_data->support_5g = nla_get_u8(iter);
			if (cmd_data->support_5g == 0) {
				cmd_data->nan_band = NAN_BAND_B;
			} else if (cmd_data->support_5g == 1) {
				cmd_data->nan_band = NAN_BAND_AUTO;
			} else {
				cmd_data->nan_band = NAN_BAND_INVALID;
			}
			break;
		case NAN_ATTRIBUTE_CLUSTER_LOW: {
			u16 cluster_low = nla_get_u16(iter);
			cmd_data->clus_id.octet[5] = cluster_low;
			break;
		}
		case NAN_ATTRIBUTE_CLUSTER_HIGH: {
			u16 cluster_high = nla_get_u16(iter);
			cmd_data->clus_id.octet[4] = cluster_high;
			break;
		}
		case NAN_ATTRIBUTE_SID_BEACON: {
			u8 sid_beacon = nla_get_u8(iter);
			cmd_data->sid_beacon.sid_enable = ((sid_beacon & 0x80) >> 7);
			if (cmd_data->sid_beacon.sid_enable) {
				cmd_data->sid_beacon.sid_count = (sid_beacon & 0x7f);
			}
			break;
		}
		case NAN_ATTRIBUTE_SYNC_DISC_5G:
			cmd_data->sdf_5g_val = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RSSI_CLOSE:
			cmd_data->config_2dot4g_rssi_close = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RSSI_MIDDLE:
			cmd_data->config_2dot4g_rssi_middle = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RSSI_PROXIMITY:
			cmd_data->rssi_proximity_2dot4g_val = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_HOP_COUNT_LIMIT:
			cmd_data->hop_count_limit = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RANDOM_TIME:
			cmd_data->random_factor = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_MASTER_PREF:
			cmd_data->master_pref = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_PERIODIC_SCAN_INTERVAL:
			break;

		/* Nan Publish/Subscribe request Attributes */
		case NAN_ATTRIBUTE_PUBLISH_ID:
			cmd_data->pub_id = nla_get_u16(iter);
			cmd_data->local_id = cmd_data->pub_id;
			break;
		case NAN_ATTRIBUTE_SUBSCRIBE_ID:
			cmd_data->sub_id = nla_get_u16(iter);
			cmd_data->local_id = cmd_data->sub_id;
			break;
		case NAN_ATTRIBUTE_SUBSCRIBE_TYPE:
			cmd_data->flags |= nla_get_u8(iter) ? WL_NAN_SUB_ACTIVE : 0;
			break;
		case NAN_ATTRIBUTE_PUBLISH_COUNT:
			cmd_data->life_count = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_PUBLISH_TYPE: {
			u8 val_u8 =  nla_get_u8(iter);
			if (val_u8 == 0) {
				cmd_data->flags |= WL_NAN_PUB_UNSOLICIT;
			} else if (val_u8 == 1) {
				cmd_data->flags |= WL_NAN_PUB_SOLICIT;
			} else {
				cmd_data->flags |= WL_NAN_PUB_BOTH;
			}
			break;
		}
		case NAN_ATTRIBUTE_PERIOD: {
			u16 period = nla_get_u16(iter);

			/*
			 * Conversion needed in case of google framework,
			 * as period from host is in units of ms.
			 */
			if (period > WL_NAN_DW_INTERVAL) {
				int remainder = period % WL_NAN_DW_INTERVAL;
				if (remainder < (WL_NAN_DW_INTERVAL/2)) {
					period = ROUNDDN(period, WL_NAN_DW_INTERVAL);
				} else {
					period = ROUNDUP(period, WL_NAN_DW_INTERVAL);
				}
				cmd_data->period = period / WL_NAN_DW_INTERVAL;
			} else {
				cmd_data->period = 1;
			}
			break;
		}
		case NAN_ATTRIBUTE_REPLIED_EVENT_FLAG:
			break;
		case NAN_ATTRIBUTE_TTL:
			cmd_data->ttl = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_MAC_ADDR:
			memcpy((char*)&cmd_data->mac_addr, (char*)nla_data(iter),
				ETHER_ADDR_LEN);
			break;
		case NAN_ATTRIBUTE_IF_ADDR:
			memcpy((char*)&cmd_data->if_addr, (char*)nla_data(iter),
				ETHER_ADDR_LEN);
			break;
		case NAN_ATTRIBUTE_SERVICE_NAME_LEN: {
			u16 hash_len = nla_get_u16(iter);
			if (hash_len == WL_NAN_SVC_HASH_LEN) {
				cmd_data->svc_hash.dlen = hash_len;
			} else {
				WL_ERR(("%s: invalid svc_hash length = %u\n",
					__FUNCTION__, hash_len));
			}
			break;
		}
		case NAN_ATTRIBUTE_SERVICE_NAME:
			if (cmd_data->svc_hash.dlen != WL_NAN_SVC_HASH_LEN) {
				WL_ERR(("%s: invalid svc_hash length = %u\n",
					__FUNCTION__, cmd_data->svc_hash.dlen));
				break;
			}

			cmd_data->svc_hash.data = kzalloc(cmd_data->svc_hash.dlen, kflags);
			if (!cmd_data->svc_hash.data) {
				WL_ERR(("failed to allocate svc_hash data, len=%d\n",
					cmd_data->svc_hash.dlen));
				break;
			}
			memcpy(cmd_data->svc_hash.data, nla_data(iter), cmd_data->svc_hash.dlen);
			break;
		case NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN:
			cmd_data->svc_info.dlen = nla_get_u16(iter);
			if (!cmd_data->svc_info.dlen) {
				WL_ERR(("invalid svc info length = %u\n",
					cmd_data->svc_info.dlen));
				break;
			}
			break;
		case NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO:
			if (!cmd_data->svc_info.dlen) {
				WL_ERR(("failed to allocate svc info by invalid len=%d\n",
					cmd_data->svc_info.dlen));
				break;
			}

			cmd_data->svc_info.data = kzalloc(cmd_data->svc_info.dlen, kflags);
			if (cmd_data->svc_info.data == NULL) {
				WL_ERR(("failed to allocate svc info data, len=%d\n",
					cmd_data->svc_info.dlen));
				break;
			}
			memcpy(cmd_data->svc_info.data, nla_data(iter), cmd_data->svc_info.dlen);
			break;
		case NAN_ATTRIBUTE_PEER_ID:
			cmd_data->remote_id = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_INST_ID:
			cmd_data->local_id = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_SUBSCRIBE_COUNT:
			cmd_data->life_count = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_SSIREQUIREDFORMATCHINDICATION: {
			u32 bit_flag = (u32)nla_get_u8(iter);
			cmd_data->flags |= bit_flag ? WL_NAN_SUB_MATCH_IF_SVC_INFO : 0;
			break;
		}
		case NAN_ATTRIBUTE_SUBSCRIBE_MATCH:
		case NAN_ATTRIBUTE_PUBLISH_MATCH: {
			u8 flag_match = nla_get_u8(iter);
			if (flag_match == NAN_MATCH_ALG_MATCH_ONCE) {
				cmd_data->flags |= WL_NAN_MATCH_ONCE;
			} else if (flag_match == NAN_MATCH_ALG_MATCH_CONTINUOUS) {
				WL_ERR(("Unsupported match criteria - "
					"NAN_MATCH_ALG_MATCH_CONTINUOUS\n"));
			} else if (flag_match == NAN_MATCH_ALG_MATCH_NEVER) {
				cmd_data->flags |= WL_NAN_MATCH_NEVER;
			} else {
				WL_ERR(("invalid nan match alg = %u\n", flag_match));
			}
			break;
		}
		case NAN_ATTRIBUTE_SERVICERESPONSEFILTER:
			cmd_data->srf_type = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_SERVICERESPONSEINCLUDE:
			cmd_data->srf_include = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_USESERVICERESPONSEFILTER:
			cmd_data->use_srf = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RX_MATCH_FILTER_LEN:
			cmd_data->rx_match.dlen = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_RX_MATCH_FILTER:
			if (!cmd_data->rx_match.dlen) {
				WL_ERR(("RX match filter len cant be zero!!\n"));
				break;
			}
			cmd_data->rx_match.data = kzalloc(cmd_data->rx_match.dlen, kflags);
			if (cmd_data->rx_match.data == NULL) {
				WL_ERR(("failed to allocate LEN=[%u]\n",
					cmd_data->rx_match.dlen));
				break;
			}
			memcpy(cmd_data->rx_match.data, nla_data(iter), cmd_data->rx_match.dlen);
			break;
		case NAN_ATTRIBUTE_TX_MATCH_FILTER_LEN:
			cmd_data->tx_match.dlen = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_TX_MATCH_FILTER:
			if (!cmd_data->tx_match.dlen) {
				WL_ERR(("TX match filter len cant be zero!!\n"));
				break;
			}
			cmd_data->tx_match.data = kzalloc(cmd_data->tx_match.dlen, kflags);
			if (cmd_data->tx_match.data == NULL) {
				WL_ERR(("failed to allocate LEN=[%u]\n",
					cmd_data->tx_match.dlen));
				break;
			}
			memcpy(cmd_data->tx_match.data, nla_data(iter), cmd_data->tx_match.dlen);
			break;
		case NAN_ATTRIBUTE_MAC_ADDR_LIST_NUM_ENTRIES:
			cmd_data->mac_list.num_mac_addr = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_MAC_ADDR_LIST:
			if (!cmd_data->mac_list.num_mac_addr) {
				WL_ERR(("num of mac addr cant be zero!!\n"));
				break;
			}
			cmd_data->mac_list.list = kzalloc((cmd_data->mac_list.num_mac_addr
						* ETHER_ADDR_LEN), kflags);
			if (cmd_data->mac_list.list == NULL) {
				WL_ERR(("failed to allocate LEN=[%u]\n",
				(cmd_data->mac_list.num_mac_addr * ETHER_ADDR_LEN)));
				break;
			}
			memcpy(cmd_data->mac_list.list, nla_data(iter),
				(cmd_data->mac_list.num_mac_addr * ETHER_ADDR_LEN));
			break;
		case NAN_ATTRIBUTE_TX_TYPE: {
			u8 val_u8 =  nla_get_u8(iter);
			if (val_u8 == 0) {
				cmd_data->flags |= WL_NAN_PUB_BCAST;
				WL_ERR(("NAN_ATTRIBUTE_TX_TYPE: flags=NAN_PUB_BCAST\n"));
			}
			break;
		}
		case NAN_ATTRIBUTE_OUI:
			cmd_data->nan_oui = nla_get_u32(iter);
			WL_ERR(("NAN_ATTRIBUTE_OUI: nan_oui=%d\n", cmd_data->nan_oui));
			break;
#ifdef NAN_DP
		case NAN_ATTRIBUTE_CHANNEL: {
			int chan = nla_get_u8(iter);

			sprintf((char*)cmd_data->channel, "%d", chan);
			if ((!strlen((char*)cmd_data->channel)) || (!cmd_data->channel)) {
				WL_ERR(("Channel len and data cant be zero!!\n"));
				break;
			}

			if (cmd_data->channel != NULL) {
				cmd_data->chanspec = wf_chspec_aton(cmd_data->channel);
				cmd_data->chanspec =
					wl_chspec_host_to_driver(cmd_data->chanspec);
				if (NAN_INVALID_CHANSPEC(cmd_data->chanspec)) {
					WL_ERR(("invalid chanspec, chanspec = 0x%04x \n",
							cmd_data->chanspec));
					ret = -EINVAL;
				}
				if (cmd_data->chanspec == 0) {
					WL_ERR(("Channel is not valid \n"));
					ret = -EINVAL;
				}
			}
			break;
		}
		case NAN_ATTRIBUTE_NDP_ID:
			cmd_data->ndp_instance_id = nla_get_u32(iter);
			break;
		case NAN_ATTRIBUTE_IFACE:
			WL_ERR(("NAN_ATTRIBUTE_IFACE\n"));
			if ((!strlen((char*) nla_data(iter))) || (!(char*)nla_data(iter))) {
				WL_ERR(("Iface_name len and data cant be zero!!\n"));
				break;
			}
			strncpy((char*)cmd_data->ndp_iface, (char*)nla_data(iter),
				strlen((char*)nla_data(iter)));
		case NAN_ATTRIBUTE_SECURITY:
			cmd_data->ndp_cfg.security_cfg = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_QOS:
			cmd_data->ndp_cfg.qos_cfg = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_RSP_CODE:
			cmd_data->rsp_code = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_INST_COUNT:
			cmd_data->num_ndp_instances = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_PEER_DISC_MAC_ADDR:
			memcpy((char*)&cmd_data->peer_disc_mac_addr,
				(char*)nla_data(iter), ETHER_ADDR_LEN);
			break;
		case NAN_ATTRIBUTE_PEER_NDI_MAC_ADDR:
			memcpy((char*)&cmd_data->peer_ndi_mac_addr,
				(char*)nla_data(iter), ETHER_ADDR_LEN);
			break;
#endif /* NAN_DP */
		case NAN_ATTRIBUTE_WARMUP_TIME:
			cmd_data->warmup_time = nla_get_u16(iter);
			break;
		case NAN_ATTRIBUTE_RECV_IND_CFG:
			cmd_data->recv_ind_flag = nla_get_u8(iter);
			break;
		case NAN_ATTRIBUTE_AMBTT:
			WL_DBG(("%s: Unhandled attribute, %d\n", __FUNCTION__, attr_type));
			break;
		case NAN_ATTRIBUTE_MASTER_RANK:
			WL_DBG(("%s: Unhandled attribute, %d\n", __FUNCTION__, attr_type));
			break;
		case NAN_ATTRIBUTE_CONNMAP:
			WL_DBG(("%s: Unhandled attribute, %d\n", __FUNCTION__, attr_type));
			break;
		case NAN_ATTRIBUTE_RSSI_THRESHOLD_FLAG:
			WL_DBG(("%s: Unhandled attribute, %d\n", __FUNCTION__, attr_type));
			break;
		default:
			WL_ERR(("%s: Unknown type, %d\n", __FUNCTION__, attr_type));
			ret = BCME_ERROR;
			break;
		}
	}

	/* We need to call set_config_handler b/f calling start enable TBD */
	return ret;

}

int wl_cfgvendor_send_nan_event(struct wiphy *wiphy, struct net_device *dev,
	int event_id, void *data, wl_nan_tlv_data_t *nan_opt_tlvs)
{
	int ret = BCME_OK;
	int buf_len = NAN_EVENT_BUFFER_SIZE;
	u16 kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct sk_buff *msg;

	NAN_DBG_ENTER();

	/* Allocate the skb for vendor event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
	msg = cfg80211_vendor_event_alloc(wiphy, NULL, buf_len, event_id, kflags);
#else
	msg = cfg80211_vendor_event_alloc(wiphy, buf_len, event_id, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
	/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */

	if (!msg) {
		WL_ERR(("%s: fail to allocate skb for vendor event\n", __FUNCTION__));
		return -ENOMEM;
	}

	if (event_id == GOOGLE_NAN_EVENT_DE_EVENT) {
		nan_de_event_data_t *de_event_data = (nan_de_event_data_t*)data;
		wl_nan_cfg_status_t *status = de_event_data->nstatus;

		WL_DBG(("GOOGLE_NAN_EVENT_DE_EVENT\n"));
		nla_put_u8(msg, NAN_ATTRIBUTE_ENABLE_STATUS, status->enabled);
		nla_put_u8(msg, NAN_ATTRIBUTE_ROLE, status->role);
		nla_put_u8(msg, NAN_ATTRIBUTE_JOIN_STATUS, status->joined);
		nla_put(msg, NAN_ATTRIBUTE_MAC_ADDR, ETH_ALEN, status->cid.octet);
		nla_put_u8(msg, NAN_ATTRIBUTE_MERGE, status->merged);
		nla_put_u8(msg, NAN_ATTRIBUTE_DE_EVENT_TYPE, de_event_data->nan_de_evt_type);

		/* additional status info */
		nla_put(msg, NAN_ATTRIBUTE_MASTER_RANK, WL_NAN_MASTER_RANK_LEN, &status->mr);
		nla_put(msg, NAN_ATTRIBUTE_ANCHOR_MASTER_RANK,
			WL_NAN_MASTER_RANK_LEN, &status->amr);
		nla_put_u32(msg, NAN_ATTRIBUTE_CNT_PEND_TXFRM, status->cnt_pend_txfrm);
		nla_put_u32(msg, NAN_ATTRIBUTE_CNT_BCN_TX, status->cnt_bcn_tx);
		nla_put_u32(msg, NAN_ATTRIBUTE_CNT_BCN_RX, status->cnt_bcn_rx);
		nla_put_u32(msg, NAN_ATTRIBUTE_CNT_SVC_DISC_TX, status->cnt_svc_disc_tx);
		nla_put_u32(msg, NAN_ATTRIBUTE_CNT_SVC_DISC_RX, status->cnt_svc_disc_rx);
		nla_put(msg, NAN_ATTRIBUTE_CLUSTER_ID, ETH_ALEN, &status->cid.octet);
		nla_put_u8(msg, NAN_ATTRIBUTE_HOP_COUNT_LIMIT, status->hop_count);

#ifdef WL_NAN_DEBUG
		WL_DBG(("NAN_EVENT_DE_EVENT:\n"));
		WL_DBG(("enabled=%d\n", status->enabled));
		WL_DBG(("joined=%d\n", status->joined));
		WL_DBG(("role=%d\n", status->role));
		WL_DBG(("chspec[0]=0x%04x, chspec[1]=0x%04x\n",
			status->chspec[0], status->chspec[1]));
		WL_DBG(("mr=" NMRSTR ", amr=" NMRSTR "\n",
			NMR2STR(status->mr), NMR2STR(status->amr)));
		WL_DBG(("cnt_pend_txfrm=%d\n", status->cnt_pend_txfrm));
		WL_DBG(("cnt_bcn_tx=%d\n", status->cnt_bcn_tx));
		WL_DBG(("cnt_bcn_rx=%d\n", status->cnt_bcn_rx));
		WL_DBG(("cnt_svc_disc_tx=%d\n", status->cnt_svc_disc_tx));
		WL_DBG(("cnt_svc_disc_rx=%d\n", status->cnt_svc_disc_rx));
		WL_DBG(("cluster id=" MACDBG "\n", MAC2STRDBG(status->cid.octet)));
		WL_DBG(("hop_count=%d\n", status->hop_count));
		WL_DBG(("de_evt_type=%d:\n", de_event_data->nan_de_evt_type));
#endif /* WL_NAN_DEBUG */
	} else if (event_id == GOOGLE_NAN_EVENT_SDF) {
		wl_nan_tlv_data_t *sub_event = (wl_nan_tlv_data_t*)nan_opt_tlvs;
		WL_DBG(("GOOGLE_NAN_EVENT_SDF\n"));
		WL_DBG(("svc info len = %d\n", sub_event->svc_info.dlen));

		if (sub_event->svc_info.dlen > 0 && sub_event->svc_info.data) {
			nla_put_u16(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN,
				sub_event->svc_info.dlen);
			nla_put(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO, sub_event->svc_info.dlen,
				sub_event->svc_info.data);
			WL_DBG(("svc info (%d): %s\n",
				sub_event->svc_info.dlen, sub_event->svc_info.data));
		} else {
			WL_INFORM(("%s: Optional svc info not present\n", __FUNCTION__));
		}
	} else if (event_id == GOOGLE_NAN_EVENT_SUBSCRIBE_MATCH) {
		wl_nan_event_disc_result_t *disc_event_data = (wl_nan_event_disc_result_t*)data;
		WL_DBG(("GOOGLE_NAN_EVENT_SUBSCRIBE_MATCH\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_HANDLE, disc_event_data->sub_id);
		nla_put_u16(msg, NAN_ATTRIBUTE_PUBLISH_ID, disc_event_data->pub_id);
		nla_put_u16(msg, NAN_ATTRIBUTE_SUBSCRIBE_ID, disc_event_data->sub_id);
		nla_put(msg, NAN_ATTRIBUTE_MAC_ADDR, ETH_ALEN, disc_event_data->pub_mac.octet);
	} else if (event_id == GOOGLE_NAN_EVENT_DISABLED) {
		WL_DBG(("GOOGLE_NAN_EVENT_DISABLED\n"));
		nla_put_u16(msg, NAN_ATTRIBUTE_HANDLE, 0);
	} else if (event_id == GOOGLE_NAN_EVENT_FOLLOWUP) {
		wl_nan_ev_receive_t *transmit_event_data = (wl_nan_ev_receive_t*)data;
		WL_DBG(("GOOGLE_NAN_EVENT_FOLLOWUP\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_HANDLE, transmit_event_data->local_id);
		nla_put_u16(msg, NAN_ATTRIBUTE_INST_ID, transmit_event_data->local_id);
		nla_put_u32(msg, NAN_ATTRIBUTE_PEER_ID, transmit_event_data->remote_id);
		nla_put(msg, NAN_ATTRIBUTE_MAC_ADDR, ETH_ALEN,
			transmit_event_data->remote_addr.octet);
		if (nan_opt_tlvs->svc_info.dlen && nan_opt_tlvs->svc_info.data) {
			nla_put_u16(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN,
					nan_opt_tlvs->svc_info.dlen);
			nla_put(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO,
					nan_opt_tlvs->svc_info.dlen, nan_opt_tlvs->svc_info.data);
		} else {
			WL_INFORM(("%s: Optional svc info data not present\n", __FUNCTION__));
		}
	} else if (event_id == GOOGLE_NAN_EVENT_PUBLISH_TERMINATED) {
		wl_nan_ev_terminated_t *term_event = (wl_nan_ev_terminated_t*)data;
		WL_DBG(("GOOGLE_NAN_EVENT_PUBLISH_TERMINATED\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_PUBLISH_ID, term_event->instance_id);
		nla_put_u32(msg, NAN_ATTRIBUTE_STATUS, term_event->reason);
		ret = wl_cfgnan_remove_inst_id(cfg, term_event->instance_id);
		if (ret) {
			WL_ERR(("failed to free publisher instance-id[%d], ret=%d\n",
				term_event->instance_id, ret));
			ret = -EINVAL;
			goto fail;
		}
	} else if (event_id == GOOGLE_NAN_EVENT_SUBSCRIBE_TERMINATED) {
		wl_nan_ev_terminated_t *term_event = (wl_nan_ev_terminated_t*)data;
		WL_ERR(("GOOGLE_NAN_EVENT_SUBSCRIBE_TERMINATED\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_SUBSCRIBE_ID, term_event->instance_id);
		nla_put_u32(msg, NAN_ATTRIBUTE_STATUS, term_event->reason);
		ret = wl_cfgnan_remove_inst_id(cfg, term_event->instance_id);
		if (ret) {
			WL_ERR(("failed to free subscriber instance-id[%d], ret=%d\n",
				term_event->instance_id, ret));
			ret = -EINVAL;
			goto fail;
		}
	}
#ifdef NAN_DP
#ifdef NOT_YET
	else if (event_id == GOOGLE_NAN_EVENT_DATA_REQUEST) {
		wl_nan_ev_data_ind_t *ndp_req_ind_event = (wl_nan_ev_data_ind_t*)data;
		WL_DBG(("GOOGLE_NAN_EVENT_DATA_REQUEST\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_PUBLISH_ID,
				ndp_req_ind_event->pub_id);
		nla_put_u16(msg, NAN_ATTRIBUTE_NDP_ID, ndp_req_ind_event->ndp_id);
		nla_put(msg, NAN_ATTRIBUTE_PEER_DISC_MAC_ADDR, ETH_ALEN,
				ndp_req_ind_event->peer_data_addr.octet);
		nla_put(msg, NAN_ATTRIBUTE_MAC_ADDR, ETH_ALEN,
				ndp_req_ind_event->local_data_addr.octet);
		if (nan_opt_tlvs->svc_info.dlen && nan_opt_tlvs->svc_info.data) {
			nla_put_u16(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN,
					WL_NAN_DATA_SVC_SPEC_INFO_LEN);
			nla_put(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO,
					WL_NAN_DATA_SVC_SPEC_INFO_LEN,
					ndp_req_ind_event->svc_spec_info);
		} else {
			WL_ERR(("%s: invalid svc info data, length\n", __FUNCTION__));
			ret = -EINVAL;
		}
	} else if (event_id == GOOGLE_NAN_EVENT_DATA_CONFIRMATION) {
		wl_nan_ev_data_conf_t *ndp_confirm_event = (wl_nan_ev_data_conf_t*)data;
		WL_DBG(("GOOGLE_NAN_EVENT_DATA_CONFIRMATION\n"));

		nla_put_u16(msg, NAN_ATTRIBUTE_NDP_ID, ndp_confirm_event->ndp_id);
		nla_put(msg, NAN_ATTRIBUTE_PEER_NDI_MAC_ADDR, ETH_ALEN,
				ndp_confirm_event->peer_data_addr.octet);
		nla_put(msg, NAN_ATTRIBUTE_MAC_ADDR, ETH_ALEN,
				ndp_confirm_event->local_data_addr.octet);
		if (nan_opt_tlvs->svc_info.dlen && nan_opt_tlvs->svc_info.data) {
			nla_put_u16(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO_LEN,
					WL_NAN_DATA_SVC_SPEC_INFO_LEN);
			nla_put(msg, NAN_ATTRIBUTE_SERVICE_SPECIFIC_INFO,
					WL_NAN_DATA_SVC_SPEC_INFO_LEN,
					ndp_confirm_event->svc_spec_info);
		} else {
			WL_ERR(("%s: invalid svc info data, length\n", __FUNCTION__));
			ret = -EINVAL;
		}
	} else if (event_id == GOOGLE_NAN_EVENT_DATA_END) {
		wl_nan_ev_data_end_t *ndp_end_event = (wl_nan_ev_data_end_t*)data;
		WL_ERR(("GOOGLE_NAN_EVENT_DATA_END\n"));

		nla_put_u8(msg, NAN_ATTRIBUTE_NDP_ID, ndp_end_event->ndp_id);
		nla_put_u8(msg, NAN_ATTRIBUTE_INST_COUNT, ndp_end_event->status);
	}
#endif /* NOT_YET */
#endif /* NAN_DP */
	else {
		goto fail;
	}

	cfg80211_vendor_event(msg, kflags);
	return ret;

fail:
	dev_kfree_skb_any(msg);
	WL_ERR(("Event not implemented or unknown -- Free skb, event_id = %d\n", event_id));
	return -EINVAL;
}

static int wl_cfgvendor_nan_req_subscribe(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)

{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	/* Blocking Subscribe if NAN is not enable and running */
	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, subscribe blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}

	ret = wl_cfgnan_generate_inst_id(cfg, WL_NAN_SVC_INST_SUBSCRIBER,
		&cmd_data.sub_id);
	if (ret) {
		WL_ERR(("failed to generate instance-id for subscriber\n"));
		goto exit;
	}

	ret = wl_cfgnan_sub_handler(wdev->netdev, cfg, "NAN_SUBSCRIBE",
		strlen("NAN_SUBSCRIBE"), &cmd_data);
	if (cmd_data.svc_hash.data) {
		kfree(cmd_data.svc_hash.data);
		cmd_data.svc_hash.dlen = 0;
	}
	if (cmd_data.rx_match.data) {
		kfree(cmd_data.rx_match.data);
		cmd_data.rx_match.dlen = 0;
	}
	if (cmd_data.tx_match.data) {
		kfree(cmd_data.tx_match.data);
		cmd_data.tx_match.dlen = 0;
	}
	if (cmd_data.mac_list.list) {
		kfree(cmd_data.mac_list.list);
		cmd_data.mac_list.num_mac_addr = 0;
	}
	if (ret) {
		WL_ERR(("failed to subscribe error[%d]\n", ret));
	}

	WL_DBG(("subscriber instance id=%d\n", cmd_data.sub_id));
exit:
	nan_req_resp.instance_id = cmd_data.sub_id;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_REQUEST_SUBSCRIBE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
	return ret;
}

static int wl_cfgvendor_nan_req_publish(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	/* Blocking Publish if NAN is not enable and running */
	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, publish blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_generate_inst_id(cfg, WL_NAN_SVC_INST_PUBLISHER,
		&cmd_data.pub_id);
	if (ret) {
		WL_ERR(("failed to generate instance-id for publisher\n"));
		goto exit;
	}
	ret = wl_cfgnan_pub_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (cmd_data.svc_hash.data) {
		kfree(cmd_data.svc_hash.data);
		cmd_data.svc_hash.dlen = 0;
	}
	if (cmd_data.svc_info.data) {
		kfree(cmd_data.svc_info.data);
		cmd_data.svc_info.dlen = 0;
	}
	if (cmd_data.rx_match.data) {
		kfree(cmd_data.rx_match.data);
		cmd_data.rx_match.dlen = 0;
	}
	if (cmd_data.tx_match.data) {
		kfree(cmd_data.tx_match.data);
		cmd_data.tx_match.dlen = 0;
	}
	if (cmd_data.mac_list.list) {
		kfree(cmd_data.mac_list.list);
		cmd_data.mac_list.num_mac_addr = 0;
	}
	if (ret) {
		WL_ERR(("failed to publish error[%d]\n", ret));
	}

	WL_DBG(("publisher instance id=%d\n", cmd_data.pub_id));
exit:
	nan_req_resp.instance_id = cmd_data.pub_id;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_REQUEST_PUBLISH;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
	return ret;
}

static int wl_cfgvendor_nan_start_handler(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	memset(&cmd_data, 0, sizeof(cmd_data));
	cmd_data.nan_band = NAN_BAND_INVALID;  /* Setting to some default */
	cmd_data.sid_beacon.sid_enable = NAN_SID_ENABLE_FLAG_INVALID; /* Setting to some default */
	cmd_data.sid_beacon.sid_count = NAN_SID_BEACON_COUNT_INVALID; /* Setting to some default */
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_start_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to start nan error[%d]\n", ret));
		goto exit;
	}

	/* Initializing Instance Id List */
	memset(cfg->nan_inst_ctrl, 0, NAN_ID_CTRL_SIZE * sizeof(nan_svc_inst_t));
	nan_req_resp.instance_id = 0;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_ENABLE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_stop_handler(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	ret = wl_cfgnan_stop_handler(wdev->netdev, cfg, NULL, 0, NULL);
	if (ret) {
		WL_ERR(("failed to stop nan error[%d]\n", ret));
		goto exit;
	}

	nan_req_resp.instance_id = 0;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DISABLE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_can_publish(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	uint8 inst_type = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	/* Blocking Cancel_Publish if NAN is not enable and running */
	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, cancel publish blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	WL_INFORM(("cancel publish instance-id=%d\n", cmd_data.pub_id));

	ret = wl_cfgnan_get_inst_type(cfg, cmd_data.pub_id, &inst_type);
	if (ret || inst_type != NAN_SVC_INST_PUBLISHER) {
		WL_ERR(("failed cancel publisher instance: id[%d], ret=%d\n",
				cmd_data.pub_id, ret));
		goto exit;
	}

	ret = wl_cfgnan_cancel_pub_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to cancel publish nan instance-id[%d] error[%d]\n",
			cmd_data.pub_id, ret));
		goto exit;
	}

exit:
	nan_req_resp.instance_id = cmd_data.pub_id;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_CANCEL_PUBLISH;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
	return ret;
}

static int wl_cfgvendor_nan_can_subscribe(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	uint8 inst_type = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	/* Blocking Cancel_Subscribe if NAN is not enable and running */
	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, cancel subscribe blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	WL_INFORM(("cancel subscribe instance-id=%d\n", cmd_data.sub_id));

	ret = wl_cfgnan_get_inst_type(cfg, cmd_data.sub_id, &inst_type);
	if (ret || inst_type != NAN_SVC_INST_SUBSCRIBER) {
		WL_ERR(("failed to cancel subscribe instance-id[%d], ret=%d\n",
				cmd_data.sub_id, ret));
		goto exit;
	}
	ret = wl_cfgnan_cancel_sub_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to cancel subscribe nan instance-id[%d] error[%d]\n",
			cmd_data.sub_id, ret));
		goto exit;
	}

exit:
	nan_req_resp.instance_id = cmd_data.sub_id;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_CANCEL_SUBSCRIBE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		&nan_req_resp, sizeof(nan_req_resp));
	return ret;
}


static int wl_cfgvendor_nan_transmit(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	/* Blocking Transmit if NAN is not enable and running */
	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, transmit blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_transmit_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to transmit-followup nan error[%d]\n", ret));
		goto exit;
	}

	nan_req_resp.instance_id = cmd_data.local_id;
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_TRANSMIT;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        &nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

#ifdef NAN_DP
static int wl_cfgvendor_nan_data_path_iface_create(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_data_path_iface_create_delete_handler(wdev->netdev, cfg,
			NULL, 0, &cmd_data, NAN_WIFI_SUBCMD_DATA_PATH_IFACE_CREATE);
	if (ret) {
		WL_ERR(("failed to create ndp interface [%d]\n", ret));
		goto exit;
	}
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_IFACE_CREATE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
			&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_data_path_iface_delete(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_data_path_iface_create_delete_handler(wdev->netdev, cfg,
			NULL, 0, &cmd_data, NAN_WIFI_SUBCMD_DATA_PATH_IFACE_DELETE);
	if (ret) {
		WL_ERR(("failed to delete ndp iface [%d]\n", ret));
		goto exit;
	}
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_IFACE_DELETE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
			&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_data_path_request(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;
	uint8 ndp_instance_id = 0;

	NAN_DBG_ENTER();

	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, nan data path request blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));
	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_data_path_request_handler(wdev->netdev, cfg, NULL, 0,
			&cmd_data, &ndp_instance_id);
	if (ret) {
		WL_ERR(("failed to request nan data path [%d]\n", ret));
		goto exit;
	}
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_REQUEST;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	nan_req_resp.ndp_instance_id = cmd_data.ndp_instance_id;
	WL_DBG(("ndp_instance_id = %d \n", cmd_data.ndp_instance_id));
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
			&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_data_path_response(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, nan data path response blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));

	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_data_path_response_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to response nan data path [%d]\n", ret));
		goto exit;
	}
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_RESPONSE;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
			&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

static int wl_cfgvendor_nan_data_path_end(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void * data, int len)
{
	int ret = 0;
	nan_cmd_data_t cmd_data;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	nan_hal_resp_t nan_req_resp;

	NAN_DBG_ENTER();

	if (!cfg->nan_enable || !cfg->nan_running) {
		WL_ERR(("nan is not enabled or running, nan data path end blocked\n"));
		return BCME_ERROR;
	}

	memset(&cmd_data, 0, sizeof(nan_cmd_data_t));

	ret = wl_cfgvendor_nan_parse_args(wiphy, data, len, &cmd_data);
	if (ret) {
		WL_ERR(("failed to parse nan vendor args\n"));
		goto exit;
	}
	ret = wl_cfgnan_data_path_end_handler(wdev->netdev, cfg, NULL, 0, &cmd_data);
	if (ret) {
		WL_ERR(("failed to end nan data path [%d]\n", ret));
		goto exit;
	}
	nan_req_resp.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_END;
	nan_req_resp.status = 0;
	nan_req_resp.value = ret;
	wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
			&nan_req_resp, sizeof(nan_req_resp));
exit:
	return ret;
}

#endif /* NAN_DP */
#endif /* WL_NAN */

#ifdef LINKSTAT_SUPPORT
#define NUM_RATE 32
#define NUM_PEER 1
#define NUM_CHAN 11
#define HEADER_SIZE sizeof(ver_len)
static int wl_cfgvendor_lstats_get_info(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	static char iovar_buf[WLC_IOCTL_MAXLEN];
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	int err = 0, i;
	wifi_iface_stat *iface;
	wifi_radio_stat *radio;
	wl_wme_cnt_t *wl_wme_cnt;
	const wl_cnt_wlc_t *wlc_cnt;
	const wl_cnt_v_le10_mcst_t *macstat_cnt;
	scb_val_t scbval;
	char *output;

	WL_INFORM(("%s: Enter \n", __func__));
	RETURN_EIO_IF_NOT_UP(cfg);

	bzero(cfg->ioctl_buf, WLC_IOCTL_MAXLEN);
	bzero(iovar_buf, WLC_IOCTL_MAXLEN);

	output = cfg->ioctl_buf;

	err = wldev_iovar_getbuf(bcmcfg_to_prmry_ndev(cfg), "radiostat", NULL, 0,
		iovar_buf, WLC_IOCTL_MAXLEN, NULL);
	if (err != BCME_OK && err != BCME_UNSUPPORTED) {
		WL_ERR(("error (%d) - size = %zu\n", err, sizeof(wifi_radio_stat)));
		return err;
	}
	radio = (wifi_radio_stat *)iovar_buf;
	radio->num_channels = NUM_CHAN;
	memcpy(output, iovar_buf+HEADER_SIZE, sizeof(wifi_radio_stat)-HEADER_SIZE);

	output += (sizeof(wifi_radio_stat) - HEADER_SIZE);
	output += (NUM_CHAN*sizeof(wifi_channel_stat));

	err = wldev_iovar_getbuf(bcmcfg_to_prmry_ndev(cfg), "wme_counters", NULL, 0,
		iovar_buf, WLC_IOCTL_MAXLEN, NULL);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}
	wl_wme_cnt = (wl_wme_cnt_t *)iovar_buf;
	iface = (wifi_iface_stat *)output;

	iface->ac[WIFI_AC_VO].ac = WIFI_AC_VO;
	iface->ac[WIFI_AC_VO].tx_mpdu = wl_wme_cnt->tx[AC_VO].packets;
	iface->ac[WIFI_AC_VO].rx_mpdu = wl_wme_cnt->rx[AC_VO].packets;
	iface->ac[WIFI_AC_VO].mpdu_lost = wl_wme_cnt->tx_failed[WIFI_AC_VO].packets;

	iface->ac[WIFI_AC_VI].ac = WIFI_AC_VI;
	iface->ac[WIFI_AC_VI].tx_mpdu = wl_wme_cnt->tx[AC_VI].packets;
	iface->ac[WIFI_AC_VI].rx_mpdu = wl_wme_cnt->rx[AC_VI].packets;
	iface->ac[WIFI_AC_VI].mpdu_lost = wl_wme_cnt->tx_failed[WIFI_AC_VI].packets;

	iface->ac[WIFI_AC_BE].ac = WIFI_AC_BE;
	iface->ac[WIFI_AC_BE].tx_mpdu = wl_wme_cnt->tx[AC_BE].packets;
	iface->ac[WIFI_AC_BE].rx_mpdu = wl_wme_cnt->rx[AC_BE].packets;
	iface->ac[WIFI_AC_BE].mpdu_lost = wl_wme_cnt->tx_failed[WIFI_AC_BE].packets;

	iface->ac[WIFI_AC_BK].ac = WIFI_AC_BK;
	iface->ac[WIFI_AC_BK].tx_mpdu = wl_wme_cnt->tx[AC_BK].packets;
	iface->ac[WIFI_AC_BK].rx_mpdu = wl_wme_cnt->rx[AC_BK].packets;
	iface->ac[WIFI_AC_BK].mpdu_lost = wl_wme_cnt->tx_failed[WIFI_AC_BK].packets;
	bzero(iovar_buf, WLC_IOCTL_MAXLEN);

	err = wldev_iovar_getbuf(bcmcfg_to_prmry_ndev(cfg), "counters", NULL, 0,
		iovar_buf, WLC_IOCTL_MAXLEN, NULL);
	if (unlikely(err)) {
		WL_ERR(("error (%d) - size = %zu\n", err, sizeof(wl_cnt_wlc_t)));
		return err;
	}

	CHK_CNTBUF_DATALEN(iovar_buf, WLC_IOCTL_MAXLEN);
	/* Translate traditional (ver <= 10) counters struct to new xtlv type struct */
	err = wl_cntbuf_to_xtlv_format(NULL, iovar_buf, WLC_IOCTL_MAXLEN, 0);
	if (err != BCME_OK) {
		WL_ERR(("%s wl_cntbuf_to_xtlv_format ERR %d\n",
			__FUNCTION__, err));
		return err;
	}

	if (!(wlc_cnt = GET_WLCCNT_FROM_CNTBUF(iovar_buf))) {
		WL_ERR(("%s wlc_cnt NULL!\n",
			__FUNCTION__));
		return BCME_ERROR;
	}

	iface->ac[WIFI_AC_BE].retries = wlc_cnt->txretry;

	if ((macstat_cnt = (const wl_cnt_v_le10_mcst_t *)
			bcm_get_data_from_xtlv_buf(((const wl_cnt_info_t *)wlc_cnt)->data,
			((const wl_cnt_info_t *)wlc_cnt)->datalen,
			WL_CNT_XTLV_CNTV_LE10_UCODE, NULL,
			BCM_XTLV_OPTION_ALIGN32)) == NULL) {
		macstat_cnt = (const wl_cnt_v_le10_mcst_t *)
			bcm_get_data_from_xtlv_buf(((const wl_cnt_info_t *)wlc_cnt)->data,
				((const wl_cnt_info_t *)wlc_cnt)->datalen,
				WL_CNT_XTLV_LT40_UCODE_V1, NULL,
				BCM_XTLV_OPTION_ALIGN32);
	}

	if (macstat_cnt == NULL) {
		printf("wlmTxGetAckedPackets: macstat_cnt NULL!\n");
		return FALSE;
	}

	iface->beacon_rx = macstat_cnt->rxbeaconmbss;

	err = wldev_get_rssi(bcmcfg_to_prmry_ndev(cfg), &scbval);
	if (unlikely(err)) {
		WL_ERR(("get_rssi error (%d)\n", err));
		return err;
	}
	iface->rssi_mgmt = scbval.val;

	iface->num_peers = NUM_PEER;
	iface->peer_info->num_rate = NUM_RATE;

	bzero(iovar_buf, WLC_IOCTL_MAXLEN);
	output = (char *)iface + sizeof(wifi_iface_stat) + NUM_PEER*sizeof(wifi_peer_info);

	err = wldev_iovar_getbuf(bcmcfg_to_prmry_ndev(cfg), "ratestat", NULL, 0,
		iovar_buf, WLC_IOCTL_MAXLEN, NULL);
	if (err != BCME_OK && err != BCME_UNSUPPORTED) {
		WL_ERR(("error (%d) - size = %zu\n", err, NUM_RATE*sizeof(wifi_rate_stat)));
		return err;
	}
	for (i = 0; i < NUM_RATE; i++)
		memcpy(output, iovar_buf+HEADER_SIZE+i*sizeof(wifi_rate_stat),
		sizeof(wifi_rate_stat)-HEADER_SIZE);

	err =  wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
		cfg->ioctl_buf,
		sizeof(wifi_radio_stat)-HEADER_SIZE +
		NUM_CHAN*sizeof(wifi_channel_stat) +
		sizeof(wifi_iface_stat)+NUM_PEER*sizeof(wifi_peer_info) +
		NUM_RATE*(sizeof(wifi_rate_stat)-HEADER_SIZE));
	if (unlikely(err))
		WL_ERR(("Vendor Command reply failed ret:%d \n", err));

	return err;
}
#endif /* LINKSTAT_SUPPORT */

#ifdef DEBUGABILITY
static int wl_cfgvendor_dbg_start_logging(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = BCME_OK, rem, type;
	char ring_name[DBGRING_NAME_MAX] = {0};
	int log_level = 0, flags = 0, time_intval = 0, threshold = 0;
	const struct nlattr *iter;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;
	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case DEBUG_ATTRIBUTE_RING_NAME:
				strncpy(ring_name, nla_data(iter),
					MIN(sizeof(ring_name) -1, nla_len(iter)));
				break;
			case DEBUG_ATTRIBUTE_LOG_LEVEL:
				log_level = nla_get_u32(iter);
				break;
			case DEBUG_ATTRIBUTE_RING_FLAGS:
				flags = nla_get_u32(iter);
				break;
			case DEBUG_ATTRIBUTE_LOG_TIME_INTVAL:
				time_intval = nla_get_u32(iter);
				break;
			case DEBUG_ATTRIBUTE_LOG_MIN_DATA_SIZE:
				threshold = nla_get_u32(iter);
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				ret = BCME_BADADDR;
				goto exit;
		}
	}

	ret = dhd_os_start_logging(dhd_pub, ring_name, log_level, flags, time_intval, threshold);
	if (ret < 0) {
		WL_ERR(("start_logging is failed ret: %d\n", ret));
	}
exit:
	return ret;
}

static int wl_cfgvendor_dbg_reset_logging(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = BCME_OK;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;

	ret = dhd_os_reset_logging(dhd_pub);
	if (ret < 0) {
		WL_ERR(("reset logging is failed ret: %d\n", ret));
	}

	return ret;
}

static int wl_cfgvendor_dbg_get_version(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void *data, int len)
{
	int ret = BCME_OK, rem, type;
	int buf_len = 1024;
	bool dhd_ver = FALSE;
	char *buf_ptr;
	const struct nlattr *iter;
	gfp_t kflags;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	buf_ptr = kzalloc(buf_len, kflags);
	if (!buf_ptr) {
		WL_ERR(("failed to allocate the buffer for version n"));
		ret = BCME_NOMEM;
		goto exit;
	}
	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case DEBUG_ATTRIBUTE_GET_DRIVER:
				dhd_ver = TRUE;
				break;
			case DEBUG_ATTRIBUTE_GET_FW:
				dhd_ver = FALSE;
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				ret = BCME_ERROR;
				goto exit;
		}
	}
	ret = dhd_os_get_version(bcmcfg_to_prmry_ndev(cfg), dhd_ver, &buf_ptr, buf_len);
	if (ret < 0) {
		WL_ERR(("failed to get the version %d\n", ret));
		goto exit;
	}
	ret = wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        buf_ptr, strlen(buf_ptr));
exit:
	kfree(buf_ptr);
	return ret;
}

static int wl_cfgvendor_dbg_get_ring_status(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = BCME_OK;
	int ring_id, i;
	int ring_cnt;
	struct sk_buff *skb;
	dhd_dbg_ring_status_t dbg_ring_status[DEBUG_RING_ID_MAX];
	dhd_dbg_ring_status_t ring_status;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;
	memset(dbg_ring_status, 0, DBG_RING_STATUS_SIZE * DEBUG_RING_ID_MAX);
	ring_cnt = 0;
	for (ring_id = DEBUG_RING_ID_INVALID + 1; ring_id < DEBUG_RING_ID_MAX; ring_id++) {
		ret = dhd_os_get_ring_status(dhd_pub, ring_id, &ring_status);
		if (ret == BCME_NOTFOUND) {
			WL_DBG(("The ring (%d) is not found \n", ring_id));
		} else if (ret == BCME_OK) {
			dbg_ring_status[ring_cnt++] = ring_status;
		}
	}
	/* Alloc the SKB for vendor_event */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy,
		(DBG_RING_STATUS_SIZE * ring_cnt) + 100);
	if (!skb) {
		WL_ERR(("skb allocation is failed\n"));
		ret = BCME_NOMEM;
		goto exit;
	}

	nla_put_u32(skb, DEBUG_ATTRIBUTE_RING_NUM, ring_cnt);
	for (i = 0; i < ring_cnt; i++) {
		nla_put(skb, DEBUG_ATTRIBUTE_RING_STATUS, DBG_RING_STATUS_SIZE,
				&dbg_ring_status[i]);
	}
	ret = cfg80211_vendor_cmd_reply(skb);

	if (ret) {
		WL_ERR(("Vendor Command reply failed ret:%d \n", ret));
	}
exit:
	return ret;
}

static int wl_cfgvendor_dbg_get_ring_data(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = BCME_OK, rem, type;
	char ring_name[DBGRING_NAME_MAX] = {0};
	const struct nlattr *iter;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;

	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case DEBUG_ATTRIBUTE_RING_NAME:
				strncpy(ring_name, nla_data(iter),
					MIN(sizeof(ring_name) -1, nla_len(iter)));
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				return ret;
		}
	}

	ret = dhd_os_trigger_get_ring_data(dhd_pub, ring_name);
	if (ret < 0) {
		WL_ERR(("trigger_get_data failed ret:%d\n", ret));
	}

	return ret;
}

static int wl_cfgvendor_dbg_get_feature(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret = BCME_OK;
	u32 supported_features = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;

	ret = dhd_os_dbg_get_feature(dhd_pub, &supported_features);
	if (ret < 0) {
		WL_ERR(("dbg_get_feature failed ret:%d\n", ret));
		goto exit;
	}
	ret = wl_cfgvendor_send_cmd_reply(wiphy, bcmcfg_to_prmry_ndev(cfg),
	        &supported_features, sizeof(supported_features));
exit:
	return ret;
}

static void wl_cfgvendor_dbg_ring_send_evt(void *ctx,
	const int ring_id, const void *data, const uint32 len,
	const dhd_dbg_ring_status_t ring_status)
{
	struct net_device *ndev = ctx;
	struct wiphy *wiphy;
	gfp_t kflags;
	struct sk_buff *skb;
	if (!ndev) {
		WL_ERR(("ndev is NULL\n"));
		return;
	}
	kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	wiphy = ndev->ieee80211_ptr->wiphy;
	/* Alloc the SKB for vendor_event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
	skb = cfg80211_vendor_event_alloc(wiphy, NULL, len + 100,
			GOOGLE_DEBUG_RING_EVENT, kflags);
#else
	skb = cfg80211_vendor_event_alloc(wiphy, len + 100,
			GOOGLE_DEBUG_RING_EVENT, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
	if (!skb) {
		WL_ERR(("skb alloc failed"));
		return;
	}
	nla_put(skb, DEBUG_ATTRIBUTE_RING_STATUS, sizeof(ring_status), &ring_status);
	nla_put(skb, DEBUG_ATTRIBUTE_RING_DATA, len, data);
	cfg80211_vendor_event(skb, kflags);
}

static void wl_cfgvendor_dbg_send_urgent_evt(void *ctx, const void *data,
	const uint32 len, const uint32 fw_len)
{
	struct net_device *ndev = ctx;
	struct wiphy *wiphy;
	gfp_t kflags;
	struct sk_buff *skb;
	if (!ndev) {
		WL_ERR(("ndev is NULL\n"));
		return;
	}
	kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
	wiphy = ndev->ieee80211_ptr->wiphy;
	/* Alloc the SKB for vendor_event */
#if (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || \
	LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
	skb = cfg80211_vendor_event_alloc(wiphy, NULL, len + 100,
			GOOGLE_FW_DUMP_EVENT, kflags);
#else
	skb = cfg80211_vendor_event_alloc(wiphy, len + 100,
			GOOGLE_FW_DUMP_EVENT, kflags);
#endif /* (defined(CONFIG_ARCH_MSM) && defined(SUPPORT_WDEV_CFG80211_VENDOR_EVENT_ALLOC)) || */
		/* LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0) */
	if (!skb) {
		WL_ERR(("skb alloc failed"));
		return;
	}
	nla_put_u32(skb, DEBUG_ATTRIBUTE_FW_DUMP_LEN, fw_len);
	nla_put(skb, DEBUG_ATTRIBUTE_RING_DATA, len, data);
	cfg80211_vendor_event(skb, kflags);
}
#endif /* DEBUGABILITY */

#ifdef DBG_PKT_MON
static int wl_cfgvendor_dbg_start_pkt_fate_monitoring(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void *data, int len)
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;
	int ret;

	ret = dhd_os_dbg_attach_pkt_monitor(dhd_pub);
	if (unlikely(ret)) {
		WL_ERR(("failed to start pkt fate monitoring, ret=%d", ret));
	}

	return ret;
}

typedef int (*dbg_mon_get_pkts_t) (dhd_pub_t *dhdp, void __user *user_buf,
	uint16 req_count, uint16 *resp_count);

static int __wl_cfgvendor_dbg_get_pkt_fates(struct wiphy *wiphy,
	const void *data, int len, dbg_mon_get_pkts_t dbg_mon_get_pkts)
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;
	struct sk_buff *skb = NULL;
	const struct nlattr *iter;
	void __user *user_buf = NULL;
	uint16 req_count = 0, resp_count = 0;
	int ret, tmp, type, mem_needed;

	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);
		switch (type) {
			case DEBUG_ATTRIBUTE_PKT_FATE_NUM:
				req_count = nla_get_u32(iter);
				break;
			case DEBUG_ATTRIBUTE_PKT_FATE_DATA:
				user_buf = (void __user *)(unsigned long) nla_get_u64(iter);
				break;
			default:
				WL_ERR(("%s: no such attribute %d\n", __FUNCTION__, type));
				ret = -EINVAL;
				goto exit;
		}
	}

	if (!req_count || !user_buf) {
		WL_ERR(("%s: invalid request, user_buf=%p, req_count=%u\n",
			__FUNCTION__, user_buf, req_count));
		ret = -EINVAL;
		goto exit;
	}

	ret = dbg_mon_get_pkts(dhd_pub, user_buf, req_count, &resp_count);
	if (unlikely(ret)) {
		WL_ERR(("failed to get packets, ret:%d \n", ret));
		goto exit;
	}

	mem_needed = VENDOR_REPLY_OVERHEAD + ATTRIBUTE_U32_LEN;
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
	if (unlikely(!skb)) {
		WL_ERR(("skb alloc failed"));
		ret = -ENOMEM;
		goto exit;
	}

	nla_put_u32(skb, DEBUG_ATTRIBUTE_PKT_FATE_NUM, resp_count);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret)) {
		WL_ERR(("vendor Command reply failed ret:%d \n", ret));
	}

exit:
	return ret;
}

static int wl_cfgvendor_dbg_get_tx_pkt_fates(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret;

	ret = __wl_cfgvendor_dbg_get_pkt_fates(wiphy, data, len,
			dhd_os_dbg_monitor_get_tx_pkts);
	if (unlikely(ret)) {
		WL_ERR(("failed to get tx packets, ret:%d \n", ret));
	}

	return ret;
}

static int wl_cfgvendor_dbg_get_rx_pkt_fates(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	int ret;

	ret = __wl_cfgvendor_dbg_get_pkt_fates(wiphy, data, len,
			dhd_os_dbg_monitor_get_rx_pkts);
	if (unlikely(ret)) {
		WL_ERR(("failed to get rx packets, ret:%d \n", ret));
	}

	return ret;
}
#endif /* DBG_PKT_MON */

#ifdef KEEP_ALIVE
static int wl_cfgvendor_start_mkeep_alive(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int len)
{
	/* max size of IP packet for keep alive */
	const int MKEEP_ALIVE_IP_PKT_MAX = 256;

	int ret = BCME_OK, rem, type;
	uint8 mkeep_alive_id = 0;
	uint8 *ip_pkt = NULL;
	uint16 ip_pkt_len = 0;
	uint8 src_mac[ETHER_ADDR_LEN];
	uint8 dst_mac[ETHER_ADDR_LEN];
	uint32 period_msec = 0;
	const struct nlattr *iter;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;
	gfp_t kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;

	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case MKEEP_ALIVE_ATTRIBUTE_ID:
				mkeep_alive_id = nla_get_u8(iter);
				break;
			case MKEEP_ALIVE_ATTRIBUTE_IP_PKT_LEN:
				ip_pkt_len = nla_get_u16(iter);
				if (ip_pkt_len > MKEEP_ALIVE_IP_PKT_MAX) {
					ret = BCME_BADARG;
					goto exit;
				}
				break;
			case MKEEP_ALIVE_ATTRIBUTE_IP_PKT:
				ip_pkt = (u8 *)kzalloc(ip_pkt_len, kflags);
				if (ip_pkt == NULL) {
					ret = BCME_NOMEM;
					WL_ERR(("Failed to allocate mem for ip packet\n"));
					goto exit;
				}
				memcpy(ip_pkt, (u8*)nla_data(iter), ip_pkt_len);
				break;
			case MKEEP_ALIVE_ATTRIBUTE_SRC_MAC_ADDR:
				memcpy(src_mac, nla_data(iter), ETHER_ADDR_LEN);
				break;
			case MKEEP_ALIVE_ATTRIBUTE_DST_MAC_ADDR:
				memcpy(dst_mac, nla_data(iter), ETHER_ADDR_LEN);
				break;
			case MKEEP_ALIVE_ATTRIBUTE_PERIOD_MSEC:
				period_msec = nla_get_u32(iter);
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				ret = BCME_BADARG;
				goto exit;
		}
	}

	ret = dhd_dev_start_mkeep_alive(dhd_pub, mkeep_alive_id, ip_pkt, ip_pkt_len, src_mac,
		dst_mac, period_msec);
	if (ret < 0) {
		WL_ERR(("start_mkeep_alive is failed ret: %d\n", ret));
	}

exit:
	if (ip_pkt) {
		kfree(ip_pkt);
	}

	return ret;
}

static int wl_cfgvendor_stop_mkeep_alive(struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int len)
{
	int ret = BCME_OK, rem, type;
	uint8 mkeep_alive_id = 0;
	const struct nlattr *iter;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd_pub = cfg->pub;

	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case MKEEP_ALIVE_ATTRIBUTE_ID:
				mkeep_alive_id = nla_get_u8(iter);
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				ret = BCME_BADARG;
				break;
		}
	}

	ret = dhd_dev_stop_mkeep_alive(dhd_pub, mkeep_alive_id);
	if (ret < 0) {
		WL_ERR(("stop_mkeep_alive is failed ret: %d\n", ret));
	}

	return ret;
}
#endif /* KEEP_ALIVE */

#if defined(PKT_FILTER_SUPPORT) && defined(APF)
static int
wl_cfgvendor_apf_get_capabilities(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void *data, int len)
{
	struct net_device *ndev = wdev_to_ndev(wdev);
	struct sk_buff *skb;
	int ret, ver, max_len, mem_needed;

	/* APF version */
	ver = 0;
	ret = dhd_dev_apf_get_version(ndev, &ver);
	if (unlikely(ret)) {
		WL_ERR(("APF get version failed, ret=%d\n", ret));
		return ret;
	}

	/* APF memory size limit */
	max_len = 0;
	ret = dhd_dev_apf_get_max_len(ndev, &max_len);
	if (unlikely(ret)) {
		WL_ERR(("APF get maximum length failed, ret=%d\n", ret));
		return ret;
	}

	mem_needed = VENDOR_REPLY_OVERHEAD + (ATTRIBUTE_U32_LEN * 2);

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, mem_needed);
	if (unlikely(!skb)) {
		WL_ERR(("%s: can't allocate %d bytes\n", __FUNCTION__, mem_needed));
		return -ENOMEM;
	}

	nla_put_u32(skb, APF_ATTRIBUTE_VERSION, ver);
	nla_put_u32(skb, APF_ATTRIBUTE_MAX_LEN, max_len);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret)) {
		WL_ERR(("vendor command reply failed, ret=%d\n", ret));
	}

	return ret;
}

static int
wl_cfgvendor_apf_set_filter(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	struct net_device *ndev = wdev_to_ndev(wdev);
	const struct nlattr *iter;
	u8 *program = NULL;
	u32 program_len = 0;
	int ret, tmp, type;
	gfp_t kflags;

	/* assumption: length attribute must come first */
	nla_for_each_attr(iter, data, len, tmp) {
		type = nla_type(iter);
		switch (type) {
			case APF_ATTRIBUTE_PROGRAM_LEN:
				program_len = nla_get_u32(iter);
				if (unlikely(!program_len)) {
					WL_ERR(("zero program length\n"));
					ret = -EINVAL;
					goto exit;
				}
				break;
			case APF_ATTRIBUTE_PROGRAM:
				if (unlikely(!program_len)) {
					WL_ERR(("program len is not set\n"));
					ret = -EINVAL;
					goto exit;
				}
				kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
				program = kzalloc(program_len, kflags);
				if (unlikely(!program)) {
					WL_ERR(("%s: can't allocate %d bytes\n",
					      __FUNCTION__, program_len));
					ret = -ENOMEM;
					goto exit;
				}
				memcpy(program, (u8*)nla_data(iter), program_len);
				break;
			default:
				WL_ERR(("%s: no such attribute %d\n", __FUNCTION__, type));
				ret = -EINVAL;
				goto exit;
		}
	}

	ret = dhd_dev_apf_add_filter(ndev, program, program_len);

exit:
	if (program) {
		kfree(program);
	}
	return ret;
}
#endif /* PKT_FILTER_SUPPORT && APF */

#ifdef NDO_CONFIG_SUPPORT
static int wl_cfgvendor_configure_nd_offload(struct wiphy *wiphy,
	struct wireless_dev *wdev, const void  *data, int len)
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	const struct nlattr *iter;
	int ret = BCME_OK, rem, type;
	u8 enable = 0;

	nla_for_each_attr(iter, data, len, rem) {
		type = nla_type(iter);
		switch (type) {
			case ANDR_WIFI_ATTRIBUTE_ND_OFFLOAD_VALUE:
				enable = nla_get_u8(iter);
				break;
			default:
				WL_ERR(("Unknown type: %d\n", type));
				ret = BCME_BADARG;
				goto exit;
		}
	}

	ret = dhd_dev_ndo_cfg(bcmcfg_to_prmry_ndev(cfg), enable);
	if (ret < 0) {
		WL_ERR(("dhd_dev_ndo_cfg() failed: %d\n", ret));
	}

exit:
	return ret;
}
#endif /* NDO_CONFIG_SUPPORT */

static const struct wiphy_vendor_command wl_vendor_cmds [] = {
#if defined(BCMDONGLEHOST)
	{
		{
			.vendor_id = OUI_BRCM,
			.subcmd = BRCM_VENDOR_SCMD_PRIV_STR
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_priv_string_handler
	},
#endif /* defined(BCMDONGLEHOST) */
	{
		{
			.vendor_id = OUI_BRCM,
			.subcmd = BRCM_VENDOR_SCMD_BCM_STR
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_priv_bcm_handler
	},
#ifdef GSCAN_SUPPORT
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_GET_CAPABILITIES
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_gscan_get_capabilities
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_SET_CONFIG
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_set_scan_cfg
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_SET_SCAN_CONFIG
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_set_batch_scan_cfg
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_ENABLE_GSCAN
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_initiate_gscan
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_ENABLE_FULL_SCAN_RESULTS
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_enable_full_scan_result
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_SET_HOTLIST
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_hotlist_cfg
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_SET_SIGNIFICANT_CHANGE_CONFIG
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_significant_change_cfg
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_GET_SCAN_RESULTS
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_gscan_get_batch_results
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = GSCAN_SUBCMD_GET_CHANNEL_LIST
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_gscan_get_channel_list
	},
#endif /* GSCAN_SUPPORT */
#ifdef RTT_SUPPORT
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = RTT_SUBCMD_SET_CONFIG
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_rtt_set_config
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = RTT_SUBCMD_CANCEL_CONFIG
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_rtt_cancel_config
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = RTT_SUBCMD_GETCAPABILITY
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_rtt_get_capability
	},
#endif /* RTT_SUPPORT */
#if defined(BCMDONGLEHOST)
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = ANDR_WIFI_SUBCMD_GET_FEATURE_SET
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_get_feature_set
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = ANDR_WIFI_SUBCMD_GET_FEATURE_SET_MATRIX
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_get_feature_set_matrix
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = ANDR_WIFI_RANDOM_MAC_OUI
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_set_rand_mac_oui
	},
#endif /* defined(BCMDONGLEHOST) */
#ifdef CUSTOM_FORCE_NODFS_FLAG
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = ANDR_WIFI_NODFS_CHANNELS
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_set_nodfs_flag

	},
#endif /* CUSTOM_FORCE_NODFS_FLAG */
#ifdef LINKSTAT_SUPPORT
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = LSTATS_SUBCMD_GET_INFO
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_lstats_get_info
	},
#endif /* LINKSTAT_SUPPORT */
#ifdef DEBUGABILITY
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_START_LOGGING
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_start_logging
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_RESET_LOGGING
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_reset_logging
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_VER
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_version
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_RING_STATUS
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_ring_status
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_RING_DATA
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_ring_data
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_FEATURE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_feature
	},
#endif /* DEBUGABILITY */
#ifdef DBG_PKT_MON
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_START_PKT_FATE_MONITORING
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_start_pkt_fate_monitoring
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_TX_PKT_FATES
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_tx_pkt_fates
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = DEBUG_GET_RX_PKT_FATES
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_dbg_get_rx_pkt_fates
	},
#endif /* DBG_PKT_MON */
#ifdef KEEP_ALIVE
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = WIFI_OFFLOAD_SUBCMD_START_MKEEP_ALIVE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_start_mkeep_alive
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = WIFI_OFFLOAD_SUBCMD_STOP_MKEEP_ALIVE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_stop_mkeep_alive
	},
#endif /* KEEP_ALIVE */
#ifdef WL_NAN
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_ENABLE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_start_handler
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DISABLE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_stop_handler
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_REQUEST_PUBLISH
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_req_publish
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_REQUEST_SUBSCRIBE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_req_subscribe
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_CANCEL_PUBLISH
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_can_publish
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_CANCEL_SUBSCRIBE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_can_subscribe
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_TRANSMIT
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_transmit
	},

#ifdef NAN_DP
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_IFACE_CREATE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_data_path_iface_create
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_IFACE_DELETE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_data_path_iface_delete
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_REQUEST
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_data_path_request
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_RESPONSE
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_data_path_response
	},
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = NAN_WIFI_SUBCMD_DATA_PATH_END
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_nan_data_path_end
	},
#endif /* NAN_DP */
#endif /* WL_NAN */
#if defined(PKT_FILTER_SUPPORT) && defined(APF)
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = APF_SUBCMD_GET_CAPABILITIES
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_apf_get_capabilities
	},

	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = APF_SUBCMD_SET_FILTER
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_apf_set_filter
	},
#endif /* PKT_FILTER_SUPPORT && APF */
#ifdef NDO_CONFIG_SUPPORT
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = WIFI_SUBCMD_CONFIG_ND_OFFLOAD
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_configure_nd_offload
	},
#endif /* NDO_CONFIG_SUPPORT */
#ifdef RSSI_MONITOR_SUPPORT
	{
		{
			.vendor_id = OUI_GOOGLE,
			.subcmd = WIFI_SUBCMD_SET_RSSI_MONITOR
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV | WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit = wl_cfgvendor_set_rssi_monitor
	}
#endif /* RSSI_MONITOR_SUPPORT */
};

static const struct  nl80211_vendor_cmd_info wl_vendor_events [] = {
		{ OUI_BRCM, BRCM_VENDOR_EVENT_UNSPEC },
		{ OUI_BRCM, BRCM_VENDOR_EVENT_PRIV_STR },
		{ OUI_GOOGLE, GOOGLE_GSCAN_SIGNIFICANT_EVENT },
		{ OUI_GOOGLE, GOOGLE_GSCAN_GEOFENCE_FOUND_EVENT },
		{ OUI_GOOGLE, GOOGLE_GSCAN_BATCH_SCAN_EVENT },
		{ OUI_GOOGLE, GOOGLE_SCAN_FULL_RESULTS_EVENT },
		{ OUI_GOOGLE, GOOGLE_RTT_COMPLETE_EVENT },
		{ OUI_GOOGLE, GOOGLE_SCAN_COMPLETE_EVENT },
		{ OUI_GOOGLE, GOOGLE_GSCAN_GEOFENCE_LOST_EVENT },
		{ OUI_GOOGLE, GOOGLE_SCAN_EPNO_EVENT },
		{ OUI_GOOGLE, GOOGLE_DEBUG_RING_EVENT },
		{ OUI_GOOGLE, GOOGLE_FW_DUMP_EVENT },
		{ OUI_GOOGLE, GOOGLE_PNO_HOTSPOT_FOUND_EVENT },
		{ OUI_GOOGLE, GOOGLE_RSSI_MONITOR_EVENT },
		{ OUI_GOOGLE, GOOGLE_MKEEP_ALIVE_EVENT },
#ifdef WL_NAN
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_ENABLED},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_DISABLED},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_PUBLISH_TERMINATED},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_SUBSCRIBE_MATCH},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_SUBSCRIBE_UNMATCH},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_SUBSCRIBE_TERMINATED},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_DE_EVENT},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_FOLLOWUP},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_TCA},
#ifdef NAN_DP
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_DATA_REQUEST},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_DATA_CONFIRMATION},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_DATA_END},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_BEACON},
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_SDF},
#endif /* NAN_DP */
		{ OUI_GOOGLE, GOOGLE_NAN_EVENT_UNKNOWN}
#endif /* WL_NAN */
};

#if defined(BCMDONGLEHOST)
int wl_cfgvendor_attach(struct wiphy *wiphy, dhd_pub_t *dhd)
#else
int wl_cfgvendor_attach(struct wiphy *wiphy)
#endif /* defined(BCMDONGLEHOST) */
{

	WL_INFORM(("Vendor: Register BRCM cfg80211 vendor cmd(0x%x) interface \n",
		NL80211_CMD_VENDOR));

	wiphy->vendor_commands	= wl_vendor_cmds;
	wiphy->n_vendor_commands = ARRAY_SIZE(wl_vendor_cmds);
	wiphy->vendor_events	= wl_vendor_events;
	wiphy->n_vendor_events	= ARRAY_SIZE(wl_vendor_events);

#ifdef DEBUGABILITY
	dhd_os_dbg_register_callback(FW_VERBOSE_RING_ID, wl_cfgvendor_dbg_ring_send_evt);
	dhd_os_dbg_register_callback(FW_EVENT_RING_ID, wl_cfgvendor_dbg_ring_send_evt);
	dhd_os_dbg_register_callback(DHD_EVENT_RING_ID, wl_cfgvendor_dbg_ring_send_evt);
	dhd_os_dbg_register_callback(NAN_EVENT_RING_ID, wl_cfgvendor_dbg_ring_send_evt);
	dhd_os_dbg_register_urgent_notifier(dhd, wl_cfgvendor_dbg_send_urgent_evt);
#endif /* DEBUGABILITY */

	return 0;
}

int wl_cfgvendor_detach(struct wiphy *wiphy)
{
	WL_INFORM(("Vendor: Unregister BRCM cfg80211 vendor interface \n"));

	wiphy->vendor_commands  = NULL;
	wiphy->vendor_events    = NULL;
	wiphy->n_vendor_commands = 0;
	wiphy->n_vendor_events  = 0;

	return 0;
}
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT) */
