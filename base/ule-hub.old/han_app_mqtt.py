#!/usr/bin/env python3
#
# SPDX-License-Identifier: MIT
"""
HAN App
"""

from __future__ import unicode_literals
import shlex
import sys
import datetime
from prompt_toolkit import prompt
from prompt_toolkit.history import InMemoryHistory
from prompt_toolkit.contrib.completers import WordCompleter
#from prompt_toolkit.completion import WordCompleter
#import prompt_toolkit.completion
#from prompt_toolkit.patch_stdout import patch_stdout
 
import han_client
import _thread

import logging
import copy
from profile_hanfun import *
from snom_utils import *


def log(str):
    print(timestamp() + " " + str)


def timestamp():
    curr_time = datetime.datetime.now()
    formatted_time = curr_time.strftime("%H:%M:%S.%f")[:-3]
    return formatted_time

#
# callback handlers
#
def process_rx_data(client, data_str):
    """Custom handler for the received ascii messages from the HAN server."""
    # Do something with the data received from the CMBS block.
    # This could be something received over the air or from
    # inside the CMBS block
    log("HAN Client <<-- HAN Server\n" + data_str)


def handle_dev_registered(client, msg):
    device_id = int(msg.params["DEV_ID"])
    log("Device {}: registered (or registration updated)".format(device_id))


def handle_reg_closed(client, msg):
    reason = msg.params["REASON"]
    log("Registration window closed (reason: {})".format(reason))


def handle_fun_msg(client, msg):
    device_id = int(msg.params["SRC_DEV_ID"])
    unit_id = int(msg.params["SRC_UNIT_ID"])
    interface_id = int(msg.params["INTRF_ID"])

    if unit_id == 0 and interface_id == 0x0115:
        # device management unit, keep-alive interface
        log("Device {}: keep alive".format(device_id))

    if unit_id == 1:
        # voice call unit
        log("Device {}: message from voice call unit".format(device_id))
        
        log("LEN {}".format(int(msg.params["DATALEN"])))
        log("data raw {}".format(msg.params["DATA"]))
        
    if unit_id == 2:
        # smoke unit
        log("Device {}: message from smoke unit".format(device_id))

    if unit_id == 3:
        # ULEasy unit (raw data)
        data = msg.data.decode("utf-8")
        log("Device {}: message from raw data unit: '{}'".format(device_id, data))


def handle_fun_msg_res(client, msg):
    device_id = int(msg.params["DEV_ID"])
    log("Device {}: message delivered.".format(device_id))


def handle_call_establish_ind(client, msg):
    call_id = int(msg.params["CALL_ID"])
    participant = msg.params["PARTICIPANT"]
    if participant[0] == "D":
        participant = "Device {}".format(int(participant[1:]))
    else:
        participant = "Handset " + participant
    log("Call {}: established with {}".format(call_id, participant))


def handle_call_dev_released_ind(client, msg):
    call_id = int(msg.params["CALL_ID"])
    device_id = int(msg.params["DEV_ID"])
    log("Call {}: device {} released".format(call_id, device_id))


def handle_call_release_ind(client, msg):
    call_id = int(msg.params["CALL_ID"])
    log("Call {}: released".format(call_id))
    pass


#
# helpers
#
def send_attr_request(client_handle, device_id, unit_id, interface_id, attr_id):
    """Sends request attribute pack for mandatory and optional attributes
    """
    cookie = client_handle.fun_msg(
        ipui=devices_list[device_id-1].ipui, # device 6 fixed
        src_dev_id=0,
        src_unit_id=2,
        dst_dev_id=device_id,
        dst_unit_id=unit_id,  # call unit
        interface_type=1,  # interface_type: 0 = server, 1 = client
        interface_id=interface_id,  # Metering 
        # cmd and cmd_id
        msg_type=0x04,  # get attribute, HF Protocol 7.3.2
        interface_member=attr_id, # get # attribute 
        data='' # no payload data,
    )
    return cookie


def send_attr_pack_request(client_handle, device_id, interface_id):
    """Sends request attribute pack for mandatory and optional attributes
    """
    cookie = client_handle.fun_msg(
        ipui=devices_list[device_id-1].ipui, # device 6 fixed
        src_dev_id=0,
        src_unit_id=2,
        dst_dev_id=device_id,
        dst_unit_id=1,  # call unit
        interface_type=1,  # interface_type: 0 = server, 1 = client
        interface_id=interface_id,  # Metering 
        # cmd and cmd_id
        msg_type=0x09,  # get attrobute pack request, HF Protocol 7.3.2
        #msg_type=0x03,  # get attrobute pack request, HF Protocol 7.3.2
        interface_member=0xfe, # mandatory and optional attributes
        data='' # no payload data,
    )
    return cookie

def send_data(client_handle, device_id, data):
    """Sends RAW <data> to <device_id>

    All ULE expansion boards are configured to have a Unit 3 which will accept
    any message payload. Send anything you want.
    """
    '''
    # send raw data to Unit 3 (ULEasy)
    cookie = client_handle.fun_msg(
        ipui="02c3cc8e37", # device 6 fixed
        src_dev_id=0,
        src_unit_id=0,
        dst_dev_id=device_id,
        dst_unit_id=3,  # ULEasy unit
        interface_type=0,  # server
        interface_id=0x7f16,  # ULeasy interface
        interface_member=1,
        data=data,
    )
    '''
    cookie = client_handle.fun_msg(
        ipui=devices_list[device_id-1].ipui, # device 6 fixed
        src_dev_id=0,
        src_unit_id=0,
        dst_dev_id=device_id,
        dst_unit_id=1,  # call unit
        interface_type=1,  # interface_type: 0 = server, 1 = client
        interface_id=0x0200,  # Metering 
        # cmd and cmd_id
        msg_type=1,  # msg_type: message type, e.g. command=1 or attribute request=0x04 , HF Protocol 7.3.2
        interface_member=1, # interface_member: attribute index or command id
        data=data # no payload data,
    )
    return cookie

# temperature 
# send_cmd 5 2 769 4 1
# 3 bytes statt S16
def send_cmd(client_handle, device_id, unit_id, interface_id, msg_type, cmd_id, data):
    """Sends CMD with RAW <data> to <device_id>
    """
    # send CMD ON 
    '''
    reset W/h
    ----------
    HAN Client -->> HAN Server:
[HAN]
FUN_MSG
 DEV_IPUI: 02c3cc8e37
 SRC_DEV_ID: 0
 SRC_UNIT_ID: 0
 DST_DEV_ID: 6
 DST_UNIT_ID: 1
 DEST_ADDRESS_TYPE: 0
 MSG_TRANSPORT: 0
 MSG_SEQ: 0
 MSGTYPE: 1
 INTRF_TYPE: 1
 INTRF_ID: 768
 INTRF_MEMBER: 1
 DATALEN: 0
 '''
    #data = str(bytes([1,1]), encoding='ISO-8859-1')
    print(f'data given={data}')
    for d in devices_list:
        if d.id == device_id:
            dev = d
            break

    cookie = client_handle.fun_msg(
        #ipui=devices_list[device_id-1].ipui, 
        ipui=dev.ipui, 
        src_dev_id=0,
        src_unit_id=2,  # das war 0 
        dst_dev_id=device_id,
        dst_unit_id=unit_id,  # device can have many units != 0 
        interface_type=1,  # interface_type: 0 = server, 1 = client
        interface_id=interface_id,  # On-off=256 etc. 
        # cmd and cmd_id
        msg_type=msg_type,  # msg_type: message type, e.g. command=1 or HF Protocol 7.3.1
        interface_member=cmd_id, # interface_member: attribute index or command id
        data=data # payload data given in characters .. no is ''
    )
    return cookie

