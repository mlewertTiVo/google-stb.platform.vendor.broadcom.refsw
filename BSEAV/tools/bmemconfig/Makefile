############################################################
#	  (c)2003-2014 Broadcom Corporation
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
#  1.	  This program, including its structure, sequence and organization, constitutes the valuable trade
#  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
#  and to use this information only in connection with your use of Broadcom integrated circuit products.
#
#  2.	  TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
#  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
#  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
#  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
#  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
#  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
#  USE OR PERFORMANCE OF THE SOFTWARE.
#
#  3.	  TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
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
# Created: 08/28/2007 by Jon Garrett
#
# $brcm_Log: $
#
############################################################
# Nexus examples
B_THIS_DIR:=BSEAV/tools/bmemconfig
PHONY := all api boa clean_api clean clean_apps install nexus_headers clean_boa
.PHONY: ${PHONY}
.NOTPARALLEL: api apps boa

THEAPPS = \
	bmemconfig \
	memusage_tester

WEB_FILES = bmemconfig.html bmemconfig.js bmemconfig.png bmemconfig_faq.txt

NEXUS_PLATFORM ?= $(PLATFORM)
ifndef NEXUS_PLATFORM
$(error NEXUS_PLATFORM is not defined)
endif

ifeq ($(B_REFSW_REAL_MAKE),)
NEXUS_TOP ?= $(subst /${B_THIS_DIR},,$(CURDIR))/nexus

B_REFSW_OBJ_DIR ?= obj.${NEXUS_PLATFORM}
B_REFSW_OBJ_ROOT ?= ${NEXUS_TOP}/../${B_REFSW_OBJ_DIR}
OBJDIR ?= ${NEXUS_TOP}/../${B_REFSW_OBJ_DIR}/nexus/bin
BINDIR := ${B_REFSW_OBJ_ROOT}/nexus/bmemconfig

${PHONY} ${THEAPPS}: ${BINDIR}/exists
	@${MAKE} -C ${BINDIR} B_REFSW_REAL_NEXUS_TOP=${NEXUS_TOP} B_REFSW_REAL_MAKE=1 -f ${CURDIR}/Makefile ${MAKECMDGOALS}

${BINDIR}/exists:
	@mkdir -p $@

else # B_REFSW_REAL_MAKE
NEXUS_TOP?=${B_REFSW_REAL_NEXUS_TOP}
OBJDIR ?= ${NEXUS_TOP}/../${B_REFSW_OBJ_DIR}/nexus/bin

include $(NEXUS_TOP)/examples/Makefile_prologue.inc

APPS = $(THEAPPS)
APPS ?= $(APP)

vpath %.c ${NEXUS_TOP}/../BSEAV/tools/bmemconfig

all: $(OBJDIR)/bmemconfig_box_info.c apps

apps: $(APPS)

LDFLAGS += $(NEXUS_LDFLAGS) $(NEXUS_LD_LIBRARIES)
EXTRA_OBJS += ${NEXUS_TOP}/../${B_THIS_DIR}/${NEXUS_PLATFORM}/boxmodes.c ${NEXUS_TOP}/../${B_THIS_DIR}/memusage.c
EXTRA_OBJS += ${NEXUS_TOP}/../BSEAV/tools/bmemperf/common/bmemperf_utils.c ${NEXUS_TOP}/../BSEAV/tools/bmemperf/common/bmemperf_lib.c $(OBJDIR)/bmemconfig_box_info.c
nexus_print_options nexus_headers:
	$(MAKE) -C $(NEXUS_TOP)/build $@

# This cleans nexus and local apps
clean: clean_api clean_apps clean_boa

api:
	$(MAKE) -C $(NEXUS_TOP)/build
boa:
	$(MAKE) -C $(NEXUS_TOP)/../BSEAV/lib/boa
clean_api:
	$(MAKE) -C $(NEXUS_TOP)/build clean
clean_boa:
	$(MAKE) -C $(NEXUS_TOP)/../BSEAV/lib/boa clean

clean_apps:
	@-$(RM) $(APPS) *.d *.o *.out
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig.html
	@-$(RM) ${NEXUS_BIN_DIR}/index.html
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig.js
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig.png
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig_faq.txt
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig.cgi
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig.state.*
	@-$(RM) ${NEXUS_BIN_DIR}/bmemconfig_box_info.c
	@-$(RM) ${NEXUS_BIN_DIR}/memusage_tester*
	@-$(RM) ${NEXUS_BIN_DIR}/boxmode*.png

# This is the minimum needed to compile and link with Nexus
CFLAGS += $(NEXUS_CFLAGS) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES))
CFLAGS += -I$(NEXUS_TOP)/../BSEAV/api/include
CFLAGS += -I${NEXUS_TOP}/../${B_THIS_DIR}/${NEXUS_PLATFORM} -Wimplicit-function-declaration -Wall -lnexus -lm
CFLAGS += -I$(NEXUS_TOP)/../BSEAV/tools/bmemperf/include
CFLAGS += -I$(NEXUS_TOP)/../BSEAV/lib/media

CFLAGS += -DBMEMCONFIG_READ32_SUPPORTED

# Always build with debug
CFLAGS += -g
CFLAGS += -I$(NEXUS_TOP)/../BSEAV/tools/bmemconfig

ifeq ($(filter $(BCHP_CHIP),7445), $(BCHP_CHIP))
ifeq ($(filter $(BCHP_VER),E0), $(BCHP_VER))
BCHP_VER_LOWER=d0
BCHP_VER=D0
endif
endif

