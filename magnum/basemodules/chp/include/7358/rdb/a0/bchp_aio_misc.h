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
 * Date:           Generated on         Mon Sep 19 21:37:15 2011
 *                 MD5 Checksum         e020a976c1177b900eff0f3d00f22e40
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

#ifndef BCHP_AIO_MISC_H__
#define BCHP_AIO_MISC_H__

/***************************************************************************
 *AIO_MISC - Audio I/O Miscellaneous Registers
 ***************************************************************************/
#define BCHP_AIO_MISC_REVISION                   0x00880000 /* Audio I/O System Revision */
#define BCHP_AIO_MISC_RESET                      0x00880004 /* AIO Soft Reset */
#define BCHP_AIO_MISC_CTRL                       0x00880008 /* AIO Misc Controls */
#define BCHP_AIO_MISC_PWRDOWN                    0x0088000c /* Audio Powerdown Control */
#define BCHP_AIO_MISC_MAILBOX                    0x00880010 /* Mailbox Test Register */

/***************************************************************************
 *REVISION - Audio I/O System Revision
 ***************************************************************************/
/* AIO_MISC :: REVISION :: reserved0 [31:16] */
#define BCHP_AIO_MISC_REVISION_reserved0_MASK                      0xffff0000
#define BCHP_AIO_MISC_REVISION_reserved0_SHIFT                     16

/* AIO_MISC :: REVISION :: MAJOR [15:08] */
#define BCHP_AIO_MISC_REVISION_MAJOR_MASK                          0x0000ff00
#define BCHP_AIO_MISC_REVISION_MAJOR_SHIFT                         8
#define BCHP_AIO_MISC_REVISION_MAJOR_DEFAULT                       18

/* AIO_MISC :: REVISION :: MINOR [07:00] */
#define BCHP_AIO_MISC_REVISION_MINOR_MASK                          0x000000ff
#define BCHP_AIO_MISC_REVISION_MINOR_SHIFT                         0
#define BCHP_AIO_MISC_REVISION_MINOR_DEFAULT                       0

/***************************************************************************
 *RESET - AIO Soft Reset
 ***************************************************************************/
/* AIO_MISC :: RESET :: reserved0 [31:03] */
#define BCHP_AIO_MISC_RESET_reserved0_MASK                         0xfffffff8
#define BCHP_AIO_MISC_RESET_reserved0_SHIFT                        3

/* AIO_MISC :: RESET :: RESET_FMM [02:02] */
#define BCHP_AIO_MISC_RESET_RESET_FMM_MASK                         0x00000004
#define BCHP_AIO_MISC_RESET_RESET_FMM_SHIFT                        2
#define BCHP_AIO_MISC_RESET_RESET_FMM_DEFAULT                      0
#define BCHP_AIO_MISC_RESET_RESET_FMM_Inactive                     0
#define BCHP_AIO_MISC_RESET_RESET_FMM_Reset                        1

/* AIO_MISC :: RESET :: reserved_for_eco1 [01:00] */
#define BCHP_AIO_MISC_RESET_reserved_for_eco1_MASK                 0x00000003
#define BCHP_AIO_MISC_RESET_reserved_for_eco1_SHIFT                0
#define BCHP_AIO_MISC_RESET_reserved_for_eco1_DEFAULT              0

/***************************************************************************
 *CTRL - AIO Misc Controls
 ***************************************************************************/
/* AIO_MISC :: CTRL :: reserved0 [31:09] */
#define BCHP_AIO_MISC_CTRL_reserved0_MASK                          0xfffffe00
#define BCHP_AIO_MISC_CTRL_reserved0_SHIFT                         9

/* AIO_MISC :: CTRL :: MEMORY_CS [08:08] */
#define BCHP_AIO_MISC_CTRL_MEMORY_CS_MASK                          0x00000100
#define BCHP_AIO_MISC_CTRL_MEMORY_CS_SHIFT                         8
#define BCHP_AIO_MISC_CTRL_MEMORY_CS_DEFAULT                       0
#define BCHP_AIO_MISC_CTRL_MEMORY_CS_ON                            1
#define BCHP_AIO_MISC_CTRL_MEMORY_CS_OFF                           0

/* AIO_MISC :: CTRL :: reserved_for_eco1 [07:00] */
#define BCHP_AIO_MISC_CTRL_reserved_for_eco1_MASK                  0x000000ff
#define BCHP_AIO_MISC_CTRL_reserved_for_eco1_SHIFT                 0
#define BCHP_AIO_MISC_CTRL_reserved_for_eco1_DEFAULT               0

/***************************************************************************
 *PWRDOWN - Audio Powerdown Control
 ***************************************************************************/
/* AIO_MISC :: PWRDOWN :: reserved0 [31:07] */
#define BCHP_AIO_MISC_PWRDOWN_reserved0_MASK                       0xffffff80
#define BCHP_AIO_MISC_PWRDOWN_reserved0_SHIFT                      7

/* AIO_MISC :: PWRDOWN :: DAC0_POWERDN_REF [06:06] */
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_REF_MASK                0x00000040
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_REF_SHIFT               6
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_REF_DEFAULT             0

/* AIO_MISC :: PWRDOWN :: reserved1 [05:02] */
#define BCHP_AIO_MISC_PWRDOWN_reserved1_MASK                       0x0000003c
#define BCHP_AIO_MISC_PWRDOWN_reserved1_SHIFT                      2

/* AIO_MISC :: PWRDOWN :: DAC0_POWERDN_LR [01:00] */
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_LR_MASK                 0x00000003
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_LR_SHIFT                0
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_LR_DEFAULT              0
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_LR_Normal               0
#define BCHP_AIO_MISC_PWRDOWN_DAC0_POWERDN_LR_Powerdown            3

/***************************************************************************
 *MAILBOX - Mailbox Test Register
 ***************************************************************************/
/* AIO_MISC :: MAILBOX :: MAIL [31:00] */
#define BCHP_AIO_MISC_MAILBOX_MAIL_MASK                            0xffffffff
#define BCHP_AIO_MISC_MAILBOX_MAIL_SHIFT                           0
#define BCHP_AIO_MISC_MAILBOX_MAIL_DEFAULT                         0

#endif /* #ifndef BCHP_AIO_MISC_H__ */

/* End of File */
