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


# CMND message (network byte order)
#   2 byte start code = 0xdada
#   2 byte length
#   1 byte cookie
#   1 byte unit id
#   2 byte service id
#   1 byte message id
#   1 byte checksum (sum of bytes [length, message id] + payload)
#   payload

SYNC = 0xdada


SERVICE_ID_DEVICE_MANAGEMENT            = 0x0001
SERVICE_ID_IDENTIFY                     = 0x0004
SERVICE_ID_ATTRIBUTE_REPORTING          = 0x0006

# General service ids
SERVICE_ID_GENERAL                      = 0x0000
SERVICE_ID_ALERT                        = 0x0100
SERVICE_ID_TAMPER_ALERT                 = 0x0101
SERVICE_ID_DETECTOR_PROBLEM_ALERT       = 0x0102
SERVICE_ID_BATTERY                      = 0x0103
SERVICE_ID_KEEP_ALIVE                   = 0x0104
SERVICE_ID_ARM_DISARM                   = 0x0105
SERVICE_ID_ON_OFF                       = 0x0106
SERVICE_ID_FUN                          = 0x0108
SERVICE_ID_DEBUG                        = 0x0109
SERVICE_ID_KEY_PRESS                    = 0x010A

# System service Ids
SERVICE_ID_SYSTEM                       = 0x0201
SERVICE_ID_TECHNICIAN                   = 0x0202
SERVICE_ID_PARAMETERS                   = 0x0203
SERVICE_ID_SLEEP                        = 0x0204
SERVICE_ID_MANUFACTURE_CONFIGURATION    = 0x0206
SERVICE_ID_ULE_VOICE_CALL               = 0x020A
SERVICE_ID_PRODUCTION                   = 0x020B
SERVICE_ID_SUOTA                        = 0x020C
SERVICE_ID_CERTIFICATION                = 0x020D
SERVICE_ID_REMOTE_CONTROL               = 0x020E
SERVICE_ID_SUOTA_PROPRIETARY            = 0x020F  # for compatibillity with priorietary fun mess
SERVICE_ID_BROADCASTING                 = 0x0210
SERVICE_ID_UNKNOWN                      = 0xFFFF

# for reverse lookups
_service_shorthands = {
    "SYSTEM": "SYS",
    "PARAMETERS": "PARAM",
    "PRODUCTION": "PROD",
}

MSG_GENERAL_HELLO_IND                   = 0x05
MSG_GENERAL_ERROR_IND                   = 0x06
MSG_GENERAL_LINK_CFM                    = 0x07
MSG_GENERAL_GET_STATUS_REQ              = 0x08
MSG_GENERAL_GET_STATUS_RES              = 0x09
MSG_GENERAL_HELLO_REQ                   = 0x0a
MSG_GENERAL_GET_VERSION_REQ             = 0x0b
MSG_GENERAL_GET_VERSION_RES             = 0x0c
MSG_GENERAL_TRANSACTION_START_REQ       = 0x0d
MSG_GENERAL_TRANSACTION_START_CFM       = 0x0e
MSG_GENERAL_TRANSACTION_END_REQ         = 0x0f
MSG_GENERAL_TRANSACTION_END_CFM         = 0x10
MSG_GENERAL_LINK_MAINTAIN_START_REQ     = 0x11
MSG_GENERAL_LINK_MAINTAIN_START_CFM     = 0x12
MSG_GENERAL_LINK_MAINTAIN_STOP_REQ      = 0x13
MSG_GENERAL_LINK_MAINTAIN_STOP_CFM      = 0x14
MSG_GENERAL_LINK_MAINTAIN_STOPPED_IND   = 0x15

MSG_SYS_BATTERY_MEASURE_GET_REQ         = 0x01
MSG_SYS_BATTERY_MEASURE_GET_RES         = 0x02
MSG_SYS_RSSI_GET_REQ                    = 0x03
MSG_SYS_RSSI_GET_RES                    = 0x04
MSG_SYS_BATTERY_IND_ENABLE_REQ          = 0x05
MSG_SYS_BATTERY_IND_DISABLE_REQ         = 0x06
MSG_SYS_BATTERY_IND_LOW_IND             = 0x07
MSG_SYS_RESET_REQ                       = 0x08
MSG_SYS_BATTERY_END_LIFE_IND            = 0x09

MSG_PARAM_GET_REQ                       = 0x01
MSG_PARAM_GET_RES                       = 0x02
MSG_PARAM_SET_REQ                       = 0x03
MSG_PARAM_SET_RES                       = 0x04
MSG_PARAM_GET_DIRECT_REQ                = 0x05
MSG_PARAM_GET_DIRECT_RES                = 0x06
MSG_PARAM_SET_DIRECT_REQ                = 0x07
MSG_PARAM_SET_DIRECT_RES                = 0x08

