/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_AttributeReporting.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Add report request message
bool p_CmndMsg_AttributeReporting_CreateAddReportReq(   OUT t_st_hanCmndApiMsg*     pst_hanCmndApiMsg,
                                                        t_st_hanCmndIeAddReport*    pst_AddReport )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ATTRIBUTE_REPORTING;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ATTRREP_ADD_REPORT_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    ok = p_hanCmndApi_IeAddReportInfoAdd( &st_IeList, pst_AddReport );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of report notification request message
bool p_CmndMsg_AttributeReporting_CreateReportNotificationReq(  OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                                                const t_st_hanCmndIeReportInfoInd*  pst_ReportInfo )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ATTRIBUTE_REPORTING;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ATTRREP_REPORT_NOTIFICATION_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );
    // Add AttributeReporting Rep Notif IE
    ok = p_hanCmndApi_IeReportInfoAdd( &st_IeList, pst_ReportInfo );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of report notification message
bool p_CmndMsg_AttributeReporting_CreateGetReportValuesRes( OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                                            const t_st_hanCmndIeReportInfoInd*  pst_ReportInfo )
{
    t_st_hanCmndIeResponse  st_Response = {0};

    bool                    ok;
    t_st_hanIeList          st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ATTRIBUTE_REPORTING;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ATTRREP_GET_REPORT_VALUES_RES;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    ok = p_hanCmndApi_IeResponseAdd( &st_IeList, &st_Response );
    if( ok )
    {
        // Add AttributeReporting Rep Notif IE
        ok = p_hanCmndApi_IeReportInfoAdd( &st_IeList, pst_ReportInfo );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of add report responce message
bool p_CmndMsg_AttributeReporting_CreateAddReportRes(   OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                                        t_en_hanCmndRc                  en_Response,
                                                        const t_st_hanCmndIeReportId*   pst_ReportId )
{
    bool                            ok;
    t_st_hanIeList                  st_IeList;
    t_st_hanCmndIeAttrRepResponse   st_Response;


    st_Response.u8_Result = (u8)en_Response;
    st_Response.u8_ReportId = (u8)pst_ReportId->u8_ReportId;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ATTRIBUTE_REPORTING;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ATTRREP_ADD_REPORT_RES;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add AttributeReporting Report Id IE
    ok = p_hanCmndApi_IeAttrRepResponseAdd (&st_IeList, &st_Response);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Add Report indication message
bool p_CmndMsg_AttributeReporting_CreateAddReportInd(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                        const t_st_hanCmndIeAddReportEntries*   pst_ReportEntry )
{
    bool            ok;
    t_st_hanIeList  st_IeList;
    u16             u16_Length      = sizeof(t_st_hanCmndIeAddReportEntries)
                                        - CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE
                                        + pst_ReportEntry->u8_NumEntries;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ATTRIBUTE_REPORTING;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ATTRREP_ADD_REPORT_IND;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );
    // Add AttributeReporting Add Entry IE
    ok = p_hanCmndApi_IeAttrReportAddEntryAdd( &st_IeList, pst_ReportEntry, u16_Length ); //TODO a.strunin: potential bug

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
