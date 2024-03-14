/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "IeList.h"
#include "CmndApiExported.h"
#include "Endian.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static bool p_GetNextIe( t_st_hanIeList* pst_IeList, t_st_hanIeStruct* pst_Ie );

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// function uses Stream buffer to traverse over raw buffer and detect begining of each IE
void p_hanIeList_CreateWithPayload( IN const void* pv_IePayload,
                                    u16 u16_IePayloadSize,
                                    OUT t_st_hanIeList* pst_IeList )
{
    p_hanStreamBuffer_CreateWithPayload(    &pst_IeList->st_Buffer,
                                            (void*) pv_IePayload,
                                            u16_IePayloadSize,
                                            u16_IePayloadSize );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// function uses Stream buffer to traverse over raw buffer and detect begining of each IE
// Pay attention to the usage of funtions that call p_hanIeList_CreateWithPayloadAppendable - when
// more than one Ie List is created with the same writable payload.
// Functions that use p_hanIeList_CreateWithPayloadAppendable must be called after ones that
// use external Ie List.
void p_hanIeList_CreateWithPayloadAppendable(   IN void*            pv_IePayload,
                                                u16                 u16_IePayloadSize,
                                                u16                 u16_IeBufferSize,
                                                OUT t_st_hanIeList* pst_IeList )
{
    p_hanStreamBuffer_CreateWithPayload(    &pst_IeList->st_Buffer,
                                            pv_IePayload,
                                            u16_IeBufferSize,
                                            u16_IePayloadSize );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void p_hanIeList_CreateEmpty( void* pv_Buffer,  u16 u16_BufferSize, t_st_hanIeList* pst_IeList )
{
    p_hanStreamBuffer_CreateEmpty(  &pst_IeList->st_Buffer,
                                    pv_Buffer,
                                    u16_BufferSize );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void p_hanIeList_ResetToFirstIe( IN t_st_hanIeList* pst_IeList )
{
    p_hanStreamBuffer_Reset( &pst_IeList->st_Buffer );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool p_hanIeList_GetFirstIe( t_st_hanIeList* pst_IeList, t_st_hanIeStruct* pst_Ie )
{
    p_hanStreamBuffer_Reset( &pst_IeList->st_Buffer );

    return p_GetNextIe( pst_IeList, pst_Ie );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool p_hanIeList_GetNextIe( t_st_hanIeList* pst_IeList, t_st_hanIeStruct* pst_Ie )
{
    return p_GetNextIe( pst_IeList, pst_Ie );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool p_hanIeList_AddIe( t_st_hanIeList* pst_IeList, const t_st_hanIeStruct* pst_Ie )
{
    // type
    if ( !p_hanStreamBuffer_AddData8( &pst_IeList->st_Buffer, pst_Ie->u8_Type ) )
    {
        return false;
    }

    // length
    if ( !p_hanStreamBuffer_AddData16(  &pst_IeList->st_Buffer,
                                        p_Endian_hos2net16(pst_Ie->u16_Len)) )
    {
        return false;
    }

    // data
    if (    pst_Ie->u16_Len != 0 &&
            (!p_hanStreamBuffer_AddData8Array(  &pst_IeList->st_Buffer,
                                                pst_Ie->pu8_Data,
                                                pst_Ie->u16_Len )) )
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool p_hanIeList_AddIeSimple(   t_st_hanIeList*     pst_IeList,
                                u8                  u8_IeType,
                                u8*                 pu8_Data,
                                u16                 u16_DataLen )
{
    t_st_hanIeStruct st_Ie;

    st_Ie.u8_Type   = u8_IeType;
    st_Ie.u16_Len   = u16_DataLen;
    st_Ie.pu8_Data  = pu8_Data;

    return p_hanIeList_AddIe( pst_IeList, &st_Ie );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_hanIeList_GetListSize( const t_st_hanIeList* pst_IeList )
{
    return p_hanStreamBuffer_GetDataSize( &pst_IeList->st_Buffer );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool p_hanIeList_FindIeByType( IN t_st_hanIeList* pst_IeList, u8 u8_IeType, OUT t_st_hanIeStruct* pst_Ie )
{
    bool Ret = false;

    // clear the cursors to search from the start
    p_hanStreamBuffer_Reset( &pst_IeList->st_Buffer );

    Ret = p_hanIeList_GetFirstIe( pst_IeList, pst_Ie );
    while ( Ret )
    {
        // return when request IE type is found
        if ( pst_Ie->u8_Type == u8_IeType )
        {
            Ret = true;
            break;
        }

        Ret = p_hanIeList_GetNextIe( pst_IeList, pst_Ie );
    }

    // must reset the steam to defaults , otherwise next search will continue from
    // current location and not from the begining of the stream
    p_hanStreamBuffer_Reset( &pst_IeList->st_Buffer );

    return Ret;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

u8* p_hanIeList_GetDataPtr( const t_st_hanIeList* pst_IeList )
{
    if ( !pst_IeList )
    {
        return NULL;
    }
    return pst_IeList->st_Buffer.pu8_Data;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

u16 p_hanIeList_GetDataSize( const t_st_hanIeList* pst_IeList )
{
    if ( !pst_IeList )
    {
        return 0;
    }
    return pst_IeList->st_Buffer.u16_DataSize;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static bool p_GetNextIe( t_st_hanIeList* pst_IeList, t_st_hanIeStruct* pst_Ie )
{
    bool RetVal = true;

    // return if there's no header in the stream buffer
    if ( p_hanStreamBuffer_GetDataSize( &pst_IeList->st_Buffer ) < CMND_IE_HEADER_SIZE )
    {
        return false;
    }

    pst_Ie->u8_Type = p_hanStreamBuffer_GetData8( &pst_IeList->st_Buffer );
    pst_Ie->u16_Len = p_Endian_net2hos16( p_hanStreamBuffer_GetData16( &pst_IeList->st_Buffer ) );
    pst_Ie->pu8_Data = NULL;

    // when there's not data, the length will be zero
    // otherwise get the pointer to the data
    if ( pst_Ie->u16_Len )
    {
        pst_Ie->pu8_Data = p_hanStreamBuffer_GetPointer( &pst_IeList->st_Buffer );
    }

    // skip IE data and point to next IE.
    // if there's not enough data in the buffer then return false
    if ( pst_Ie->u16_Len <= p_hanStreamBuffer_GetDataSize( &pst_IeList->st_Buffer ) )
    {
        p_hanSreamBuffer_SkipData8Array( &pst_IeList->st_Buffer, pst_Ie->u16_Len );
    }
    else
    {
        RetVal = false;
    }

    // return true if this IE data is fully stored in the stream buffer
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