MSG_PROD_START_REQ                      = 0x01  # Set production mode enable - needs a restart
MSG_PROD_END_REQ                        = 0x02  # Set production mode disable - needs a restart
MSG_PROD_CFM                            = 0x03  # Default Response to a Request
MSG_PROD_REF_CLK_TUNE_START_REQ         = 0x04  # Start 13.824MHz Reference Clock Tuning
MSG_PROD_REF_CLK_TUNE_END_REQ           = 0x05  # End 13.824MHz Reference Clock Tuning
MSG_PROD_REF_CLK_TUNE_END_RES           = 0x06  # Confirm on End Process with New Value returned
MSG_PROD_REF_CLK_TUNE_ADJ_REQ           = 0x07  # Adjust up/down request
MSG_PROD_BG_REQ                         = 0x08  # Band Gap Calibration
MSG_PROD_BG_RES                         = 0x09  # Band Gap Calibration Res with ADC and POR returned
MSG_PROD_ATE_INIT_REQ                   = 0x0A  # ATE Test initialize request
MSG_PROD_ATE_STOP_REQ                   = 0x0B  # ATE Test STOP request - for all modes ( continuous /
MSG_PROD_ATE_CONTINUOUS_START_REQ       = 0x0C  # CONTINUOUS REQUEST
MSG_PROD_ATE_RX_START_REQ               = 0x0D  # Rx Slot Test request status
MSG_PROD_ATE_RX_START_RES               = 0x0E  # Rx Slot Test req - with value measured, returned multiple ti
MSG_PROD_ATE_TX_START_REQ               = 0x0F  # Tx Slot Test request
MSG_PROD_ATE_GET_BER_FER_REQ            = 0x10  # Get BER FER Measurmenet
MSG_PROD_INIT_EEPROM_DEF_REQ            = 0x11  # Initialize eeprom to preset values ( selecting 9 onine menu
MSG_PROD_SPECIFIC_PRESET_REQ            = 0x12  # Initialize eeprom to preset values ( selecting 9 onine menu
MSG_PROD_SLEEP_REQ                      = 0x13  # Turn into hibernation mode
MSG_PROD_SET_SIMPLE_GPIO_LOW            = 0x14  # Set simple GPIO to low
MSG_PROD_SET_SIMPLE_GPIO_HIGH           = 0x15  # Set simple GPIO to high
MSG_PROD_GET_SIMPLE_GPIO_STATE          = 0x16  # Get simple GPIO's state
MSG_PROD_GET_SIMPLE_GPIO_STATE_RES      = 0x17  # Get simple GPIO's state - response
MSG_PROD_SET_ULE_GPIO_LOW               = 0x18  # Set ULE GPIO to low
MSG_PROD_SET_ULE_GPIO_HIGH              = 0x19  # Set ULE GPIO to high
MSG_PROD_GET_ULE_GPIO_STATE             = 0x1a  # Get ULE GPIO's state
MSG_PROD_GET_ULE_GPIO_STATE_RES         = 0x1b  # Get ULE GPIO's state - response
MSG_PROD_SET_ULE_GPIO_DIR_INPUT_REQ     = 0x1c  # Set ULE GPIO to input direction
MSG_PROD_RESET_EEPROM                   = 0x1d  # RESET EEPROM HAN/DECT
MSG_PROD_FW_UPDATE_REQ                  = 0x1e  # Perform FW update over UART
MSG_PROD_GPIO_LOOPBACK_TEST_REQ         = 0x1f  # GPIOs loopback test
MSG_PROD_ATE_RX_LOCKING_START_REQ       = 0x20  # RX Locking Test

