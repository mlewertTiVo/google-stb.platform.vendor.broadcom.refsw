/******************************************************************************
 *  Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
 ******************************************************************************/

#ifndef BHDM_CONFIG_H__
#define BHDM_CONFIG_H__

#include "bhdm.h"
#include "bchp_common.h"
#include "bchp_hdmi.h"

/* HDMI Tx Configuration Options */

/*
** All HDCP Rx are required to support Short or Fast I2C Reads, where a
** I2C read without an offset indicates to read data from the default I2C
** offset of the device.  In the case of the HDCP Rx (0x74), the default
** offset is 0x08 (Ri).
**
** Some early DVI receivers did not  properly implement the Short Read
** resulting in Authentication errors.
** Disable the I2C Short Read here if the STB is having trouble authenticating
** with an older DVI Rx.  Failures might show the same HDCP R0'  value on each
** authentication attempt
*/

#define BHDM_CONFIG_HDCP_RI_SHORT_READ  1

/* enable PLL KICKSTART workaround; useful for IPSTB or excessive jitter streams */
#if (BCHP_CHIP == 7038) || (BCHP_CHIP == 7401)
#define BHDM_CONFIG_PLL_KICKSTART_WORKAROUND 1
#endif


/*
** Time to wait for HDCP An Value to be generated by core
** the value should be generated within 1 frame
** default value of 50ms here is about
*/
#define BHDM_HDCP_CONFIG_AN_TIMEOUT_MS 50

#define BHDM_CONFIG_HDMI_3D_SUPPORT 1

/* available debug options; enables BDBG_MSGs etc.  */
#define BHDM_CONFIG_DEBUG_HDMI_PACKETS 0
#define BHDM_CONFIG_DEBUG_GCP_PACKET_USAGE 0
#define BHDM_CONFIG_DEBUG_FIFO 0
#define BHDM_CONFIG_DEBUG_HDCP_BCAPS 0
#define BHDM_CONFIG_DEBUG_EDID_3DSTRUCT 0
#define BHDM_CONFIG_DEBUG_EDID_3DMASK 0

#define BHDM_CONFIG_DEBUG_MAI_CHANNEL_MAP 0

#define BHDM_CONFIG_LINK_INTEGRITY_FAILURE_SIMULATION 0

#define BHDM_CONFIG_DEBUG_PJ_CHECKING 0

#define BHDM_CONFIG_DEBUG_TMDS 0
#define BHDM_CONFIG_DEBUG_RSEN 0


/* enable if BTMR is passed in BHDM_Open settings */
#define BHDM_CONFIG_BTMR_SUPPORT 1

#if BHDM_CONFIG_BTMR_SUPPORT
/* set BHDM_CONFIG_HOTPLUG_DELAY_MS (milliseconds) to delay hotplug processing */
/*
-- this works as a filter when connected to HDMI Rxs that do not drive the HPD
-- signal to less than the specified 2.0v.  Voltage between 2 and 4 are undefined
-- and can generate spurious Hotplug interrupts
-- this setting is only required for 65nm chips as other chips have
-- a built in hardware filter
-- Do not set the delay above 100ms as correct missed interrupts can occur.
--
-- Non-65nm chips (40nm, 28nm, and smaller) do not need this setting,
-- however, the delay can be enabled as a DEBUG TOOL, if there is an issue
-- with a particular TV.  Prior to using this setting make sure the problematic TV
-- has the latest up to date firmware which may fix the issue.  TV manufacturers
-- find/fix bugs all the time. Use of this setting is NOT RECOMMENDED as it will
-- delay HP processing by the time specified.  As with 65nm setting:
-- Do not set the delay above 100ms as correct missed interrupts can occur.
*/
#if BHDM_CONFIG_65NM_SUPPORT
#define BHDM_CONFIG_HOTPLUG_DELAY_MS 70
#else
#define BHDM_CONFIG_HOTPLUG_DELAY_MS 0
#endif

/* STATUS monitors */
/* periodic timer (seconds) to read debug/status registers
--    detect stable input formats to HDMI core
--    detect RxSense changes
--    detect rapid HPD changes
*/

