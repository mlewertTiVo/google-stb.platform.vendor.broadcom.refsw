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
 * Date:           Generated on         Wed Apr 13 13:22:20 2011
 *                 MD5 Checksum         c1b53a9408066d1e6123c0860dfb8065
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

#ifndef BCHP_SDS_DSEC_1_H__
#define BCHP_SDS_DSEC_1_H__

/***************************************************************************
 *SDS_DSEC_1 - SDS DiSEqC Register Set 1
 ***************************************************************************/
#define BCHP_SDS_DSEC_1_DSRST                    0x00701800 /* DiSEqC Reset Control */
#define BCHP_SDS_DSEC_1_DSCGDIV                  0x00701804 /* DiSEqC Clock Divider Control */
#define BCHP_SDS_DSEC_1_DSCTL00                  0x00701810 /* DiSEqC Control 00 (Formerly, DSCTL4,DSCTL3,DSCTL2,DSCTL1) */
#define BCHP_SDS_DSEC_1_DSCTL01                  0x00701814 /* DiSEqC Control 01 (Formerly, DSCTL8,DSCTL7,DSCTL6,DSCTL5) */
#define BCHP_SDS_DSEC_1_DSCTL02                  0x00701818 /* DiSEqC Control 02 (Formerly, DSCTL11,DSCTL10,7'b0,DSCTL9[8:0]) */
#define BCHP_SDS_DSEC_1_DSCTL03                  0x0070181c /* DiSEqC Control 03 (Formerly, DSCTL14,DSCTL13,DSCTL12) */
#define BCHP_SDS_DSEC_1_DSCMD                    0x00701820 /* DiSEqC Command Data */
#define BCHP_SDS_DSEC_1_DSRPLY                   0x00701824 /* DiSEqC Receive Data */
#define BCHP_SDS_DSEC_1_DSCMEMADR                0x00701828 /* DiSEqC FIR Filter Coefficient Memory Start Address */
#define BCHP_SDS_DSEC_1_DSCMEMDAT                0x0070182c /* DiSEqC FIR Filter Coefficient Register */
#define BCHP_SDS_DSEC_1_DSFIRCTL                 0x00701830 /* DiSEqC FIR Filter Control Register */
#define BCHP_SDS_DSEC_1_DS_MISC_CONTROL          0x00701834 /* DiSEqC Misc. Control */
#define BCHP_SDS_DSEC_1_DS_PARITY                0x00701838 /* DiSEqC Misc. Control */
#define BCHP_SDS_DSEC_1_ADCTL                    0x00701840 /* RX A/D Control. */
#define BCHP_SDS_DSEC_1_Q15T                     0x00701844 /* Quiet 15ms and auto control word timing */
#define BCHP_SDS_DSEC_1_TPWC                     0x00701848 /* Tone absent timing, TX PWK period and one/zero on counts */
#define BCHP_SDS_DSEC_1_RXBT                     0x0070184c /* RX bit max/min timing */
#define BCHP_SDS_DSEC_1_RXRT                     0x00701850 /* RX max cycle and reply timing */
#define BCHP_SDS_DSEC_1_RBDT                     0x00701854 /* RX low duty min and bit timing */
#define BCHP_SDS_DSEC_1_SLEW                     0x00701858 /* Slew rate control and TOA to TOD timing */
#define BCHP_SDS_DSEC_1_RERT                     0x0070185c /* RX end reply timing and voltage top/bottom levels */
#define BCHP_SDS_DSEC_1_DSCT                     0x00701860 /* misc diseqc controls */
#define BCHP_SDS_DSEC_1_DTCT                     0x00701864 /* voltage count and the TX frequency control word */
#define BCHP_SDS_DSEC_1_DDIO                     0x00701868 /* diseqc general purpose I/O control */
#define BCHP_SDS_DSEC_1_RTDC1                    0x0070186c /* RX clip thresholds and testport control */
#define BCHP_SDS_DSEC_1_RTDC2                    0x00701870 /* RX noise integration control */
#define BCHP_SDS_DSEC_1_TCTL                     0x00701874 /* TOA/TOD control */
#define BCHP_SDS_DSEC_1_CICC                     0x00701878 /* CIC length */
#define BCHP_SDS_DSEC_1_FCIC                     0x0070187c /* CIC1 threshold and majority vote control */
#define BCHP_SDS_DSEC_1_SCIC                     0x00701880 /* CIC2 threshold and majority vote control */
#define BCHP_SDS_DSEC_1_TSTM                     0x00701884 /* TOA/TOD or testport status */
#define BCHP_SDS_DSEC_1_DST1                     0x00701888 /* diseqc status 1 */
#define BCHP_SDS_DSEC_1_DST2                     0x0070188c /* diseqc status 2 */
#define BCHP_SDS_DSEC_1_DS_SAR_THRSH             0x007018a0 /* DiSEqC SAR Status Thresh Control */
#define BCHP_SDS_DSEC_1_DS_SAR_DATA_OUT          0x007018a4 /* DiSEqC SAR Data Output */
#define BCHP_SDS_DSEC_1_DS_SAR_DC_OFFSET         0x007018a8 /* DiSEqC SAR DC Offset Calibration */
#define BCHP_SDS_DSEC_1_DS_SAR_LPF_INT           0x007018ac /* DiSEqC SAR Low-pass Filter Integrator */
#define BCHP_SDS_DSEC_1_DS_SAR_CONTROL           0x007018b0 /* DiSEqC SAR Control */
#define BCHP_SDS_DSEC_1_DS_COMMON_CONTROL        0x007018b4 /* DiSEqC Common Control */
#define BCHP_SDS_DSEC_1_DSTMRCTL                 0x007018c0 /* DiSEqC Timer Control */
#define BCHP_SDS_DSEC_1_DSGENTMR1                0x007018c4 /* DiSEqC General Timer 1 */
#define BCHP_SDS_DSEC_1_DSGENTMR2                0x007018c8 /* DiSEqC General Timer 2 */

#endif /* #ifndef BCHP_SDS_DSEC_1_H__ */

/* End of File */
