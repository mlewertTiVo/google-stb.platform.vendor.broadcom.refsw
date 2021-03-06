/***************************************************************************
 *     Copyright (c) 1999-2007, Broadcom Corporation
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
 * Date:           Generated on         Mon Jul 23 11:59:09 2007
 *                 MD5 Checksum         61f9c4d8dcdcd06017506dddbf23f434
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008004
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_TTE_656_H__
#define BCHP_TTE_656_H__

/***************************************************************************
 *TTE_656 - Teletext/NABTS Encoder
 ***************************************************************************/
#define BCHP_TTE_656_status                      0x0018a204 /* teletext_status Register */
#define BCHP_TTE_656_control                     0x0018a208 /* teletext_control Register */
#define BCHP_TTE_656_lines_active                0x0018a20c /* teletext_lines_active Register */
#define BCHP_TTE_656_read_address_top            0x0018a210 /* Base Address of top Teletext buffer */
#define BCHP_TTE_656_read_address_bottom         0x0018a214 /* Base Address of bottom Teletext buffer */
#define BCHP_TTE_656_top_mask                    0x0018a220 /* Teletext Top Mask Register */
#define BCHP_TTE_656_bottom_mask                 0x0018a224 /* Teletext Top Mask Register */
#define BCHP_TTE_656_revid                       0x0018a228 /* Teletext Encoder Revision ID Register */

/***************************************************************************
 *status - teletext_status Register
 ***************************************************************************/
/* TTE_656 :: status :: reserved0 [31:02] */
#define BCHP_TTE_656_status_reserved0_MASK                         0xfffffffc
#define BCHP_TTE_656_status_reserved0_SHIFT                        2

/* TTE_656 :: status :: data_sent_bf [01:01] */
#define BCHP_TTE_656_status_data_sent_bf_MASK                      0x00000002
#define BCHP_TTE_656_status_data_sent_bf_SHIFT                     1

/* TTE_656 :: status :: data_sent_tf [00:00] */
#define BCHP_TTE_656_status_data_sent_tf_MASK                      0x00000001
#define BCHP_TTE_656_status_data_sent_tf_SHIFT                     0

/***************************************************************************
 *control - teletext_control Register
 ***************************************************************************/
/* TTE_656 :: control :: reserved0 [31:17] */
#define BCHP_TTE_656_control_reserved0_MASK                        0xfffe0000
#define BCHP_TTE_656_control_reserved0_SHIFT                       17

/* TTE_656 :: control :: serial_port_mode [16:15] */
#define BCHP_TTE_656_control_serial_port_mode_MASK                 0x00018000
#define BCHP_TTE_656_control_serial_port_mode_SHIFT                15
#define BCHP_TTE_656_control_serial_port_mode_DATA_ONLY            0
#define BCHP_TTE_656_control_serial_port_mode_MAGAZINE_DATA        1
#define BCHP_TTE_656_control_serial_port_mode_FRM_MAGAZINE_DATA    2
#define BCHP_TTE_656_control_serial_port_mode_RUNIN_FRM_MAGAZINE_DATA 3

/* TTE_656 :: control :: serial_port [14:14] */
#define BCHP_TTE_656_control_serial_port_MASK                      0x00004000
#define BCHP_TTE_656_control_serial_port_SHIFT                     14
#define BCHP_TTE_656_control_serial_port_ENABLE                    1
#define BCHP_TTE_656_control_serial_port_DISABLE                   0

/* TTE_656 :: control :: constant_phase [13:13] */
#define BCHP_TTE_656_control_constant_phase_MASK                   0x00002000
#define BCHP_TTE_656_control_constant_phase_SHIFT                  13

/* TTE_656 :: control :: reserved1 [12:12] */
#define BCHP_TTE_656_control_reserved1_MASK                        0x00001000
#define BCHP_TTE_656_control_reserved1_SHIFT                       12

/* TTE_656 :: control :: anci656_enable [11:11] */
#define BCHP_TTE_656_control_anci656_enable_MASK                   0x00000800
#define BCHP_TTE_656_control_anci656_enable_SHIFT                  11

/* TTE_656 :: control :: anci656_output_fc [10:10] */
#define BCHP_TTE_656_control_anci656_output_fc_MASK                0x00000400
#define BCHP_TTE_656_control_anci656_output_fc_SHIFT               10

/* TTE_656 :: control :: start_delay [09:04] */
#define BCHP_TTE_656_control_start_delay_MASK                      0x000003f0
#define BCHP_TTE_656_control_start_delay_SHIFT                     4

/* TTE_656 :: control :: teletext_mode [03:03] */
#define BCHP_TTE_656_control_teletext_mode_MASK                    0x00000008
#define BCHP_TTE_656_control_teletext_mode_SHIFT                   3
#define BCHP_TTE_656_control_teletext_mode_ETSTeletext             0
#define BCHP_TTE_656_control_teletext_mode_NABTS                   1

