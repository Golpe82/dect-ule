# SPDX-License-Identifier: MIT
import time
import struct
import sys


class TimeoutError(Exception):
    pass


class ChecksumError(Exception):
    pass


class IENotFoundError(Exception):
    pass


class IEUnpackError(Exception):
    pass


# CMBS packet:
#    4 byte sync: 0xdadadada
#    8 byte msghdr:
#      - u16 TotalLength = (header length + payload length)
#      - u16 PacketNr = 0
#      - u16 EventId = event id, e.g.
#      - u16 ParamLength = payload length
#   <payload>
#   (6 byte checksum)

SYNC = 0xdadadada


#
# Commands
#

CMD_HELLO = 1  # Host send this command to target for registration
CMD_HELLO_RPLY = 2  # Target replies on an Hello with this command
CMD_FLOW_NOK = 3  # The reception was not successful
CMD_FLOW_RESTART = 4  # Restart with contained packet number
CMD_RESET = 5  # Reset the communication between host and target
CMD_FLASH_START_REQ = 6  # CMBS requests to enter flashing state
CMD_FLASH_START_RES = 7  # Host approves / reject request to enter flashing state
CMD_FLASH_STOP_REQ = 8  # CMBS requests to leave flashing state
CMD_FLASH_STOP_RES = 9  # Host approves / reject request to leave flashing state
CMD_CAPABILITIES = 10  # Host sends Capabilities, e.g. Checksum support (bit 1)
CMD_CAPABILITIES_RPLY = 11  # Target replies Capabilities, e.g. Checksum support (bit 1)
CMD_STORE_RAM_DUMP = 12   # This is to forcefully reset the CMBS to capture RAM dump


#
# Events
#

EV_UNDEF = 0
EV_DSR_HS_PAGE_START = 1  # Performs paging handsets
EV_DSR_HS_PAGE_START_RES = 2   # Response to CMBS_EV_DSR_HS_PAGE_START
EV_DSR_HS_PAGE_STOP = 3   # Performs stop paging handsets
EV_DSR_HS_PAGE_STOP_RES = 4   # Response to CMBS_EV_DSR_HS_PAGE_STOP
EV_DSR_HS_DELETE = 5   # Delete one or more handsets from the base's database
EV_DSR_HS_DELETE_RES = 6   # Response to CMBS_EV_DSR_HS_DELETE
EV_DSR_HS_REGISTERED = 7   # Unsolicited event generated on successful register/unregister operation of a handset
EV_DSR_HS_IN_RANGE = 8   # Generated when a handset in range
EV_DSR_CORD_OPENREG = 9   # Starts registration mode on the base station
EV_DSR_CORD_OPENREG_RES = 10   # Response to CMBS_EV_DSR_CORD_OPENREG
EV_DSR_CORD_CLOSEREG = 11   # Stops registration mode on the base station
EV_DSR_CORD_CLOSEREG_RES = 12   # Response to CMBS_EV_DSR_CORD_CLOSEREG
EV_DSR_PARAM_GET = 13   # Get a parameter value
EV_DSR_PARAM_GET_RES = 14   # Response to CMBS_EV_DSR_PARAM_GET
EV_DSR_PARAM_SET = 15   # Sets / updates a parameter value
EV_DSR_PARAM_SET_RES = 16   # Response to CMBS_EV_DSR_PARAM_SET
EV_DSR_FW_UPD_START = 17  # Starts firmware update on the base station
EV_DSR_FW_UPD_START_RES = 18  # Response to CMBS_EV_DSR_FW_UPD_START
EV_DSR_FW_UPD_PACKETNEXT = 19  # Sends a chunk of firmware to the base station
EV_DSR_FW_UPD_PACKETNEXT_RES = 20  # Response to CMBS_EV_DSR_FW_UPD_PACKETNEXT
EV_DSR_FW_UPD_END = 21  # Ending firmware update process with last chunk of data
EV_DSR_FW_UPD_END_RES = 22  # Response to CMBS_EV_DSR_FW_UPD_END
EV_DSR_FW_VERSION_GET = 23  # Gets the base's current firmware version of a particular module
EV_DSR_FW_VERSION_GET_RES = 24  # Response to CMBS_EV_DSR_FW_VERSION_GET
EV_DSR_SYS_START = 25  # Starts the base station's CMBS after parameters were set
EV_DSR_SYS_START_RES = 26  # Response to CMBS_EV_DSR_SYS_START
EV_DSR_SYS_SEND_RAWMSG = 27  # Event containing a raw message to the target
EV_DSR_SYS_SEND_RAWMSG_RES = 28  # Response to CMBS_EV_DSR_SYS_SEND_RAWMSG
EV_DSR_SYS_STATUS = 29  # Announce current target status, e.g. up, down, removed
EV_DSR_SYS_LOG = 30  # Event containing target system logs
EV_DSR_SYS_RESET = 31  # Performs a base station reboot
EV_DSR_SYS_POWER_OFF = 32  # Performs a base station power off

EV_DSR_HS_SUBSCRIBED_LIST_GET = 33  # Get list of subscribed handsets
EV_DSR_HS_SUBSCRIBED_LIST_GET_RES = 34  # result of CMBS_EV_DSR_HS_SUBSCRIBED_LIST_GET
EV_DSR_HS_SUBSCRIBED_LIST_SET = 35  # Set list of subscribed handsets
EV_DSR_HS_SUBSCRIBED_LIST_SET_RES = 36  # result of CMBS_EV_DSR_HS_SUBSCRIBED_LIST_SET

EV_DSR_LINE_SETTINGS_LIST_GET = 37  # Get list of subscribed handsets
EV_DSR_LINE_SETTINGS_LIST_GET_RES = 38  # result of CMBS_EV_DSR_LINE_SETTINGS_LIST_GET
EV_DSR_LINE_SETTINGS_LIST_SET = 39  # Set list of subscribed handsets
EV_DSR_LINE_SETTINGS_LIST_SET_RES = 40  # result of CMBS_EV_DSR_LINE_SETTINGS_LIST_SET

