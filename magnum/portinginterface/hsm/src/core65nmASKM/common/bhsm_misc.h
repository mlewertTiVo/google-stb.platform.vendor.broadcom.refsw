/******************************************************************************
 *    (c)2007-2015 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
 *
 *****************************************************************************/

#ifndef BHSM_MISC_H__
#define BHSM_MISC_H__



/* added to remove header file dependency chain*/
#include "bhsm_priv.h"
#include "bsp_s_misc.h"

#include "bhsm.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Module Specific Functions */

/**************************************************************************************************
Summary:

Description:
Structure that defines BHSM_ReadScArrayIO members

See Also:
BHSM_ReadScArray
**************************************************************************************************/
typedef struct BHSM_ReadScArrayIO {

    /* Out: 0 for success, otherwise failed. */
    unsigned int		unStatus;
    
    /* Out: This array represents the scrambling control bits (SCB) for each of the 256 
    possible pid channels (2 bits per pid channel).  
    SCB for Pid0 = bits[1:0] of unScArrayStatus[0].
    SCB for Pid15 = bits[31:30] of unScArrayStatus[0].
    SCB for Pid255 = bits[31:30] of unScArrayStatus[15]. 
    
    Translation of SCB:
        2'b11: scrambled with odd key
	    2'b10: scrambled with even key 
	    2'b00: no scrambling */
	uint32_t			unScArrayStatus[16];

} BHSM_ReadScArrayIO_t;

/**************************************************************************************************
Summary:
This function is to read out a snapshot copy of scrambling controlling bits for all PID channels.

Description:
This function reads out the current SCB values for all PID channels (256).  This function can
be called multiple times, each time returning the current SCB values for each PID channel.

Calling Context:
It can be called anytime during actual (de)scrambling is occurring for one or multiple data 
streams.

Performance and Timing:
This is a synchronous/blocking function that will not return until it completes.

Input:
in_handle - BHSM_Handle, Host Secure module handle.
inoutp_readScArrayIO - BHSM_ReadScArrayIO_t.
			
Output:
inoutp_readScArrayIO - BHSM_ReadScArrayIO_t.

Returns:
BERR_SUCCESS - success
BHSM_STATUS_FAILED - failure

See Also:
N/A
**************************************************************************************************/
BERR_Code   BHSM_ReadScArray (
		BHSM_Handle				in_handle,
		BHSM_ReadScArrayIO_t		*inoutp_readScArrayIO
);



/**************************************************************************************************
Summary:

Description:
Structure that defines BHSM_SetMiscBitsIO members

See Also:
BHSM_SetMiscBits
**************************************************************************************************/
typedef struct BHSM_SetMiscBitsIO {

	/* In: value must be '0'. */
	BCMD_SetMiscBitsSubCmd_e	setMiscBitsSubCmd;
	
	/* In: value must be '1'. */
	bool 						bEnableWriteIMem;
	
	/* In: value must be '1'. */
	bool 						bEnableReadIMem;
	
	/* In: value must be '1'. */
	bool 						bEnableReadDMem;
	
	/* In: value must be '0'. */
    bool                   		 		bEnableEncBypass;
	/* In: When set, this will remove the RAVE from reset.  RAVE is placed in reset
	whenever code is written to RAVE IMEM. */
    bool                    				bDisableClear;

	/* In: reserved */
	bool               bSetMiscReservedBit0;

	/* In: reserved */
	bool               bSetMiscReservedBit1;
	
	/* In: reserved */	
	bool               bSetMiscReservedBit2;
	
	/* In: reserved */
	bool               bSetMiscReservedBit3;
	
	/* In: reserved */	
	bool               bSetMiscReservedBit4;
	
	/* In: reserved */	
	bool               bSetMiscReservedBit5;

	/* Out: 0 for success, otherwise failed. */
	uint32_t				unStatus;

	/* Out: returns original value of RAVE bits before overwriting with new values. */	
	uint8_t 				ucRaveStatus;

} BHSM_SetMiscBitsIO_t;


