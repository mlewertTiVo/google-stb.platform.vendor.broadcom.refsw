/***************************************************************************
 *     (c)2004-2013 Broadcom Corporation
 *  
 *  This program is the proprietary software of Broadcom Corporation and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
 *   
 *  Except as expressly set forth in the Authorized License,
 *   
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *   
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *  
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
 *  ANY LIMITED REMEDY.
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
 *******************************************************/
#include "nexus_platform_module.h"
#include "support/libgcc/quad.h"


#if 1
int64_t __aeabi_ldivmod_c(int64_t n, int64_t d, int64_t *rem)
{
    /* Run-time ABI for the ARM Architecture, Table 9  */
    *rem =  __moddi3(n, d);
    return __divdi3(n, d);
}

uint64_t __aeabi_uldivmod_c(uint64_t n, uint64_t d, uint64_t *rem)
{
    /* Run-time ABI for the ARM Architecture, Table 9  */
    return __qdivrem(n, d, rem);
}
#else
int64_t __aeabi_ldivmod_c(int64_t n, int64_t d, int64_t *rem);
uint64_t __aeabi_uldivmod_c(uint64_t n, uint64_t d, uint64_t *rem);

int64_t __aeabi_ldivmod(int64_t n, int64_t d)
{
    int64_t quot, rem;
    uint32_t r_lo,r_hi;
    quot = __aeabi_ldivmod_c(n, d, &rem);
    /* this function return result in 4 32-bit registers, r0,r1,r2 and r3 */
    r_lo = (uint32_t)rem;
    r_hi = (uint32_t)(rem >> 32);
     __asm__ __volatile__( /* use inline assembly to populate r2 and r3 */
        "mov %%r3,%0\n"
        "mov %%r2,%1\n"
       :
       :"r"(r_hi),"r"(r_lo)
       :"%r2","%r3"
       );

    return quot; /* use return type to populate r0 and r1 */
}

uint64_t __aeabi_uldivmod(uint64_t n, uint64_t d)
{
    /* Run-time ABI for the ARM Architecture, Table 9  */
    uint64_t quot, rem;
    uint32_t r_lo,r_hi;
    quot  = __aeabi_uldivmod_c(n, d, &rem);
    /* this function return result in 4 32-bit registers, r0,r1,r2 and r3 */
    r_lo = (uint32_t)rem;
    r_hi = (uint32_t)(rem >> 32);
     __asm__ __volatile__( /* use inline assembly to populate r2 and r3 */
        "mov %%r3,%0\n"
        "mov %%r2,%1\n"
       :
       :"r"(r_hi),"r"(r_lo)
       :"%r2","%r3"
       );

    return quot; /* use return type to populate r0 and r1 */
}
#endif

