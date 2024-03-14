/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_H
#define _CMND_MSG_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef VAR_ON_CMND
#include "TypeDefs.h"
#endif // VAR_ON_CMND
#include "CmndApiExported.h"
#include "StreamBuffer.h"
#include "CmndApiHost.h"
#include "IeList.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief      Check if CMND_IE_RESPONSE is succeeded
///
/// @param[in]  pst_Msg - CmndApiMsg
///
/// @return     true if succeeded
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_IeResponseIsOk( t_st_hanCmndApiMsg* pst_Msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      This function adds Response Required IE (CMND_IE_RESPONSE_REQUIRED) to existing message.
///
/// @param[in,out]  pst_hanCmndApiMsg       - Pointer to CmndApiMsg structure
///
/// @return     true if ok
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_IeResponseRequiredAdd( INOUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      This function adds Unit address IE (CMND_IE_UNIT_ADDR) to existing message.
///
/// @details    Add this IE when destination device is not BS.
///             Unit address is used to set a destination remote Device ID and Unit ID.
///             For example, it can be used:
///             1) OnOff service: to switch on/off of remote switchable device.
///             2) Alert service: to send alert to remote alertable device.
///             3) some other service
///
/// @param[in,out]  pst_hanCmndApiMsg       - Pointer to CmndApiMsg structure
/// @param[in]      pst_UnitId  - Pointer to IeUnitAddr structure that contains Destination device and unit IDs
///
/// @return     true if ok
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_IeUnitAddrAdd( INOUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeUnitAddr* pst_IeUnitAddr );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Macro to build getter function name out of IE enum name
#define p_CMND_IE_GETTER(cmnd_ie_enum_name) p_CMND_IE_GETTER_##cmnd_ie_enum_name

// Function pointer type to store IE getter method
// Converts data from t_st_StreamBuffer to certain struct, i.e. t_st(CMND_IE_GENERAL_STATUS):
typedef t_en_hanCmndInfoElemType (*tpf_CmndIeGetter)(INOUT t_st_StreamBuffer* pst_IeDataStream, OUT void* pv_st_hanCmndIeAny );

// Macro to build IE structure type name out of IE enum name
#define t_st(type_suffix) t_st##type_suffix

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Typedef of known IE structure to be available by IE enum name with macro t_st()
// Getter and setter function declaration

typedef t_st_hanCmndIeResponse              t_st(CMND_IE_RESPONSE                   );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_RESPONSE)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeAttributeId           t_st(CMND_IE_ATTRIBUTE_ID               );
typedef t_st_hanCmndIeAttributeValue        t_st(CMND_IE_ATTRIBUTE_VALUE            );

typedef t_st_hanCmndIeUnitAddr              t_st(CMND_IE_UNIT_ADDR                  );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_UNIT_ADDR)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef u8                                  t_st(CMND_IE_RESPONSE_REQUIRED          );

typedef t_st_hanCmndIeFun                   t_st(CMND_IE_FUN                        );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_FUN  )(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeAlert                 t_st(CMND_IE_ALERT                      );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_ALERT)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeSleepInfo             t_st(CMND_IE_SLEEP_INFO                 );

typedef t_st_hanCmndIeVersion               t_st(CMND_IE_VERSION                    );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_VERSION)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBatteryLevel          t_st(CMND_IE_BATTERY_LEVEL              );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_BATTERY_LEVEL)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeParameter             t_st(CMND_IE_PARAMETER                  );
typedef t_st_hanCmndIeParameterDirect       t_st(CMND_IE_PARAMETER_DIRECT           );

typedef t_st_hanCmndIeGeneralStatus         t_st(CMND_IE_GENERAL_STATUS             );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_GENERAL_STATUS)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeDeRegistration        t_st(CMND_IE_DEREGISTRATION             );

typedef t_st_hanCmndIeOtaCookie             t_st(CMND_IE_OTA_COOKIE                 );

typedef t_st_hanCmndIeUleCallSetting        t_st(CMND_IE_ULE_CALL_SETTING           );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_ULE_CALL_SETTING)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBatteryMeasureInfo    t_st(CMND_IE_BATTERY_MEASURE_INFO       );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_BATTERY_MEASURE_INFO)(t_st_StreamBuffer* p_Stream, void* p_Ie );

/*
typedef t_st_hanCmndIeCreateAttrReportEvent                                         t_st(CMND_IE_CREATE_ATTR_REPORT_EVENT   );
typedef t_st_hanCmndIeAttrAddReportEntry                                            t_st(CMND_IE_ATTR_ADD_REPORT_ENTRY      );
typedef t_st_hanCmndIeCreateAttrReportResponse                                      t_st(CMND_IE_CREATE_ATTR_REPORT_RESPONSE    );
typedef t_st_hanCmndIeAttrDeleteReport                                              t_st(CMND_IE_ATTR_DELETE_REPORT         );
typedef t_st_hanCmndIeAttrReportNotif                                               t_st(CMND_IE_ATTR_REPORT_NOTIF          );
*/

