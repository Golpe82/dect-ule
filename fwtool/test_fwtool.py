# SPDX-License-Identifier: MIT
import unittest
import fwtool

class TestFwtool(unittest.TestCase):

    def test_parse_range(self):
        offset, length = fwtool.parse_range("0x100")
        self.assertEqual(offset, 256)
        self.assertEqual(length, 0)

        offset, length = fwtool.parse_range("0x100+1")
        self.assertEqual(offset, 256)
        self.assertEqual(length, 1)

        offset, length = fwtool.parse_range("200")
        self.assertEqual(offset, 200)
        self.assertEqual(length, 0)

        offset, length = fwtool.parse_range("200+16")
        self.assertEqual(offset, 200)
        self.assertEqual(length, 16)

    def test_format_bytes(self):
        s = fwtool.format_bytes(b"\x00\x01\x02\x03")
        self.assertEqual(s, "00 01 02 03")

        s = fwtool.format_bytes(b"\x00"*16 + b"\x01\x02\x03\x04")
        self.assertEqual(s, "00 "*15 + "00\n" + "01 02 03 04")

if __name__ == '__main__':
    unittest.main()
