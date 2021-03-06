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
 * Date:           Generated on         Mon Sep 19 21:39:42 2011
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

#ifndef BCHP_GIO_AON_H__
#define BCHP_GIO_AON_H__

/***************************************************************************
 *GIO_AON - GPIO
 ***************************************************************************/
#define BCHP_GIO_AON_ODEN_LO                     0x00408c00 /* GENERAL PURPOSE I/O OPEN DRAIN ENABLE [20:0] */
#define BCHP_GIO_AON_DATA_LO                     0x00408c04 /* GENERAL PURPOSE I/O DATA [20:0] */
#define BCHP_GIO_AON_IODIR_LO                    0x00408c08 /* GENERAL PURPOSE I/O DIRECTION [20:0] */
#define BCHP_GIO_AON_EC_LO                       0x00408c0c /* GENERAL PURPOSE I/O EDGE CONFIGURATION [20:0] */
#define BCHP_GIO_AON_EI_LO                       0x00408c10 /* GENERAL PURPOSE I/O EDGE INSENSITIVE [20:0] */
#define BCHP_GIO_AON_MASK_LO                     0x00408c14 /* GENERAL PURPOSE I/O INTERRUPT MASK [20:0] */
#define BCHP_GIO_AON_LEVEL_LO                    0x00408c18 /* GENERAL PURPOSE I/O INTERRUPT TYPE [20:0] */
#define BCHP_GIO_AON_STAT_LO                     0x00408c1c /* GENERAL PURPOSE I/O INTERRUPT STATUS [20:0] */
#define BCHP_GIO_AON_ODEN_EXT                    0x00408c40 /* GENERAL PURPOSE I/O OPEN DRAIN ENABLE [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_DATA_EXT                    0x00408c44 /* GENERAL PURPOSE I/O DATA [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_IODIR_EXT                   0x00408c48 /* GENERAL PURPOSE I/O DIRECTION [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_EC_EXT                      0x00408c4c /* GENERAL PURPOSE I/O EDGE CONFIGURATION [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_EI_EXT                      0x00408c50 /* GENERAL PURPOSE I/O EDGE INSENSITIVE [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_MASK_EXT                    0x00408c54 /* GENERAL PURPOSE I/O INTERRUPT MASK [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_LEVEL_EXT                   0x00408c58 /* GENERAL PURPOSE I/O INTERRUPT TYPE [65:64] (SGPIO[1:0]) */
#define BCHP_GIO_AON_STAT_EXT                    0x00408c5c /* GENERAL PURPOSE I/O INTERRUPT STATUS [65:64] (SGPIO[1:0]) */

/***************************************************************************
 *ODEN_LO - GENERAL PURPOSE I/O OPEN DRAIN ENABLE [20:0]
 ***************************************************************************/
/* GIO_AON :: ODEN_LO :: spare [31:21] */
#define BCHP_GIO_AON_ODEN_LO_spare_MASK                            0xffe00000
#define BCHP_GIO_AON_ODEN_LO_spare_SHIFT                           21
#define BCHP_GIO_AON_ODEN_LO_spare_DEFAULT                         0

/* GIO_AON :: ODEN_LO :: oden [20:00] */
#define BCHP_GIO_AON_ODEN_LO_oden_MASK                             0x001fffff
#define BCHP_GIO_AON_ODEN_LO_oden_SHIFT                            0
#define BCHP_GIO_AON_ODEN_LO_oden_DEFAULT                          0

/***************************************************************************
 *DATA_LO - GENERAL PURPOSE I/O DATA [20:0]
 ***************************************************************************/
/* GIO_AON :: DATA_LO :: spare [31:21] */
#define BCHP_GIO_AON_DATA_LO_spare_MASK                            0xffe00000
#define BCHP_GIO_AON_DATA_LO_spare_SHIFT                           21
#define BCHP_GIO_AON_DATA_LO_spare_DEFAULT                         0

/* GIO_AON :: DATA_LO :: data [20:00] */
#define BCHP_GIO_AON_DATA_LO_data_MASK                             0x001fffff
#define BCHP_GIO_AON_DATA_LO_data_SHIFT                            0
#define BCHP_GIO_AON_DATA_LO_data_DEFAULT                          0

/***************************************************************************
 *IODIR_LO - GENERAL PURPOSE I/O DIRECTION [20:0]
 ***************************************************************************/
/* GIO_AON :: IODIR_LO :: spare [31:21] */
#define BCHP_GIO_AON_IODIR_LO_spare_MASK                           0xffe00000
#define BCHP_GIO_AON_IODIR_LO_spare_SHIFT                          21
#define BCHP_GIO_AON_IODIR_LO_spare_DEFAULT                        31

