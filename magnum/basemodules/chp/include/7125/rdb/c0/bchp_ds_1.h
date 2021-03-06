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
 * Date:           Generated on         Fri Jan 22 20:09:40 2010
 *                 MD5 Checksum         a2d1f2163f65e87d228a0fb491cb442d
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

#ifndef BCHP_DS_1_H__
#define BCHP_DS_1_H__

/***************************************************************************
 *DS_1 - Downstream Receiver Registers 1
 ***************************************************************************/
#define BCHP_DS_1_GBL                            0x04c40000 /* Global Core Control Register */
#define BCHP_DS_1_AFECLK                         0x04c40004 /* AFE Clock Phase Alignment Control */
#define BCHP_DS_1_PD                             0x04c40010 /* Global Power Down Register */
#define BCHP_DS_1_IRQSTS1                        0x04c40040 /* Interrupt Status Register 1 */
#define BCHP_DS_1_IRQSET1                        0x04c40044 /* Set Interrupt Status Register 1 */
#define BCHP_DS_1_IRQCLR1                        0x04c40048 /* Clear Interrupt Status Register 1 */
#define BCHP_DS_1_IRQMSK1                        0x04c4004c /* Interrupt Mask Register 1 */
#define BCHP_DS_1_IRQMSET1                       0x04c40050 /* Set Interrupt Mask Register 1 */
#define BCHP_DS_1_IRQMCLR1                       0x04c40054 /* Clear Interrupt Mask Register 1 */
#define BCHP_DS_1_IRQSTS2                        0x04c40058 /* Interrupt Status Register 2 */
#define BCHP_DS_1_IRQSET2                        0x04c4005c /* Set Interrupt Status Register 2 */
#define BCHP_DS_1_IRQCLR2                        0x04c40060 /* Clear Interrupt Status Register 2 */
#define BCHP_DS_1_IRQMSK2                        0x04c40064 /* Interrupt Mask Register 2 */
#define BCHP_DS_1_IRQMSET2                       0x04c40068 /* Set Interrupt Mask Register 2 */
#define BCHP_DS_1_IRQMCLR2                       0x04c4006c /* Clear Interrupt Mask Register 2 */
#define BCHP_DS_1_STAT                           0x04c400fc /* Receiver Status Register */
#define BCHP_DS_1_RST                            0x04c40100 /* Global Reset Register */
#define BCHP_DS_1_FRZ                            0x04c40104 /* Global Freeze Register */
#define BCHP_DS_1_REVID                          0x04c40108 /* Revision ID Register */
#define BCHP_DS_1_STS                            0x04c4010c /* Loop Status Control Register */
#define BCHP_DS_1_BURST_FRZ                      0x04c40110 /* Global Burst noise detector Freeze  Register */
#define BCHP_DS_1_CLK                            0x04c40184 /* Clock Generation Control Register */
#define BCHP_DS_1_NCOU                           0x04c40190 /* NCO Instantaneous Value (Upper) */
#define BCHP_DS_1_NCOL                           0x04c40194 /* NCO Instantaneous Value (Lower) */
#define BCHP_DS_1_FECIN_NCON                     0x04c40198 /* FEC Clock Counter Numerator Value */
#define BCHP_DS_1_FECIN_NCODL                    0x04c4019c /* FEC Clock Counter Delta Value */
#define BCHP_DS_1_FECOUT_NCON                    0x04c401a0 /* OI PLL Clock Rate Numerator */
#define BCHP_DS_1_FECOUT_NCODL                   0x04c401a4 /* OI PLL Clock Rate Delta */
#define BCHP_DS_1_US_FCW_DWELL                   0x04c401a8 /* Upstream Frequency Control Word Dwell-count register */
#define BCHP_DS_1_SGCG                           0x04c401ac /* Clockgen Signature Analyzer */
#define BCHP_DS_1_ICB_CTL                        0x04c40200 /* Internal Configuration Bus Control and Status */
#define BCHP_DS_1_MBOX_CSR_P                     0x04c40204 /* Mail Box Command and Status for processor */
#define BCHP_DS_1_MBOX_DATA_P                    0x04c40208 /* Mail Box Data for processor */
#define BCHP_DS_1_HI_TST                         0x04c40214 /* Test configuration for down steam core's bus interface */
#define BCHP_DS_1_MBOX_CSR_S                     0x04c40218 /* Mail Box Command and Status for serial test interface. */
#define BCHP_DS_1_MBOX_DATA_S                    0x04c4021c /* Mail Box Data for serial test interface */
#define BCHP_DS_1_FE                             0x04c40280 /* Digital Front End Control Register */
#define BCHP_DS_1_SGFE                           0x04c40284 /* Front end Signature Analyzer */
#define BCHP_DS_1_BR                             0x04c40300 /* Baud Receiver Control Register */
#define BCHP_DS_1_BBDCI                          0x04c40304 /* Baseband DC Canceller Integrator Value */
#define BCHP_DS_1_NCH1                           0x04c40308 /* Notch Filter Control Register 1 */
#define BCHP_DS_1_NCH2                           0x04c4030c /* Notch Filter Control Register 2 (Notch 0) */
#define BCHP_DS_1_NCH3                           0x04c40310 /* Notch Filter Control Register 3 (Notch 1) */
#define BCHP_DS_1_NCH4                           0x04c40314 /* Notch Filter Control Register 4 (Notch 2) */
#define BCHP_DS_1_NCH5                           0x04c40318 /* Notch Filter Control Register 5 */
#define BCHP_DS_1_NCH0I                          0x04c4031c /* Notch 0 Integrator Value */
#define BCHP_DS_1_NCH1I                          0x04c40320 /* Notch 1 Integrator Value */
#define BCHP_DS_1_NCH2I                          0x04c40324 /* Notch 2 Integrator Value */
#define BCHP_DS_1_NSWP1                          0x04c40328 /* Notch Filter Sweep Control Register 1 */
#define BCHP_DS_1_NSWP2                          0x04c4032c /* Notch Filter Sweep Control Register 2 */
#define BCHP_DS_1_NSWP3                          0x04c40330 /* Notch Filter Sweep Control Register 3 */
#define BCHP_DS_1_NSWP0I                         0x04c40334 /* Notch Filter Sweep Status Register 0 */
#define BCHP_DS_1_NSWP1I                         0x04c40338 /* Notch Filter Sweep Status Register 1 */
#define BCHP_DS_1_NSWP2I                         0x04c4033c /* Notch Filter Sweep Status Register 2 */
#define BCHP_DS_1_NSWP3I                         0x04c40340 /* Notch Filter Sweep Status Register 3 */
#define BCHP_DS_1_NSWP4I                         0x04c40344 /* Notch Filter Sweep Status Register 4 */
#define BCHP_DS_1_NSWP5I                         0x04c40348 /* Notch Filter Sweep Status Register 5 */
#define BCHP_DS_1_AGCB                           0x04c4034c /* Digital AGCB Control Register */
#define BCHP_DS_1_AGCBI                          0x04c40350 /* Digital AGCB Integrator Value */
#define BCHP_DS_1_AGCBLI                         0x04c40354 /* Digital AGCB Leaky Integrator Value */
#define BCHP_DS_1_SGBR                           0x04c40358 /* Baud Receiver Signature Analyzer */
#define BCHP_DS_1_THLD_CNT                       0x04c4035c /* threshold counter for NTSC power level */
#define BCHP_DS_1_HI_THLD                        0x04c40360 /* hi threshold for NTSC notch power */
#define BCHP_DS_1_LO_THLD                        0x04c40364 /* lo threshold for NTSC notch power */
#define BCHP_DS_1_AGCB_IR                        0x04c40368 /* Digital AGCB Control Register for IMC path */
#define BCHP_DS_1_AGCBI_IR                       0x04c4036c /* Digital AGCB Integrator Value for IMC path */
#define BCHP_DS_1_AGCBLI_IR                      0x04c40370 /* Digital AGCB Leaky Integrator Value for IMC path */
#define BCHP_DS_1_QMLPS                          0x04c40400 /* QAM Loop Control */
#define BCHP_DS_1_CFL                            0x04c40410 /* Carrier Frequency Loop Control Register */
#define BCHP_DS_1_CFLC                           0x04c40414 /* Carrier Frequency Loop Coefficient Control Register */
#define BCHP_DS_1_CFLI                           0x04c40418 /* Carrier Frequency Loop Integrator Value */
#define BCHP_DS_1_CFLSP                          0x04c4041c /* Carrier Frequency Loop Sweep Control Register */
#define BCHP_DS_1_CFLPA                          0x04c40460 /* Carrier Frequency Loop Phase Accumulator Value */
#define BCHP_DS_1_CFLFOS                         0x04c40480 /* Carrier Frequency Loop Frequency Offset Control Register */
#define BCHP_DS_1_CFLFO                          0x04c40488 /* Carrier Frequency Loop Filter Output Value */
#define BCHP_DS_1_TL                             0x04c40494 /* Timing Loop Control Register */
#define BCHP_DS_1_TLC                            0x04c40498 /* Timing Loop Coefficient Control Register */
#define BCHP_DS_1_TLI                            0x04c4049c /* Timing Loop Integrator Value */
#define BCHP_DS_1_TLSWP                          0x04c404a0 /* Timing Loop Sweep Control Value */
#define BCHP_DS_1_TLTHRS                         0x04c404a4 /* Timing Loop Integrator Threshold Register */
#define BCHP_DS_1_TLFOS                          0x04c404a8 /* Timing Loop Phase Offset Control Register */
#define BCHP_DS_1_TLFO                           0x04c404ac /* Timing Loop Filter Output Value */
#define BCHP_DS_1_TLAGC                          0x04c40504 /* Timing Loop AGC Control Register */
#define BCHP_DS_1_TLAGCI                         0x04c40508 /* Timing Loop AGC Integrator Value */
#define BCHP_DS_1_TLAGCL                         0x04c4050c /* Timing Loop AGC Leaky Integrator Value */
#define BCHP_DS_1_PERF                           0x04c40510 /* Performance Monitoring Control/Status Register */
#define BCHP_DS_1_TLDHT                          0x04c40514 /* Timing Lock Detector High Threshold Control Register */
#define BCHP_DS_1_TLDLT                          0x04c40518 /* Timing Lock Detector Low Threshold Control Register */
#define BCHP_DS_1_TLDA                           0x04c4051c /* Timing Lock Detector Accumulator Value */
#define BCHP_DS_1_TLDC                           0x04c40520 /* Timing Lock Detector Maximum Count Control Register */
#define BCHP_DS_1_TLDCI                          0x04c40524 /* Timing Lock Detector Counter Value */
#define BCHP_DS_1_US_IFC                         0x04c40530 /* Upstream/Downstream interface control register */
#define BCHP_DS_1_US_FCW_HI                      0x04c40534 /* Upper-part of the Upstream Frequency Control Word register */
#define BCHP_DS_1_US_FCW_LO                      0x04c40538 /* Lower-part of the Upstream Frequency Control Word register */
#define BCHP_DS_1_US_TL_OFFSET                   0x04c4053c /* Upstream Timing Offset register */
#define BCHP_DS_1_US_DSBCLK                      0x04c40540 /* Upstream baud clock register */
#define BCHP_DS_1_SGCFL                          0x04c40560 /* Carrier Frequency Loop Signature Analyzer */
#define BCHP_DS_1_SGPHS                          0x04c40564 /* Baud phase detector signals Signature Analyzer */
#define BCHP_DS_1_SGMSC                          0x04c40568 /* Miscellaneous signals Signature Analyzer */
#define BCHP_DS_1_FEC                            0x04c40600 /* FEC Control / Status Register */
#define BCHP_DS_1_FEC_TM                         0x04c40604 /* FEC SRAM Test Mode Control */
#define BCHP_DS_1_FECU                           0x04c40610 /* FEC Initialization Register (Upper) */
#define BCHP_DS_1_FECM                           0x04c40614 /* FEC Initialization Register (Middle) */
#define BCHP_DS_1_FECL                           0x04c40618 /* FEC Initialization Register (Lower) */
#define BCHP_DS_1_SGFEC                          0x04c40620 /* FEC Signature Analyzer */
#define BCHP_DS_1_FEC_MAC_OR                     0x04c40624 /* FEC Mac output override */
#define BCHP_DS_1_OI_VCO                         0x04c40640 /* OI VCO Control */
#define BCHP_DS_1_OI_CTL                         0x04c40680 /* OI Control */
#define BCHP_DS_1_OI_OUT                         0x04c40684 /* OI PS Output Control */
#define BCHP_DS_1_OI_ERR                         0x04c4069c /* OI Frame Error Count */
#define BCHP_DS_1_OI_SG                          0x04c406a0 /* Output Interface Signature Analyzer (Test) */
#define BCHP_DS_1_OI_BER_CTL                     0x04c406a4 /* OI BER Estimation Control Register */
#define BCHP_DS_1_OI_BER                         0x04c406a8 /* OI BER Estimation Error Counter Value */
#define BCHP_DS_1_BER                            0x04c40700 /* Pre-FEC BER Estimation Control Register */
#define BCHP_DS_1_BERI                           0x04c40704 /* Pre-FEC BER Estimation Error Counter Value */
#define BCHP_DS_1_CERC1                          0x04c40710 /* FEC RS Corrected Bit Counter */
#define BCHP_DS_1_UERC1                          0x04c40714 /* FEC Uncorrectable RS-Block Counter */
#define BCHP_DS_1_NBERC1                         0x04c40718 /* FEC Clean RS-Block Counter */
#define BCHP_DS_1_CBERC1                         0x04c4071c /* FEC Corrected RS-Block Counter */
#define BCHP_DS_1_BMPG1                          0x04c40720 /* FEC Bad MPEG-Packet Counter */
#define BCHP_DS_1_CERC2                          0x04c40724 /* FEC RS Corrected Bit Counter */
#define BCHP_DS_1_UERC2                          0x04c40728 /* FEC Uncorrectable RS-Block Counter */
#define BCHP_DS_1_NBERC2                         0x04c4072c /* FEC Clean RS-Block Counter */
#define BCHP_DS_1_CBERC2                         0x04c40730 /* FEC Corrected RS-Block Counter */
#define BCHP_DS_1_BMPG2                          0x04c40734 /* FEC Bad MPEG-Packet Counter */
#define BCHP_DS_1_TPFEC                          0x04c40738 /* Testport Control Register for FEC */
#define BCHP_DS_1_EUSEDC1                        0x04c4073c /* FEC Erasure used RS-Block Counter */
#define BCHP_DS_1_EDISCARDC1                     0x04c40740 /* FEC Erasure discarded RS-Block Counter */
#define BCHP_DS_1_EUSEDC2                        0x04c40744 /* FEC Erasure used RS-Block Counter */
#define BCHP_DS_1_EDISCARDC2                     0x04c40748 /* FEC Erasure discarded RS-Block Counter */
#define BCHP_DS_1_TP                             0x04c40780 /* Testport Control Register */
#define BCHP_DS_1_TPOUT                          0x04c40784 /* Testport Output */
#define BCHP_DS_1_FSCNT1                         0x04c40790 /* Sample Rate Counter 1 */
#define BCHP_DS_1_FSCNT2                         0x04c40794 /* Sample Rate Counter 2 */
#define BCHP_DS_1_FBCNT1                         0x04c407c0 /* Baud Rate Counter 1 */
#define BCHP_DS_1_FBCNT2                         0x04c407c4 /* Baud Rate Counter 2 */
#define BCHP_DS_1_SPARE                          0x04c407fc /* Reserved for Future Expansion */
#define BCHP_DS_1_BND                            0x04c40b00 /* BND Control Register */
#define BCHP_DS_1_BND_THR                        0x04c40b04 /* BND threshold value Register */
#define BCHP_DS_1_BND_FRZ                        0x04c40b08 /* BND Freeze Control Register */
#define BCHP_DS_1_BND_THRFRZ                     0x04c40b0c /* BND threshold value Register */

#endif /* #ifndef BCHP_DS_1_H__ */

/* End of File */
