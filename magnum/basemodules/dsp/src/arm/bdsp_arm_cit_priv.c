/******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *****************************************************************************/


#include "bdsp_arm_priv_include.h"

BDBG_MODULE(bdsp_arm_cit_priv);

static void BDSP_Arm_P_PopulateAlgoMode(
                BDSP_ArmStage *pArmPrimaryStage,
                BDSP_CIT_P_sAlgoModePresent *sAlgoModePresent)
{
    BDBG_ASSERT(NULL != pArmPrimaryStage);

    /*Initialize first*/
    sAlgoModePresent->ui32DolbyPulsePresent   = BDSP_CIT_P_ABSENT;
    sAlgoModePresent->ui32DDP_PassThruPresent = BDSP_CIT_P_ABSENT;
    sAlgoModePresent->ui32DTS_EncoderPresent  = BDSP_CIT_P_ABSENT;
    sAlgoModePresent->ui32AC3_EncoderPresent  = BDSP_CIT_P_ABSENT;
    sAlgoModePresent->ui32DdrePresent         = BDSP_CIT_P_ABSENT;

    BDSP_ARM_STAGE_TRAVERSE_LOOP_BEGIN(pArmPrimaryStage, pArmConnectStage)
    BSTD_UNUSED(macroBrId);
    BSTD_UNUSED(macroStId);
    {
        /* Handle special case algorithms */
        switch ( pArmConnectStage->algorithm )
        {
            case BDSP_Algorithm_eDolbyPulseAdtsDecode:
            case BDSP_Algorithm_eDolbyPulseLoasDecode:
                sAlgoModePresent->ui32DolbyPulsePresent = BDSP_CIT_P_PRESENT;
                break;
            case BDSP_Algorithm_eAc3PlusPassthrough:
                sAlgoModePresent->ui32DDP_PassThruPresent = BDSP_CIT_P_PRESENT;
                break;
            case BDSP_Algorithm_eDtsCoreEncode:
                sAlgoModePresent->ui32DTS_EncoderPresent = BDSP_CIT_P_PRESENT;
                break;
            case BDSP_Algorithm_eAc3Encode:
                sAlgoModePresent->ui32AC3_EncoderPresent = BDSP_CIT_P_PRESENT;
                break;
            case BDSP_Algorithm_eDdre:
                sAlgoModePresent->ui32DdrePresent = BDSP_CIT_P_PRESENT;
                break;
            default:
                break;
        }
    }
    BDSP_ARM_STAGE_TRAVERSE_LOOP_END(pArmConnectStage)
}

static BERR_Code BDSP_Arm_P_PopulateFwHwBuffer(
                                void *pPrimaryStageHandle,
                                BDSP_AF_P_sFW_HW_CFG        *psFwHwCfg
                            )
{
    BERR_Code errCode = BERR_SUCCESS;
    unsigned output;

    unsigned ui32Count = 0, ui32PPMCount = 0, ui32BufferId = 0;

    BDSP_ArmStage *pArmPrimaryStage = (BDSP_ArmStage *)pPrimaryStageHandle;

    BDBG_ASSERT(NULL != pArmPrimaryStage);

    /*Initialization*/
    for(ui32Count =0; ui32Count<BDSP_AF_P_MAX_ADAPTIVE_RATE_BLOCKS;ui32Count++)
    {
        psFwHwCfg->sPpmCfg[ui32Count].ePPMChannel       = BDSP_AF_P_eDisable;
        psFwHwCfg->sPpmCfg[ui32Count].ui32PPMCfgAddr    = (dramaddr_t)((uintptr_t)NULL);
    }

    BDSP_ARM_STAGE_TRAVERSE_LOOP_BEGIN(pArmPrimaryStage, pArmConnectStage)
    BSTD_UNUSED(macroBrId);
    BSTD_UNUSED(macroStId);
    {
        for(output=0;output<BDSP_AF_P_MAX_OP_FORKS;output++)
        {
            if(pArmConnectStage->sStageOutput[output].eConnectionType == BDSP_ConnectionType_eFmmBuffer &&
                pArmConnectStage->sStageOutput[output].eNodeValid==BDSP_AF_P_eValid)
            {
                for(ui32Count =0; ui32Count<BDSP_AF_P_MAX_CHANNEL_PAIR;ui32Count++)
                {
                    ui32BufferId = pArmConnectStage->sStageOutput[output].Metadata.rateController[ui32Count].wrcnt;
                    /*ui32BufferId would be -1 by default by init if FMM dest not added*/
                    if(ui32BufferId != BDSP_CIT_P_PI_INVALID)
                    {
                        if(ui32PPMCount >= BDSP_AF_P_MAX_ADAPTIVE_RATE_BLOCKS)
                        {
                            errCode = BERR_LEAKED_RESOURCE;
                            goto error;
                        }

                        psFwHwCfg->sPpmCfg[ui32PPMCount].ePPMChannel = BDSP_AF_P_eEnable;
                        psFwHwCfg->sPpmCfg[ui32PPMCount].ui32PPMCfgAddr = ui32BufferId; /*Writing the PPM config address without the Offset convertion as done in case of Raaga*/

                        ui32PPMCount = ui32PPMCount + 1;
                    }
                }
            }
        }
    }
    BDSP_ARM_STAGE_TRAVERSE_LOOP_END(pArmConnectStage)

error:
    return errCode;

}