PARAM_EEPROM_RXTUN                      = 0x00
PARAM_EEPROM_IPEI                       = 0x01
PARAM_EEPROM_TBR6                       = 0x02
PARAM_EEPROM_DECT_CARRIER               = 0x03
PARAM_EEPROM_PROD_ENABLE                = 0x04
PARAM_EEPROM_EXT_SLOT_TYPE              = 0x05
PARAM_EEPROM_FRIENDLY_NAME              = 0x06
PARAM_EEPROM_SW_VERISON                 = 0x07
PARAM_EEPROM_HW_VERISON                 = 0x08
PARAM_EEPROM_MANUFACTURE_NAME           = 0x09
PARAM_EEPROM_INFO_TABLE                 = 0x0a
PARAM_EEPROM_PLUGIN_MAP                 = 0x0b
PARAM_EEPROM_AUX_BG_PROG                = 0x0c
PARAM_EEPROM_POR_BG_CFG                 = 0x0d
PARAM_EEPROM_DECT_FULL_POWER            = 0x0e
PARAM_EEPROM_DECT_PA2_COMP              = 0x0f
PARAM_EEPROM_DECT_SUPPORT_FCC           = 0x10
PARAM_EEPROM_DECT_DEVIATION             = 0x11
PARAM_EEPROM_HAN_REG_RETRY_TIMEOUT      = 0x12
PARAM_EEPROM_HAN_LOCK_MAX_RETRY         = 0x13
PARAM_EEPROM_HAN_REG_PIN_CODE           = 0x14
PARAM_EEPROM_HAN_ENABLE_AUTO_REG        = 0x15
PARAM_EEPROM_HAN_SYS_OFF_USED           = 0x16
PARAM_EEPROM_HAN_INFO_LOCATION          = 0x17
PARAM_EEPROM_HAN_HBR_OSC                = 0x18
PARAM_EEPROM_HAN_RETRANSMIT_URGENT      = 0x19
PARAM_EEPROM_HAN_RETRANSMIT_NORMAL      = 0x1a
PARAM_EEPROM_HAN_PAGING_CAPS            = 0x1b
PARAM_EEPROM_HAN_MIN_SLEEP_TIME         = 0x1c
PARAM_EEPROM_HAN_PLUGIN_SUPPORTED       = 0x1d
PARAM_EEPROM_DECT_EMC                   = 0x1e
PARAM_EEPROM_RSSI_SETTINGS              = 0x1f
PARAM_EEPROM_HAN_GENERAL_FLAGS          = 0x20
PARAM_EEPROM_HAN_HANDLED_EXTERNALLY     = 0x21
PARAM_EEPROM_HAN_ACTUAL_RESPONSE_TIME   = 0x22
PARAM_EEPROM_HAN_DEVICE_ENABLE          = 0x23
PARAM_EEPROM_HAN_DEVICE_UID             = 0x24
PARAM_EEPROM_HAN_SERIAL_NUM             = 0x25
PARAM_DEFINED_HF_CORE_RELEASE_VER       = 0x26
PARAM_DEFINED_PROFILE_RELEASE_VER       = 0x27
PARAM_DEFINED_INTERFACE_RELEASE_VER     = 0x28
PARAM_EEPROM_HAN_KEEPALIVE_TIMEOUT      = 0x29
PARAM_EEPROM_REGISTRATION_STATUS        = 0x2a
PARAM_EEPROM_HAN_HIBERNATION_WATCHDOG   = 0x2b
PARAM_EEPROM_ULE_GPIO_MAPPING_EVENT     = 0x2c
PARAM_EEPROM_ATTR_REPORTING_SUPPORTED   = 0x2d
PARAM_EEPROM_HW_TYPE                    = 0x2e
PARAM_EEPROM_MULTICAST_TYPE             = 0x2f

PARAM_ADDRESS_TYPE_HAN_EEPROM           = 0x00
PARAM_ADDRESS_TYPE_RAM                  = 0x01
PARAM_ADDRESS_TYPE_DECT_EEPROM          = 0x02
PARAM_ADDRESS_TYPE_DAIF                 = 0x03


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
        buf = struct.pack("!BH", self.__id__, len(buf)) + buf
        return buf

    def _pack_content(self):
        buf = b''
        for name, fmt in self.__fields__:
            buf += struct.pack('!'+fmt, getattr(self, name))
        return buf

    def unpack(self, buf):
        (id, length), buf = struct.unpack("!BH", buf[:3]), buf[3:]
        if id != self.__id__:
            raise IEUnpackError("unexpected identifier")
        if length != len(buf):
            raise IEUnpackError("unexpected buffer length")

        self._unpack_content(buf)
        return self

    def _unpack_content(self, buf):
        for name, fmt in self.__fields__:
            fieldsize = struct.calcsize(fmt)
            setattr(self, name, struct.unpack("!"+fmt, buf[:fieldsize])[0])
            buf = buf[fieldsize:]

    def __str__(self):
        fields = []
        for name, _ in self.__fields__:
            fields.append("{}={}".format(name, getattr(self, name, None)))

        clsname = type(self).__name__
        s = "{clsname}({fields})".format(clsname=clsname, fields=", ".join(fields))
        return s


class IEResponse(IE):
    __id__ = 0x00
    __fields__ = (
        ('result', 'B'),
    )


class IEVersion(IE):
    # FIXME: implement packing
    __id__ = 0x09

    def _unpack_content(self, buf):
        (length,), buf = struct.unpack('B', buf[:1]), buf[1:]
        self.version = buf[:length]
        return self


