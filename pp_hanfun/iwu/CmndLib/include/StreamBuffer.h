/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _STREAM_BUFFER_H
#define _STREAM_BUFFER_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      This structure is a Stream buffer object
///
/// @details    It contains a pointer to memory block and maintains pointers to
/// head of data, last position of data and the maximal size of the memory block.
/// The "Add" operations append data and move the <u16_DataSize> by the number of bytes added.
/// The "Get" operation returns the requested number of bytes and move the <u16_DataSize> pointer back.
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8*         pu8_Data;                   //!< Points to block of data
    u16         u16_DataSize;               //!< Current size - "add" operation appends data to the end of the buffer
    u16         u16_HeadPointer;            //!< Current point in pu8_Data, next read operation starts from it
    u16         u16_MaxSize;                //!< Max possible data size
    u8          u8_State;                   //!< Underrun and Overwrite states
}
t_st_StreamBuffer;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      Create new Stream buffer object which does not convey any payload at the moment
///                 Use this function when constructing a new stream
///
/// @param[out] pst_StreamBuffer    Pointer to stream object which should be initialized
/// @param[in]  pu8_Data            Pointer to allocated buffer which will be used by
///                                 the Stream object to store the payload
/// @param[in]  u16_BufferSize      Buffer size in bytes
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_hanStreamBuffer_CreateEmpty( OUT t_st_StreamBuffer*      pst_StreamBuffer,
                                    IN  u8*                     pu8_Data,
                                        u16                     u16_BufferSize );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Create new Stream buffer object using an existing payload
///                 Use this function when there's a need to parse existing array of bytes
///
/// @param[out] pst_StreamBuffer    Pointer to stream object which should be initialized
/// @param[in]  pu8_Data            Pointer to allocated buffer which will be used by
///                                 the Stream object to store the payload
/// @param[in]  u16_BufferSize      Size of pu8_Data
/// @param[in]  u16_DataSize        The length of actual data in pu8_Data
///
/// @note       It is possible that pu8_Data might be allocated as large buffer
///             but only a fraction of it is filled with valuable payload
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_hanStreamBuffer_CreateWithPayload(   OUT t_st_StreamBuffer*      pst_StreamBuffer,
                                            IN  u8*                     pu8_Data,
                                                u16                     u16_BufferSize,
                                                u16                     u16_DataSize        );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Set the data cursor to the begining of the stream
///             and clear the underrun and overrun states
///             (start using the buffer from scratch...)
///////////////////////////////////////////////////////////////////////////////
void p_hanStreamBuffer_Reset( t_st_StreamBuffer* pst_StreamBuffer );


///////////////////////////////////////////////////////////////////////////////
/// @brief      Return the number of bytes left in the stream
///////////////////////////////////////////////////////////////////////////////
u16 p_hanStreamBuffer_GetDataSize( const t_st_StreamBuffer* pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get reference to stream buffer (the non read part)
///////////////////////////////////////////////////////////////////////////////
u8* p_hanStreamBuffer_GetPointer( t_st_StreamBuffer*    pst_StreamBuffer );


///////////////////////////////////////////////////////////////////////////////
/// @brief      Check is stream buffer is empty
///
/// @return     Returns true if there's no unread data in the stream
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_IsEmpty( const t_st_StreamBuffer* pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Returns true if there's a case of underrun (reading more than
///             the size of the stream)
///             This flag is reset when p_hanStreamBuffer_Reset() is called
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_CheckUnderrun( const t_st_StreamBuffer*  pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Returns true if there's a case of overrun
///             ( attempt to write more than the max size of the stream internal buffer)
///             This flag is reset when p_hanStreamBuffer_Reset() is called
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_CheckOverrun( const t_st_StreamBuffer* pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      Serialize 8bit value to the stream
///             Use p_hanStreamBuffer_CheckOverrun to check overrun
///
/// @return     true if serialization succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_AddData8         ( t_st_StreamBuffer* pst_StreamBuffer, u8 u8_Data );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Serialize array of bytes to the stream
///             Use p_hanStreamBuffer_CheckOverrun to check overrun
///
/// @return     true if serialization succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_AddData8Array(   t_st_StreamBuffer*  pst_StreamBuffer,
                                        const u8*           pu8_Data,
                                        u16                 u16_SizeInBytes );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Serialize 16bit value to the stream
///             Data is stored in host byte order
///             Use p_hanStreamBuffer_CheckOverrun to check overrun
///
// @return      true if serialization succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_AddData16( t_st_StreamBuffer* pst_StreamBuffer, u16 u16_Data );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Serialize 32bit value to the stream
///             Data is stored in host byte order
///             Use p_hanStreamBuffer_CheckOverrun to check overrun
///
/// @return     true if serialization succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanStreamBuffer_AddData32( t_st_StreamBuffer* pst_StreamBuffer, u32 u32_Data );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      Deserialize array of bytes from the stream
///             Use p_hanStreamBuffer_CheckUnderrun to check underrun
///
/// @return     true if deserialization succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanSreamBuffer_GetData8Array(    t_st_StreamBuffer*  pst_StreamBuffer,
                                        u8*                 pu8_Dst,
                                        u16                 u16_SizeInBytes );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Deserialize 8bit value from the stream
///             Use p_hanStreamBuffer_CheckUnderrun to check underrun
///
/// @return     true if deserialization succeeded
///////////////////////////////////////////////////////////////////////////////
u8 p_hanStreamBuffer_GetData8   ( t_st_StreamBuffer*    pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Deserialize 16bit value from the stream
///             Use p_hanStreamBuffer_CheckUnderrun to check underrun
///
/// @return     true if deserialization succeeded
///////////////////////////////////////////////////////////////////////////////
u16 p_hanStreamBuffer_GetData16 ( t_st_StreamBuffer*    pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Deserialize 32bit value from the stream
///             Use p_hanStreamBuffer_CheckUnderrun to check underrun
///
/// @return     true if deserialization succeeded
///////////////////////////////////////////////////////////////////////////////
u32 p_hanStreamBuffer_GetData32 ( t_st_StreamBuffer*    pst_StreamBuffer );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// @brief      This is kind of "Deserialize" function but is does not return
///             the data, just skips the data as specified in u16_SizeInBytes
///             Use p_hanStreamBuffer_CheckUnderrun to check underrun
///
/// @return     true if skip succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_hanSreamBuffer_SkipData8Array( t_st_StreamBuffer* pst_StreamBuffer, u16 u16_SizeInBytes );


///////////////////////////////////////////////////////////////////////////////
/// @brief      Return how much bytes can be added to this stream
///////////////////////////////////////////////////////////////////////////////
u16 p_hanStreamBuffer_GetFreeSpace( const t_st_StreamBuffer*    pst_StreamBuffer );

extern_c_end

#endif // _STREAM_BUFFER_H
