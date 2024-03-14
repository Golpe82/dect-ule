/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndApiPacket.h"
#include "Endian.h"
#include "CmndApiIe.h"
#include "CmndApiHost.h"
#include "Logger.h"

#include <string.h> //memcpy

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/// A sync word of CMND API message
static const u16 syncWord = 0xDADA;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Set Length field in CMND API message buffer
static void p_CmndApiPacket_SetLength( INOUT u8 *pu8_Buffer, u16 u16_len );

// Update Checksum field
static void p_CmndApiPacket_UpdateCheckSumField( INOUT u8 *pu8_Buffer, u16 u16_len );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create a CMND API message
u16 p_CmndApiPacket_CreateFromCmndApiMsg(   OUT     u8*                 pu8_Buffer,
                                            const   t_st_hanCmndApiMsg* p_Msg )
{
    u16 len;
    if( p_Msg->dataLength > CMNDLIB_DATA_PAYLOAD_MAX_LENGTH )
    {
        return 0;
    }

    len = p_CmndApiPacket_CreateNoIe(   pu8_Buffer,
                                            p_Msg->serviceId,
                                            p_Msg->messageId,
                                            p_Msg->unitId,
                                            p_Msg->cookie);

    if ( p_Msg->dataLength > 0 )
    {
        t_st_hanIeList st_IeList;
        p_hanIeList_CreateWithPayload(  p_Msg->data,
                                        p_Msg->dataLength,
                                        &st_IeList );

        len = p_CmndApiPacket_AppendIeList( pu8_Buffer,
                                            len,
                                            &st_IeList );
    }
    return len;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_CmndApiPacket_CreateNoIe( OUT u8* pu8_Buffer,
                                    u16 u16_ServiceId,
                                    u8  u8_MessageId,
                                    u8  u8_UnitId,
                                    u8  u8_Cookie )
{
    u16 pos = 0;

    // Set start code (sync)
    memcpy( pu8_Buffer, &syncWord, sizeof(syncWord) );
    pos += sizeof(syncWord);

    // Set Length
    p_CmndApiPacket_SetLength( pu8_Buffer, CMND_API_PROTOCOL_SIZE_WITHOUT_DATA );
    pos += sizeof(pos);

    // Set Cookie
    pu8_Buffer[pos] = u8_Cookie;
    pos++;

    // Set Unit ID
    pu8_Buffer[pos] = u8_UnitId;
    pos++;

    // Set Service ID
    {
        u16 u16_TempServiceId = p_Endian_hos2net16( u16_ServiceId );
        memcpy( &pu8_Buffer[pos], &u16_TempServiceId, sizeof(u16_TempServiceId) );
        pos += sizeof(u16_TempServiceId);
    }

    // Set Message ID
    pu8_Buffer[pos] = u8_MessageId;
    pos++;

    // Set Checksum
    pu8_Buffer[pos] = 0; // Checksum will be calculated later
    pos++;

    // Update checksum field
    p_CmndApiPacket_UpdateCheckSumField( pu8_Buffer, pos );

    return pos;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create a CMND API message
u16 p_CmndApiPacket_Create( OUT u8* pu8_Buffer,
                                u16 u16_ServiceId,
                                u8 u8_MessageId,
                                u8 u8_UnitId,
                                u8 u8_Cookie,
                                const t_st_hanIeList* pst_IeList )
{
    u16 pos = 0;

    pos = p_CmndApiPacket_CreateNoIe( pu8_Buffer, u16_ServiceId, u8_MessageId, u8_UnitId, u8_Cookie );

    if ( pst_IeList )
    {
        // Copy buffer of IEs data
        pos = p_CmndApiPacket_AppendIeList( pu8_Buffer, pos, pst_IeList );
    }

    return pos;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Calculate Error checking of the received message.
// CS = 8 LSBs of byte summation from Length to Data (including Length, not including Checksum field)
u8 p_CmndApiPacket_CalcCheckSum( const u8 *pu8_Buffer, u16 u16_len )
{
    u16 i;
    u8 u8_sum = 0;

    for( i=0; i<u16_len; i++ )
    {
        u8_sum += pu8_Buffer[i];
    }

    return u8_sum;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_CmndApiPacket_AppendIeList(   INOUT   u8*                     pu8_Buffer,
                                            u16                     u16_len,
                                            const t_st_hanIeList*   pst_IeList )
{
    u16 pos             = 0;
    u8* pu8_Data        = NULL;
    u16 u16_DataSize    = 0;

    if (    !pu8_Buffer
         || !pst_IeList )
    {
        return 0;
    }

    u16_DataSize = p_hanIeList_GetDataSize( pst_IeList );

    if ( u16_len + u16_DataSize > CMNDLIB_API_PACKET_MAX_SIZE )
    {
        return 0;
    }

    pu8_Data = p_hanIeList_GetDataPtr ( pst_IeList );

    pos = u16_len;

    // Copy IeList data to message buffer
    memcpy( &pu8_Buffer[pos], pu8_Data, u16_DataSize );
    pos += u16_DataSize;

    // Set Length
    p_CmndApiPacket_SetLength( pu8_Buffer, pos - CMND_API_PROTOCOL_SIZE_HEADER );

    // Update checksum field
    p_CmndApiPacket_UpdateCheckSumField( pu8_Buffer, pos );

    return pos;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Set Length field in CMND API message buffer
static void p_CmndApiPacket_SetLength( INOUT u8 *pu8_Buffer, u16 u16_len )
{
    u16 u16_Length = p_Endian_hos2net16(u16_len);

    // Copy Length
    memcpy( &pu8_Buffer[sizeof(syncWord)], &u16_Length, sizeof(u16_Length) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Update Checksum field
static void p_CmndApiPacket_UpdateCheckSumField( INOUT u8 *pu8_Buffer, u16 u16_len )
{
    u8 checksum = 0;
    pu8_Buffer[CMND_API_PROTOCOL_CHECKSUM_POS_WITH_HEADERS] = 0;
    checksum = p_CmndApiPacket_CalcCheckSum( &pu8_Buffer[sizeof(syncWord)], u16_len - sizeof(syncWord) );
    pu8_Buffer[CMND_API_PROTOCOL_CHECKSUM_POS_WITH_HEADERS] = checksum;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndApiPacket_Print( t_st_Packet* pst_Packet, const char* direction )
{
    LOG_BUFFER( LOG_LEVEL_INFO, (const u8*)pst_Packet->buffer, pst_Packet->length, "%s",direction);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
