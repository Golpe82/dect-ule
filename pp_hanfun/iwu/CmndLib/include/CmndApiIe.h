/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_IE_H
#define _CMND_API_IE_H

#include "IeList.h"

#ifndef VAR_ON_MCU
#include "TypeDefs.h"
#include "CmndApiExported.h"
#else
#include "TypeDefs.h"
#include "CmndApiExported.h"
#include "CmndApiHost.h"
#endif // VAR_ON_MCU

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATTRIBUTE_VALUE to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_AttrValue       CMND_IE_ATTRIBUTE_VALUE
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAttributeValueAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttributeValue* pst_AttrValue );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_ATTRIBUTE_VALUE from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_AttrValue       CMND_IE_ATTRIBUTE_VALUE
/// @param[in]      u16_Length          length of pst_AttrValue
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAttributeValueGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAttributeValue* pst_AttrValue, u16 u16_Length );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_RESPONSE to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_Response        CMND_IE_RESPONSE
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeResponseAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeResponse* pst_Response );

/// @brief      Add CMND_IE_SET_ATTRIBUTE_VALUE to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SetAttrValue    CMND_IE_SET_ATTRIBUTE_VALUE
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSetAttributeValueAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue );

/// @brief      Get CMND_IE_SET_ATTRIBUTE_VALUE from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_SetAttrValue    CMND_IE_ATTRIBUTE_VALUE
/// @param[in]      u16_MaxLength       length of pst_SetAttrValue
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSetAttributeValueGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue, u16 u16_MaxLength );

bool p_hanCmndApi_IeGeneralStatusAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeGeneralStatus* pst_GenStat );

bool p_hanCmndApi_IeUnitAddrAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeUnitAddr* pst_UnitAddr );

bool p_hanCmndApi_IeDeRegistrationAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeDeRegistration* pst_DeRegIe );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_GENERAL_STATUS from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_GenStat         CMND_IE_GENERAL_STATUS
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeGeneralStatusGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeGeneralStatus* pst_GenStat );

bool p_hanCmndApi_IeDeRegistrationResAdd( IN t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeDeRegistrationResponse* pst_DeRegResIe );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_BASE_WANTED to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_BaseWanted      CMND_IE_BASE_WANTED
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeBaseWantedAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBaseWanted* pst_BaseWanted );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_BATTERY_MEASURE_INFO to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_MeasureInfo     CMND_IE_BATTERY_MEASURE_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeBatteryMeasureInfoAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBatteryMeasureInfo* pst_MeasureInfo );

bool p_hanCmndApi_IeDeRegistrationResGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeDeRegistrationResponse* pst_DeRegResIe );

bool p_hanCmndApi_IeVersionAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeVersion* pst_Version );

bool p_hanCmndApi_IeParameterAdd( t_st_hanIeList*                   pst_IeList,
                                  const t_st_hanCmndIeParameter*    pst_hanCmndIeParameter );

bool p_hanCmndApi_IeParameterDirectAdd( t_st_hanIeList* pst_IeList,const t_st_hanCmndIeParameterDirect *pst_hanCmndIeParameter );

bool p_hanCmndApi_IeAttributeIdGet( IN  t_st_hanIeList*             pst_IeList,
                                    OUT t_st_hanCmndIeAttributeId*  pst_AttributeId );
bool p_hanCmndApi_IeAttributeIdSet ( OUT t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeAttributeId*    pst_AttributeId );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATTRIBUTE_ID to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_AttributeId     CMND_IE_ATTRIBUTE_ID
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAttributeIdAdd ( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttributeId*   pst_AttributeId );

bool p_hanCmndApi_IeAttrReportEventCreateGet(   IN  t_st_hanIeList*             pst_IeList,
                                            OUT t_st_hanCmndIeAttrRepInt*   pst_AttributeReport );
bool p_hanCmndApi_IeAttrReportPeriodicCreateGet(    IN  t_st_hanIeList*             pst_IeList,
                                            OUT t_st_hanCmndIeAttrRepInt*   pst_AttributeReport );
bool p_hanCmndApi_IeAttrReportAddEntryGet(  IN  t_st_hanIeList*                 pst_IeList,
                                            OUT t_st_hanCmndIeAddReportEntries* pst_addEventReport );
