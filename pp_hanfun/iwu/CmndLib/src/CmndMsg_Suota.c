/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_Suota.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Suota_CreateDownloadStartReq(    OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeSuotaNewSwReady*    pst_SwInfo,
                                            const t_st_hanCmndIeU8*                 pst_ImageType )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg->messageId = CMND_MSG_SUOTA_DOWNLOAD_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add SUOTA NewSwReady IE
    ok = p_hanCmndApi_IeSuotaNewSwReadyAdd ( &st_IeList, pst_SwInfo );
    if( ok )
    {
        // Add Suota image type IE
        ok = p_hanCmndApi_IeSuotaImageTypeAdd( &st_IeList, pst_ImageType );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create Download abort message
bool p_CmndMsg_Suota_CreateDownloadAbortReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SUOTA_DOWNLOAD_ABORT_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of read file message
bool p_CmndMsg_Suota_CreateReadFileReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeFileDataReq* pst_ReadFileData )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg->messageId = CMND_MSG_SUOTA_READ_FILE_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add SUOTA read file req IE
    ok = p_hanCmndApi_IeSuotaReadFileReqAdd ( &st_IeList, pst_ReadFileData );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of upgrade completed message
bool p_CmndMsg_Suota_CreateUpgradeCompletedReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeSuotaSwVer* pst_UpgradeCompleted )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg->messageId = CMND_MSG_SUOTA_UPGRADE_COMPLETED_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add SuotaSwVer IE
    ok = p_hanCmndApi_IeSuotaSwVerAdd( &st_IeList, pst_UpgradeCompleted );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of new software response message
bool p_CmndMsg_Suota_CreateNewSwRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeResponse* pst_Response )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg->messageId = CMND_MSG_SUOTA_NEW_SW_RES;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );


    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd( &st_IeList, pst_Response );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of image ready response message
bool p_CmndMsg_Suota_CreateImageReadyRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SUOTA;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SUOTA_IMAGE_READY_RES;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