EV_DSR_RF_SUSPEND = 41  # RF Suspend on CMBS target
EV_DSR_RF_RESUME = 42  # RF Resume on CMBS target
EV_DSR_TURN_ON_NEMO = 43  # Turn On NEMo mode for the CMBS base
EV_DSR_TURN_OFF_NEMO = 44  # Turn Off NEMo mode for the CMBS base
EV_DCM_CALL_STATE = 45  # Call state
EV_DEE_CALL_ESTABLISH = 46  # Event generated on start of a new call( incoming or outgoing )
EV_DEE_CALL_PROGRESS = 47  # Events for various call progress states
EV_DEE_CALL_ANSWER = 48  # Generated when a call is answered
EV_DEE_CALL_RELEASE = 49  # Generated when a call is released
EV_DEE_CALL_RELEASECOMPLETE = 50  # Generated when call instance deleted
EV_DEE_CALL_INBANDINFO = 51  # Events created for inband keys
EV_DEE_CALL_MEDIA_OFFER = 52  # Offer media
EV_DEE_CALL_MEDIA_OFFER_RES = 53  # Response to CMBS_EV_DEE_CALL_MEDIA_OFFER
EV_DEE_CALL_MEDIA_UPDATE = 54  # Received when cordless module updated the media
EV_DEE_CALL_HOLD = 55  # Generated on call HOLD
EV_DEE_CALL_RESUME = 56  # Generated on call RESUME
EV_DEE_CALL_HOLD_RES = 57  # this is response for the call hold request.
EV_DEE_CALL_RESUME_RES = 58  # this is response for the call resume request
EV_DSR_HS_PAGE_PROGRESS = 59  # Events for various handset locator progress states
EV_DSR_HS_PAGE_ANSWER = 60  # Generated when a handset locator is answered
EV_DEM_CHANNEL_START = 61  # Start sending (voice) data on a particular channel
EV_DEM_CHANNEL_START_RES = 62  # Response to CMBS_EV_DEM_CHANNEL_START
EV_DEM_CHANNEL_INTERNAL_CONNECT = 63  # Modify the IN/Out Connection for Media Channels
EV_DEM_CHANNEL_INTERNAL_CONNECT_RES = 64  # Response to CMBS_EV_DEM_CHANNEL_INTERNAL_CONNECT
EV_DEM_CHANNEL_STOP = 65  # Stop sending data on a particular channel
EV_DEM_CHANNEL_STOP_RES = 66  # Response to CMBS_EV_DEM_CHANNEL_STOP
EV_DEM_TONE_START = 67  # Start the tone generation on a particular media channel
EV_DEM_TONE_START_RES = 68  # Response to CMBS_EV_DEM_TONE_START
EV_DEM_TONE_STOP = 69  # Stop tone generation on a particular media channel
EV_DEM_TONE_STOP_RES = 70  # Response to CMBS_EV_DEM_TONE_STOP
EV_DSR_SYS_LOG_START = 71  # Start system logging
EV_DSR_SYS_LOG_STOP = 72  # Stop system logging
EV_DSR_SYS_LOG_REQ = 73  # Request to get content of the log buffer
EV_DSR_PARAM_UPDATED = 74  # Unsolicited event from the CMBS target when a parameter was internally modified
EV_DSR_PARAM_AREA_UPDATED = 75  # Unsolicited event from the CMBS target when a parameter area was internally modified
EV_DSR_PARAM_AREA_GET = 76  # Get parameter area data
EV_DSR_PARAM_AREA_GET_RES = 77  # Response to CMBS_EV_DSR_PARAM_AREA_GET
EV_DSR_PARAM_AREA_SET = 78  # Sets / updates data in parameter area
EV_DSR_PARAM_AREA_SET_RES = 79  # Response to CMBS_EV_DSR_PARAM_AREA_SET

# CAT-iq 2 events
EV_DSR_GEN_SEND_MWI = 80  # Send Voice/SMS/Email Message Waiting Indication to one or more handsets
EV_DSR_GEN_SEND_MWI_RES = 81  # Response to CMBS_EV_DSR_GEN_SEND_MWI
EV_DSR_GEN_SEND_MISSED_CALLS = 82  # Send Missed Calls Indication to one or more handsets
EV_DSR_GEN_SEND_MISSED_CALLS_RES = 83  # Response to CMBS_EV_DSR_GEN_SEND_MISSED_CALLS
EV_DSR_GEN_SEND_LIST_CHANGED = 84  # Send List Changed event to one or more handsets
EV_DSR_GEN_SEND_LIST_CHANGED_RES = 85  # Response to CMBS_EV_DSR_GEN_SEND_LIST_CHANGED
EV_DSR_GEN_SEND_WEB_CONTENT = 86  # Send Web Content event to one or more handsets
EV_DSR_GEN_SEND_WEB_CONTENT_RES = 87  # Response to CMBS_EV_DSR_GEN_SEND_WEB_CONTENT
EV_DSR_GEN_SEND_PROP_EVENT = 88  # Send Escape to Proprietary event to one or more handsets
EV_DSR_GEN_SEND_PROP_EVENT_RES = 89  # Response to CMBS_EV_DSR_GEN_SEND_PROP_EVENT
EV_DSR_TIME_UPDATE = 90  # Send Time-Date update event to one or more handsets
EV_DSR_TIME_UPDATE_RES = 91  # Response to CMBS_EV_DSR_TIME_UPDATE
EV_DSR_TIME_INDICATION = 92  # Event received when a handset has updated its Time-Date setting
EV_DSR_HS_DATA_SESSION_OPEN = 93
EV_DSR_HS_DATA_SESSION_OPEN_RES = 94
EV_DSR_HS_DATA_SESSION_CLOSE = 95
EV_DSR_HS_DATA_SESSION_CLOSE_RES = 96
EV_DSR_HS_DATA_SEND = 97
EV_DSR_HS_DATA_SEND_RES = 98
EV_DSR_LA_SESSION_START = 99
EV_DSR_LA_SESSION_START_RES = 100
EV_DSR_LA_SESSION_END = 101
EV_DSR_LA_SESSION_END_RES = 102
EV_DSR_LA_QUERY_SUPP_ENTRY_FIELDS = 103
EV_DSR_LA_QUERY_SUPP_ENTRY_FIELDS_RES = 104
EV_DSR_LA_READ_ENTRIES = 105
EV_DSR_LA_READ_ENTRIES_RES = 106
EV_DSR_LA_SEARCH_ENTRIES = 107
EV_DSR_LA_SEARCH_ENTRIES_RES = 108
EV_DSR_LA_EDIT_ENTRY = 109
EV_DSR_LA_EDIT_ENTRY_RES = 110
EV_DSR_LA_SAVE_ENTRY = 111
EV_DSR_LA_SAVE_ENTRY_RES = 112
EV_DSR_LA_DELETE_ENTRY = 113
EV_DSR_LA_DELETE_ENTRY_RES = 114
EV_DSR_LA_DELETE_LIST = 115
EV_DSR_LA_DELETE_LIST_RES = 116
EV_DSR_LA_DATA_PACKET_RECEIVE = 117
EV_DSR_LA_DATA_PACKET_RECEIVE_RES = 118
EV_DSR_LA_DATA_PACKET_SEND = 119
EV_DSR_LA_DATA_PACKET_SEND_RES = 120
EV_DCM_CALL_TRANSFER = 121
EV_DCM_CALL_TRANSFER_RES = 122
EV_DCM_CALL_CONFERENCE = 123
EV_DCM_CALL_CONFERENCE_RES = 124
EV_DSR_TARGET_UP = 125
EV_DEE_HANDSET_LINK_RELEASE = 126
EV_DSR_GPIO_CONNECT = 127  # Request to connect GPIO for RXTUN
EV_DSR_GPIO_CONNECT_RES = 128  # Response to CMBS_EV_DSR_GPIO_CONNECT
EV_DSR_GPIO_DISCONNECT = 129  # Request to disconnect and restore old configuration for RXTUN
EV_DSR_GPIO_DISCONNECT_RES = 130  # Response to CMBS_EV_DSR_GPIO_DISCONNECT
EV_DSR_ATE_TEST_START = 131  # Request to start ATE test
EV_DSR_ATE_TEST_START_RES = 132  # Response to CMBS_EV_DSR_ATE_TEST_START
EV_DSR_ATE_TEST_LEAVE = 133  # Request to leave ATE test
EV_DSR_ATE_TEST_LEAVE_RES = 134  # Response to CMBS_EV_DSR_ATE_TEST_LEAVE
EV_DSR_SUOTA_HS_VERSION_RECEIVED = 135  # HS version message was received from HS
EV_DSR_SUOTA_URL_RECEIVED = 136  # URL message was received from HS
EV_DSR_SUOTA_NACK_RECEIVED = 137  # NACK indication message was received from HS
EV_DSR_SUOTA_SEND_SW_UPD_IND = 138  # Host requested to send SW update IND
EV_DSR_SUOTA_SEND_SW_UPD_IND_RES = 139  # Response to CMBS_EV_DSR_SUOTA_SEND_SW_UPD_IND
EV_DSR_SUOTA_SEND_VERS_AVAIL = 140  # Host requested to send Version available IND
EV_DSR_SUOTA_SEND_VERS_AVAIL_RES = 141  # Response CMBS_EV_DSR_SUOTA_SEND_VERS_AVAIL
EV_DSR_SUOTA_SEND_URL = 142  # Host requested to send URL message
EV_DSR_SUOTA_SEND_URL_RES = 143  # Response to CMBS_EV_DSR_SUOTA_SEND_URL
EV_DSR_SUOTA_SEND_NACK = 144  # Host requested to send NACK message
EV_DSR_SUOTA_SEND_NACK_RES = 145  # Responce to CMBS_EV_DSR_SUOTA_SEND_NACK
EV_DSR_TARGET_LIST_CHANGE_NOTIF = 146  # Target notifies Host on a change in LA list maintained on Target side
EV_DSR_HW_VERSION_GET = 147  # Gets the HW version of a particular module
EV_DSR_HW_VERSION_GET_RES = 148  # Response to CMBS_EV_DSR_HW_VERSION_GET
EV_DSR_DECT_SETTINGS_LIST_GET = 149  # Get DECT Settings
EV_DSR_DECT_SETTINGS_LIST_GET_RES = 150  # result of CMBS_EV_DSR_DECT_SETTINGS_LIST_GET
EV_DSR_DECT_SETTINGS_LIST_SET = 151  # Set DECT Settings
EV_DSR_DECT_SETTINGS_LIST_SET_RES = 152  # result of CMBS_EV_DSR_DECT_SETTINGS_LIST_SET

