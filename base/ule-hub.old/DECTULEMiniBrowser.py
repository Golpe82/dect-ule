#!/usr/bin/env python3
import logging
import os

from profile_hanfun import *

from DECTMessagingConfig import *
 
global MINI_URL 
global ULE_URL 

ULE_URL = f'http://{XML_SERVER_IP}:8881'
MINI_URL = f'{ULE_URL}/miniULE'
HTML_URL = f'{ULE_URL}/htmlULE'

XML_FILE_PATH = './'

import urllib.parse

def return_minibrowser_cmd_payload_change(dev_id: int, i_id: int, u_id: int, c: HFC2SCommand, label: str, idx: int) -> str:
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneInput>\n'
    c_txt += f'<Title>{c.cmd_name}</Title>\n'

    c_txt +=  f'<URL>'
    c_txt +=  f'{ULE_URL}/snom_ule_cmd/snom_set_cmd_attribute_value'
    cc_txt  = f' {dev_id} {u_id}'
    cc_txt += f' {i_id} 1 {c.cmd_id}'
    # encode all slashes into url encoded backslashes. 
    label_enc = label.replace("/", "%2C")
    cc_txt += f' "{label_enc}"'
    # value 
    cc_txt += f' X'
    cc_txt  = urllib.parse.quote(cc_txt)
    c_txt += cc_txt
    c_txt += '</URL>\n'
    c_txt +=  '<InputItem>\n'
    c_txt +=  f'<DisplayName>{label}</DisplayName>\n'
    c_txt +=  '<InputToken>X</InputToken>\n'
    # payload idx is always 0
    # get old value as default 
    # in case value is part of a bitfield, we need another URL to evaluate the real attribute value 
    last_val = c.payload_descriptions[0].get_attribute_value_by_description(label)
    c_txt +=  f'<DefaultValue>{last_val}</DefaultValue>\n'
    c_txt +=  '</InputItem>\n'
    c_txt += '</SnomIPPhoneInput>\n'

    return c_txt

def create_minibrowser_cmd_payload_change(dev: HFDevice, i: HFInterface, u: HFUnit, c: HFC2SCommand, label: str, idx: int, write_xml=True) -> str:
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneInput>\n'
    c_txt += f'<Title>{c.cmd_name}</Title>\n'

    c_txt +=  f'<URL>{ULE_URL}/snom_ule_cmd/snom_set_cmd_attribute_value {dev.device_id} {u.unit_id}'
    c_txt += f' {i.intrf_id} 1 {c.cmd_id}'
    c_txt += f' "{label}"'
    # value 
    c_txt += f' X'
    c_txt += '</URL>\n'
    c_txt +=  '<InputItem>\n'
    c_txt +=  f'<DisplayName>{label}</DisplayName>\n'
    c_txt +=  '<InputToken>X</InputToken>\n'
    # payload idx is always 0
    # get old value as default 
    # in case value is part of a bitfield, we need another URL to evaluate the real attribute value 
    last_val = c.payload_descriptions[0].get_attribute_value_by_description(label)
    c_txt +=  f'<DefaultValue>{last_val}</DefaultValue>\n'
    c_txt +=  '</InputItem>\n'
    c_txt += '</SnomIPPhoneInput>\n'

    if c_txt != '':    
        filename = f'{dev.device_id}_{u.unit_id}_{i.intrf_id}_{c.cmd_id}_{idx}.xml'
        if os.path.exists(filename):
            os.remove(filename)

        with open(filename, 'w') as file:
            file.write(c_txt)

    return c_txt