def send_cmd_to_server(client_handle, device_id, unit_id, interface_id, msg_type, cmd_id, data):
    """Sends CMD with RAW <data> to <device_id>
    """
    # send CMD ON 
    '''
    reset W/h
    ----------
    HAN Client -->> HAN Server:
[HAN]
FUN_MSG
 DEV_IPUI: 02c3cc8e37
 SRC_DEV_ID: 0
 SRC_UNIT_ID: 0
 DST_DEV_ID: 6
 DST_UNIT_ID: 1
 DEST_ADDRESS_TYPE: 0
 MSG_TRANSPORT: 0
 MSG_SEQ: 0
 MSGTYPE: 1
 INTRF_TYPE: 1
 INTRF_ID: 768
 INTRF_MEMBER: 1
 DATALEN: 0
 '''
    #data = str(bytes([1,1]), encoding='ISO-8859-1')
    print(f'data given={data}')
    cookie = client_handle.fun_msg(
        ipui=devices_list[device_id-1].ipui, # device 6 fixed
        src_dev_id=0,
        src_unit_id=2,  # das war 0 
        dst_dev_id=device_id,
        dst_unit_id=unit_id,  # back to 1 ?????? call unit
        interface_type=0,  # interface_type: 0 = server, 1 = client
        interface_id=interface_id,  # On-off=256 etc. 
        # cmd and cmd_id
        msg_type=msg_type,  # msg_type: message type, e.g. command=1 or HF Protocol 7.3.1
        interface_member=cmd_id, # interface_member: attribute index or command id
        data=data # payload data given in characters .. no is ''
    )
    return cookie

#
# commands
#

devices_list = []

def list_devices(client_handle, argv):
    """
NAME
devices - get information about all the devices

SYNOPSIS
devices

DESCRIPTION
Lists the all the information for each device registered to the hub.
    """
    global devices_list
    devices_list = []


    index = 0
    count = 5
    while True:
        resp = client_handle.get_dev_table(index=index, count=count)
        for dev in resp.devices:
            print(dev)
            devices_list.append(dev)

        # If there are fewer devices than we have asked for we have retrieved them all,
        # otherwise we need to move the index and check if there are more.
        if len(resp.devices) < count:
            break
        else:
            index += count

    for dev in devices_list:
        for unit in dev.units:
            log('dev {}: unit #{}={}'.format(dev.id,unit.id, hex(unit.type)))
            for interface in unit.interfaces:
                log('interface {}, type={}'.format(hex(interface.id), hex(interface.type)))

    # save all in the HF structure
    snom_store_devices(client_handle, argv)

def snom_store_devices(client_handle, argv):
    """
NAME
snom_store_devices - store information about all the devices in HFDevices

DESCRIPTION
Stores all the information for each device registered to the hub in HFDevices  
    """
    global devices_list
    global hf_devices

    devices_list = []

    index = 0
    count = 5
    while True:
        resp = client_handle.get_dev_table(index=index, count=count)
        for dev in resp.devices:
            print(dev)
            devices_list.append(dev)

        # If there are fewer devices than we have asked for we have retrieved them all,
        # otherwise we need to move the index and check if there are more.
        if len(resp.devices) < count:
            break
        else:
            index += count

    for dev in devices_list:
        new_units_list = []
        for unit in dev.units:
            new_profile = HFProfiles().get_profile_by_id(unit.type) or HFProfile(profile_id=unit.type)
            new_interfaces_list = []  
            for interface in unit.interfaces:
                # unknown interfaces will add 'ID' to the interface list
                new_interface = hf_interfaces.get_interface_by_id(interface.id)
                if new_interface != None:
                    new_interfaces_list.append(new_interface)
                #print(new_interface)
                #log('interface {}, type={}'.format(hex(interface.id), hex(interface.type)))
            new_unit = HFUnit(unit_id=unit.id, unit_name=str(unit.id), profile=new_profile, interfaces=new_interfaces_list)
            new_units_list.append(new_unit)
            #print(new_unit)
        new_device = HFDevice(device_id=dev.id, device_name=str(dev.id), device_ipui=dev.ipui, units=new_units_list)
        hf_devices.update_device(new_device)
        
    print('####################################################')
    print('correct interface attributes depending on profile')
    print('apply known profile changes for interfaces')
    for dev in hf_devices.get_devices():
        print(f'update dev {dev.device_id}:{dev.device_name}')
        dev_update_profile_changes(dev)    
    print('####################################################')
    
    print(hf_devices)

def get_black_list_dev_table(client_handle, argv):
    """
NAME
get_black_list - list devices that are marked for deletion

SYNOPSIS
get_black_list

DESCRIPTION
Lists all the registered devices that are marked for deletion. If none,
reports number as zero.
    """

    index = 0
    count = 5
    devices = []

    while True:
        resp = client_handle.get_black_list_dev_table(index=index, count=count)
        devices += resp.devices

        # If there are fewer devices than we have asked for we have retrieved them all,
        # otherwise we need to move the index and check if there are more.
        if len(resp.devices) < count:
            break
        else:
            index += count

    num_blacklisted = len(devices)
    if num_blacklisted == 0:
        print("{} devices are black listed.".format(num_blacklisted))
    else:
        print("{} devices are black listed:".format(num_blacklisted))
        for dev in devices:
            print(dev)


def device_info(client_handle, argv):
    """
NAME
device_info - get information about a specified device

SYNOPSIS
device_info device_id

DESCRIPTION
List all the information for the device specified by device_id. Will return
a device ID error if the specified device is not registered.
    """

    if len(argv) < 2:
        print("device_info requires a device ID")
        return

    device_id = argv[1]
    try:
        int(device_id)
    except ValueError:
        print("The device ID has to be a number")
        return

    client_handle.get_dev_info(device_id)
    device_info_snom(client_handle, device_id)

def device_info_snom(client_handle, device_id):
    global hf_devices

    dev = hf_devices.get_device_by_id(int(device_id))
    if dev != None:
        for u in dev.units:
            if u.unit_id != 0:
                for i in u.interfaces:
                    attributes = i.server_attributes
                    for a in attributes:
                        send_cmd(client_handle, dev.device_id, u.unit_id, i.intrf_id, 4, a.attribute_id, '')

        print(dev)


def snom_attrib_info(client_handle, argv):
    """
    """

    if len(argv) < 5:
        print("attrib_info requires a device ID, unit ID, Interface ID, Attribute ID")
        return
    try:
        _, device_id, unit_id, interface_id, attribute_id = argv
        dev = hf_devices.get_device_by_id(int(device_id))
        unit = dev.get_unit_by_id(int(unit_id))
        interface = unit.get_interface_by_id(int(interface_id))
        attributes = interface.server_attributes
        try:
            attribute_id = int(attribute_id)
            # read the attribute from the device.
            send_cmd(client_handle, int(device_id), int(unit_id), int(interface_id), 4, attribute_id, '')
            print(interface.get_attribute_by_id(attribute_id))
        except:
            for attrb in attributes:
                print(attrb)
    except: 
        logging.exception(f'cannot find attribute or interface.') 
        print("attrib_info requires a device ID, unit ID, Interface ID, Attribute ID")
        return