class IEParameter(IE):
    __id__ = 0x0b

    def __init__(self, typ=0, id=0, data=b''):
        self.type = typ
        self.id = id
        self.data = data

    def _pack_content(self):
        buf = struct.pack('!BBH', self.type, self.id, len(self.data))
        buf += self.data
        return buf

    def _unpack_content(self, buf):
        (self.type, self.id, length), buf = struct.unpack("!BBH", buf[:4]), buf[4:]
        self.data = buf[:length]


class IEParameterDirect(IE):
    __id__ = 0x0c

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
        buf = struct.pack('!BLH', self.type, self.offset, self.length)
        buf += self.data
        return buf

    def _unpack_content(self, buf):
        (self.type, self.offset, length), buf = struct.unpack("!BLH", buf[:7]), buf[7:]
        self.data = buf[:length]


class IEGeneralStatus(IE):
    __id__ = 0x0d
    __fields__ = (
        ('powerup_mode', 'B'),
        ('registration_status', 'B'),
        ('eeprom_status', 'B'),
        ('device_id', 'H'),
    )


class IEU8(IE):
    __id__ = 0x1e
    __fields__ = (
        ('data', 'B'),
    )


POWERUP_MODE_NORMAL = 0
POWERUP_MODE_SAFE = 1
POWERUP_MODE_PRODUCTION = 2


class Message(object):
    def __init__(self, *args):
        self.payload = b''

        if args:
            args = list(args)
        if args:
            self.unit = args.pop(0)
        if args:
            self.service = args.pop(0)
        if args:
            self.id = args.pop(0)

        # remainder are information elements
        for ie in args:
            self.add_ie(ie)

    def pack(self):
        cookie = 104
        buf = b''
        buf += struct.pack('!H', SYNC)
        buf += struct.pack('!HBBHB', 6 + len(self.payload), cookie, self.unit, self.service, self.id)

        checksum = sum(bytearray(buf[2:9]))
        checksum += sum(bytearray(self.payload))

        buf += struct.pack('!B', checksum & 0xff)
        buf += self.payload
        return buf

    def unpack(self, buf):
        _, self.unit, self.service, self.id, checksum = struct.unpack("!BBHBB", buf[4:10])
        self.payload = buf[10:len(buf)]

        mychecksum = sum(bytearray(buf[2:9]))
        mychecksum += sum(bytearray(self.payload))
        mychecksum &= 0xff
        if checksum != mychecksum:
            raise ChecksumError()

        return self

    def get_ie(self, cls):
        b = self.payload
        while len(b) > 0:
            id, length = struct.unpack("!BH", b[:3])
            if id == cls.__id__:
                break
            b = b[3+length:]
        else:
            raise IENotFoundError()

        iebuf = b[:3+length]
        ie = cls().unpack(iebuf)
        return ie

    def add_ie(self, ie):
        self.payload += ie.pack()

    def __str__(self):
        service = lookup_service(self.service)
        message = lookup_message(service, self.id)
        return "{}<{:#06x}> {}<{:#04x}>".format(service, self.service, message, self.id)


def send(f, unit, service, message, *ies):
    msg = Message(unit, service, message, *ies)
    log_tx(msg)
    f.write(msg.pack())


def receive(f, timeout=0):
    buf = b''
    expire_at = time.time() + timeout
    while not timeout or expire_at > time.time():
        byt = f.read(1)
        if byt:
            buf += byt
        if len(buf) < 4:
            continue

        if buf[0:2] != b'\xda\xda':
            buf = buf[1:]
            continue

        (length,) = struct.unpack("!H", buf[2:4])
        if len(buf) < 4 + length:
            continue

        # we have a complete message buffer now
        break
    else:
        raise TimeoutError()

    msg = Message().unpack(buf)
    log_rx(msg)
    return msg


def wait(f, service, cmd):
    while True:
        msg = receive(f)
        if msg.service == service and msg.id == cmd:
            return msg


def lookup_service(id):
    thismodule = sys.modules[__name__]
    prefix = "SERVICE_ID_"
    for attrname in dir(thismodule):
        if not attrname.startswith(prefix):
            continue

        value = getattr(thismodule, attrname)
        if id == value:
            return attrname[len(prefix):]

    return "UNKNOWN"


def lookup_message(service, id):
    thismodule = sys.modules[__name__]
    service = _service_shorthands.get(service, service)
    prefix = "MSG_" + service + "_"
    for attrname in dir(thismodule):
        if not attrname.startswith(prefix):
            continue

        value = getattr(thismodule, attrname)
        if id == value:
            return attrname[len(prefix):]

    return "UNKNOWN"


_logger = None


def set_logger(logger):
    global _logger
    _logger = logger


def log_tx(msg):
    if not _logger:
        return
    _logger.info("-> {}".format(msg))


def log_rx(msg):
    if not _logger:
        return
    _logger.info("<- {}".format(msg))
