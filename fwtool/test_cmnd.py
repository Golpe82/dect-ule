# SPDX-License-Identifier: MIT
import unittest
import struct
import cmnd


class TestIe(unittest.TestCase):

    def test_IEU8(self):
        ie = cmnd.IEU8()

        ie = cmnd.IEU8(0)
        self.assertEqual(ie.data, 0)

        ie = cmnd.IEU8(0xff)
        self.assertEqual(ie.data, 0xff)

        buf = ie.pack()
        self.assertEqual(buf, struct.pack('!BHB', 0x1e, 1, 0xff))

    def test_IEParameter(self):
        ie = cmnd.IEParameter()
        self.assertEqual(ie.type, 0)
        self.assertEqual(ie.id, 0)
        self.assertEqual(ie.data, b'')

        buf = b'\x01\x02\x00\x03\xf0\xf1\xf2'

        ie = cmnd.IEParameter()
        ie._unpack_content(buf)
        self.assertEqual(ie.type, 1)
        self.assertEqual(ie.id, 2)
        self.assertEqual(ie.data, b'\xf0\xf1\xf2')

        ie = cmnd.IEParameter(1, 2, b'\xf0\xf1\xf2')
        self.assertEqual(ie._pack_content(), buf)

    def test_lookup_service(self):
        self.assertEqual(cmnd.lookup_service(0x0000), "GENERAL")
        self.assertEqual(cmnd.lookup_service(0x0001), "DEVICE_MANAGEMENT")
        self.assertEqual(cmnd.lookup_service(0xffff), "UNKNOWN")

    def test_lookup_messagee(self):
        self.assertEqual(cmnd.lookup_message("GENERAL", 0x00), "UNKNOWN")
        self.assertEqual(cmnd.lookup_message("GENERAL", 0x05), "HELLO_IND")
        self.assertEqual(cmnd.lookup_message("SYSTEM", 0x08), "RESET_REQ")


if __name__ == '__main__':
    unittest.main()