static BERR_Code BDSP_Arm_P_FillNodeCfg(
                    BDSP_Stage *pPrimaryStageHandle,
                    uint32_t dspIndex,
                    BDSP_ARM_AF_P_sNODE_CONFIG  *psNodeCfg,
                    unsigned *ui32TotalNodes)
{

    BERR_Code   errCode;
    uint32_t    ui32Node;
    uint32_t    ui32NumNodesInAlgo;
    uint32_t    ui32NodeIndex, ui32Ip, ui32Op;
    unsigned    ui32IOPhysAddr, ui32IOGenPhysAddr, i;
    bool        collectResidue;
    BDSP_AF_P_sIO_GENERIC_BUFFER *pTempIoGenBuffer_Cached = NULL;
    BDSP_AF_P_sIO_BUFFER         *pTempIoBuffer_Cached = NULL;

    uint32_t    j;
    uint32_t    ui32FmmPortDstCount[BDSP_AF_P_DistinctOpType_eMax] = {0};
	BDSP_MMA_Memory Memory;

    const BDSP_Arm_P_AlgorithmInfo *sAlgoInfo;

    BDSP_ArmStage *pArmPrimaryStage;
    BDSP_ArmContext *pArmContext;

    BDSP_Arm *pDevice;

    BDSP_AF_P_ValidInvalid          eNodeValid = BDSP_AF_P_eInvalid;

    BDBG_ENTER(BDSP_Arm_P_FillNodeCfg);

    errCode = BERR_SUCCESS;

    BSTD_UNUSED(dspIndex);

    pArmPrimaryStage = (BDSP_ArmStage *)pPrimaryStageHandle;
    BDBG_ASSERT(NULL != pArmPrimaryStage->pContext);
    pArmContext = (BDSP_ArmContext *)pArmPrimaryStage->pContext;
    BDBG_ASSERT(NULL != pArmContext->pDevice);
    pDevice = (BDSP_Arm *)pArmContext->pDevice;

    ui32NodeIndex=0;

    collectResidue = true;

    /*Now, traverse through all the stages. Hold back the stage forking for later traversing.
    The assumption is that the stage o/p connection handle will be NULL if it has no o/p interstage connection */
    BDSP_ARM_STAGE_TRAVERSE_LOOP_BEGIN(pArmPrimaryStage, pArmConnectStage)
    BSTD_UNUSED(macroBrId);
    BSTD_UNUSED(macroStId);
    {
        sAlgoInfo = BDSP_Arm_P_LookupAlgorithmInfo(pArmConnectStage->algorithm);

        ui32NumNodesInAlgo = sAlgoInfo->algoExecInfo.NumNodes;

        for( ui32Node=0; ui32Node<ui32NumNodesInAlgo; ui32Node++ )
        {
            /*Intialization for Master Slave Configuration Counter */
            for(j = 0; j< BDSP_AF_P_DistinctOpType_eMax; j++)
            {
                ui32FmmPortDstCount[j] = 0;
            }

            if(sAlgoInfo->algoExecInfo.eAlgoIds[ui32Node] != BDSP_ARM_AF_P_AlgoId_eInvalid )
            {
                psNodeCfg->uiNodeId = ui32NodeIndex;/*increment to next node*/
                /*Populating the Collect Residual Flag */
                /*Branch Id is populated during the stage traverse to Download FW execs. Use it now.*/
                psNodeCfg->eCollectResidual = (collectResidue) ? BDSP_AF_P_eEnable : BDSP_AF_P_eDisable;
                BDBG_MSG(("Collect Residue [%s] = %d", (BDSP_Arm_P_LookupAlgorithmInfo(pArmConnectStage->algorithm))->pName, collectResidue));

                psNodeCfg->eAlgoId = sAlgoInfo->algoExecInfo.eAlgoIds[ui32Node];

                /* Audio Algorithm Type */
                psNodeCfg->ui32AudioAlgorithmType = sAlgoInfo->algorithm;

                BDBG_MSG(("ui32NodeIndex=%d", ui32NodeIndex));
                BDBG_MSG(("sAlgoInfo->algoExecInfo.eAlgoIds[ui32Node]=%d, ui32Node=%d", sAlgoInfo->algoExecInfo.eAlgoIds[ui32Node], ui32Node));

				/* Copy file name */
				BKNI_Memcpy((void *)(&psNodeCfg->ui32AlgoAddrFile[0]),(void *)(BDSP_ARM_AlgoFileName[psNodeCfg->eAlgoId]),sizeof(psNodeCfg->ui32AlgoAddrFile));

                /*  Code Buffer */
                psNodeCfg->sDramAlgoCodeBuffer.ui32DramBufferAddress =
                                                pDevice->imgCache[BDSP_ARM_IMG_ID_CODE(psNodeCfg->eAlgoId)].offset;
                psNodeCfg->sDramAlgoCodeBuffer.ui32BufferSizeInBytes =
                                                pDevice->imgCache[BDSP_ARM_IMG_ID_CODE(psNodeCfg->eAlgoId)].size;


                if(ui32Node==0)
                {
                    /*  Inter-Frame buffer */
                    psNodeCfg->sDramInterFrameBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramInterFrameBuffer.Buffer.offset +
                                                    pArmConnectStage->sFrameSyncOffset.ui32IfOffset;
                }else if(ui32Node==1)
                {
                    /*  Inter-Frame buffer */
                    psNodeCfg->sDramInterFrameBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramInterFrameBuffer.Buffer.offset;
                }
                else
                {
                    BDBG_ERR(("Number of nodes more than 2 in the branch %d and stage %d ", pArmConnectStage->ui32BranchId, pArmConnectStage->ui32StageId));
                    BDBG_ASSERT(0);
                }

                psNodeCfg->sDramInterFrameBuffer.ui32BufferSizeInBytes =
                                                BDSP_ARM_sNodeInfo[psNodeCfg->eAlgoId].ui32InterFrmBuffSize;

                /*  ROM Table buffer */
                psNodeCfg->sDramLookupTablesBuffer.ui32DramBufferAddress =
                                                pDevice->imgCache[BDSP_ARM_IMG_ID_TABLE(psNodeCfg->eAlgoId)].offset;
                psNodeCfg->sDramLookupTablesBuffer.ui32BufferSizeInBytes =
                                                pDevice->imgCache[BDSP_ARM_IMG_ID_TABLE(psNodeCfg->eAlgoId)].size;
                if(psNodeCfg->sDramLookupTablesBuffer.ui32BufferSizeInBytes)
                {
					Memory.pAddr = pDevice->imgCache[BDSP_ARM_IMG_ID_TABLE(psNodeCfg->eAlgoId)].pMemory;
					Memory.offset=	pDevice->imgCache[BDSP_ARM_IMG_ID_TABLE(psNodeCfg->eAlgoId)].offset;
                    BDSP_Arm_P_InsertEntry_MapTable(&(pArmConnectStage->sStageMapTable[0]),
                       &Memory,
                       psNodeCfg->sDramLookupTablesBuffer.ui32BufferSizeInBytes,
                       (BDSP_ARM_AF_P_Map_eDram),
                       BDSP_ARM_MAX_ALLOC_STAGE);
                }
                if(ui32Node == 0)
                {
                    /*  User Config buffer*/
                    psNodeCfg->sDramUserConfigBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramUserConfigBuffer.Buffer.offset
                                                    + pArmConnectStage->sFrameSyncOffset.ui32UserCfgOffset;
                    /*  Status Buffer*/
                    psNodeCfg->sDramStatusBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramStatusBuffer.Buffer.offset
                                                    + pArmConnectStage->sFrameSyncOffset.ui32StatusOffset;

                }else if(ui32Node == 1)
                {
                    /*  User Config buffer */
                    psNodeCfg->sDramUserConfigBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramUserConfigBuffer.Buffer.offset;
                    /*  Status Buffer*/
                    psNodeCfg->sDramStatusBuffer.ui32DramBufferAddress =
                                                    pArmConnectStage->sDramStatusBuffer.Buffer.offset;;
                }
                else
                {
                    BDBG_ERR(("Number of nodes more than 2 in the branch %d and stage %d which cannot happen ", pArmConnectStage->ui32BranchId, pArmConnectStage->ui32StageId));
                    BDBG_ASSERT(0);
                }

                psNodeCfg->sDramUserConfigBuffer.ui32BufferSizeInBytes =
                                                BDSP_ARM_sNodeInfo[psNodeCfg->eAlgoId].ui32UserCfgBuffSize;
                psNodeCfg->sDramStatusBuffer.ui32BufferSizeInBytes =
                                                BDSP_ARM_sNodeInfo[psNodeCfg->eAlgoId].ui32StatusBuffSize;

                /*  Num Src and destination for the node */
                psNodeCfg->ui32NumSrc = pArmConnectStage->totalInputs;/*inputs of all type*/
                psNodeCfg->ui32NumDst = pArmConnectStage->totalOutputs; /*Use the modified Dst o/ps*/

                /*The logic is in filling the node configuration for a stage which has more than one node, say decoder is:
                Both use the same IO Buffer and IO Gen Buffer as input and output respectively */

                pTempIoBuffer_Cached = NULL;
                pTempIoGenBuffer_Cached = NULL;
                /*  Input Configuration */
                for( ui32Ip=0; ui32Ip<BDSP_AF_P_MAX_IP_FORKS; ui32Ip++ )
                {
                    /*BDBG_ERR(("%s:%d - psNodeCfg->eNodeIpValidFlag[ui32Ip] %d", pRaagaConnectStage->sStageInput[ui32Ip].eNodeValid));*/
                    eNodeValid = pArmConnectStage->sStageInput[ui32Ip].eNodeValid;
                    psNodeCfg->eNodeIpValidFlag[ui32Ip] = eNodeValid;

                    if(eNodeValid)
                    {
                        BDBG_ASSERT(pArmConnectStage->sStageInput[ui32Ip].IoBuffDesc.pAddr);
                        pTempIoBuffer_Cached =(BDSP_AF_P_sIO_BUFFER *)pArmConnectStage->sStageInput[ui32Ip].IoBuffDesc.pAddr;
                        ui32IOPhysAddr = pArmConnectStage->sStageInput[ui32Ip].StageIOBuffDescAddr;

                        BDBG_ASSERT(pArmConnectStage->sStageInput[ui32Ip].IoGenBuffDesc.pAddr);
                        pTempIoGenBuffer_Cached =(BDSP_AF_P_sIO_GENERIC_BUFFER *)pArmConnectStage->sStageInput[ui32Ip].IoGenBuffDesc.pAddr;
                        ui32IOGenPhysAddr = pArmConnectStage->sStageInput[ui32Ip].StageIOGenericBuffDescAddr;

                        switch (pArmConnectStage->sStageInput[ui32Ip].eConnectionType)
                        {
                            case BDSP_ConnectionType_eFmmBuffer:
                            case BDSP_ConnectionType_eRaveBuffer:
                            case BDSP_ConnectionType_eRDBBuffer:
                                pTempIoBuffer_Cached->eBufferType = pArmConnectStage->sStageInput[ui32Ip].IoBuffer.eBufferType;
                                pTempIoBuffer_Cached->ui32NumBuffers= pArmConnectStage->sStageInput[ui32Ip].IoBuffer.ui32NumBuffers;

                                for(i=0;i<pTempIoBuffer_Cached->ui32NumBuffers;i++)/*audio channels*/
                                {   /*ensure that the descriptors for FMM and RAVE that are passed are physical address*/
                                    pTempIoBuffer_Cached->sCircBuffer[i].ui32BaseAddr =pArmConnectStage->sStageInput[ui32Ip].IoBuffer.sCircBuffer[i].ui32BaseAddr;
                                    pTempIoBuffer_Cached->sCircBuffer[i].ui32EndAddr  =pArmConnectStage->sStageInput[ui32Ip].IoBuffer.sCircBuffer[i].ui32EndAddr;
                                    pTempIoBuffer_Cached->sCircBuffer[i].ui32ReadAddr =pArmConnectStage->sStageInput[ui32Ip].IoBuffer.sCircBuffer[i].ui32ReadAddr;
                                    pTempIoBuffer_Cached->sCircBuffer[i].ui32WrapAddr =pArmConnectStage->sStageInput[ui32Ip].IoBuffer.sCircBuffer[i].ui32WrapAddr;
                                    pTempIoBuffer_Cached->sCircBuffer[i].ui32WriteAddr=pArmConnectStage->sStageInput[ui32Ip].IoBuffer.sCircBuffer[i].ui32WriteAddr;
                                }
                                BDSP_MMA_P_FlushCache(pArmConnectStage->sStageInput[ui32Ip].IoBuffDesc, sizeof(BDSP_AF_P_sIO_BUFFER));
                                BDSP_MMA_P_FlushCache(pArmConnectStage->sStageInput[ui32Ip].IoGenBuffDesc, sizeof(BDSP_AF_P_sIO_GENERIC_BUFFER));

                                ui32IOGenPhysAddr = pArmConnectStage->sStageInput[ui32Ip].IoGenBuffDesc.offset;
                                BDBG_MSG(("FMM,RAVE,RDB i/p connection,ui32Ip=%d",ui32Ip));
                                break;
                            case BDSP_ConnectionType_eStage:
								for (i = 0; i < pTempIoBuffer_Cached->ui32NumBuffers; i++)
								{
									pTempIoBuffer_Cached->sCircBuffer[i].ui32BaseAddr
										= pDevice->memInfo.sScratchandISBuff.InterStageIOBuff[pArmConnectStage->ui32BranchId].IOBuff.sCircBuffer[i].ui32BaseAddr;
									pTempIoBuffer_Cached->sCircBuffer[i].ui32EndAddr
										= pDevice->memInfo.sScratchandISBuff.InterStageIOBuff[pArmConnectStage->ui32BranchId].IOBuff.sCircBuffer[i].ui32EndAddr;
									pTempIoBuffer_Cached->sCircBuffer[i].ui32ReadAddr
										= pDevice->memInfo.sScratchandISBuff.InterStageIOBuff[pArmConnectStage->ui32BranchId].IOBuff.sCircBuffer[i].ui32ReadAddr;
									pTempIoBuffer_Cached->sCircBuffer[i].ui32WriteAddr
										= pDevice->memInfo.sScratchandISBuff.InterStageIOBuff[pArmConnectStage->ui32BranchId].IOBuff.sCircBuffer[i].ui32WriteAddr;
									pTempIoBuffer_Cached->sCircBuffer[i].ui32WrapAddr
										= pDevice->memInfo.sScratchandISBuff.InterStageIOBuff[pArmConnectStage->ui32BranchId].IOBuff.sCircBuffer[i].ui32WrapAddr;
								}
								pTempIoGenBuffer_Cached->sCircBuffer.ui32BaseAddr
									= pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[pArmConnectStage->ui32BranchId].IOGenericBuff.sCircBuffer.ui32BaseAddr;
								pTempIoGenBuffer_Cached->sCircBuffer.ui32EndAddr
									= pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[pArmConnectStage->ui32BranchId].IOGenericBuff.sCircBuffer.ui32EndAddr;
								pTempIoGenBuffer_Cached->sCircBuffer.ui32ReadAddr
									= pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[pArmConnectStage->ui32BranchId].IOGenericBuff.sCircBuffer.ui32ReadAddr;
								pTempIoGenBuffer_Cached->sCircBuffer.ui32WriteAddr
									= pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[pArmConnectStage->ui32BranchId].IOGenericBuff.sCircBuffer.ui32WriteAddr;
								pTempIoGenBuffer_Cached->sCircBuffer.ui32WrapAddr
									= pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[pArmConnectStage->ui32BranchId].IOGenericBuff.sCircBuffer.ui32WrapAddr;
                                BDSP_MMA_P_FlushCache(pArmConnectStage->sStageInput[ui32Ip].IoBuffDesc, sizeof(BDSP_AF_P_sIO_BUFFER));
                                BDSP_MMA_P_FlushCache(pArmConnectStage->sStageInput[ui32Ip].IoGenBuffDesc, sizeof(BDSP_AF_P_sIO_GENERIC_BUFFER));
								BDBG_MSG(("Stage Ip connection and Branch id of interstage=%d", pArmConnectStage->ui32BranchId));
                                break;
                            case BDSP_ConnectionType_eInterTaskBuffer:
                                /* Do nothing for inter task connection as the descriptors are populated
                                at create inter task buffer and inter task buffer flush */
                                break;
                            default:

                                BDBG_ERR(("ERROR: Invalid Connection type %d in BDSP_Arm_P_FillNodeCfg",pArmConnectStage->sStageInput[ui32Ip].eConnectionType));
                                break;
                        }

                        BDBG_MSG(("ui32Ip= %d of a stage,pArmConnectStage->algorithm=%d,ui32IOPhysAddr =%x,ui32IOGenPhysAddr=%x ",ui32Ip, pArmConnectStage->algorithm, ui32IOPhysAddr, ui32IOGenPhysAddr));

                        psNodeCfg->ui32NodeIpBuffCfgAddr[ui32Ip] = ui32IOPhysAddr;
                        psNodeCfg->ui32NodeIpGenericDataBuffCfgAddr[ui32Ip] = ui32IOGenPhysAddr;
                        /*psNodeCfg->ui32NodeIpBuffCfgAddr[ui32Ip] = (unsigned) pTempIoBuffer;*//*pRaagaConnectStage->sStageInput[ui32Ip].ui32StageIOBuffCfgAddr;*/

                    }
                }

                pTempIoBuffer_Cached = NULL;
                pTempIoGenBuffer_Cached = NULL;
                /* Output Configuration */
                /*Only for a decoder, this ui32Node is valid. For other stages, lets say SRC, node 0 is invalid and wont even enter this 'if'.*/
                if ((ui32Node == 0) && (ui32NumNodesInAlgo > 1))
                {
                    /* The output buffer descriptors for IDS (node 0 of a decode / mixer stage)
                    will always be the inter-stage buffer of branch 0 */
                    BDBG_MSG(("IDS of pArmConnectStage->algorithm=%d",pArmConnectStage->algorithm));
                    pTempIoBuffer_Cached =(BDSP_AF_P_sIO_BUFFER *)pArmConnectStage->sIdsStageOutput.IoBuffDesc.pAddr;
                    ui32IOPhysAddr = pArmConnectStage->sIdsStageOutput.StageIOBuffDescAddr;

                    pTempIoGenBuffer_Cached =(BDSP_AF_P_sIO_GENERIC_BUFFER *)pArmConnectStage->sIdsStageOutput.IoGenBuffDesc.pAddr;
                    ui32IOGenPhysAddr = pArmConnectStage->sIdsStageOutput.StageIOGenericBuffDescAddr;
                    /* Output IO buffer descriptor population */
                    pTempIoBuffer_Cached->eBufferType = BDSP_AF_P_BufferType_eDRAM_IS;
                    pTempIoBuffer_Cached->ui32NumBuffers = 0;
                    pArmConnectStage->sIdsStageOutput.eNodeValid = BDSP_AF_P_eValid;
                    pArmConnectStage->sIdsStageOutput.eConnectionType = BDSP_ConnectionType_eStage;

                    /* Output IO Generic buffer descriptor population */
                    pTempIoGenBuffer_Cached->eBufferType = BDSP_AF_P_BufferType_eDRAM_IS;
                    pTempIoGenBuffer_Cached->ui32NumBuffers = 1;
                    pTempIoGenBuffer_Cached->sCircBuffer.ui32BaseAddr=pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[0].IOGenericBuff.sCircBuffer.ui32BaseAddr;
                    pTempIoGenBuffer_Cached->sCircBuffer.ui32EndAddr=pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[0].IOGenericBuff.sCircBuffer.ui32EndAddr;
                    pTempIoGenBuffer_Cached->sCircBuffer.ui32ReadAddr=pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[0].IOGenericBuff.sCircBuffer.ui32ReadAddr;
                    pTempIoGenBuffer_Cached->sCircBuffer.ui32WrapAddr=pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[0].IOGenericBuff.sCircBuffer.ui32WrapAddr;
                    pTempIoGenBuffer_Cached->sCircBuffer.ui32WriteAddr=pDevice->memInfo.sScratchandISBuff.InterStageIOGenericBuff[0].IOGenericBuff.sCircBuffer.ui32WriteAddr;

                    BDSP_MMA_P_FlushCache(pArmConnectStage->sIdsStageOutput.IoBuffDesc, sizeof(BDSP_AF_P_sIO_BUFFER));
                    BDSP_MMA_P_FlushCache(pArmConnectStage->sIdsStageOutput.IoGenBuffDesc, sizeof(BDSP_AF_P_sIO_GENERIC_BUFFER));
                    psNodeCfg->ui32NodeOpBuffCfgAddr[0] = ui32IOPhysAddr;
                    psNodeCfg->ui32NodeOpGenericDataBuffCfgAddr[0] = ui32IOGenPhysAddr;
                    psNodeCfg->eNodeOpBuffDataType[0] = BDSP_AF_P_DistinctOpType_eGenericIsData;

                    psNodeCfg->ui32NumDst = 1; /* Number of outputs from IDS node is always one */
                }
                else
                {

                    for( ui32Op=0; ui32Op<BDSP_AF_P_MAX_OP_FORKS; ui32Op++ )
                    {
                        eNodeValid = pArmConnectStage->sStageOutput[ui32Op].eNodeValid;
                        if(eNodeValid)
                        {
                            pTempIoBuffer_Cached =(BDSP_AF_P_sIO_BUFFER *)pArmConnectStage->sStageOutput[ui32Op].IoBuffDesc.pAddr;
                            ui32IOPhysAddr = pArmConnectStage->sStageOutput[ui32Op].StageIOBuffDescAddr;

                            pTempIoGenBuffer_Cached =(BDSP_AF_P_sIO_GENERIC_BUFFER *)pArmConnectStage->sStageOutput[ui32Op].IoGenBuffDesc.pAddr;
                            ui32IOGenPhysAddr = pArmConnectStage->sStageOutput[ui32Op].StageIOGenericBuffDescAddr;
                            switch (pArmConnectStage->sStageOutput[ui32Op].eConnectionType)
                            {
                                case BDSP_ConnectionType_eFmmBuffer:
                                case BDSP_ConnectionType_eRaveBuffer:
                                case BDSP_ConnectionType_eRDBBuffer:
                                    /*no IO Generic o/p to Rave(same for MDAL also - check ?)*/

                                    BKNI_Memset(pTempIoGenBuffer_Cached, 0, sizeof(BDSP_AF_P_sIO_GENERIC_BUFFER));

                                    if( BDSP_ConnectionType_eFmmBuffer == pArmConnectStage->sStageOutput[ui32Op].eConnectionType)
                                    {
                                        /*Detecting the Master /Slave port */
                                        /*
                                        The Algorithm is :
                                            The PI will not provide any information telling a buffer is Master or slave..

                                                Following are the Assumptions..
                                                    All slaves of a master port comes in a Distinct output as different output ports..
                                                    The first FMM output port of a distinct port is considered as Master and all other FMM ports are
                                                    considered as slave to the Master..

                                            CIT-gen counts the FMM ports in a Distinct output. If the number of FMM ports in a distinct output is >1,
                                            then the other FMM slave ports are present.

                                            If an FMM port is identified as Slave, them the Buffer type should be ....
                                        */
                                        if(0 != ui32FmmPortDstCount[pArmConnectStage->eStageOpBuffDataType[ui32Op]])
                                        {
                                            pTempIoBuffer_Cached->eBufferType = BDSP_AF_P_BufferType_eFMMSlave;
                                        }
                                        else
                                        {
                                            pTempIoBuffer_Cached->eBufferType = BDSP_AF_P_BufferType_eFMM;
                                        }
                                        ui32FmmPortDstCount[pArmConnectStage->eStageOpBuffDataType[ui32Op]]++;
                                    }
                                    else
                                    {
                                        pTempIoBuffer_Cached->eBufferType = pArmConnectStage->sStageOutput[ui32Op].IoBuffer.eBufferType;
                                    }

                                    pTempIoBuffer_Cached->ui32NumBuffers= pArmConnectStage->sStageOutput[ui32Op].IoBuffer.ui32NumBuffers;

                                    for(i=0;i<pTempIoBuffer_Cached->ui32NumBuffers;i++)/*audio channels*/
                                    {   /*ensure that the descriptors for FMM and RAVE that are passed are physical address*/
                                        uint32_t size;
                                        pTempIoBuffer_Cached->sCircBuffer[i].ui32BaseAddr =(pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32BaseAddr|BCHP_PHYSICAL_OFFSET);
                                        pTempIoBuffer_Cached->sCircBuffer[i].ui32EndAddr  =(pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32EndAddr|BCHP_PHYSICAL_OFFSET);
                                        pTempIoBuffer_Cached->sCircBuffer[i].ui32ReadAddr =(pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32ReadAddr|BCHP_PHYSICAL_OFFSET);
                                        pTempIoBuffer_Cached->sCircBuffer[i].ui32WrapAddr =(pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32WrapAddr|BCHP_PHYSICAL_OFFSET);
                                        pTempIoBuffer_Cached->sCircBuffer[i].ui32WriteAddr=(pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32WriteAddr|BCHP_PHYSICAL_OFFSET);

										/* We are trying to MAP the registers here, hence we pass the same in both physical and virtual Address */
										Memory.offset = pTempIoBuffer_Cached->sCircBuffer[i].ui32ReadAddr;
										Memory.pAddr  = (void *)((uintptr_t)pTempIoBuffer_Cached->sCircBuffer[i].ui32ReadAddr);
										errCode = BDSP_Arm_P_InsertEntry_MapTable(
											&(pArmConnectStage->sStageMapTable[0]),
											&Memory,
											(6*sizeof(uint32_t)),
											(BDSP_ARM_AF_P_Map_eDevice),
											BDSP_ARM_MAX_ALLOC_STAGE);
                                        if (BERR_SUCCESS != errCode)
                                        {
                                            BDBG_ERR(("BDSP_Arm_P_FillNodeCfg: Error in updating the MAP Table for FMM registers"));
                                        }
										/* The Memory we are trying to MAP is actual Buffer which is allocated by APE/AIO for the FMM buffer. The address is physical and stored in the FMM register.
										Since Insert enrty table function expects both physical and virtual. The processing inside the function is based on virtual and at the end we pass the physical
										address to Firmware. We read the value and pass it as both physical and virtual address, so that Insert Entry function processes it correctly. */
										Memory.pAddr =  (void *)((uintptr_t)BDSP_Read32(pDevice->regHandle ,pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32BaseAddr));
										Memory.offset=  BDSP_Read32(pDevice->regHandle ,pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32BaseAddr);
										size = (uint32_t)(((uint32_t)BDSP_Read32(pDevice->regHandle ,pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32EndAddr) -
												(uint32_t)BDSP_Read32(pDevice->regHandle ,pArmConnectStage->sStageOutput[ui32Op].IoBuffer.sCircBuffer[i].ui32BaseAddr)) + 1);
										errCode = BDSP_Arm_P_InsertEntry_MapTable(
											&(pArmConnectStage->sStageMapTable[0]),
											&Memory,
											size,
											(BDSP_ARM_AF_P_Map_eDram),
											BDSP_ARM_MAX_ALLOC_STAGE);

                                        if (BERR_SUCCESS != errCode)
                                        {
                                            BDBG_ERR(("BDSP_Arm_P_FillNodeCfg: Error in updating the MAP Table for Actual FMM buffer"));
                                        }
                                    }
                                    BDBG_MSG(("FMM RAVE RDB output connection"));
                                    break;

                                case BDSP_ConnectionType_eInterTaskBuffer: /* Do nothing as descriptor is populated at inter task buffer create */
								case BDSP_ConnectionType_eStage:
                                    break;
                                default:

                                    BDBG_ERR(("ERROR: Invalid Connection type %d in BDSP_Arm_P_FillNodeCfg",pArmConnectStage->sStageOutput[ui32Op].eConnectionType));
                                    break;
                            }

                            BDSP_MMA_P_FlushCache(pArmConnectStage->sStageOutput[ui32Op].IoBuffDesc, sizeof(BDSP_AF_P_sIO_BUFFER));
                            BDSP_MMA_P_FlushCache(pArmConnectStage->sStageOutput[ui32Op].IoGenBuffDesc, sizeof(BDSP_AF_P_sIO_GENERIC_BUFFER));
                            BDBG_MSG(("ui32Op= %d of a stage,pArmConnectStage->algorithm=%d, ui32IOPhysAddr=%x, ui32IOGenPhysAddr=%x",ui32Op, pArmConnectStage->algorithm, ui32IOPhysAddr, ui32IOGenPhysAddr));

                            psNodeCfg->ui32NodeOpBuffCfgAddr[ui32Op] = ui32IOPhysAddr;
                            psNodeCfg->ui32NodeOpGenericDataBuffCfgAddr[ui32Op] = ui32IOGenPhysAddr;
                            psNodeCfg->eNodeOpBuffDataType[ui32Op] = pArmConnectStage->eStageOpBuffDataType[ui32Op];
                        }
                    }
                }

                psNodeCfg++;/*next node*/
                ui32NodeIndex++;
             }/*if node of a stage is valid*/
         }/*for( ui32Node=0; ui32Node<ui32NumNodesInAlgo; ui32Node++ )*/

        /*Node config fill for a stage ends here. Traverse to the next stage*/
        for (i = 0; i < BDSP_AF_P_MAX_OP_FORKS; i++)
        {
            if ((pArmConnectStage->sStageOutput[i].eConnectionType == BDSP_ConnectionType_eFmmBuffer)
                 && (pArmConnectStage->sStageOutput[i].eNodeValid == BDSP_AF_P_eValid))
            {
                collectResidue = false;
            }
        }
    }
    BDSP_ARM_STAGE_TRAVERSE_LOOP_END(pArmConnectStage)

     *ui32TotalNodes = ui32NodeIndex;

    BDBG_LEAVE(BDSP_Arm_P_FillNodeCfg);

    return errCode;
}