bool p_hanCmndApi_IeAttrRepResponseGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAttrRepResponse* pst_Response );

bool p_hanCmndApi_IeAttrRepDeleteGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeDeleteReport* pst_delReport );

bool p_hanCmndApi_IeUpdatePeriodicRepIntervalGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeUpdatePeriodicReport* pst_UpdatePeriodicReport );

bool p_hanCmndApi_IeAttrRepResponseAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttrRepResponse* pst_Response );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_REPORT_ID to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_ReportId        CMND_IE_REPORT_ID
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAttrRepReportIdAdd (t_st_hanIeList* pst_IeList, const t_st_hanCmndIeReportId* pst_ReportIdWithType);

bool p_hanCmndApi_IeAttrRepNotifAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeReportInfoInd* pst_Event );

bool p_hanCmndApi_IeRegistrationResponseAdd( t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeRegistrationResponse* pst_RegResponse );
bool p_hanCmndApi_IeBaseWantedGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeBaseWanted *pst_BaseWanted );

bool p_hanCmndApi_IeBatteryLevelAdd( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBatteryLevel* pst_BatLevel );

bool p_hanCmndApi_IeU8Add( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU8* pst_U8Gen );

bool p_hanCmndApi_IeU16Add( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU16* pst_U16Gen );

bool p_hanCmndApi_IeU32Add( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU32* pst_U32Gen );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_BG_REQ to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_BandGap         CMND_IE_BG_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeBandGapAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBandGap* pst_BandGap );

bool p_hanCmndApi_IeBandGapResAdd( t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeBandGapRes* pst_BandGapRes );

bool p_hanCmndApi_IeAteContReqGet( t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeAteContReq* pst_AteContReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATE_CONT_REQ to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList              pointer to target IeList
/// @param[in]      pst_AteContReq          CMND_IE_ATE_CONT_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteContReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteContReq* pst_AteContReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATE_RX_REQ to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_AteRxRes        CMND_IE_ATE_RX_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteRxReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteRxReq* pst_AteRxRes );

bool p_hanCmndApi_IeAteRxResAdd( t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeAteRxRes* pst_AteRxRes );

bool p_hanCmndApi_IeAteTxReqGet( t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeAteTxReq* pst_AteRxReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_ATE_RX_REQ from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_AteRxReq        CMND_IE_ATE_RX_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteRxReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteRxReq* pst_AteRxReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_ATE_RX_RES from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_AteRxRes        CMND_IE_ATE_RX_RES
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteRxResGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteRxRes* pst_AteRxRes );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATE_RX_RES to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_AteRxRes        CMND_IE_ATE_RX_RES
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteRxResAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteRxRes* pst_AteRxRes );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ATE_RX_REQ to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_AteRxReq        CMND_IE_ATE_RX_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteTxReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteTxReq* pst_AteRxReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_ATE_TX_REQ from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_AteTxReq        CMND_IE_ATE_TX_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAteTxReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteTxReq* pst_AteTxReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add ReportId to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      u8_ReportId         ReportId
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeReportIdAdd( t_st_hanIeList* pst_IeList, u8  u8_ReportId);

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get ReportId from IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     u8_ReportId         ReportId
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeReportIdGet( t_st_hanIeList* pst_IeList, u8* pu8_ReportId );

bool p_hanCmndApi_IeAttrReportAddEntryAdd(  t_st_hanIeList*                             pst_IeList,
                                            const t_st_hanCmndIeAddReportEntries*       pst_addReportEntries,
                                            u16                                         u16_Length );
bool p_hanCmndApi_IeReportEntriesAdd(   t_st_hanIeList*                     pst_IeList,
                                        const t_st_hanCmndIeReportEntries*  pst_ReportEntries,
                                        u16                                 u16_Length );

bool p_hanCmndApi_IeAddReportInfoAdd(   t_st_hanIeList*                 pst_IeList,
                                        const t_st_hanCmndIeAddReport*  pst_AddReport );

bool p_hanCmndApi_IeAddReportInfoGet(   IN  t_st_hanIeList*             pst_IeList,
                                        OUT t_st_hanCmndIeAddReport*    pst_addReport );

