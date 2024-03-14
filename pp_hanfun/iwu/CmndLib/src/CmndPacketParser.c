/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndPacketParser.h"
#include "Logger.h"
#include "Endian.h"
#include "CmndApiExported.h"
#include "CmndApiIe.h"
#include "CmndApiHost.h"
#include <string.h> //memcpy

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndPacketParser_ParseCmndApiPacket( const t_st_Packet* packet, OUT t_st_hanCmndApiMsg* pst_cmndApiMsg )
{
    return p_CmndPacketParser_ParseCmndPacket( packet->length, packet->buffer, pst_cmndApiMsg );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Parse CMND API packet buffer
bool p_CmndPacketParser_ParseCmndPacket(    u16                     u16_BufferLength,
                                            const u8*               pu8_Buffer,
                                            OUT t_st_hanCmndApiMsg* pst_cmndApiMsg)
{
    bool ok = true;

    if (    ( u16_BufferLength < CMND_API_PROTOCOL_SIZE_WITHOUT_DATA )
        || !pst_cmndApiMsg )
    {
        return false;
    }

    memset( pst_cmndApiMsg, 0, sizeof(t_st_hanCmndApiMsg) );
    pst_cmndApiMsg->cookie      = pu8_Buffer[CMND_API_PROTOCOL_COOKIE_POS];
    pst_cmndApiMsg->unitId      = pu8_Buffer[CMND_API_PROTOCOL_UNITID_POS];
    memcpy( &(pst_cmndApiMsg->serviceId), &(pu8_Buffer[CMND_API_PROTOCOL_SERVICEID_POS]), sizeof(pst_cmndApiMsg->serviceId) );
    pst_cmndApiMsg->serviceId   = p_Endian_net2hos16(pst_cmndApiMsg->serviceId);
    pst_cmndApiMsg->messageId   = pu8_Buffer[CMND_API_PROTOCOL_MESSAGEID_POS];
    pst_cmndApiMsg->checkSum    = pu8_Buffer[CMND_API_PROTOCOL_CHECKSUM_POS];
    pst_cmndApiMsg->dataLength  = 0;

    if ( u16_BufferLength > CMND_API_PROTOCOL_SIZE_WITHOUT_DATA )
    {
        pst_cmndApiMsg->dataLength = u16_BufferLength - CMND_API_PROTOCOL_SIZE_WITHOUT_DATA;
        if ( pst_cmndApiMsg->dataLength < CMNDLIB_API_PACKET_MAX_SIZE )
        {
            memcpy(pst_cmndApiMsg->data, &(pu8_Buffer[CMND_API_PROTOCOL_DATASTART_POS]), pst_cmndApiMsg->dataLength);
        }
        else
        {
            ok = false;
        }
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