/* periodic timer to monitor/confirm stable format into HDMI core from BVN */
/* available only on chips with FORMAT_DET_* registers */
#if BCHP_HDMI_FORMAT_DET_CFG
/* monitor format change every N seconds */
/* 0 = disabled */
#define BHDM_CONFIG_MONITOR_FORMAT_CHANGE_SECONDS 5
#endif

/* enable/disable monitor for excessive HPD changes */
/* Hot Plug Detect is monitored every second */
/* 0 = disabled */
#define BHDM_CONFIG_MONITOR_HPD_CHANGES 1

/* periodic timer (seconds) to monitor status registers i.e. RxSense */
/* 0 = disabled */
#define BHDM_CONFIG_MONITOR_STATUS_SECONDS 1
#endif

/*
Some platforms can translate alternative NTSC,480p formats (480+ lines) to
digital formats (exactly 480 lines).   If using older code, this config option
can be disabled to allow for successful compilation
*/

#define BHDM_CONFIG_ANALOG_TRANSLATION_SUPPORT 1


/******************************************************************/
/* chip revision specific configuration - DO NOT MODIFY ANY BELOW */
/******************************************************************/

#if BCHP_HDMI_TX_AUTO_I2C_REG_START
#define BHDM_CONFIG_HAS_HDCP22 1
#endif


/* all chips to use polling to determine Receiver Sense */
#define BHDM_CONFIG_RSEN_POLLING 1


/***************************************
        BHDM_CONFIG_DVO_SUPPORT
        - compile for HDMI Transmitter Core
*/
#define BHDM_CONFIG_TX 1

#if ((BCHP_CHIP == 7400) && (BCHP_VER >= BCHP_VER_B0)) \
||   (BCHP_CHIP == 7405) || (BCHP_CHIP == 7325) || (BCHP_CHIP == 7335) \
||   (BCHP_CHIP == 7336) || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) \
||   (BCHP_CHIP == 7420) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
||   (BCHP_CHIP == 7208) || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) \
||   (BCHP_CHIP == 7601) || (BCHP_CHIP == 7630) \
||   (BCHP_CHIP == 7635)
#define BHDM_CONFIG_65NM_SUPPORT 1
#endif

/* 40nm platforms */
#if ((BCHP_CHIP == 7422)  || (BCHP_CHIP == 7425)  || (BCHP_CHIP == 7231)  \
||   (BCHP_CHIP == 7358)  || (BCHP_CHIP == 7344)  || (BCHP_CHIP == 7346)  \
||   (BCHP_CHIP == 7552)  || (BCHP_CHIP == 7640)  || (BCHP_CHIP == 7429)  \
||   (BCHP_CHIP == 7435)  || (BCHP_CHIP == 7360)  || (BCHP_CHIP == 7362)  \
||   (BCHP_CHIP == 7584)  || (BCHP_CHIP == 7563)  || (BCHP_CHIP == 7543)  \
||   (BCHP_CHIP == 7228)  || (BCHP_CHIP == 75635) || (BCHP_CHIP == 73625) \
||   (BCHP_CHIP == 75845) || (BCHP_CHIP == 74295) || (BCHP_CHIP == 75525) \
||   (BCHP_CHIP == 73465) )

#define BHDM_CONFIG_40NM_SUPPORT 1
#define BHDM_CONFIG_SCHEDULER_REV_2 1
#define BHDM_CONFIG_HDCP_ADVANCED_HW_AUTO_RI_PJ_SUPPORT 1

#else

#define BHDM_CONFIG_CEC_LEGACY_SUPPORT 1
#endif

/* 28nm platforms */
#if BHDM_HAS_HDMI_20_SUPPORT || (BCHP_CHIP==74371)
#define BHDM_CONFIG_28NM_SUPPORT 1
#define BHDM_CONFIG_SCHEDULER_REV_2 1
#define BHDM_CONFIG_HDCP_ADVANCED_HW_AUTO_RI_PJ_SUPPORT 1
#endif