# RTP Extension
EV_RTP_SESSION_START = 153  # Start a RTP session
EV_RTP_SESSION_START_RES = 154  # Response to CMBS_EV_RTP_SESSION_START
EV_RTP_SESSION_STOP = 155  # Stop an ongoing RTP session
EV_RTP_SESSION_STOP_RES = 156  # Response to CMBS_EV_RTP_SESSION_STOP
EV_RTP_SESSION_UPDATE = 157  # Update a RTP session
EV_RTP_SESSION_UPDATE_RES = 158  # Response to CMBS_EV_RTP_SESSION_UPDATE
EV_RTCP_SESSION_START = 159  # Start a RTCP session
EV_RTCP_SESSION_START_RES = 160  # Response to CMBS_EV_RTCP_SESSION_START
EV_RTCP_SESSION_STOP = 161  # Stop an ongoing RTCP session
EV_RTCP_SESSION_STOP_RES = 162  # Response to CMBS_EV_RTCP_SESSION_STOP
EV_RTP_SEND_DTMF = 163  # Send a RTP DTMF
EV_RTP_SEND_DTMF_RES = 164  # Response to CMBS_EV_RTP_SEND_DTMF
EV_RTP_DTMF_NOTIFICATION = 165  # Signaling out of band DTMF event
EV_RTP_FAX_TONE_NOTIFICATION = 166  # Signaling out of detected fax tone
EV_RTP_ENABLE_FAX_AUDIO_PROCESSING_MODE = 167  # Disable Audio Processing during a fax session
EV_RTP_ENABLE_FAX_AUDIO_PROCESSING_MODE_RES = 168  # Response to CMBS_EV_RTP_ENABLE_FAX_AUDIO_PROCESSING_MODE
EV_RTP_DISABLE_FAX_AUDIO_PROCESSING_MODE = 169  # Resume Audio Processing after a fax session
EV_RTP_DISABLE_FAX_AUDIO_PROCESSING_MODE_RES = 170  # Response to CMBS_EV_RTP_DISABLE_FAX_AUDIO_PROCESSING_MODE

EV_DCM_INTERNAL_TRANSFER = 171  # Informative event to host - internal transfer performed
EV_DSR_ADD_EXTENSION = 172  # Add new extension (FXS) to internal names list
EV_DSR_ADD_EXTENSION_RES = 173  # Add new extension (FXS) to internal names list
EV_DSR_RESERVED = 174
EV_DSR_RESERVED_RES = 175
EV_DSR_LA_ADD_TO_SUPP_LISTS = 176  # Add non-mandatory LiA List ID to List of supported lists
EV_DSR_LA_ADD_TO_SUPP_LISTS_RES = 177  # response to CMBS_EV_DSR_LA_ADD_TO_SUPP_LISTS
EV_DSR_LA_PROP_CMD = 178  # PP Performs proprietary List Access command
EV_DSR_LA_PROP_CMD_RES = 179  # FP responds to proprietary List Access command
EV_DSR_ENCRYPT_DISABLE = 180  # Disables encryption on the base station
EV_DSR_ENCRYPT_ENABLE = 181  # Enables encryption on the base station
EV_DSR_SET_BASE_NAME = 182  # Set new base name
EV_DSR_SET_BASE_NAME_RES = 183  # Set new base name responce
EV_DSR_FIXED_CARRIER = 184  # Set a fixed frequency for test
EV_DSR_FIXED_CARRIER_RES = 185  # Set a fixed frequency for test - response
EV_DEE_HS_CODEC_CFM_FAILED = 186
EV_DSR_EEPROM_SIZE_GET = 187  # Get eeprom size
EV_DSR_EEPROM_SIZE_GET_RES = 188  # Get eeprom size - response
EV_DSR_RECONNECT_REQ = 189  # Reconnect request from target to host
EV_DSR_RECONNECT_RES = 190  # Reconnect request responce
EV_DSR_GET_BASE_NAME = 191
EV_DSR_GET_BASE_NAME_RES = 192
EV_DSR_EEPROM_VERSION_GET = 193  # Gets the base's current EEPROM version of a particular module
EV_DSR_EEPROM_VERSION_GET_RES = 194  # Response to CMBS_EV_DSR_EEPROM_VERSION_GET
EV_DSR_START_DECT_LOGGER = 195
EV_DSR_START_DECT_LOGGER_RES = 196
EV_DSR_STOP_AND_READ_DECT_LOGGER = 197
EV_DSR_STOP_AND_READ_DECT_LOGGER_RES = 198
EV_DSR_DECT_DATA_IND = 199
EV_DSR_DECT_DATA_IND_RES = 200
EV_DSR_DC_SESSION_START = 201  # Start a Data Call Session
EV_DSR_DC_SESSION_START_RES = 202  # PP responds to Data Call start command
EV_DSR_DC_SESSION_STOP = 203  # PP responds to Data Call stop command
EV_DSR_DC_SESSION_STOP_RES = 204  # Stop a Data Call Session
EV_DSR_DC_DATA_SEND = 205  # Send data in the Data Call Session
EV_DSR_DC_DATA_SEND_RES = 206  # Send data in the Data Call Session response

