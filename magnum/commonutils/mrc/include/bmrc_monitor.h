/***************************************************************************
 *     Copyright (c) 2003-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *
 * Implementation of the Realtime Memory Monitor for 7038
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/
#ifndef BMRC_MONITOR_H_
#define BMRC_MONITOR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "bmrc.h"
#include "bint.h"
#include "bmem.h"
#include "bmma_types.h"

/*=Module Overview: ********************************************************
BMRC_Monitor provides a way to monitor which hardware clients access certain blocks of memory.
BMRC_Monitor uses the Address Range Checker (ARC) hardware which is a feature of the memory controller (MEMC).
It does this via the BMRC module.

This module easily integrates with magnum's memory manager (BMEM), which automatically configures
clients based on BMEM_Alloc allocations, using the debug tag filename to associate a memory block
with a certain set of MEMC clients.

Because there are limited ARC's on each chip, BMRC_Monitor performs a combine operation on all BMRC_Monitor
regions and clients. The end result is that multiple blocks and hardware clients may be combined into a
single region. This limits the accuracy of the check. Not every bad memory access can be caught.

To print the final BMRC_Monitor configuration, enable DBG's BDBG_eMsg Level for the BMRC_Monitor module and
call BMRC_Monitor_JailUpdate.  For Settop API, set msg_modules=mem_monitor.

Example:
This shows the default configuration for BMRC_Monitor using the automatic configuration
from BMEM. Error checking code is ommited for the sake of simplicity.

void main(void)
{
	BMRC_Monitor_Handle hMonitor;
	BMEM_Handle heap;
	BREG_Handle reg;
	BINT_Handle isr;
	BCHP_Handle chp;
	BMRC_Handle mrc;
	BERR_Code rc;
	static BMEM_MonitorInterface interface;

	.... Initialize MemoryManager, RegisterInterface, InterruptInterface, ChipInterface, and MemoryRangeChecker ...

	rc = BMRC_Monitor_Open(&hMonitor, reg, isr, chp, mrc, 0, 256 * 1024 * 1024); ... Control all 256MBytes window ...
	rc = BMRC_Monitor_GetMemoryInterface(hMonitor, &interface);
	rc = BMEM_InstallMonitor(heap, &interface);
}

This shows the use of a jail for debugging a specific client:
	BMRC_Monitor_Handle hMonitor;
	BMEM_Handle heap;
	BREG_Handle reg;
	BINT_Handle isr;
	BCHP_Handle chp;
	BERR_Code rc;
	static BMEM_MonitorInterface interface;

	.... Initialize MemoryManager, RegisterInterface, InterruptInterface and ChipInterface ...

	rc = BMRC_Monitor_Open(&hMonitor, reg, isr, chp, mrc, 0, 256 * 1024 * 1024); ... Control all 256MBytes window ...
	rc = BMRC_Monitor_GetMemoryInterface(hMonitor, &interface);
	rc = BMEM_InstallMonitor(heap, &interface);
	... Performing AVD debug, so optimize range checking for AVD ...
	BMRC_Monitor_JailAdd(hMonitor, BRMM_CLIENT_AVD_BLK_0);
	BMRC_Monitor_JailAdd(hMonitor, BRMM_CLIENT_AVD_ILA_0);
	BMRC_Monitor_JailAdd(hMonitor, BRMM_CLIENT_AVD_OLA_0);
	BMRC_Monitor_JailAdd(hMonitor, BRMM_CLIENT_AVD_CAB_0);
	BMRC_Monitor_JailAdd(hMonitor, BRMM_CLIENT_AVD_SYM_0);
	BMRC_Monitor_JailUpdate(hMonitor);
}


See Also:
	BMEM_InstallMonitor
	BRMM_Open
****************************************************************************/

/*
 * Summary this type is used to specify instance of the memory monitor
 */
typedef struct BMRC_P_MonitorContext *BMRC_Monitor_Handle;

typedef struct BMRC_Monitor_Settings
{
	BMRC_AccessType eKernelBlockMode;    /* blocking mode when violations occur for kernel memory */
	BMRC_AccessType eBlockMode;          /* blocking mode when violations occur for non-kernel memory */
	unsigned ulNumCheckersToUse;  /* number of hardware checkers used by this instance of mrc monitor */
} BMRC_Monitor_Settings;

/***************************************************************************
Summary:
	Opens a realtime memory monitor

Description:
	You may create only one instance of RMM per system.

Returns:
	BERR_SUCCESS - Memory monitor was opened.

See Also:
	BMRC_Close
****************************************************************************/
BERR_Code BMRC_Monitor_Open(
		BMRC_Monitor_Handle *phMonitor,   /* [out] this argument is used to return instance (handle) of memory monitor */
		BREG_Handle hReg,                 /* RegisterInterface handle */
		BINT_Handle iIsr,                 /* InterruptInterface handle */
		BCHP_Handle hChp,                 /* ChipInterface handle */
		BMRC_Handle hMrc,                 /* Memory Range Checker Handle */
		BMMA_DeviceOffset ulMemLow,                /* lowest address to control by the memory monitor, usually 0 */
		BMMA_DeviceOffset ulMemHigh,               /* highest address to control by the memory monitor, usualy 256*2^20 - 1 */
		BMRC_Monitor_Settings *pSettings  /* monitor configuration settings.  use NULL for default settings. */
		);

/***************************************************************************
Summary:
	Closes memory monitor

Description:
	This function is used to release resources allocated by
	BMRC_Monitor_Open.

Returns:
	N/A

See Also:
	BMRC_Open
****************************************************************************/
void BMRC_Monitor_Close(
		BMRC_Monitor_Handle hMonitor /* Instance of the memory monitor */
		);

