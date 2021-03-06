/***************************************************************************
 *     Copyright (c) 1999-2011, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Fri Feb  4 12:04:23 2011
 *                 MD5 Checksum         651635341c0017dd0a351cfe3d5e51fe
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_SDS_EQ_4_H__
#define BCHP_SDS_EQ_4_H__

/***************************************************************************
 *SDS_EQ_4 - SDS Equalizer Register Set 4
 ***************************************************************************/
#define BCHP_SDS_EQ_4_EQMISCCTL                  0x000d0200 /* Equalizer Misc Control Register (Formerly,EQMISC,EQBLND,EQMODE) */
#define BCHP_SDS_EQ_4_EQFFECTL                   0x000d0204 /* Equalizer FFE Control Register (Formerly,EQMU,EQFFE3,EQFFE2,EQFFE1) */
#define BCHP_SDS_EQ_4_EQCFAD                     0x000d0208 /* Equalizer FFE Coefficients Control Register */
#define BCHP_SDS_EQ_4_EQFRZCTL                   0x000d020c /* Equalizer FFE Freeze Control Register (Formerly,EQFRZ3,EQFRZ2,EQFRZ1) */
#define BCHP_SDS_EQ_4_F0B                        0x000d0210 /* FFE Coefficient Read/Write */
#define BCHP_SDS_EQ_4_HD8PSK1                    0x000d0214 /* 8psk hard decision level 1 and cos(22.5deg) values */
#define BCHP_SDS_EQ_4_HD8PSK2                    0x000d0218 /* 8psk hard decision level 2 and sin(22.5deg) values */
#define BCHP_SDS_EQ_4_HDQPSK                     0x000d021c /* QPSK hard decision level */
#define BCHP_SDS_EQ_4_HD16QAM                    0x000d0220 /* 16QAM hard decision levels */
#define BCHP_SDS_EQ_4_CMA                        0x000d0224 /* CMA modulus values */
#define BCHP_SDS_EQ_4_CMATH                      0x000d0228 /* CMA Threshold */
#define BCHP_SDS_EQ_4_VLCTL                      0x000d0230 /* Viterbi Loading Control Register (Formerly,VLCTL3,VLCTL2,VLCTL1) */
#define BCHP_SDS_EQ_4_VLCI                       0x000d0234 /* VLC I-rail Gain */
#define BCHP_SDS_EQ_4_VLCQ                       0x000d0238 /* VLC Q-rail Gain */
#define BCHP_SDS_EQ_4_VCOS                       0x000d023c /* VLC 8PSK and QPSK levels */
#define BCHP_SDS_EQ_4_TSFT                       0x000d0240 /* VLC Advanced FEC Soft Decisions */
#define BCHP_SDS_EQ_4_EQSFT                      0x000d0244 /* EQ Soft Decisions */
#define BCHP_SDS_EQ_4_PILOTCTL                   0x000d024c /* Pilot Control */
#define BCHP_SDS_EQ_4_PLDCTL                     0x000d0250 /* PLDCTL */
#define BCHP_SDS_EQ_4_HDRD                       0x000d0260 /* HP Header Symbol I,Q */
#define BCHP_SDS_EQ_4_HDRA                       0x000d0264 /* HP Header Symbol Memory Address */
#define BCHP_SDS_EQ_4_XSEED                      0x000d026c /* Physical Layer Descrambler Seed */
#define BCHP_SDS_EQ_4_XTAP1                      0x000d0270 /* Physical Layer Descrambler x1 */
#define BCHP_SDS_EQ_4_XTAP2                      0x000d0274 /* Physical Layer Descrambler x2 */
#define BCHP_SDS_EQ_4_LUPD                       0x000d0278 /* Carrier loop PD loop-up-table memory read/write data */
#define BCHP_SDS_EQ_4_LUPA                       0x000d027c /* Carrier loop PD loop-up-table memory read/write address */
#define BCHP_SDS_EQ_4_SDSLEN                     0x000d0280 /* Soft Decision Signature Analyzer Symbol Length */
#define BCHP_SDS_EQ_4_SDSIG                      0x000d0284 /* Soft Decision Signature Analyzer Output */
#define BCHP_SDS_EQ_4_MGAIND                     0x000d0288 /* MODCOD Gain Table read/write data */
#define BCHP_SDS_EQ_4_MGAINA                     0x000d028c /* MODCOD Gain Table read/write address */
#define BCHP_SDS_EQ_4_CWC_CTRL                   0x000d0290 /* CW canceller control */
#define BCHP_SDS_EQ_4_CWC_SPUR_FREQ              0x000d0294 /* CW canceller FCW for spur */
#define BCHP_SDS_EQ_4_CWC_LFC                    0x000d0298 /* CW canceller loop filter coefficient */
#define BCHP_SDS_EQ_4_CWC_LF_INT                 0x000d029c /* CW canceller loop filter integrator */

#endif /* #ifndef BCHP_SDS_EQ_4_H__ */

/* End of File */
