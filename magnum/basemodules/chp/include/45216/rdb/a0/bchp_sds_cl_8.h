/****************************************************************************
 *     Copyright (c) 1999-2014, Broadcom Corporation
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
 * Date:           Generated on              Thu Feb 20 15:33:52 2014
 *                 Full Compile MD5 Checksum 1766fea499add5f6ee91330ef96d35c5
 *                   (minus title and desc)
 *                 MD5 Checksum              4c358fb5b94802f03aec82d8df2c9afa
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 *
 ***************************************************************************/



/****************************************************************************
 ***************************************************************************/

#ifndef BCHP_SDS_CL_8_H__
#define BCHP_SDS_CL_8_H__

/***************************************************************************
 *SDS_CL_8 - SDS Carrier Loop Register Set
 ***************************************************************************/
#define BCHP_SDS_CL_8_CLCTL1                     0x04800180 /* Carrier Loop Control 1 (Formerly, CLQCFD,CLPDCTL,CLCTL) */
#define BCHP_SDS_CL_8_CLCTL2                     0x04800184 /* Carrier Loop Control 2 (Formerly, CLOON,CLSTS,CLMISC2,CLMISC) */
#define BCHP_SDS_CL_8_FLLC                       0x04800188 /* Set 1 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_8_FLLC1                      0x0480018c /* Set 2 front carrier loop linear coefficient and scaling values */
#define BCHP_SDS_CL_8_FLIC                       0x04800190 /* Set 1 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_8_FLIC1                      0x04800194 /* Set 2 front carrier loop integrator coefficient and scaling values */
#define BCHP_SDS_CL_8_FLSW                       0x04800198 /* Front carrier loop sweep rate */
#define BCHP_SDS_CL_8_FLI                        0x0480019c /* Front carrier loop integrator value */
#define BCHP_SDS_CL_8_FLIF                       0x048001a0 /* Front carrier loop IF frequency control word */
#define BCHP_SDS_CL_8_FLPA                       0x048001a4 /* Front carrier loop phase accumulator value */
#define BCHP_SDS_CL_8_FLTD                       0x048001a8 /* Front carrier loop frequency detector threshold */
#define BCHP_SDS_CL_8_PEEST                      0x048001ac /* Back carrier loop phase error estimator output */
#define BCHP_SDS_CL_8_PLTD                       0x048001b0 /* Back carrier loop frequency detector threshold */
#define BCHP_SDS_CL_8_PLC                        0x048001b4 /* 1st set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_8_PLC1                       0x048001b8 /* 2nd set of back carrier loop linear and integrator coefficients and scaling values */
#define BCHP_SDS_CL_8_PLSW                       0x048001bc /* Back carrier loop sweep rate */
#define BCHP_SDS_CL_8_PLI                        0x048001c0 /* Back carrier loop integrator value */
#define BCHP_SDS_CL_8_PLPA                       0x048001c4 /* Back carrier loop phase accumulator value */
#define BCHP_SDS_CL_8_CRBFD                      0x048001c8 /* Carrier loop QAM/8PSK phase detector threshold */
#define BCHP_SDS_CL_8_CLHT                       0x048001cc /* Carrier loop lock high threshold value */
#define BCHP_SDS_CL_8_CLLT                       0x048001d0 /* Carrier loop lock low threshold value */
#define BCHP_SDS_CL_8_CLLA                       0x048001d4 /* Carrier loop lock accumulator value */
#define BCHP_SDS_CL_8_CLCT                       0x048001d8 /* Carrier loop lock control */
#define BCHP_SDS_CL_8_CLFFCTL                    0x048001dc /* Fine Frequency Mixer Control */
#define BCHP_SDS_CL_8_FFLPA                      0x048001e0 /* Fine frequency adjustment phase accumulator */
#define BCHP_SDS_CL_8_CLFBCTL                    0x048001e4 /* Forward Backward Loop Control (Formerly, CLFBCTL2,CLFBCTL) */
#define BCHP_SDS_CL_8_FBLC                       0x048001e8 /* Forward Backward Loop Filter Coefficient */
#define BCHP_SDS_CL_8_FBLI                       0x048001ec /* Forward Backward Loop Filter Integrator */
#define BCHP_SDS_CL_8_FBPA                       0x048001f0 /* Forward Backward Loop Phase Accumulator */
#define BCHP_SDS_CL_8_CLDAFECTL                  0x048001f4 /* DAFE Loop Control */
#define BCHP_SDS_CL_8_DAFELI                     0x048001f8 /* DAFE Loop Filter Integrator */
#define BCHP_SDS_CL_8_DAFEINT                    0x048001fc /* DAFE Loop Filter Integrator */

#endif /* #ifndef BCHP_SDS_CL_8_H__ */

/* End of File */
