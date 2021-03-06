/***************************************************************************
 *     Copyright (c) 1999-2010, Broadcom Corporation
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
 * Date:           Generated on         Thu Apr 22 17:17:15 2010
 *                 MD5 Checksum         e645cb91be669b614b9400e5da71f4fa
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

#ifndef BCHP_AUD_DSP_RGR_H__
#define BCHP_AUD_DSP_RGR_H__

/***************************************************************************
 *AUD_DSP_RGR - Audio DSP RGR Bridge Registers
 ***************************************************************************/
#define BCHP_AUD_DSP_RGR_REVISION                0x00380000 /* RGR Bridge Revision */
#define BCHP_AUD_DSP_RGR_CTRL                    0x00380004 /* RGR Bridge Control Register */
#define BCHP_AUD_DSP_RGR_RBUS_TIMER              0x00380008 /* RGR Bridge RBUS Timer Register */

/***************************************************************************
 *REVISION - RGR Bridge Revision
 ***************************************************************************/
/* AUD_DSP_RGR :: REVISION :: reserved0 [31:16] */
#define BCHP_AUD_DSP_RGR_REVISION_reserved0_MASK                   0xffff0000
#define BCHP_AUD_DSP_RGR_REVISION_reserved0_SHIFT                  16

/* AUD_DSP_RGR :: REVISION :: MAJOR [15:08] */
#define BCHP_AUD_DSP_RGR_REVISION_MAJOR_MASK                       0x0000ff00
#define BCHP_AUD_DSP_RGR_REVISION_MAJOR_SHIFT                      8

/* AUD_DSP_RGR :: REVISION :: MINOR [07:00] */
#define BCHP_AUD_DSP_RGR_REVISION_MINOR_MASK                       0x000000ff
#define BCHP_AUD_DSP_RGR_REVISION_MINOR_SHIFT                      0

/***************************************************************************
 *CTRL - RGR Bridge Control Register
 ***************************************************************************/
/* AUD_DSP_RGR :: CTRL :: reserved0 [31:02] */
#define BCHP_AUD_DSP_RGR_CTRL_reserved0_MASK                       0xfffffffc
#define BCHP_AUD_DSP_RGR_CTRL_reserved0_SHIFT                      2

/* AUD_DSP_RGR :: CTRL :: rbus_error_intr [01:01] */
#define BCHP_AUD_DSP_RGR_CTRL_rbus_error_intr_MASK                 0x00000002
#define BCHP_AUD_DSP_RGR_CTRL_rbus_error_intr_SHIFT                1
#define BCHP_AUD_DSP_RGR_CTRL_rbus_error_intr_INTR_DISABLE         0
#define BCHP_AUD_DSP_RGR_CTRL_rbus_error_intr_INTR_ENABLE          1

/* AUD_DSP_RGR :: CTRL :: gisb_error_intr [00:00] */
#define BCHP_AUD_DSP_RGR_CTRL_gisb_error_intr_MASK                 0x00000001
#define BCHP_AUD_DSP_RGR_CTRL_gisb_error_intr_SHIFT                0
#define BCHP_AUD_DSP_RGR_CTRL_gisb_error_intr_INTR_DISABLE         0
#define BCHP_AUD_DSP_RGR_CTRL_gisb_error_intr_INTR_ENABLE          1

/***************************************************************************
 *RBUS_TIMER - RGR Bridge RBUS Timer Register
 ***************************************************************************/
/* AUD_DSP_RGR :: RBUS_TIMER :: reserved0 [31:16] */
#define BCHP_AUD_DSP_RGR_RBUS_TIMER_reserved0_MASK                 0xffff0000
#define BCHP_AUD_DSP_RGR_RBUS_TIMER_reserved0_SHIFT                16

/* AUD_DSP_RGR :: RBUS_TIMER :: timer_value [15:00] */
#define BCHP_AUD_DSP_RGR_RBUS_TIMER_timer_value_MASK               0x0000ffff
#define BCHP_AUD_DSP_RGR_RBUS_TIMER_timer_value_SHIFT              0

#endif /* #ifndef BCHP_AUD_DSP_RGR_H__ */

/* End of File */