def return_minibrowser_cmd_payload(dev_id: int, i_id: int, u_id: int, c: HFC2SCommand) -> str:
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneMenu>\n'
    c_txt += f'<Title>{c.cmd_name}</Title>\n'

    for pd in c.payload_descriptions:
      
        for idx, ad in enumerate(pd.attribute_descriptions):
            if len(ad) == 3:
                label, start, end = ad
            if len(ad) == 4: # with sign 
                label, start, end, sign = ad
            c_txt +=  '<MenuItem>\n'
            label = label.strip()
            c_txt +=  f'<Name>{label}</Name>\n'
            c_txt +=  f'<URL>{MINI_URL}/{dev_id}/{u_id}/{i_id}/{c.cmd_id}/{idx}</URL>\n'
            c_txt +=  '</MenuItem>\n'

    if c_txt != '':
        # back to cmds
        c_txt +=  '<MenuItem>\n'
        c_txt +=  '<Name>back</Name>\n'
        c_txt +=  f'<URL>{MINI_URL}/{dev_id}/{u_id}/{i_id}</URL>\n'
        c_txt +=  '</MenuItem>\n'
        c_txt += '</SnomIPPhoneMenu>\n'

    return c_txt

def create_minibrowser_cmd_payload(dev: HFDevice, i: HFInterface, u: HFUnit, c: HFC2SCommand) -> str:
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneMenu>\n'
    c_txt += f'<Title>{c.cmd_name}</Title>\n'

    for pd in c.payload_descriptions:
      
        for idx, ad in enumerate(pd.attribute_descriptions):
            if len(ad) == 3:
                label, start, end = ad
            if len(ad) == 4: # with sign 
                label, start, end, sign = ad
            c_txt +=  '<MenuItem>\n'
            label = label.strip()
            c_txt +=  f'<Name>{label}</Name>\n'
            c_txt +=  f'<URL>{MINI_URL}/{dev.device_id}_{u.unit_id}_{i.intrf_id}_{c.cmd_id}_{idx}.xml</URL>\n'
            c_txt +=  '</MenuItem>\n'

            create_minibrowser_cmd_payload_change(dev, i, u, c, label, idx)
            # set value
            #pd.set_attribute_value_by_description(label, 0x03)
    
    if c_txt != '':
        # back to cmds
        c_txt +=  '<MenuItem>\n'
        c_txt +=  '<Name>back</Name>\n'
        c_txt +=  f'<URL>{MINI_URL}/{dev.device_id}_{u.unit_id}_{i.intrf_id}.xml</URL>\n'
        c_txt +=  '</MenuItem>\n'
        c_txt += '</SnomIPPhoneMenu>\n'

        filename = f'{dev.device_id}_{u.unit_id}_{i.intrf_id}_{c.cmd_id}.xml'
        if os.path.exists(filename):
            os.remove(filename)

        with open(filename, 'w') as file:
            file.write(c_txt)

    return c_txt

def return_cmd_payload(dev_id: int, i_name: str, i_id: int, u_id: int, c: HFC2SCommand) -> tuple:
    # create jinja data 
    data_header = {}
    data_header['DeviceID'] = dev_id
    data_header['InterfaceName'] = i_name
    data_header['InterfaceId'] = i_id
    data_header['UnitID'] = u_id
    data_header['CName'] = c.cmd_name
    data_header['BackUrl'] = f'{HTML_URL}/{dev_id}/{u_id}/{i_id}'
    
    data_list = []
    sign = False 

    for pd in c.payload_descriptions:
      
        for idx, ad in enumerate(pd.attribute_descriptions):
            if len(ad) == 3:
                label, start, end = ad
            if len(ad) == 4: # with sign 
                label, start, end, sign = ad
            label = label.strip()
            # convert data into int
            last_val = c.payload_descriptions[0].get_attribute_value_by_description(label)
            data_tuple = (idx, label, start, end, sign, last_val)
            data_list.append(data_tuple)

    return (data_header, data_list)

