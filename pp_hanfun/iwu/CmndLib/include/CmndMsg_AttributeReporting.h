/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_ATTRIBUTE_REPORTING_H
#define _CMND_MSG_ATTRIBUTE_REPORTING_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiHost.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create AttributeReporting notification request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_ReportEntry     - pointer to the report info structure
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_AttributeReporting_CreateReportNotificationReq(  OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                                                const t_st_hanCmndIeReportInfoInd*  pst_ReportEntry );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create AttributeReporting AddReportInd packet buffer based on given parameters.
///         The <pu8_Buffer> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_ReportEntry     - pointer to the report info structure
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_AttributeReporting_CreateAddReportInd(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                        const t_st_hanCmndIeAddReportEntries*   pst_ReportEntry );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create AttributeReporting AddReportInd response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      en_Response                 - response code
/// @param[in]      pst_ReportId                - pointer to CMND_IE_REPORT_ID
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_AttributeReporting_CreateAddReportRes(   OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                                        t_en_hanCmndRc                  en_Response,
                                                        const t_st_hanCmndIeReportId*   pst_ReportId );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create AttributeReporting GetReportValueNotification response message
///         buffer based on given parameters. The <pu8_Buffer> will contain all
///         CMND API message fields. Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_ReportInfo              - pointer to the report info structure
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_AttributeReporting_CreateGetReportValuesRes( OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                                            const t_st_hanCmndIeReportInfoInd*  pst_ReportInfo );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create AttributeReporting AddReport request packet buffer based on
///         given parameters. The <pst_hanCmndApiMsg> will contain all CMND API message
///         fields. Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_AddReport               - pointer to the add report structure
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_AttributeReporting_CreateAddReportReq(   OUT t_st_hanCmndApiMsg*     pst_hanCmndApiMsg,
                                                        t_st_hanCmndIeAddReport*    pst_AddReport );

extern_c_end

#endif  //_CMND_MSG_ATTRIBUTE_REPORTING_H
