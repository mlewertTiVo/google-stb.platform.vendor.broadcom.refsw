#############################################################################
#    (c)2013 Broadcom Corporation
# 
# This program is the proprietary software of Broadcom Corporation and/or its licensors,
# and may only be used, duplicated, modified or distributed pursuant to the terms and
# conditions of a separate, written license agreement executed between you and Broadcom
# (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
# no license (express or implied), right to use, or waiver of any kind with respect to the
# Software, and Broadcom expressly reserves all rights in and to the Software and all
# intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
# HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
# NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
#  
# Except as expressly set forth in the Authorized License,
#  
# 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
# secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
# and to use this information only in connection with your use of Broadcom integrated circuit products.
#  
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
# AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
# WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
# THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
# LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
# OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
# USE OR PERFORMANCE OF THE SOFTWARE.
# 
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
# LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
# EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
# USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
# ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
# LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
# ANY LIMITED REMEDY.
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
#############################################################################

IS_VC5 := $(shell test -e $(NEXUS_TOP)/../magnum/basemodules/chp/include/$(BCHP_CHIP)/rdb/$(BCHP_VER_LOWER)/bchp_v3d_tfu.h && echo y)
ifeq ($(IS_VC5),y)
V3D_PREFIX ?= vc5
else
V3D_PREFIX ?= v3d
endif

######################
# Define this module #
######################
NEXUS_MODULES += GRAPHICSV3D

##################
# Module Options #
##################
NEXUS_GRAPHICSV3D_OPTIONS := 

###################
# Module Includes #
###################
V3D_NEXUS_TOP ?= $(NEXUS_TOP)/modules/graphicsv3d/$(V3D_PREFIX)
NEXUS_GRAPHICSV3D_PUBLIC_INCLUDES := $(V3D_NEXUS_TOP)/include

##################
# Module Sources #
##################
NEXUS_GRAPHICSV3D_SOURCES := \
	$(V3D_NEXUS_TOP)/src/nexus_graphicsv3d.c

##################
# Module Defines #
##################

#######################
# Module Dependencies #
#######################
NEXUS_GRAPHICSV3D_DEPENDENCIES += BASE CORE

##################
# Magnum Modules #
##################
V3D_MAGNUM_TOP ?= $(MAGNUM)/portinginterface/$(V3D_PREFIX)
NEXUS_GRAPHICSV3D_MAGNUM_MODULES :=  \
	$(V3D_MAGNUM_TOP)/b$(V3D_PREFIX).inc

###############
# Thunk Layer #
###############
NEXUS_GRAPHICSV3D_PRIVATE_INCLUDES += $(V3D_NEXUS_TOP)/src $(NEXUS_SYNCTHUNK_DIR)
NEXUS_GRAPHICSV3D_SOURCES += $(NEXUS_SYNCTHUNK_DIR)/nexus_graphicsv3d_thunks.c

