/***************************************************************************
 *     Copyright (c) 2003-2010, Broadcom Corporation
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
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#include "bstd.h"           /* standard types */
#include "bdbg.h"           /* Dbglib */
#include "bkni.h"			/* For critical sections */
#include "bvbi.h"           /* VBI processing, this module. */
#include "bvbi_cap.h"
#include "bvbi_priv.h"      /* VBI internal data structures */

BDBG_MODULE(BVBI);

#if (BVBI_NUM_IN656 > 0) /** { **/

/***************************************************************************
* Forward declarations of static (private) functions
***************************************************************************/

static BERR_Code BVBI_P_Decode_ApplyChanges (
	BVBI_P_Decode_Handle* pVbi_Dec, BVBI_P_Decode_CNState* cnState, bool force);

static BERR_Code BVBI_P_Decode_Set_VBI(
	BVBI_Decode_Handle decodeHandle,
	bool bEnabled,
	uint32_t ulSelect_Standard);

static BERR_Code BVBI_P_Decode_Get_VBI(
	BVBI_Decode_Handle decodeHandle,
	bool* pbEnabled,
	uint32_t ulSelect_Standard);

static void BVBI_P_Decode_ChangesApplied (BVBI_P_Decode_Handle *pVbi_Dec);


/***************************************************************************
* Implementation of API functions
***************************************************************************/

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_Create(BVBI_Handle vbiHandle,
                             BAVC_SourceId eSource,
                             BVBI_Decode_Handle *pDecodeHandle)
{
	BVBI_P_Handle *pVbi;
	BVBI_P_Decode_Handle *pVbi_Dec = NULL;
	BVBI_P_TTData* pttDataT = NULL;
	BVBI_P_TTData* pttDataB = NULL;
	BERR_Code eErr = BERR_SUCCESS;

	BDBG_ENTER(BVBI_Decode_Create);

	/* check parameters */
	BVBI_P_GET_CONTEXT(vbiHandle, pVbi);
	if((!pVbi) || (!pDecodeHandle))
	{
		BDBG_ERR(("Invalid parameter"));
		eErr = BERR_TRACE(BERR_INVALID_PARAMETER);
		goto BVBI_Decode_Create_Done;
	}

	/* This check is important.  I don't have to do this check again when
	decoding VBI data.  It saves time and simplifies the software. */
	switch (eSource)
	{
	case BAVC_SourceId_eVdec0:
		break;
	case BAVC_SourceId_e656In0:
		break;
	default:
		BDBG_ERR(("Invalid parameter"));
		eErr = BERR_TRACE(BERR_INVALID_PARAMETER);
		goto BVBI_Decode_Create_Done;
	}

	/* Check to see if the caller has already opened a decode handle that
	   controls the same video source */
	for (pVbi_Dec = BLST_D_FIRST (&pVbi->decode_contexts) ;
		 pVbi_Dec ;
		 pVbi_Dec = BLST_D_NEXT (pVbi_Dec, link))
	{
		if (pVbi_Dec->eSource == eSource)
		{
			BDBG_ERR(("Multiple handles for one VBI core"));
			eErr = BERR_TRACE(BVBI_ERR_HW_CONFLICT);
			goto BVBI_Decode_Create_Done;
		}
	}

	/* Alloc the VBI decode handle. */
	pVbi_Dec =
		(BVBI_P_Decode_Handle*)(BKNI_Malloc(sizeof(BVBI_P_Decode_Handle)));
	if(!pVbi_Dec)
	{
		eErr = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
		goto BVBI_Decode_Create_Done;
	}

	/* Clear out the context and set defaults. */
	BKNI_Memset((void*)pVbi_Dec, 0x0, sizeof(BVBI_P_Decode_Handle));
	pVbi_Dec->gsOptionsChanged = true;

	/* Initialize current/next state */
	pVbi_Dec->curr.eVideoFormat                      = BFMT_VideoFmt_eNTSC;
	pVbi_Dec->curr.ulActive_Standards                = 0x0;
	pVbi_Dec->curr.e656Format                        = BVBI_656Fmt_SAA7113;
	pVbi_Dec->curr.SMPTE291Moptions.fParseCb         =
										BVBI_Decode_656_SMPTE291MbrcmCb_isr;
	pVbi_Dec->curr.SMPTE291Moptions.fParseCbArg0     = 0x0;
	pVbi_Dec->curr.SMPTE291Moptions.bLongHeader      = false;
	pVbi_Dec->curr.SMPTE291Moptions.bBrokenDataCount = true;
	pVbi_Dec->curr.wssVline576i             = 23;
	pVbi_Dec->curr.gsOptions.baseline_top   = 10;
	pVbi_Dec->curr.gsOptions.linemask_top   = 0x01F;
	pVbi_Dec->curr.gsOptions.baseline_bot   = 273;
	pVbi_Dec->curr.gsOptions.linemask_bot   = 0x01F;
	pVbi_Dec->next = pVbi_Dec->curr;

	/* Save back pointer to VBI handle */
	pVbi_Dec->pVbi = pVbi;

	/* Remember where to decode from */
	pVbi_Dec->eSource = eSource;

	/* Initialize teletext pointers for LCO */
	pttDataT = (BVBI_P_TTData*)(BKNI_Malloc(sizeof(BVBI_P_TTData)));
	if(!pttDataT)
	{
		eErr = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
		goto BVBI_Decode_Create_Done;
	}
	BKNI_Memset((void*)pttDataT, 0x0, sizeof(BVBI_P_TTData));
	eErr = BVBI_P_TTData_Alloc (
		pVbi->hMem, BVBI_TT_MAX_LINES, BVBI_TT_MAX_LINELENGTH, pttDataT);
	if (eErr != BERR_SUCCESS)
	{
		eErr = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
		goto BVBI_Decode_Create_Done;
	}

	pttDataB = (BVBI_P_TTData*)(BKNI_Malloc(sizeof(BVBI_P_TTData)));
	if(!pttDataB)
	{
		eErr = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
		goto BVBI_Decode_Create_Done;
	}
	BKNI_Memset((void*)pttDataB, 0x0, sizeof(BVBI_P_TTData));
	eErr = BVBI_P_TTData_Alloc (
		pVbi->hMem, BVBI_TT_MAX_LINES, BVBI_TT_MAX_LINELENGTH, pttDataB);
	if (eErr != BERR_SUCCESS)
	{
		eErr = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
		goto BVBI_Decode_Create_Done;
	}

	/* Initialize 656 ancillary data parser (software) */
	if ((eSource == BAVC_SourceId_e656In0) ||
	    (eSource == BAVC_SourceId_e656In1))
	{
		if ((eErr = BERR_TRACE (BVBI_P_P656_Init (pVbi_Dec))) !=
			BERR_SUCCESS)
		{
			goto BVBI_Decode_Create_Done;
		}
	}
	
	/* Smart pointers come last. This will make the error handling logic easier
	 * for Coverity to understand. */
	BVBI_P_LCOP_CREATE (pVbi_Dec, topTTDataO, pttDataT, clink);
	BVBI_P_LCOP_CREATE (pVbi_Dec, botTTDataO, pttDataB, clink);

	/* Link decode context into VBI handle's list */
	BLST_D_INSERT_HEAD (&pVbi->decode_contexts, pVbi_Dec, link);

	/* Initialize magic number to the size of the struct */
	pVbi_Dec->ulBlackMagic = sizeof(BVBI_P_Decode_Handle);

	/* All done. now return the new fresh context to user. */
	*pDecodeHandle = (BVBI_Decode_Handle)pVbi_Dec;

BVBI_Decode_Create_Done:

	/* In case of failure, take care of partial allocations */
	if (BERR_SUCCESS != eErr)
	{
		if (NULL != pttDataB)
		{
			BVBI_P_TTData_Alloc (pVbi->hMem, 0, 0, pttDataB);
			BKNI_Free ((void*)pttDataB);
		}
		if (NULL != pttDataT)
		{
			BVBI_P_TTData_Alloc (pVbi->hMem, 0, 0, pttDataT);
			BKNI_Free ((void*)pttDataT);
		}
		if (NULL != pVbi_Dec)
			BKNI_Free((void*)pVbi_Dec);
	}

	BDBG_LEAVE(BVBI_Decode_Create);
	return eErr;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_Destroy(BVBI_Decode_Handle decodeHandle)
{
	BVBI_P_Handle *pVbi;
	BVBI_P_Decode_Handle* pVbi_Dec;
	BVBI_P_TTData* pttData;

	BDBG_ENTER(BVBI_Decode_Destroy);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Shut down any active cores */
	pVbi_Dec->next.ulActive_Standards = 0x0;
	BVBI_Decode_ApplyChanges( decodeHandle );

	/* The handle will be invalid */
	pVbi_Dec->ulBlackMagic = 0;

	/* Deallocate bulky data storage if necessary */
	pVbi = pVbi_Dec->pVbi;
	BVBI_P_LCOP_DESTROY (pVbi_Dec, topTTDataO, &pVbi->ttFreelist, clink);
	pttData = BVBI_P_LCOP_GET (pVbi_Dec, topTTDataO);
	BVBI_P_TTData_Alloc (pVbi->hMem, 0, 0, pttData);
	BKNI_Free ((void*)pttData);
	BVBI_P_LCOP_DESTROY (pVbi_Dec, botTTDataO, &pVbi->ttFreelist, clink);
	pttData = BVBI_P_LCOP_GET (pVbi_Dec, botTTDataO);
	BVBI_P_TTData_Alloc (pVbi->hMem, 0, 0, pttData);
	BKNI_Free ((void*)pttData);

	/* Shut down 656 ancillary data parser (software) */
	if ((pVbi_Dec->eSource == BAVC_SourceId_e656In0) ||
	    (pVbi_Dec->eSource == BAVC_SourceId_e656In1))
	{
		BVBI_P_P656_DeInit (pVbi_Dec);
	}

	/* Unlink from parent VBI context */
	BLST_D_REMOVE (&pVbi->decode_contexts, pVbi_Dec, link);

	/* Release context in system memory */
	BKNI_Free((void*)pVbi_Dec);

	BDBG_LEAVE(BVBI_Decode_Destroy);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_ApplyChanges( BVBI_Decode_Handle decodeHandle )
{
	BVBI_P_Decode_Handle *pVbi_Dec;
	BERR_Code eErr;

	BDBG_ENTER(BVBI_Decode_ApplyChanges);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Try to apply changes */
	eErr = BVBI_P_Decode_ApplyChanges (pVbi_Dec, &pVbi_Dec->next, false);

	/* On failure, restore state */
	if (eErr != BERR_SUCCESS)
	{
		(void)BVBI_P_Decode_ApplyChanges (pVbi_Dec, &pVbi_Dec->curr, true);
		goto done;
	}

	/* Hardware should now be fully programmed */
	BVBI_P_Decode_ChangesApplied (pVbi_Dec);

done:
	BDBG_LEAVE(BVBI_Decode_ApplyChanges);
	return eErr;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_SetVideoFormat(BVBI_Decode_Handle decodeHandle,
									 BFMT_VideoFmt eVideoFormat)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_SetVideoFormat);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Remember format as requested */
	pVbi_Dec->next.eVideoFormat = eVideoFormat;

	BDBG_LEAVE(BVBI_Decode_SetVideoFormat);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_GetVideoFormat(BVBI_Decode_Handle decodeHandle,
									 BFMT_VideoFmt *peVideoFormat)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_GetVideoFormat);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if((!pVbi_Dec) || (!peVideoFormat))
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Return format as requested */
	*peVideoFormat = pVbi_Dec->curr.eVideoFormat;

	BDBG_LEAVE(BVBI_Decode_GetVideoFormat);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_656_SetFormat(BVBI_Decode_Handle decodeHandle,
									BVBI_656Fmt anci656Fmt)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_656_SetFormat);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		BDBG_LEAVE(BVBI_Decode_SetVideoFormat);
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Remember format as requested */
	switch (anci656Fmt)
	{
	case BVBI_656Fmt_SAA7113:
	case BVBI_656Fmt_SAA7114A:
	case BVBI_656Fmt_SAA7114B:
	case BVBI_656Fmt_SAA7114C:
	case BVBI_656Fmt_SAA7115:
	case BVBI_656Fmt_SMPTE291:
		pVbi_Dec->next.e656Format = anci656Fmt;
		break;
	default:
		BDBG_LEAVE(BVBI_Decode_SetVideoFormat);
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	BDBG_LEAVE(BVBI_Decode_656_SetFormat);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_656_GetFormat(BVBI_Decode_Handle decodeHandle,
									BVBI_656Fmt* pAnci656Fmt)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_656_GetFormat);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if((!pVbi_Dec) || (!pAnci656Fmt))
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Return format as requested */
	*pAnci656Fmt = pVbi_Dec->curr.e656Format;

	BDBG_LEAVE(BVBI_Decode_656_GetFormat);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_656_SetSMPTE291Moptions (
	BVBI_Decode_Handle decodeHandle,
	BVBI_Decode_656_SMPTE291M_Cb fParserCb_isr,
	void* arg0,
	bool bLongHeader,
	bool bBrokenDataCount
)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_656_SetSMPTE291Moptions);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if ((!pVbi_Dec) || (!fParserCb_isr))
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Remember options as requested */
	pVbi_Dec->next.SMPTE291Moptions.fParseCb         = fParserCb_isr;
	pVbi_Dec->next.SMPTE291Moptions.fParseCbArg0     = arg0;
	pVbi_Dec->next.SMPTE291Moptions.bLongHeader      = bLongHeader;
	pVbi_Dec->next.SMPTE291Moptions.bBrokenDataCount = bBrokenDataCount;

	BDBG_LEAVE(BVBI_Decode_656_SetSMPTE291Moptions);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_SetGemstarOptions(
	BVBI_Decode_Handle decodeHandle,
	const BVBI_GSOptions* gsOptions
)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_SetGemstarOptions);

	/* check parameters */
	if (gsOptions == 0x0)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if (!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	if (gsOptions->baseline_bot <= 256)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Remember options as requested */
	pVbi_Dec->next.gsOptions = *gsOptions;
	pVbi_Dec->gsOptionsChanged = true;

	BDBG_LEAVE(BVBI_Decode_SetGemstarOptions);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_GetGemstarOptions(
	BVBI_Decode_Handle decodeHandle,
	BVBI_GSOptions*    gsOptions
)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_GetGemstarOptions);

	/* check parameters */
	if (gsOptions == 0x0)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if (!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Return options as requested */
	*gsOptions = pVbi_Dec->curr.gsOptions;

	BDBG_LEAVE(BVBI_Decode_GetGemstarOptions);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_SetWssOptions(
	BVBI_Decode_Handle decodeHandle,
	uint16_t              vline576i
)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_SetWssOptions);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if (!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	if (vline576i > 255)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Remember options as requested */
	pVbi_Dec->next.wssVline576i = vline576i;

	BDBG_LEAVE(BVBI_Decode_SetWssOptions);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_GetWssOptions(
	BVBI_Decode_Handle decodeHandle,
	uint16_t*            pVline576i
)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_Decode_GetWssOptions);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if (!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	if (pVline576i == 0x0)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Return options as requested */
	*pVline576i = pVbi_Dec->curr.wssVline576i;

	BDBG_LEAVE(BVBI_Decode_GetWssOptions);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_Data_isr( BVBI_Decode_Handle decodeHandle,
							    BVBI_Field_Handle  fieldHandle,
								BAVC_Polarity polarity )
{
	BVBI_P_Decode_Handle* pVbi_Dec;
	BVBI_P_Field_Handle*  pVbi_Fld;
	BVBI_P_Handle* pVbi;
	BVBI_P_Decode_CNState* currentState;
	bool bDataFound;
	uint32_t whatActive;

	BDBG_ENTER(BVBI_Decode_Data_isr);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}
	currentState = &pVbi_Dec->curr;
	pVbi = pVbi_Dec->pVbi;
	whatActive = currentState->ulActive_Standards;
	BVBI_P_GET_FIELD_CONTEXT(fieldHandle, pVbi_Fld);
	if(!pVbi_Fld)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Clear out the error bits.  The next set of function calls may
	   set them again. */
	pVbi_Fld->ulErrInfo = 0x0;
	pVbi_Fld->ulWhichPresent = 0x0;

	/* Decode the various forms of ITU-R 656 VBI data as configured */
	BVBI_P_IN656_Decode_Data_isr (
						pVbi->hReg,
						pVbi_Dec->eSource,
						polarity,
						&bDataFound);
	if (bDataFound)
		BVBI_P_P656_Process_Data_isr (polarity, pVbi_Dec, pVbi_Fld);

	/* Record the data's field polarity */
	if (polarity == BAVC_Polarity_eFrame)
		pVbi_Fld->polarityMask = (1<<BAVC_Polarity_eTopField);
	else
		pVbi_Fld->polarityMask = (1<<polarity);

	BDBG_LEAVE(BVBI_Decode_Data_isr);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
static BERR_Code BVBI_P_Decode_ApplyChanges (
	BVBI_P_Decode_Handle* pVbi_Dec, BVBI_P_Decode_CNState* cnState, bool force)
{
	BVBI_P_Handle *pVbi;
	uint32_t whatActive;
	bool isActive;
	BERR_Code eErr;
	BERR_Code firstErr = BERR_SUCCESS;

	BDBG_ENTER(BVBI_P_Decode_ApplyChanges);

	BSTD_UNUSED(force);
	BSTD_UNUSED(cnState);
	BSTD_UNUSED(pVbi_Dec);

	/* Get back pointer to VBI module, for convenience. */
	pVbi = pVbi_Dec->pVbi;

	/* More convenience */
	whatActive = cnState->ulActive_Standards;
	isActive = (whatActive != 0);

	/* Program the IN656 core */
	eErr = BERR_TRACE (BVBI_P_IN656_Dec_Program (
		pVbi->hReg,
		pVbi->hMem,
		pVbi_Dec->eSource,
		isActive,
		cnState->e656Format,
		&(cnState->SMPTE291Moptions),
		cnState->eVideoFormat,
		pVbi_Dec->top656Data,
		pVbi_Dec->bot656Data));
	if (eErr != BERR_SUCCESS)
	{
		if (firstErr == BERR_SUCCESS)
			firstErr = eErr;
		if (!force)
			goto done;
	}

done:
	BDBG_LEAVE(BVBI_P_Decode_ApplyChanges);
	return firstErr;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Decode_SetCC(BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_CC);
}
BERR_Code BVBI_Decode_SetCGMS(BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_CGMSA);
}
BERR_Code BVBI_Decode_SetWSS(BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_WSS);
}
BERR_Code BVBI_Decode_SetVPS(BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_VPS);
}
BERR_Code BVBI_Decode_SetTeletext(
	BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_TT);
}
BERR_Code BVBI_Decode_SetGemstar(BVBI_Decode_Handle decodeHandle, bool bEnabled)
{
	return BVBI_P_Decode_Set_VBI (decodeHandle, bEnabled, BVBI_P_SELECT_GS);
}