def delete_device(client_handle, argv):
    """
NAME
delete - delete the registration of a device

SYNOPSIS
delete device_id [y]

DESCRIPTION
Deletes the registration for the specified device. Defaults to blacklist
deletion if the y parameter is not included. Returns a fail if the
specified device is not registered.

OPTIONS
delete device_id - blacklists the device for future deletion
delete device_id Y - locally deletes the device
delete device_id y - locally deletes the device
delete device_id * - where * is not y or Y, blacklists the device for future deletion
    """

    # Will default to blacklist deletion if there is no delete option in the
    # command or if the delete option is not y or Y

    if len(argv) < 2:
        print("delete requires a device ID")
        return

    device_id = argv[1]
    try:
        int(device_id)
    except ValueError:
        print("The device ID has to be a number")
        return

    if len(argv) == 3:
        local_delete = (argv[2].lower() == "y")
    else:
        local_delete = False

    client_handle.delete_dev(device_id, local=local_delete)
    # remove it from snom data 
    hf_devices.delete_device_by_id(device_id)


def start_voice_call(client_handle, argv):
    """
NAME
call - start a voice call with a device

SYNOPSIS
call device_id

DESCRIPTION
Starts a voice call with the specified device. Returns a fail if the specified
device is not registered.
    """
    if len(argv) < 2:
        print("This command requires a device ID")
        return

    device_id = argv[1]
    try:
        int(device_id)
    except ValueError:
        print("The device ID has to be a number")
        return

    client_handle.fun_msg(
        src_dev_id=0,
        src_unit_id=2,
        dst_dev_id=device_id,
        dst_unit_id=1,  # ULE Voice Call unit
        interface_type=1,  # server
        interface_id=0x7f11,  # ULE Voice Call interface
        interface_member=1,
    )
    log("Device {}: message has been queued for delivery ...".format(device_id))


def end_voice_call(client_handle, argv):
    """
NAME
release - end a specified voice call

SYNOPSIS
release call_id

DESCRIPTION
End a voice call with the specified call identity.
    """
    try:
        if (argv[1]).isdigit():
            client_handle.call_release(argv[1])
        else:
            print("The call ID (%s) has to be a number" % argv[1])
    except IndexError:
        print("release requires a call ID value")


def send_get_attribute_request(client_handle, argv):
    """
NAME
send_get_attribute_request - requests attribute with id from interface on device 

SYNOPSIS
send_get_attribute_request device_id unit_id interface attribute_id

DESCRIPTION
Requests an attribute from unit-interface on device 
    """

    if len(argv) == 6:
        _, device_id, unit_id, interface_id, attr_id, user_data = argv
        print("deviceID={}, interface_id={}, attr_id={} and data=({})".format(device_id, unit_id, interface_id, attr_id, user_data))
    if len(argv) == 5:
        _, device_id, unit_id, interface_id, attr_id = argv
        user_data = ''
        print("deviceID={}, interface_id={}, attr_id={} and data=({})".format(device_id, unit_id, interface_id, attr_id, user_data))
    if len(argv) < 5 or len(argv) > 6:
        print("send requires a device ID, unit_id, interface_id and attr_id")
        return

    try:
        device_id = int(device_id)
        unit_id = int(unit_id)
        interface_id = int(interface_id)
        attr_id = int(attr_id)
    except ValueError:
        print("The device ID ({}), unit_id={}, interface_id ({}) attr_id ({}) has to be a number".format(device_id, unit_id, interface_id, attr_id))
        return

    ## create attribute request 
    send_attr_request(client_handle, device_id, unit_id, interface_id, attr_id) # request attribute # ID
    log("Device {}: attribute {} requested on unit {}-interface {}. message has been queued for delivery ...".format(device_id, attr_id, unit_id, interface_id))


def send_get_attributes_pack_request(client_handle, argv):
    """
NAME
send_get_attributes_pack_request - requests mandatory and optional attributes from interface on device 

SYNOPSIS
send_get_attributes_pack_request device_id interface

DESCRIPTION
Requests and attribute from interface on device 
    """

    if len(argv) == 5:
        _, device_id, interface_id, attr_id, user_data = argv
        print("deviceID={}, interface_id={}, attr_id={} and data=({})".format(device_id, interface_id, attr_id, user_data))
    if len(argv) == 4:
        _, device_id, interface_id, attr_id = argv
        user_data = ''
        print("deviceID={}, interface_id={}, attr_id={} and data=({})".format(device_id, interface_id, attr_id, user_data))
    if len(argv) < 4 or len(argv) > 5:
        print("send requires a device ID, attr_id and optional data")
        return

    try:
        device_id = int(device_id)
        interface_id = int(interface_id)
    except ValueError:
        print("The device ID ({}), interface_id ({}) has to be a number".format(device_id, interface_id, attr_id))
        return

    ## create attribute request 
    send_attr_pack_request(client_handle, int(device_id), int(interface_id)) # request all attributes # ID
    log("Device {}: attribute pack requested on interface {}. message has been queued for delivery ...".format(device_id, interface_id))


def send_report(client_handle, argv):
    """
NAME
send - send a cmd to a device

SYNOPSIS
send device_id cmd data_string

DESCRIPTION
Send a cmd with paylad=character string from the hub to the specified device. The string
must not contain spaces. Returns a fail if the specified device is not
registered.
    """

    if len(argv) == 5:
        _, device_id, interface_id, cmd_id, user_data = argv
        print("deviceID={}, interface_id={}, CMD_ID={} and data=({})".format(device_id, interface_id, cmd_id, user_data))
    if len(argv) == 4:
        _, device_id, interface_id, cmd_id = argv
        user_data = ''
        print("deviceID={}, interface_id={}, CMD_ID={} and data=({})".format(device_id, interface_id, cmd_id, user_data))
    if len(argv) < 4 or len(argv) > 5:
        print("send requires a device ID, CMD_ID and optional data")
        return

    try:
        device_id = int(device_id)
        cmd_id = int(cmd_id)
        interface_id = int(interface_id)
    except ValueError:
        print("The device ID ({}), interface_id ({}) and the CMD_ID ({}) has to be a number".format(device_id, interface_id, cmd_id))
        return

    # report data 
    octets = []
    report_type = 0x01 # Periodic 0, Event = 1
    report_ID = 0x11 & 0x7e # 15 bit id
    octets.append((report_type << 8) + report_ID)
    
    address_type = 0x00 # individual = 0, Group = 1, 
    receiver_device_address = 0x6
    octets.append((address_type << 8) + (receiver_device_address >> 8) & 0x7f)
    octets.append(receiver_device_address & 0xff) # LSB
    
    receiver_unit_ID = 0x01 # 
    octets.append(receiver_unit_ID & 0xff) # reciever unit id
    
    periodic_interval_value = 0x10 # 10s
    octets.append((periodic_interval_value >> 24) & 0xff)
    octets.append((periodic_interval_value >> 16) & 0xff)
    octets.append((periodic_interval_value >> 8) & 0xff)
    octets.append(periodic_interval_value & 0xff) # LSB

    number_of_report_entries = 0x01 # try one first
    octets.append(number_of_report_entries & 0xff) # number of report entries
    
    # periodic report entry
    unit_id = 0x01 
    octets.append(unit_id & 0xff) # interface uid temperature

    interface_type = 0x00 # client = 0, server = 1
    interface_uid = 0x0301
    octets.append((interface_type << 8) + (interface_uid >> 8) & 0x7f)
    
    attribute_pack_uid = 0xFE # store mandatory and optional  
    octets.append(attribute_pack_uid & 0xff)

    ## create periodic report 
    octets = []
    address_type = 0x00 # individual = 0, Group = 1, 
    receiver_device_address = 0x00 # 
    octets.append((address_type << 8) + (receiver_device_address >> 8) & 0x7f)
    octets.append(receiver_device_address & 0xff) # LSB
    
    receiver_unit_ID = 0x06 # 
    octets.append(receiver_unit_ID & 0xff) # reciever unit id

    periodic_interval_value = 10 # 10s
    octets.append((periodic_interval_value >> 24) & 0xff)
    octets.append((periodic_interval_value >> 16) & 0xff)
    octets.append((periodic_interval_value >> 8) & 0xff)
    octets.append(periodic_interval_value & 0xff) # LSB

    stringoct = str(bytes(octets), encoding='ISO-8859-1')

    send_cmd(client_handle, device_id, 0, 6 , 1, 1, stringoct) # create periodic report 
    log("Device {}: CMD={} message has been queued for delivery ...".format(device_id, cmd_id))


