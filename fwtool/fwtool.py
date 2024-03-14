#!/usr/bin/env python3
#
# SPDX-License-Identifier: MIT

from __future__ import print_function

__version__ = "0.1"

# Usage:
#  $ fwtool list # list connected devices
#  $ fwtool region <"eu"|"us"|"jp"|"kr"> # set target firmware
#  $ fwtool param <name> [value] # set/get parameter
#  $ fwtool eeprom <range> <bytes> # set/get eeprom values
#  $ fwtool preset <name/id> # apply eeprom preset

# TODO: eeprom: add --dump feature
#   - implement as stream, yielding bytes as we go
#   - on CMND, we do not have the eeprom size
#     - read in 256 byte chunks
#     - on ie.response = 1, half chunk size, retry until chunk size is 0
# TODO: eeprom: dump binary data when connected to pipe (tty detection)

import logging
import sys
import struct
from collections import OrderedDict

import click
import serial

import cmbs
import cmnd


class ResponseError(Exception):
    def __init__(self, code):
        self.code = code

    def __str__(self):
        return "error code: {:#04x}".format(self.code)


region_settings = {
    #      us_dect, support_fcc, full_power, deviation, pa2_comp
    "eu": (0x00,    0x00,        0x7f,       0x13,      0x3c),
    "us": (0x01,    0x01,        0xde,       0x23,      0x3c),
    "jp": (0x12,    0x02,        0xde,       0x00,      0xac),
    "kr": (0x0b,    0x00,        0x7f,       0x13,      0x3c),
}


def err_exit(message):
    click.echo("Error: {}".format(message), err=True)
    sys.exit(1)


def list_devices():
    from serial.tools.list_ports import comports
    devices = []
    for port in comports():
        if port.vid == 0x0403 and port.pid == 0x6001:
            devices.append((port.device, 'cmnd'))
        if port.vid == 0x3006 and port.pid == 0x1977:
            devices.append((port.device, 'cmbs'))
    return sorted(devices)


def find_device_byname(dev, devices):
    for devname, devtype in devices:
        if dev == devname:
            return devname, devtype
    return None


def find_device(dev):
    # dev == None: autodetect
    # dev == "<devname>": autodetect type on devname
    # dev == "<devname>:": autodetect type on devname
    # dev == "<devname>:[cmbs|cmnd]": skip autodetection
    devices = list_devices()

    if not dev and not devices:
        err_exit("no devices found during auto detection, try '--dev'?")

    if not dev and len(devices) > 1:
        err_exit("more than one device found, use '--dev'.")

    if not dev:
        return devices[0]

    dev = dev.rsplit(":", 1)

    if len(dev) == 2 and not dev[1]:
        del dev[1]

    if len(dev) == 1:
        devname = dev[0]
        dev = find_device_byname(devname, devices)
        if not dev:
            err_exit("detection on '{}' failed, specify cmbs/cmnd.".format(devname))
        return dev

    devname, devtype = dev
    devtype = devtype.lower()
    if devtype not in ['cmbs', 'cmnd']:
        err_exit("unknown device type: '{}'. Use 'cmbs' or 'cmnd'.".format(devtype))
    return devname, devtype


def connect_target(ctx):
    dev = ctx.obj["DEV"]
    devname, devtype = find_device(dev)

    try:
        ser = serial.Serial(devname, baudrate=115200, timeout=0)
    except serial.SerialException:
        err_exit("cannot open serial port. Another application using it?")

    target = None
    if devtype == 'cmbs':
        target = CMBS(ser)
    if devtype == 'cmnd':
        target = CMND(ser)
    return target


