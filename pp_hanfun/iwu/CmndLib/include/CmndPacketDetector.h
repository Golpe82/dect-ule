/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_DETECTOR_H
#define _CMND_API_DETECTOR_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiPacket.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// State of CMND API packet detection
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    E_DETECT_PACKET_OK              = 0,    //!< Packet detection ends successful
    E_DETECT_PACKET_ONGOING         = 1,    //!< Packet accumulation in process
    E_DETECT_PACKET_CHECKSUM_ERROR  = 2,    //!< Packet has bad checksum
}
t_en_CmndApi_DetectCode;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      Enable checksum validation
///
/// @param[in]  b_ValidateCheckSum  - Flag of Enable checksum validation
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndApiDetector_EnableCheckSum( bool b_ValidateCheckSum );

///////////////////////////////////////////////////////////////////////////////
/// State to Identify and Accumulate message
///////////////////////////////////////////////////////////////////////////////
typedef enum {
    MSG_ST_SYNC_WAIT1,      // Waiting for first sync DA
    MSG_ST_SYNC_WAIT2,      // Waiting for second sync DA
    MSG_ST_PACKET_LENGTH1,  // Waiting for first byte of length u16
    MSG_ST_PACKET_LENGTH2,  // Waiting for second byte of length u16
    MSG_ST_ACCUMULATE       // Accumulating payload
} e_message_state;

typedef struct
{
    t_en_CmndApi_DetectCode result;
    t_st_Packet packet;

    u16 inIndex;
    u16 lengthFromPacket;
    e_message_state state;
}
t_stReceiveData;

bool p_hanCmndApi_HandleByte( t_stReceiveData* context, u8 byte, t_st_Msg* msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Detect and accumulate CMND API packet
///
/// @param[in,out]  context     - context
/// @param[in]      newByte     - incoming byte
///
/// @return     t_en_CmndApi_DetectCode
///////////////////////////////////////////////////////////////////////////////
t_en_CmndApi_DetectCode p_CmndApiDetector_DetectAppendByte( INOUT t_stReceiveData* context, u8 newByte );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Detect and accumulate CMND API packet
///
/// @param[in]      pu8_InputBuf        - pointer to incoming buffer
/// @param[in]      u16_InputBufLen     - pointer to incoming buffer length
/// @param[in,out]  u16_InputBufIndex   - pointer to current incoming buffer position
/// @param[out]     pu8_OutputBuf       - pointer to CMND output packet buffer
/// @param[out]     pu16_OutputBufLen   - pointer to CMND output packet length
///
/// @return     t_en_CmndApi_DetectCode
///////////////////////////////////////////////////////////////////////////////
t_en_CmndApi_DetectCode p_CmndApiDetector_Detect(   const   u8*     pu8_InputBuf,       // input buffer
                                                    u16     u16_InputBufLen,    // InputBuf size
                                                    INOUT   u16*    u16_InputBufIndex,  // IN - start position, OUT - new position
                                                    OUT     u8*     pu8_OutputBuf,      // buffer for detected payload, limited by CMND_API_PACKET_MAX_SIZE
                                                    OUT     u16*    pu16_OutputBufLen,  // detected packet size.
                                                    u16* pu16_OutputBufIndex,
                                                    e_message_state *pe_messageState);


extern_c_end

#endif // _CMND_API_DETECTOR_H