/* TTE_656 :: control :: shift_direction [02:02] */
#define BCHP_TTE_656_control_shift_direction_MASK                  0x00000004
#define BCHP_TTE_656_control_shift_direction_SHIFT                 2
#define BCHP_TTE_656_control_shift_direction_LSBToMSB              0
#define BCHP_TTE_656_control_shift_direction_MSBToLSB              1

/* TTE_656 :: control :: enable_bf [01:01] */
#define BCHP_TTE_656_control_enable_bf_MASK                        0x00000002
#define BCHP_TTE_656_control_enable_bf_SHIFT                       1

/* TTE_656 :: control :: enable_tf [00:00] */
#define BCHP_TTE_656_control_enable_tf_MASK                        0x00000001
#define BCHP_TTE_656_control_enable_tf_SHIFT                       0

/***************************************************************************
 *lines_active - teletext_lines_active Register
 ***************************************************************************/
/* TTE_656 :: lines_active :: reserved0 [31:26] */
#define BCHP_TTE_656_lines_active_reserved0_MASK                   0xfc000000
#define BCHP_TTE_656_lines_active_reserved0_SHIFT                  26

/* TTE_656 :: lines_active :: startline_bf [25:21] */
#define BCHP_TTE_656_lines_active_startline_bf_MASK                0x03e00000
#define BCHP_TTE_656_lines_active_startline_bf_SHIFT               21

/* TTE_656 :: lines_active :: reserved1 [20:10] */
#define BCHP_TTE_656_lines_active_reserved1_MASK                   0x001ffc00
#define BCHP_TTE_656_lines_active_reserved1_SHIFT                  10

/* TTE_656 :: lines_active :: startline_tf [09:05] */
#define BCHP_TTE_656_lines_active_startline_tf_MASK                0x000003e0
#define BCHP_TTE_656_lines_active_startline_tf_SHIFT               5

/* TTE_656 :: lines_active :: reserved2 [04:00] */
#define BCHP_TTE_656_lines_active_reserved2_MASK                   0x0000001f
#define BCHP_TTE_656_lines_active_reserved2_SHIFT                  0

/***************************************************************************
 *read_address_top - Base Address of top Teletext buffer
 ***************************************************************************/
/* TTE_656 :: read_address_top :: SlabAddress [31:00] */
#define BCHP_TTE_656_read_address_top_SlabAddress_MASK             0xffffffff
#define BCHP_TTE_656_read_address_top_SlabAddress_SHIFT            0

/***************************************************************************
 *read_address_bottom - Base Address of bottom Teletext buffer
 ***************************************************************************/
/* TTE_656 :: read_address_bottom :: SlabAddress [31:00] */
#define BCHP_TTE_656_read_address_bottom_SlabAddress_MASK          0xffffffff
#define BCHP_TTE_656_read_address_bottom_SlabAddress_SHIFT         0

/***************************************************************************
 *top_mask - Teletext Top Mask Register
 ***************************************************************************/
/* TTE_656 :: top_mask :: reserved0 [31:18] */
#define BCHP_TTE_656_top_mask_reserved0_MASK                       0xfffc0000
#define BCHP_TTE_656_top_mask_reserved0_SHIFT                      18

/* TTE_656 :: top_mask :: mask [17:00] */
#define BCHP_TTE_656_top_mask_mask_MASK                            0x0003ffff
#define BCHP_TTE_656_top_mask_mask_SHIFT                           0

/***************************************************************************
 *bottom_mask - Teletext Top Mask Register
 ***************************************************************************/
/* TTE_656 :: bottom_mask :: reserved0 [31:18] */
#define BCHP_TTE_656_bottom_mask_reserved0_MASK                    0xfffc0000
#define BCHP_TTE_656_bottom_mask_reserved0_SHIFT                   18

/* TTE_656 :: bottom_mask :: mask [17:00] */
#define BCHP_TTE_656_bottom_mask_mask_MASK                         0x0003ffff
#define BCHP_TTE_656_bottom_mask_mask_SHIFT                        0

/***************************************************************************
 *revid - Teletext Encoder Revision ID Register
 ***************************************************************************/
/* TTE_656 :: revid :: reserved0 [31:16] */
#define BCHP_TTE_656_revid_reserved0_MASK                          0xffff0000
#define BCHP_TTE_656_revid_reserved0_SHIFT                         16

/* TTE_656 :: revid :: REVID [15:00] */
#define BCHP_TTE_656_revid_REVID_MASK                              0x0000ffff
#define BCHP_TTE_656_revid_REVID_SHIFT                             0

#endif /* #ifndef BCHP_TTE_656_H__ */

/* End of File */
