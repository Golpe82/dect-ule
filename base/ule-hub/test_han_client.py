# SPDX-License-Identifier: MIT
import unittest
import han_client

INIT_RESPONSE = """
INIT_RES
 VERSION: 1

"""[1:].replace("\n", han_client.EOL)

OPEN_REG_RESPONSE = """
OPEN_RES
 SUCCEED

"""[1:].replace("\n", han_client.EOL)

DEV_INFO_PHASE_2_RESPONSE = """
DEV_INFO_PHASE_2
 DEV_ID:  7
 DEV_IPUI:  2 195 192 80 126
 DEV_EMC:  60 44
 ULE_CAPABILITIES: 1
 ULE_PROTOCOL_ID: 1
 ULE_PROTOCOL_VERSION: 0
 NO_UNITS: 3
 UNIT_ID:  0
 UNIT_TYPE:  0
 NO_OF_INTRF: 4
 INTRF_TYPE:  1
 INTRF_ID:  257
 INTRF_TYPE:  1
 INTRF_ID:  272
 INTRF_TYPE:  1
 INTRF_ID:  1024
 INTRF_TYPE:  1
 INTRF_ID:  277
 UNIT_ID:  1
 UNIT_TYPE:  515
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32514
 UNIT_ID:  2
 UNIT_TYPE:  65281
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32513

"""[1:].replace("\n", han_client.EOL)

DEV_TABLE_RESPONSE = """
DEV_TABLE
 DEV_INDEX: 0
 NO_OF_DEVICES: 1
 DEV_ID:  1
 DEV_IPUI:  2 233 229 181 121
 DEV_EMC:  235 15
 NO_UNITS: 4
 UNIT_ID:  0
 UNIT_TYPE:  0
 NO_OF_INTRF: 3
 INTRF_TYPE:  1
 INTRF_ID:  277
 INTRF_TYPE:  1
 INTRF_ID:  32513
 INTRF_TYPE:  1
 INTRF_ID:  1024
 UNIT_ID:  1
 UNIT_TYPE:  65290
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32529
 UNIT_ID:  2
 UNIT_TYPE:  516
 NO_OF_INTRF: 0
 UNIT_ID:  3
 UNIT_TYPE:  65293
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32534

"""[1:].replace("\n", han_client.EOL)

DEV_TABLE_PHASE_2_RESPONSE = """
DEV_TABLE_PHASE_2
 DEV_INDEX: 0
 NO_OF_DEVICES: 1
 DEV_ID:  1
 DEV_IPUI:  2 233 229 181 121
 DEV_EMC:  235 15
 ULE_CAPABILITIES: 5
 ULE_PROTOCOL_ID: 1
 ULE_PROTOCOL_VERSION: 2
 NO_UNITS: 4
 UNIT_ID:  0
 UNIT_TYPE:  0
 NO_OF_INTRF: 3
 INTRF_TYPE:  1
 INTRF_ID:  277
 INTRF_TYPE:  1
 INTRF_ID:  32513
 INTRF_TYPE:  1
 INTRF_ID:  1024
 UNIT_ID:  1
 UNIT_TYPE:  65290
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32529
 UNIT_ID:  2
 UNIT_TYPE:  516
 NO_OF_INTRF: 0
 UNIT_ID:  3
 UNIT_TYPE:  65293
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32534

"""[1:].replace("\n", han_client.EOL)

BLACK_LIST_DEV_TABLE_RESPONSE_EMPTY = """
BLACK_LIST_DEV_TABLE
 DEV_INDEX: 0
 NO_OF_DEVICES: 0

"""[1:].replace("\n", han_client.EOL)

BLACK_LIST_DEV_TABLE_RESPONSE_ONE = """
BLACK_LIST_DEV_TABLE
 DEV_INDEX: 0
 NO_OF_DEVICES: 1
 DEV_ID:  2
 DEV_IPUI:  0 0 51 51 52
 DEV_EMC:  235 15
 ULE_CAPABILITIES: 5
 ULE_PROTOCOL_ID: 1
 ULE_PROTOCOL_VERSION: 2
 NO_UNITS: 4
 UNIT_ID:  0
 UNIT_TYPE:  0
 NO_OF_INTRF: 3
 INTRF_TYPE:  1
 INTRF_ID:  277
 INTRF_TYPE:  1
 INTRF_ID:  32513
 INTRF_TYPE:  1
 INTRF_ID:  1024
 UNIT_ID:  1
 UNIT_TYPE:  65290
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32529
 UNIT_ID:  2
 UNIT_TYPE:  516
 NO_OF_INTRF: 0
 UNIT_ID:  3
 UNIT_TYPE:  65293
 NO_OF_INTRF: 1
 INTRF_TYPE:  1
 INTRF_ID:  32534
"""[1:].replace("\n", han_client.EOL)

GET_TARGET_HW_VERSION_RESPONSE = """
[SRV]
GET_TARGET_HW_VERSION_RES
 STATUS: SUCCEED
 HW_CHIP:  HW_CHIP_DCX81
 HW_CHIP_VERSION:  HW_CHIP_VERSION_C
 HW_BOARD:  HW_BOARD_MOD
 HW_COM_TYPE:  HW_COM_TYPE_USB

"""[1:].replace("\n", han_client.EOL)

FUN_MSG_MESSAGE = """
FUN_MSG
 SRC_DEV_ID:  1
 SRC_UNIT_ID:  3
 DST_DEV_ID:  0
 DST_UNIT_ID:  2
 DEST_ADDRESS_TYPE:  0
 MSG_TRANSPORT:  0
 MSG_SEQ:  0
 MSGTYPE:  1
 INTRF_TYPE:  1
 INTRF_ID:  32534
 INTRF_MEMBER:  1
 DATALEN:  14
 DATA:   48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 00

"""[1:].replace("\n", han_client.EOL)