EV_DSR_PING = 207  # Ping request
EV_DSR_PING_RES = 208  # Ping response

EV_DSR_SUOTA_SESSION_CREATE = 209  # GMEP_US_SESSION_CREATE
EV_DSR_SUOTA_SESSION_CREATE_ACK = 210  # GMEP_US_SESSION_CREATE_ACK
EV_DSR_SUOTA_OPEN_SESSION = 211  # GMEP_US_OPEN_SESSION
EV_DSR_SUOTA_OPEN_SESSION_ACK = 212  # GMEP_US_OPEN_SESSION_ACK
EV_DSR_SUOTA_DATA_SEND = 213  # GMEP_US_DATA_SEND
EV_DSR_SUOTA_DATA_SEND_ACK = 214  # GMEP_US_DATA_SEND_ACK
EV_DSR_SUOTA_REG_CPLANE_CB = 215  # GMEP_US_REG_CPLANE_CB
EV_DSR_SUOTA_REG_CPLANE_CB_ACK = 216  # GMEP_US_REG_CPLANE_CB_ACK
EV_DSR_SUOTA_REG_APP_CB = 217  # GMEP_US_REG_APP_CB
EV_DSR_SUOTA_REG_APP_CB_ACK = 218  # GMEP_US_REG_APP_CB_ACK
EV_DSR_SUOTA_DATA_RECV = 219  # GMEP_US_DATA_RECV
EV_DSR_SUOTA_DATA_RECV_ACK = 220  # GMEP_US_DATA_RECV_ACK
EV_DSR_SUOTA_HS_VER_IND_ACK = 221  # GMEP_US_HS_VER_IND_ACK
EV_DSR_SUOTA_SESSION_CLOSE = 222  # GMEP_US_SESSION_CLOSE
EV_DSR_SUOTA_SESSION_CLOSE_ACK = 223  # GMEP_US_SESSION_CLOSE_ACK
EV_DSR_SUOTA_CONTROL_SET = 224  # GMEP_US_CONTROL_SET
EV_DSR_SUOTA_CONTROL_SET_ACK = 225  # GMEP_US_CONTROL_SET_ACK
EV_DSR_SUOTA_COTROL_RESET = 226  # GMEP_US_COTROL_RESET
EV_DSR_SUOTA_COTROL_RESET_ACK = 227  # GMEP_US_COTROL_RESET_ACK
EV_DSR_SUOTA_UPDATE_OPTIONAL_GRP = 228  # GMEP_US_UPDATE_OPTIONAL_GRP
EV_DSR_SUOTA_UPDATE_OPTIONAL_GRP_ACK = 229  # GMEP_US_UPDATE_OPTIONAL_GRP_ACK
EV_DSR_SUOTA_FACILITY_CB = 230  # GMEP_US_FACILITY_CB
EV_DSR_SUOTA_PUSH_MODE = 231  # GMEP_US_PUSH_MODE
EV_DSR_SUOTA_UPLANE_COMMANDS_END = 232

EV_DSR_HS_PROP_EVENT = 233  # Handset proprietary event
EV_DSR_FW_APP_INVALIDATE = 234  # Special event to invalidate the application before FW upgrade
EV_DSR_FW_APP_INVALIDATE_RES = 235  # Response to the invalidate event

EV_DSR_AFE_ENDPOINT_CONNECT = 236  # Notify target which two end points need to be connected
EV_DSR_AFE_ENDPOINT_CONNECT_RES = 237  # Response to CMBS_EV_DSR_AFE_ENDPOINT_CONNECT
EV_DSR_AFE_ENDPOINT_ENABLE = 238  # To enable the path between two endpoints
EV_DSR_AFE_ENDPOINT_ENABLE_RES = 239  # Response to CMBS_EV_DSR_AFE_ENDPOINT_ENABLE
EV_DSR_AFE_ENDPOINT_DISABLE = 240  # To disable the path between two endpoints
EV_DSR_AFE_ENDPOINT_DISABLE_RES = 241  # Response to CMBS_EV_DSR_AFE_ENDPOINT_DISABLE
EV_DSR_AFE_ENDPOINT_GAIN = 242  # Define the gain to AFE end point
EV_DSR_AFE_ENDPOINT_GAIN_RES = 243  # Response to CMBS_EV_DSR_AFE_ENDPOINT_GAIN
EV_DSR_AFE_AUX_MEASUREMENT = 244  # Define: input, define manually measure/via BMP, activate measurement.
EV_DSR_AFE_AUX_MEASUREMENT_RES = 245  # Response to CMBS_EV_DSR_AFE_AUX_MEASUREMENT
EV_DSR_AFE_AUX_MEASUREMENT_RESULT = 246  # An event from CMBS indicating the AUX measurement result
EV_DSR_AFE_CHANNEL_ALLOCATE = 247  # Allocate AFE Channel and codec
EV_DSR_AFE_CHANNEL_ALLOCATE_RES = 248  # Response to resource allocation request, includes channel and codec or error
EV_DSR_AFE_CHANNEL_DEALLOCATE = 249  # Free AFE Channel and codec
EV_DSR_AFE_CHANNEL_DEALLOCATE_RES = 250  # Response to resource de-allocation request
EV_DSR_DHSG_SEND_BYTE = 251  # Send byte to the DSHG
EV_DSR_DHSG_SEND_BYTE_RES = 252  # Response to send byte to the DSHG
EV_DSR_DHSG_NEW_DATA_RCV = 253  # Unsolicited event to pass the DSHG data to the host
EV_DSR_GPIO_ENABLE = 254  # Enable specifyed GPIO
EV_DSR_GPIO_ENABLE_RES = 255  # Response to enable specifyed GPIO
EV_DSR_GPIO_DISABLE = 256  # Disable specifyed GPIO
EV_DSR_GPIO_DISABLE_RES = 257  # Response to disable specifyed GPIO
EV_DSR_GPIO_CONFIG_SET = 258  # Configure following parameters of specifyed GPIO: OUT/IN, SET/RESET, PULLUP/DOWN
EV_DSR_GPIO_CONFIG_SET_RES = 259  # Response to config specifyed GPIO
EV_DSR_GPIO_CONFIG_GET = 260  # Get current configuration of specifyed GPIO
EV_DSR_GPIO_CONFIG_GET_RES = 261  # Response to get current configuration of specifyed GPIO
EV_DSR_TURN_ON_NEMO_RES = 262  # Response to NEMO TURN ON event
EV_DSR_TURN_OFF_NEMO_RES = 263  # Response to NEMO TURN OFF event
EV_DSR_EXT_INT_CONFIG = 264  # Request to configure an external interrupt (GPIO INT)
EV_DSR_EXT_INT_CONFIG_RES = 265  # Response to external INT configuration
EV_DSR_EXT_INT_ENABLE = 266  # Request to enable external interrupt
EV_DSR_EXT_INT_ENABLE_RES = 267  # Reesponse to external interrupt enable
EV_DSR_EXT_INT_DISABLE = 268  # Request to disable external interrupt
EV_DSR_EXT_INT_DISABLE_RES = 269  # Reesponse to external interrupt disable
EV_DSR_EXT_INT_INDICATION = 270  # Unsolicited event indicating external interrupt occured (on GPIO)
EV_DSR_LOCATE_SUGGEST_REQ = 271  # Enforce the HS to perform locate
EV_DSR_LOCATE_SUGGEST_RES = 272  # Responce to CMBS_EV_DSR_LOCATE_SUGGEST_REQ
EV_DSR_TERMINAL_CAPABILITIES_IND = 273  # Transfer to Host termonal capabilities of a HS
EV_DSR_HS_PROP_DATA_RCV_IND = 274  # Indication that proprietary data received
EV_CHECKSUM_FAILURE = 275  # indicates a checksum error to thesending side of an event
EV_DSR_HS_REGISTRATION_IN_PROGRESS = 276  # indicates HS registration is started
EV_DSR_HS_DEREGISTRATION_IN_PROGRESS = 277  # indicates HS de-registration is started
EV_DEE_MERGE_CALLS = 278  # Generated on merge calls
EV_DEE_MERGE_CALLS_RES = 279  # this is response for the merge calls request