def get_attribute_pack(client_handle, argv):
    """
    NAME
    get_attribute_pack - send attribute pack request to device

    SYNOPSIS
    get_attribute_pack device_id interface_id 

    DESCRIPTION
    Sends get attribute pack request to interface_id on device device_id.
    Returns a fail if the specified device is not registered.
    """

    ## does nothing, but looks correct
    # get_attributes_pack 12 1 517 9 254 
    #
    if len(argv) == 7: 
        _, device_id, unit_id, interface_id, msg_type, cmd_id, user_data = argv
    elif len(argv) == 6: 
        _, device_id, unit_id, interface_id, msg_type, cmd_id = argv
        user_data = ''
    else:
        print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={} data=""')".format(0,0,0,0,0))
        print('with or without data')
   
    # get All attribute pack
    print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={}, data={})".format(device_id, unit_id, interface_id, msg_type, cmd_id, user_data))
    #send_cmd(client_handle, int(device_id), int(unit_id), int(interface_id), int(msg_type), 0xFE, user_data) # create periodic report 
    send_cmd(client_handle, int(device_id), int(unit_id), int(interface_id), int(msg_type), int(cmd_id), user_data) # create periodic report 

    ### try get attrib
    #send_attr_pack_request(client_handle, int(device_id), int(interface_id)) # request attribute # ID
    #log("Device {}: attribute pack requested on interface {}. message has been queued for delivery ...".format(device_id, interface_id))
    return

# snom_send_cmd 12 1 517 1 1 a
def snom_send_cmd(client_handle, argv):
    global hf_devices
    global hf_interfaces

    device_id = unit_id = interface_id = msg_type = cmd_id = 0

    if len(argv) >= 7:
        device_id = argv[1]
        unit_id = argv[2]
        interface_id = argv[3]
        msg_type = argv[4]
        cmd_id = argv[5]
        try:
            user_data =  bytes([int(x) for x in argv[6:]])
            user_data = str(bytes(user_data), encoding='ISO-8859-1')
        except:
            print(f'snom_send_cmd: invalid data:{argv[6:]}, need int[0,255]')
            return


        #_, device_id, unit_id, interface_id, msg_type, cmd_id, user_data = argv
        
        print(f'data in ints = {user_data}')

    elif len(argv) == 4: # get the list of known commands
        print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={} data=""')".format(0,0,0,0,0))
        try: 
            device = unit = interface = None
            _, device_id, unit_id, interface_id = argv
            device_id = int(device_id)
            interface_id = int(interface_id)
            device = hf_devices.get_device_by_id(device_id)
            if device:
                unit = device.get_unit_by_id(int(unit_id))
            if unit:
                interface = unit.get_interface_by_id(interface_id)
            for i in range(16):
                print(f'{i:02}: {global_message_type(i)}')
            print('Commands----------------------------')
            if interface:
                interface.pp_c2s_cmds()
            print('Server Commands---------------------')
            if interface:
                interface.pp_s2c_cmds()
            print('Attributes--------------------------')
            if interface:
                print(interface.server_attributes)
            return
        except:
            print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={}, data={})".format(device_id, unit_id, interface_id, msg_type, cmd_id, user_data))
            logging.exception('number conversion didnt work')
        return
    else:
        return
    
    # we got user data as ints 
    try: 
        device_id = int(device_id)
        interface_id = int(interface_id)
        unit_id = int(unit_id)
        msg_type = int(msg_type)
        cmd_id = int(cmd_id)
    except:
        print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={}, data={})".format(0,0,0,0,0,0))
        logging.exception('number conversion didnt work')
    print("deviceID={}, unitID={}, interfaceIDn={}, msg_type={}, cmdID={}, data={})".format(device_id, unit_id, interface_id, msg_type, cmd_id, user_data))
    send_cmd(client_handle, device_id, unit_id, interface_id, msg_type, cmd_id, user_data)


def send_blind_level(client_handle, argv):
    """
NAME
send_blind_level - sends ldevicesevel of blind 0=closed, 0xff=open

SYNOPSIS
send_blind_level device_id level

DESCRIPTION
Sends several commands to control a becker blind
    """
    if len(argv) == 3:
        _, device_id, level = argv
        print("deviceID={}, and level=({})".format(device_id, level))
    elif len(argv) < 3 or len(argv) > 3:
        print("send requires a device ID, level and data [0,255]")
        return

    try:
        device_id = int(device_id)
        level = int(level)
    except ValueError:
        print("The device ID ({}), and the level ({}) has to be a number".format(device_id, level))
        return
    
    # level
    #if (level == -1)    
    #    send_cmd(client_handle, device_id, 1, 513, 3, 1, '')
    #if (level == 1)    
    #    send_cmd(client_handle, device_id, 1, 513, 2, 1, '')

    # read the current level 
    # server cmd 
    #send_cmd_to_server(client_handle, device_id, 1, 513, 1, 1, '')

    octets = []
    val = level # 255 close   
    octets.append(val & 0xff) # LSB
    stringoct = str(bytes(octets), encoding='ISO-8859-1')
    user_data = stringoct
    # send CMD_ID=1, SetLevel
    send_cmd(client_handle, device_id, 1, 513, 1, 1, user_data)