class CMND(object):
    presets = OrderedDict([
        ("cr_local",             0x00),
        ("cr_cmnd",              0x01),
        ("ac",                   0x02),
        ("smoke_uart",           0x03),
        ("smoke",                0x04),
        ("ule_voice_call",       0x05),
        ("ule_voice_call_cmnd",  0x06),
        ("spmkt",                0x07),
        ("ac_uart",              0x08),
        ("simple_pwr_mtr_uart",  0x09),
        ("sws_btn",              0x0a),
        ("wakeup_uart",          0x0b),
        ("simple_pwr_mtr",       0x0c),
        ("euro_thermostat",      0x0d),
        ("euro_wallswitch",      0x0e),
        ("euro_window",          0x0f),
        ("host_extention",       0x10),
        ("smoke_pageable",       0x11),
        ("ac_broadcast",         0x12),
        ("ac_broadcast_cmnd",    0x13),
        ("generic_cmnd",         0x14),
        ("expansion_board",      0x15),
    ])

    params = OrderedDict([
        ("keep_alive",         dict(id=0x29, format="<L", desc="Keep alive interval in ms.")),
        ("minimum_sleep_time", dict(id=0x1c, format=">L", desc="Minimum time the device should be sleeping between pages, in ms.")) # noqa
    ])

    class ProductionModeContext(object):
        def __init__(self, target):
            self._target = target

        def __enter__(self):
            self._target.into_production()

        def __exit__(self, exc_type, exc_value, traceback):
            self._target.into_normal()

    def __init__(self, ser):
        self._ser = ser

        logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s:%(name)s: %(message)s")
        # cmnd._logger = logging.getLogger('CMND')

        # try to attach via reset/hello
        cmnd.send(self._ser, 0, cmnd.SERVICE_ID_SYSTEM, cmnd.MSG_SYS_RESET_REQ)
        while True:
            msg = cmnd.receive(ser, timeout=4)
            if msg.service == cmnd.SERVICE_ID_GENERAL and msg.id == cmnd.MSG_GENERAL_HELLO_IND:
                break

    def is_cmnd(self):
        return True

    def send(self, svc, msg, *ies):
        cmnd.send(self._ser, 0, svc, msg, *ies)

    def wait(self, svc, msg):
        return cmnd.wait(self._ser, svc, msg)

    def release(self):
        self.into_normal()

    def reset(self):
        self.send(cmnd.SERVICE_ID_SYSTEM, cmnd.MSG_SYS_RESET_REQ)
        self.wait(cmnd.SERVICE_ID_GENERAL, cmnd.MSG_GENERAL_HELLO_IND)

    def into_normal(self):
        self.send(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_END_REQ)
        resp = self.wait(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_CFM)
        ie = resp.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)
        self.reset()

    def into_production(self):
        self.send(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_START_REQ)
        resp = self.wait(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_CFM)
        ie = resp.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)
        self.reset()

    def production_mode(self):
        return self.ProductionModeContext(self)

    def get_param(self, name):
        param = self.params[name]
        id = param["id"]
        fmt = param["format"]

        ie = cmnd.IEParameter(0x00, id)
        self.send(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_GET_REQ, ie)
        msg = self.wait(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_GET_RES)
        ie = msg.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)
        ie = msg.get_ie(cmnd.IEParameter)
        if ie.id != id:
            raise ValueError()
        if ie.type != 0:
            raise ValueError()

        (value,) = struct.unpack(fmt, ie.data)
        return value

    def set_param(self, name, value):
        if isinstance(name, int):
            id = name
            data = value  # raw data passed via value
        else:
            param = self.params[name]
            id = param["id"]
            fmt = param["format"]
            data = struct.pack(fmt, int(value))

        ie = cmnd.IEParameter(0x00, id, data)
        self.send(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_REQ, ie)
        msg = self.wait(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_RES)
        ie = msg.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

    def get_param_direct(self, typ, offset, length):
        ie = cmnd.IEParameterDirect(typ, offset, length=length)
        self.send(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_GET_DIRECT_REQ, ie)
        msg = self.wait(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_GET_DIRECT_RES)
        ie = msg.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)
        ie = msg.get_ie(cmnd.IEParameterDirect)
        if ie.type != typ:
            raise ValueError()
        if ie.offset != offset:
            raise ValueError()
        return ie.data

    def set_param_direct(self, typ, offset, data):
        ie = cmnd.IEParameterDirect(typ, offset, data)
        self.send(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_DIRECT_REQ, ie)
        msg = self.wait(cmnd.SERVICE_ID_PARAMETERS, cmnd.MSG_PARAM_SET_DIRECT_RES)
        ie = msg.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

    def region(self, settings):
        us_dect, support_fcc, full_power, deviation, pa2_comp = settings

        self.set_param(cmnd.PARAM_EEPROM_DECT_CARRIER, struct.pack("B", us_dect))
        self.set_param(cmnd.PARAM_EEPROM_DECT_SUPPORT_FCC, struct.pack("B", support_fcc))
        self.set_param(cmnd.PARAM_EEPROM_DECT_FULL_POWER, struct.pack("B", full_power))
        self.set_param(cmnd.PARAM_EEPROM_DECT_DEVIATION, struct.pack("B", deviation))
        self.set_param(cmnd.PARAM_EEPROM_DECT_PA2_COMP, struct.pack("B", pa2_comp))

    def get_eeprom(self, offset, length):
        # dect eeprom only for now
        return self.get_param_direct(cmnd.PARAM_ADDRESS_TYPE_DECT_EEPROM, offset, length)

    def set_eeprom(self, offset, data):
        # dect eeprom only for now
        self.set_param_direct(cmnd.PARAM_ADDRESS_TYPE_DECT_EEPROM, offset, data)

    def set_preset(self, id):
        self.send(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_SPECIFIC_PRESET_REQ, cmnd.IEU8(id))
        msg = self.wait(cmnd.SERVICE_ID_PRODUCTION, cmnd.MSG_PROD_CFM)
        ie = msg.get_ie(cmnd.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

    def delete_subscription(self):
        self.set_eeprom(58, b"\x00")


class CMBS(object):
    params = {}

    # stub, before calling sending EV_DSR_SYS_START we already are
    # in what CMND calls production mode
    class ProductionModeContext(object):
        def __init__(self, target):
            self._target = target

        def __enter__(self):
            pass

        def __exit__(self, exc_type, exc_value, traceback):
            pass

    def __init__(self, ser):
        self._ser = ser

        logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s:%(name)s: %(message)s")
        # cmbs._logger = logging.getLogger('CMBS')

        cmbs.send_cmd(self._ser, cmbs.CMD_HELLO, payload=b"\x00"*6)
        while True:
            msg = cmbs.receive(self._ser, timeout=4)
            if msg.id == 0xff00 + cmbs.CMD_HELLO_RPLY:
                break
        # msg.payload contains:
        # - u16 target api version
        #    if(u16_Version & 0xF000)
        #    {
        #        CFR_DBG_OUT( "TARGET API version: %x.%02x.%x\n", (u16_Version>>12),((u16_Version & 0xFF0)>>4), (u16_Version & 0xF) );
        #    }
        #    else
        #    {
        #        CFR_DBG_OUT( "TARGET API version: %02x.%02x\n", (u16_Version>>8),(u16_Version &0xFF) );
        #    }
        #    if (u16_Version == 0x0001) { /* bootloader running */ }
        # - u16 target build
        # - u8  target mode

        # no checksum support, otherwise set last byte to 0x01
        cmbs.send_cmd(self._ser, cmbs.CMD_CAPABILITIES, payload=b"\x04\x00\x00\x00\x00")
        cmbs.wait_cmd(self._ser, cmbs.CMD_CAPABILITIES_RPLY)

    def is_cmnd(self):
        return False

    def send(self, event, *ies):
        cmbs.send(self._ser, event, *ies)

    def wait(self, event):
        msg = cmbs.wait(self._ser, event)
        return msg

    def reset(self):
        # FIXME: for USB targets the serial device will disappear during reset, need to close
        # and reopen it and use a timeout in between
        pass

    def production_mode(self):
        return self.ProductionModeContext(self)

    def get_param(self, name):
        param = self.params[name]
        id = param["id"]
        fmt = param["format"]

        ie = cmbs.IEParameter(id, 0x00)
        self.send(cmbs.EV_DSR_PARAM_GET, ie)
        resp = self.wait(cmbs.EV_DSR_PARAM_GET_RES)
        ie = resp.get_ie(cmbs.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

        ie = resp.get_ie(cmbs.IEParameter)
        if ie.id != id:
            raise ValueError()
        if ie.type != 0:
            raise ValueError()

        (value,) = struct.unpack(fmt, ie.data)
        return value

    def set_param(self, name, value):
        if isinstance(name, int):
            id = name
            data = value  # raw data passed via value
        else:
            param = self.params[name]
            id = param["id"]
            fmt = param["format"]
            data = struct.pack(fmt, int(value))

        ie = cmbs.IEParameter(id, 0x00, data)
        self.send(cmbs.EV_DSR_PARAM_SET, ie)
        msg = self.wait(cmbs.EV_DSR_PARAM_SET_RES)
        ie = msg.get_ie(cmbs.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

    def get_param_area(self, typ, offset, length):
        ie = cmbs.IEParameterArea(typ, offset, length=length)
        self.send(cmbs.EV_DSR_PARAM_AREA_GET, ie)
        msg = self.wait(cmbs.EV_DSR_PARAM_AREA_GET_RES)
        ie = msg.get_ie(cmbs.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)
        ie = msg.get_ie(cmbs.IEParameterArea)
        if ie.type != typ:
            raise ValueError()
        if ie.offset != offset:
            raise ValueError()
        if len(ie.data) != length:
            raise ValueError()
        return ie.data

    def set_param_area(self, typ, offset, data):
        ie = cmbs.IEParameterArea(typ, offset, data)
        self.send(cmbs.EV_DSR_PARAM_AREA_SET, ie)
        msg = self.wait(cmbs.EV_DSR_PARAM_AREA_SET_RES)
        ie = msg.get_ie(cmbs.IEResponse)
        if ie.result != 0:
            raise ResponseError(ie.result)

    def region(self, settings):
        us_dect, support_fcc, full_power, deviation, pa2_comp = settings

        self.set_eeprom(0x20, struct.pack("B", us_dect))
        self.set_param(cmbs.PARAM_RF19APU_SUPPORT_FCC, struct.pack("B", support_fcc))
        self.set_param(cmbs.PARAM_RF_FULL_POWER, struct.pack("B", full_power))
        self.set_param(cmbs.PARAM_RF19APU_DEVIATION, struct.pack("B", deviation))
        self.set_param(cmbs.PARAM_RF19APU_PA2_COMP, struct.pack("B", pa2_comp))

    def get_eeprom(self, offset, length):
        return self.get_param_area(cmbs.PARAM_AREA_TYPE_EEPROM, offset, length)

    def set_eeprom(self, offset, data):
        self.set_param_area(cmbs.PARAM_AREA_TYPE_EEPROM, offset, data)

    def session(self):
        return self


@click.group()
@click.option("--dev", help="serial device to use, format: <dev>[:<cmnd|cmbs>]")
@click.version_option(__version__)
@click.pass_context
def cli(ctx, dev):
    ctx.obj["DEV"] = dev


@cli.command()
@click.pass_context
def list(ctx):
    """List available devices."""
    devices = list_devices()
    if not devices:
        err_exit("no devices found.")

    for dev in devices:
        click.echo(":".join(dev))
    click.echo("Found {} device(s)".format(len(devices)))


@cli.command()
@click.option("--list", is_flag=True, help="List supported regions.")
@click.argument("name", required=False)
@click.pass_context
def region(ctx, list, name):
    """Apply settings for a specific earth region."""
    if list or not name:
        click.echo("Supported regions:")
        for regionname in region_settings:
            click.echo("  {}".format(regionname))
        return

    settings = region_settings.get(name)
    if not settings:
        err_exit("region not supported: '{}'. Use '--list' to check available regions.".format(name))

    target = connect_target(ctx)
    with target.production_mode():
        target.region(settings)

    click.echo("Configured target device for region '{}'.".format(name))


@cli.command()
@click.option("--list", is_flag=True, help="List supported parameters.")
@click.argument("name", required=False)
@click.argument("value", required=False)
@click.pass_context
def param(ctx, list, name, value):
    """Get, set or list firmware parameters."""
    if not list and not name:
        click.echo(param.get_help(ctx), err=True)
        return

    target = connect_target(ctx)

    if list and not target.params:
        click.echo("This device exports no parameters.", err=True)
        return

    if list:
        click.echo("Supported parameters:")
        for paramname in target.params:
            click.echo("  {}".format(paramname))
        return

    if not name:
        err_exit("parameter name not specified.")

    if name not in target.params:
        err_exit("unknown parameter name: '{}'.".format(name))

    if value:
        write = True
    else:
        write = False

    with target.production_mode():
        if write:
            target.set_param(name, value)
            if target.is_cmnd():
                target.delete_subscription()
        else:
            value = target.get_param(name)

    if write:
        click.echo("Updated parameter '{}'.".format(name))
    else:
        click.echo(value)

    if write and target.is_cmnd():
        click.secho("Pairing information with the base has been deleted!", fg="red")
        click.secho("Please re-register your device.", fg="green")


def parse_range(range):
    if '+' in range:
        offset, length = range.split("+", 1)
        offset = int(offset, 0)
        length = int(length, 0)
    else:
        offset = int(range, 0)
        length = 0

    return offset, length


def format_bytes(bytes):
    """Generates a hex string with up to 16 bytes per line."""
    res = []
    for i in range(0, len(bytes), 16):
        chunk = bytes[i:i+16]
        res.append(" ".join("{:02x}".format(ord(b)) for b in chunk))
    return "\n".join(res)


@cli.command()
@click.argument("range")
@click.argument("bytes", required=False, nargs=-1)
@click.pass_context
def eeprom(ctx, range, bytes):
    """Modify EEPROM values."""
    try:
        offset, length = parse_range(range)
    except ValueError as e:
        err_exit(e)

    if length and bytes and length != len(bytes):
        err_exit("specified length does not match specified number of bytes")

    if not bytes and not length:
        length = 1

    if bytes:
        tmp = bytearray()
        for b in bytes:
            try:
                b = int(b, 16)
                tmp.append(b)
            except ValueError as e:
                err_exit(e)
        bytes = tmp

    if bytes:
        write = True
    else:
        write = False

    target = connect_target(ctx)
    with target.production_mode():
        if write:
            target.set_eeprom(offset, bytes)
        else:
            bytes = target.get_eeprom(offset, length)

    if write:
        click.echo("Wrote {} byte(s) to offset {:#010x}.".format(len(bytes), offset))
    else:
        click.echo(format_bytes(bytes))


@cli.command()
@click.option("--list", is_flag=True, help="List supported presets.")
@click.argument("name", required=False)
@click.pass_context
def preset(ctx, list, name):
    """Apply a preset by name or id."""

    target = connect_target(ctx)

    if not getattr(target, "presets", None):
        click.echo("This device does not support applying presets.", err=True)
        return

    if list:
        click.echo("Supported presets:")
        for preset in target.presets:
            click.echo("  {}".format(preset))
        return

    if not name:
        err_exit("need to provide name/id of preset.")

    try:
        id = int(name, 0)
    except ValueError:
        if name not in target.presets:
            err_exit("preset name unknown and not a number: '{}'.".format(name))
        id = target.presets[name]

    with target.production_mode():
        target.set_preset(id)

    click.secho("Pairing information with the base has been deleted!", fg="red")
    click.secho("Please re-register your device.", fg="green")


if __name__ == "__main__":
    cli(obj={})
