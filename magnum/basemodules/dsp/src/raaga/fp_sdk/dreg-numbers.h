/************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ************************************************************************/

/************************************************************************
 *                                                                      *
 *                    #####   ######     ###    ######                  *
 *                   ##   ##    ##      ## ##   ##   ##                 *
 *                   ##         ##     ##   ##  ##   ##                 *
 *                    #####     ##     ##   ##  ######                  *
 *                        ##    ##     ##   ##  ##                      *
 *                   ##   ##    ##      ## ##   ##                      *
 *                    #####     ##       ###    ##                      *
 *                                                                      *
 *           This file is auto generated by SCons - DO NOT EDIT!        *
 *                                                                      *
 ************************************************************************/


#if defined(__FPM1015__) && defined(GENERIC)
#  include "config/fpm1015/generic/dreg-numbers.h"
#endif

#if defined(__FP2008__) && defined(RAAGA)
#  include "config/fp2008/raaga/dreg-numbers.h"
#endif

#if defined(__FP2011__) && defined(RAAGA)
#  include "config/fp2011/raaga/dreg-numbers.h"
#endif

#if defined(__FP4015__) && defined(RAAGA)
#  include "config/fp4015/raaga/dreg-numbers.h"
#endif

#if defined(__FP2012__) && defined(MCPHY)
#  include "config/fp2012/mcphy/dreg-numbers.h"
#endif

#if defined(__FPM1015__) && defined(LEAP_PHY)
#  include "config/fpm1015/leap_phy/dreg-numbers.h"
#endif

#if defined(__FPM1015__) && defined(PMC3)
#  include "config/fpm1015/pmc3/dreg-numbers.h"
#endif

#if defined(__FP2012__) && defined(RAAGA)
#  include "config/fp2012/raaga/dreg-numbers.h"
#endif

#if !( \
     (defined(__FPM1015__) && defined(GENERIC)) || \
     (defined(__FP2008__) && defined(RAAGA)) || \
     (defined(__FP2011__) && defined(RAAGA)) || \
     (defined(__FP4015__) && defined(RAAGA)) || \
     (defined(__FP2012__) && defined(MCPHY)) || \
     (defined(__FPM1015__) && defined(LEAP_PHY)) || \
     (defined(__FPM1015__) && defined(PMC3)) || \
     (defined(__FP2012__) && defined(RAAGA)) || \
     0 )
#  error "Missing or unsupported chip and/or architecture version definition."
#endif