bool p_hanCmndApi_IeReportInfoGet(  t_st_hanIeList*                 pst_IeList,
                                    t_st_hanCmndIeReportInfoInd*    pst_ReportInfo );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_REPORT_INFO to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_ReportInfo      CMND_IE_REPORT_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeReportInfoAdd(  t_st_hanIeList*                     pst_IeList,
                                    const t_st_hanCmndIeReportInfoInd*  pst_ReportInfo );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_PMID to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_Pmid            CMND_IE_PMID
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IePmidAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIePMID* pst_Pmid );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_PORTABLE_IDENTITY to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_PI              CMND_IE_PORTABLE_IDENTITY
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IePortableIdentityAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIePortableIdentity* pst_PI );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_PORTABLE_IDENTITY from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_PI              CMND_IE_PORTABLE_IDENTITY
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IePortableIdentityGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIePortableIdentity* pst_PI );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_NEW_SW_INFO from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_SuotaSwReady    CMND_IE_NEW_SW_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaNewSwReadyGet( t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeSuotaNewSwReady* pst_SuotaSwReady );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_NEW_SW_INFO to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SuotaSwReady    CMND_IE_NEW_SW_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaNewSwReadyAdd( t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeSuotaNewSwReady* pst_SuotaSwReady );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add ImageType to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_ImageType       pointer to IeU8 with ImageType
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaImageTypeAdd ( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU8* pst_ImageType );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get current sw version from IeList
///             Usually, this way using for getting IE's to CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_SuotaSw     current sw version structure
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaCurrentSwGet( IN t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeSuotaCurrentSw* pst_SuotaSw );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_CURRENT_SW_INFO to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SuotaCurrentSw  CMND_IE_CURRENT_SW_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaCurrentSwAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSuotaCurrentSw* pst_SuotaCurrentSw );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get Suota image type from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_U8Gen           result placed inside an IE U8
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaImageTypeGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeU8* pst_U8Gen );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_SW_VER_INFO to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SuotaSwVer      CMND_IE_SW_VER_INFO
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaSwVerAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSuotaSwVer* pst_SuotaSwVer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_READ_FILE_DATA_RES to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SuotaFileData   CMND_IE_READ_FILE_DATA_RES
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaReadFileAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFileDataRes* pst_SuotaFileData );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_READ_FILE_DATA_REQ to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_SuotaFileData   CMND_IE_READ_FILE_DATA_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaReadFileReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFileDataReq* pst_SuotaFileData );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_READ_FILE_DATA_REQ from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList                  pointer to target IeList
/// @param[out]     pst_SuotaFileDataReq        CMND_IE_READ_FILE_DATA_REQ
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaReadFileReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeFileDataReq* pst_SuotaFileDataReq );

bool p_hanCmndApi_IeLogAdd( t_st_hanIeList* pst_IeList, const u8 *pLogBuf );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_READ_FILE_DATA_RES from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList                  pointer to target IeList
/// @param[out]     pst_SuotaFileDataRes        CMND_IE_READ_FILE_DATA_RES
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeSuotaReadFileResGet( IN t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeFileDataRes* pst_SuotaFileDataRes );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_TAMPER_ALERT from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_TamperAlert     CMND_IE_TAMPER_ALERT
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeTamperAlertStatusGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeTamperAlert* pst_TamperAlert );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_FUN to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_Fun             CMND_IE_FUN
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeFunAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFun* pst_Fun );

bool p_hanCmndApi_IeSeqNumberAdd( t_st_hanIeList* pst_IeList, const u8* pu8_SeqNumber );