/**************************************************************************************************
Summary:
This function is used to remove RAVE from reset.

Description:
This function is used to remove the RAVE from reset.  RAVE is placed in reset whenever the Host
CPU writes to RAVE IMEM.  When RAVE is removed from reset, it will begin executing code from
IMEM.  Video/Audio will not be descrambled until the RAVE is out of reset.

Calling Context:
This function is called after RAVE firmware has been loaded into IMEM and descrambling of 
video/audio is ready to begin.

Performance and Timing:
This is a synchronous/blocking function that will not return until it completes.

Input:
in_handle  - BHSM_Handle, Host Secure module handle.
inoutp_setMiscBitsIO  - BHSM_SetMiscBitsIO_t.
			
Output:
inoutp_setMiscBitsIO  - BHSM_SetMiscBitsIO_t.

Returns:
BERR_SUCCESS - success
BHSM_STATUS_FAILED - failure

See Also:
N/A
**************************************************************************************************/
BERR_Code   BHSM_SetMiscBits (
		BHSM_Handle			in_handle,
		BHSM_SetMiscBitsIO_t		*inoutp_setMiscBitsIO
);


typedef enum BCMD_PCIArchType_e
{
	BCMD_PCIArchType_eNonPCIWin,
	BCMD_PCIArchType_ePCIWin,
	BCMD_PCIArchType_ePCIeWin,

	BCMD_PCIArchType_eMax
}	BCMD_PCIArchType_e;

typedef enum BCMD_ArchSelect_e
{
	BCMD_ArchSelect_eSel00,
	BCMD_ArchSelect_eSel01,
	BCMD_ArchSelect_eSel02,
	BCMD_ArchSelect_eSel03,

	BCMD_ArchSelect_eMax

}	BCMD_ArchSelect_e;

typedef enum BCMD_DRAMSelect_e
{
	BCMD_DRAMSelect_eSel00,
	BCMD_DRAMSelect_eSel01,

	BCMD_DRAMSelect_eMax

}	BCMD_DRAMSelect_e;



/**************************************************************************************************
Summary:

Description:
Structure that defines BHSM_SetArchIO members

See Also:
BHSM_SetArch
**************************************************************************************************/
typedef struct BHSM_SetArchIO {

	/* In: defines the start address for the secure region. */
	uint32_t			unLowerRangeAddress;
	
	/* In: defines the end address (inclusive) for the secure region. */	
	uint32_t			unUpperRangeAddress;

	/* In: defines the arch Select */	
	BCMD_ArchSelect_e	ArchSel;

	/*In: defines the PCI Arch type */
	BCMD_PCIArchType_e	PCIArch;

	/*In: defines DRAM Select */
	BCMD_DRAMSelect_e	DRAMSel;	

	/* Out: 0 for success, otherwise failed. */
	uint32_t			unStatus;
} BHSM_SetArchIO_t;

/**************************************************************************************************
Summary:
This function configures the address range of secure region. It is currently designed for future 
enhanced features.

Description:
The host DRAM is partitioned into Secure Region and Global Region. This function specifies 
the address range of the secure region.  Everything outside the specified region would be  
the global region.   

Calling Context:
It shall be called after the heap is allocated for secure region and specific buffer pointers are
initialized.

Performance and Timing:
This is a synchronous/blocking function that will not return until it completes.

Input:
in_handle  - BHSM_Handle, Host Secure module handle.
inoutp_setArchIO  -  BHSM_SetArchIO_t.
			
Output:
inoutp_setArchIO  - BHSM_SetArchIO_t.

Returns:
BERR_SUCCESS - success
BHSM_STATUS_FAILED - failure

See Also:
N/A
**************************************************************************************************/
BERR_Code   BHSM_SetArch (
		BHSM_Handle			in_handle,
		BHSM_SetArchIO_t	*inoutp_setArchIO
);


/* End of Module Specific Functions */
#if BHSM_SECURE_PI_SUPPORT_MISC
#include   "bhsm_misc_enc.h"
#endif


#ifdef __cplusplus
}
#endif

#endif /* BHSM_MISC_H__ */
