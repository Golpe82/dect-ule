/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndApiIe.h"
#include <stdlib.h>
#include "Endian.h"
#include "IeList.h"
#include "Logger.h"
#include "FunProfiles.h"
#include "CmndMsg.h"

#include <string.h> //memcpy

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttributeValueAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttributeValue* pst_AttrValue )
{
    // The content of pst_AttrValue can be copied as is
    // no conversions or special copies required.
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATTRIBUTE_VALUE,
                                    (u8*)pst_AttrValue,
                                    CMND_IE_ATTRIBUTE_VALUE_HEADER_SIZE + pst_AttrValue->u8_DataLength  );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttributeValueGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAttributeValue* pst_AttrValue, u16 u16_MaxLength )
{
    bool                RetVal = false;
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATTRIBUTE_VALUE, &st_Ie ) )
    {
        // verify the length
        if ( st_Ie.u16_Len <= u16_MaxLength )
        {
            // copy all fields
            memcpy( pst_AttrValue, st_Ie.pu8_Data, st_Ie.u16_Len );
            return true;
        }
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSetAttributeValueAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue )
{
    // The content of pst_AttrValue can be copied as is
    // no conversions or special copies required.
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_SET_ATTRIBUTE_VALUE,
                                    (u8*)pst_SetAttrValue,
                                    CMND_IE_SET_ATTRIBUTE_VALUE_HEADER_SIZE + pst_SetAttrValue->u8_DataLength  );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSetAttributeValueGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue, u16 u16_MaxLength )
{
    bool                RetVal = false;
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( p_hanIeList_FindIeByType( pst_IeList, CMND_IE_SET_ATTRIBUTE_VALUE, &st_Ie ) )
    {
        // verify the length
        if ( st_Ie.u16_Len <= u16_MaxLength )
        {
            // copy all fields
            memcpy( pst_SetAttrValue, st_Ie.pu8_Data, st_Ie.u16_Len );
            return true;
        }
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeResponseAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeResponse* pst_Response )
{
    t_st_hanIeStruct st_Ie;

    st_Ie.u8_Type   = CMND_IE_RESPONSE;
    st_Ie.u16_Len   = sizeof(t_st_hanCmndIeResponse);
    st_Ie.pu8_Data  = (u8*)pst_Response;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_RESPONSE)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_RESPONSE)* pst_Ie = (t_st(CMND_IE_RESPONSE)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u8_Result = p_hanStreamBuffer_GetData8(pst_Stream);
    }
    return CMND_IE_RESPONSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_BATTERY_LEVEL)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_BATTERY_LEVEL)* pst_Ie = (t_st(CMND_IE_BATTERY_LEVEL)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u16_BatteryLevel = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
    }
    return CMND_IE_BATTERY_LEVEL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_GENERAL_STATUS)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_GENERAL_STATUS)* pst_Ie = (t_st(CMND_IE_GENERAL_STATUS)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );
        pst_Ie->u16_DeviceID = p_Endian_net2hos16( pst_Ie->u16_DeviceID );
    }
    return CMND_IE_GENERAL_STATUS;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeBatteryMeasureInfoAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBatteryMeasureInfo* pst_MeasureInfo )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_BATTERY_MEASURE_INFO,
                                    (u8*)pst_MeasureInfo,
                                    sizeof(t_st_hanCmndIeBatteryMeasureInfo) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeBaseWantedAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBaseWanted* pst_baseWanted )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_BASE_WANTED,
                                    (u8*)pst_baseWanted,
                                    sizeof(t_st_hanCmndIeBaseWanted) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeUnitAddrAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeUnitAddr* pst_UnitAddr )
{
    t_st_hanCmndIeUnitAddr  st_NewUnitAddr;

    // converting  from host to network because all IEs payloads are assumed to be in network order
    // (meaning IE always come from MCU in network order, but here are the ones who generated the IE,
    // so a conversion is required)
    memcpy( &st_NewUnitAddr, pst_UnitAddr, sizeof( t_st_hanCmndIeUnitAddr ) );
    st_NewUnitAddr.u16_DeviceId = p_Endian_hos2net16( st_NewUnitAddr.u16_DeviceId );

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_UNIT_ADDR,
                                    (u8*) &st_NewUnitAddr,
                                    sizeof(t_st_hanCmndIeUnitAddr) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_UNIT_ADDR)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_UNIT_ADDR)* pst_Ie = (t_st(CMND_IE_UNIT_ADDR)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );
        pst_Ie->u16_DeviceId = p_Endian_hos2net16(pst_Ie->u16_DeviceId);
    }
    return CMND_IE_UNIT_ADDR;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_VERSION)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_VERSION)* pst_Ie = (t_st(CMND_IE_VERSION)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u8_lenVersion = p_hanStreamBuffer_GetData8(pst_Stream);
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, pst_Ie->u8_lenVersion);
    }
    return CMND_IE_VERSION;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_PARAMETER)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_PARAMETER)* pst_Ie = (t_st(CMND_IE_PARAMETER)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required

        pst_Ie->e_ParamType = p_hanStreamBuffer_GetData8(pst_Stream);
        pst_Ie->e_Param = p_hanStreamBuffer_GetData8(pst_Stream);
        pst_Ie->u16_DataLen = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
        p_hanSreamBuffer_GetData8Array( pst_Stream, pst_Ie->pu8_Data, pst_Ie->u16_DataLen);
    }
    return CMND_IE_PARAMETER;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeParameterAdd( t_st_hanIeList*                       pst_IeList,
                                  const t_st(CMND_IE_PARAMETER)*    pst_hanCmndIeParameter )
{
    t_st_hanIeStruct st_Ie;
    bool ok = false;

    st_Ie.u16_Len = CMND_IE_PARAMETER_HEADER_SIZE + pst_hanCmndIeParameter->u16_DataLen;

    if ( st_Ie.u16_Len <= CMND_API_PAYLOAD_MAX_LENGTH )
    {

        ((t_st(CMND_IE_PARAMETER)*)pst_hanCmndIeParameter)->u16_DataLen = p_Endian_hos2net16(pst_hanCmndIeParameter->u16_DataLen);

        st_Ie.u8_Type   = CMND_IE_PARAMETER;
        st_Ie.pu8_Data  = (u8*)pst_hanCmndIeParameter;

        ok = p_hanIeList_AddIe( pst_IeList, &st_Ie );
        ((t_st(CMND_IE_PARAMETER)*)pst_hanCmndIeParameter)->u16_DataLen = p_Endian_net2hos16(pst_hanCmndIeParameter->u16_DataLen);
    }

    return ok;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma pack(1)
#endif
typedef PACK_STRUCT
{
    //t_en_hanCmndParamerterAddrType    e_AddressType;      // EEPROM or RAM destination address
    u8                              u8_AddressType;     // EEPROM or RAM destination address (see t_en_hanCmndParamerterAddrType)
    u32                             u32_Offset;         // EERPOM offset or RAM address
    u16                             u16_DataLen;        // Length of data
}
t_st_hanCmndIeParameterDirectSpec;
#ifdef _MSC_VER
#pragma pack()
#endif

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_PARAMETER_DIRECT)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_PARAMETER_DIRECT)* pst_Ie = (t_st(CMND_IE_PARAMETER_DIRECT)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->e_AddressType = p_hanStreamBuffer_GetData8(pst_Stream);
        pst_Ie->u32_Offset = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(pst_Stream));
        pst_Ie->u16_DataLen = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
        p_hanSreamBuffer_GetData8Array( pst_Stream, pst_Ie->pu8_Data, pst_Ie->u16_DataLen);
    }
    return CMND_IE_PARAMETER_DIRECT;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeParameterDirectAdd( t_st_hanIeList* pst_IeList,
                                        const t_st_hanCmndIeParameterDirect *pst_hanCmndIeParameter )
{
    t_st_hanIeStruct st_Ie;
    u16 datalength = pst_hanCmndIeParameter->u16_DataLen;
    u8  directParamBuf[CMND_API_PAYLOAD_MAX_LENGTH] = {0};
    t_st_hanCmndIeParameterDirectSpec st_ParameterSpec = {0};

    st_Ie.u8_Type   = CMND_IE_PARAMETER_DIRECT;
    st_Ie.u16_Len   = sizeof(t_st_hanCmndIeParameterDirect) - CMND_IE_PARAMETER_DIRECT_DATA_MAX_LENGTH + datalength;

    st_ParameterSpec.u8_AddressType     = pst_hanCmndIeParameter->e_AddressType;
    st_ParameterSpec.u32_Offset         = p_Endian_hos2net32(pst_hanCmndIeParameter->u32_Offset);
    st_ParameterSpec.u16_DataLen        = p_Endian_hos2net16(pst_hanCmndIeParameter->u16_DataLen);

    memcpy( directParamBuf, &st_ParameterSpec, sizeof(st_ParameterSpec) );
    memcpy( &(directParamBuf[sizeof(st_ParameterSpec)]), pst_hanCmndIeParameter->pu8_Data, datalength );


    st_Ie.pu8_Data = directParamBuf;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttributeIdGet( IN  t_st_hanIeList*             pst_IeList,
                                    OUT t_st_hanCmndIeAttributeId*  pst_AttributeId )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATTRIBUTE_ID, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    // CMND IE payload is network order, so conversion is required
    pst_AttributeId->u8_AttributeId = p_hanStreamBuffer_GetData8( &st_IeDataStream );

    return !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttributeIdAdd( t_st_hanIeList* pst_IeList,
                                    const t_st_hanCmndIeAttributeId*    pst_AttributeId )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATTRIBUTE_ID,
                                    (u8*) pst_AttributeId,
                                    sizeof(t_st_hanCmndIeAttributeId) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrReportAddEntryAdd(  t_st_hanIeList*                         pst_IeList,
                                            const t_st_hanCmndIeAddReportEntries*   pst_addReportEntries,
                                            u16                                     u16_Length )
{
    // The content of t_st_hanCmndIeAddReportEntries can be copied as is
    // no conversions or special copies required.
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATTR_ADD_REPORT_ENTRY,
                                    (u8*)pst_addReportEntries,
                                    u16_Length );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrReportAddEntryGet(  IN  t_st_hanIeList*                 pst_IeList,
                                            OUT t_st_hanCmndIeAddReportEntries* pst_addReportEntries )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data
    u16                 left2Copy;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATTR_ADD_REPORT_ENTRY, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    // CMND IE payload is network order, so conversion is required
    pst_addReportEntries->u8_ReportId = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;
    pst_addReportEntries->u8_NumEntries = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;
    left2Copy = p_hanStreamBuffer_GetDataSize( &st_IeDataStream );
    if( left2Copy <= CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE )
    {
        p_hanSreamBuffer_GetData8Array( &st_IeDataStream, (u8 *)pst_addReportEntries->u8_ReportEntries,left2Copy );
    }

    return !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrRepResponseAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAttrRepResponse* pst_Response )
{
    t_st_hanIeStruct st_Ie;

    st_Ie.u8_Type   = CMND_IE_CREATE_ATTR_REPORT_RESPONSE;
    st_Ie.u16_Len   = sizeof(t_st_hanCmndIeAttrRepResponse);
    st_Ie.pu8_Data  = (u8*)pst_Response;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

bool p_hanCmndApi_IeAttrRepReportIdAdd(t_st_hanIeList* pst_IeList, const t_st_hanCmndIeReportId* pst_ReportIdWithType)
{
    t_st_hanIeStruct st_Ie;

    st_Ie.u8_Type   = CMND_IE_REPORT_ID;
    st_Ie.u16_Len   = sizeof(t_st_hanCmndIeReportId);
    st_Ie.pu8_Data  = (u8*)pst_ReportIdWithType;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrRepResponseGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAttrRepResponse* pst_Response )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_CREATE_ATTR_REPORT_RESPONSE, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    pst_Response->u8_Result = p_hanStreamBuffer_GetData8( &st_IeDataStream );
    pst_Response->u8_ReportId = p_hanStreamBuffer_GetData8( &st_IeDataStream );

    return !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrRepDeleteGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeDeleteReport* pst_delReport )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_REPORT_ID, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    pst_delReport->u8_ReportId = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;

    return !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAttrRepNotifAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeReportInfoInd* pst_Event )
{
    t_st_StreamBuffer                   st_IeDataStream;        // Stream for working with IE data
    t_st_hanIeStruct                    st_Ie;                  // The new constructed IE
    u8                                  au8_IeData[CMND_API_PAYLOAD_MAX_LENGTH];        // Container buffer for IE data
    bool                                RetVal = true;
    const t_st_hanCmndIeNtfReportEntry* pst_CurrentEntry = NULL;
    const t_st_hanCmndIeAttrCond*       pst_CurrentAttr = NULL;
    u32                                 i, j;
    u8                                  u8_NumOfAttributes;

    // use Stream Buffer to construct the IE payload/data
    p_hanStreamBuffer_CreateEmpty( &st_IeDataStream, au8_IeData, CMND_API_PAYLOAD_MAX_LENGTH );

    p_hanStreamBuffer_AddData8(     &st_IeDataStream, pst_Event->u8_ReportId );
    p_hanStreamBuffer_AddData8(     &st_IeDataStream, pst_Event->u8_NumOfReportEntries );

    for ( i = 0; i < pst_Event->u8_NumOfReportEntries ; i++ )
    {
        pst_CurrentEntry = &(pst_Event->st_NtfReportEntries[i]);
        //Unit ID
        p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentEntry->u8_UnitId );
        //Interface ID
        p_hanStreamBuffer_AddData16( &st_IeDataStream, p_Endian_hos2net16(pst_CurrentEntry->u16_InterfaceId) );
        //NumOfAttrib
        u8_NumOfAttributes = pst_CurrentEntry->u8_NumOfAttrib;
        p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentEntry->u8_NumOfAttrib );

        for ( j = 0; j < u8_NumOfAttributes; j++ )
        {
            pst_CurrentAttr = &(pst_CurrentEntry->st_ReportDataFields[j]);
            p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttr->u8_AttributeId );
            p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttr->u8_TypeOfReporting );
            p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttr->u8_AttributeSize );
            if( pst_CurrentAttr->u8_AttributeSize <=4)
            {
                p_hanStreamBuffer_AddData32( &st_IeDataStream, p_Endian_hos2net32(pst_CurrentAttr->u32_AttributeValue) );
            }
            else
            {
                // unsupported
                return false;
            }

        }
    }

    if ( RetVal )
    {
        // if there was an overrun when constructing IE data then return failure
        if ( p_hanStreamBuffer_CheckOverrun( &st_IeDataStream ) )
        {
            RetVal = false;
        }
        else
        {
            // fill IE fields
            st_Ie.u8_Type   = CMND_IE_ADD_REPORT_INFO;
            st_Ie.u16_Len   = p_hanStreamBuffer_GetDataSize( &st_IeDataStream );
            st_Ie.pu8_Data  = au8_IeData;

            RetVal = p_hanIeList_AddIe( pst_IeList, &st_Ie );
        }
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_REGISTRATION_RESPONSE)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_REGISTRATION_RESPONSE)* pst_Ie = (t_st(CMND_IE_REGISTRATION_RESPONSE)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );
        pst_Ie->u16_DeviceAddress = p_Endian_net2hos16( pst_Ie->u16_DeviceAddress );
    }
    return CMND_IE_REGISTRATION_RESPONSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeBatteryLevelAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBatteryLevel* pst_BatLevel )
{
    bool RetVal = true;

    t_st_hanCmndIeBatteryLevel st_BatLevel;
    st_BatLevel.u16_BatteryLevel    = p_Endian_hos2net16( pst_BatLevel->u16_BatteryLevel );

    RetVal = p_hanIeList_AddIeSimple(   pst_IeList,
                                        CMND_IE_BATTERY_LEVEL,
                                        (u8*)&st_BatLevel,
                                        sizeof(t_st_hanCmndIeBatteryLevel) );

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_BATTERY_MEASURE_INFO)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_BATTERY_MEASURE_INFO)* pst_Ie = (t_st(CMND_IE_BATTERY_MEASURE_INFO)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );

    }
    return CMND_IE_BATTERY_MEASURE_INFO;
}

