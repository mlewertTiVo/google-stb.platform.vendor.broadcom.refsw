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

#ifndef ATLAS_PANEL_VBI_H__
#define ATLAS_PANEL_VBI_H__

#include "panel.h"
#include "timer.h"
#include "widget_label.h"
#include "widget_button.h"
#include "widget_check_button.h"
#include "widget_menu.h"
#include "widget_modalmsgbox.h"

#ifdef __cplusplus
extern "C" {
#endif

class CPanelVbi : public CPanel
{
public:
    CPanelVbi(
            CWidgetEngine * pWidgetEngine,
            CScreenMain *   pScreenMain,
            CWidgetBase *   pParentWidget,
            MRect           geometry,
            bwin_font_t     font,
            bwin_t          parentWin = NULL
            ); /* optional */
    ~CPanelVbi(void);

    eRet initialize(CModel * pModel, CConfig * pConfig);
    void uninitialize(void);
    eRet notifyObservers(eNotification notification, void * data, bool bHandleErrors = false);
    void onClick(bwidget_t widget);
    void processNotification(CNotification & notification);

protected:
    CWidgetMenu *        _pVbiMenu;
    CWidgetCheckButton * _ClosedCaptionPassthru;
    CWidgetCheckButton * _Teletext;
    CWidgetCheckButton * _VideoProgramSys;
    CWidgetCheckButton * _WidescreenSignaling;
    CWidgetCheckButton * _Cgms;
    CWidgetCheckButton * _Gemstar;
    CWidgetButton *      _NielsenAmol;
    CWidgetLabel *       _NielsenAmolLabel;
    CWidgetPopupList *   _NielsenAmolPopup;
    CWidgetButton *      _Macrovision;
    CWidgetLabel *       _MacrovisionLabel;
    CWidgetPopupList *   _MacrovisionPopup;
    CWidgetButton *      _Dcs;
    CWidgetLabel *       _DcsLabel;
    CWidgetPopupList *   _DcsPopup;
    CWidgetModalMsgBox * _MsgBox;
    CWidgetButton *      _Back;
    bool                 _bHandleErrors;
};

#ifdef __cplusplus
}
#endif

#endif /* ATLAS_PANEL_VBI_H__ */