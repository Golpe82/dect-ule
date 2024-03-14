/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg.h"
#include "CmndMsg_Tamper.h"
#include "CmndApiIe.h"
#include "Endian.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeResponseIsOk( t_st_hanCmndApiMsg* pst_Msg )
{
    t_st(CMND_IE_RESPONSE)  st_Response = { 0 };

    if ( p_CmndMsg_IeGet(IN pst_Msg, p_CMND_IE_GETTER(CMND_IE_RESPONSE), OUT &st_Response, sizeof(st_Response)) )
    {
        if (st_Response.u8_Result == CMND_RC_OK)
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeResponseRequiredAdd( INOUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    bool ok = false;
    t_st_hanIeList  st_IeList = {0};
    u8 u8_ResponseRequired = 1;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateWithPayloadAppendable(    &pst_hanCmndApiMsg->data,
        pst_hanCmndApiMsg->dataLength,
        sizeof(pst_hanCmndApiMsg->data),
        &st_IeList );

    // Add CMND_IE_RESPONSE_REQUIRED
    if ( p_hanIeList_AddIeSimple( &st_IeList, CMND_IE_RESPONSE_REQUIRED, &u8_ResponseRequired, sizeof(u8_ResponseRequired) ) )
    {
        pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );
        ok = true;
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeUnitAddrAdd( INOUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeUnitAddr* pst_IeUnitAddr )
{
    bool ok = false;
    t_st_hanIeList  st_IeList = {0};

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateWithPayloadAppendable(    &pst_hanCmndApiMsg->data,
                                                pst_hanCmndApiMsg->dataLength,
                                                sizeof(pst_hanCmndApiMsg->data),
                                                &st_IeList );

    // Add CMND_IE_UNIT_ADDR
    if ( p_hanCmndApi_IeUnitAddrAdd( &st_IeList, pst_IeUnitAddr ) )
    {
        pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );
        ok = true;
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeGet(IN const t_st_hanCmndApiMsg* pst_Msg, tpf_CmndIeGetter pf_Getter, OUT void* pv_IeValue, u16 u16_IeSize)
{
    t_st_hanIeList              st_IeList = { { 0 } };

    p_hanIeList_CreateWithPayload( IN pst_Msg->data, pst_Msg->dataLength, OUT &st_IeList );
    return p_CmndMsg_IeGetFromList(IN &st_IeList, pf_Getter, OUT pv_IeValue, u16_IeSize);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeGetFromList(IN const t_st_hanIeList* pst_IeList, tpf_CmndIeGetter pf_Getter, OUT void* pv_IeValue, u16 u16_IeSize)
{
    bool                        ok = false;
    t_st_hanIeStruct            st_Ie;
    t_st_StreamBuffer           st_IeDataStream;        // Stream for working with IE data
    t_en_hanCmndInfoElemType    en_IeType;

    do
    {
        BREAK_IF( !pf_Getter || !pv_IeValue );

        //First call is required just to obtain IE type
        en_IeType = pf_Getter( NULL, NULL );

        // Find the IE
        BREAK_IF( !p_hanIeList_FindIeByType( IN (t_st_hanIeList*) pst_IeList, en_IeType, OUT &st_Ie ) );

        // Ensure the length is matched exactly
        BREAK_IF( st_Ie.u16_Len > u16_IeSize );

        // Put found IE payload into Stream Buffer
        p_hanStreamBuffer_CreateWithPayload(    OUT &st_IeDataStream,
                                                st_Ie.pu8_Data,
                                                st_Ie.u16_Len,
                                                st_Ie.u16_Len );

        // Call IE-specific getter to fill IE value
        pf_Getter( IN &st_IeDataStream, OUT pv_IeValue ); // No need to save return value this time

        // Ensure there was no attempts to read more data than Stream Buffer has
        BREAK_IF( p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream ) );

        ok = true;
    }while(0);

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