BOXMODE_FILES = $(shell ls -1v $(NEXUS_TOP)/../magnum/commonutils/box/src/$(BCHP_CHIP)/$(BCHP_VER_LOWER)/bbox_memc_box*_config.c )

# Implicit rule for building local apps
%$(APPSUFFIX): %.c $(patsubst %.o,%.c,$(EXTRA_OBJS)) $(if ${B_NEXUS_API_BUILD_COMPLETED}, , api) boa
	@echo [Compile... $@]
	$(Q_)$(CC) -o $@ $(filter %.c %.s %.o, $^) $(CFLAGS) ${CDEP_FLAG} $(LDFLAGS)
	$(Q_)echo [Copy $@ to ${NEXUS_BIN_DIR}]
	$(if $(filter $@,bmemconfig),$(Q_)cp -fp $@ $(NEXUS_BIN_DIR)/$@.cgi,$(Q_)cp -fp $@ $(NEXUS_BIN_DIR))
	$(Q_)cp -fp ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig.html ${NEXUS_BIN_DIR}
	$(Q_)cp -fp ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig.js   ${NEXUS_BIN_DIR}
	$(Q_)cp -fp ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig.png  ${NEXUS_BIN_DIR}
	$(Q_)cp -fp ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_faq.txt  ${NEXUS_BIN_DIR}
ifneq ($(wildcard ${NEXUS_TOP}/../${B_THIS_DIR}/${NEXUS_PLATFORM}/boxmode*.png),)
	$(Q_)cp -fp $(wildcard ${NEXUS_TOP}/../${B_THIS_DIR}/${NEXUS_PLATFORM}/boxmode*.png) ${NEXUS_BIN_DIR}
	$(Q_)chmod 644 ${NEXUS_BIN_DIR}/boxmode*.png
endif

### the awk command returns some type of error code to the Makefile; added echo at the end to signal to Makefile that awk command was successful
$(OBJDIR)/bmemconfig_box_info.c:
	@test -d "$(OBJDIR)" || mkdir -p $(OBJDIR)
	@echo [Create... $@ for CHIP $(BCHP_CHIP)]
ifeq ($(findstring $(BCHP_CHIP),7429 7435 7425 7231 7346),$(BCHP_CHIP))
        ### memc_0_default_config.h files was extracted from 74xxb0_single_encode_RefSW_RTS.docx document which was found on 74xx Twiki page
	@echo [Parsing for 40nm client info for chip ../$(BCHP_CHIP)/$(BCHP_VER_LOWER) and platform $(NEXUS_PLATFORM)];
	@if [ -a ${NEXUS_TOP}/../${B_THIS_DIR}/../bmemperf/$(BCHP_CHIP)/$(BCHP_VER_LOWER)/memc_0_default_config.h  ] ; then \
		awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info_pre.awk ${NEXUS_TOP}/../${B_THIS_DIR}/Makefile > $(OBJDIR)/bmemconfig_box_info.c; \
		awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info_40nm.awk ${NEXUS_TOP}/../${B_THIS_DIR}/../bmemperf/$(BCHP_CHIP)/$(BCHP_VER_LOWER)/memc_0_default_config.h >> $(OBJDIR)/bmemconfig_box_info.c || echo [Awk done]; \
		awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info_post.awk $(OBJDIR)/bmemconfig_box_info.c > $(OBJDIR)/bmemconfig_box_info.tmp || echo [Awk done]; \
		cat $(OBJDIR)/bmemconfig_box_info.tmp >> $(OBJDIR)/bmemconfig_box_info.c; \
		rm $(OBJDIR)/bmemconfig_box_info.tmp; \
	else \
		printf "\nERROR ... file ${NEXUS_TOP}/../${B_THIS_DIR}/../bmemperf/$(BCHP_CHIP)/$(BCHP_VER_LOWER)/memc_0_default_config.h does not exist\n\n"; \
		exit 1; \
	fi
else
	@echo [Parsing for 28nm client info for chip ../$(BCHP_CHIP)/$(BCHP_VER_LOWER) and platform $(NEXUS_PLATFORM)]
	@awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info_pre.awk ${NEXUS_TOP}/../${B_THIS_DIR}/Makefile > $(OBJDIR)/bmemconfig_box_info.c
###  When platforms have box mode 0, the pre.awk script above will output the DDR and SCB speeds. For platforms
###  that do NOT have box mode 0, we have to complete the initialization string that was started in the pre.awk
###  script above.
	@echo ",\"unknown DDR SCB\",\"unknown\"}" >> $(OBJDIR)/bmemconfig_box_info.c
###	@echo [BOXMODE_FILES ... $(BOXMODE_FILES)]
	@$(foreach myfile,$(BOXMODE_FILES), awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info.awk $(myfile) >> $(OBJDIR)/bmemconfig_box_info.c || echo [Awk $(myfile) done] ; ) >/dev/null
	@awk -f ${NEXUS_TOP}/../${B_THIS_DIR}/bmemconfig_box_info_post.awk $(OBJDIR)/bmemconfig_box_info.c > $(OBJDIR)/bmemconfig_box_info.tmp || echo [Awk done];
	@cat $(OBJDIR)/bmemconfig_box_info.tmp >> $(OBJDIR)/bmemconfig_box_info.c
	@rm $(OBJDIR)/bmemconfig_box_info.tmp
endif

-include *.d

install: all
	$(Q_)cp -fp ${APPS} ${NEXUS_BIN_DIR}

endif # B_REFSW_REAL_MAKE