EV_DEM_TDM_LOOPBACK_START = 280  # TDM loopback start
EV_DEM_TDM_LOOPBACK_START_RES = 281  # TDM loopback start response
EV_DEM_TDM_LOOPBACK_STOP = 282  # TDM loopback stop
EV_DEM_TDM_LOOPBACK_STOP_RES = 283  # TDM loopback stop response

EV_DSR_GEN_SEND_LINE_USE_STATUS_IND = 284  # Send Line Use Notification
EV_DSR_GEN_SEND_HS_USE_STATUS_IND = 285  # Send HS Use Notification
EV_DSR_GEN_SEND_DIAGNOSTIC_STATUS_IND = 286  # Send Diagnostic Status Notification
EV_DSR_GEN_LINE_USE_STATUS_RES = 287  # Response to Line Use Notification
EV_DSR_GEN_HS_USE_STATUS_RES = 288  # Response to HS Use Notification
EV_DSR_GEN_DIAGNOSTIC_STATUS_RES = 289  # Response to Diagnostic Status Notification

EV_DSR_DTAM_START_SESSION = 290  # Request to start DTAM session
EV_DSR_DTAM_START_SESSION_CFM = 291  # Confirm or Reject DTAM session
EV_DSR_DTAM_COMMAND_NACK = 292  # Negative response to previous DTAM command
EV_DSR_DTAM_STATUS = 293  # Command to inform the PP of the current command status
EV_DSR_DTAM_SELECT_NEIGHBOUR_MESSAGE = 294  # Allows the PP to select the next or previous message
EV_DSR_DTAM_SELECT_NEIGHBOUR_MESSAGE_CFM = 295  # Confirms the next or previous DTAM message selection
EV_DSR_DTAM_PLAY_MESSAGE = 296  # Play DTAM message
EV_DSR_DTAM_PLAY_MESSAGE_CFM = 297  # Play DTAM message confirm
EV_DSR_DTAM_DELETE_MESSAGE = 298  # Delete indicated DTAM incoming message or welcome message
EV_DSR_DTAM_DELETE_MESSAGE_CFM = 299  # Confirm to Delete indicated DTAM incoming message or welcome message
EV_DSR_DTAM_PAUSE_RESUME_MESSAGE = 300  # Request that the current playing of a message be paused or resumed
EV_DSR_DTAM_PAUSE_RESUME_MESSAGE_CFM = 301  # Confirm request that the current playing of a message be paused or resumed
EV_DSR_DTAM_STOP_MESSAGE_PLAY = 302  # Request from the PP that the current playing of message will be stopped
EV_DSR_DTAM_STOP_MESSAGE_PLAY_CFM = 303  # Confirm request from the PP that the current playing of message will be stopped
EV_DSR_DTAM_RECORD_WELCOME_MESSAGE = 304  # Request the recording of a new welcome message at the indicated welcome message index
EV_DSR_DTAM_RECORD_WELCOME_MESSAGE_CFM = 305  # Confirm request the recording of a new welcome message at the indicated welcome message index
EV_DSR_DTAM_RECORD_WELCOME_MESSAGE_STOP = 306  # Indicate to the DTAM that recording shall be terminated
EV_DSR_DTAM_RECORD_WELCOME_MESSAGE_STOP_CFM = 307  # Confirmation from the DTAM that recording shall be terminated

EV_DSR_GEN_SEND_SMS_MSG_NOTIFICATION = 308  # SMS message notification
EV_DSR_GEN_SEND_SMS_MSG_NOTIFICATION_RES = 309  # Response to SMS message notification

EV_DSR_HS_RSSI_REQ = 310  # Request for RSSI value of a specific HS
EV_DSR_HS_RSSI_RES = 311  # Response for RSSI value of a specific HS

EV_DSR_CALL_STATE_SET_FILTER = 312  # Setting filter to send only specific call states to the Host
EV_DSR_CALL_STATE_SET_FILTER_RES = 313  # Indicates if the filter for call states was set successfully

EV_DSR_SLIC_LINE_TEST_REQ = 314  # Request to start SLIC Line Tests
EV_DSR_SLIC_LINE_TEST_RES = 315  # Reesponse to SLIC Line Tests with results
EV_DSR_SLIC_LINE_TEST_STOP_REQ = 316  # Request to stop the SLIC Line Tests
EV_DSR_SLIC_LINE_TEST_STOP_RES = 317  # Reesponse to stop request of SLIC Line tests
EV_DEE_FXS_EVENT = 318  # FXS Simulation Event
EV_DSR_FXS_STATUS_REQ = 319  # Request to get FXS channel status
EV_DSR_FXS_STATUS_RES = 320  # Response FXS channel status request
EV_DEE_CALL_EMERGENCY_RELEASE = 321  # Generated by host when Emergency call is finally disconnected from network
EV_DEE_CALL_EMERGENCY_RELEASE_RES = 322  # Response by target after Emergency call is finally disconnected
EV_DSR_FXS_STATUS_IND = 323  # Indication of Offhook / Onhook of FXS
EV_DSR_FXS_RING_TEST_START_REQ = 324  # Request to start Ring Test
EV_DSR_FXS_RING_TEST_START_RES = 325  # Reesponse to start Ring Test
EV_DSR_FXS_RING_TEST_STOP_REQ = 326  # Request to stop Ring Test
EV_DSR_FXS_RING_TEST_STOP_RES = 327  # Reesponse to stop Ring Test
EV_DSR_FXS_OPEN_LOOP = 328  # Open Loop to FXS port