FUN_MSG_DATA = [
    0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57,
    0x6f, 0x72, 0x6c, 0x64, 0x21, 0x00,
]


class MessageTest(unittest.TestCase):

    def test_new(self):
        msg = han_client.Message()
        self.assertTrue(msg)

        msg = han_client.Message(service="[HAN]", name="OPEN_REG")
        self.assertTrue(msg)

    def test_camelcase(self):
        self.assertEqual(han_client.Message.camelcase("DEV_TABLE"), "DevTable")

    def test_encode(self):
        str = "\x01\x0f\x13\xab\x05\x06"
        self.assertEqual(han_client.Message.encode(str), "1 F 13 AB 5 6")

    def test_init_response(self):
        msg = han_client.Message(INIT_RESPONSE)
        self.assertTrue(isinstance(msg, han_client.Message))
        self.assertEqual(msg.service, "[HAN]")
        self.assertEqual(msg.name, "INIT_RES")
        self.assertTrue(("VERSION", "1") in msg._params)
        self.assertEqual(msg.params["VERSION"], "1")

    def test_open_reg_response(self):
        msg = han_client.Message(OPEN_REG_RESPONSE)
        self.assertTrue(msg.success)

    def test_dev_table_response(self):
        msg = han_client.Message(DEV_TABLE_RESPONSE)
        self.assertTrue(isinstance(msg, han_client.DevTableMessage))
        self.assertEqual(msg.service, "[HAN]")
        self.assertEqual(msg.name, "DEV_TABLE")
        self.assertEqual(msg.index, 0)
        self.assertEqual(len(msg.devices), 1)

    def test_get_target_hw_version_response(self):
        msg = han_client.Message(GET_TARGET_HW_VERSION_RESPONSE)
        self.assertTrue(isinstance(msg, han_client.Message))
        self.assertEqual(msg.service, "[SRV]")


class DevInfoPhase2MessageTest(unittest.TestCase):

    def test_response(self):
        msg = han_client.DevInfoPhase2Message(DEV_INFO_PHASE_2_RESPONSE)

        dev = msg.device
        self.assertEqual(dev.id, 7)
        self.assertEqual(dev.ipui, "02c3c0507e")
        self.assertEqual(dev.emc, "3c2c")
        self.assertEqual(len(dev.units), 3)

        unit0, unit1, unit2 = dev.units
        self.assertEqual(unit0.id, 0)
        self.assertEqual(unit0.type, 0)
        self.assertEqual(len(unit0.interfaces), 4)
        self.assertEqual(unit1.id, 1)
        self.assertEqual(unit1.type, 0x203)
        self.assertEqual(len(unit1.interfaces), 1)
        self.assertEqual(unit2.id, 2)
        self.assertEqual(unit2.type, 0xff01)
        self.assertEqual(len(unit2.interfaces), 1)


class DevTablePhase2MessageTest(unittest.TestCase):

    def test_parse_interface(self):
        params = [
            ("INTRF_TYPE", "0"),
            ("INTRF_ID", "256"),
            ("INTRF_TYPE", "0"),
            ("INTRF_ID", "257"),
        ]

        msg = han_client.DevTablePhase2Message()
        interface, params = msg._parse_object(params, msg.Interface)

        self.assertEqual(interface.type, 0)
        self.assertEqual(interface.id, 256)
        self.assertEqual(params, [
            ("INTRF_TYPE", "0"),
            ("INTRF_ID", "257"),
        ])

    def test_hexstr(self):
        self.assertEqual(han_client._hexstr("1 100 255"), "0164ff")

    def test_response(self):
        msg = han_client.DevTablePhase2Message(DEV_TABLE_PHASE_2_RESPONSE)
        self.assertEqual(msg.index, 0)
        self.assertEqual(len(msg.devices), 1)

        dev = msg.devices[0]
        self.assertEqual(dev.id, 1)
        self.assertEqual(dev.ipui, "02e9e5b579")
        self.assertEqual(dev.emc, "eb0f")
        self.assertEqual(len(dev.units), 4)

        unit0, unit1, unit2, unit3 = dev.units
        self.assertEqual(unit0.id, 0)
        self.assertEqual(unit0.type, 0)
        self.assertEqual(len(unit0.interfaces), 3)
        self.assertEqual(unit1.id, 1)
        self.assertEqual(unit1.type, 0xff0a)  # ULE voice call
        self.assertEqual(len(unit1.interfaces), 1)
        self.assertEqual(unit2.id, 2)
        self.assertEqual(unit2.type, 0x204)  # Smoke
        self.assertEqual(len(unit2.interfaces), 0)
        self.assertEqual(unit3.id, 3)
        self.assertEqual(unit3.type, 0xff0d)  # ULEasy
        self.assertEqual(len(unit3.interfaces), 1)


class BlackListDevTableMessageTest(unittest.TestCase):

    def test_message(self):
        msg = han_client.BlackListDevTableMessage(BLACK_LIST_DEV_TABLE_RESPONSE_EMPTY)
        self.assertEqual(len(msg.devices), 0)

        msg = han_client.BlackListDevTableMessage(BLACK_LIST_DEV_TABLE_RESPONSE_ONE)
        self.assertEqual(len(msg.devices), 1)


class FunMsgMessageTest(unittest.TestCase):

    def test_message(self):
        msg = han_client.Message(FUN_MSG_MESSAGE)
        for i, x in enumerate(msg.data):
            self.assertEqual(x, FUN_MSG_DATA[i])


if __name__ == "__main__":
    unittest.main()