def return_minibrowser_cmds(dev_id: int, u_id: int, i: HFInterface) -> str:
    c_txt = ''
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneMenu>\n'
    c_txt += f'<Title>{i.intrf_name}</Title>\n'

    for c in i.c2s_cmds:
        c_txt +=  '<MenuItem>\n'
        c_txt +=  f'<Name>{c.cmd_name}</Name>\n'
         
        if c.payload_descriptions != []:
            c_txt +=  f'<URL>{MINI_URL}/{dev_id}/{u_id}/{i.intrf_id}/{c.cmd_id}</URL>\n'
            c_txt +=  '</MenuItem>\n'
        else:
            c_txt += f'<URL>{ULE_URL}/snom_ule_cmd/snom_send_cmd {dev_id} {u_id}'
            c_txt += f' {i.intrf_id} 1 {c.cmd_id}'
            c_txt += ' 22'
            c_txt += '</URL>\n'
            c_txt +=  '</MenuItem>\n'
            
    if c_txt != '':
        # back to Interfaces
        c_txt +=  '<MenuItem>\n'
        c_txt +=  '<Name>back</Name>\n'
        c_txt +=  f'<URL>{MINI_URL}/{dev_id}</URL>\n'
        c_txt +=  '</MenuItem>\n'
        c_txt += '</SnomIPPhoneMenu>\n'
    
    return c_txt

def return_cmds(dev_id: int, u_id: int, i: HFInterface, u_profile: str) -> tuple:
    # create jinja data 
    data_header = {}
    data_header['DeviceID'] = dev_id
    data_header['IName'] = i.intrf_name
    data_header['UnitID'] = u_id
    data_header['UnitProfile'] = u_profile
    data_header['BackUrl'] = f'{HTML_URL}/{dev_id}'
    
    data_list = []

    for c in i.c2s_cmds:
        if c.payload_descriptions != []:
            url =  f'{HTML_URL}/{dev_id}/{u_id}/{i.intrf_id}/{c.cmd_id}'
        else:
             # fire command
            url = f'{ULE_URL}/snom_ule_cmd_nr/snom_send_cmd {dev_id} {u_id}'
            url += f' {i.intrf_id} 1 {c.cmd_id}'
            url += ' 22'
    
        data_tuple = (c.cmd_name, c.cmd_id, url)
        data_list.append(data_tuple)

    return (data_header, data_list)

def create_minibrowser_cmd(dev: HFDevice, i: HFInterface, u: HFUnit, filename: str) -> str:
    c_txt = ''
    c_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    c_txt += '<SnomIPPhoneMenu>\n'
    c_txt += f'<Title>{i.intrf_name}</Title>\n'

    for c in i.c2s_cmds:
        c_txt +=  '<MenuItem>\n'
        c_txt +=  f'<Name>{c.cmd_name}</Name>\n'
         
        # paramter 
        if c.payload_descriptions != []:
            # print('CMD with payload')
            c_txt +=  f'<URL>{MINI_URL}/{dev.device_id}_{u.unit_id}_{i.intrf_id}_{c.cmd_id}.xml</URL>\n'
            c_txt +=  '</MenuItem>\n'
            create_minibrowser_cmd_payload(dev, i, u, c)
        else:
            c_txt += f'<URL>{ULE_URL}/snom_ule_cmd/snom_send_cmd {dev.device_id} {u.unit_id}'
            c_txt += f' {i.intrf_id} 1 {c.cmd_id}'
            c_txt += ' 99'
            c_txt += '</URL>\n'
            c_txt +=  '</MenuItem>\n'
            
    if c_txt != '':
        # back to Interfaces
        c_txt +=  '<MenuItem>\n'
        c_txt +=  '<Name>back</Name>\n'
        c_txt +=  f'<URL>{MINI_URL}/{dev.device_id}.xml</URL>\n'
        c_txt +=  '</MenuItem>\n'
        c_txt += '</SnomIPPhoneMenu>\n'
    
        if os.path.exists(filename):
            os.remove(filename)

        with open(filename, 'w') as file:
            file.write(c_txt)

    return c_txt