def send_bulb_color(client_handle, argv):
    """
NAME
send_bulb_color - sends color, saturation, transition data and level to light bulb

SYNOPSIS
send_bulb_color device_id color saturation transition level

DESCRIPTION
Sends several commands to a Colour Control Interface to change color, saturation and level of a light bulb.
    """
    if len(argv) == 6:
        _, device_id, color, saturation, transition, level = argv
        print("deviceID={}, color={}, saturation={}, transition={} and level=({})".format(device_id, color, saturation, transition, level))
    elif len(argv) < 6 or len(argv) > 6:
        print("send requires a device ID, color, saturation, transition, level and dummy user_data")
        return

    try:
        device_id = int(device_id)
        color = int(color)
        saturation = int(saturation)
        transition = int(transition)
        level = int(level)
    except ValueError:
        print("The device ID ({}), color ({}), saturation ({}), the transition ({}) and the level ({}) has to be a number".format(device_id, color, saturation, transition, level))
        return
    
    # light bulb Hue
    octets = []
    val = color # blue    
    octets.append((val >> 8) & 0xff)
    octets.append(val & 0xff) # LSB
    direction = 4
    octets.append(direction & 0xff) # LSB
    time = transition # * 100 msecs
    octets.append((time >> 8) & 0xff)
    octets.append(time & 0xff) # LSB
    
    stringoct = str(bytes(octets), encoding='ISO-8859-1')
    user_data = stringoct
    # send CMD_ID=1, MoveToHue
    send_cmd(client_handle, device_id, 1, 514, 1, 1, user_data)

    # Saturation
    octets = []
    val = saturation    
    octets.append(val & 0xff) # LSB
    direction = 1
    octets.append(direction & 0xff) # LSB
    time = transition # * 100 msecs
    octets.append((time >> 8) & 0xff)
    octets.append(time & 0xff) # LSB
    
    stringoct = str(bytes(octets), encoding='ISO-8859-1')
    user_data = stringoct
    # send CMD_ID=4, MoveToSaturation
    send_cmd(client_handle, device_id, 1, 514, 1, 4, user_data)

    # level
    octets = []
    val = level # 255 fully bright    
    octets.append(val & 0xff) # LSB
    stringoct = str(bytes(octets), encoding='ISO-8859-1')
    user_data = stringoct
    # send CMD_ID=1, SetLevel
    send_cmd(client_handle, device_id, 1, 513, 1, 1, user_data)

def send_cmd_data(client_handle, argv):
    """
NAME
send - send a cmd to a device

SYNOPSIS
send device_id cmd data_string

DESCRIPTION
Send a cmd with paylad=character string from the hub to the specified device. The string
must not contain spaces. Returns a fail if the specified device is not
registered.
    """
    
    ### try report.. 
    #send_report(client_handle, argv)
    #return

    if len(argv) == 7:
        _, device_id, unit_id, interface_id, msg_type, cmd_id, user_data = argv
        print("deviceID={}, unit_id={}, interface_id={}, msg_type={}, CMD_ID={} and data=({})".format(device_id, unit_id, interface_id, msg_type, cmd_id, user_data))
    if len(argv) == 6:
        _, device_id, unit_id, interface_id, msg_type, cmd_id = argv
        user_data = ''
        print("deviceID={}, unit_id={}, interface_id={}, msg_type={}, CMD_ID={} and data=({})".format(device_id, unit_id, interface_id, msg_type, cmd_id, user_data))
    if len(argv) < 6 or len(argv) > 7:
        print("send requires a device ID, unit_id, interface_id, CMD_ID and optional data")
        return

    try:
        device_id = int(device_id)
        cmd_id = int(cmd_id)
        unit_id = int(unit_id)
        interface_id = int(interface_id)
        msg_type = int(msg_type)
    except ValueError:
        print("The device ID ({}), interface_id ({}), msg_type ({}) and the CMD_ID ({}) has to be a number".format(device_id, interface_id, msg_type, cmd_id))
        return

    # light bulb HUE change to 10 degrees (Blue?)
    '''octets = []
    hue = 240 # blue    
    octets.append((hue >> 8) & 0xff)
    octets.append(hue & 0xff) # LSB
    direction = 4
    octets.append(direction & 0xff) # LSB
    time = 10 # * 100 msecs
    octets.append((time >> 8) & 0xff)
    octets.append(time & 0xff) # LSB
    
    stringoct = str(bytes(octets), encoding='ISO-8859-1')
    user_data = stringoct
    '''
    print(user_data)
    send_cmd(client_handle, device_id, unit_id, interface_id, msg_type, cmd_id, user_data)
    log("Device {}: CMD={} with msg_type={} message has been queued for delivery ...".format(device_id, cmd_id, msg_type))


def send_user_data(client_handle, argv):
    """
NAME
send - send a string to a device

SYNOPSIS
send device_id data_string

DESCRIPTION
Send a character string from the hub to the specified device. The string
must not contain spaces. Returns a fail if the specified device is not
registered.
    """

    if len(argv) == 3:
        _, device_id, user_data = argv
    else:
        print("send requires a device ID and data")
        return

    try:
        device_id = int(device_id)
    except ValueError:
        print("The device ID ({}) has to be a number".format(device_id))
        return

    send_data(client_handle, device_id, user_data)
    log("Device {}: message has been queued for delivery ...".format(device_id))


def debug_print(client_handle, argv):
    """
NAME
debug_print - switches extra print output from the HAN client on or off

SYNOPSIS
debug_print on|off

DESCRIPTION
Switches on or off print messages from within the HAN client that may be useful
when debugging programs using the API.

OPTIONS
debug_print on - switches debug printing on
debug_print off - switches debug printing off

    """
    if len(argv) != 2:
        print("debug_print needs a on/off parameter.")
        return

    if argv[1] == "on":
        client_handle.set_debug_printing(1)
    else:
        client_handle.set_debug_printing(0)


def open_reg(client_handle, argv):
    """
NAME
open_reg - open the hub for registration

SYNOPSIS
open_reg [open_duration]

DESCRIPTION
Open the hub to allow a device to register. The hub remains open
for 120 seconds or the time specified in the open_duration parameter.

OPTIONS
open_reg - opens the base for 120 seconds
open_reg * - opens the base for * seconds
    """
    open_duration = "120"   # default value

    if len(argv) == 2:
        if not argv[1].isdigit():
            print("The open duration (%s) has to be a number" % argv[1])
            return
        else:
            open_duration = argv[1]

    resp = client_handle.open_reg(open_duration)
    if resp.success:
        log("Registration window open")
    else:
        log("Error: failed to open registration window")


def close_reg(client_handle, argv):
    """
NAME
close_reg - close the hub to prohibit registration

SYNOPSIS
close_reg

DESCRIPTION
Forces the hub to close to prohibit registration.
    """
    resp = client_handle.close_reg()
    if resp.success:
        log("Registration window closed")
    else:
        log("Error: failed to close registration window")


def get_software_version(client_handle, argv):
    """
NAME
get_sw_version - get the hub software version

SYNOPSIS
get_sw_version

DESCRIPTION
Get the hub software version
    """
    client_handle.get_sw_version()


def get_hardware_version(client_handle, argv):
    """
NAME
get_hw_version - get the hub hardware version

SYNOPSIS
get_hw_version

DESCRIPTION
Get the hub hardware version
    """
    client_handle.get_hw_version()


def list_eeprom_parameters(list_all):
    for param in han_client.EEPROM_PARAMS:
        length = han_client.EEPROM_PARAMS[param]
        if (length > 0) or list_all:
            print(param)


def get_eeprom_parameter(client_handle, argv):
    """
NAME
get_eeprom_parameter - gets the value of an EEPROM parameter or all parameters

SYNOPSIS
get_eeprom_parameter <parameter | 'list' | 'all'>

DESCRIPTION
Shows the value of the specified EEPROM  parameter or lists the names of all the
EEPROM parameter or shows the value of all the EEPROM paramters.

OPTIONS
get_eeprom_parameter EEPROM_paramter_name - returns the value of the specified parameter
get_eeprom_parameter list - lists the names of all the EEPROM paramters
get_eeprom_parameter all - lists the name and value of all the EEPROM parameters
    """
    if len(argv) != 2:
        print("get_eeprom_parameter needs a parameter.")
        return

    if argv[1] == "list":
        list_eeprom_parameters(True)
    elif argv[1] == "all":
        for name in han_client.EEPROM_PARAMS:
            client_handle.get_eeprom_parameter(name)
    else:
        param = argv[1].upper()  # EEPROM parameter names are upper case
        if param in han_client.EEPROM_PARAMS:
            client_handle.get_eeprom_parameter(param)
        else:
            print("Error: unknown EEPROM parameter: '{}'".format(param))