/***************************************************************************
Summary:
	Enables interrupts which were disabled

Description:
    To reduce amount of noise, once violation is detected and printed, interrupts are disabled from this particular region.
    This function would enable interrupts which were previously disabled

Returns:
	N/A
****************************************************************************/
void BMRC_Monitor_RestoreInterrupts(
    BMRC_Monitor_Handle hMonitor /* Instance of the memory monitor */
    );

/***************************************************************************
Summary:
	Gets default settings for a MRC Monitor

Description:
	This function gets the default configuration settings for a MRC
	Monitor by filling in pDefSettings.  It will fill the usMaxNumCheckers
	with the value of -1 which indicates the monitor will use the maximum
	available number of hardware checkers.  It can be overridden to specify
	a number less than that.

Returns:
	N/A

See Also:
	BMRC_Monitor_Open
****************************************************************************/
BERR_Code
BMRC_Monitor_GetDefaultSettings(
		BMRC_Monitor_Settings *pDefSettings
		);

/***************************************************************************
Summary:
	Gets current settings for a MRC Monitor

Description:
	This function gets the current configuration settings for a MRC
	Monitor by filling in pDefSettings.

Returns:
	N/A

See Also:
	BMRC_Monitor_Open
****************************************************************************/
BERR_Code
BMRC_Monitor_GetSettings(
		BMRC_Monitor_Handle hMonitor, /* Instance of the memory monitor */
		BMRC_Monitor_Settings *pSettings
		);

/***************************************************************************
Summary:
	Returns BMEM_MonitorInterface which is passed into BMEM_InstallMonitor
	for automatic configuration.

Description:
	If your system uses multiple instances of BMEM, you can pass the
	BMEM_MonitorInterface into each one.

	BMEM_MonitorInterface is passed to BMEM by reference and so it is the
	application's responsibility to insure that the instance of
	BMEM_MonitorInterface remains intact all the time while the interface
	can be referenced. For example, do not use an instance of
	BMEM_MonitorInterface which is allocated as a function's local variable.


Returns:
	BERR_SUCCESS - Memory monitor was opened.

****************************************************************************/
BERR_Code
BMRC_Monitor_GetMemoryInterface(
		BMRC_Monitor_Handle hMonitor, /* Instance of the memory monitor */
		BMEM_MonitorInterface *pInterface /* [out] memory interface */
		);

/***************************************************************************
Summary:
Print configuration of memory range checkers
****************************************************************************/
void BMRC_Monitor_Print(BMRC_Monitor_Handle hMonitor);

/***************************************************************************
Summary:
HW blocks available on the system
****************************************************************************/
typedef enum BMRC_Monitor_HwBlock {
#define BCHP_P_MEMC_DEFINE_HWBLOCK(block) BMRC_Monitor_HwBlock_e##block,
#include "memc/bchp_memc_hwblock.h"
#undef BCHP_P_MEMC_DEFINE_HWBLOCK
    BMRC_Monitor_HwBlock_eInvalid,
    BMRC_Monitor_HwBlock_eMax = BMRC_Monitor_HwBlock_eInvalid
} BMRC_Monitor_HwBlock;


/***************************************************************************
Summary:
Specifies how to use the client list
****************************************************************************/
typedef enum BMRC_Monitor_ListType {
    BMRC_Monitor_ListType_eSpecifiedClients,
    BMRC_Monitor_ListType_eOtherClients,
    BMRC_Monitor_ListType_eMax
} BMRC_Monitor_ListType;


/***************************************************************************
Summary:
Configuration for BMRC_MonitorRegion
****************************************************************************/
typedef struct BMRC_MonitorRegion_Settings {
    BMMA_DeviceOffset addr; /*  start of range */
    size_t length;
    bool exclusive; /* this controls how range [addr .. addr+length] used, if 'exclusive' set to true it matches transactions outside of the range, otherwise it matches transactions inside the range */
    bool blockRead;
    bool blockWrite;
    BMRC_Monitor_ListType listType; /* this control clients selection, 'specifiedClients' is used when matching access against known clients, and 'otherClients' is used when known list of clients which should bypass match */
} BMRC_MonitorRegion_Settings;

/***************************************************************************
Summary:
Set default values for BMRC_MonitorRegion_Settings
****************************************************************************/
void BMRC_MonitorRegion_GetDefaultSettings(BMRC_MonitorRegion_Settings *settings);


/***************************************************************************
Summary:
Instance of BMRC_MonitorRegion
****************************************************************************/
typedef struct BMRC_MonitorRegion *BMRC_MonitorRegion_Handle;

/***************************************************************************
Summary:
Add new BMRC_MonitorRegion instance to BMRC_Monitor
****************************************************************************/
BERR_Code BMRC_MonitorRegion_Add(
    BMRC_Monitor_Handle hMonitor,
    BMRC_MonitorRegion_Handle *phRegion,
    const BMRC_MonitorRegion_Settings *settings,
    const BMRC_Monitor_HwBlock *clientList,
    size_t clientListLength /* number of entries in the client list */
    );

/***************************************************************************
Summary:
Remove instance BMRC_MonitorRegion from BMRC_Monitor
****************************************************************************/
void BMRC_MonitorRegion_Remove(BMRC_Monitor_Handle hMonitor, BMRC_MonitorRegion_Handle hRegion);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif /* BMRC_MONITOR_H_ */

/* End of File */