def return_minibrowser_device(dev: HFDevice) -> str:
    i_txt = ''
    u_txt = ''
    u_i_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    u_i_txt += '<SnomIPPhoneMenu>\n'

    for u in dev.units:
        if u.unit_id != 0:
            u_txt = ''
            
            #if len(u.interfaces) > 0:
            #    u_txt += f'<Title>{u.profile.profile_name}:{u.unit_id}</Title>\n'
            
            #if dev.device_id == 12:
            #    print('nie')
            for i in u.interfaces:
                i_txt = ''
                # new file 

                i_txt +=  '<MenuItem>\n'
                i_txt +=  f'<Name>{i.intrf_name}-{u.profile.profile_name}</Name>\n'
                i_txt +=  f'<URL>{MINI_URL}/{dev.device_id}/{u.unit_id}/{i.intrf_id}</URL>\n'
                i_txt +=  '</MenuItem>\n'

                #   check if the device has any interfaces to use
                content = return_minibrowser_cmds(dev.device_id, u.unit_id, i)
                
                if 'snom_ule_cmd' not in content:
                    # the menu is not needed, no executable cmd
                    i_txt = ''
                u_txt += i_txt
            u_i_txt += u_txt

    if u_i_txt != '':
        # back to Devices Menu
        u_i_txt +=  '<MenuItem>\n'
        u_i_txt +=  '<Name>back</Name>\n'
        u_i_txt +=  f'<URL>{MINI_URL}</URL>\n'
        u_i_txt +=  '</MenuItem>\n'

        u_i_txt += '</SnomIPPhoneMenu>\n'

    return u_i_txt

def create_minibrowser_device(dev: HFDevice) -> str:
    i_txt = ''
    u_txt = ''
    u_i_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    u_i_txt += '<SnomIPPhoneMenu>\n'

    for u in dev.units:
        if u.unit_id != 0:
            u_txt = ''
            
            if len(u.interfaces) > 0:
                u_txt += f'<Title>{u.profile.profile_name}:{u.unit_id}</Title>\n'
            
            #if dev.device_id == 12:
            #    print('nie')
            for i in u.interfaces:
                i_txt = ''
                # new file 

                i_txt +=  '<MenuItem>\n'
                i_txt +=  f'<Name>{i.intrf_name}</Name>\n'
                i_txt +=  f'<URL>{MINI_URL}/{dev.device_id}_{u.unit_id}_{i.intrf_id}.xml</URL>\n'
                i_txt +=  '</MenuItem>\n'

                # create interface cmd memu file 
                filename = f'{XML_FILE_PATH}/{dev.device_id}_{u.unit_id}_{i.intrf_id}.xml'
                content = create_minibrowser_cmd(dev, i, u, filename)
                
                if 'snom_ule_cmd' not in content:
                    # the menu is not needed, no executable cmd
                    i_txt = ''
                u_txt += i_txt
        u_i_txt += u_txt

    if u_i_txt != '':
        # back to Devices Menu
        u_i_txt +=  '<MenuItem>\n'
        u_i_txt +=  '<Name>back</Name>\n'
        u_i_txt +=  f'<URL>{MINI_URL}/ULE_devices.xml</URL>\n'
        u_i_txt +=  '</MenuItem>\n'

        u_i_txt += '</SnomIPPhoneMenu>\n'

        filename = f'{XML_FILE_PATH}/{dev.device_id}.xml'
        if os.path.exists(filename):
            os.remove(filename)

        with open(filename, 'w') as file:
            file.write(u_i_txt)

    return u_i_txt

def return_device(dev: HFDevice) -> tuple:
    # create jinja data 
    data_header = {}
    data_header['DeviceID'] = dev.device_id
    data_header['BackUrl'] = f'{HTML_URL}'

    
    if len(dev.device_name) < 3:
        # there is no real name, use the unit 1 profile name instead. 
        device_name = dev.get_unit_by_id(1).profile.profile_name
    else:
        device_name = dev.device_name
    data_header['DeviceName'] = device_name
    
    data_list = []

    for u in dev.units:
        if u.unit_id != 0:
            u_txt = ''
            
            for i in u.interfaces:
                i_txt = ''

                #   check if the device has any interfaces to use
                content = return_minibrowser_cmds(dev.device_id, u.unit_id, i)
                
                if 'snom_ule_cmd' not in content:
                    # the menu is not needed, no executable cmd
                    continue
                else: 
                    url =  f'{HTML_URL}/{dev.device_id}/{u.unit_id}/{i.intrf_id}'
                    data_tuple = (i.intrf_id, i.intrf_name, u.unit_id, u.profile.profile_name, url)
                    data_list.append(data_tuple)

    return (data_header, data_list)

