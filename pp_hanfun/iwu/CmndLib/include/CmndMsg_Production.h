/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_PRODUCTION_H
#define _CMND_MSG_PRODUCTION_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#ifndef VAR_ON_CMND
#include "CmndApiHost.h"
#else
#include "chanCmndApiExported.h"
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production start production mode packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateStartReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production stop production mode packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateEndReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production init eeprom packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateInitEepromDefReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndMsgProdResetEeprom en_hanCmndMsgProdResetEeprom );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production start clock tune request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_Gpio            - specifies GPIO to use (other than UART GPIO)
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateRefClkTuneStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Gpio );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create Production create end reference clock tuning packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateRefClkTuneEndReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production change RXTUN return value packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_Val             - value for setting to RXTUN
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateReqClkTuneAdjReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Val);

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production band gap calibration packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_BandGap         - pointer to BandGapReq IE
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateBandGapCalibrationReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeBandGap* pst_BandGap );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production LMAC ATE test initialization packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_PacketLength    - packet length of the slot
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteInitReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_PacketLength );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production LMAC ATE test stop packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteStopReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production LMAC ATE continuous test start packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_AteContReq      - ATE process parameters
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteContinuousStartReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteContReq* pst_AteContReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production LMAC ATE Rx test start packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_AteRxReq        - consists Slot type, PP Sync, Slot#, Carrier, Ant, BER-FER and number of frames
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteRxStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteRxReq* pst_AteRxReq );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production LMAC ATE Tx test start packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_AteTxReq        - consists Slot ,Preamble, Slot#, Carrier, Power Level, Ant and Pattern
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteTxStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteTxReq* pst_AteTxReq );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create Production get BER-FER measurement request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateAteGetBerFerReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set EEPROM to automatic preset packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_Val             - see CMND API SPEC 4.23.3.18
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSpecificPresetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Val);

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production hibernation mode for ULE operation packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSleepReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set GPIO low packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioNumber      - GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSetSimpleGpioLow( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set GPIO high packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_GpioNumber              - GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSetSimpleGpioHigh( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production get GPIO state packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioNumber      - GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateGetSimpleGpioState( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set ULE GPIO low packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioNumber      - GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSetUleGpioLow( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set ULE GPIO high packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioNumber      - ULE GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSetUleGpioHigh( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production get ULE GPIO state packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioNumber      - ULE GPIO number
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateGetUleGpioState( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production set input ULE GPIO direction packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_GpioId          - ULE GPIO id
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateSetUleGpioDirInputReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioId );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production firmware update request
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      en_FwMode           - erase factory or not
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateFwUpdateReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_en_hanCmndFwUpdateMode en_FwMode );


///////////////////////////////////////////////////////////////////////////////
/// @brief  Create Production reset eeprom packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         This is more specific API to reset defined EEPROM block (DECT or HAN)
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg            - pointer to message
/// @param[in]      en_hanCmndMsgProdResetEeprom - defines eeprom type to reset (DECT or HAN)
///
/// @return     true when success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Production_CreateResetEeepromReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndMsgProdResetEeprom en_hanCmndMsgProdResetEeprom );

extern_c_end

#endif  //_CMND_MSG_PRODUCTION_H