#if BHDM_HAS_HDMI_20_SUPPORT
#define BHDM_CONFIG_MAX_TMDS_RATE BHDM_HDMI_2_0_MAX_RATE
#define BHDM_CONFIG_4Kx2K_60HZ_SUPPORT 1
#else
#define BHDM_CONFIG_MAX_TMDS_RATE BHDM_HDMI_1_4_MAX_RATE
#endif



#if (((BCHP_CHIP == 7422) || (BCHP_CHIP == 7425) || (BCHP_CHIP == 7231) \
||    (BCHP_CHIP == 7358) || (BCHP_CHIP == 7344) || (BCHP_CHIP == 7346) \
||    (BCHP_CHIP == 7640)) \
&&    (BCHP_VER < BCHP_VER_B0)) \
|| (BCHP_CHIP == 7552) || (BCHP_CHIP == 7543)

#define BHDM_CONFIG_SINGLE_CEC_INTERRUPT  1
#endif


#if ((BCHP_VER >= BCHP_VER_B0) \
&&    ((BCHP_CHIP == 7422) || (BCHP_CHIP == 7425) || (BCHP_CHIP == 7231) \
||     (BCHP_CHIP == 7358) || (BCHP_CHIP == 7344) || (BCHP_CHIP == 7346) \
||     (BCHP_CHIP == 7640))) \
 \
|| (BCHP_CHIP == 7429) || (BCHP_CHIP == 7435)  || (BCHP_CHIP == 7360)  \
|| (BCHP_CHIP == 7362) || (BCHP_CHIP == 7228)  || (BCHP_CHIP == 7563)  \
|| (BCHP_CHIP == 7584) || (BCHP_CHIP == 75635) || (BCHP_CHIP == 7543)  \
|| (BCHP_CHIP == 73625)|| (BCHP_CHIP == 75845) || (BCHP_CHIP == 74295) \
|| (BCHP_CHIP == 75525)|| (BCHP_CHIP == 73465) \
|| BHDM_CONFIG_28NM_SUPPORT

#define BHDM_CONFIG_DUAL_CEC_TXRX_INTERRUPT 1
#define BHDM_CONFIG_PIXEL_OVERRIDE_UPDATE 1
#define BHDM_CONFIG_CLOCK_STOP_SUPPORT 1
#endif


#if (BCHP_CHIP == 7435) || (BCHP_CHIP == 7436) || (BCHP_CHIP == 7360) \
||  (BCHP_CHIP == 7362) || (BCHP_CHIP == 7228) || (BCHP_CHIP == 7584)   \
|| ((BCHP_CHIP == 7429) && (BCHP_VER >= BCHP_VER_B0)) \
|| (BCHP_CHIP == 7563) || (BCHP_CHIP == 7543)  || (BCHP_CHIP == 75635) \
|| (BCHP_CHIP == 73625) || (BCHP_CHIP == 75845) || (BCHP_CHIP == 74295) \
|| (BCHP_CHIP == 75525) \
|| BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_DUAL_HPD_SUPPORT 1
#endif


  /* 7231, 7358 and 7552 requires swapping of the HDMI_TX_PHY channel settings
         The default values/settings do not work */
#if (BCHP_CHIP == 7231) || (BCHP_CHIP == 7358) || (BCHP_CHIP == 7552) || (BCHP_CHIP == 7543) \
 || (BCHP_CHIP == 7346) || (BCHP_CHIP == 7344) || (BCHP_CHIP == 7429) || (BCHP_CHIP == 74295)

/* the following macro is used to correctly read the clock channel which is internally swapped  */
#define BHDM_CONFIG_SWAP_DEFAULT_PHY_CHANNELS 1

#if (BCHP_CHIP == 7358)  || (BCHP_CHIP == 7552) || (BCHP_CHIP == 7543) \
 || (BCHP_CHIP == 7346)  || (BCHP_CHIP == 7344) || (BCHP_CHIP == 7429) \
 || (BCHP_CHIP == 74295) || (BCHP_CHIP == 73465)
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TXCK_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX2_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX1_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX0_OUT_INV 0

#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TXCK_OUT_SEL 2
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX2_OUT_SEL 3
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX1_OUT_SEL 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX0_OUT_SEL 1
#else
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TXCK_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX2_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX1_OUT_INV 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX0_OUT_INV 0

