/***************************************************************************
 * (c) 2002-2015 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
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
 *****************************************************************************/

#ifndef ATLAS_PANEL_NETWORK_WIFI_H__
#define ATLAS_PANEL_NETWORK_WIFI_H__

#include "panel.h"
#include "timer.h"
#include "widget_progress.h"
class CWidgetMenu;
class CWidgetButton;
class CWidgetLabel;
class CWidgetGrid;
class CPanelKeyboard;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NETAPP_SUPPORT
#define PERCENT_TO_UINT16(percent)  ((percent) * 65535 / 100)

typedef enum eWifiMode
{
    eWifiMode_A,
    eWifiMode_B,
    eWifiMode_G,
    eWifiMode_N,
    eWifiMode_AC,
    eWifiMode_Max
} eWifiMode;

/* CPanelNetworkWifiProp is the tag/value button used to display each network wifi property
 * in the list widget. */
class CPanelNetworkWifiProp
{
public:
    CPanelNetworkWifiProp(
            CPanel *      pPanel,
            CWidgetMenu * pMenu,
            const char *  strName,
            bwin_font_t   font
            );
    ~CPanelNetworkWifiProp();

    eRet            initialize(CConfiguration * pCfg);
    CWidgetButton * getContainer(void) { return(_pContainer); }
    void            setConnected(bool bConnected);
    bool            isConnected(void) { return(_bConnected); }
    void            setSsid(const char * pText);
    MString         getSsid(void)       { return(_pSsid->getText()); }
    CWidgetButton * getSsidButton(void) { return(_pSsid); }
    void            setChannel(const char * pText);
    int             getChannel(void) { return(MString(_pChannel->getText()).toInt()); }
    void            setMode(eWifiMode mode);
    eWifiMode       getMode(void) { return(_mode); }
    void            setSignalLevel(uint8_t percent);
    void            setSecurity(bool bSecurity);
    bool            isSecure(void) { return(_pSecurity->isVisible()); }
    void            dump(bool bForce = false);
#ifdef NETAPP_SUPPORT
    void update(NETAPP_WIFI_AP_INFO * pInfo);
#endif
    /* comparison operator overload */
    bool operator ==(const NETAPP_WIFI_AP_INFO & rhs);

protected:
    CConfiguration *           _pCfg;
    CWidgetListViewContainer * _pContainer;
    CWidgetLabel *             _pContainerBkgnd;
    CWidgetLabel *             _pConnection;
    CWidgetButton *            _pSsid;
    CWidgetLabel *             _pChannel;
    CWidgetLabel *             _pModeA;
    CWidgetLabel *             _pModeB;
    CWidgetLabel *             _pModeG;
    CWidgetLabel *             _pModeN;
    CWidgetLabel *             _pModeAC;
    CWidgetProgress *          _pSignalLevel;
    CWidgetLabel *             _pSecurity;
    CWidgetLabel *             _pModeToLabel[eWifiMode_Max + 1];
    eWifiMode                  _mode;
    bool _bConnected;
};

/* CPanelNetworkWifiStatus is the tag/value button used to display each tuner property
 * in the list widget. */
class CPanelNetworkWifiStatus
{
public:
    CPanelNetworkWifiStatus(
            CPanel *      pPanel,
            CWidgetMenu * pMenu,
            const char *  strName,
            bwin_font_t   font,
            uint8_t       labelPercentage = 50
            )
    {
        eRet ret = eRet_Ok;

        BDBG_ASSERT(NULL != pPanel);
        ret = pPanel->addDualLabelButton(pMenu, strName, &_pContainer, &_pTag, &_pValue, font, labelPercentage);
        if (eRet_Ok != ret)
        {
            fprintf(stderr, "unable to allocate double label button");
            goto error;
        }
        _pContainer->setFocusable(false);

error:
        return;
    }

    ~CPanelNetworkWifiStatus()
    {
        DEL(_pContainer);
        DEL(_pTag);
        DEL(_pValue);
    }

    CWidgetButton * getContainer(void) { return(_pContainer); }

    MString getTag(void) { return(MString(_pTag->getText())); }
    void    setTag(const char * pText)
    {
        if (NULL != _pTag)
        {
            _pTag->setText(pText, bwidget_justify_horiz_left, bwidget_justify_vert_middle);
        }
    }

    MString getValue(void) { return(MString(_pValue->getText())); }
    void    setValue(const char * pText)
    {
        if (NULL != _pValue)
        {
            _pValue->setText(pText, bwidget_justify_horiz_left, bwidget_justify_vert_middle);
        }
    }

protected:
    CWidgetButton * _pContainer;
    CWidgetLabel *  _pTag;
    CWidgetLabel *  _pValue;
};

class CPanelNetworkWifi : public CPanel
{
public:
    CPanelNetworkWifi(
            CWidgetEngine * pWidgetEngine,
            CScreenMain *   pScreenMain,
            CWidgetBase *   pParentWidget,
            MRect           geometry,
            bwin_font_t     font,
            bwin_t          parentWin = NULL
            ); /* optional */
    ~CPanelNetworkWifi(void);

    eRet                    initialize(CModel * pModel, CConfig * pConfig);
    void                    uninitialize(void);
    int                     getConnectedIndex(void);
    void                    onClick(bwidget_t widget);
    void                    show(bool bShow);
    void                    scan(bool bStart = true);
    void                    clear(void);
    void                    clearConnectionStatus(void);
    CPanelNetworkWifiProp * findProp(const char * strSsid, const int nChannel);
    void                    setMenuTitleStatus(const char * pStr = NULL);
    void                    processNotification(CNotification & notification);
    eRet                    connect(CPanelNetworkWifiProp * pNetworkProp);
    eRet                    disconnect(CPanelNetworkWifiProp * pNetworkProp);
    CPanelNetworkWifiProp * findEmptyProp(void);
    void                    updateConnectStatus(void);
    void                    updateConnectStatus(CNetwork * pNetwork);
    eRet                    updateSignalStrength(CNetwork * pNetwork);
    void                    updateNetworkList(CNetwork * pNetwork);
    void                    updateConnectedIcon(CNetwork * pNetwork);
    void                    expand(bool bExpand = true);
    void                    startUpdateTimers(bool bStart = true);
    void                    activateProp(CPanelNetworkWifiProp * pProp, bool bActivate = true);
    bool                    isPropActive(CPanelNetworkWifiProp * pProp);
    void                    activateStatus(CPanelNetworkWifiStatus * pStatus, bool bActivate = true);
    bool                    isStatusActive(CPanelNetworkWifiStatus * pStatus);
    void                    layout(void);
    void                    dump(bool bForce = false);

protected:
    CWidgetMenu *                       _pNetworkWifiMenu;
    CWidgetMenu *                       _pPropertiesMenu;
    CWidgetMenu *                       _pStatusMenu;
    bool                                _bExpandPanel;
    CWidgetButton *                     _pExpand;
    CWidgetLabel *                      _pHeadingProperties;
    CWidgetLabel *                      _pGridTitle;
    CWidgetGrid *                       _pGrid;
    CTimer                              _timerUpdate;
    CWidgetModalMsgBox *                _MsgBoxStatus;
    CWidgetModalMsgBox *                _MsgBoxError;
    CNetwork *                          _pNetwork;
    CWidgetButton *                     _pBack;
    MAutoList <CPanelNetworkWifiProp>   _propList;
    MAutoList <CPanelNetworkWifiStatus> _statusList;
};

#endif /* ifdef NETAPP_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* ATLAS_PANEL_NETWORK_WIFI_H__ */