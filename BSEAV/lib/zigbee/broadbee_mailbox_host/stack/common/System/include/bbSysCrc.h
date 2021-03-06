/******************************************************************************
* (c) 2014 Broadcom Corporation
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
******************************************************************************/
/*****************************************************************************
*
* FILENAME: $Workfile: trunk/stack/common/System/include/bbSysCrc.h $
*
* DESCRIPTION:
*   CRC module declaration.
*
* $Revision: 2000 $
* $Date: 2014-03-31 15:34:06Z $
*
****************************************************************************************/
#ifndef _SYS_CRC_H
#define _SYS_CRC_H

#include "bbSysTypes.h"
#include "bbSysUtils.h"

/**************************************************************************//**
\brief Folds sequence to the crc8.

\param [in] initValue - CRC initial value
\param [in] data - data pointer
\param [in] dataLength - sequence length

\return calculated CRC value
******************************************************************************/
INLINE uint8_t SYS_Crc8(uint8_t startValue, uint8_t *data, uint8_t dataLength)
{
    (void)data;
    (void)dataLength;
    return startValue;
}

/**************************************************************************//**
\brief Calculates CRC using CRC-16-CCITT algorithm

\param [in] initValue - CRC initial value
\param [in] byte - next byte to calculate CRC

\return  calculated CRC value
******************************************************************************/
INLINE uint16_t SYS_Crc16CcittByte(uint16_t initValue, const uint8_t oneByte)
{
    uint8_t value = oneByte ^ (initValue & 0xffU);
    value ^= value << 4U;

    return ((((uint16_t)value << 8) | ((initValue & 0xff00U) >> 8))
            ^ (uint8_t)(value >> 4) ^ ((uint16_t)value << 3));
}

/**************************************************************************//**
\brief Folds sequence to the crc16.

\param [in] initValue - CRC initial value
\param [in] data - data pointer
\param [in] dataLength - sequence length

\return calculated CRC value
******************************************************************************/
INLINE uint16_t SYS_Crc16(uint16_t startValue, const uint8_t *const data, uint8_t dataLength)
{
    uint16_t crc = startValue;
    for (uint8_t i = 0; i < dataLength; ++i)
        crc = SYS_Crc16CcittByte(crc, *(data + i));
    return crc;
}

#endif /* _SYS_CRC_H */
/* eof bbSysCrc.h */