def set_eeprom_parameter(client_handle, argv):
    """
NAME
set_eeprom_parameter - sets the value of an EEPROM parameter

SYNOPSIS
set_eeprom_parameter <parameter hex_value | 'list'>

DESCRIPTION
Set the value of the specified EEPROM paramter. Returns an error if the parameter
does not exist or is not writable, or if the hex_value length does not match the
required length (for example if a parameter requires a 16 bit value entering ABC
will cause and error where 0ABC will be accepted). Or list all the writable
EEPROM parameters.

OPTIONS
set_eeprom_parameter parameter hex_value - sets the parameter to hex_value
set_eeprom_parameter list - lists all the writable EEPROM parameters
    """
    if len(argv) < 2:
        print("set_eeprom_parameter needs more parameters.")
        return

    if argv[1] == "list":
        # Only going to list eeprom paramters that are writable
        list_eeprom_parameters(False)
        return
    else:
        param = argv[1].upper()  # EEPROM parameter names are upper case

    if eeprom_parameter_is_writable(param):
        if len(argv) != 3:
            print("set_eeprom_parameter needs two parameters.")
            return
    else:
        # No point in checking anything else if we can't write to this parameter
        return

    value = argv[2]

    if eeprom_request_valid(param, value):
        resp = client_handle.set_eeprom_parameter(param, value)
        if resp.params["STATUS"] == "SUCCEED":
            print("EEPROM parameter '{}' updated.".format(param))
        else:
            print("Error: failed updating EEPROM parameter '{}'".format(param))


def eeprom_parameter_is_writable(eeprom_parameter):
    """
    Check the eeprom parameter exists and that it is writeable
    """

    is_writable = True

    if eeprom_parameter not in han_client.EEPROM_PARAMS:
        print("Error: unknown EEPROM parameter: '{}'".format(eeprom_parameter))
        is_writable = False
    else:
        length = han_client.EEPROM_PARAMS[eeprom_parameter]
        if length == 0:
            print("Error: read-only EEPROM parameter: '{}'".format(eeprom_parameter))
            is_writable = False

    return is_writable


def eeprom_request_valid(param, value):
    """
    Check that the user supplied values are acceptable.
    """

    request_valid = True

    try:
        int(value, 16)    # check this is a hex number, will hit except ValueError if not
    except ValueError:
        print("Error: the value must be a hex number")
        request_valid = False

    length = han_client.EEPROM_PARAMS[param]
    if not len(value) == length * 2:
        print("Error: the value is the wrong length, it has to be {} bytes".format(length))
        request_valid = False

    return request_valid

import subprocess

def end_han_app(client_handle, argv):
    """
NAME
q - exit the han_app program

SYNOPSIS
q

DESCRIPTION
Exit the han_app program.
    """
    print("Quitting HAN Client...")
    client_handle.destroy()
    # stop the mqtt thread
    print("Quitting MQTT Client...")
    mqttc.loop_stop()
    print("Kill all han_app related processes...")
    subprocess.check_call(['pkill', '-f', '-9', 'han_app'])

    sys.exit(0)


def command_not_found(client_handle, argv):
    print("'{}' is not a command. 'help' for a list of commands".format(argv[0]))


def handle_Controllable_Thermostat(device_id, data):
    print(data)

def handle_light_bulb(device_id, message_type, interface_id, interface_member, hl):
     if message_type == 5:
        log(f'Light Bulp: Attribute={interface_member} on interface_id={interface_id}, with data={hl} received.')
        log(f'Response {global_response_code(hl[0])} to Cmd={interface_member} on interface_id={interface_id}, with data={hl[1:]} ')

        dev = hf_devices.get_device_by_id(device_id)
        interface = dev.get_unit_by_id(1).get_interface_by_id(interface_id)
        if len(hl) >= 2:
            # cut the code
            interface.set_attribute_values_by_id(interface_member, hl[1:])
        print(interface.get_attribute_by_id(interface_member))
  
def handle_blind(device_id, message_type, interface_id, interface_member, data):
    global hf_interfaces
    print('Blind Server data arrived')

    if message_type == 1:
        interface = hf_interfaces.get_interface_by_id(interface_id)
        log(f'Blind: Server CMD={interface_member} for interface="{interface.intrf_name}" with data={data} received.')
        try:
            state = int.from_bytes(bytes(data), "big")
            s2c_cmds = interface.get_s2c_cmds()
            scmd = interface.get_s2c_cmd_by_ref(1)
            if scmd:
                log(f'fire action {scmd.cmd_action}')
                try:
                    eval(scmd.cmd_action)
                except:
                    logging.exception('ACTION not found!')

        except:
            logging.exception(f'cannot convert data={data}')
    '''
    #  MSGTYPE:  5  <------ server sends Attribute state to client
    elif message_type == 5 or message_type == 8:
        log(f'Blind: Attribute={interface_member} on interface_id={interface_id}, with data={data} received.')
        log(f'Response {global_response_code(data[0])} to Cmd={interface_member} on interface_id={interface_id}, with data={data[1:]} ')

        dev = hf_devices.get_device_by_id(device_id)
        interface = dev.get_unit_by_id(1).get_interface_by_id(interface_id)
        if len(data) >= 2:
            # cut the code
            interface.set_attribute_values_by_id(interface_member, data[1:])
        print(interface.get_attribute_by_id(interface_member))
    elif message_type == 3:
        log(global_response_code(data[0]))
        log(f'Response {data[0]} to Cmd={interface_member} on interface_id={interface_id}, with data={data[1:]} ')
    else:
        log(f'Blind: Messasge_type={message_type}, interface_id={interface_id}, with data={data} received.')
    '''