/*  This function fills the global task configuration */
static BERR_Code BDSP_Arm_P_FillGblTaskCfg (
                        BDSP_ArmTask *pArmTask,
                        BDSP_ARM_AF_P_sTASK_CONFIG  *psCit,
                        unsigned ui32TotalNodes)

{
	BERR_Code ui32Error = BERR_SUCCESS;
    int32_t  taskindex, index, index2;
    BDSP_ArmStage *pPrimaryStage;
    BDSP_ArmContext *pArmContext;
    BDSP_Arm *pDevice;
    BDSP_AF_P_sGLOBAL_TASK_CONFIG *psGblTaskCfg;
    BDSP_TaskStartSettings *pStartSettings;
    BDSP_AF_P_sIO_BUFFER   *pIOBuffer;

    BDSP_MMA_Memory TaskPortConfigAddr, TaskGateOpenConfigAddr, TaskFwHwCfgAddr;
    BDSP_MMA_Memory FwOpSamplingFreqMapLutAddr, StcTriggerCfgAddr;
	BDSP_AF_P_sFMM_DEST_CFG  *psFmmDestCfg;
	BDSP_AF_P_TASK_sFMM_GATE_OPEN_CONFIG	*psTaskFmmGateOpenConfig;
    BDSP_AF_P_sFMM_GATE_OPEN_CONFIG *psFmmGateOpenConfig;
    BDSP_TaskGateOpenSettings   sDependentTaskGateOpenSettings;
    BDSP_AF_P_sFW_HW_CFG        sFwHwCfg;
    BDSP_AF_P_sStcTrigConfig    sStcTrigConfig;
    BDSP_CIT_P_sAlgoModePresent sAlgoModePresent;
    BDSP_AF_P_DolbyMsUsageMode  eDolbyMsUsageMode;
    unsigned ui32ZeroFillSamples;

    BDBG_ENTER(BDSP_Arm_P_FillGblTaskCfg);
    BDBG_OBJECT_ASSERT(pArmTask, BDSP_ArmTask);
    pPrimaryStage = (BDSP_ArmStage *)pArmTask->startSettings.primaryStage->pStageHandle;
    pArmContext = (BDSP_ArmContext *)pPrimaryStage->pContext;
    pDevice = (BDSP_Arm *)pArmContext->pDevice;
    psGblTaskCfg = &psCit->sGlobalTaskConfig;
    pStartSettings = &pArmTask->startSettings;

    /*  Fill in the scratch buffer details */
    psGblTaskCfg->sDramScratchBuffer.ui32DramBufferAddress = pDevice->memInfo.sScratchandISBuff.DspScratchMemGrant.Buffer.offset;
    psGblTaskCfg->sDramScratchBuffer.ui32BufferSizeInBytes = pDevice->memInfo.sScratchandISBuff.DspScratchMemGrant.ui32Size;

    /*  Start node index */
    psGblTaskCfg->ui32StartNodeIndexOfCoreAudioAlgorithm = BDSP_CIT_P_NUM_SPECIAL_NODES;

    TaskPortConfigAddr         = pArmTask->taskMemGrants.sTaskCfgBufInfo.Buffer;

    TaskGateOpenConfigAddr     = TaskPortConfigAddr;
    TaskGateOpenConfigAddr.pAddr  =(void *)((uint8_t *)TaskGateOpenConfigAddr.pAddr + BDSP_CIT_P_TASK_PORT_CONFIG_MEM_SIZE);
    TaskGateOpenConfigAddr.offset = TaskGateOpenConfigAddr.offset + BDSP_CIT_P_TASK_PORT_CONFIG_MEM_SIZE;

    TaskFwHwCfgAddr = TaskGateOpenConfigAddr;
    TaskFwHwCfgAddr.pAddr = (void *)((uint8_t *)TaskGateOpenConfigAddr.pAddr + BDSP_CIT_P_TASK_FMM_GATE_OPEN_CONFIG);
    TaskFwHwCfgAddr.offset = TaskFwHwCfgAddr.offset + BDSP_CIT_P_TASK_FMM_GATE_OPEN_CONFIG;

    FwOpSamplingFreqMapLutAddr = TaskFwHwCfgAddr;
    FwOpSamplingFreqMapLutAddr.pAddr = (void *)((uint8_t *)FwOpSamplingFreqMapLutAddr.pAddr + BDSP_CIT_P_TASK_HW_FW_CONFIG);
    FwOpSamplingFreqMapLutAddr.offset = FwOpSamplingFreqMapLutAddr.offset + BDSP_CIT_P_TASK_HW_FW_CONFIG;

    StcTriggerCfgAddr = FwOpSamplingFreqMapLutAddr;
    StcTriggerCfgAddr.pAddr = (void *)((uint8_t *)StcTriggerCfgAddr.pAddr + BDSP_CIT_P_TASK_FS_MAPPING_LUT_SIZE);
    StcTriggerCfgAddr.offset= StcTriggerCfgAddr.offset+ BDSP_CIT_P_TASK_FS_MAPPING_LUT_SIZE;

    /*BDSP_CIT_P_TASK_STC_TRIG_CONFIG_SIZE;*/
	psTaskFmmGateOpenConfig = (BDSP_AF_P_TASK_sFMM_GATE_OPEN_CONFIG *)BKNI_Malloc(SIZEOF(BDSP_AF_P_TASK_sFMM_GATE_OPEN_CONFIG));
	if(NULL == psTaskFmmGateOpenConfig)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Couldn't Allocate Memory for Gate Open Config"));
		return BERR_OUT_OF_DEVICE_MEMORY;
	}

	psFmmDestCfg = (BDSP_AF_P_sFMM_DEST_CFG *)BKNI_Malloc(SIZEOF(BDSP_AF_P_sFMM_DEST_CFG)*BDSP_AF_P_MAX_NUM_PLLS);
	if(NULL == psFmmDestCfg)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Couldn't Allocate Memory for FMM DEST CONFIG"));
        ui32Error = BERR_TRACE(BERR_OUT_OF_DEVICE_MEMORY);
		goto err_fmmdesc_alloc;
	}

	BDSP_P_InitializeFmmDstCfg(psFmmDestCfg);
	ui32Error = BDSP_MMA_P_CopyDataToDram(&TaskPortConfigAddr, (void *)psFmmDestCfg, SIZEOF(BDSP_AF_P_sFMM_DEST_CFG));
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in copying the Initialised FMM settings for FMM Port"));
		goto end;
	}
    /* Add port Config and SPDIF Config */
    psGblTaskCfg->ui32FmmDestCfgAddr = TaskPortConfigAddr.offset;
	BKNI_Memset(psTaskFmmGateOpenConfig,0,sizeof(BDSP_AF_P_TASK_sFMM_GATE_OPEN_CONFIG));
    if(true == pStartSettings->gateOpenReqd)
    {
        pPrimaryStage->pArmTask = pArmTask;
        ui32Error = BDSP_Arm_P_PopulateGateOpenFMMStages(
                                        (void *)pPrimaryStage,
										psTaskFmmGateOpenConfig,
                                        pStartSettings->maxIndependentDelay
                                    );
		if(ui32Error != BERR_SUCCESS)
		{
			BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in Populating the GateOpen Settings"));
			goto end;
		}

        if(pStartSettings->DependentTaskInfo.numTasks >= BDSP_MAX_DEPENDENT_TASK)
        {
            BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Total number of Dependent task to open their respective gates is %d exceeding limit %d !!!!!!!!!!",pStartSettings->DependentTaskInfo.numTasks, BDSP_MAX_DEPENDENT_TASK));
            ui32Error = BERR_INVALID_PARAMETER;
			goto end;
        }

        for(taskindex=0; taskindex<(int32_t)pStartSettings->DependentTaskInfo.numTasks; taskindex++)
        {
            BKNI_Memset(&sDependentTaskGateOpenSettings,0,sizeof(BDSP_TaskGateOpenSettings));
            sDependentTaskGateOpenSettings.psFmmGateOpenConfig = BKNI_Malloc(BDSP_AF_P_MAX_FMM_OP_PORTS_IN_TASK* sizeof(BDSP_AF_P_sFMM_GATE_OPEN_CONFIG));
            if(NULL == sDependentTaskGateOpenSettings.psFmmGateOpenConfig)
            {
                BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Couldn't allocated memory for retreiving the FMM config of dependent task"));
				ui32Error = BERR_TRACE(BERR_OUT_OF_DEVICE_MEMORY);
				goto end;
            }
            ui32Error = BDSP_Task_RetrieveGateOpenSettings( pStartSettings->DependentTaskInfo.DependentTask[taskindex],
                                                &sDependentTaskGateOpenSettings);
			if(ui32Error != BERR_SUCCESS)
			{
				BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in Retreiving the Gate Open Settings for Dependenttask %d",taskindex));
				goto end;
			}
            if(sDependentTaskGateOpenSettings.ui32MaxIndepDelay != psTaskFmmGateOpenConfig->ui32MaxIndepDelay)
            {
                BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg:Different Max Independent Delay provided: For Dependent task is (%d) and For Gate Open Incharge Task is (%d)",
                    sDependentTaskGateOpenSettings.ui32MaxIndepDelay,
                    psTaskFmmGateOpenConfig->ui32MaxIndepDelay));
				ui32Error = BERR_INVALID_PARAMETER;
				goto end;
            }

            /* Modify the Addresses Returned to ARM based Address*/
            psFmmGateOpenConfig = (BDSP_AF_P_sFMM_GATE_OPEN_CONFIG *)(sDependentTaskGateOpenSettings.psFmmGateOpenConfig);
            for(index = 0; index < (int32_t)sDependentTaskGateOpenSettings.ui32NumPorts; index++)
            {
                for(index2 = 0; index2 < (int32_t)BDSP_AF_P_MAX_CHANNELS; index2++)
                {
                    psFmmGateOpenConfig->uin32RingBufStartWrPointAddr[index2]=(psFmmGateOpenConfig->uin32RingBufStartWrPointAddr[index2]+BCHP_PHYSICAL_OFFSET);
                }

				pIOBuffer =(BDSP_AF_P_sIO_BUFFER *)((BDSP_AF_P_sIO_BUFFER *)sDependentTaskGateOpenSettings.PortAddr.pAddr + index);
				for(index2=0;index2< (int32_t)pIOBuffer->ui32NumBuffers;index2++)/*audio channels*/
				{	/*ensure that the descriptors for FMM and RAVE that are passed are physical address*/
				   pIOBuffer->sCircBuffer[index2].ui32BaseAddr = (pIOBuffer->sCircBuffer[index2].ui32BaseAddr+BCHP_PHYSICAL_OFFSET);
				   pIOBuffer->sCircBuffer[index2].ui32EndAddr  = (pIOBuffer->sCircBuffer[index2].ui32EndAddr+BCHP_PHYSICAL_OFFSET);
				   pIOBuffer->sCircBuffer[index2].ui32ReadAddr = (pIOBuffer->sCircBuffer[index2].ui32ReadAddr+BCHP_PHYSICAL_OFFSET);
				   pIOBuffer->sCircBuffer[index2].ui32WrapAddr = (pIOBuffer->sCircBuffer[index2].ui32WrapAddr+BCHP_PHYSICAL_OFFSET);
				   pIOBuffer->sCircBuffer[index2].ui32WriteAddr= (pIOBuffer->sCircBuffer[index2].ui32WriteAddr+BCHP_PHYSICAL_OFFSET);
				}
                psFmmGateOpenConfig++;
            }
			BDSP_MMA_P_FlushCache(sDependentTaskGateOpenSettings.PortAddr,(sDependentTaskGateOpenSettings.ui32NumPorts*sizeof(BDSP_AF_P_sIO_BUFFER)));

            if((psTaskFmmGateOpenConfig->ui32NumPorts + sDependentTaskGateOpenSettings.ui32NumPorts)> BDSP_AF_P_MAX_FMM_OP_PORTS_IN_TASK)
            {
                BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Total number of FMM ports (%d) in the ecosystem exceeding the limit %d !!!!!!!!!!",
                    (psTaskFmmGateOpenConfig->ui32NumPorts + sDependentTaskGateOpenSettings.ui32NumPorts),
                    BDSP_AF_P_MAX_FMM_OP_PORTS_IN_TASK));
                BKNI_Free(sDependentTaskGateOpenSettings.psFmmGateOpenConfig);
				ui32Error = BERR_INVALID_PARAMETER;
				goto end;
            }
            else
            {
                BKNI_Memcpy((void *)&(psTaskFmmGateOpenConfig->sFmmGateOpenConfig[psTaskFmmGateOpenConfig->ui32NumPorts]),
                            sDependentTaskGateOpenSettings.psFmmGateOpenConfig,
                            (sDependentTaskGateOpenSettings.ui32NumPorts * sizeof(BDSP_AF_P_sFMM_GATE_OPEN_CONFIG)));
                psTaskFmmGateOpenConfig->ui32NumPorts += sDependentTaskGateOpenSettings.ui32NumPorts;
            }
            BKNI_Free(sDependentTaskGateOpenSettings.psFmmGateOpenConfig);
        }
    }

    /*Adding Gate open */
    ui32Error = BDSP_MMA_P_CopyDataToDram(&TaskGateOpenConfigAddr, (void *)psTaskFmmGateOpenConfig, SIZEOF(BDSP_AF_P_TASK_sFMM_GATE_OPEN_CONFIG));
    psGblTaskCfg->ui32FmmGateOpenConfigAddr = TaskGateOpenConfigAddr.offset;
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in Copying the Gate Open Configurations to Firmware Offset"));
		goto end;
	}

    /*Populating FwHw starts here*/
    ui32Error = BDSP_Arm_P_PopulateFwHwBuffer( (void *)pPrimaryStage, &sFwHwCfg);
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in Populating the FwHwBuffer"));
		goto end;
	}
    ui32Error = BDSP_MMA_P_CopyDataToDram(&TaskFwHwCfgAddr, (void *)&sFwHwCfg, SIZEOF(BDSP_AF_P_sFW_HW_CFG));
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in copying the FwHwBuffer to Firmware Offset"));
		goto end;
	}
    /* Add Fw Hw cfg address*/
    psGblTaskCfg->ui32TaskFwHwCfgAddr = TaskFwHwCfgAddr.offset;

    /*First populate sAlgoModePresent, along with that get the number of nodes in Network also*/
    BDSP_Arm_P_PopulateAlgoMode(pPrimaryStage, &sAlgoModePresent);

    if (pStartSettings->pSampleRateMap)
    {
        /*Filling the Fw Op sampling map LUT */
        ui32Error = BDSP_MMA_P_CopyDataToDram(&FwOpSamplingFreqMapLutAddr, (void *)pStartSettings->pSampleRateMap, BDSP_CIT_P_TASK_FS_MAPPING_LUT_SIZE);
		if(ui32Error != BERR_SUCCESS)
		{
			BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in Copying the SampleRate LUT provided by APE for Firmware"));
			goto end;
		}
    }
    else
    {
        eDolbyMsUsageMode = BDSP_AF_P_DolbyMsUsageMode_eSingleDecodeMode;
        ui32Error = BDSP_P_FillSamplingFrequencyMapLut(eDolbyMsUsageMode, &sAlgoModePresent, &FwOpSamplingFreqMapLutAddr);
		if(ui32Error != BERR_SUCCESS)
		{
			BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in filling the Samplerate LUT when APE doesnot provide the same"));
			goto end;
		}
    }
    /* Add FW Op Sampling Frequency Cfg*/
    psGblTaskCfg->ui32FwOpSamplingFreqMapLutAddr = FwOpSamplingFreqMapLutAddr.offset;

    /*First populate sStcTrigConfig*/
    BDSP_P_PopulateStcTrigConfig(&sStcTrigConfig, pStartSettings);
    ui32Error = BDSP_MMA_P_CopyDataToDram(&StcTriggerCfgAddr, (void *)&sStcTrigConfig, SIZEOF(BDSP_AF_P_sStcTrigConfig));
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in copying the STCTrigger configuration to firmware offset"));
		goto end;
	}

    /* STC trigger config address */
    psGblTaskCfg->ui32StcTrigConfigAddr = StcTriggerCfgAddr.offset;