bool p_hanCmndApi_IeU8Add( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU8* pst_U8Gen )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_U8,
                                    (u8*)pst_U8Gen,
                                    sizeof(t_st_hanCmndIeU8) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_U8)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_U8)* pst_Ie = (t_st(CMND_IE_U8)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u8_Data = p_hanStreamBuffer_GetData8(pst_Stream);
    }
    return CMND_IE_U8;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeU16Add( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU16* pst_U16Gen )
{
    t_st_hanCmndIeU16 st_U16Gen = {0};

    st_U16Gen.u16_Data = p_Endian_hos2net16(pst_U16Gen->u16_Data);
    return  p_hanIeList_AddIeSimple(    pst_IeList,
                                        CMND_IE_U16,
                                        (u8*)&st_U16Gen,
                                        sizeof(t_st_hanCmndIeU16) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_U16)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_U16)* pst_Ie = (t_st(CMND_IE_U16)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u16_Data = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
    }
    return CMND_IE_U16;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeU32Add( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU32* pst_U32Gen )
{
    t_st_hanCmndIeU32 st_U32Gen = {0};

    st_U32Gen.u32_Data = p_Endian_hos2net32(pst_U32Gen->u32_Data);
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_U32,
                                    (u8*)&st_U32Gen,
                                    sizeof(t_st_hanCmndIeU32) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_U32)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_U32)* pst_Ie = (t_st(CMND_IE_U32)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u32_Data = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(pst_Stream));
    }
    return CMND_IE_U32;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeBandGapAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBandGap* pst_BandGap )
{
    t_st_hanCmndIeBandGap st_ForAdd = {0};

    memcpy( &st_ForAdd, pst_BandGap, sizeof( t_st_hanCmndIeBandGap ) );
    st_ForAdd.u16_SupplyVolt = p_Endian_hos2net16( st_ForAdd.u16_SupplyVolt );

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_BG_REQ,
                                    (u8*)&st_ForAdd,
                                    sizeof( t_st_hanCmndIeBandGap ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_BG_REQ)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_BG_REQ)* pst_Ie = (t_st(CMND_IE_BG_REQ)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );
        pst_Ie->u16_SupplyVolt = p_Endian_net2hos16(pst_Ie->u16_SupplyVolt);
    }
    return CMND_IE_BG_REQ;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_BG_RES)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_BG_RES)* pst_Ie = (t_st(CMND_IE_BG_RES)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u32_AdcInput = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(pst_Stream));
        pst_Ie->u32_PorInput = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(pst_Stream));
    }
    return CMND_IE_BG_RES;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeBandGapResAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeBandGapRes* pst_BandGapRes )
{
    t_st_hanCmndIeBandGapRes    st_BandGapRes;

    // converting  from host to network because all IEs payloads are assumed to be in network order
    // (meaning IE always come from MCU in network order, but here are the ones who generated the IE,
    // so a conversion is required)

    st_BandGapRes.u32_AdcInput = p_Endian_hos2net32(pst_BandGapRes->u32_AdcInput);
    st_BandGapRes.u32_PorInput = p_Endian_hos2net32(pst_BandGapRes->u32_PorInput);

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_BG_RES,
                                    (u8*) &st_BandGapRes,
                                    sizeof(t_st_hanCmndIeBandGapRes) );

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteContReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteContReq* pst_AteContReq )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATE_CONT_REQ,
                                    (u8*) pst_AteContReq,
                                    sizeof(t_st_hanCmndIeAteContReq) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteContReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteContReq* pst_AteContReq )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATE_CONT_REQ, &st_Ie ) )
    {
        return false;
    }

    if ( st_Ie.u16_Len != sizeof(t_st_hanCmndIeAteContReq) )
    {
        return false;
    }

    // copy field from IE to t_st_hanCmndIeBandGap as is
    memcpy( pst_AteContReq, st_Ie.pu8_Data, st_Ie.u16_Len );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteRxReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteRxReq* pst_AteRxReq )
{

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATE_RX_REQ,
                                    (u8*) pst_AteRxReq,
                                    sizeof( t_st_hanCmndIeAteRxReq ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteRxReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteRxReq* pst_AteRxReq )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATE_RX_REQ, &st_Ie ) )
    {
        return false;
    }

    if ( st_Ie.u16_Len != sizeof(t_st_hanCmndIeAteRxReq) )
    {
        return false;
    }

    // copy field from IE to t_st_hanCmndIeBandGap as is
    memcpy( pst_AteRxReq, st_Ie.pu8_Data, st_Ie.u16_Len );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteRxResGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteRxRes* pst_AteRxRes )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATE_RX_RES, &st_Ie ) )
    {
        return false;
    }

    if ( st_Ie.u16_Len != sizeof( t_st_hanCmndIeAteRxRes ) )
    {
        return false;
    }

    // copy field from IE to t_st_hanCmndIeBandGap as is
    memcpy( pst_AteRxRes, st_Ie.pu8_Data, st_Ie.u16_Len );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteRxResAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteRxRes* pst_AteRxRes )
{
    t_st_hanCmndIeAteRxRes st_AteRxRes;

    st_AteRxRes.u16_Ber = p_Endian_hos2net16(pst_AteRxRes->u16_Ber);
    st_AteRxRes.u8_Fer  = pst_AteRxRes->u8_Fer;
    st_AteRxRes.u8_Rssi = pst_AteRxRes->u8_Rssi;

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATE_RX_RES,
                                    (u8*) &st_AteRxRes,
                                    sizeof(t_st_hanCmndIeAteRxRes) );

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteTxReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAteTxReq* pst_AteTxReq )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ATE_TX_REQ,
                                    (u8*) pst_AteTxReq,
                                    sizeof( t_st_hanCmndIeAteTxReq ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAteTxReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeAteTxReq* pst_AteTxReq )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_ATE_TX_REQ, &st_Ie ) )
    {
        return false;
    }

    if ( st_Ie.u16_Len != sizeof(t_st_hanCmndIeAteTxReq) )
    {
        return false;
    }

    // copy field from IE to t_st_hanCmndIeBandGap as is
    memcpy( pst_AteTxReq, st_Ie.pu8_Data, st_Ie.u16_Len );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeReportIdAdd(t_st_hanIeList* pst_IeList, u8 u8_ReportId)
{
    t_st_hanIeStruct st_Ie;

    st_Ie.u8_Type   = CMND_IE_REPORT_ID;
    st_Ie.u16_Len   = sizeof(u8);
    st_Ie.pu8_Data  = &u8_ReportId;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeReportIdGet( t_st_hanIeList* pst_IeList, u8* pu8_ReportId )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data

    //ASSERT(NULL != pu8_ReportId);

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_REPORT_ID, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    *pu8_ReportId = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;

    return !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeReportEntriesAdd(   t_st_hanIeList*                     pst_IeList,
                                        const t_st_hanCmndIeReportEntries*  pst_ReportEntries,
                                        u16                                 u16_Length )
{
    // The content of t_st_hanCmndIeAddReportEntries can be copied as is
    // no conversions or special copies required.
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_REPORT_ENTRIES,
                                    (u8*)pst_ReportEntries,
                                    u16_Length );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeReportInfoAdd(  t_st_hanIeList*                 pst_IeList,
                                    const t_st_hanCmndIeReportInfoInd*  pst_ReportInfo )
{
    t_st_StreamBuffer st_IeDataStream;
    u8 i, c;
    u8 u8_NumOfAttrib, u8_NumOfEntries;
    t_st_hanIeStruct st_Ie = {0};
    const t_st_hanCmndIeNtfReportEntry* pst_CurrentRepEntry;
    const t_st_hanCmndIeAttrCond*           pst_CurrentAttrCond;

    t_st_hanCmndIeNtfReportEntry    st_EmptyRepEntry = {0};
    t_st_hanCmndIeAttrCond          st_EmptyAttrCond = {0};

    u8 pu8_IeData[CMND_API_PAYLOAD_MAX_LENGTH];
    // use Stream Buffer to construct the IE payload/data
    p_hanStreamBuffer_CreateEmpty( &st_IeDataStream, pu8_IeData, CMND_API_PAYLOAD_MAX_LENGTH );

    p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_ReportInfo->u8_ReportId );
    p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_ReportInfo->u8_NumOfReportEntries );

    u8_NumOfEntries = pst_ReportInfo->u8_NumOfReportEntries;

    for( i=0; i< CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ENTRIES; i++)
    {
        pst_CurrentRepEntry = &pst_ReportInfo->st_NtfReportEntries[i];
        if( i< u8_NumOfEntries )
        {
            p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentRepEntry->u8_UnitId );
            p_hanStreamBuffer_AddData16( &st_IeDataStream, p_Endian_hos2net16( pst_CurrentRepEntry->u16_InterfaceId ) );

            u8_NumOfAttrib = pst_CurrentRepEntry->u8_NumOfAttrib;
            p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentRepEntry->u8_NumOfAttrib );
            for( c=0; c< CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ATTR; c++)
            {
                pst_CurrentAttrCond = &pst_CurrentRepEntry->st_ReportDataFields[c];
                if( c< u8_NumOfAttrib )
                {
                    p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttrCond->u8_AttributeId );
                    p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttrCond->u8_TypeOfReporting );
                    p_hanStreamBuffer_AddData8( &st_IeDataStream, pst_CurrentAttrCond->u8_AttributeSize );

                    if( pst_CurrentAttrCond->u8_AttributeSize <= 4 )
                    {
                        p_hanStreamBuffer_AddData32( &st_IeDataStream, p_Endian_hos2net32( pst_CurrentAttrCond->u32_AttributeValue ) );
                    }
                    else
                    {
                        // unsupported
                        return false;
                    }
                }
                else
                {
                    p_hanStreamBuffer_AddData8Array( &st_IeDataStream, (u8*)&st_EmptyAttrCond, sizeof( t_st_hanCmndIeAttrCond ));
                }
            }
        }
        else
        {
            p_hanStreamBuffer_AddData8Array( &st_IeDataStream, (u8*)&st_EmptyRepEntry, sizeof( t_st_hanCmndIeNtfReportEntry ));
        }
    }
    st_Ie.u8_Type   = CMND_IE_REPORT_INFO;
    st_Ie.u16_Len   = p_hanStreamBuffer_GetDataSize( &st_IeDataStream );
    st_Ie.pu8_Data  = pu8_IeData;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeAddReportInfoAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAddReport* pst_AddReport )
{
    t_st_StreamBuffer       st_IeDataStream;        // Stream for working with IE data
    t_st_StreamBuffer       st_ReportEntriesStream; // Stream is used for getting the report entries from pst_AddReport
    t_st_hanIeStruct        st_Ie;                  // The new constructed IE
    u8                      au8_IeData[CMND_API_PAYLOAD_MAX_LENGTH];        // Container buffer for IE data
    bool                    RetVal = true;
    u32                     i, j;
    u8                      u8_AttribPackType, u8_NumOfAttributes, u8_TypeOfReporting, u8_AttributeSize;

    // use Stream Buffer to construct the IE payload/data
    p_hanStreamBuffer_CreateEmpty( &st_IeDataStream, au8_IeData, CMND_API_PAYLOAD_MAX_LENGTH );

    p_hanStreamBuffer_CreateWithPayload( &st_ReportEntriesStream, (u8 *)pst_AddReport->u8_ReportEntries, CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE, CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE );

    p_hanStreamBuffer_AddData16 ( &st_IeDataStream, p_Endian_hos2net16(pst_AddReport->u16_ReceiverDevAddr) );
    p_hanStreamBuffer_AddData8  ( &st_IeDataStream, pst_AddReport->u8_ReceiverUnitId );
    p_hanStreamBuffer_AddData32 ( &st_IeDataStream, p_Endian_hos2net32(pst_AddReport->u32_PeriodicInterval) );
    p_hanStreamBuffer_AddData8  ( &st_IeDataStream, pst_AddReport->u8_ReportId );
    p_hanStreamBuffer_AddData8  ( &st_IeDataStream, pst_AddReport->u8_NumEntries );

    for ( i = 0; i < pst_AddReport->u8_NumEntries && RetVal ; i++ )
    {
        //Unit ID
        p_hanStreamBuffer_AddData8( &st_IeDataStream, p_hanStreamBuffer_GetData8(&st_ReportEntriesStream) );
        //Interface ID
        p_hanStreamBuffer_AddData16( &st_IeDataStream, p_Endian_hos2net16(p_hanStreamBuffer_GetData16(&st_ReportEntriesStream)) );
        //AttribPack
        u8_AttribPackType = p_hanStreamBuffer_GetData8(&st_ReportEntriesStream);
        p_hanStreamBuffer_AddData8( &st_IeDataStream, u8_AttribPackType );

        if( (HAN_ATTR_REP_REPORT_TYPE_EVENT & pst_AddReport->u8_ReportId )
            && u8_AttribPackType != FUN_ATTRIBUTE_PACK_TYPE_DYNAMIC )
        {
            //Type Of Reporting
            p_hanStreamBuffer_AddData8( &st_IeDataStream, p_hanStreamBuffer_GetData8(&st_ReportEntriesStream) );
        }

        if( u8_AttribPackType == FUN_ATTRIBUTE_PACK_TYPE_DYNAMIC)
        {
            //Num Of Attributes
            u8_NumOfAttributes =  p_hanStreamBuffer_GetData8(&st_ReportEntriesStream);
            p_hanStreamBuffer_AddData8( &st_IeDataStream, u8_NumOfAttributes );

            for ( j = 0; j < u8_NumOfAttributes && RetVal ; j++ )
            {
                // Attirbute ID
                p_hanStreamBuffer_AddData8( &st_IeDataStream, p_hanStreamBuffer_GetData8(&st_ReportEntriesStream) );
                if(HAN_ATTR_REP_REPORT_TYPE_EVENT & pst_AddReport->u8_ReportId )
                {
                    //Type Of Reporting
                    u8_TypeOfReporting =  p_hanStreamBuffer_GetData8(&st_ReportEntriesStream);
                    p_hanStreamBuffer_AddData8( &st_IeDataStream, u8_TypeOfReporting );

                    // Attribute Size
                    /* If COV this field is interpreted as percentage of change */
                    u8_AttributeSize = p_hanStreamBuffer_GetData8(&st_ReportEntriesStream);
                    p_hanStreamBuffer_AddData8( &st_IeDataStream, u8_AttributeSize );

                    if( u8_TypeOfReporting != FUN_IF_ATTR_REPORTING_TYPE_CHANGE_OF_VAL )
                    {
                        switch( u8_AttributeSize )
                        {
                        case 1:
                            p_hanStreamBuffer_AddData8( &st_IeDataStream, p_hanStreamBuffer_GetData8(&st_ReportEntriesStream) );
                            break;
                        case 2:
                            p_hanStreamBuffer_AddData16( &st_IeDataStream, p_Endian_hos2net16(p_hanStreamBuffer_GetData16(&st_ReportEntriesStream) ));
                            break;
                        case 4:
                            p_hanStreamBuffer_AddData32( &st_IeDataStream, p_Endian_hos2net32(p_hanStreamBuffer_GetData32(&st_ReportEntriesStream) ));
                            break;
                        default:
                            RetVal = false;
                        }
                    }
                }
            }
        }
    }

    if ( RetVal )
    {
        // if there was an overrun when constructing IE data then return failure
        if ( p_hanStreamBuffer_CheckOverrun( &st_IeDataStream ) )
        {
            RetVal = false;
        }
        else
        {
            // fill IE fields
            st_Ie.u8_Type   = CMND_IE_ADD_REPORT_INFO;
            st_Ie.u16_Len   = p_hanStreamBuffer_GetDataSize( &st_IeDataStream );
            st_Ie.pu8_Data  = au8_IeData;

            RetVal = p_hanIeList_AddIe( pst_IeList, &st_Ie );
        }
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeReportInfoGet(  t_st_hanIeList*                 pst_IeList,
                                    t_st_hanCmndIeReportInfoInd*    pst_ReportInfo )
{
    t_st_hanIeStruct    st_Ie;
    t_st_StreamBuffer   st_IeDataStream;        // Stream for working with IE data
    u8                  i = 0;
    u8                  j = 0;
    bool                RetVal = true;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_REPORT_INFO, &st_Ie ) )
    {
        return false;
    }

    // use Stream Buffer to parse IE payload
    p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                            st_Ie.pu8_Data,
                                            st_Ie.u16_Len,
                                            st_Ie.u16_Len );

    // CMND IE payload is network order, so conversion is required
    pst_ReportInfo->u8_ReportId = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;
    pst_ReportInfo->u8_NumOfReportEntries = p_hanStreamBuffer_GetData8( &st_IeDataStream ) ;
    if(pst_ReportInfo->u8_NumOfReportEntries > CHANCMNDAPI_ADDEVENT_REPORT_NUM_ENTRIES)
    {
        RetVal = false;
    }
    else
    {
        for (i = 0; i < pst_ReportInfo->u8_NumOfReportEntries; ++i)
        {
            /* Parse Report Entry */
            pst_ReportInfo->st_NtfReportEntries[i].u8_UnitId = p_hanStreamBuffer_GetData8(&st_IeDataStream);
            pst_ReportInfo->st_NtfReportEntries[i].u16_InterfaceId = p_hanStreamBuffer_GetData16(&st_IeDataStream);
            pst_ReportInfo->st_NtfReportEntries[i].u8_NumOfAttrib = p_hanStreamBuffer_GetData8(&st_IeDataStream);
            if(pst_ReportInfo->st_NtfReportEntries[i].u8_NumOfAttrib > CHANCMNDAPI_ADDEVENT_REPORT_NUM_ATTR)
            {
                RetVal = false;
                break;
            }

            for (j = 0; j < pst_ReportInfo->st_NtfReportEntries[i].u8_NumOfAttrib; ++j)
            {
                /* Parse Report Entry Data Field */
                pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u8_AttributeId = p_hanStreamBuffer_GetData8(&st_IeDataStream);
                pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u8_TypeOfReporting = p_hanStreamBuffer_GetData8(&st_IeDataStream);
                pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u8_AttributeSize = p_hanStreamBuffer_GetData8(&st_IeDataStream);

                pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u32_AttributeValue = 0;
                switch(pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u8_AttributeSize)
                {
                case 1:
                    pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u32_AttributeValue = p_hanStreamBuffer_GetData8(&st_IeDataStream);
                    break;
                case 2:
                    pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u32_AttributeValue = p_hanStreamBuffer_GetData16(&st_IeDataStream);
                    break;
                case 4:
                    pst_ReportInfo->st_NtfReportEntries[i].st_ReportDataFields[j].u32_AttributeValue = p_hanStreamBuffer_GetData32(&st_IeDataStream);
                    break;
                default:
                    RetVal = false;
                    break;
                }
            }
        }
    }

    if(true == RetVal)
    {
        RetVal = !p_hanStreamBuffer_CheckUnderrun( &st_IeDataStream );
    }
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IePmidAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIePMID* pst_Pmid )
{
    return  p_hanIeList_AddIeSimple(    pst_IeList,
                                        CMND_IE_PMID,
                                        (u8*)pst_Pmid,
                                        sizeof(t_st_hanCmndIePMID) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_PMID)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_PMID)* pst_Ie = (t_st(CMND_IE_PMID)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, sizeof(*pst_Ie) );
    }
    return CMND_IE_PMID;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IePortableIdentityAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIePortableIdentity* pst_PI )
{
    return p_hanIeList_AddIeSimple( pst_IeList,
                                        CMND_IE_PORTABLE_IDENTITY,
                                        (u8*)pst_PI,
                                        sizeof(t_st_hanCmndIePortableIdentity) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IePortableIdentityGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIePortableIdentity* pst_PI )
{
    bool                    RetVal = true;
    t_st_hanIeStruct        st_Ie;
    t_st_StreamBuffer       st_IeDataStream;    // Stream for working with IE data

    RetVal = p_hanIeList_FindIeByType(pst_IeList, CMND_IE_PORTABLE_IDENTITY, &st_Ie);

    if(true == RetVal)
    {
        // use Stream Buffer to parse IE payload
        p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                                st_Ie.pu8_Data,
                                                st_Ie.u16_Len,
                                                st_Ie.u16_Len );

        pst_PI->u8_UseAltPIFlag = p_hanStreamBuffer_GetData8(&st_IeDataStream);
        pst_PI->u8_PI1 = p_hanStreamBuffer_GetData8(&st_IeDataStream);
        pst_PI->u8_PI2 = p_hanStreamBuffer_GetData8(&st_IeDataStream);
        pst_PI->u8_PI3 = p_hanStreamBuffer_GetData8(&st_IeDataStream);
        pst_PI->u8_PI4 = p_hanStreamBuffer_GetData8(&st_IeDataStream);
        pst_PI->u8_PI5 = p_hanStreamBuffer_GetData8(&st_IeDataStream);
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaNewSwReadyGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeSuotaNewSwReady* pst_SuotaSwReady )
{
    t_st_hanIeStruct    st_Ie;
    u8                  u8_dataIndx=0;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_NEW_SW_INFO, &st_Ie ) )
    {
        return false;
    }

    pst_SuotaSwReady->u8_swVerLen = st_Ie.pu8_Data[u8_dataIndx];
    u8_dataIndx+=sizeof(pst_SuotaSwReady->u8_swVerLen);
    memcpy( (void *)pst_SuotaSwReady->swStr,&(st_Ie.pu8_Data[u8_dataIndx]),pst_SuotaSwReady->u8_swVerLen );
    u8_dataIndx+=pst_SuotaSwReady->u8_swVerLen;

    pst_SuotaSwReady->u8_hwVerLen = st_Ie.pu8_Data[u8_dataIndx];
    u8_dataIndx+=sizeof(pst_SuotaSwReady->u8_hwVerLen);
    memcpy( (void *)pst_SuotaSwReady->hwStr,&(st_Ie.pu8_Data[u8_dataIndx]),pst_SuotaSwReady->u8_hwVerLen);
    u8_dataIndx+=pst_SuotaSwReady->u8_hwVerLen;

    pst_SuotaSwReady->u8_urlStrLen=st_Ie.pu8_Data[u8_dataIndx];
    u8_dataIndx+=sizeof(pst_SuotaSwReady->u8_urlStrLen);
    memcpy( (void *)pst_SuotaSwReady->urlStr,&(st_Ie.pu8_Data[u8_dataIndx]),pst_SuotaSwReady->u8_urlStrLen);
    u8_dataIndx+=pst_SuotaSwReady->u8_urlStrLen;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaNewSwReadyAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSuotaNewSwReady* pst_SuotaSwReady )
{
    u16 datalength=0;
    u8  newSwInfoBuf[CMND_API_PAYLOAD_MAX_LENGTH];

    newSwInfoBuf[datalength] = pst_SuotaSwReady->u8_swVerLen;
    datalength+=sizeof(pst_SuotaSwReady->u8_swVerLen);
    memcpy( (void *)&(newSwInfoBuf[datalength]),(const void *)pst_SuotaSwReady->swStr,pst_SuotaSwReady->u8_swVerLen  );
    datalength+=pst_SuotaSwReady->u8_swVerLen;

    newSwInfoBuf[datalength] = pst_SuotaSwReady->u8_hwVerLen;
    datalength+=sizeof(pst_SuotaSwReady->u8_hwVerLen);
    memcpy( (void *)&(newSwInfoBuf[datalength]),(const void *)pst_SuotaSwReady->hwStr,pst_SuotaSwReady->u8_hwVerLen);
    datalength+=pst_SuotaSwReady->u8_hwVerLen;

    newSwInfoBuf[datalength] = pst_SuotaSwReady->u8_urlStrLen;
    datalength+=sizeof(pst_SuotaSwReady->u8_urlStrLen);
    memcpy( (void *)&(newSwInfoBuf[datalength]),(const void *)pst_SuotaSwReady->urlStr,pst_SuotaSwReady->u8_urlStrLen);
    datalength+=pst_SuotaSwReady->u8_urlStrLen;

    p_hanIeList_AddIeSimple(    pst_IeList,
                                CMND_IE_NEW_SW_INFO,
                                (u8*) newSwInfoBuf,
                                datalength );
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaCurrentSwAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSuotaCurrentSw* pst_SuotaCurrentSw )
{
    // TODO: fix length usage
    return p_hanIeList_AddIeSimple(     pst_IeList,
                                        CMND_IE_CURRENT_SW_INFO,
                                        (u8*) pst_SuotaCurrentSw,
                                        sizeof(t_st_hanCmndIeSuotaCurrentSw) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaSwVerAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeSuotaSwVer* pst_SuotaSwVer )
{
    return p_hanIeList_AddIeSimple(     pst_IeList,
                                        CMND_IE_SW_VER_INFO,
                                        (u8*) pst_SuotaSwVer,
                                        CMND_IE_SUOTA_SW_VER_HEADER_SIZE + pst_SuotaSwVer->u8_swVerLen );

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeGpioStateGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeGpioState* pst_GpioState )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    if ( !p_hanIeList_FindIeByType( pst_IeList, CMND_IE_GPIO_STATE, &st_Ie ) )
    {
        return false;
    }
    memcpy( pst_GpioState, st_Ie.pu8_Data, st_Ie.u16_Len );

    return true;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaImageTypeAdd ( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeU8* pst_ImageType )
{
    return p_hanIeList_AddIeSimple(     pst_IeList,
                                        CMND_IE_IMAGE_TYPE,
                                        (u8*)pst_ImageType,
                                        sizeof(t_st_hanCmndIeU8) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaImageTypeGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeU8* pst_U8Gen )
{
    bool                    RetVal = true;
    t_st_hanIeStruct        st_Ie;
    t_st_StreamBuffer       st_IeDataStream;    // Stream for working with IE data

    RetVal = p_hanIeList_FindIeByType(pst_IeList, CMND_IE_IMAGE_TYPE, &st_Ie);

    if(true == RetVal)
    {
        // use Stream Buffer to parse IE payload
        p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                                st_Ie.pu8_Data,
                                                st_Ie.u16_Len,
                                                st_Ie.u16_Len );

        pst_U8Gen->u8_Data = p_hanStreamBuffer_GetData8(&st_IeDataStream);
    }
    else
    {
        // set dummy value in case return value is not checked by callee
        pst_U8Gen->u8_Data = 0xFF;
        //LOG_ERROR_PRINT( "%s: ERROR Get Image Type failed\n", __func__);
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaReadFileAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFileDataRes* pst_SuotaFileData )
{
    u16 length;
    t_st_hanCmndIeFileDataRes st_SuotaFileData;

    st_SuotaFileData.u32_Offset = p_Endian_hos2net32(pst_SuotaFileData->u32_Offset);
    st_SuotaFileData.u16_Length = p_Endian_hos2net16(pst_SuotaFileData->u16_Length);
    memcpy( &st_SuotaFileData.u8_Data[0], &pst_SuotaFileData->u8_Data[0], pst_SuotaFileData->u16_Length );

    /* length of data + length field + offset field */
    length = pst_SuotaFileData->u16_Length + sizeof(pst_SuotaFileData->u16_Length) + sizeof(pst_SuotaFileData->u32_Offset);

    return p_hanIeList_AddIeSimple( pst_IeList, CMND_IE_READ_FILE_DATA_RES, (u8*) &st_SuotaFileData, length );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaReadFileReqAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFileDataReq* pst_SuotaFileData )
{
    t_st_hanCmndIeFileDataReq st_SuotaFileData;

    LOG_DEBUG(  "CMND_IE_READ_FILE_DATA_REQ: u16_Length=%d, u32_Offset=%lu",
                pst_SuotaFileData->u16_Length,
                pst_SuotaFileData->u32_Offset );

    st_SuotaFileData.u16_Length = p_Endian_hos2net16(pst_SuotaFileData->u16_Length);
    st_SuotaFileData.u32_Offset = p_Endian_hos2net32(pst_SuotaFileData->u32_Offset);

    return p_hanIeList_AddIeSimple( pst_IeList, CMND_IE_READ_FILE_DATA_REQ, (u8*) &st_SuotaFileData, sizeof(st_SuotaFileData) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaReadFileReqGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeFileDataReq* pst_SuotaFileDataReq )
{
    bool                    RetVal = false;
    t_st_hanIeStruct        st_Ie;
    t_st_StreamBuffer       st_IeDataStream;

    RetVal = p_hanIeList_FindIeByType(pst_IeList, CMND_IE_READ_FILE_DATA_REQ, &st_Ie);
    if(true == RetVal)
    {
        // use Stream Buffer to parse IE payload
        p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream, st_Ie.pu8_Data,st_Ie.u16_Len,st_Ie.u16_Len );

        p_hanSreamBuffer_GetData8Array(&st_IeDataStream,(u8 *)pst_SuotaFileDataReq,sizeof(t_st_hanCmndIeFileDataReq));
        pst_SuotaFileDataReq->u16_Length = p_Endian_net2hos16(pst_SuotaFileDataReq->u16_Length);
        pst_SuotaFileDataReq->u32_Offset = p_Endian_net2hos32(pst_SuotaFileDataReq->u32_Offset);
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeSuotaReadFileResGet( IN t_st_hanIeList* pst_IeList, OUT t_st_hanCmndIeFileDataRes* pst_SuotaFileDataRes )
{
    bool                    RetVal = false;
    t_st_hanIeStruct        st_Ie;
    t_st_StreamBuffer       st_IeDataStream;

    RetVal = p_hanIeList_FindIeByType(pst_IeList, CMND_IE_READ_FILE_DATA_RES, OUT &st_Ie);
    if(true == RetVal)
    {
        // use Stream Buffer to parse IE payload
        p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream, st_Ie.pu8_Data, st_Ie.u16_Len, st_Ie.u16_Len );
        pst_SuotaFileDataRes->u32_Offset = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(&st_IeDataStream));
        pst_SuotaFileDataRes->u16_Length = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(&st_IeDataStream));
        p_hanSreamBuffer_GetData8Array(&st_IeDataStream,(u8 *)pst_SuotaFileDataRes->u8_Data, pst_SuotaFileDataRes->u16_Length);
    }
    else
    {
        //LOG_ERROR_PRINT( "%s: ERROR Get File Req \n", __func__);
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_TAMPER_ALERT)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_TAMPER_ALERT)* pst_Ie = (t_st(CMND_IE_TAMPER_ALERT)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u8_AlertStatus = p_hanStreamBuffer_GetData8(pst_Stream);
    }
    return CMND_IE_TAMPER_ALERT;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeTamperAlertStatusGet( t_st_hanIeList* pst_IeList, t_st_hanCmndIeTamperAlert* pst_TamperAlert )
{
    bool                    RetVal = true;
    t_st_hanIeStruct        st_Ie;
    t_st_StreamBuffer       st_IeDataStream;    // Stream for working with IE data

    RetVal = p_hanIeList_FindIeByType(pst_IeList, CMND_IE_TAMPER_ALERT, &st_Ie);

    if(true == RetVal)
    {
        // use Stream Buffer to parse IE payload
        p_hanStreamBuffer_CreateWithPayload(    &st_IeDataStream,
                                                st_Ie.pu8_Data,
                                                st_Ie.u16_Len,
                                                st_Ie.u16_Len );

        pst_TamperAlert->u8_AlertStatus = p_hanStreamBuffer_GetData8(&st_IeDataStream);
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeFunAdd(t_st_hanIeList* pst_IeList, const t_st_hanCmndIeFun* pst_Fun)
{
    t_st_hanIeStruct    st_Ie;
    t_st_hanCmndIeFun   st_Fun = {0};

    memcpy( &st_Fun, pst_Fun, sizeof(t_st_hanCmndIeFun));

    st_Fun.u16_SrcDeviceId  = p_Endian_hos2net16( pst_Fun->u16_SrcDeviceId );
    st_Fun.u16_DstDeviceId  = p_Endian_hos2net16( pst_Fun->u16_DstDeviceId );
    st_Fun.u16_InterfaceId  = p_Endian_hos2net16( pst_Fun->u16_InterfaceId );
    st_Fun.u16_DataLen      = p_Endian_hos2net16( pst_Fun->u16_DataLen );

    st_Ie.u8_Type   = CMND_IE_FUN;
    st_Ie.u16_Len   = MIN( CMND_IE_FUN_HEADER_SIZE + pst_Fun->u16_DataLen, sizeof(t_st_hanCmndIeFun) );
    st_Ie.pu8_Data  = (u8*)&st_Fun;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_FUN)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_FUN)* pst_Ie = (t_st(CMND_IE_FUN)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, pst_Stream->u16_DataSize );
        pst_Ie->u16_SrcDeviceId = p_Endian_net2hos16( pst_Ie->u16_SrcDeviceId );
        pst_Ie->u16_DstDeviceId = p_Endian_net2hos16( pst_Ie->u16_DstDeviceId  );
        pst_Ie->u16_InterfaceId = p_Endian_net2hos16( pst_Ie->u16_InterfaceId );
        pst_Ie->u16_DataLen = p_Endian_net2hos16( pst_Ie->u16_DataLen  );
    }
    return CMND_IE_FUN;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_DEREGISTRATION_RESPONSE)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_DEREGISTRATION_RESPONSE)* pst_Ie = (t_st(CMND_IE_DEREGISTRATION_RESPONSE)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u8_ResponseCode = p_hanStreamBuffer_GetData8(pst_Stream);
    }
    return CMND_IE_DEREGISTRATION_RESPONSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


