# SPDX-License-Identifier: MIT
"""Wrapper for the HAN server protocol.

This module implements and slightly abstracts the HAN server protocol. It allows to build, send,
receive and parse messages. See the HAN server protocol specification for more information.

"""

# TODO:
# - add more API calls


from __future__ import print_function
#import multiprocessing
import threading
import socket
import collections
import _thread

#from multiprocessing import Process



EOL = "\r\n"
PARAM_DELIM = ": "

# EEPROM parameter name : writeable length
# Parameters marked with a length of 0 are read only, attempting to set them will
# return a status of FAIL, the actual length is in the comment next to them.
EEPROM_PARAMS = {
    "RFPI": 5,
    "RXTUN": 1,
    "RF_FULL_POWER": 1,
    "PREAM_NORM": 1,
    "RF19APU_SUPPORT_FCC": 1,
    "RF19APU_DEVIATION": 1,
    "RF19APU_PA2_COMP": 1,
    "MAX_USABLE_RSSI": 1,
    "LOWER_RSSI_LIMIT": 1,
    "PHS_SCAN_PARAM": 1,
    "JDECT_LEVEL1_M82": 1,
    "JDECT_LEVEL2_M62": 1,
    "SUBS_DATA": 250,
    "RVREF": 1,
    "GFSK": 10,
    "HAN_DECT_SUB_DB_START": 0,  # 4
    "HAN_DECT_SUB_DB_END": 0,  # 4
    "HAN_ULE_SUB_DB_START": 0,  # 4
    "HAN_ULE_SUB_DB_END": 0,  # 4
    "HAN_FUN_SUB_DB_START": 0,  # 4
    "HAN_FUN_SUB_DB_END": 0,  # 4
    "HAN_ULE_NEXT_TPUI": 3,
    "MAX_TRANSFER_SIZE": 0,  # 2
    "HAN_FUN_GROUP_LIST_START": 0,  # 4
    "HAN_FUN_GROUP_LIST_END": 0,  # 4
    "HAN_FUN_GROUP_TABLE_START": 0,  # 4
    "HAN_FUN_GROUP_TABLE_END": 0,  # 4
    "HAN_ULE_BROADCAST_CONVERSION_TABLE_START": 0,  # 4
    "HAN_ULE_BROADCAST_CONVERSION_TABLE_END": 0,  # 4
    "ULE_MULTICAST_ENC_PARAMS": 48,
}


def _hexstr(str):
    """convert str '16 256' to '10ff'"""
    return "".join(format(int(x), "02x") for x in str.split())


class TimeoutException(Exception):
    """Exception raised when a waiting for a response times out."""
    pass


class BlockedRxException(Exception):
    """Exception raised when a blocking API method is called from a callback."""
    pass