BERR_Code BVBI_Decode_GetCC(BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_CC);
}
BERR_Code BVBI_Decode_GetCGMS(BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_CGMSA);
}
BERR_Code BVBI_Decode_GetWSS(BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_WSS);
}
BERR_Code BVBI_Decode_GetVPS(BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_VPS);
}
BERR_Code BVBI_Decode_GetTeletext(
	BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_TT);
}
BERR_Code BVBI_Decode_GetGemstar(
	BVBI_Decode_Handle decodeHandle, bool* pbEnabled)
{
	return BVBI_P_Decode_Get_VBI (decodeHandle, pbEnabled, BVBI_P_SELECT_GS);
}

/***************************************************************************
* Static (private) functions
***************************************************************************/

/***************************************************************************
 *
 */
static BERR_Code BVBI_P_Decode_Set_VBI(
	BVBI_Decode_Handle decodeHandle,
	bool bEnabled,
	uint32_t ulSelect_Standard)
{
	BVBI_P_Decode_Handle *pVbi_Dec;

	BDBG_ENTER(BVBI_P_Decode_Set_VBI);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if(!pVbi_Dec)
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Record change to desired state */
	if (bEnabled)
	{
		pVbi_Dec->next.ulActive_Standards |= ulSelect_Standard;
	}
	else
	{
		pVbi_Dec->next.ulActive_Standards &= ~ulSelect_Standard;
	}

	BDBG_LEAVE(BVBI_P_Decode_Set_VBI);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
static BERR_Code BVBI_P_Decode_Get_VBI(
	BVBI_Decode_Handle decodeHandle,
	bool* pbEnabled,
	uint32_t ulSelect_Standard)
{
	BVBI_P_Decode_Handle* pVbi_Dec;

	BDBG_ENTER(BVBI_P_Decode_Get_VBI);

	/* check parameters */
	BVBI_P_GET_DECODE_CONTEXT(decodeHandle, pVbi_Dec);
	if((!pVbi_Dec) || (!pbEnabled))
	{
		BDBG_ERR(("Invalid parameter"));
		return BERR_TRACE(BERR_INVALID_PARAMETER);
	}

	/* Return info */
	*pbEnabled =
		((pVbi_Dec->curr.ulActive_Standards & ulSelect_Standard) != 0);

	BDBG_LEAVE(BVBI_P_Decode_Get_VBI);
	return BERR_SUCCESS;
}

/***************************************************************************
 *
 */
static void BVBI_P_Decode_ChangesApplied (BVBI_P_Decode_Handle *pVbi_Dec)
{
	pVbi_Dec->curr = pVbi_Dec->next;
	pVbi_Dec->gsOptionsChanged = false;
}

#endif /** }  (BVBI_NUM_IN656 > 0) **/