#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TXCK_OUT_SEL 3
#if defined NEXUS_USE_7250_DGL && (BCHP_CHIP == 7250 && BCHP_VER >= BCHP_VER_B0) /* Male HDMI connector (all dongle boards) */
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX2_OUT_SEL 0
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX1_OUT_SEL 1
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX0_OUT_SEL 2
#else
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX2_OUT_SEL 2
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX1_OUT_SEL 1
#define BHDM_CONFIG_HDMI_TX_PHY_CHANNEL_SWAP_TX0_OUT_SEL 0
#endif
#endif

#endif


#if BHDM_CONFIG_65NM_SUPPORT \
||  BHDM_CONFIG_40NM_SUPPORT \
||      BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_RECEIVER_SENSE_SUPPORT 1

#define BHDM_CONFIG_PRE_EMPHASIS_SUPPORT 1
#endif



#if (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7125) \
||  (BCHP_CHIP == 7340) ||  (BCHP_CHIP == 7342) \
||  (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) || (BCHP_CHIP == 7550) \
||  (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
|| BHDM_CONFIG_40NM_SUPPORT \
|| BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_HDMI_1_3_SUPPORT 1
#endif

#if (BCHP_CHIP == 7425)  || (BCHP_CHIP == 7429) || (BCHP_CHIP == 7435) \
 || (BCHP_CHIP == 74295) || BHDM_CONFIG_28NM_SUPPORT

#define BHDM_CONFIG_4Kx2K_30HZ_SUPPORT 1
#endif


/* check if a Tx Box is using DVO */
#if !defined(BHDM_CONFIG_DVO_SUPPORT)
#if BHDM_ENABLE_DVO
#define BHDM_CONFIG_DVO_SUPPORT 1
#endif
#endif


/***************************************
        BHDM_CONFIG_DISABLE_MUX_VSYNC_ON_AVMUTE
        - disable mux_vsync when using AvMute in case HDCP is enabled
        - See PR11060
****************************************/
#if (BCHP_CHIP == 7038) \
        && ((BCHP_VER <= BCHP_VER_B1))
#define BHDM_CONFIG_DISABLE_MUX_VSYNC_ON_AVMUTE 1
#endif


/***************************************
        BHDM_CONFIG_DISABLE_HDCP_AUTH_REPEATER_DEVCOUNT0
        - revoke authentication for repeaters with an attached device count of 0
        - See PR15217
***************************************/
#if (BCHP_CHIP == 7038) \
        && (((BCHP_VER >= BCHP_VER_B0) && (BCHP_VER <= BCHP_VER_B2)) \
        ||        ((BCHP_VER <= BCHP_VER_C1)))

#define BHDM_CONFIG_DISABLE_HDCP_AUTH_REPEATER_DEVCOUNT0 1
#endif


/***************************************
        BHDM_CONFIG_DRIVER_AMP_DEFAULT
****************************************/
#if (BCHP_CHIP == 7038)

#if BCHP_VER <= BCHP_VER_B2
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 6

#elif BCHP_VER >= BCHP_VER_C0
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 4

#else
#error "Unknown 7038 revision; need to specify PREDRIVER_AMP_DEFAULT value"
#endif

#elif (BCHP_CHIP == 7438) || (BCHP_CHIP == 7440) || (BCHP_CHIP == 7403)
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 5

#elif (BCHP_CHIP == 7401)
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 4

#elif ((BCHP_CHIP == 7400) && (BCHP_VER >= BCHP_VER_B0)) || (BCHP_CHIP == 7405) \
        || (BCHP_CHIP == 7325) || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336)
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 3

#elif (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
   || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
   || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
   || BHDM_CONFIG_40NM_SUPPORT \
   || BHDM_CONFIG_28NM_SUPPORT \
   || BHDM_HAS_HDMI_20_SUPPORT
#define BHDM_CONFIG_DRIVER_AMP_DEFAULT 5

#elif !defined(BHDM_CONFIG_TX)
        /* not used */
#else
#error "New/Unknown chip; no PREDRIVER_AMP_DEFAULT value"
#endif


