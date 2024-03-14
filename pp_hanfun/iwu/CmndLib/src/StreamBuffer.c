/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "StreamBuffer.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// TODO: ADD BIG/ENDIAN SUPPORT

#define STREAM_BUF_OVERRUN_MASK     (0x1)
#define STREAM_BUF_UNDERRUN_MASK    (0x2)

#define SET_OVERRUN( Buf )      ( (Buf)->u8_State |= STREAM_BUF_OVERRUN_MASK )
#define SET_UNDERRUN( Buf )     ( (Buf)->u8_State |= STREAM_BUF_UNDERRUN_MASK )

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_hanStreamBuffer_CreateEmpty( t_st_StreamBuffer*  pst_StreamBuffer,
                                    u8*                 pu8_Data,
                                    u16                 u16_BufferSize )
{
    pst_StreamBuffer->pu8_Data          = pu8_Data;
    pst_StreamBuffer->u16_MaxSize       = u16_BufferSize;
    pst_StreamBuffer->u16_DataSize      = 0;
    pst_StreamBuffer->u16_HeadPointer   = 0;
    pst_StreamBuffer->u8_State          = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_hanStreamBuffer_CreateWithPayload(   t_st_StreamBuffer*      pst_StreamBuffer,
                                            u8*                     pu8_Data,
                                            u16                     u16_BufferSize,
                                            u16                     u16_DataSize )
{
    p_hanStreamBuffer_CreateEmpty( pst_StreamBuffer, pu8_Data, u16_BufferSize );
    pst_StreamBuffer->u16_DataSize = u16_DataSize;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_hanStreamBuffer_Reset( t_st_StreamBuffer* pst_StreamBuffer )
{
    pst_StreamBuffer->u16_HeadPointer   = 0;
    pst_StreamBuffer->u8_State          = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_hanStreamBuffer_GetDataSize( const t_st_StreamBuffer* pst_StreamBuffer )
{
    // return the number of bytes which were still not read from the buffer
    // DataSize is the index of the last byte added
    // HeadPonter is the index of the last byte read
    return pst_StreamBuffer->u16_DataSize - pst_StreamBuffer->u16_HeadPointer;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u8* p_hanStreamBuffer_GetPointer( t_st_StreamBuffer* pst_StreamBuffer )
{
    return pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_HeadPointer;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_IsEmpty( const t_st_StreamBuffer*    pst_StreamBuffer )
{
    return (pst_StreamBuffer->u16_HeadPointer >= pst_StreamBuffer->u16_DataSize)
        ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_CheckUnderrun( const t_st_StreamBuffer*  pst_StreamBuffer )
{
    return ( pst_StreamBuffer->u8_State & STREAM_BUF_UNDERRUN_MASK ) ? 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_CheckOverrun( const t_st_StreamBuffer* pst_StreamBuffer )
{
    return ( pst_StreamBuffer->u8_State & STREAM_BUF_OVERRUN_MASK ) ? 1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_AddData8( t_st_StreamBuffer* pst_StreamBuffer, u8 u8_Data )
{
    bool RetVal = false;
    if ( pst_StreamBuffer->u16_DataSize < pst_StreamBuffer->u16_MaxSize )
    {
        pst_StreamBuffer->pu8_Data[pst_StreamBuffer->u16_DataSize] = u8_Data;
        pst_StreamBuffer->u16_DataSize++;

        RetVal = true;
    }
    else
    {
        SET_OVERRUN(pst_StreamBuffer);
    }
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_AddData8Array(   t_st_StreamBuffer*  pst_StreamBuffer,
                                        const u8*           pu8_Data,
                                        u16                 u16_SizeInBytes )
{
    bool RetVal = false;

    if ( (pst_StreamBuffer->u16_DataSize + u16_SizeInBytes) <= pst_StreamBuffer->u16_MaxSize )
    {
        memcpy( pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_DataSize,
                pu8_Data,
                u16_SizeInBytes );

        pst_StreamBuffer->u16_DataSize += u16_SizeInBytes;
        RetVal = true;
    }
    else
    {
        SET_OVERRUN(pst_StreamBuffer);
    }
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_AddData16( t_st_StreamBuffer* pst_StreamBuffer, u16 u16_Data )
{
    bool RetVal = false;

    if ( (pst_StreamBuffer->u16_DataSize + sizeof(u16_Data)) <= pst_StreamBuffer->u16_MaxSize )
    {
        // host byte order
        memcpy( pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_DataSize,
                &u16_Data,
                sizeof(u16_Data) );

        pst_StreamBuffer->u16_DataSize += sizeof(u16_Data);
        RetVal = true;
    }
    else
    {
        SET_OVERRUN(pst_StreamBuffer);
    }
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanStreamBuffer_AddData32( t_st_StreamBuffer* pst_StreamBuffer, u32 u32_Data )
{
    bool RetVal = false;
    if ( (pst_StreamBuffer->u16_DataSize + sizeof(u32_Data)) <= pst_StreamBuffer->u16_MaxSize )
    {
        // host byte order
        memcpy( pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_DataSize,
                &u32_Data,
                sizeof(u32_Data) );

        pst_StreamBuffer->u16_DataSize += sizeof(u32_Data);
        RetVal = true;
    }
    else
    {
        SET_OVERRUN(pst_StreamBuffer);
    }
    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u8 p_hanStreamBuffer_GetData8   ( t_st_StreamBuffer*    pst_StreamBuffer )
{
    u8  u8_Data = 0;

    if ( pst_StreamBuffer->u16_HeadPointer < pst_StreamBuffer->u16_DataSize )
    {
        u8_Data = pst_StreamBuffer->pu8_Data[pst_StreamBuffer->u16_HeadPointer];
        pst_StreamBuffer->u16_HeadPointer++;
    }
    else
    {
        SET_UNDERRUN( pst_StreamBuffer );
    }
    return u8_Data;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanSreamBuffer_GetData8Array(    t_st_StreamBuffer*  pst_StreamBuffer,
                                        u8*                 pu8_Dst,
                                        u16                 u16_SizeInBytes )
{
    bool RetVal = false;
    if ( (pst_StreamBuffer->u16_HeadPointer + u16_SizeInBytes) <= pst_StreamBuffer->u16_DataSize )
    {
        memcpy( pu8_Dst,
                pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_HeadPointer,
                u16_SizeInBytes );

        pst_StreamBuffer->u16_HeadPointer += u16_SizeInBytes;
        RetVal = true;
    }
    else
    {
        SET_UNDERRUN( pst_StreamBuffer );
    }

    return RetVal;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_hanStreamBuffer_GetData16( t_st_StreamBuffer* pst_StreamBuffer )
{
    u16 u16_Data16 = 0;
    if ( (pst_StreamBuffer->u16_HeadPointer + sizeof(u16)) <= pst_StreamBuffer->u16_DataSize )
    {
        memcpy( &u16_Data16,
                pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_HeadPointer,
                sizeof(u16) );

        pst_StreamBuffer->u16_HeadPointer += sizeof(u16);
    }
    else
    {
        SET_UNDERRUN( pst_StreamBuffer );
    }
    return u16_Data16;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u32 p_hanStreamBuffer_GetData32 ( t_st_StreamBuffer*    pst_StreamBuffer )
{
    u32 u32_Data = 0;

    if ( (pst_StreamBuffer->u16_HeadPointer + sizeof(u32)) <= pst_StreamBuffer->u16_DataSize )
    {
        memcpy( &u32_Data,
                pst_StreamBuffer->pu8_Data + pst_StreamBuffer->u16_HeadPointer,
                sizeof(u32) );

        pst_StreamBuffer->u16_HeadPointer += sizeof(u32);
    }
    else
    {
        SET_UNDERRUN( pst_StreamBuffer );
    }
    return u32_Data;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_hanSreamBuffer_SkipData8Array( t_st_StreamBuffer* pst_StreamBuffer, u16 u16_SizeInBytes )
{
    if ( (pst_StreamBuffer->u16_HeadPointer + u16_SizeInBytes) <= pst_StreamBuffer->u16_DataSize )
    {
        pst_StreamBuffer->u16_HeadPointer += u16_SizeInBytes;
        return true;
    }
    else
    {
        SET_UNDERRUN( pst_StreamBuffer );
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

u16 p_hanStreamBuffer_GetFreeSpace( const t_st_StreamBuffer*    pst_StreamBuffer )
{
    return pst_StreamBuffer->u16_MaxSize - pst_StreamBuffer->u16_DataSize;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