bool p_hanCmndApi_IeAlertAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeAlert* pst_IeAlarm )
{
    t_st_hanCmndIeAlert             st_IeAlarm;

    st_IeAlarm.u16_UnitType = p_Endian_hos2net16(pst_IeAlarm->u16_UnitType);
    st_IeAlarm.u32_AlertState = p_Endian_hos2net32(pst_IeAlarm->u32_AlertState);

    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_ALERT,
                                    (u8*)&st_IeAlarm,
                                    sizeof(t_st_hanCmndIeAlert) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_ALERT)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_ALERT)* pst_Ie = (t_st(CMND_IE_ALERT)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u16_UnitType = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
        pst_Ie->u32_AlertState = p_Endian_net2hos32(p_hanStreamBuffer_GetData32(pst_Stream));
    }
    return CMND_IE_ALERT;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeResposeRequiredAdd( t_st_hanIeList*     pst_IeList,
                                        bool                b_ResponseRequired )
{
    u8 u8_ResponseRequired = 0;
    if ( b_ResponseRequired )
    {
        u8_ResponseRequired = 1;
    }
    return p_hanIeList_AddIeSimple( pst_IeList, CMND_IE_RESPONSE_REQUIRED, &u8_ResponseRequired, sizeof(u8_ResponseRequired) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeResposeRequiredGet( IN  t_st_hanIeList*     pst_IeList,
                                        OUT bool*               pb_ResponseRequired )
{
    t_st_hanIeStruct    st_Ie;

    // find the IE first
    *pb_ResponseRequired = p_hanIeList_FindIeByType( pst_IeList, CMND_IE_RESPONSE_REQUIRED, &st_Ie );

    return *pb_ResponseRequired;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeLinkMaintainAdd( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeLinkMaintain* pst_IeLinkMaintain )
{
    t_st_hanCmndIeLinkMaintain st_LinkMaintainNet;
    st_LinkMaintainNet.u16_LinkMaintainTime = p_Endian_hos2net16( pst_IeLinkMaintain->u16_LinkMaintainTime );
    st_LinkMaintainNet.u16_PingInterval = p_Endian_hos2net16( pst_IeLinkMaintain->u16_PingInterval );
    return p_hanIeList_AddIeSimple( pst_IeList,
                                    CMND_IE_LINK_MAINTAIN,
                                    (u8*)&st_LinkMaintainNet,
                                    sizeof(t_st_hanCmndIeLinkMaintain) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_LINK_MAINTAIN)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_LINK_MAINTAIN)* pst_Ie = (t_st(CMND_IE_LINK_MAINTAIN)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        pst_Ie->u16_LinkMaintainTime = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
        pst_Ie->u16_PingInterval = p_Endian_net2hos16(p_hanStreamBuffer_GetData16(pst_Stream));
    }
    return CMND_IE_LINK_MAINTAIN;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_hanCmndInfoElemType p_CMND_IE_GETTER(CMND_IE_ULE_CALL_SETTING)
(t_st_StreamBuffer* pst_Stream, void* pv_Ie )
{
    t_st(CMND_IE_ULE_CALL_SETTING)* pst_Ie = (t_st(CMND_IE_ULE_CALL_SETTING)*)pv_Ie;
    if( pst_Stream && pst_Ie )
    {
        // CMND IE payload is network order, so conversion is required
        p_hanSreamBuffer_GetData8Array( pst_Stream, (u8*)pst_Ie, pst_Stream->u16_DataSize );
        pst_Ie->u32_FieldMask = p_Endian_net2hos32( pst_Ie->u32_FieldMask);
    }
    return CMND_IE_ULE_CALL_SETTING;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_IeCallSettingsAdd ( t_st_hanIeList* pst_IeList, const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings )
{
    t_st_hanCmndIeUleCallSetting st_CallSettingsNet;
    memcpy( &st_CallSettingsNet, pst_IeCallSettings, sizeof ( t_st_hanCmndIeUleCallSetting ) );
    st_CallSettingsNet.u32_FieldMask = p_Endian_hos2net32( pst_IeCallSettings->u32_FieldMask );

    return p_hanIeList_AddIeSimple ( pst_IeList,
                                     CMND_IE_ULE_CALL_SETTING,
                                     (u8*) &st_CallSettingsNet,
                                     sizeof( t_st_hanCmndIeUleCallSetting ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