/* GIO_AON :: IODIR_LO :: iodir [20:00] */
#define BCHP_GIO_AON_IODIR_LO_iodir_MASK                           0x001fffff
#define BCHP_GIO_AON_IODIR_LO_iodir_SHIFT                          0
#define BCHP_GIO_AON_IODIR_LO_iodir_DEFAULT                        2097151

/***************************************************************************
 *EC_LO - GENERAL PURPOSE I/O EDGE CONFIGURATION [20:0]
 ***************************************************************************/
/* GIO_AON :: EC_LO :: spare [31:21] */
#define BCHP_GIO_AON_EC_LO_spare_MASK                              0xffe00000
#define BCHP_GIO_AON_EC_LO_spare_SHIFT                             21
#define BCHP_GIO_AON_EC_LO_spare_DEFAULT                           0

/* GIO_AON :: EC_LO :: edge_config [20:00] */
#define BCHP_GIO_AON_EC_LO_edge_config_MASK                        0x001fffff
#define BCHP_GIO_AON_EC_LO_edge_config_SHIFT                       0
#define BCHP_GIO_AON_EC_LO_edge_config_DEFAULT                     0

/***************************************************************************
 *EI_LO - GENERAL PURPOSE I/O EDGE INSENSITIVE [20:0]
 ***************************************************************************/
/* GIO_AON :: EI_LO :: spare [31:21] */
#define BCHP_GIO_AON_EI_LO_spare_MASK                              0xffe00000
#define BCHP_GIO_AON_EI_LO_spare_SHIFT                             21
#define BCHP_GIO_AON_EI_LO_spare_DEFAULT                           0

/* GIO_AON :: EI_LO :: edge_insensitive [20:00] */
#define BCHP_GIO_AON_EI_LO_edge_insensitive_MASK                   0x001fffff
#define BCHP_GIO_AON_EI_LO_edge_insensitive_SHIFT                  0
#define BCHP_GIO_AON_EI_LO_edge_insensitive_DEFAULT                0

/***************************************************************************
 *MASK_LO - GENERAL PURPOSE I/O INTERRUPT MASK [20:0]
 ***************************************************************************/
/* GIO_AON :: MASK_LO :: spare [31:21] */
#define BCHP_GIO_AON_MASK_LO_spare_MASK                            0xffe00000
#define BCHP_GIO_AON_MASK_LO_spare_SHIFT                           21
#define BCHP_GIO_AON_MASK_LO_spare_DEFAULT                         0

/* GIO_AON :: MASK_LO :: irq_mask [20:00] */
#define BCHP_GIO_AON_MASK_LO_irq_mask_MASK                         0x001fffff
#define BCHP_GIO_AON_MASK_LO_irq_mask_SHIFT                        0
#define BCHP_GIO_AON_MASK_LO_irq_mask_DEFAULT                      0

/***************************************************************************
 *LEVEL_LO - GENERAL PURPOSE I/O INTERRUPT TYPE [20:0]
 ***************************************************************************/
/* GIO_AON :: LEVEL_LO :: spare [31:21] */
#define BCHP_GIO_AON_LEVEL_LO_spare_MASK                           0xffe00000
#define BCHP_GIO_AON_LEVEL_LO_spare_SHIFT                          21
#define BCHP_GIO_AON_LEVEL_LO_spare_DEFAULT                        0

/* GIO_AON :: LEVEL_LO :: level [20:00] */
#define BCHP_GIO_AON_LEVEL_LO_level_MASK                           0x001fffff
#define BCHP_GIO_AON_LEVEL_LO_level_SHIFT                          0
#define BCHP_GIO_AON_LEVEL_LO_level_DEFAULT                        0

/***************************************************************************
 *STAT_LO - GENERAL PURPOSE I/O INTERRUPT STATUS [20:0]
 ***************************************************************************/
/* GIO_AON :: STAT_LO :: spare [31:21] */
#define BCHP_GIO_AON_STAT_LO_spare_MASK                            0xffe00000
#define BCHP_GIO_AON_STAT_LO_spare_SHIFT                           21
#define BCHP_GIO_AON_STAT_LO_spare_DEFAULT                         0

/* GIO_AON :: STAT_LO :: irq_status [20:00] */
#define BCHP_GIO_AON_STAT_LO_irq_status_MASK                       0x001fffff
#define BCHP_GIO_AON_STAT_LO_irq_status_SHIFT                      0
#define BCHP_GIO_AON_STAT_LO_irq_status_DEFAULT                    0

/***************************************************************************
 *ODEN_EXT - GENERAL PURPOSE I/O OPEN DRAIN ENABLE [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: ODEN_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_ODEN_EXT_reserved0_MASK                       0xfffffffc
#define BCHP_GIO_AON_ODEN_EXT_reserved0_SHIFT                      2

/* GIO_AON :: ODEN_EXT :: oden [01:00] */
#define BCHP_GIO_AON_ODEN_EXT_oden_MASK                            0x00000003
#define BCHP_GIO_AON_ODEN_EXT_oden_SHIFT                           0
#define BCHP_GIO_AON_ODEN_EXT_oden_DEFAULT                         0

