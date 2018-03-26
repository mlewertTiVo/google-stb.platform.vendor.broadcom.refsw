/******************************************************************************
 *  Copyright (C) 2018 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
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

 ******************************************************************************/

/******************************************************************************
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Tue Jan  9 16:23:05 2018
 *                 Full Compile MD5 Checksum  2ff608fe362eb86bea5ba570b815521f
 *                     (minus title and desc)
 *                 MD5 Checksum               f58b8ef6a7a1966baf956e0d7af563c5
 *
 * lock_release:   r_1777
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1859
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              home/pt902453/sbin/combo_header.pl
 *                 DVTSWVER                   LOCAL home/pt902453/sbin/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_HEVD_OL_CPU_DMA_1_H__
#define BCHP_HEVD_OL_CPU_DMA_1_H__

/***************************************************************************
 *HEVD_OL_CPU_DMA_1
 ***************************************************************************/
#define BCHP_HEVD_OL_CPU_DMA_1_CH0_SD_ADDR       0x00d00400 /* [RW][64] DMA 0 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH0_LCL_ADDR      0x00d00408 /* [RW][32] DMA 0 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH0_LEN           0x00d0040c /* [RW][32] DMA 0 Length */
#define BCHP_HEVD_OL_CPU_DMA_1_CH1_SD_ADDR       0x00d00410 /* [RW][64] DMA 1 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH1_LCL_ADDR      0x00d00418 /* [RW][32] DMA 1 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH1_LEN           0x00d0041c /* [RW][32] DMA 1 Length */
#define BCHP_HEVD_OL_CPU_DMA_1_CH2_SD_ADDR       0x00d00420 /* [RW][64] DMA 2 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH2_LCL_ADDR      0x00d00428 /* [RW][32] DMA 2 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH2_LEN           0x00d0042c /* [RW][32] DMA 2 Length */
#define BCHP_HEVD_OL_CPU_DMA_1_CH3_SD_ADDR       0x00d00430 /* [RW][64] DMA 3 SDRAM address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH3_LCL_ADDR      0x00d00438 /* [RW][32] DMA 3 Local Memory Address */
#define BCHP_HEVD_OL_CPU_DMA_1_CH3_LEN           0x00d0043c /* [RW][32] DMA 3 Length */
#define BCHP_HEVD_OL_CPU_DMA_1_STATUS            0x00d00440 /* [RO][32] REG_DMA_STATUS */

#endif /* #ifndef BCHP_HEVD_OL_CPU_DMA_1_H__ */

/* End of File */