class Message(object):
    """Build and parse a HAN server protocol message.

    When parsing a message, this class acts as a factory going through the list of available
    subclasses and will instantiate the best fit.

    Parsing of parameters is handled in :func:`_parse_params` which can be overridden by any
    subclass. By default, all message parameters are parsed into the params dict.

    For example, the following:

        FOO_MSG
         HELLO: World

    Gets parsed into:

        > msg.params
        {"HELLO": "World"}

    """

    def __new__(cls, data=None, service=None, name=None):
        """Factory for generating a matching response (sub)class instance from data

        Based on: https://stackoverflow.com/questions/5953759/using-a-class-new-method-as-a-factory-init-gets-called-twice
        """  # noqa

        new = super(Message, cls).__new__

        if not data:
            return new(cls)

        subclasses = cls.__subclasses__()

        msgname, params = data.split(EOL, 1)
        clsname = cls.camelcase(msgname) + "Message"

        for subclass in subclasses:
            if subclass.__name__ == clsname:
                return new(subclass)
        return new(cls)

    def __init__(self, data=None, service=None, name=None):
        """Initialize message instance. If data is supplied, parse it"""
        self.service = service
        self.name = name
        self.params = collections.OrderedDict()

        if data:
            self._parse_data(data)

    def _parse_data(self, data):
        """Parse data into .service, .name and ._params, call _parse_params()"""
        data = data.strip()

        # some messages are prefixed with a service identifier (e.g. "[HAN]"), some are not
        first, rest = data.split(EOL, 1)
        if first.startswith("["):
            self.service = first
            self.name, params = rest.split(EOL, 1)
        else:
            self.name = first
            params = rest

        if not self.service:
            self.service = "[HAN]"

        params = params.split(EOL)

        self._params = []
        for param in params:
            param = param.strip()
            try:
                key, value = param.split(PARAM_DELIM)
                value = value.strip()
            except ValueError:
                if param == "SUCCEED":
                    key, value = "SUCCEED", True
                if param == "FAIL":
                    key, value = "FAIL", True
            finally:
                self._params.append((key, value))

        self._parse_params()

    def _parse_params(self):
        """Parse ._params into .params using a more appropriate presentation (dict/classes)"""
        self.params = {}
        for key, value in self._params:
            self.params[key] = value

    def _find_param(self, name):
        for key, value in self._params:
            if key == name:
                return value
        raise KeyError("Parameter '{}' not found".format(name))

    @staticmethod
    def camelcase(str):
        """Convert a message name to camelcase (e.g. "DEV_TABLE" to "DevTable")"""
        str = str.lower()
        str = str[0].upper() + str[1:]

        while True:
            pos = str.find("_")
            if pos == -1:
                break
            str = str[:pos] + str[pos+1].upper() + str[pos+2:]

        return str

    @staticmethod
    def encode(data):
        if not isinstance(data, str):
            raise TypeError("Need type str as argument")

        return " ".join(format(ord(c), 'X') for c in data)

    def to_string(self):
        # command name should not be empty
        if not self.name:
            raise ValueError(".name needs to be set")

        if self.service:
            str = self.service + EOL
        else:
            str = "[HAN]" + EOL

        # add command name
        str += self.name + EOL

        # add params
        for key, value in self.params.items():
            str += " " + key + PARAM_DELIM + value + EOL

        # the end
        str += EOL
        return str

    def to_bytes(self):
        return bytearray(self.to_string(), "utf-8")


class OpenResMessage(Message):
    """Parse an OPEN_RES message

    Result:
        .success: True or False
    """
    def _parse_params(self):
        self.success = ("SUCCEED", True) in self._params


class CloseResMessage(Message):
    """Parse a CLOSE_RES message

    Result:
        .success: True of False
    """
    def _parse_params(self):
        self.success = ("SUCCEED", True) in self._params


class DevTableParser(object):

    class Device:
        _map = {
            "DEV_ID": ("id", int),
            "DEV_IPUI": ("ipui", _hexstr),
            "DEV_EMC": ("emc", _hexstr),
            "ULE_CAPABILITIES": ("ule_capabilities", int),
            "ULE_PROTOCOL_ID": ("ule_protocol_id", int),
            "ULE_PROTOCOL_VERSION": ("ule_protocol_version", int),
        }

        def __str__(self):
            return "Device(id={}, ipui={})".format(self.id, self.ipui)

    class Unit:
        _map = {
            "UNIT_ID": ("id", int),
            "UNIT_TYPE": ("type", int),
        }

    class Interface:
        _map = {
            "INTRF_TYPE": ("type", int),
            "INTRF_ID": ("id", int),
        }

    def _parse_params(self):
        self.index = int(self._find_param("DEV_INDEX"))

        params = self._params[2:]  # skip dev_index and no_of_devices

        self.devices = []
        while params:
            device, params = self._parse_device(params)
            self.devices.append(device)

    def _parse_device(self, params):
        device, params = self._parse_object(params, self.Device)

        key, value = params[0]
        if key != "NO_UNITS":
            raise KeyError("Unexpected paramter: {}".format(key))

        params = params[1:]
        device.units = []
        for i in range(int(value)):
            unit, params = self._parse_unit(params)
            device.units.append(unit)

        return device, params

    def _parse_unit(self, params):
        unit, params = self._parse_object(params, self.Unit)

        key, value = params[0]
        if key != "NO_OF_INTRF":
            raise KeyError("Unexpected paramter: {}".format(key))

        params = params[1:]
        unit.interfaces = []
        for i in range(int(value)):
            interface, params = self._parse_object(params, self.Interface)
            unit.interfaces.append(interface)

        return unit, params

    def _parse_object(self, params, cls):
        obj = cls()
        while params:
            key, value = params[0]

            # unknown parameter name? stop parsing
            if key not in obj._map:
                break

            dest, typ = obj._map[key]

            # try setting attribute again? stop parsing
            if hasattr(obj, dest):
                break

            setattr(obj, dest, typ(value))
            params = params[1:]

        return obj, params


