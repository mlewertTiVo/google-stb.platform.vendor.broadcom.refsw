/***************************************************************************
 *     Copyright (c) 2003-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/


/* this file should not be directly included by unprepared code, but properly constructed code included it multiple times  */
/* ************** THIS FILE IS AUTOGENERATED. DO NOT EDIT **************************/
/*****
GENERATED by:
perl magnum/basemodules/chp/src/common/bchp_memc_clients_chip.pl magnum/basemodules/chp/include/7340/common/memc/bchp_memc_clients_chip.txt magnum/basemodules/chp/include/7340/common/memc/bchp_memc_clients_chip.h
*******/
/* # entries copied (as 'select' -> copy -> 'paste into this file') from the http://home.irv.broadcom.com/~rel7125/doc/BCM7125_SCB_Clients.xls */
/* # Various fixes */
/* #7400 clients    "7340 DDR0 */
/* Client #" */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_WR0,0)                        /*  */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_WR1,1)                        /*  */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_WR2,2)                        /*  */
BCHP_P_MEMC_DEFINE_CLIENT(M2MC0_GFX,3)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_DBLK_0,4)                     /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_ILARC_0,5)                    /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_OLARC_0,6)                    /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_CAB_0,7)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_SYM_0,8)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_RD0,9)                        /*  */
/* M2MC1 */
/* AVD_BLK_1 */
/* AVD_ILA_1 */
/* AVD_OLA_1 */
/* AVD_CAB_1 */
/* AVD_SYM_1 */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_RD1,16)                       /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVNM_FGT,17)                      /*  */
/* BVN_I656_ANC1 */
BCHP_P_MEMC_DEFINE_CLIENT(AUD_ZSP,19)                       /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AUD_AIO,20)                       /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_I656_ANC0,21)                 /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AEGIS_BSP,22)                     /*  */
BCHP_P_MEMC_DEFINE_CLIENT(XPT_RD2,23)                       /*  */
/* VEC_VBI_ENC2 */
BCHP_P_MEMC_DEFINE_CLIENT(VEC_VBI_ENC1,25)                  /*  */
BCHP_P_MEMC_DEFINE_CLIENT(VEC_VBI_ENC0,26)                  /*  */
/* BVN_GFD2 */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_GFD1,28)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_GFD0,29)                      /*  */
/* BVN_CAP3 */
/* BVN_CAP2 */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_CAP1,32)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_CAP0,33)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_RDC,34)                       /*  */
/* BVN_VFD3 */
/* BVN_VFD2 */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_VFD1,37)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_VFD0,38)                      /*  */
/* BVN_MFD1 */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_MFD0,40)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(CPU_BRIDGE,41)                    /*  */
BCHP_P_MEMC_DEFINE_CLIENT(PCI,42)                           /*  */
/* M2M_DMA1 */
BCHP_P_MEMC_DEFINE_CLIENT(M2M_DMA0,44)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(GENET_WR,45)                      /*  */
BCHP_P_MEMC_DEFINE_CLIENT(GENET_RD,49)                      /*  */
/* PIX_3D or GPORT    46 (PX3D) */
BCHP_P_MEMC_DEFINE_CLIENT(USB_20,47)                        /*  */
BCHP_P_MEMC_DEFINE_CLIENT(USB_11,48)                        /*  */
BCHP_P_MEMC_DEFINE_CLIENT(CPU_CACHE0,124)                   /*  */
BCHP_P_MEMC_DEFINE_CLIENT(CPU_CACHE1,125)                   /*  */
/* BVN_FGT */
/* BLKAVG_0 */
/* BLKAVG_1 */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_MAD_PIX,54)                   /*  */
BCHP_P_MEMC_DEFINE_CLIENT(BVN_MAD_QUANT,55)                 /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MCIF_RD,56)                       /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MCIF_WR,57)                       /*  */
/* SATA */
BCHP_P_MEMC_DEFINE_CLIENT(MOCA_DATA_WR,59)                  /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MOCA_DATA_RD,60)                  /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MOCA_MIPS,62)                     /*  */
/*  */
/*  */
BCHP_P_MEMC_DEFINE_CLIENT(TPCAP,65)                         /*  */
BCHP_P_MEMC_DEFINE_CLIENT(AVD_PFR_0,120)                    /*  */
/* AVD_PFR_1 */
BCHP_P_MEMC_DEFINE_CLIENT(DRAM_INIT,126)                    /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MEMC64_SOFT,118)                  /*  */
BCHP_P_MEMC_DEFINE_CLIENT(MEMC64_REF,127)                   /*  */
/* MEMC16_SOFT0 */
/* MEMC16_REF0 */
/* MEMC16_SOFT1 */
/* MEMC16_REF1 */
