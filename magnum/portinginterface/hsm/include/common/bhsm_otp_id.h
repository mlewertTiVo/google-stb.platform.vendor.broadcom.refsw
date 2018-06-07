/******************************************************************************
 *  Copyright (C) 2018 Broadcom.
 *  The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to
 *  the terms and conditions of a separate, written license agreement executed
 *  between you and Broadcom (an "Authorized License").  Except as set forth in
 *  an Authorized License, Broadcom grants no license (express or implied),
 *  right to use, or waiver of any kind with respect to the Software, and
 *  Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 *  THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 *  IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization,
 *  constitutes the valuable trade secrets of Broadcom, and you shall use all
 *  reasonable efforts to protect the confidentiality thereof, and to use this
 *  information only in connection with your use of Broadcom integrated circuit
 *  products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 *  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 *  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 *  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 *  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 *  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 *  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 *  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 *  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 *  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 *  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 *  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/


#ifndef BHSM_OTP_ID_H__
#define BHSM_OTP_ID_H__


#include "bhsm.h"

#ifdef __cplusplus
extern "C" {
#endif


#define	BHSM_OTP_ID_LEN			8		/* OTP ID is 8 bytes long */

typedef enum BHSM_OtpIdRead_e
{
	BHSM_OtpIdRead_eOTPIdA = 0,
	BHSM_OtpIdRead_eOTPIdB = 1,
	BHSM_OtpIdRead_eOTPIdC = 2,
	BHSM_OtpIdRead_eOTPIdD = 3,
	BHSM_OtpIdRead_eOTPIdE = 4,
	BHSM_OtpIdRead_eOTPIdF = 5,
	BHSM_OtpIdRead_eOTPIdG = 6,
	BHSM_OtpIdRead_eOTPIdH = 7,
	BHSM_OtpIdRead_eMax

}	BHSM_OtpIdRead_e;

/**************************************************************************************************
Summary:

Description:
Structure that defines BHSM_ReadOTPIdIO members

See Also:
BHSM_ReadOTPId()
**************************************************************************************************/

typedef struct BHSM_ReadOTPIdIO {

	/* In:   Which OTP ID to read */
	BHSM_OtpIdRead_e		OtpId;
	/* Out: 0 for success, otherwise failed. */
	uint32_t				unStatus;
	/* Out: if usStatus is successful, this is the OTP ID, as requested */
	unsigned char			aucOTPId[BHSM_OTP_ID_LEN];
	/* Out: actual number of bytes returned for OTP ID */
	uint32_t				unIdSize;

}	BHSM_ReadOTPIdIO_t;


BERR_Code	BHSM_ReadOTPId(
    BHSM_Handle         hHsm,
    BHSM_ReadOTPIdIO_t *pReadOtpId
);


#ifdef __cplusplus
}
#endif

#endif