class DevParser(DevTableParser):
    def _parse_params(self):
        device, _ = self._parse_device(self._params)
        self.device = device


class DevInfoPhase2Message(DevParser, Message):
    """Parse a DEV_INFO_PHASE_2 message.

    Example result:

        .device.id = <int>
        .device.ipui = <int>
        .device.units[n].id = <int>
        .device.units[n].type = <int>
        .device.units[n].interfaces[i].id = <int>
        .device.units[n].interfaces[i].type = <int>
    """
    pass


class DevTableMessage(DevTableParser, Message):
    """Parse a DEV_TABLE message.

    Example result:

        .index = <int>
        .devices[n].id = <int>
        .devices[n].ipui = <int>
        .devices[n].units[m].id = <int>
        .devices[n].units[m].type = <int>
        .devices[n].units[m].interfaces[i].id = <int>
        .devices[n].units[m].interfaces[i].type = <int>
    """
    pass


class DevTablePhase2Message(DevTableParser, Message):
    """Parse a DEV_TABLE_PHASE_2 message.

    Example result:

        .index = <int>
        .devices[n].id = <int>
        .devices[n].ipui = <int>
        .devices[n].units[m].id = <int>
        .devices[n].units[m].type = <int>
        .devices[n].units[m].interfaces[i].id = <int>
        .devices[n].units[m].interfaces[i].type = <int>
    """
    pass


class BlackListDevTableMessage(DevTableParser, Message):
    """Parse a BLACK_LIST_DEV_TABLE message.

    Example result:

        .index = <int>
        .devices[n].id = <int>
        .devices[n].ipui = <int>
        .devices[n].units[m].id = <int>
        .devices[n].units[m].type = <int>
        .devices[n].units[m].interfaces[i].id = <int>
        .devices[n].units[m].interfaces[i].type = <int>
    """
    pass


class FunMsgMessage(Message):

    def _parse_params(self):
        super(FunMsgMessage, self)._parse_params()

        datalen = int(self.params["DATALEN"])
        if datalen:
            data = self._find_param("DATA")
            self.data = bytearray([int(x, 16) for x in data.split()])
        else:
            self.data = bytearray()


