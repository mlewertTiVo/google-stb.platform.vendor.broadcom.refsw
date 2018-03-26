/******************************************************************************
 * Copyright (C) 2018 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *****************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "bwidgets.h"
#include "widget_engine.h"
#include "mvc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CTimer is a model class used to provide timers that are synchronized with
 * with atlas bwidgets main loop.  It relies on the b_os_lib's scheduler
 * to set/clear/trigger timers.  This class can only notify CObserver classes
 * which restricts it to controller and view classes - this ensures compliance
 * with the underlying MVC architecture.
 */
class CTimer : public CMvcModel
{
public:
    CTimer(
            CWidgetEngine * pWidgetEngine,
            CObserver *     pObserver,
            int             timeout
            );
    CTimer(CObserver * pObserver);
    CTimer(CSubject * pSubject);
    ~CTimer(void);

    void            start(int timeout = -1);
    void            stop(void);
    bool            isStarted(void)         { return(NULL != _timerId); }
    void            clearTimerId(void)      { _timerId = NULL; }
    int             getTimeout(void)        { return(_timeout); }
    void            setTimeout(int timeout) { _timeout = timeout; }
    CWidgetEngine * getWidgetEngine(void)   { return(_pWidgetEngine); }
    void            setWidgetEngine(CWidgetEngine * pWidgetEngine);
    CObserver *     getObserver(void) { return(_pObserver); }
    CSubject *      getSubject(void)  { return(_pSubject); }

protected:
    CObserver *        _pObserver;
    CSubject *         _pSubject;
    B_SchedulerTimerId _timerId;
    int                _timeout;
    CWidgetEngine *    _pWidgetEngine;
};

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */