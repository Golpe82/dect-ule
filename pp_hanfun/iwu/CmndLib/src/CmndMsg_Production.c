/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_Production.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of start production mode message
bool p_CmndMsg_Production_CreateStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_START_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of stop production mode message
bool p_CmndMsg_Production_CreateEndReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_END_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of init EEPROM message
bool p_CmndMsg_Production_CreateInitEepromDefReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndMsgProdResetEeprom en_hanCmndMsgProdResetEeprom )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_INIT_EEPROM_DEF_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );
    {
        t_st_hanCmndIeU8 st_U8Gen;
        st_U8Gen.u8_Data = en_hanCmndMsgProdResetEeprom;
        // Add U8 IE
        ok = p_hanCmndApi_IeU8Add( &st_IeList, &st_U8Gen );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateRefClkTuneStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Gpio)
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_REF_CLK_TUNE_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_Gpio );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateRefClkTuneEndReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_REF_CLK_TUNE_END_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateReqClkTuneAdjReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Val)
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_REF_CLK_TUNE_ADJ_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_Val );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateBandGapCalibrationReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeBandGap* pst_BandGap )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_BG_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add Band Gap IE
    ok = p_hanCmndApi_IeBandGapAdd( &st_IeList, pst_BandGap );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteInitReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_PacketLength )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_ATE_INIT_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_PacketLength );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteStopReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_ATE_STOP_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteContinuousStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteContReq* pst_AteContReq )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_ATE_CONTINUOUS_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add CMND_IE_ATE_CONT_REQ
    ok = p_hanCmndApi_IeAteContReqAdd( &st_IeList, pst_AteContReq );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteRxStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteRxReq* pst_AteRxReq)
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_ATE_RX_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add CMND_IE_ATE_RX_REQ
    ok = p_hanCmndApi_IeAteRxReqAdd( &st_IeList, pst_AteRxReq );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteTxStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAteTxReq* pst_AteTxReq )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_ATE_TX_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add CMND_IE_ATE_RX_REQ
    ok = p_hanCmndApi_IeAteTxReqAdd( &st_IeList, pst_AteTxReq );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateAteGetBerFerReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_ATE_GET_BER_FER_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSpecificPresetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_Val)
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SPECIFIC_PRESET_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_Val );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSleepReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_SLEEP_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSetSimpleGpioLow( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SET_SIMPLE_GPIO_LOW;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSetSimpleGpioHigh( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SET_SIMPLE_GPIO_HIGH;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateGetSimpleGpioState( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_GET_SIMPLE_GPIO_STATE;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSetUleGpioLow( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SET_ULE_GPIO_LOW;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSetUleGpioHigh( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SET_ULE_GPIO_HIGH;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateGetUleGpioState( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioNumber )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_GET_ULE_GPIO_STATE;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioNumber );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateSetUleGpioDirInputReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeU8* pst_GpioId )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_SET_ULE_GPIO_DIR_INPUT_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, pst_GpioId );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_Production_CreateFwUpdateReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_en_hanCmndFwUpdateMode en_FwMode )
{
    bool                ok;
    t_st_hanIeList      st_IeList;
    t_st_hanCmndIeU8    st_UpdateMode;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PROD_FW_UPDATE_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    st_UpdateMode.u8_Data = (u8) en_FwMode;

    // Add U8 IE
    ok = p_hanCmndApi_IeU8Add( &st_IeList, &st_UpdateMode );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Reset EEPROM message (differ from Init Eeprom to defaults message)
bool p_CmndMsg_Production_CreateResetEeepromReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndMsgProdResetEeprom en_hanCmndMsgProdResetEeprom )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PRODUCTION;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PROD_RESET_EEPROM;
    pst_hanCmndApiMsg ->unitId    = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );
    {
        t_st_hanCmndIeU8 st_U8Gen;
        st_U8Gen.u8_Data = (u8)en_hanCmndMsgProdResetEeprom;
        // Add U8 IE
        ok = p_hanCmndApi_IeU8Add( &st_IeList, &st_U8Gen );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
