#!/usr/bin/env python
#
# SPDX-License-Identifier: MIT
#
# TODO:
#   - specify serial device via --dev command line switch

from __future__ import print_function
import sys
import argparse
import serial
import cmnd


class ResponseError(Exception):
    pass


class CMND(object):
    def __init__(self, ser):
        self._ser = ser

        # try to attach via reset/hello
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_SYSTEM, cmnd.MSG_SYS_RESET_REQ)
        while True:
            msg = cmnd.receive(ser, timeout=2)
            if msg.service == cmnd.SERVICE_ID_GENERAL and msg.id == cmnd.MSG_GENERAL_HELLO_IND:
                break

    def get_software_version(self):
        payload = cmnd.ie_topayload(cmnd.IeU8(1))
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_GENERAL, cmnd.MSG_GENERAL_GET_VERSION_REQ, payload)
        while True:
            msg = cmnd.receive(self._ser)
            if msg.service == cmnd.SERVICE_ID_GENERAL and msg.id == cmnd.MSG_GENERAL_GET_VERSION_RES:
                break

        ie = cmnd.ie_get(msg.payload, cmnd.IeVersion)
        return ie.version

    def set_param(self, id, value):
        ie = cmnd.IeParameter()
        ie.type = 0x00
        ie.id = id
        ie.length = 1
        ie.data = value

        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_REQ, cmnd.ie_topayload(ie))
        msg = cmnd.wait(self._ser, cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_RES)
        ie = cmnd.ie_get(msg.payload, cmnd.IeResponse)
        if ie.result != 0:
            raise ResponseError()

    def set_dect_eeprom(self, address, value):
        ie = cmnd.IeParameterDirect(0x02, address, bytearray([value]))
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_DIRECT_REQ, cmnd.ie_topayload(ie))
        resp = cmnd.wait(self._ser, cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_DIRECT_RES)
        ie = cmnd.ie_get(resp.payload, cmnd.IeResponse)
        if ie.result != 0:
            raise ResponseError()
        print("[*] Set DECT EEPROM {:#06x} = {:#04x}".format(address, value))

    def into_normal(self):
        print("[ ] Requesting normal mode...")
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_END_REQ)
        resp = cmnd.wait(self._ser, cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_CFM)
        ie = cmnd.ie_get(resp.payload, cmnd.IeResponse)
        if ie.result != 0:
            raise ResponseError()
        print("[*] Normal mode confirmed")
        self.reset()

    def into_production(self):
        print("[ ] Requesting production mode...")
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_START_REQ)
        resp = cmnd.wait(self._ser, cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_CFM)
        ie = cmnd.ie_get(resp.payload, cmnd.IeResponse)
        if ie.result != 0:
            raise ResponseError()
        print("[*] Production mode confirmed")
        self.reset()


    def reset(self):
        print("[ ] Resetting target...")
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_SYSTEM, cmnd.MSG_SYS_RESET_REQ)
        cmnd.wait(self._ser, cmnd.SERVICE_ID_GENERAL, cmnd.MSG_GENERAL_HELLO_IND)
        print("[*] Target reset")


def list_devices():
    from serial.tools.list_ports import comports
    devices = []
    for port in comports():
        if port.vid == 0x0403 and port.pid == 0x6001:
            devices.append(port.device)
    return devices


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dev', help='serial device/com port to use')
    args = parser.parse_args(sys.argv[1:])

    if args.dev:
        dev = args.dev
    else:
        devices = list_devices()
        if not devices:
            print("Error: no device found, plug in usb cable?")
            return 1
        if len(devices) > 1:
            print("Error: multiple devices found, specify --dev")
            return 1
        dev = devices[0]
        print("Autodetected DU-EB on", dev)

    try:
        ser = serial.Serial(dev, 115200, timeout=0)
    except serial.SerialException:
        print("Error: cannot open serial port. Wrong device/com port? Another application running?")
        return 1

    try:
        target = CMND(ser)
    except cmnd.TimeoutException:
        print("Error: connecting to target failed. Wrong device? Try running without --dev.")
        return 1

    print("Connected to target")

    version = target.get_software_version()
    if version != "34.24":
        print("Error: bad target version: expected '34.24', got '{}'".format(version))

    target.into_production()

    # AEC_MODE - disable AEC
    target.set_dect_eeprom(0x226, 0x00)
    target.set_dect_eeprom(0x227, 0x00)
    # ACL_V_MIN/ACL_V_MAX - disable dynamic volume, fix to 0x0800
    target.set_dect_eeprom(0x273, 0x00)
    target.set_dect_eeprom(0x274, 0x08)
    target.set_dect_eeprom(0x275, 0x00)
    target.set_dect_eeprom(0x276, 0x08)

    target.into_normal()


if __name__ == "__main__":
    sys.exit(main())
