############################################################
#     (c)2007-2013 Broadcom Corporation
#
#  This program is the proprietary software of Broadcom Corporation and/or its licensors,
#  and may only be used, duplicated, modified or distributed pursuant to the terms and
#  conditions of a separate, written license agreement executed between you and Broadcom
#  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
#  no license (express or implied), right to use, or waiver of any kind with respect to the
#  Software, and Broadcom expressly reserves all rights in and to the Software and all
#  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
#  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
#  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
#
#  Except as expressly set forth in the Authorized License,
#
#  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
#  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
#  and to use this information only in connection with your use of Broadcom integrated circuit products.
#
#  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
#  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
#  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
#  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
#  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
#  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
#  USE OR PERFORMANCE OF THE SOFTWARE.
#
#  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
#  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
#  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
#  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
#  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
#  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
#  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
#  ANY LIMITED REMEDY.
#
# $brcm_Workfile: $
# $brcm_Revision: $
# $brcm_Date: $
#
# Module Description:
#
# Revision History:
#
# $brcm_Log: $
#
############################################################

#
# Conditional Options for this Makefile
#

######################
# Define this module #
######################
NEXUS_MODULES += SAGE

###################
# Module Includes #
###################
NEXUS_SAGE_PUBLIC_INCLUDES = $(NEXUS_TOP)/modules/sage/include
NEXUS_SAGE_PRECOMPILED_HEADER := $(NEXUS_TOP)/modules/sage/src/nexus_sage_module.h

##################
# Module Sources #
##################
NEXUS_SAGE_SOURCES := \
$(NEXUS_TOP)/modules/sage/src/nexus_sage.c \
$(NEXUS_TOP)/modules/sage/src/nexus_sage_utils.c \
$(NEXUS_TOP)/modules/sage/src/nexus_sage_module.c \
$(NEXUS_TOP)/modules/sage/src/nexus_sage_watchdog.c

##################
#  IMG Interface #
##################
ifeq ($(B_REFSW_OS),linuxuser)
ifeq ($(NEXUS_MODE),)
NEXUS_SAGE_SOURCES += $(NEXUS_TOP)/modules/sage/src/linuxuser/nexus_sage_image.c
endif
endif #B_REFSW_OS

######################
# SVP Interface #
######################
NEXUS_SAGE_SOURCES += $(NEXUS_TOP)/modules/sage/src/nexus_sage_svp.c
NEXUS_SAGE_PRIVATE_INCLUDES += $(BSEAV)/lib/security/sage/platforms/include
ifeq ($(NEXUS_SAGE_SVP_TEST),y)
NEXUS_SAGE_SOURCES += $(NEXUS_TOP)/modules/sage/src/nexus_sage_svp_test.c
NEXUS_SAGE_SOURCES += $(NEXUS_TOP)/modules/sage/src/nexus_sage_svp_bvn.c
NEXUS_CFLAGS += -DNEXUS_SAGE_SVP_TEST
endif

###############
# thunk layer #
###############
NEXUS_SAGE_PRIVATE_INCLUDES += $(NEXUS_TOP)/modules/sage/src $(NEXUS_SYNCTHUNK_DIR)
NEXUS_SAGE_PRIVATE_INCLUDES += $(BSEAV)/lib/security/sage/include/private
NEXUS_SAGE_SOURCES += $(NEXUS_SYNCTHUNK_DIR)/nexus_sage_thunks.c

###################
# Private Sources #
###################
NEXUS_SAGE_SOURCES += $(NEXUS_TOP)/modules/sage/src/nexus_sage_priv.c


#######################
# Module Dependencies #
#######################
NEXUS_SAGE_DEPENDENCIES := BASE CORE SECURITY

##################
# Magnum Modules #
##################
NEXUS_SAGE_MAGNUM_MODULES :=  \
    $(MAGNUM)/syslib/sagelib/bsagelib.inc

