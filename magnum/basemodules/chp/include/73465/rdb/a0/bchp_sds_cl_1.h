/****************************************************************************
 *     Copyright (c) 1999-2015, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Tue Aug 25 10:46:54 2015
 *                 Full Compile MD5 Checksum  add20cb7888302c2ee8be1277223a4e4
 *                     (minus title and desc)
 *                 MD5 Checksum               f64b4ec86ad9ad7523e5d75b1dc732c5
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     126
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
 ***************************************************************************/

#ifndef BCHP_SDS_CL_1_H__
#define BCHP_SDS_CL_1_H__

/***************************************************************************
 *SDS_CL_1 - SDS Carrier Loop Register Set
 ***************************************************************************/
#define BCHP_SDS_CL_1_CLCTL1                     0x00701180 /* [RW] Carrier Loop Control 1 (Formerly, CLQCFD,CLPDCTL,CLCTL) */
#define BCHP_SDS_CL_1_CLCTL2                     0x00701184 /* [RW] Carrier Loop Control 2 (Formerly, CLOON,CLSTS,CLMISC2,CLMISC) */
#define BCHP_SDS_CL_1_FLLC                       0x00701188 /* [RW] Set 1 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_1_FLLC1                      0x0070118c /* [RW] Set 2 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_1_FLIC                       0x00701190 /* [RW] Set 1 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_1_FLIC1                      0x00701194 /* [RW] Set 2 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_1_FLSW                       0x00701198 /* [RW] Front carrier loop sweep rate */
#define BCHP_SDS_CL_1_FLI                        0x0070119c /* [RW] Front carrier loop integrator value */
#define BCHP_SDS_CL_1_FLIF                       0x007011a0 /* [RW] Front carrier loop IF frequency control word */
#define BCHP_SDS_CL_1_FLPA                       0x007011a4 /* [RO] Front carrier loop phase accumulator value */
#define BCHP_SDS_CL_1_FLTD                       0x007011a8 /* [RW] Front carrier loop frequency detector threshold */
#define BCHP_SDS_CL_1_PEEST                      0x007011ac /* [RO] Back carrier loop phase error estimator output */
#define BCHP_SDS_CL_1_PLTD                       0x007011b0 /* [RW] Back carrier loop frequency detector threshold */
#define BCHP_SDS_CL_1_PLC                        0x007011b4 /* [RW] 1st set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_1_PLC1                       0x007011b8 /* [RW] 2nd set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_1_PLSW                       0x007011bc /* [RW] Back carrier loop sweep rate */
#define BCHP_SDS_CL_1_PLI                        0x007011c0 /* [RW] Back carrier loop integrator value */
#define BCHP_SDS_CL_1_PLPA                       0x007011c4 /* [RO] Back carrier loop phase accumulator value */
#define BCHP_SDS_CL_1_CRBFD                      0x007011c8 /* [WO] Carrier loop QAM/8PSK phase detector threshold */
#define BCHP_SDS_CL_1_CLHT                       0x007011cc /* [WO] Carrier loop lock high threshold value */
#define BCHP_SDS_CL_1_CLLT                       0x007011d0 /* [WO] Carrier loop lock low threshold value */
#define BCHP_SDS_CL_1_CLLA                       0x007011d4 /* [RW] Carrier loop lock accumulator value */
#define BCHP_SDS_CL_1_CLCT                       0x007011d8 /* [WO] Carrier loop lock control */
#define BCHP_SDS_CL_1_CLFFCTL                    0x007011dc /* [RW] Fine Frequency Mixer Control */
#define BCHP_SDS_CL_1_FFLPA                      0x007011e0 /* [RO] Fine frequency adjustment phase accumulator */
#define BCHP_SDS_CL_1_CLFBCTL                    0x007011e4 /* [RW] Forward Backward Loop Control (Formerly, CLFBCTL2,CLFBCTL) */
#define BCHP_SDS_CL_1_FBLC                       0x007011e8 /* [RW] Forward Backward Loop Filter Coefficient */
#define BCHP_SDS_CL_1_FBLI                       0x007011ec /* [RW] Forward Backward Loop Filter Integrator */
#define BCHP_SDS_CL_1_FBPA                       0x007011f0 /* [RO] Forward Backward Loop Phase Accumulator */
#define BCHP_SDS_CL_1_CLDAFECTL                  0x007011f4 /* [RW] DAFE Loop Control */
#define BCHP_SDS_CL_1_DAFELI                     0x007011f8 /* [RW] DAFE Loop Filter Integrator */
#define BCHP_SDS_CL_1_DAFEINT                    0x007011fc /* [RO] DAFE Loop Filter Integrator */

#endif /* #ifndef BCHP_SDS_CL_1_H__ */

/* End of File */
