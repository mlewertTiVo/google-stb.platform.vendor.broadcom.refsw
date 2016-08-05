/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
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
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Tue May  3 15:23:06 2016
 *                 Full Compile MD5 Checksum  1da1c3226a9ac5d06cc27c06c1aefe52
 *                     (minus title and desc)
 *                 MD5 Checksum               a58ec1309d39852a0c8fb54050da5f77
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     930
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_DS_TOPS_0_H__
#define BCHP_DS_TOPS_0_H__

/***************************************************************************
 *DS_TOPS_0 - DS Top FS clock domain Registers
 ***************************************************************************/
#define BCHP_DS_TOPS_0_FSCNT1                    0x04203000 /* [RW] Sample Rate Counter 1 */
#define BCHP_DS_TOPS_0_FSCNT2                    0x04203004 /* [RW] Sample Rate Counter 2 */
#define BCHP_DS_TOPS_0_SW_SPARE1                 0x04203090 /* [RW] DS Top Software SPARE1 Register */
#define BCHP_DS_TOPS_0_SW_SPARE2                 0x04203094 /* [RW] DS Top Software SPARE2 Register */
#define BCHP_DS_TOPS_0_SPARE1                    0x04203098 /* [RW] DS Top SPARE1 Register */
#define BCHP_DS_TOPS_0_CNTL                      0x0420309c /* [RW] DS Top Control Register */

/***************************************************************************
 *FSCNT1 - Sample Rate Counter 1
 ***************************************************************************/
/* DS_TOPS_0 :: FSCNT1 :: COUNTVAL [31:00] */
#define BCHP_DS_TOPS_0_FSCNT1_COUNTVAL_MASK                        0xffffffff
#define BCHP_DS_TOPS_0_FSCNT1_COUNTVAL_SHIFT                       0
#define BCHP_DS_TOPS_0_FSCNT1_COUNTVAL_DEFAULT                     0x00000000

/***************************************************************************
 *FSCNT2 - Sample Rate Counter 2
 ***************************************************************************/
/* DS_TOPS_0 :: FSCNT2 :: COUNTVAL [31:00] */
#define BCHP_DS_TOPS_0_FSCNT2_COUNTVAL_MASK                        0xffffffff
#define BCHP_DS_TOPS_0_FSCNT2_COUNTVAL_SHIFT                       0
#define BCHP_DS_TOPS_0_FSCNT2_COUNTVAL_DEFAULT                     0x00000000

/***************************************************************************
 *CFLPA_%i - DS Carrier Frequency Loop Phase accumulator
 ***************************************************************************/
#define BCHP_DS_TOPS_0_CFLPA_i_ARRAY_BASE                          0x04203050
#define BCHP_DS_TOPS_0_CFLPA_i_ARRAY_START                         0
#define BCHP_DS_TOPS_0_CFLPA_i_ARRAY_END                           1
#define BCHP_DS_TOPS_0_CFLPA_i_ARRAY_ELEMENT_SIZE                  32

/***************************************************************************
 *CFLPA_%i - DS Carrier Frequency Loop Phase accumulator
 ***************************************************************************/
/* DS_TOPS_0 :: CFLPA_i :: CFLPA [31:00] */
#define BCHP_DS_TOPS_0_CFLPA_i_CFLPA_MASK                          0xffffffff
#define BCHP_DS_TOPS_0_CFLPA_i_CFLPA_SHIFT                         0
#define BCHP_DS_TOPS_0_CFLPA_i_CFLPA_DEFAULT                       0x00000000


/***************************************************************************
 *SW_SPARE1 - DS Top Software SPARE1 Register
 ***************************************************************************/
/* DS_TOPS_0 :: SW_SPARE1 :: SPARE [31:00] */
#define BCHP_DS_TOPS_0_SW_SPARE1_SPARE_MASK                        0xffffffff
#define BCHP_DS_TOPS_0_SW_SPARE1_SPARE_SHIFT                       0
#define BCHP_DS_TOPS_0_SW_SPARE1_SPARE_DEFAULT                     0x00000000

/***************************************************************************
 *SW_SPARE2 - DS Top Software SPARE2 Register
 ***************************************************************************/
/* DS_TOPS_0 :: SW_SPARE2 :: SPARE [31:00] */
#define BCHP_DS_TOPS_0_SW_SPARE2_SPARE_MASK                        0xffffffff
#define BCHP_DS_TOPS_0_SW_SPARE2_SPARE_SHIFT                       0
#define BCHP_DS_TOPS_0_SW_SPARE2_SPARE_DEFAULT                     0x00000000

/***************************************************************************
 *SPARE1 - DS Top SPARE1 Register
 ***************************************************************************/
/* DS_TOPS_0 :: SPARE1 :: SPARE [31:00] */
#define BCHP_DS_TOPS_0_SPARE1_SPARE_MASK                           0xffffffff
#define BCHP_DS_TOPS_0_SPARE1_SPARE_SHIFT                          0
#define BCHP_DS_TOPS_0_SPARE1_SPARE_DEFAULT                        0x00000000

/***************************************************************************
 *CNTL - DS Top Control Register
 ***************************************************************************/
/* DS_TOPS_0 :: CNTL :: LFSR_ENABLE_OB [31:31] */
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_OB_MASK                    0x80000000
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_OB_SHIFT                   31
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_OB_DEFAULT                 0x00000000

/* DS_TOPS_0 :: CNTL :: LFSR_ENABLE [30:30] */
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_MASK                       0x40000000
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_SHIFT                      30
#define BCHP_DS_TOPS_0_CNTL_LFSR_ENABLE_DEFAULT                    0x00000000

/* DS_TOPS_0 :: CNTL :: reserved_for_eco0 [29:02] */
#define BCHP_DS_TOPS_0_CNTL_reserved_for_eco0_MASK                 0x3ffffffc
#define BCHP_DS_TOPS_0_CNTL_reserved_for_eco0_SHIFT                2
#define BCHP_DS_TOPS_0_CNTL_reserved_for_eco0_DEFAULT              0x00000000

/* DS_TOPS_0 :: CNTL :: FIFO_RESET_OB [01:01] */
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_OB_MASK                     0x00000002
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_OB_SHIFT                    1
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_OB_DEFAULT                  0x00000000

/* DS_TOPS_0 :: CNTL :: FIFO_RESET [00:00] */
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_MASK                        0x00000001
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_SHIFT                       0
#define BCHP_DS_TOPS_0_CNTL_FIFO_RESET_DEFAULT                     0x00000000

#endif /* #ifndef BCHP_DS_TOPS_0_H__ */

/* End of File */