#if 0  /* Finding the Zero Fill Samples  */  /*Need to check whether FW is using */
    /* Finding the Zero Fill Samples  */  /*Need to check whether FW is using */
    ui32Error = BDSP_Raaga_P_GetNumZeroFillSamples(
                                    &ui32ZeroFillSamples,
                                    pPrimaryStage
                                );
	if(ui32Error != BERR_SUCCESS)
	{
		BDBG_ERR(("BDSP_Arm_P_FillGblTaskCfg: Error in filling the Number of Zerofill samples"));
		goto end;
	}
#else
    ui32ZeroFillSamples = 0;
#endif

    psGblTaskCfg->ui32NumberOfNodesInTask = ui32TotalNodes;

    /* Zero Fill samples ::: Currently not used by FW */
    psGblTaskCfg->ui32NumberOfZeroFillSamples = ui32ZeroFillSamples;
    psGblTaskCfg->eTimeBaseType = pArmTask->startSettings.timeBaseType;

end:
	BKNI_Free(psFmmDestCfg);
err_fmmdesc_alloc:
	BKNI_Free(psTaskFmmGateOpenConfig);

    BDBG_LEAVE(BDSP_Arm_P_FillGblTaskCfg);
    return ui32Error;
}

BERR_Code BDSP_Arm_P_GenCit(void* pTaskHandle)
{
    BERR_Code ui32Err = BERR_SUCCESS ;

    BDSP_ArmTask *pArmTask = (BDSP_ArmTask *)pTaskHandle;
    BDSP_ARM_CIT_P_Output   *psCitOp = &(pArmTask->citOutput);

    /*BDSP_ARM_CIT_P_sTaskBuffInfo sTaskBuffInfo;*/
    unsigned ui32TotalNodes = 0;

    BDBG_ENTER(BDSP_Arm_P_GenCit);

    BDBG_ASSERT(NULL != pArmTask);
    BDBG_ASSERT(NULL != pArmTask->startSettings.primaryStage);

    ui32Err = BDSP_Arm_P_FillNodeCfg(
                    (void *) pArmTask->startSettings.primaryStage->pStageHandle,
                    pArmTask->settings.dspIndex,
                    &psCitOp->sCit.sNodeConfig[0],
                    &ui32TotalNodes);

    if( ui32Err != BERR_SUCCESS || ui32TotalNodes == 0)
    {
        goto BDSP_CITGENMODULE_P_EXIT_POINT;
    }

    if(ui32TotalNodes > BDSP_AF_P_MAX_NODES)
    {
        BDBG_ERR(("Error : The number of nodes in the system is %d. Maximum Allowed is %d", ui32TotalNodes,BDSP_AF_P_MAX_NODES));
        return(BERR_NOT_SUPPORTED);
    }
    BDBG_MSG(("ui32TotalNodes in Network = %d", ui32TotalNodes));

    /*  Fill the global task configuration into CIT */
    ui32Err = BDSP_Arm_P_FillGblTaskCfg(
                            pArmTask,
                            &psCitOp->sCit,
                            ui32TotalNodes); /*BDSP_AF_P_sTASK_CONFIG*/

BDSP_CITGENMODULE_P_EXIT_POINT:

    /* Check for Error and assert */
    if(ui32Err !=BERR_SUCCESS)
    {
	BDBG_ERR(("BDSP_Arm_P_GenCit: Error in Filling the CIT for ARM"));
        BDBG_ASSERT(0);
    }

    BDBG_LEAVE(BDSP_Arm_P_GenCit);
    return ui32Err;
}
