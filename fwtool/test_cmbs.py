# SPDX-License-Identifier: MIT
import unittest
import cmbs

class TestMessage(unittest.TestCase):

    def test_empty(self):
        msg = cmbs.Message()
        self.assertEqual(msg.id, 0)
        self.assertEqual(msg.payload, b"")

    def test_payload(self):
        msg = cmbs.Message(cmbs.CMD_HELLO, b"\x01\x02")
        self.assertEqual(msg.id, cmbs.CMD_HELLO)
        self.assertEqual(msg.payload, b"\x01\x02")

if __name__ == '__main__':
    unittest.main()