EV_DSR_SUOTA_UNREG_APP_CB = 329  # GMEP_US_UNREG_APP_CB
EV_DSR_SUOTA_UNREG_APP_CB_ACK = 330  # GMEP_US_UNREG_APP_CB_ACK

EV_DSR_JEDEC_ID_GET = 331  # Get JEDEC information
EV_DSR_JEDEC_ID_GET_RES = 332  # Response by target with information about JEDEC ID

EV_DSR_EEPROM_EXTENDED_SIZE_GET = 333  # Get eeprom size - used for systems with EEPROM size > 64KB
EV_DSR_EEPROM_EXTENDED_SIZE_GET_RES = 334  # Get eeprom size - response
EV_DSR_GEN_SEND_PROP_EVENT_NOTIFY = 335  # Send Escape to Proprietary event to one or more handsets
EV_DSR_GEN_SEND_PROP_EVENT_NOTIFY_RES = 336  # Response to CMBS_EV_DSR_GEN_SEND_PROP_EVENT

EV_DSR_GET_LIST_OF_ACTIVE_CALLS = 337  # Host reads call info table from Target
EV_DSR_GET_LIST_OF_ACTIVE_CALLS_RES = 338

EV_DEM_CONFIG_TDM_SLOTS = 339  # TDM Slots Configuration
EV_DEM_CONFIG_TDM_SLOTS_RES = 340  # Response for CMBS_EV_DEM_CONFIG_TDM_SLOTS

EV_DSR_SLIC_NLT_CAP_TEST_START_REQ = 341  # Request to start SLIC NLT Capacitance Tests
EV_DSR_SLIC_NLT_CAP_TEST_START_RES = 342  # Reesponse to SLIC NLT Capacitance Tests with results
EV_DSR_SLIC_NLT_CAP_TEST_STOP_REQ = 343  # Request to stop the SLIC NLT Capacitance Tests
EV_DSR_SLIC_NLT_CAP_TEST_STOP_RES = 344  # Reesponse to stop request of SLIC NLT Capacitance tests

EV_DSR_NO_VALID_DATA_TIME_AVAIL = 345  # Event received when a handset send no valid Time-Date available as response for Date-Time request

EV_DEE_CALL_SCREENING = 346  # Generated on call screening Accept / Intercept

EV_DSR_LA_READ_SELECTED_ENTRIES = 347
EV_DSR_LA_READ_SELECTED_ENTRIES_RES = 348

EV_DSR_LA_WRITE_ENTRY = 349
EV_DSR_LA_WRITE_ENTRY_RES = 350
EV_DEE_CALL_DEFLECTION = 351  # Generated on call deflection
EV_DEE_CALL_DEFLECTION_RES = 352  # Generated on call deflection response
EV_DSR_NO_OF_HS_REPSUB = 353
EV_DSR_NO_OF_HS_REPSUB_RES = 354

EV_DSR_DSP_PARAM_SET = 355  # Set DSP module parameters
EV_DSR_DSP_PARAM_SET_RES = 356  # Response to set DSP module param
EV_DEE_ANS_FXS_CALL = 357  # Answer FXS Call
EV_DEE_ANS_FXS_CALL_RES = 358  # FXS Call Answer Response
EV_DEE_SPK_OG_CALL = 359  # SPK Make OG Call
EV_DEE_SPK_OG_CALL_RES = 360  # Response for SPK OG call

# Crash events will start at 0x2900
EV_DSR_CRASH_DUMP_CONFIG = 0x2900  # CMBS CRASH DUMP Configuration command
EV_DSR_CRASH_DUMP_CONFIG_RES = 0x2901  # CMBS CRASH DUMP Configuration command Response
EV_DSR_CRASH_DUMP_START = 0x2902  # CMBS CRASH DUMP start command from Host
EV_DSR_CRASH_DUMP_START_RES = 0x2903  # Response for CMBS CRASH DUMP start command
EV_DSR_CRASH_DUMP_PACKETSEND = 0x2904  # CMBS CRASH DUMP packet send command to Host
EV_DSR_CRASH_DUMP_PACKETSEND_RE = 0x2905  # Response from host to CMBS CRASH DUMP packet send command
EV_DSR_CRASH_DUMP_END = 0x2906  # CMBS CRASH DUMP End command from Host
EV_DSR_CRASH_DUMP_END_RES = 0x2907  # Response for CMBS CRASH DUMP End command
EV_DSR_CRASH_DUMP_READ_CONFIG = 0x2908  # CMBS CRASH DUMP Read Configuration command
EV_DSR_CRASH_DUMP_READ_CONFIG_RES = 0x2909  # Response for CMBS CRASH DUMP Read Configuration command

EV_DSR_PNCAP_DATA_TX = 0x2950  # Carries PNCAP ETP from HOST ==> Target
EV_DSR_PNCAP_DATA_TX_RES = 0x2951  # Carries PNCAP ETP from HOST ==> Target
EV_DSR_PNCAP_DATA_RX = 0x2952  # Carries PNCAP ETP from Target ==> HOST

EV_DSR_HAN_DEFINED_START = 0x3000  # Home Area Network start of values
EV_DSR_HAN_DEFINED_END = 0x3FFF  # Home Area Network end of values

EV_DSR_USER_DEFINED_START = 0x4000  # User defined start of values
EV_INFO_SUGGEST = 0x4001  # send MM-INFO-SUGGEST
EV_INFO_SUGGEST_RES = 0x4002  # response to MM-INFO-SUGGEST

EV_DSR_USER_DEFINED_END = EV_DSR_USER_DEFINED_START + 9  # User defined end of values


#
# Parameters
#

