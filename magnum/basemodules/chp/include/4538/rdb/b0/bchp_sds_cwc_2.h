/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
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
 * Date:           Generated on         Thu Mar 14 11:46:11 2013
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
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

#ifndef BCHP_SDS_CWC_2_H__
#define BCHP_SDS_CWC_2_H__

/***************************************************************************
 *SDS_CWC_2 - 2 SDS CW Canceller Register Set
 ***************************************************************************/
#define BCHP_SDS_CWC_2_CTRL1                     0x000cc600 /* CWC Control 1 */
#define BCHP_SDS_CWC_2_CTRL2                     0x000cc604 /* CWC Control 2 */
#define BCHP_SDS_CWC_2_LEAK                      0x000cc608 /* CWC Leak Control */
#define BCHP_SDS_CWC_2_SPUR_FCW1                 0x000cc610 /* CWC1 NCO frequency control word */
#define BCHP_SDS_CWC_2_SPUR_FCW2                 0x000cc614 /* CWC2 NCO frequency control word */
#define BCHP_SDS_CWC_2_SPUR_FCW3                 0x000cc618 /* CWC3 NCO frequency control word */
#define BCHP_SDS_CWC_2_SPUR_FCW4                 0x000cc61c /* CWC4 NCO frequency control word */
#define BCHP_SDS_CWC_2_SPUR_FCW5                 0x000cc620 /* CWC5 NCO frequency control word */
#define BCHP_SDS_CWC_2_SPUR_FCW6                 0x000cc624 /* CWC6 NCO frequency control word */
#define BCHP_SDS_CWC_2_LFC1                      0x000cc630 /* CWC1 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_LFC2                      0x000cc634 /* CWC2 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_LFC3                      0x000cc638 /* CWC3 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_LFC4                      0x000cc63c /* CWC4 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_LFC5                      0x000cc640 /* CWC5 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_LFC6                      0x000cc644 /* CWC6 Loop Filter Coefficients */
#define BCHP_SDS_CWC_2_INT1                      0x000cc650 /* CWC1 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_INT2                      0x000cc654 /* CWC2 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_INT3                      0x000cc658 /* CWC3 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_INT4                      0x000cc65c /* CWC4 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_INT5                      0x000cc660 /* CWC5 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_INT6                      0x000cc664 /* CWC6 Loop Filter INtegrator */
#define BCHP_SDS_CWC_2_COEFF_RWCTRL              0x000cc670 /* CWC Coefficient Read/Write Control */
#define BCHP_SDS_CWC_2_COEFF                     0x000cc674 /* CWC Coefficients */
#define BCHP_SDS_CWC_2_FOFS1                     0x000cc680 /* Frequency Offset for CWC1 */
#define BCHP_SDS_CWC_2_FOFS2                     0x000cc684 /* Frequency Offset for CWC2 */
#define BCHP_SDS_CWC_2_FOFS3                     0x000cc688 /* Frequency Offset for CWC3 */
#define BCHP_SDS_CWC_2_FOFS4                     0x000cc68c /* Frequency Offset for CWC4 */
#define BCHP_SDS_CWC_2_FOFS5                     0x000cc690 /* Frequency Offset for CWC5 */
#define BCHP_SDS_CWC_2_FOFS6                     0x000cc694 /* Frequency Offset for CWC6 */

#endif /* #ifndef BCHP_SDS_CWC_2_H__ */

/* End of File */
