/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef C_HAN_IE_LIST_H
#define C_HAN_IE_LIST_H

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "StreamBuffer.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern_c_begin

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Information element structure: Type (1bytes), Len (2bytes) , Data (pointer to data)
typedef struct
{
    u8      u8_Type;    //!< Type of IE
    u16     u16_Len;    //!< IE data length
    u8*     pu8_Data;   //!< IE data
}
t_st_hanIeStruct;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// IE list
typedef struct
{
    t_st_StreamBuffer   st_Buffer;
}
t_st_hanIeList;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief      Initialize IE List object with a buffer that contains one or more
///             information elements.
///
/// @details    Call this function when IE list is used to traverse over existing IEs
///             List is allocated statically therefore the Init function should
///             receive a container for the list as the last parameter.
///
///             This API can also be used when IE list is not finalized and may be modified
///             by adding another IEs in that case the u16_IePayloadSize <= u16_IeBufferSize
///
/// @param[in]  pv_IePayload        - Buffer which contains IEs
/// @param[in]  u16_IePayloadSize   - Length of data occupied by IEs
/// @param[in]  u16_IeBufferSize    - Length of buffer which contans the IEs (Buffer is greater than Paylpad)
/// @param[out] pst_IeList          - Initialized IE list object
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_hanIeList_CreateWithPayload( IN const void* pv_IePayload,
                                    u16 u16_IePayloadSize,
                                    OUT t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Initialize IE List object with a buffer that contains one or more information
///             elements.
///
/// @details    This API is similar to p_hanIeList_CreateWithPayload but allows adding
///             new IEs to IE list
///
/// @param[in]  pv_IePayload        - Buffer which contains IEs
/// @param[in]  u16_IePayloadSize   - Length of data occupied by IEs
/// @param[in]  u16_IeBufferSize    - Length of buffer which contans the IEs (Buffer is greater than Paylpad)
/// @param[out] pst_IeList          - Initialized IE list object
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_hanIeList_CreateWithPayloadAppendable(   IN void*            pv_IePayload,
                                                u16                 u16_IePayloadSize,
                                                u16                 u16_IeBufferSize,
                                                OUT t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Initialize IE List object with a buffer that does not contain any IEs.
///
/// @details    Use this function to construct a new list of IEs
///
/// @param[in]  pv_Buffer           - Pointer to buffer to contain IEs
/// @param[in]  u16_BufferSize      - Buffer size
/// @param[out] pst_IeList          - initialized IE list object
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_hanIeList_CreateEmpty( IN void* pv_Buffer,  u16 u16_BufferSize, OUT t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Reset pointer to the first IE in the IE list
/// @note       It can be used to get first IE in list with #p_hanIeList_GetNextIe()
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void  p_hanIeList_ResetToFirstIe( IN t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get a pointer to the first IE in the IE list
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
/// @param[out] pp_Ie       - will point to the first IE in the IE List
///
/// @return     Function returns success / failure status
///////////////////////////////////////////////////////////////////////////////
bool  p_hanIeList_GetFirstIe( IN t_st_hanIeList* pst_IeList, OUT t_st_hanIeStruct* pst_Ie );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get a pointer to the next IE in the IE list
///
/// @details    Note that data is not copied, this function just returns a reference to the
///             data contained by Ie List
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
/// @param[out] pp_Ie       - will point to the first IE in the IE List
///
/// @return     Function returns success / failure status
///////////////////////////////////////////////////////////////////////////////
bool p_hanIeList_GetNextIe( IN t_st_hanIeList* pst_IeList, OUT t_st_hanIeStruct* pst_Ie );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add given IE to the IE list
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
/// @param[in]  pst_Ie      - pointer to IE structure
///
/// @return     Function returns success / failure status
///////////////////////////////////////////////////////////////////////////////
bool p_hanIeList_AddIe( IN t_st_hanIeList* pst_IeList, const t_st_hanIeStruct* pst_Ie );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add given IE to the IE list
///
/// @param[in]  pv_IeList       - list of IEs, each IE is of t_st_funIeHeader structure
/// @param[in]  u8_IeType       - IE Type
/// @param[in]  pu8_Data        - IE payload
/// @param[in]  u16_DataLen     - payload length
///
/// @return     Function returns success / failure status
///////////////////////////////////////////////////////////////////////////////
bool p_hanIeList_AddIeSimple(   t_st_hanIeList*     pst_IeList,
                                u8                  u8_IeType,
                                u8*                 pu8_Data,
                                u16                 u16_DataLen );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Return total size of IEs contained in the list
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
///
/// @return     Total occupied size (including IE type, header and payload fields
///////////////////////////////////////////////////////////////////////////////
u16 p_hanIeList_GetListSize( const t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Search for specific IE by type, start searching from the begging of the IE List
///
/// @details    Note that data is not copied, this function just returns a reference to the
///             data contained by Ie List
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
/// @param[out] pst_Ie      - will contain the found IE
///
/// @return     True if IE is found. the
///////////////////////////////////////////////////////////////////////////////
bool p_hanIeList_FindIeByType(  IN  t_st_hanIeList*     pst_IeList,
                                    u8                  u8_IeType,
                                OUT t_st_hanIeStruct*   pst_Ie      );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Return a pointer to IE list data
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
///
/// @return     a pointer to IE list data
///////////////////////////////////////////////////////////////////////////////
u8* p_hanIeList_GetDataPtr( const t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Return used size of IE list data
///
/// @param[in]  pv_IeList   - list of IEs, each IE is of t_st_funIeHeader structure
///
/// @return     used size of IE list data
///////////////////////////////////////////////////////////////////////////////
u16 p_hanIeList_GetDataSize( const t_st_hanIeList* pst_IeList );

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern_c_end

#endif // C_HAN_IE_LIST_H