PARAM_UNKNOWN = 0
PARAM_RFPI = 1  # Base identity
PARAM_RVBG = 2  # VBG register
PARAM_RVREF = 3  # VREF register
PARAM_RXTUN = 4  # RTUN register
PARAM_MASTER_PIN = 5  # Base master PIN code
PARAM_AUTH_PIN = 6  # Authentication PIN code
PARAM_COUNTRY = 7  # Configure cordless module to specific country settings
PARAM_SIGNALTONE_DEFAULT = 8  # Define the default behavior for outgoing calls
PARAM_TEST_MODE = 9  # Test mode. 0x00: Normal operation; 0x81: TBR6; 0x82: TBR10 (see E_CMBS_TEST_MODE)
PARAM_ECO_MODE = 10  # Eco mode. See E_CMBS_ECO_MODE_TYPE values
PARAM_AUTO_REGISTER = 11  # Automatic registration
PARAM_NTP = 12  # NTP
PARAM_GFSK = 13  # Gaussian frequency shift keying calibration
PARAM_RESET_ALL = 14  # Reset complete parameter area (EEprom) to default settings, RFPI and tuning parameter are kept
PARAM_RESERVED = 15  # Reserved >
PARAM_SUBS_DATA = 16  # Returns subscription data of CMBS (this data contains alll the registered HS>
PARAM_AUXBGPROG = 17  # BG Calibrate
PARAM_ADC_MEASUREMENT = 18  # ADCMeasurement
PARAM_PMU_MEASUREMENT = 19  # PMUMeasurement
PARAM_RSSI_VALUE = 20  # RSSI value
PARAM_DECT_TYPE = 21  # DECT type (EU, Japan , US). See E_CMBS_DECT_TYPE values;
PARAM_MAX_NUM_ACT_CALLS_PT = 22  # Maximum number of active calls of 1 PT.
PARAM_ANT_SWITCH_MASK = 23  # ANTENNA select in TBR6 Test Mode. 0x0: null, 0x1: antenna 0; 0x2: antenna 1; 0x3: antenna 0 && 1
PARAM_PORBGCFG = 24  # PORBGCFG
PARAM_AUXBGPROG_DIRECT = 25  # AUXBGPROG set in EEPROM without calibration
PARAM_BERFER_VALUE = 26  # BER-FER value
PARAM_FP_CUSTOM_FEATURES = 27  # Internal Call disable/enable
PARAM_INBAND_COUNTRY = 28  # country selection for inband tones, e.g. Default, French, polish and swiss
PARAM_HAN_DECT_SUB_DB_START = 29  # HAN DECT subscription data base start address
PARAM_HAN_DECT_SUB_DB_END = 30  # HAN DECT subscription data base end address
PARAM_HAN_ULE_SUB_DB_START = 31  # HAN ULE  subscription data base start address
PARAM_HAN_ULE_SUB_DB_END = 32  # HAN ULE  subscription data base end address
PARAM_HAN_FUN_SUB_DB_START = 33  # HAN FUN  subscription data base start address
PARAM_HAN_FUN_SUB_DB_END = 34  # HAN FUN  subscription data base end address
PARAM_HAN_ULE_NEXT_TPUI = 35  # HAN ULE next TPUI to be used for subscription
PARAM_DHSG_ENABLE = 36  # Enable DHSG, Initialize DHSG GPIO's
PARAM_PREAM_NORM = 37  # Enable DHSG, Initialize DHSG GPIO's
PARAM_RF_FULL_POWER = 38  # RF full power
PARAM_RF_LOW_POWER = 39  # RF Low power
PARAM_RF_LOWEST_POWER = 40  # RF Lowest power
PARAM_RF19APU_MLSE = 41  # RF19APU MLSE
PARAM_RF19APU_KCALOVR = 42  # RF19APU KCALOVR
PARAM_RF19APU_KCALOVR_LINEAR = 43  # RF19APU KCALOVR_Linear
PARAM_RF19APU_SUPPORT_FCC = 44  # RF19APU Support FCC
PARAM_RF19APU_DEVIATION = 45  # RF19APU Deviation
PARAM_RF19APU_PA2_COMP = 46  # RF19APU PA2 compatibility
PARAM_RFIC_SELECTION = 47  # RFIC Selectionr
PARAM_MAX_USABLE_RSSI = 48  # MAX usable RSSI
PARAM_LOWER_RSSI_LIMIT = 49  # Lower RSSI Limit
PARAM_PHS_SCAN_PARAM = 50  # PHS scan
PARAM_JDECT_LEVEL1_M82 = 51  # L1 - minus 82 dBm RSSI threshold for Japan regulation
PARAM_JDECT_LEVEL2_M62 = 52  # L2 - minus 62 dBm RSSI threshold for Japan regulation
PARAM_AUXBGP_DCIN = 53  # Auxiliary BG DCIN input (DCIN0/DCIN1...)
PARAM_AUXBGP_RESISTOR_FACTOR = 54  # Auxiliary BG Resistor Factor
PARAM_DAC1_VOL = 55  # DAC1 sidetone volume
PARAM_DAC2_VOL = 56  # DAC2 Sidetone volume
PARAM_INL_DEL = 57  # Delete 1, several or all handsets from internal name list
PARAM_SYPO_GPIO = 58  # Holds the number of GPIO that will be used as SYPO input
PARAM_SYPO_WAIT_FOR_SYNC = 59  # Defines if the target should wait for synchronization before starting DECT stack
PARAM_SYPO_MODE = 60  # Holds SYPO mode (slave/master/none)
PARAM_UART_DELAY_TIMER = 61  # UART delay (in resolution of 10msec, i.e if value = 2 => delay = 20msec)
PARAM_MAX_TRANSFER_SIZE = 62  # MAX transfer according to targets buffer size limitation
PARAM_IOM_TEST_MODE = 63  # IOM/PCM loopback test mode. 0x1: enabled; 0x0: disabled
PARAM_INT_START_CALL_TO_HOST = 64  # Enable or Disable route internal * call to host. 0x0: disabled; 0x1: enabled(default) Used for CatIQ2.0 certificate

PARAM_RING_ON_OFF = 65  # Enable/Disable ring on/off procedure
PARAM_NEMO_MODE = 66  # Enable/Disable the no emission mode
PARAM_HS_CW_DISABLED = 67  # Enable/Disable Call Waiting per handset
PARAM_INL_ADD = 68  # Used to ADD FXS-1 & FXS-2 only (HS-1, HS-2)
PARAM_BBD_UPDATE = 69  # Perform BBD download from Host
PARAM_CLOCK_MASTER_EDIT = 70  # Disable/Enable clock master editing by handset

PARAM_FXS_CALLEE_REGRET_TIME = 71  # Regret Timer configuration
PARAM_FXS_FIRST_DIGIT_TIMER = 72  # First digit timer configuration
PARAM_FXS_INTER_DIGIT_TIMER = 73  # Inter digit timer configuration
PARAM_FXS_STAR_HASH_CON_TIMER = 74
PARAM_PREP_QSPI_FOR_HW_RESET = 75  # Requests sending of a reset command to SST QSPI flash before performing a HW reset

PARAM_FXS_TONE_CONFIG = 76
PARAM_ENC_DISABLE = 77  # Status of subscription disable (testmode for Air Sniffer), not for product
PARAM_SUBS_DATA_EX = 78  # Returns extended subscription data of CMBS (this data contains alll the registered HS>
PARAM_HAN_FUN_GROUP_LIST_START = 79  # HAN ULE  group list start address
PARAM_HAN_FUN_GROUP_LIST_END = 80  # HAN ULE  group list end address
PARAM_HAN_FUN_GROUP_TABLE_START = 81  # HAN ULE  group table start address
PARAM_HAN_FUN_GROUP_TABLE_END = 82  # HAN ULE  group table end address
PARAM_HAN_ULE_BROADCAST_CONVERSION_TABLE_START = 83  # HAN ULE  broadcast conversion table start address
PARAM_HAN_ULE_BROADCAST_CONVERSION_TABLE_END = 84  # HAN ULE  broadcast conversion table end address
PARAM_ULE_MULTICAST_ENC_PARAMS = 85  # ULE  multicast encryption params