/***************************************
        BHDM_CONFIG_DVO_7038BX_VERSION

        changes DVO pins in starting in 7038 C0
***************************************/

#if (BCHP_CHIP == 7038)

#if (BCHP_VER >=  BCHP_VER_B0) && (BCHP_VER <= BCHP_VER_B2)
#define BHDM_CONFIG_DVO_7038BX_VERSION 1
#else
#define BHDM_CONFIG_DVO_7038CX_VERSION 1
#endif

#elif (BCHP_CHIP == 7401)
#define BHDM_CONFIG_DVO_7401_VERSION 1
#endif


/***************************************
        BHDM_CONFIG_RDB_NAME_FIX
*/

#if (BCHP_CHIP == 7038) || (BCHP_CHIP == 7438) || (BCHP_CHIP == 3560)
#define BHDM_CONFIG_RDB_NAME_FIX 1
#endif


/***************************************
        BHDM_CONFIG_88_2KHZ_AUDIO_SUPPORT
***************************************/
#if ((BCHP_CHIP == 7038) && (BCHP_VER >= BCHP_VER_C0))  || (BCHP_CHIP == 7438) \
        || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_88_2KHZ_AUDIO_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_96KHZ_AUDIO_SUPPORT
***************************************/
#if (BCHP_CHIP == 7438) ||      (BCHP_CHIP == 7440) || (BCHP_CHIP==7400) || (BCHP_CHIP == 7405) \
        || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_96KHZ_AUDIO_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_176_4KHZ_AUDIO_SUPPORT
***************************************/
#if ((BCHP_CHIP == 7038) && (BCHP_VER >= BCHP_VER_C0))  \
        || (BCHP_CHIP == 7438) ||  (BCHP_CHIP == 7405) || (BCHP_CHIP == 7400) \
        || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336) || ((BCHP_CHIP == 7325) && (BCHP_VER >= BCHP_VER_B0)) \
        || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_176_4KHZ_AUDIO_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_192KHZ_AUDIO_SUPPORT
***************************************/
#if ((BCHP_CHIP == 7038) && (BCHP_VER >= BCHP_VER_C0))  || (BCHP_CHIP == 7438) \
        || (BCHP_CHIP == 7440) || (BCHP_CHIP == 7443) || (BCHP_CHIP==7400) || (BCHP_CHIP == 7405) \
        || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336) || (BCHP_CHIP == 7325) || (BCHP_CHIP == 7401) \
        || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_192KHZ_AUDIO_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_1080P_5060HZ_SUPPORT

        7340/7342/7408/7208 uses dvp_ht_lite core, does not support 1080p/60 & 1080p/50
***************************************/
#if (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) || (BCHP_CHIP == 7468) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_1080P_5060HZ_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_1366_FORMAT_CHECK
**************************************/
#if !((BCHP_CHIP == 7401) || (BCHP_CHIP == 7038))
#define BHDM_CONFIG_1366_FORMAT_CHECK 1
#endif


#if BHDM_CEC_SUPPORT
/***************************************
        BHDM_CONFIG_CEC_DEVICE_TYPE
**************************************/
#if (BCHP_CHIP == 7438) || (BCHP_CHIP == 7440) || (BCHP_CHIP == 7601) \
        || (BCHP_CHIP==7635) || (BCHP_CHIP == 7630) || (BCHP_CHIP == 7640)
#define BHDM_CONFIG_CEC_DEVICE_TYPE 0x04
#else
#define BHDM_CONFIG_CEC_DEVICE_TYPE 0x03
#endif


/***************************************
        Uninitialized CEC logical address
**************************************/
#define BHDM_CONFIG_CEC_UNINITIALIZED_LOGICAL_ADDR 0xFF


/***************************************
        Older platforms use CEC PAD_SW_RESET register
        7601/7420 and newer platforms no longer use this.
*/
#if (BCHP_CHIP == 7038) || (BCHP_CHIP == 7438) || (BCHP_CHIP == 7440) || (BCHP_CHIP==7400) \
        || (BCHP_CHIP == 7401) || (BCHP_CHIP == 7405) || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336) \
        || (BCHP_CHIP == 7325)