def handle_Simple_Power_Metering_Interface(device_id, data):
    '''
        18:09:22.633 LEN 48
        18:09:22.633 data raw 
        09 report ids
        01:
        00 00 00 01 72 
        02:
        00 00 00 00 00 
        03:
        00 00 00 00 00 
        04: 
        10 00 01 1a 8f 
        07:
        10 00 03 91 2a 
        08: 
        10 00 00 02 00
        09: 
        00 00 00 00 32 
        0a:
        9a 0b 03 84

    '''
    energy = 0
    energy_lreset = 0
    time_lreset = 0
    inst_power = 0
    average_power = 0
    average_power_int = 0
    voltage = 0
    current = 0
    frequency = 0
    power_factor = 0
    report_interval = 0

    # iterate over all available IDs
    number_of_Attributes = data[0]
    idx = 1 
    for i in range(number_of_Attributes):
        attributeID = data[idx]
        log("Attribute ID:{}".format(attributeID))
        idx += 1 
        # illegal ID?
        if attributeID > 0x0b:
            log("Simple_Power_Metering_Interface: unknown Attribute ID {}".format(attributeID))
            continue
        # ID ok
        if attributeID == 0x01:
            # Energy U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            energy = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue

        if attributeID == 0x02:
            # Energy at Last Reset U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            energy_lreset = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue

        if attributeID == 0x03:
            # Time at Last Reset U8+U32
            log("Table 40:{}".format(data[idx]))
            idx += 1 
            time_lreset = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue

        if attributeID == 0x04:
            # Instantaneous Power U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            inst_power = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue

        if attributeID == 0x05:
            # Average Power U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            average_power = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue

        if attributeID == 0x06:
            # Average Power Interval U16
            average_power_int = (data[idx] << 8) + data[idx+1]
            idx += 2
            continue
 
        if attributeID == 0x07:
            # Voltage U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            voltage = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue
     
        if attributeID == 0x08:
            # Current U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            current = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue
            
        if attributeID == 0x09:
            # Frequency U8+U32
            log("Table 39:{}".format(data[idx]))
            idx += 1 
            frequency = (data[idx] << 24) + (data[idx+1] << 16) + (data[idx+2] << 8) + data[idx+3]
            idx += 4
            continue
        
        if attributeID == 0x0A:
            # Power Factor U8
            power_factor = data[idx]
            idx += 1
            continue
     
        if attributeID == 0x0B:
            # Report Interval U16
            report_interval = (data[idx] << 8) + data[idx+1]
            idx += 2
            continue
     
    log("Energy Watt/h:{}".format(energy))
    log("last reset Watt/h:{}".format(energy_lreset))
    log("time last reset (s):{}".format(time_lreset))
    log("Instantaneous power (W):{}".format(inst_power))
    log("Average power: (W){}".format(average_power))
    log("Average power intervall (s):{}".format(average_power_int))
    log("Voltage: (V){}".format(voltage))
    log("Current (A):{}".format(current))
    log("Frequency (Hz):{}".format(frequency))
    log("Power Factor:{}".format(power_factor))
    log("Report Interval (s):{}".format(report_interval))
    
    mqttc.send_simple_power_meter_data('SPMI', device_id, int(energy), 
                                        energy_lreset, time_lreset,
                                        inst_power, average_power, average_power_int,
                                        voltage, current, 
                                        frequency, power_factor, 
                                        report_interval)

def snom_handle_x0300_report(device_id, unit_id, interface_id, cmd_id, hl):
    log(f"Meter Report with data={hl}")
    handle_Simple_Power_Metering_Interface(device_id, hl)


def snom_handle_x0100_state(device_id, unit_id, interface_id, cmd_id, hl):
    log("Window Open Close Detector")
    # read profile from payload
    profile = (hl[0] << 8) + hl[1]
    print(f'incoming server command for profile={profile}:{hex(profile)}')
    # U32: Table 5 - Data Ordering of Payload of a Status Command
    proximity = (hl[2] << 24) + (hl[3] << 16) + (hl[4] << 8) + hl[5]

    # store data in hf
    unit = hf_devices.get_device_by_id(device_id).get_unit_by_id(unit_id)
    unit.get_interface_by_id(interface_id).set_attribute_values_by_id(1, hl[2:])
    print(unit.get_interface_by_id(interface_id).get_attribute_by_id(1))

    
    # shoot mqtt message
    mqttc.send_sensor_data('WOCD', device_id, int(proximity))
    log("proximimty:{}".format(proximity))
        


def snom_handle_server_cmds(device_id, unit_id, interface_id, cmd_id, hl):
    interface = hf_interfaces.get_interface_by_id(interface_id)
    s2c_cmd = interface.get_s2c_cmd_by_id(cmd_id)
    command_routine = s2c_cmd.cmd_action
    payload = hl
    try:
        eval(command_routine)
    except:
        print(f'action {command_routine} on server command {cmd_id} for interface {interface_id} is not possible!')
    return

def  get_list_id_by_device_id(device_id: int) -> int:
    for idx, dev in enumerate(devices_list):
        if dev.id == device_id:
            return idx
    return -1

def snom_handle_fun_msg(client, msg):
    device_id = int(msg.params["SRC_DEV_ID"])
    unit_id = int(msg.params["SRC_UNIT_ID"])
    interface_id = int(msg.params["INTRF_ID"])
    interface_member = int(msg.params["INTRF_MEMBER"])
    message_type = int(msg.params["MSGTYPE"])
    
    list_id = get_list_id_by_device_id(device_id)
    if list_id == -1:
        print('FATAL, dev id not found')
        return 

    profile = devices_list[list_id].units[unit_id].type
    profile_type = devices_list[list_id].units[unit_id].type

    print('Profile: device_id={}, src_unit_idx={}: unit_id={}, profile_type={}, interface={}'.format(device_id, unit_id,
            hex(devices_list[list_id].units[unit_id].id),
            hex(devices_list[list_id].units[unit_id].type),
            hex(interface_id)))

    log(f'MSGTYPE received={global_message_type(int(message_type))}:{message_type}')
    if message_type == 8:
        # answer to set attribute
        datastr=msg.params["DATA"]
        hl = [int(x, 16) for x in datastr.split(' ')]
        print(f'Response to Set Attribute {global_response_code(hl[0])}')
        log("data raw {}".format(hl))
        #return   
    if message_type == 5:
        # answer to get attribute
        datastr=msg.params["DATA"]
        hl = [int(x, 16) for x in datastr.split(' ')]
        if hl[0] == 0x03:
            log("get attr not supported".format(hl))    
        log("data raw {}".format(hl))
        #return   
    if unit_id == 0:
        if interface_id == 0x0115:
            # device management unit, keep-alive interface
            log("Device {}: keep alive".format(device_id))
        
        if profile_type == 0x0411:
            # generic application logic interface
            log("Device {}: generic application logic".format(device_id))
        
        if profile_type == 0x0400:
            # software update over the air
            log("Device {}: SUOTA ".format(device_id))

    if unit_id != 0:
        log("Device {}: message from unit {}".format(device_id, unit_id))
        #for entry in msg.params:
        #    log("param {}".format(entry))
        log("LEN {}".format(int(msg.params["DATALEN"])))
        log("data raw {}".format(msg.params["DATA"]))
        datastr=msg.params["DATA"]

        hl = [int(x, 16) for x in datastr.split(' ')]
        if len(hl) == 0:
            profile = devices_list[list_id].units[unit_id].type
            print('Profile: # unit={}, unit_id={}, profile(type)={}'.format(unit_id,
            hex(devices_list[list_id].units[unit_id].id),
            hex(profile)))
            log('we got as message from unit={},profile={} without payload'.format(unit_id, hex(profile)))
        # incoming message from device!     
        if message_type == 1: # server command
            try:
                datastr=msg.params["DATA"]
                hl = [int(x, 16) for x in datastr.split(' ')]
            except:
                hl=[]
            snom_handle_server_cmds(device_id, unit_id, interface_id, interface_member, hl)

        if message_type == 5:
            log(f'profile={profile} Attribute={interface_member} on interface_id={interface_id}, with data={hl} received.')
            log(f'Response {global_response_code(hl[0])} to Cmd={interface_member} on interface_id={interface_id}, with data={hl[1:]} ')

            dev = hf_devices.get_device_by_id(device_id)
            interface = dev.get_unit_by_id(unit_id).get_interface_by_id(interface_id)
            if len(hl) >= 2:
                # cut the code
                interface.set_attribute_values_by_id(interface_member, hl[1:])
            print(interface.get_attribute_by_id(interface_member))
    
   
        #if profile == 0x107:
        #    handle_Simple_Power_Metering_Interface(device_id, hl)
        #    return

        if profile == 0x0111:
            # Simple Button Interface Server commands.
            command = interface_member
            if command == '1':  
                log('Button pressed')
                mqttc.send_simple_button_data('SB', f'{device_id}-SBP', 'SBP')

            if command == '2':  
                log('Button long pressed')
                mqttc.send_simple_button_data('SB', f'{device_id}-LBP', 'LBP')

            if command == '3':  
                log('Button extra long pressed')
                mqttc.send_simple_button_data('SB', f'{device_id}-EBP', 'EBP')

            if command == '4':  
                log('Button double pressed')
                mqttc.send_simple_button_data('SB', f'{device_id}-DBP', 'DBP')

        if profile == 0x112:
            handle_Controllable_Thermostat(device_id, hl)
            return

        if profile == 0x0117:
            log("Simple_Power_Metering_Interface")
            handle_Simple_Power_Metering_InterfaceXXXXXXXX()

        if profile == 0x0116:
            log("Light Bulb")
            log(f'MSGTYPE={global_message_type(int(message_type))}')
            handle_light_bulb(device_id, message_type, interface_id, interface_member, hl)

        if profile == 0x0119:
            log("Blind")
            log(f'MSGTYPE={global_message_type(int(message_type))}')
            # U32: Table 5 - Data Ordering of Payload of a Status Command
            handle_blind(device_id, message_type, interface_id, interface_member, hl)

        if profile == 0x0203:
            log("Motion Detector, Status Command from Server")
            log(f'MSGTYPE={message_type}')
            # read profile UID from payload
            profile_t = (hl[0] << 8) + hl[1]
            
            # U32: Table 5 - Data Ordering of Payload of a Status Command
            proximity = (hl[2] << 24) + (hl[3] << 16) + (hl[4] << 8) + hl[5]
            log(f"Status Command from Server, profile UID={profile_t}, proximity={proximity}")
    
            # shoot mqtt message
            mqttc.send_sensor_data('MD', device_id, int(proximity))

        if profile == 0x0202:
            log("Window Open Close Detector")
            # read profile from payload
            profile = (hl[0] << 8) + hl[1]
            
            # U32: Table 5 - Data Ordering of Payload of a Status Command
            proximity = (hl[2] << 24) + (hl[3] << 16) + (hl[4] << 8) + hl[5]
            
            # shoot mqtt message
            mqttc.send_sensor_data('WOCD', device_id, int(proximity))
            log("proximimty:{}".format(proximity))
            
    
    if unit_id == 2:
        # smoke unit
        log("Device {}: message from smoke unit".format(device_id))
        
    if unit_id == 3:
        # ULEasy unit (raw data)
        data = msg.data.decode("utf-8")
        log("Device {}: message from raw data unit: '{}'".format(device_id, data))

    if unit_id == 4:
        # ??
        data = msg.data.decode("utf-8")
        log("Device {}: message from xx data unit: '{}'".format(device_id, data))