PARAM_NEMO_CONTROL = 86  # MSB enables HM00_NEMO_DUMMY_USE_RSSI_FOR_IF_CHECK, others are for extending NEMO WA bearer time
PARAM_REPEATER_TYPE = 87  # repeater type for enabling or disabling enhanced repeater support
PARAM_REPEATER_SUBS_START = 88  # start address of repeater subscriptiopn records, including handset records over repeater (4+4*6)
PARAM_REPEATER_SUBS_END = 89  # end address of repeater subscriptiopn records, including handset records over repeater (4+4*6)
PARAM_TEST_FLAGS = 90  # Test flags, if set: bit 0: BMP Driver Recover disabled, bit 1: Watchdog not enabled, bit 3: disable activity led, bit 7: ULE support enabled

PARAM_AREA_TYPE_EEPROM = 0
PARAM_AREA_TYPE_RAM = 1


class Message(object):
    def __init__(self, id=0, *args):
        self.id = id
        self.payload = b''
        for arg in args:
            if isinstance(arg, bytes):
                self.payload += arg
            if isinstance(arg, IE):
                self.payload += arg.pack()

    def pack(self):
        buf = struct.pack("<L", SYNC)
        buf += struct.pack("<HHHH", 8 + len(self.payload), 0, self.id, len(self.payload))
        buf += self.payload
        return buf

    def unpack(self, buf):
        (self.id, payloadlen), buf = struct.unpack("<HH", buf[8:12]), buf[12:]
        self.payload = buf[:payloadlen]
        return self

    def get_ie(self, cls):
        b = self.payload
        while len(b) > 0:
            id, length = struct.unpack("<HH", b[:4])
            if id == cls.__id__:
                break
            b = b[4+length:]
        else:
            raise IENotFoundError()

        iebuf = b[:4+length]
        ie = cls().unpack(iebuf)
        return ie

    def __str__(self):
        msgname = lookup_msg(self.id)
        payloadstr = ":".join("{:02x}".format(ord(c)) for c in self.payload)
        return "{}<{:#06x}> {}".format(msgname, self.id, payloadstr)


class IE(object):
    def __init__(self, *args):
        if args:
            if len(args) != len(self.__fields__):
                errstr = "Expected {} arguments ({} given)".format(len(self.__fields__), len(args))
                raise TypeError(errstr)
            args = list(args)
            for fieldname, _ in self.__fields__:
                setattr(self, fieldname, args.pop())

    def pack(self):
        buf = self._pack_content()
        buf = struct.pack("<HH", self.__id__, len(buf)) + buf  # prepend header
        return buf

    def _pack_content(self):
        buf = b''
        for name, fmt in self.__fields__:
            buf += struct.pack('<'+fmt, getattr(self, name))
        return buf

    def unpack(self, buf):
        (id, length), buf = struct.unpack("<HH", buf[:4]), buf[4:]
        if id != self.__id__:
            raise IEUnpackError("unexpected identifier")
        if length != len(buf):
            raise IEUnpackError("unexpected buffer length")

        self._unpack_content(buf)
        return self

    def _unpack_content(self, buf):
        for name, fmt in self.__fields__:
            fieldsize = struct.calcsize(fmt)
            setattr(self, name, struct.unpack("<"+fmt, buf[:fieldsize])[0])
            buf = buf[fieldsize:]


class IEParameter(IE):
    __id__ = 16

    def __init__(self, id=0, typ=0, data=b''):
        self.id = id
        self.type = typ
        self.data = data

    def _pack_content(self):
        buf = struct.pack("<BBH", self.id, self.type, len(self.data))
        buf += self.data
        return buf

    def _unpack_content(self, buf):
        (self.id, self.type, length), buf = struct.unpack("<BBH", buf[:4]), buf[4:]
        self.data = buf[:length]


class IEParameterArea(IE):
    __id__ = 26

    def __init__(self, typ=0, offset=0, data=b"", length=0):
        self.type = typ
        self.offset = offset
        self.data = data
        if length:
            self.length = length
        else:
            self.length = len(data)

    def _pack_content(self):
        if self.data and len(self.data) != self.length:
            raise ValueError("length does not match size of data")
        buf = struct.pack("<BLH", self.type, self.offset, self.length)
        buf += self.data
        return buf

    def _unpack_content(self, buf):
        (self.type, self.offset, self.length), buf = struct.unpack("<BLH", buf[:7]), buf[7:]
        self.data = buf[:self.length]


class IEResponse(IE):
    __id__ = 22
    __fields__ = (
        ('result', 'B'),
    )


def send(f, event, *ies):
    msg = Message(event, *ies)
    log_tx(msg)
    f.write(msg.pack())


def send_cmd(f, cmd, payload=b''):
    id = 0xff00 + cmd
    msg = Message(id, payload)
    log_tx(msg)
    f.write(msg.pack())


# CMBS packet:
#    4 byte sync: 0xdadadada
#    8 byte msghdr:
#      - u16 TotalLength = (header length + payload length)
#      - u16 PacketNr = 0
#      - u16 EventId = event id, e.g.
#      - u16 ParamLength = payload length
#   <payload>
#   (6 byte checksum)
def receive(f, timeout=0):
    buf = b''
    expire_at = time.time() + timeout
    while not timeout or expire_at > time.time():
        byt = f.read(1)
        if byt:
            buf += byt
        if len(buf) < 6:
            continue

        if buf[0:4] != b'\xda\xda\xda\xda':
            buf = buf[1:]
            continue

        (length,) = struct.unpack("<H", buf[4:6])
        if len(buf) < 4 + length:
            continue

        # we have a complete message buffer now
        break
    else:
        raise TimeoutError()

    msg = Message().unpack(buf)
    log_rx(msg)
    return msg


def wait(f, event):
    while True:
        msg = receive(f)
        if msg.id == event:
            return msg


def wait_cmd(f, cmd):
    id = 0xff00 + cmd
    return wait(f, id)


def lookup_msg(id):
    thismodule = sys.modules[__name__]

    if id > 0xff00:
        prefix = "CMD_"
        id = id - 0xff00
    else:
        prefix = "EV_"

    for attrname in dir(thismodule):
        if not attrname.startswith(prefix):
            continue

        value = getattr(thismodule, attrname)
        if id == value:
            return attrname

    return "UNKNOWN"


_logger = None


def log_tx(msg):
    if not _logger:
        return
    _logger.info("-> {}".format(msg))


def log_rx(msg):
    if not _logger:
        return
    _logger.info("<- {}".format(msg))