#define BHDM_CONFIG_CEC_USE_PAD_SW_RESET 1
#endif


#endif


/***************************************
        Broadcom Supported audio formats;
        use to determine support for Audio Formats specified in EDID
**************************************/
#if (BCHP_CHIP == 7401) || (BCHP_CHIP == 7400) || (BCHP_CHIP == 7402) || (BCHP_CHIP == 7403) \
        || (BCHP_CHIP == 7405) || (BCHP_CHIP == 7325) || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336) \
        || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) || (BCHP_CHIP == 7550) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_AUDIO_SUPPORT_DDP 1
#endif


#if (BCHP_CHIP == 7401) ||      (BCHP_CHIP == 7400) || (BCHP_CHIP == 7402) || (BCHP_CHIP == 7403) \
        || (BCHP_CHIP == 7405) || (BCHP_CHIP == 7325) || (BCHP_CHIP == 7335) || (BCHP_CHIP == 7336)
#define BHDM_CONFIG_AUDIO_SUPPORT_WMAPRO 1
#endif


#if  (BCHP_CHIP == 7440) || (BCHP_CHIP == 7601) || (BCHP_CHIP == 7420) || (BCHP_CHIP == 7635) \
        || (BCHP_CHIP == 7630) || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) || (BCHP_CHIP == 7125) \
        || (BCHP_CHIP == 7550) || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_AUDIO_SUPPORT_DTSHD 1
#define BHDM_CONFIG_AUDIO_SUPPORT_MATMLP 1
#endif


/***************************************
        Force sending ENC_DIS signal from PI
        if this is not handled by the hardware
***************************************/
#if ((BCHP_CHIP==7400) && (BCHP_VER>=BCHP_VER_E0))
        /* Do not force ENC_DIS/ENC_EN signal */
#else
#define BHDM_CONFIG_HDCP_FORCE_ENC_SIGNAL 1
#endif


/***************************************
        BHDM_CONFIG_AUTO_RI_PJ_CHECK
***************************************/
#if ((BCHP_CHIP == 7601) && (BCHP_VER >= BCHP_VER_B0)) \
        || (BCHP_CHIP == 7420) || (BCHP_CHIP==7125) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) \
        || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7550) || (BCHP_CHIP == 7408) \
        || (BCHP_CHIP == 7468) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_HDCP_AUTO_RI_PJ_CHECKING_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_AUDIO_MAI_BUS_DISABLE_SUPPORT
**************************************/
#if ((BCHP_CHIP == 7601) && (BCHP_VER >= BCHP_VER_B0)) \
        || (BCHP_CHIP == 7420) || (BCHP_CHIP==7125) \
        || (BCHP_CHIP == 7340) || (BCHP_CHIP == 7342) \
        || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) \
        || (BCHP_CHIP == 7550) || (BCHP_CHIP == 7468) \
        || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7208) \
        || BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_AUDIO_MAI_BUS_DISABLE_SUPPORT 1
#endif


/***************************************
        BHDM_CONFIG_REGISTER_GCP_FOR_AV_MUTE
        use Register based General Control Packet (GCP)
        vs RAM Packet based General Control Packet
***************************************/
#if BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define BHDM_CONFIG_REGISTER_GCP_FOR_AV_MUTE 1
#endif


/***************************************
        BHDM_CONFIG_BLURAY_PLATFORMS
***************************************/
#if (BCHP_CHIP == 7601) || (BCHP_CHIP == 7635) || (BCHP_CHIP == 7630) || (BCHP_CHIP == 7640)
#define BHDM_CONFIG_BLURAY_PLATFORMS
#endif


/*****************************************
        PRE-EMPHASIS Settings to work around bad PLL in some 40nm
        A0 platforms. This hardware     issue is fixed in December 2010
******************************************/
#if (((BCHP_CHIP == 7422) || (BCHP_CHIP == 7425) || (BCHP_CHIP == 7231) \
        || (BCHP_CHIP == 7344) || (BCHP_CHIP == 7346) || (BCHP_CHIP == 7640)) \
        && (BCHP_VER < BCHP_VER_A1))
