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
 * Date:           Generated on         Fri Jul  1 18:06:52 2011
 *                 MD5 Checksum         e403e647ebd6e61a7eb0825000ed4941
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

#ifndef BCHP_COMMON_H__
#define BCHP_COMMON_H__

/***************************************************************************
 *BCM3461_B0 - BCM3461 B0 RBUS
 ***************************************************************************/
#define BCHP_PHYSICAL_OFFSET                               0x00000000
#define BCHP_REGISTER_START                                0x00000000 /* LEAP_PROG0_MEM is first */
#define BCHP_REGISTER_END                                  0x000f1400 /* HRC_FIR is last */
#define BCHP_REGISTER_SIZE                                 0x0003c500 /* Number of registers */

/****************************************************************************
 * Core instance register start address.
 ***************************************************************************/
#define BCHP_LEAP_PROG0_MEM_REG_START                      0x00000000
#define BCHP_LEAP_PROG0_MEM_REG_END                        0x0001fffc
#define BCHP_LEAP_PROG1_MEM_REG_START                      0x00020000
#define BCHP_LEAP_PROG1_MEM_REG_END                        0x0002fffc
#define BCHP_LEAP_DATA_MEM_REG_START                       0x00040000
#define BCHP_LEAP_DATA_MEM_REG_END                         0x0005fffc
#define BCHP_LEAP_ROM_REG_START                            0x00060000
#define BCHP_LEAP_ROM_REG_END                              0x00068ffc
#define BCHP_LEAP_HAB_MEM_REG_START                        0x00078000
#define BCHP_LEAP_HAB_MEM_REG_END                          0x000781fc
#define BCHP_LEAP_UART_REG_START                           0x00079000
#define BCHP_LEAP_UART_REG_END                             0x00079ffc
#define BCHP_LEAP_WDG_REG_START                            0x0007a000
#define BCHP_LEAP_WDG_REG_END                              0x0007affc
#define BCHP_LEAP_CTRL_REG_START                           0x00080000
#define BCHP_LEAP_CTRL_REG_END                             0x0008007c
#define BCHP_LEAP_L1_REG_START                             0x00080100
#define BCHP_LEAP_L1_REG_END                               0x00080118
#define BCHP_LEAP_L2_REG_START                             0x00080200
#define BCHP_LEAP_L2_REG_END                               0x00080214
#define BCHP_LEAP_HOST_IRQ_REG_START                       0x00080300
#define BCHP_LEAP_HOST_IRQ_REG_END                         0x0008032c
#define BCHP_LEAP_ROM_PATCH_REG_START                      0x00080400
#define BCHP_LEAP_ROM_PATCH_REG_END                        0x0008043c
#define BCHP_DS_GR_REG_START                               0x00091f00
#define BCHP_DS_GR_REG_END                                 0x00091f0c
#define BCHP_DS_TOPM_REG_START                             0x00092000
#define BCHP_DS_TOPM_REG_END                               0x00092060
#define BCHP_DS_TOPS_REG_START                             0x00093000
#define BCHP_DS_TOPS_REG_END                               0x0009309c
#define BCHP_DS_REG_START                                  0x00094000
#define BCHP_DS_REG_END                                    0x00095090
#define BCHP_TM_REG_START                                  0x000b0000
#define BCHP_TM_REG_END                                    0x000b023c
#define BCHP_PER_IRQ_REG_START                             0x000b0400
#define BCHP_PER_IRQ_REG_END                               0x000b0414
#define BCHP_JTAG_OTP_REG_START                            0x000b0800
#define BCHP_JTAG_OTP_REG_END                              0x000b0838
#define BCHP_BSCA_REG_START                                0x000b1000
#define BCHP_BSCA_REG_END                                  0x000b1054
#define BCHP_TIMER_REG_START                               0x000b1080
#define BCHP_TIMER_REG_END                                 0x000b10bc
#define BCHP_GIO_REG_START                                 0x000b1100
#define BCHP_GIO_REG_END                                   0x000b111c
#define BCHP_DIAG_CAPT_REG_START                           0x000b1400
#define BCHP_DIAG_CAPT_REG_END                             0x000b145c
#define BCHP_AVS_HW_MNTR_REG_START                         0x000b1800
#define BCHP_AVS_HW_MNTR_REG_END                           0x000b1874
#define BCHP_AVS_PVT_MNTR_CONFIG_REG_START                 0x000b1900
#define BCHP_AVS_PVT_MNTR_CONFIG_REG_END                   0x000b191c
#define BCHP_AVS_ASB_REGISTERS_REG_START                   0x000b1a00
#define BCHP_AVS_ASB_REGISTERS_REG_END                     0x000b1a18
#define BCHP_AVS_RO_REGISTERS_0_REG_START                  0x000b1b00
#define BCHP_AVS_RO_REGISTERS_0_REG_END                    0x000b1bdc
#define BCHP_AVS_RO_REGISTERS_1_REG_START                  0x000b1c00
#define BCHP_AVS_RO_REGISTERS_1_REG_END                    0x000b1c64
#define BCHP_AVS_ROSC_THRESHOLD_1_REG_START                0x000b1d00
#define BCHP_AVS_ROSC_THRESHOLD_1_REG_END                  0x000b1de4
#define BCHP_AVS_ROSC_THRESHOLD_2_REG_START                0x000b1e00
#define BCHP_AVS_ROSC_THRESHOLD_2_REG_END                  0x000b1ee4
#define BCHP_T2_BICM_SYS_REG_START                         0x000c0000
#define BCHP_T2_BICM_SYS_REG_END                           0x000c0014
#define BCHP_T2_BICM_INTR2_0_REG_START                     0x000c0200
#define BCHP_T2_BICM_INTR2_0_REG_END                       0x000c022c
#define BCHP_T2_BICM_INTR2_1_REG_START                     0x000c0300
#define BCHP_T2_BICM_INTR2_1_REG_END                       0x000c032c
#define BCHP_T2_BICM_CORE_REG_START                        0x000c0400
#define BCHP_T2_BICM_CORE_REG_END                          0x000c0628
#define BCHP_T2_GLB_REG_START                              0x000d0000
#define BCHP_T2_GLB_REG_END                                0x000d0078
#define BCHP_T2_FE_REG_START                               0x000d0200
#define BCHP_T2_FE_REG_END                                 0x000d0298
#define BCHP_T2_OFDM_REG_START                             0x000d0400
#define BCHP_T2_OFDM_REG_END                               0x000d0670
#define BCHP_T2_OI_REG_START                               0x000d0800
#define BCHP_T2_OI_REG_END                                 0x000d091c
#define BCHP_T2_INTR_REG_START                             0x000d0a00
#define BCHP_T2_INTR_REG_END                               0x000d0a2c
#define BCHP_T2_INTR_OI_REG_START                          0x000d0c00
#define BCHP_T2_INTR_OI_REG_END                            0x000d0c2c
#define BCHP_THD_CORE_REG_START                            0x000e0000
#define BCHP_THD_CORE_REG_END                              0x000e03f8
#define BCHP_THD_INTR2_REG_START                           0x000ea000
#define BCHP_THD_INTR2_REG_END                             0x000ea02c
#define BCHP_THD_INTR_REG_START                            0x000ec000
#define BCHP_THD_INTR_REG_END                              0x000ec014
#define BCHP_THD_INTR2B_REG_START                          0x000ed000
#define BCHP_THD_INTR2B_REG_END                            0x000ed02c
#define BCHP_THD_INTRB_REG_START                           0x000ef000
#define BCHP_THD_INTRB_REG_END                             0x000ef014
#define BCHP_THD_GR_REG_START                              0x000efc00
#define BCHP_THD_GR_REG_END                                0x000efc0c
#define BCHP_UFE_AFE_REG_START                             0x000f0000
#define BCHP_UFE_AFE_REG_END                               0x000f0104
#define BCHP_SDADC_REG_START                               0x000f0200
#define BCHP_SDADC_REG_END                                 0x000f021c
#define BCHP_WBADC_AI_REG_START                            0x000f0400
#define BCHP_WBADC_AI_REG_END                              0x000f047c
#define BCHP_UFE_MISC_REG_START                            0x000f0600
#define BCHP_UFE_MISC_REG_END                              0x000f06a8
#define BCHP_UFE_GR_BRIDGE_REG_START                       0x000f0700
#define BCHP_UFE_GR_BRIDGE_REG_END                         0x000f070c
#define BCHP_UFE_MISC2_REG_START                           0x000f0780
#define BCHP_UFE_MISC2_REG_END                             0x000f0790
#define BCHP_UFE_REG_START                                 0x000f0800
#define BCHP_UFE_REG_END                                   0x000f0868
#define BCHP_UFE_SAW_REG_START                             0x000f0900
#define BCHP_UFE_SAW_REG_END                               0x000f0980
#define BCHP_UFE_HRC0_REG_START                            0x000f0a00
#define BCHP_UFE_HRC0_REG_END                              0x000f0a68
#define BCHP_UFE_SAW_HRC0_REG_START                        0x000f0b00
#define BCHP_UFE_SAW_HRC0_REG_END                          0x000f0b80
#define BCHP_UFE_HRC1_REG_START                            0x000f0c00
#define BCHP_UFE_HRC1_REG_END                              0x000f0c68
#define BCHP_UFE_SAW_HRC1_REG_START                        0x000f0d00
#define BCHP_UFE_SAW_HRC1_REG_END                          0x000f0d80
#define BCHP_HRC_REG_START                                 0x000f1000
#define BCHP_HRC_REG_END                                   0x000f1024
#define BCHP_HRC_FIR_REG_START                             0x000f1200
#define BCHP_HRC_FIR_REG_END                               0x000f13fc


#endif /* #ifndef BCHP_COMMON_H__ */

/* End of File */