def return_minibrowser_ULE(devices: HFDevices()) -> str:
    device_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    device_txt += '<SnomIPPhoneMenu>\n'
    device_txt += f'<Title>ULE-Devices</Title>\n'

    for dev in devices.get_devices():
        device_txt +=  '<MenuItem>\n'
        device_txt +=  f'<Name>{dev.get_unit_by_id(1).profile.profile_name}:{dev.device_id}</Name>\n'
        device_txt +=  f'<URL>{MINI_URL}/{dev.device_id}</URL>\n'
        device_txt +=  '</MenuItem>\n'

    device_txt += '</SnomIPPhoneMenu>'

    return device_txt

def create_minibrowser_ULE(devices: HFDevices()) -> str:
    device_txt = '<?xml version="1.0" encoding="UTF-8"?>\n'
    device_txt += '<SnomIPPhoneMenu>\n'
    device_txt += f'<Title>ULE-Devices</Title>\n'

    for dev in devices.get_devices():
        device_txt +=  '<MenuItem>\n'
        device_txt +=  f'<Name>{dev.get_unit_by_id(1).profile.profile_name}:{dev.device_id}</Name>\n'
        device_txt +=  f'<URL>{MINI_URL}/{dev.device_id}.xml</URL>\n'
        device_txt +=  '</MenuItem>\n'
        create_minibrowser_device(dev)

    device_txt += '</SnomIPPhoneMenu>'

    filename = f'{XML_FILE_PATH}/ULE_devices.xml'
    if os.path.exists(filename):
        os.remove(filename)

    with open(filename, 'w') as file:
        file.write(device_txt)

    return device_txt

def return_ULE(devices: HFDevices()) -> tuple:
    # create jinja data 
    data_header = {}
    data_header['MenuName'] = 'ULE-Devices'
    
    data_list = []
    
    for dev in devices.get_devices():
        url =  f'{HTML_URL}/{dev.device_id}'
        if len(dev.device_name) < 3:
            # there is no real name, use the unit 1 profile name instead. 
            device_name = dev.get_unit_by_id(1).profile.profile_name
        else:
            device_name = dev.device_name

        # check if there are useable interfaces. if not URL should be same page or None
        check = return_device(dev)
        if len(check[1]) == 0:
            url =  f'NOINTERFACE'

        data_tuple = (dev.get_unit_by_id(1).profile.profile_name, dev.device_id, device_name, url)
        data_list.append(data_tuple)

    return (data_header, data_list)

     
    for dev in devices.get_devices():
        device_txt +=  '<MenuItem>\n'
        device_txt +=  f'<Name>{dev.get_unit_by_id(1).profile.profile_name}:{dev.device_id}</Name>\n'
        device_txt +=  f'<URL>{MINI_URL}/{dev.device_id}</URL>\n'
        device_txt +=  '</MenuItem>\n'

    device_txt += '</SnomIPPhoneMenu>'