#define BHDM_CONFIG_40NM_PREEMP_WORKAROUND      1
#endif


/***************************************
        BHDM_CONFIG_AUDIO_MAI_BUS_CHANNEL_MAP_1TO1
***************************************/
#ifdef BHDM_CONFIG_BLURAY_PLATFORMS
#define BHDM_CONFIG_AUDIO_MAI_BUS_CHANNEL_MAP_1TO1 1
#endif


/***************************************
        CRC circuit only available in DVP_HT but not the FALCON
****************************************/
#if BHDM_CONFIG_HDMI_1_3_SUPPORT
#define BHDM_CONFIG_CRC_SUPPORT 1

#if BHDM_CONFIG_40NM_SUPPORT \
        || BHDM_CONFIG_28NM_SUPPORT
#define REGADDR_CRC_CHECK_STATUS_0 BCHP_DVP_HT_HDMI_TX_0_IN_CRC_CHECK_STATUS_0
#define REGNAME_CRC_CHECK_STATUS_0 DVP_HT_HDMI_TX_0_IN_CRC_CHECK_STATUS_0

#elif BHDM_CONFIG_65NM_SUPPORT
#define REGADDR_CRC_CHECK_STATUS_0 BCHP_DVP_HT_HDMI_TX_0_IN_DVP_CRC_CHECK_STATUS_0
#define REGNAME_CRC_CHECK_STATUS_0 DVP_HT_HDMI_TX_0_IN_DVP_CRC_CHECK_STATUS_0
#endif

#endif  /*end BHDM_CONFIG_HDMI_1_3_SUPPORT */


#if BHDM_CONFIG_BTMR_SUPPORT

/* Chips that support HSync Rise and Fall saturation detection */
#if BHDM_CONFIG_40NM_SUPPORT \
||      BHDM_CONFIG_28NM_SUPPORT \
|| (BCHP_CHIP == 7125) \
|| (BCHP_CHIP == 7208) || (BCHP_CHIP == 7408) || (BCHP_CHIP == 7468) \
|| (BCHP_CHIP == 7231)
#define BHDM_CONFIG_FORMAT_DETECT_HSYNC_SATURATION_SUPPORT 1
#endif

#endif

#if BCHP_DVP_HT_1_REG_START
#define BHDM_HAS_MULTIPLE_PORTS 1
#define BHDM_CONFIG_NUM_HDMI_CORES 2
#else
#define BHDM_CONFIG_NUM_HDMI_CORES 1
#endif


#if BCHP_MPM_CPU_CTRL_REG_START
#define BHDM_CONFIG_MHL_SUPPORT 1
#endif


/* For boot loader usage */
#ifdef BHDM_FOR_BOOTUPDATER

#ifdef BHDM_CONFIG_HDMI_3D_SUPPORT
#undef BHDM_CONFIG_HDMI_3D_SUPPORT
#endif

#ifdef BHDM_CONFIG_DEBUG_FIFO
#undef BHDM_CONFIG_DEBUG_FIFO
#endif

#ifdef BHDM_CONFIG_DEBUG_RSEN
#undef BHDM_CONFIG_DEBUG_RSEN
#endif

#ifdef BHDM_CONFIG_BTMR_SUPPORT
#undef BHDM_CONFIG_BTMR_SUPPORT
#endif

#ifdef BHDM_CONFIG_RECEIVER_SENSE_SUPPORT
#undef BHDM_CONFIG_RECEIVER_SENSE_SUPPORT
#endif

#ifdef BHDM_CONFIG_RSEN_POLLING
#undef BHDM_CONFIG_RSEN_POLLING
#endif

#ifdef BHDM_CONFIG_MONITOR_FORMAT_CHANGE_SECONDS
#undef BHDM_CONFIG_MONITOR_FORMAT_CHANGE_SECONDS
#endif

#ifdef BHDM_CONFIG_MONITOR_STATUS_SECONDS
#undef BHDM_CONFIG_MONITOR_STATUS_SECONDS
#endif

#endif /* ifdef BHDM_FOR_BOOTUPDATER */



#endif /* BHDM_CONFIG_H__ */

