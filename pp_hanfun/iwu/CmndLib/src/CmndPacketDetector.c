/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndPacketDetector.h"
#include "Endian.h"
#include "CmndApiExported.h"
#include "CmndApiPacket.h"
#include "Logger.h"
#include "CmndApiHost.h"
#include "CmndPacketParser.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define SYNC_BYTE 0xDA

/// Flag of Enable checksum validation
static bool g_b_ValidateCheckSum = false;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Enable checksum validation
void p_CmndApiDetector_EnableCheckSum( bool b_ValidateCheckSum )
{
    g_b_ValidateCheckSum = b_ValidateCheckSum;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanCmndApi_HandleByte(t_stReceiveData* context, u8 byte, OUT t_st_Msg* msg )
{
    context->result = p_CmndApiDetector_DetectAppendByte( context, byte );

    if(context->result == E_DETECT_PACKET_OK)
    {
        return p_CmndPacketParser_ParseCmndPacket(context->packet.length, context->packet.buffer, msg);
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

t_en_CmndApi_DetectCode p_CmndApiDetector_DetectAppendByte( t_stReceiveData* context, u8 newByte )
{
    t_en_CmndApi_DetectCode en_RetCode = E_DETECT_PACKET_ONGOING;

    switch(context->state)
    {
        case MSG_ST_SYNC_WAIT1:
        {
            if( newByte == SYNC_BYTE )
            {   //found first sync
                context->state = MSG_ST_SYNC_WAIT2;
            }
        }
        break;

        case MSG_ST_SYNC_WAIT2:
        {
            context->state = ( newByte == SYNC_BYTE ) ? MSG_ST_PACKET_LENGTH1 : MSG_ST_SYNC_WAIT1; // fall back to start
        }
        break;

        case MSG_ST_PACKET_LENGTH1:
        {
            if( newByte != SYNC_BYTE )  // Ignore extra sync byte
            {
                context->lengthFromPacket = ( newByte << 8 );
                context->state = ( context->lengthFromPacket <= CMNDLIB_API_PACKET_MAX_SIZE ) ? MSG_ST_PACKET_LENGTH2 : MSG_ST_SYNC_WAIT1;  //length is too big - fall back to start
            }
        }
        break;

        case MSG_ST_PACKET_LENGTH2:
        {
            context->lengthFromPacket |= newByte;
            context->state = ( context->lengthFromPacket <= CMNDLIB_API_PACKET_MAX_SIZE ) ? MSG_ST_ACCUMULATE : MSG_ST_SYNC_WAIT1;  //length is too big - fall back to start
            context->inIndex = 0;
        }
        break;

        case MSG_ST_ACCUMULATE:
        {
            // accumulate current byte as payload
            context->packet.buffer[context->inIndex] = newByte;
            context->inIndex++;

            if ( context->inIndex == context->lengthFromPacket )    //detect end of packet
            {
                u8 u8_ActualChecksum = 0;
                u8 u8_ExpectedChecksum = 0;
                u16 u16_netMsgLen = 0;

                context->packet.length = context->inIndex;

                context->state = MSG_ST_SYNC_WAIT1;

                u8_ExpectedChecksum = context->packet.buffer[CMND_API_PROTOCOL_CHECKSUM_POS];

                // calculate checksum without length and checksum
                u8_ActualChecksum = p_CmndApiPacket_CalcCheckSum( context->packet.buffer, CMND_API_PROTOCOL_SIZE_MANDATORY_FIELDS - CMND_API_PROTOCOL_SIZE_HEADER - sizeof(u8_ActualChecksum) );

                // add length field checksum
                u16_netMsgLen = p_Endian_hos2net16(context->packet.length);
                u8_ActualChecksum += p_CmndApiPacket_CalcCheckSum( (u8*)&(u16_netMsgLen), sizeof(u16_netMsgLen) );

                // add Data if needed
                if ( context->packet.length > CMND_API_PROTOCOL_SIZE_WITHOUT_DATA )
                {
                    u8_ActualChecksum +=  p_CmndApiPacket_CalcCheckSum( &context->packet.buffer[CMND_API_PROTOCOL_SIZE_WITHOUT_DATA], context->packet.length - CMND_API_PROTOCOL_SIZE_WITHOUT_DATA);
                }

                if ( g_b_ValidateCheckSum && ( u8_ExpectedChecksum != u8_ActualChecksum ) )
                {
                    LOG_ERROR(  "Checksum failed. Expected<0x%x>, actual<0x%x>",
                        u8_ExpectedChecksum,
                        u8_ActualChecksum );
                    en_RetCode = E_DETECT_PACKET_CHECKSUM_ERROR;
                    break; //exit from switch-case
                }

                en_RetCode = E_DETECT_PACKET_OK;
            }
            break;
        }
    }

    return en_RetCode;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Detect and accumulate CMND API message
t_en_CmndApi_DetectCode p_CmndApiDetector_Detect(   const   u8*     pu8_InputBuf,       // input buffer
                                                    u16     u16_InputBufLen,    // InputBuf size
                                            INOUT   u16*    u16_InputBufIndex,  // IN - start position, OUT - new position
                                            OUT     u8*     pu8_OutputBuf,      // buffer for detected payload, limited by CMND_API_PACKET_MAX_SIZE
                                            OUT     u16*    pu16_OutputBufLen,  // detected packet size.
                                            INOUT   u16*    pu16_OutputBufIndex,
                                                    e_message_state *pe_messageState)
{
    t_en_CmndApi_DetectCode en_RetCode = E_DETECT_PACKET_ONGOING;

    while ( ( *u16_InputBufIndex < u16_InputBufLen )
            && ( en_RetCode == E_DETECT_PACKET_ONGOING ) )
    {
        u8 u8_Current = pu8_InputBuf[ *u16_InputBufIndex ];

        switch(*pe_messageState)
        {
            case MSG_ST_SYNC_WAIT1:
            {
                if( u8_Current == SYNC_BYTE )
                {   //found first sync
                    *pe_messageState = MSG_ST_SYNC_WAIT2;
                }
            }
            break;

            case MSG_ST_SYNC_WAIT2:
            {
                *pe_messageState = ( u8_Current == SYNC_BYTE ) ? MSG_ST_PACKET_LENGTH1 : MSG_ST_SYNC_WAIT1; // fall back to start
            }
            break;

            case MSG_ST_PACKET_LENGTH1:
            {
                if( u8_Current != SYNC_BYTE )   // Ignore extra sync byte
                {
                    *pu16_OutputBufLen = ( u8_Current << 8 );
                    *pe_messageState =  ( *pu16_OutputBufLen <= CMNDLIB_API_PACKET_MAX_SIZE ) ? MSG_ST_PACKET_LENGTH2 : MSG_ST_SYNC_WAIT1;  //length is too big - fall back to start
                }
            }
            break;

            case MSG_ST_PACKET_LENGTH2:
            {
                *pu16_OutputBufLen |= u8_Current;
                *pe_messageState =  ( *pu16_OutputBufLen <= CMNDLIB_API_PACKET_MAX_SIZE ) ? MSG_ST_ACCUMULATE : MSG_ST_SYNC_WAIT1;  //length is too big - fall back to start

                *pu16_OutputBufIndex = 0;
            }
            break;

            case MSG_ST_ACCUMULATE:
            {
                // accumulate current byte as payload
                pu8_OutputBuf[*pu16_OutputBufIndex] = u8_Current;
                (*pu16_OutputBufIndex)++;

                if ( *pu16_OutputBufIndex == *pu16_OutputBufLen )   //detect end of packet
                {
                    u8 u8_ActualChecksum = 0;
                    u8 u8_ExpectedChecksum = 0;
                    u16 u16_netMsgLen = 0;

                    *pe_messageState = MSG_ST_SYNC_WAIT1;

                    u8_ExpectedChecksum = pu8_OutputBuf[CMND_API_PROTOCOL_CHECKSUM_POS];

                    // calculate checksum without length and checksum
                    u8_ActualChecksum = p_CmndApiPacket_CalcCheckSum(   pu8_OutputBuf,
                                                                        CMND_API_PROTOCOL_SIZE_MANDATORY_FIELDS -
                                                                        CMND_API_PROTOCOL_SIZE_HEADER -
                                                                        sizeof(u8_ActualChecksum) );

                    // add length field checksum
                    u16_netMsgLen = p_Endian_hos2net16(*pu16_OutputBufLen);
                    u8_ActualChecksum += p_CmndApiPacket_CalcCheckSum( (u8*)&(u16_netMsgLen), sizeof(u16_netMsgLen) );

                    // add Data if needed
                    if ( *pu16_OutputBufLen > CMND_API_PROTOCOL_SIZE_WITHOUT_DATA )
                    {
                        u8_ActualChecksum +=  p_CmndApiPacket_CalcCheckSum( &pu8_OutputBuf[CMND_API_PROTOCOL_SIZE_WITHOUT_DATA],
                                                                            *pu16_OutputBufLen - CMND_API_PROTOCOL_SIZE_WITHOUT_DATA);

                    }

                    if ( g_b_ValidateCheckSum && ( u8_ExpectedChecksum != u8_ActualChecksum ) )
                    {
                        LOG_ERROR(  "Checksum failed. Expected<0x%x>, actual<0x%x>",
                                    u8_ExpectedChecksum,
                                    u8_ActualChecksum );
                        en_RetCode = E_DETECT_PACKET_CHECKSUM_ERROR;
                        break; //exit from switch-case
                    }

                    en_RetCode = E_DETECT_PACKET_OK;
                }
                break;
            }
        }
        (*u16_InputBufIndex)++;
    }

    return en_RetCode;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
