/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_PARAMETERS_H
#define _CMND_MSG_PARAMETERS_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Parameter get packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_Param           - defines the parameter to read
/// @param[in]      pu8_ParamData       - pointer to parameter data
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Parameter_CreateParamGetReq( OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeParameter*  pst_Param );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Parameter get direct packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_DirectParam             - defines memory address and length to read
/// @param[in]      pu8_ParamData               - pointer to parameter data
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Parameter_CreateParamGetDirectReq(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                const t_st_hanCmndIeParameterDirect*    pst_DirectParam );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Parameter set packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_Parameter       - defines the parameter to write
/// @param[in]      pu8_ParamData       - pointer to parameter data
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Parameter_CreateParamSetReq( OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeParameter*  pst_Parameter );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Parameter set direct packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_DirectParam     - defines the parameter to write
/// @param[in]      pu8_ParamData       - pointer to parameter data
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Parameter_CreateParamSetDirectReq(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                const t_st_hanCmndIeParameterDirect*    pst_DirectParam );

extern_c_end

#endif  //_CMND_MSG_PARAMETERS_H