class HANClient(object):
    """HAN Protocol client"""

    class Waiter(object):
        """Wait for a specific message, carry message once received."""
        def __init__(self, msgname):
            self.event = threading.Event()
            #self.event = multiprocessing.Event()
            self.msgname = msgname.upper()
            self.message = None

        # cannot subclass threading.Event() in python2, so we proxy
        # all calls to self.event
        def __getattr__(self, attr):
            return getattr(self.event, attr)

    def __init__(self):
        self._ip_address = "127.0.0.1"
        self._port = 3490
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._rxthread = threading.Thread(target=self._receive)
        self._rxthread.daemon = True
        # multiprocessing
        #self._rxthread = Process(target=self._receive)
        #self._rxthread.start()
        #self._rxthread.join()

        self._debug_print = False
        self._cookie = 0

        self.run = True

        # automatic internal handling of messages
        self._handlers = {
            # service, msgname
            ("[HAN]", "KEEP_ALIVE"): self._keep_alive_handler,
        }

        self._subscribers = {}
        self._waiter = None
        self._rx_message_processor = None

    def _receive(self):
        """Receives data from the HAN server UDP socket and handles it."""
        #while True:
        #print('_receive thread')
        while self.run:
            #print('run', self.run)

            data, _ = self._sock.recvfrom(4096)
            data_str = data.decode("utf-8")

            if self._debug_print:
                print("\n\nHAN Client <<-- HAN Server:")
                print(data_str)

            if self._rx_message_processor:
                self._rx_message_processor(self, data_str)

            msg = Message(data_str)

            # handled internally?
            handler = self._handlers.get((msg.service, msg.name))
            if handler:
                handler(msg)
                continue

            # wakeup any waiter
            if self._waiter:
                if self._waiter.msgname == msg.name:
                    self._waiter.message = msg
                    self._waiter.set()
                    self._waiter = None

            # subscribers
            if msg.name in self._subscribers:
                subscribers = self._subscribers[msg.name]
                for subscriber in subscribers:
                    subscriber(self, msg)

    def _keep_alive_handler(self, resp):
        """Send keep alive response.

        Occasionally the HAN server will probe known HAN client for their
        continued existence on the UDP port by sending KEEP_ALIVE messages.
        All active clients need to respond with KEEP_ALIVE_RES."""
        msg = Message(name="KEEP_ALIVE_RES")
        self.send(msg)

    def _check_rx_will_block(self):
        """Checks if caller will block the rx thread.

        Calls which are expecting a response to their request immediately
        must not be called from a callback as this will block the receive
        thread. If such a scenario is detected BlockedRxException is
        thrown."""
        thread = threading.current_thread()
        if thread == self._rxthread:
            raise BlockedRxException("Blocking API call from callback context will stall RX")

    @property
    def cookie(self):
        cookie = self._cookie
        self._cookie += 1
        return cookie

    def start(self):
        """Initialize HAN by sending INIT message."""
        msg = Message(name="INIT")
        msg.params["VERSION"] = "1"

        print('MESSAGE:::', msg)
        waiter = self.waiter("INIT_RES")
        self.send(msg)

        # must send first, otherwise the socket will have no address and the
        # receive will fail
        self._rxthread.start()
        waiter.wait()

    def send(self, msg):
        """Send message to the HAN server UDP socket."""
        if self._debug_print:
            print("\nHAN Client -->> HAN Server:")
            print(msg.to_string())

        self._sock.sendto(msg.to_bytes(), (self._ip_address, self._port))

    def destroy(self):
        self.run = False
        print('destroy')
        #self._rxthread.close()
        #self._rxthread.join()
        #del self
        pass

    def subscribe(self, msgname, callback):
        """Permanently subscribe callback to the specific incoming message.

        The callback will be called in the following form:
            callback(client, msg)

        Callback args:
            client: the HANClient instance which received the message
            msg: the parsed Message which was received
        """
        msgname = msgname.upper()
        if msgname not in self._subscribers:
            self._subscribers[msgname] = []
        self._subscribers[msgname].append(callback)

    def waiter(self, msgname):
        """Create and registers a waiter which will trigger once msgname is received."""
        self._waiter = self.Waiter(msgname)
        return self._waiter

    def send_and_wait(self, msg, respname):
        """Send msg to the HAN server and wait for a message with respname."""
        self._check_rx_will_block()

        waiter = self.waiter(respname)
        self.send(msg)
        if not waiter.wait(4):  # wait at most four seconds
            raise TimeoutException("Error: timed out waiting for '{}'".format(respname))
        return waiter.message

    #  Methods called by the app to do things
    # This is the API for the HAN app to talk to the HAN client
    # Begin API

    def set_rx_message_callback(self, callback):
        """Register a general handler which will be called for each received message.

        The callback will be called in the following form:
            callback(client, data_str)

        Callback args:
            client: the HANClient instance which received the message
            data_str: the raw message string as received (see CMND spec for details)
        """
        self._rx_message_processor = callback

    def get_debug_printing(self):
        return self._debug_print

    def set_debug_printing(self, control):
        """Enable or disable debug printing from within the HANClient."""
        if(control == 1):
            self._debug_print = True
        else:
            self._debug_print = False

    def open_reg(self, open_duration):
        """Open the registration time window.

        Arguments:
            open_duration: time in seconds to keep the registration window open

        Returns:
            The CLOSE_RES message received as an aswer to OPEN_REG.
        """
        msg = Message(name="OPEN_REG")
        msg.params["TIME"] = open_duration
        return self.send_and_wait(msg, "OPEN_RES")

    def close_reg(self):
        """Close the registration time window.

        Returns:
            The CLOSE_RES message received as an answer to CLOSE_REG."""
        msg = Message(name="CLOSE_REG")
        return self.send_and_wait(msg, "CLOSE_RES")

    def num_msg_in_q(self, device_id):
        """Get number of messages in message queue for given device.

        Arguments:
            device_id: device ID as string

        Returns:
            The message received as answer.
        """
        msg = Message(name="GET_NUM_OF_FUN_MSG_IN_Q", service="[DBG]")
        msg.params["DEV_ID"] = device_id
        return self.send_and_wait(msg, "GET_NUM_OF_FUN_MSG_IN_Q_RES")

    def fun_msg(self, **kwargs):
        """Send a FUN message to any device.

        Args:
            src_dev_id: source device id, device id 0 = base
            src_unit_id: source unit id
            dst_dev_id: destination device id
            dst_unit_id: destination unit id
            msg_type: message type, e.g. command or attribute request
            interface_type: 0 = server, 1 = client
            interface_id: identifier of the interface to send the message to
            interface_member: attribute index or command id
            data: data to be sent (bytes)

        Returns:
            The cookie used for sending the message (MSG_SEQ)."""
        msg = Message(name="FUN_MSG")

        cookie = self.cookie

        if "data" in kwargs:
            data = kwargs["data"]
        else:
            data = ""

        if "msg_type" in kwargs:
            msg_type = kwargs["msg_type"]
        else:
            msg_type = "1"

        msg.params["DEV_IPUI"] = str(kwargs["ipui"])
        msg.params["SRC_DEV_ID"] = str(kwargs["src_dev_id"])
        msg.params["SRC_UNIT_ID"] = str(kwargs["src_unit_id"])
        msg.params["DST_DEV_ID"] = str(kwargs["dst_dev_id"])
        msg.params["DST_UNIT_ID"] = str(kwargs["dst_unit_id"])
        msg.params["DEST_ADDRESS_TYPE"] = "0"
        msg.params["MSG_TRANSPORT"] = "0"
        msg.params["MSG_SEQ"] = str(cookie)
        msg.params["MSGTYPE"] = str(msg_type)
        msg.params["INTRF_TYPE"] = str(kwargs["interface_type"])
        msg.params["INTRF_ID"] = str(kwargs["interface_id"])
        msg.params["INTRF_MEMBER"] = str(kwargs["interface_member"])
        msg.params["DATALEN"] = str(len(data))

        if data:
            msg.params["DATA"] = msg.encode(data)

        self.send(msg)
        return cookie

    def delete_dev(self, device_id, local=False):
        """Request to delete a device from the table of registered devices.

        By default, this will perform a network delete procedure. Requesting to delete a device
        will place the device into the black list table first. Once the device makes contact with
        the base, both will do a handshake to confirm deletion. Following this, the black list
        table entry is removed and the DEV_DELETED message will be sent by the HAN serverself.

        In case of a local deletion, the DEV_DELETED message is sent as soon as the device entry
        is deleted from the table of registered devices. No handshake with the device will be
        performed and the device will continue to try contacting the base (base will reject).

        This call does not wait for a response from the base.

        Args:
            device_id: identifier of device to be deleted
            local: if True, only do a local deletion (no handshake with device)
        """
        msg = Message(name="DELETE_DEV")
        msg.params["DEV_ID"] = str(device_id)

        if local:
            msg.params["DEL_TYPE"] = "LOCAL"
        else:
            msg.params["DEL_TYPE"] = "BLACK_LIST"

        self.send(msg)

    def get_black_list_dev_table(self, index=0, count=5):
        """Get a list of blacklisted devices.

        Retrieving the list of blacklisted devices is paginated, starting at <index>, and will
        return <count> number of devices at maximum.

        Args:
            index: number of requested page/chunk
            count: maximum number of devices listed in response

        Returns:
            The parsed BLACK_LIST_DEV_TABLE response. See :class:`BlackListDevTableMessage`."""
        msg = Message(name="GET_BLACK_LIST_DEV_TABLE")
        respname = "BLACK_LIST_DEV_TABLE"

        msg.params["DEV_INDEX"] = str(index)
        msg.params["HOW_MANY"] = str(count)

        return self.send_and_wait(msg, respname)

    def get_dev_table(self, index=0, count=5, phase2=True):
        """Get a list of registered devices.

        Retrieving the list of registered devices is paginated, starting at <index>, and will
        return <count> number of devices at maximum.

        Args:
            index: number of requested page/chunk
            count: maximum number of devices listed in response

        Returns:
            The parsed DEV_TABLE response. See :class:`DevTablePhase2Message`."""
        if phase2:
            msg = Message(name="GET_DEV_TABLE_PHASE_2")
            respname = "DEV_TABLE_PHASE_2"
        else:
            msg = Message(name="GET_DEV_TABLE")
            respname = "DEV_TABLE"

        msg.params["DEV_INDEX"] = str(index)
        msg.params["HOW_MANY"] = str(count)

        return self.send_and_wait(msg, respname)

    def get_dev_info(self, device_id, phase2=True):
        """Get information for a specific device.

        Args:
            device_id: identifier of device to request info for

        Returns:
            The DEV_INFO response. See :class:`Message`."""
        if phase2:
            msg = Message(name="GET_DEV_INFO_PHASE_2")
            respname = "DEV_INFO_PHASE_2"
        else:
            msg = Message(name="GET_DEV_INFO")
            respname = "DEV_INFO"

        msg.params["DEV_ID"] = str(device_id)

        return self.send_and_wait(msg, respname)

    def call_release(self, call_id):
        """Release a voice call.

        Args:
            call_id: identifier of call to release (part of CALL_ESTABLISH_INDICATION)."""
        msg = Message(service="[CALL]", name="CALL_RELEASE")
        msg.params["CALL_ID"] = str(call_id)

        # no response
        self.send(msg)

    def get_sw_version(self):
        """Query software version information from HAN server."""
        msg = Message(service="[SRV]", name="GET_SW_VERSION")
        return self.send_and_wait(msg, "GET_SW_VERSION_RES")

    def get_hw_version(self):
        """Query hardware version information from HAN server."""
        msg = Message(service="[SRV]", name="GET_TARGET_HW_VERSION")
        return self.send_and_wait(msg, "GET_TARGET_HW_VERSION_RES")

    def get_eeprom_parameter(self, parameter):
        """Get an EEPROM parameter.

        Args:
            parameter: name of parameter to retrieve

        Returns:
            The GET_EEPROM_PARAM_RES response. See :class:`Message`."""
        msg = Message(service="[SRV]", name="GET_EEPROM_PARAM")
        msg.params["NAME"] = parameter
        return self.send_and_wait(msg, "GET_EEPROM_PARAM_RES")

    def set_eeprom_parameter(self, parameter, value):
        """Set an EEPROM parameter.

        Args:
            parameter: name of parameter to set
            value: new value for parameter

        Returns:
            The SET_EEPROM_PARAM_RES response. See :class:`Message`."""
        msg = Message(service="[SRV]", name="SET_EEPROM_PARAM")
        msg.params["NAME"] = parameter
        msg.params["DATA"] = value
        return self.send_and_wait(msg, "SET_EEPROM_PARAM_RES")