bool p_hanCmndApi_IeSeqNumberGet( t_st_hanIeList* pst_IeList, u8* pu8_SeqNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get CMND_IE_GPIO_STATE from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList          pointer to target IeList
/// @param[out]     pst_GpioState       CMND_IE_GPIO_STATE
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeGpioStateGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeGpioState* pst_GpioState );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_GPIO_STATE to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_GpioState       CMND_IE_GPIO_STATE
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeGpioStateAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeGpioState* pst_GpioState );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ALERT to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_IeAlarm         CMND_IE_ALERT
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeAlertAdd( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAlert* pst_IeAlarm );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add ResponseRequired flag to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      b_ResponseRequired  flag
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeResposeRequiredAdd( t_st_hanIeList* pst_IeList, bool b_ResponseRequired );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get ResponseRequired flag from IeList
///             Usually, this way using for getting IE's from CMND messages
///
/// @param[in]      pst_IeList              pointer to target IeList
/// @param[out]     pb_ResponseRequired     response required flag
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeResposeRequiredGet( t_st_hanIeList* pst_IeList, bool* pb_ResponseRequired );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_LINK_MAINTAIN to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_IeLinkMaintain  CMND_IE_LINK_MAINTAIN
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeLinkMaintainAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeLinkMaintain* pst_IeLinkMaintain );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add CMND_IE_ULE_CALL_SETTING to IeList
///             Usually, this way using for adding IE's to CMND messages
///
/// @param[inout]   pst_IeList          pointer to target IeList
/// @param[in]      pst_IeCallSettings  CMND_IE_ULE_CALL_SETTING
/// @return     true if success
///////////////////////////////////////////////////////////////////////////////
bool p_hanCmndApi_IeCallSettingsAdd ( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings );

#ifdef VAR_ON_CMND

bool p_hanCmndApi_IeCpStatisticsAdd(  IN OUT t_st_hanIeList* pst_IeList, IN const t_st_lm09_ULEStatistics* pst_StatIe );
bool p_hanCmndApi_IeAttributeSetPackResAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttributeSetPackRes* pst_AttribRes, u16 u16_Length );
bool p_hanCmndApi_IeRegistrationAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeRegistration* pst_RegIe );
bool p_hanCmndApi_IeAttributeIdListGet( IN t_st_hanIeList*                  pst_IeList,
                                       OUT t_st_hanCmndIeAttributeIdList*   pst_AttributeIdList,
                                       u16                              u16_StructMaxSize );
bool p_hanCmndApi_IeRfTestModeGet( t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeRfTestMode* pst_RfTestModeReq );

bool p_hanCmndApi_IeRfTestModeAddStat( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeRfTestModeStat* pst_StatIe );
bool p_hanCmndApi_IeOtaControlGet( t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeOtaControl* pst_OtaControl );
bool p_hanCmndApi_IeSimplePowerMeteringReportAdd(   t_st_hanIeList*                         pst_IeList,
                                                    const t_st_hanCmndIeSimplePwrMtrReport* pst_Report,
                                                    u16                                     u16_Length );
bool p_hanCmndApi_IeEepromOffsetGet(    t_st_hanIeList*                     pst_IeList,
                                        t_st_hanCmndIeEepromOffsetValue*    pst_EepromOffsetValue,
                                        u16                                 u16_MaxLength );

bool p_hanCmndApi_IeEepromOffsetAdd(    t_st_hanIeList*                     pst_IeList,
                                        const t_st_hanCmndIeEepromOffsetValue*  pst_EepromOffsetValue );

bool p_hanCmndApi_IeEepromOffsetValueAdd(   t_st_hanIeList*                         pst_IeList,
                                         const t_st_hanCmndIeEepromOffsetValue* pst_EepromOffsetValue,
                                         u16                                        u16_Length );
bool p_hanCmndApi_IeEepromOffsetValueGet(   t_st_hanIeList*                     pst_IeList,
                                         t_st_hanCmndIeEepromOffsetValue*   pst_EepromOffsetValue,
                                         u16                                    u16_MaxLength );
bool p_hanCmndApi_IeSpMktScheduleGet( IN OUT t_st_hanIeList* pst_IeList, t_st_hanCmndIeSpmktScheduleInfo* pst_hanCmndIeSpmktScheduleInfo );

bool p_hanCmndApi_IeSpMktTextGet( IN OUT t_st_hanIeList* pst_IeList, t_st_hanCmndIeSpmktTextUpdate* pst_hanCmndIeSpmktTextUpdate );

bool p_hanCmndApi_IeLedIndAdd( IN OUT t_st_hanIeList* pst_IeList, const t_st_hanCmndIeLedIndication* pst_IeLedInd );

bool p_hanCmndApi_IeLedIndGet( IN t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeLedIndication* pst_IeLedInd );

bool p_hanCmndApi_IeDebugDevStateAdd( IN OUT t_st_hanIeList* pst_IeList, IN const t_st_hanCmndIeDebugDevState* pst_DevState );

#endif // VAR_ON_CMND

extern_c_end

#endif // _CMND_API_IE_H