def send_test_mqtt(client_handle, argv):
    try:
        prox = int(argv[1])
        mqttc.send_sensor_data('MD', '1', prox)
        mqttc.send_sensor_data('WOCD', '2', prox)
        print("'{}' send to mqtt.".format(prox))
    except:
        mqttc.send_sensor_data('MD', '1', 0)
        mqttc.send_sensor_data('WOCD', '2', 0)
        print("'0' send to mqtt.")


def help_on_commands(client_handle, argv):
    """
NAME
help - prints help on API commands

SYNOPSIS
help [command]

DESCRIPTION
Prints a list of all the available commands or help on a specified command.

OPTIONS
help - prints a list of all the commands
help command - prints help on the specified command
    """
    if len(argv) < 2:
        print("The following commands are available, 'help cmd' for more information")
        command_list = commands.keys()
        for command in command_list:
            print('  ' + command)
    else:
        try:
            cmd = commands.get(argv[1])
            print(cmd.__doc__)
        except TypeError:
            print("'{}' is not a command. 'help' for a list of commands".format(argv[1]))


commands = {
    'open_reg': open_reg,
    'close_reg': close_reg,
    'send': send_user_data,
    'send_cmd': send_cmd_data,
    'get_attribute': send_get_attribute_request,
    'get_attributes_pack': get_attribute_pack,
    'call': start_voice_call,
    'release': end_voice_call,
    'devices': list_devices,
    'device_info': device_info,
    'attribute_info': snom_attrib_info,
    'snom_send_cmd': snom_send_cmd,
    'delete': delete_device,
    'get_black_list': get_black_list_dev_table,
    'get_sw_version': get_software_version,
    'get_hw_version': get_hardware_version,
    'get_eeprom_parameter': get_eeprom_parameter,
    'set_eeprom_parameter': set_eeprom_parameter,
    'debug_print': debug_print,
    'help': help_on_commands,
    'q': end_han_app,
    'mqtt': send_test_mqtt,
    'send_bulb_color': send_bulb_color,
    'send_blind_level': send_blind_level,
}

# HF class
global hf_interfaces
hf_interfaces = HFInterfaces()
hf_interfaces.create_known_interfaces()
  
global hf_devices
hf_devices = HFDevices()

# MQTT MQTT MQTT
from snom_sss_mqtt_hassio import snomSSSMqttHasssioClient
mqttc = snomSSSMqttHasssioClient()
if False:
    rc = mqttc.connect_and_subscribe('10.110.11.72', 1883, 'mqtt_user', 'mqtt_user')
    print("mqtt response:", rc)
    mqttc.loop_start()

import threading

#class MainThread(threading.Thread):
class MainThread(threading.Thread):

    def __init__(self, name='main-thread'):
        super(MainThread, self).__init__(name=name)
        self.start()

    def run(self):
        while True:
            # do nothing, unblock main
            time.sleep(1)
                    
      
def my_callback(inp):
    #evaluate the keyboard input
    print('You Entered:', inp, ' Counter is at:', showcounter)


import time

def main():

    client_handle = han_client.HANClient()
    print("1", client_handle)
    client_handle.set_debug_printing(1)
    #client_handle.set_rx_message_callback(process_rx_data)
    client_handle.subscribe("dev_registered", handle_dev_registered)
    client_handle.subscribe("reg_closed", handle_reg_closed)
    #client_handle.subscribe("fun_msg", handle_fun_msg)
    client_handle.subscribe("fun_msg", snom_handle_fun_msg)
    client_handle.subscribe("fun_msg_res", handle_fun_msg_res)
    client_handle.subscribe("call_establish_indication", handle_call_establish_ind)
    client_handle.subscribe("dev_released_from_call", handle_call_dev_released_ind)
    client_handle.subscribe("call_release_indication", handle_call_release_ind)
    print("0")

    client_handle.start()
    log("HAN client started")

    history = InMemoryHistory()

    # Make the list of commands for the completer
    command_list = commands.keys()
    command_completer = WordCompleter(command_list, ignore_case=True)

    no_interaction = False 
    # get a global list of the currently available devices..
    # remember to update frequently on register dereg etc....
    print("Gather current device information")
    list_devices(client_handle, '')

    CMD_LINE = False 
    if CMD_LINE:
        argv = sys.argv
        cmd = argv[1]
        commands.get(cmd, command_not_found)(client_handle, argv[1:])
        print('try exiting client in 5s -------')
        time.sleep(5)
        commands.get("q", command_not_found)(client_handle, "")

    #start the Keyboard thread
    kthread = MainThread()

    while True:
        if not no_interaction:
            user_command = prompt("> ",
                                history=history,
                                #patch_stdout=True,
                                completer=command_completer,
                                complete_while_typing=False)
            argv = shlex.split(user_command)

            if not argv:
                continue

            cmd = argv[0]

            commands.get(cmd, command_not_found)(client_handle, argv)
        else:
            # do nothing
            time.sleep(1)

if __name__ == "__main__":
    main()