if __name__ == "__main__":
 #create all known interfaces
    interfaces = HFInterfaces()
    interfaces.create_known_interfaces()
    profiles = HFProfiles()

    print('#################################')
    print('#################################')
    print('#################################')

    # DECT ULE supports many many different devices
    devices = HFDevices()    
    
    print(profiles.get_profile_by_id(2810))

    profile = HFProfile(profile_id=281, profile_name='Becker Rolladenmotor')
    unit = HFUnit(unit_id=1, unit_name='one', profile=profile,
                  interfaces=[interfaces.get_interface_by_id(256),
                              interfaces.get_interface_by_id(513),
                              interfaces.get_interface_by_id(516),
                              interfaces.get_interface_by_id(517),
                             ])

    new_device = HFDevice(device_id=12, device_ipui = "1234567890", device_name='Becker Rolladenmotor', units=[unit])
    # update to known vendor 
    dev_update_to_becker_rolladen(new_device)
    dev_update_profile_changes(new_device)
   
    devices.add_device(new_device)

    profiles = HFProfiles()
    profile=profiles.get_profile_by_id(0x0107)
    unit = HFUnit(unit_id=1, unit_name='Power Plug', profile=profile,
                  interfaces=[interfaces.get_interface_by_id(0x0300),
                              interfaces.get_interface_by_id(0x0200),
                             ])

    new_device = HFDevice(device_id=6, device_ipui = "1234567891", device_name='Fritz Plug', units=[unit])
    devices.add_device(new_device)

    # check to read value from interface attributes masks
    interface_t = interfaces.get_interface_by_id(0x0300)
    energy_unit = interface_t.get_attribute_by_id(1).get_attribute_value_by_description('Precision Code')
    val = interface_t.get_attribute_by_id(1).get_attribute_value_by_description('Ener')
    print(f'Value read with attribute description Ener.. = {val},{hex(val)}')
    print(f'Value with precision {hex(energy_unit)} giga {float(val)/float(1000000000)}')
    # check remove 
    # print('remove Interface 256')
    # unit.interfaces.remove(interfaces.get_interface_by_id(256))
    print(f'Number of Interfaces = {len(unit.interfaces)}')  


    profiles = HFProfiles()
    profile=profiles.get_profile_by_id(0x0115)
    unit = HFUnit(unit_id=1, unit_name='Color', profile=profile,
                  interfaces=[interfaces.get_interface_by_id(0x0201),
                              interfaces.get_interface_by_id(0x0202),
                              interfaces.get_interface_by_id(0x0200),
                             ])

    new_device = HFDevice(device_id=7, device_ipui = "1234567892", device_name='Fritz Bulb', units=[unit])
    devices.add_device(new_device)

    profiles = HFProfiles()
    profile=profiles.get_profile_by_id(0x0113) 
    unit = HFUnit(unit_id=1, unit_name='1', profile=profile,
                  interfaces=[interfaces.get_interface_by_id(0x0305),
                             ])

    new_device = HFDevice(device_id=8, device_ipui = "1234567892", device_name='Visual Effekt', units=[unit])
    devices.add_device(new_device)

    # signed values in cmd payload description
    interface_t = interfaces.get_interface_by_id(0x0202)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', -200)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'signed={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Stepa', 0xf0ff)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'unsigned={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', 0xf0ff)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'unsigned={val}')
    print(interface_t.get_c2s_cmd_by_id(10))

    _, device_id, unit_id, interface_id, attribute_id = ['', '12', '1', '517', '1']
    _, device_id, unit_id, interface_id, attribute_id = ['', '7', '1', '513', '1']
    dev = devices.get_device_by_id(int(device_id))
    #unit = dev.get_unit_by_id(int(unit_id))
    unit = dev.get_unit_by_id(int(unit_id))
    interface = unit.get_interface_by_id(int(interface_id))
    attributes = interface.server_attributes
    for attrb in attributes:
        print(attrb)

    print('\nCMDs')
    print('----')
    print(interface.get_c2s_cmds())
    print('\nServer to Client Commands')
    print('-------------------------')
    print(interface.get_s2c_cmds())

    # data for jinja 
    data_tuple = return_cmds(7, 1, interface, 'some_profile')
    print(data_tuple)
    data_tuple = return_cmd_payload(7, interface_id, unit_id, interface.get_c2s_cmds()[2])
    print(data_tuple)
    data_tuple = return_device(dev)
    print(data_tuple)
    data_tuple = return_ULE(devices)
    print(data_tuple)

    # minibrowser files -- UPDATE needed
    #print(create_minibrowser_ULE(devices))
    