typedef t_st_hanCmndIeRegistrationResponse  t_st(CMND_IE_REGISTRATION_RESPONSE      );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_REGISTRATION_RESPONSE)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeDeRegistrationResponse    t_st(CMND_IE_DEREGISTRATION_RESPONSE);
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_DEREGISTRATION_RESPONSE)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeU8                    t_st(CMND_IE_U8                         );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_U8)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeU16                   t_st(CMND_IE_U16                        );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_U16)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeU32                   t_st(CMND_IE_U32                        );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_U32)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBandGap               t_st(CMND_IE_BG_REQ                     );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_BG_REQ)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBandGapRes            t_st(CMND_IE_BG_RES                     );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_BG_RES)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeTamperAlert           t_st(CMND_IE_TAMPER_ALERT               );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_TAMPER_ALERT)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeLinkMaintain          t_st(CMND_IE_LINK_MAINTAIN              );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_LINK_MAINTAIN)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBaseWanted            t_st(CMND_IE_BASE_WANTED                );

typedef t_st_hanCmndIePMID                  t_st(CMND_IE_PMID                       );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_PMID)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeBatteryLevel          t_st(CMND_IE_BATTERY_LEVEL                      );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_BATTERY_LEVEL)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeParameter             t_st(CMND_IE_PARAMETER                  );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_PARAMETER)(t_st_StreamBuffer* p_Stream, void* p_Ie );

typedef t_st_hanCmndIeParameterDirect       t_st(CMND_IE_PARAMETER_DIRECT           );
t_en_hanCmndInfoElemType                    p_CMND_IE_GETTER(CMND_IE_PARAMETER_DIRECT)(t_st_StreamBuffer* p_Stream, void* p_Ie );
/*
 *

typedef t_st_hanCmndIeIdentify                                                      t_st(CMND_IE_IDENTIFY                   );
typedef t_st_hanCmndIeAteContReq                                                    t_st(CMND_IE_ATE_CONT_REQ               );
typedef t_st_hanCmndIeAteRxReq                                                      t_st(CMND_IE_ATE_RX_REQ                 );
typedef t_st_hanCmndIeAteRxRes                                                      t_st(CMND_IE_ATE_RX_RES                 );
typedef t_st_hanCmndIeAteTxReq                                                      t_st(CMND_IE_ATE_TX_REQ                 );
typedef t_st_hanCmndIeBaseWanted                                                    t_st(CMND_IE_BASE_WANTED                );
typedef t_st_hanCmndIeReportId                                                      t_st(CMND_IE_REPORT_ID                  );
typedef t_st_hanCmndIeAddReportInfo                                                 t_st(CMND_IE_ADD_REPORT_INFO                );
typedef t_st_hanCmndIeReportInfo                                                        t_st(CMND_IE_REPORT_INFO                    );
typedef t_st_hanCmndIeCreateAttrReportPeriodic                                      t_st(CMND_IE_CREATE_ATTR_REPORT_PERIODIC    );
typedef t_st_hanCmndIeReportEntries                                                 t_st(CMND_IE_REPORT_ENTRIES             );
typedef t_st_hanCmndIeNewSwInfo                                                     t_st(CMND_IE_NEW_SW_INFO                    );
typedef t_st_hanCmndIeCurrentSwInfo                                                 t_st(CMND_IE_CURRENT_SW_INFO                );
typedef t_st_hanCmndIeImageType                                                     t_st(CMND_IE_IMAGE_TYPE                 );
typedef t_st_hanCmndIeSwVerInfo                                                     t_st(CMND_IE_SW_VER_INFO                    );
typedef t_st_hanCmndIeReadFileDataRes                                               t_st(CMND_IE_READ_FILE_DATA_RES         );
typedef t_st_hanCmndIeReadFileDataReq                                               t_st(CMND_IE_READ_FILE_DATA_REQ         );

typedef t_st_hanCmndIePortableIdentity                                              t_st(CMND_IE_PORTABLE_IDENTITY          );
typedef t_st_hanCmndIeLog                                                               t_st(CMND_IE_LOG                            );
typedef t_st_hanCmndIeGpioState                                                     t_st(CMND_IE_GPIO_STATE                 );

typedef t_st_hanCmndIeUpdateReportInterval                                          t_st(CMND_IE_UPDATE_REPORT_INTERVAL     );
typedef t_st_hanCmndIeCheckSwVerRes                                                 t_st(CMND_IE_CHECK_SW_VER_RES           );
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief Method for reading IE structure out of the message.
///
/// @details    This method is parameterized by IE-specific getter function
///             Usage example:
///             t_st_hanCmndApiMsg                      st_Msg = {0};
///             t_st(CMND_IE_GENERAL_STATUS)        st_Status = {0};
///
///             bool result = p_CmndMsg_IeGet(  &st_Msg,
///                                             p_CMND_IE_GETTER(CMND_IE_GENERAL_STATUS),
///                                             OUT &st_Status,
///                                             sizeof(st_Status));
///
///         Negative aspects:
///          - no type checking for &st_Status
///          - excess activity when need to get more than one IE from the message
///         Positive aspects:
///              + simple code (?)
///          + small code footprint because linker will add only used getters/setters
///
/// @param[in,out]  pst_Msg     - Pointer to CmndApiMsg structure
/// @param[in]      pst_UnitId  - Pointer to IeUnitAddr structure that contains Destination device and unit IDs
///
/// @return     true on success
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_IeGet(IN const t_st_hanCmndApiMsg* pst_Msg, tpf_CmndIeGetter p_Getter, OUT void* p_Ie, u16 u16_IeSize);
bool p_CmndMsg_IeGetFromList(IN const t_st_hanIeList* pst_IeList, tpf_CmndIeGetter pf_Getter, OUT void* pv_IeValue, u16 u16_IeSize);

extern_c_end

#endif  //_CMND_MSG_H
