/***************************************************************************
 *     Copyright (c) 1999-2012, Broadcom Corporation
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
 * Date:           Generated on         Tue Jan 17 13:26:47 2012
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

#ifndef BCHP_XPT_PB2_H__
#define BCHP_XPT_PB2_H__

/***************************************************************************
 *XPT_PB2 - Playback 2 Control Registers
 ***************************************************************************/
#define BCHP_XPT_PB2_CTRL1                       0x00908200 /* Playback Control 1 Register */
#define BCHP_XPT_PB2_CTRL2                       0x00908204 /* Playback Control 2 Register */
#define BCHP_XPT_PB2_CTRL3                       0x00908208 /* Playback Control 3 Register */
#define BCHP_XPT_PB2_CTRL4                       0x0090820c /* Playback Control 4 Register */
#define BCHP_XPT_PB2_FIRST_DESC_ADDR             0x00908210 /* Playback First Descriptor Address Register */
#define BCHP_XPT_PB2_CURR_DESC_ADDR              0x00908214 /* Playback Current Descriptor Address Register */
#define BCHP_XPT_PB2_CURR_BUFF_ADDR              0x00908218 /* Playback Current Buffer Address Register */
#define BCHP_XPT_PB2_BLOCKOUT                    0x0090821c /* Data Transport Playback Block Out Control */
#define BCHP_XPT_PB2_PKTZ_CONTEXT0               0x00908220 /* Data Transport Playback Packetize Mode Context 0 Control */
#define BCHP_XPT_PB2_TS_ERR_BOUND_EARLY          0x00908224 /* Data Transport Playback Timestamp Error Bound Register */
#define BCHP_XPT_PB2_TS_ERR_BOUND_LATE           0x00908228 /* Data Transport Playback Timestamp Error Bound Register */
#define BCHP_XPT_PB2_PARSER_CTRL1                0x0090822c /* Data Transport Playback Parser Control Register */
#define BCHP_XPT_PB2_PARSER_CTRL2                0x00908230 /* Data Transport Playback Parser Control Register 2 */
#define BCHP_XPT_PB2_PARSER_TIMESTAMP            0x00908234 /* Data Transport Playback Parser Local Timestamp */
#define BCHP_XPT_PB2_INTR                        0x00908238 /* Playback Processing Error and Status Interrupt Register */
#define BCHP_XPT_PB2_INTR_EN                     0x0090823c /* Playback Processing Error and Status Interrupt Enable Register */
#define BCHP_XPT_PB2_INTR_TAGS                   0x00908240 /* Playback Interrupt Tag Register */
#define BCHP_XPT_PB2_PACING_PCR_PID              0x00908244 /* Playback PCR Based Pacing PCR Pid */
#define BCHP_XPT_PB2_PWR_CTRL                    0x00908248 /* Playback Power Control Register */
#define BCHP_XPT_PB2_MISC_STATUS_REG             0x0090824c /* Miscellaneous Playback Status Registers */
#define BCHP_XPT_PB2_ASF_CTRL                    0x00908250 /* ASF Control Register */
#define BCHP_XPT_PB2_ASF_DATA_PACKET_LENGTH      0x00908254 /* ASF_DATA_PACKET_LENGTH */
#define BCHP_XPT_PB2_ASF_SUB_PAYLOAD_PKT_TYPE    0x00908258 /* ASF_SUB_PAYLOAD_PKT_TYPE */
#define BCHP_XPT_PB2_ASF_PAYLOAD_PKT_TYPE        0x0090825c /* ASF_PAYLOAD_PKT_TYPE */
#define BCHP_XPT_PB2_PLAYBACK_PARSER_BAND_ID     0x00908260 /* PLAYBACK_PARSER_BAND_ID */
#define BCHP_XPT_PB2_NEXT_PACKET_PACING_TIMESTAMP 0x00908264 /* Next Packet Pacing Timestamp Value */
#define BCHP_XPT_PB2_PKT2PKT_PACING_TIMESTAMP_DELTA 0x00908268 /* PKT2PKT Pacing TimeStamp Delta */
#define BCHP_XPT_PB2_PACING_COUNTER              0x0090826c /* Pacing Counter Instantaneous value */
#define BCHP_XPT_PB2_ERROR_VALUE_WATERMARK       0x00908270 /* WaterMark level of the Pacing Error Bound Reached */
#define BCHP_XPT_PB2_PES_BASED_PACING_CTRL       0x00908274 /* PES Based Pacing Controls */

#endif /* #ifndef BCHP_XPT_PB2_H__ */

/* End of File */