/***************************************************************************
 *DATA_EXT - GENERAL PURPOSE I/O DATA [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: DATA_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_DATA_EXT_reserved0_MASK                       0xfffffffc
#define BCHP_GIO_AON_DATA_EXT_reserved0_SHIFT                      2

/* GIO_AON :: DATA_EXT :: data [01:00] */
#define BCHP_GIO_AON_DATA_EXT_data_MASK                            0x00000003
#define BCHP_GIO_AON_DATA_EXT_data_SHIFT                           0
#define BCHP_GIO_AON_DATA_EXT_data_DEFAULT                         0

/***************************************************************************
 *IODIR_EXT - GENERAL PURPOSE I/O DIRECTION [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: IODIR_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_IODIR_EXT_reserved0_MASK                      0xfffffffc
#define BCHP_GIO_AON_IODIR_EXT_reserved0_SHIFT                     2

/* GIO_AON :: IODIR_EXT :: iodir [01:00] */
#define BCHP_GIO_AON_IODIR_EXT_iodir_MASK                          0x00000003
#define BCHP_GIO_AON_IODIR_EXT_iodir_SHIFT                         0
#define BCHP_GIO_AON_IODIR_EXT_iodir_DEFAULT                       3

/***************************************************************************
 *EC_EXT - GENERAL PURPOSE I/O EDGE CONFIGURATION [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: EC_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_EC_EXT_reserved0_MASK                         0xfffffffc
#define BCHP_GIO_AON_EC_EXT_reserved0_SHIFT                        2

/* GIO_AON :: EC_EXT :: edge_config [01:00] */
#define BCHP_GIO_AON_EC_EXT_edge_config_MASK                       0x00000003
#define BCHP_GIO_AON_EC_EXT_edge_config_SHIFT                      0
#define BCHP_GIO_AON_EC_EXT_edge_config_DEFAULT                    0

/***************************************************************************
 *EI_EXT - GENERAL PURPOSE I/O EDGE INSENSITIVE [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: EI_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_EI_EXT_reserved0_MASK                         0xfffffffc
#define BCHP_GIO_AON_EI_EXT_reserved0_SHIFT                        2

/* GIO_AON :: EI_EXT :: edge_insensitive [01:00] */
#define BCHP_GIO_AON_EI_EXT_edge_insensitive_MASK                  0x00000003
#define BCHP_GIO_AON_EI_EXT_edge_insensitive_SHIFT                 0
#define BCHP_GIO_AON_EI_EXT_edge_insensitive_DEFAULT               0

/***************************************************************************
 *MASK_EXT - GENERAL PURPOSE I/O INTERRUPT MASK [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: MASK_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_MASK_EXT_reserved0_MASK                       0xfffffffc
#define BCHP_GIO_AON_MASK_EXT_reserved0_SHIFT                      2

/* GIO_AON :: MASK_EXT :: irq_mask [01:00] */
#define BCHP_GIO_AON_MASK_EXT_irq_mask_MASK                        0x00000003
#define BCHP_GIO_AON_MASK_EXT_irq_mask_SHIFT                       0
#define BCHP_GIO_AON_MASK_EXT_irq_mask_DEFAULT                     0

/***************************************************************************
 *LEVEL_EXT - GENERAL PURPOSE I/O INTERRUPT TYPE [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: LEVEL_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_LEVEL_EXT_reserved0_MASK                      0xfffffffc
#define BCHP_GIO_AON_LEVEL_EXT_reserved0_SHIFT                     2

/* GIO_AON :: LEVEL_EXT :: level [01:00] */
#define BCHP_GIO_AON_LEVEL_EXT_level_MASK                          0x00000003
#define BCHP_GIO_AON_LEVEL_EXT_level_SHIFT                         0
#define BCHP_GIO_AON_LEVEL_EXT_level_DEFAULT                       0

/***************************************************************************
 *STAT_EXT - GENERAL PURPOSE I/O INTERRUPT STATUS [65:64] (SGPIO[1:0])
 ***************************************************************************/
/* GIO_AON :: STAT_EXT :: reserved0 [31:02] */
#define BCHP_GIO_AON_STAT_EXT_reserved0_MASK                       0xfffffffc
#define BCHP_GIO_AON_STAT_EXT_reserved0_SHIFT                      2

/* GIO_AON :: STAT_EXT :: irq_status [01:00] */
#define BCHP_GIO_AON_STAT_EXT_irq_status_MASK                      0x00000003
#define BCHP_GIO_AON_STAT_EXT_irq_status_SHIFT                     0
#define BCHP_GIO_AON_STAT_EXT_irq_status_DEFAULT                   0

#endif /* #ifndef BCHP_GIO_AON_H__ */

/* End of File */
