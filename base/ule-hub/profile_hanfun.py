#!/usr/bin/env python3
import logging
from dataclasses import dataclass, field
from typing import List
import copy


@dataclass
class HFServerAttribute:
    attribute_id : int
    attribute_name : str
    attribute_type : int # length in bytes
    attribute_values: List[bytes] = field(default_factory=lambda: [])
    attribute_descriptions: List[dict] = field(default_factory=lambda: [])

    def f_comma(self, my_str, group=8, char='|') -> str:
        my_str = str(my_str)
        return char.join(my_str[i:i+group] for i in range(0, len(my_str), group))

    def _pp_to_str(self) -> str:
        printout:str = ''
        int_val = int.from_bytes(bytes(self.attribute_values), "big")
        bit_length = self.attribute_type*8
        bitstring_t = bin(int_val)
        bitstring = bitstring_t[2:].rjust(bit_length,'0')

        printout = ''
        printout += "\n{}: {}\n".format(self.attribute_id, self.attribute_name)
        printout += "{0}\n".format('='.rjust(len(printout)-2, '='))
        length_diff = len(self.attribute_values)*8 - bit_length
        if length_diff != 0:
            printout += f'!! received {length_diff} extra bits than expected !!\n'
        #                                                          bits       hex      padding + 0x
        header = "0b{0} :{1} {2:#0{3}x}\n".format(self.f_comma(bitstring.ljust(bit_length)), self.attribute_name, int_val, self.attribute_type*2+2)
        header += "{0}\n".format('-'.rjust(len(header)-1, '-'))
        printout += header
        # -----------------------------
        option_header = ''
        for dd in self.attribute_descriptions:
            if type(dd) != list:
                # build a one element List - not a options list
                d_list = [dd]
            else:
                # use the options list
                d_list = dd
                option_header = 'opt:'

            for d in d_list:
                sign = False
                if len(d) == 3:
                    label, start, end = d
                if len(d) == 4: # with sign 
                    label, start, end, sign = d
                    
                # bit position 0 starts from left
                end_t = bit_length - start
                start = bit_length - end
                end = end_t
                number = int(f'0b{bitstring[start:end]}', 2)
                if sign:
                    signed_bitstring = bitstring[start:end]
                    if signed_bitstring[0] == '1':
                        number ^= 2**(end-start)-1
                        number += 1
                        number= -number    
                printout_t = "{}0b{} :{} = {} ({})\n".format(option_header,
                    self.f_comma(bitstring[start:end].rjust(end,'.').ljust(bit_length, '.')),
                    label, 
                    number,
                    hex(int(f'0b{bitstring[start:end]}', 2))
                    )
                printout += printout_t
        return printout

    def __repr__(self) -> str:
        return self._pp_to_str()

    def get_description_tuple_by_name(self, attr_desc) -> tuple:
        for dd in self.attribute_descriptions:
            if type(dd) != list:
                # build a one element List
                d_list = [dd]
            else:
                # use the options list
                d_list = dd
            for d in d_list:
                if len(d) == 3:
                    label, start, end = d
                elif len(d) == 4: # with sign 
                    label, start, end, sign = d
                else:
                    return None
                if attr_desc in label:
                    return d
        return None

    def set_attribute_value_by_description(self, attr_desc: str, new_slice_value: int) -> bool:        
        sign = False
        # get all value bytes
        try:
            full_value = int.from_bytes(bytes(self.attribute_values), "big")
            desc = self.get_description_tuple_by_name(attr_desc) 
            if desc == None:
                print(f'attribute_description: {attr_desc} does not exist. value unchanged!')
                return False
        except:
            print('set_attribute_value_by_description: something went wrong!')
            return False
        if len(desc) == 3:
            label, start, end = desc
        elif len(desc) == 4: # with sign 
            label, start, end, sign = desc
        else:
            return False   
        
        slice_length = end - start
        slice_mask = 2**slice_length-1
        slice_mask = slice_mask << start
        full_value &= ~slice_mask
        if sign == True and new_slice_value < 0:
            # cut slice vlaue to valid range
            new_slice_value = abs(new_slice_value)
            new_slice_value &= 2**(slice_length) - 1
            new_slice_value *= -1
        else:
            new_slice_value &= 2**(slice_length) - 1
             
        full_value += new_slice_value << start 
        bytes_val = full_value.to_bytes(len(self.attribute_values), 'big')
        self.attribute_values = list(bytes_val)
        return True 
    
    def get_attribute_value_by_description(self, attr_desc: str) -> int:
        ad = self.get_description_tuple_by_name(attr_desc)
        if ad != None:
            sign = False
            if len(ad) == 3:
                label, start, end = ad
            if len(ad) == 4: # with sign 
                label, start, end, sign = ad
            if attr_desc in label:
                int_val = int.from_bytes(bytes(self.attribute_values), "big")
                bit_length = self.attribute_type*8
                end_t = bit_length - start
                start = bit_length - end
                end = end_t
                bitstring_t = bin(int_val)
                bitstring = bitstring_t[2:].rjust(bit_length,'0')
                #print(f"0b{bitstring[start:end]} == {int(f'0b{bitstring[start:end]}', 2)}, {hex(int(f'0b{bitstring[start:end]}', 2))}")
                number = int(f'0b{bitstring[start:end]}', 2)
                if sign:
                    signed_bitstring = bitstring[start:end]
                    if signed_bitstring[0] == '1':
                        number ^= 2**(end-start)-1
                        number += 1
                        number= -number            
                return number
        return 'NaN'

    def add_attribute_values(self, l):
        # l must be list of bytes
        # dataclass does not know .add for lists
        if self.attribute_type >= len(self.attribute_values):
            self.attribute_values += l
        else:
            print(f'cannot add to list, len of attribute_values+l={len(self.attribute_values) + len(l)} > {self.attribute_type}') 

''' 
Server-Client Command 
The server receives a CMD from the device
'''
@dataclass
class HFS2CCommand:
    cmd_id : int
    cmd_name : str
    cmd_action : str # function to get called with e.g. eval()
    payload_descriptions : List[HFServerAttribute] = field(default_factory=lambda: [])

''' Client-Server Command '''
@dataclass
class HFC2SCommand:
    cmd_id : int
    cmd_name : str
    payload_descriptions : List[HFServerAttribute] = field(default_factory=lambda: [])

    def get_attribute_value_by_description(self, attr_desc: str) -> int:
        for ad in self.payload_descriptions:     
            sign = False
            if len(ad) == 3:
                label, start, end = ad
            if len(ad) == 4: # with sign 
                label, start, end, sign = ad
            if attr_desc in label:
                int_val = int.from_bytes(bytes(self.attribute_values), "big")
                bit_length = self.attribute_type*8
                end_t = bit_length - start
                start = bit_length - end
                end = end_t
                bitstring_t = bin(int_val)
                bitstring = bitstring_t[2:].rjust(bit_length,'0')
                #print(f"0b{bitstring[start:end]} == {int(f'0b{bitstring[start:end]}', 2)}, {hex(int(f'0b{bitstring[start:end]}', 2))}")
                number = int(f'0b{bitstring[start:end]}', 2)
                if sign:
                    signed_bitstring = bitstring[start:end]
                    if signed_bitstring[0] == '1':
                        number ^= 2**(end-start)-1
                        number += 1
                        number= -number            
                return number
        return 0

@dataclass
class HFInterface:
    intrf_id : int
    intrf_name : str
    server_attributes : List[HFServerAttribute] = field(default_factory=lambda: [])
    c2s_cmds : List[HFC2SCommand] = field(default_factory=lambda: [])
    s2c_cmds : List[HFS2CCommand] = field(default_factory=lambda: [])

    def pp_c2s_cmds(self):
        for cmd in self.c2s_cmds:
            print(f'{cmd.cmd_id}: {cmd.cmd_name}')
            if len(cmd.payload_descriptions) > 0:
                print(cmd.payload_descriptions)

    def pp_s2c_cmds(self):
        for cmd in self.s2c_cmds:
            print(f'{cmd.cmd_id}: {cmd.cmd_name}')
            if len(cmd.payload_descriptions) > 0:
                print(cmd.payload_descriptions)

    def get_c2s_cmds(self) -> List[HFC2SCommand]:
        if len(self.c2s_cmds) > 0:
            return self.c2s_cmds
        else:
            print(f'no HFC2SCommand(s) Client-->Server specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 
    
    def get_c2s_cmd_by_id(self, id) -> List[HFC2SCommand]:
        if len(self.c2s_cmds) > 0:
            match = next((c2s_cmd for c2s_cmd in self.c2s_cmds if c2s_cmd and c2s_cmd.cmd_id == id), None)
            if match:
                return match
            else:
                print(f'cannot get HFC2SCommand with id={id}')
        else:
            print(f'no HFC2SCommand(s) specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 

    def get_s2c_cmds(self) -> List[HFS2CCommand]:
        if len(self.s2c_cmds) > 0:
            return self.s2c_cmds
        else:
            print(f'no HFS2CCommand(s) Server-->Client specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 
   
    def get_s2c_cmd_by_ref(self, id_or_name) -> List[HFS2CCommand]:
        if len(self.s2c_cmds) > 0:
            id = id_or_name
            if type(id_or_name) is str:
                match = next((s2c_cmd for s2c_cmd in self.s2c_cmds if s2c_cmd and s2c_cmd.cmd_name == id_or_name), None)
                return match      
            else:   
                return self.get_s2c_cmd_by_id(id)
        else:
            print(f'no HFS2CCommand(s) specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 

    def get_s2c_cmd_by_id(self, id) -> List[HFS2CCommand]:
        if len(self.s2c_cmds) > 0:
            match = next((s2c_cmd for s2c_cmd in self.s2c_cmds if s2c_cmd and s2c_cmd.cmd_id == id), None)
            if match:
                return match
            else:
                print(f'cannot get HFS2CCommand with id={id}')
        else:
            print(f'no HFS2CCommand(s) specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 

    def set_attribute_values_by_ref(self, id_or_name, values):
        if len(self.server_attributes) > 0:
            id = id_or_name
            if type(id_or_name) is str:
                match = next((attribute for attribute in self.server_attributes if attribute and attribute.attribute_name == id_or_name), None)
                id = match.attribute_id          
            self.set_attribute_values_by_id(id, values)
        else:
            print(f'cannot set Attribute with id={id_or_name}')

    def set_attribute_values_by_id(self, id, values):
        if len(self.server_attributes) > 0:
            match = next((attribute for attribute in self.server_attributes if attribute and attribute.attribute_id == id), None)
            if match:
                if len(match.attribute_values) != len(values):
                    print(f'set_attribute_values_by_id: data={values} provided too long for specified attribute={match.attribute_name},data={match.attribute_values}')
                else:
                    match.attribute_values = values
            else:
                print(f'cannot set Attribute with id={id}')
        else:
            print(f'no HFC2SCommand(s) specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 

    def get_attribute_values_by_ref(self, id_or_name):
        if len(self.server_attributes) > 0:
            id = id_or_name
            if type(id_or_name) is str:
                match = next((attribute for attribute in self.server_attributes if attribute and attribute.attribute_name == id_or_name), None)
                id = match.attribute_id          
            return self.get_attribute_values_by_id(id)
        else:
            print(f'cannot set Attribute with id={id_or_name}')

    def get_attribute_values_by_id(self, id) -> List[bytes]:
        if len(self.server_attributes) > 0:
            self.server_attributes
            match = next((attribute for attribute in self.server_attributes if attribute and attribute.attribute_id == id), None)
            if match:
                return match.attribute_values
            else:
                print(f'cannot get Attribute with id={id}')
        else:
            print(f'no HFC2SCommand(s) specified for Interface={self.intrf_id}:0x{self.intrf_id:02x}') 

    def get_attribute_by_id(self, attribute_id) -> HFServerAttribute:
        ## hex value as string should also be supported.
        if len(self.server_attributes) > 0:
            match = next((attribute for attribute in self.server_attributes if attribute and attribute.attribute_id == attribute_id), None)
            if match:
                return match
            else:
                print(f'cannot find Server Attribute={attribute_id}')
        else:
            print(f'cannot find attribute={attribute_id}, no HFServerAttribute specified.') 
        return None

    def print_server_attributes(self):
            # fire ULE attribute request and wait for answer somewhere
            for attribute in self.server_attributes:
                print(f'id={attribute.attribute_id}, name={attribute.attribute_name}, value={attribute.attribute_values}')

@dataclass
class HFInterfaces:
    intrf_list_name : str = 'Currently available Interfaces'
    interfaces : List[HFInterface] = field(default_factory=lambda: [])

    def add_interface(self, interface: HFInterface):
        # l must be list of bytes
        # dataclass does not know .add for lists
        try:
            self.interfaces.append(copy.deepcopy(interface))
        except:
            logging.exception(f'cannot add Interface {interface} to list.') 

    def get_interfaces(self) -> List[HFInterface]:
        if len(self.interfaces) > 0:
            return self.interfaces
        else:
            print(f'no HFInterfaces specified.') 

    def get_interface_by_name(self, intrf_name) -> HFInterface:
        if len(self.interfaces) > 0:
            match = next((interface for interface in self.interfaces if interface and interface.intrf_name == intrf_name), None)
            if match:
                return match
            else:
                print(f'cannot find Interface name={intrf_name}')
        else:
            print(f'cannot find an interface, no HFInterfaces specified.') 
        return None

    def get_interface_by_id(self, intrf_id) -> HFInterface:
        ## hex value as string should also be supported.
        if len(self.interfaces) > 0:
            match = next((interface for interface in self.interfaces if interface and interface.intrf_id == intrf_id), None)
            if match:
                return match
            else:
                print(f'cannot find Interface={intrf_id}')
        else:
            print(f'cannot find an interface, no HFInterfaces specified.') 
        return None

    def delete_interface_by_id(self, intrf_id) -> bool:
        ## hex value as string should also be supported.
        if len(self.interfaces) > 0:
            match = next((interface for interface in self.interfaces if interface and interface.intrf_id == intrf_id), None)
            if match:
                try:
                    self.interfaces.remove(match)
                except:
                    logging.exception('Could not remove element {} unexpectedly.', match)
                return True
            else:
                print(f'cannot find Interface={intrf_id} to be removed.')
        else:
            print(f'cannot find an interface to remove, no HFInterfaces specified.') 
        return None

    def create_known_interfaces(self):
        # service interfaces 
        interface = HFInterface(intrf_id=0x0004, intrf_name='Identify')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0101, intrf_name='Tamper Alert')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0102, intrf_name='Time')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0110, intrf_name='Power')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0111, intrf_name='RSSI')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0115, intrf_name='Keep Alive')
        self.add_interface(interface)
        interface = HFInterface(intrf_id=0x0400, intrf_name='SUOTA')
        self.add_interface(interface)
        
        # Simple Button
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Short Press Maximum Duration', 0, attribute_type*8),
                                  ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Short Press Maximum Duration', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)
        
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Extra Long Press Minimum Duration', 0, attribute_type*8),
                                  ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Extra Long Press Minimum Duration', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)
        
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Double Press Gap Duration', 0, attribute_type*8),
                                  ]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Double Press Gap Duration', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)
                                                                
        scmd1 = HFS2CCommand(cmd_id=1, cmd_name='Short Press', cmd_action='snom_handle_x0304_state(device_id, unit_id, interface_id, cmd_id, hl)')
        scmd2 = HFS2CCommand(cmd_id=2, cmd_name='Long Press', cmd_action='snom_handle_x0304_state(device_id, unit_id, interface_id, cmd_id, hl)')
        scmd3 = HFS2CCommand(cmd_id=3, cmd_name='Extra Long Press', cmd_action='snom_handle_x0304_state(device_id, unit_id, interface_id, cmd_id, hl)')
        scmd4 = HFS2CCommand(cmd_id=4, cmd_name='Double Press', cmd_action='snom_handle_x0304_state(device_id, unit_id, interface_id, cmd_id, hl)')

        interface = HFInterface(intrf_id=0x0304, intrf_name='Simple Button',
                                server_attributes=[sa1,sa2,sa3], 
                                c2s_cmds=[],
                                s2c_cmds=[scmd1,scmd2,scmd3,scmd4])
        self.add_interface(interface)
                
        '''
        # Simple Visual Control Interface
        # server commands
        attribute_type=2 # U16
        attribute_descriptions = [('Duration ms', 0, attribute_type*8),
                                  ]
        scma1 = HFServerAttribute(attribute_id=1, attribute_name='Duration ms', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)
    
        scmd1 = HFS2CCommand(cmd_id=1, cmd_name='On', cmd_action='action_on_button_changed(device_id, payload)', payload_descriptions=[scma1])

        scmd2 = HFS2CCommand(cmd_id=2, cmd_name='Off', cmd_action='action_on_button_changed(device_id, payload)')

        attribute_type=2 # U16
        attribute_descriptions = [('On Duty Cycle', 0, attribute_type*8),
                                  ]
        scma1 = HFServerAttribute(attribute_id=1, attribute_name='On Duty Cycle', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Off Duty Cycle', 0, attribute_type*8),
                                  ]
        scma2 = HFServerAttribute(attribute_id=2, attribute_name='Off Duty Cycle', attribute_type=attribute_type, attribute_values=[0x00,0xff],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Number of Duty Cycle', 0, attribute_type*8),
                                  ]
        scma2 = HFServerAttribute(attribute_id=2, attribute_name='Number of Duty Cycle', attribute_type=attribute_type, attribute_values=[0x00,0x05],
                                attribute_descriptions=attribute_descriptions)


           
        scmd3 = HFS2CCommand(cmd_id=3, cmd_name='Blink', cmd_action='action_on_button_changed(device_id, payload)', payload_descriptions=[scma1])
        '''

        ############


        # Simple Thermostat Interface
        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Heating only                0x01', 0, 1),
                                  ('Cooling only                0x02', 1, 2),
                                  ('Heating and Cooling         0x04', 2, 3),
                                  ('Fan not supported           0x10', 4, 5),
                                  ('Fan supported               0x20', 5, 6),
                                  ('Fan supports automatic mode 0x40', 6, 7),
                                  ('Supported Modes                 ', 0, attribute_type*8),
                                  ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Supported Modes', attribute_type=attribute_type, attribute_values=[0xa0],
                                attribute_descriptions=attribute_descriptions)
        
        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Operating Mode    ', 0, attribute_type*8),
                                  [('Heating mode   0x01', 0, 1),
                                  ('Cooling mode   0x02', 1, 2),
                                  ('Automatic mode 0x04', 2, 3)],
                                  ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Opertating Mode', attribute_type=attribute_type, attribute_values=[0x04],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Fan Mode                ', 0, attribute_type*8),
                                  [('Fan is OFF          0x10', 4, 5),
                                  ('Fan is ON           0x20', 5, 6),
                                  ('Fan is in AUTO mode 0x40', 6, 7)],
                                  ]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Fan Mode', attribute_type=attribute_type, attribute_values=[0x40],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Heating Mode Temperature', 0, attribute_type*8, True),
                                  ]
        sa4 = HFServerAttribute(attribute_id=4, attribute_name='Heating Mode Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Cooling Mode Temperature', 0, attribute_type*8, True),
                                  ]
        sa5 = HFServerAttribute(attribute_id=5, attribute_name='Cooling Mode Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Automatic Mode Heating Temperature', 0, attribute_type*8, True),
                                  ]
        sa6 = HFServerAttribute(attribute_id=6, attribute_name='Automatic Mode Heating Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Automatic Mode Cooling Temperature', 0, attribute_type*8, True),
                                  ]
        sa7 = HFServerAttribute(attribute_id=7, attribute_name='Automatic Mode Cooling Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Heating Mode Temperature Offset', 0, attribute_type*8, True),
                                  ]
        sa8 = HFServerAttribute(attribute_id=8, attribute_name='Heating Mode Temperature Offset', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Cooling Mode Temperature Offset', 0, attribute_type*8, True),
                                  ]
        sa9 = HFServerAttribute(attribute_id=9, attribute_name='Cooling Mode Temperature Offset', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Boost Duration', 0, attribute_type*8),
                                  ]
        sa10 = HFServerAttribute(attribute_id=10, attribute_name='Boost Duration', attribute_type=attribute_type, attribute_values=[0x64],
                                attribute_descriptions=attribute_descriptions)

        # cmd with complex payload
        attribute_type=1 # U8 
        payload_descriptions = [
                                ('Duration', 0, attribute_type*8),
                               ]
        pl1 = HFServerAttribute(attribute_id=1, attribute_name='Duration', attribute_type=attribute_type, attribute_values=[0xa0],
                                attribute_descriptions=payload_descriptions)
        
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='Boost Start', payload_descriptions=[pl1])
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Boost Stop')
        
        interface = HFInterface(intrf_id=0x0303, intrf_name='Simple Thermostat',
                                server_attributes=[sa1,sa2,sa3,sa4,sa5,sa6,sa7,sa8,sa9,sa10], 
                                c2s_cmds=[cmd1,cmd2],
                                s2c_cmds=[])
        self.add_interface(interface)
        
        # Simple Temperature
        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Measured Temperature 1/100 of C', 0, attribute_type*8, True),
                                  ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Measured Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Minimum Measureable Temperature 1/100 of C', 0, attribute_type*8, True),
                                  ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Minimum Measureable Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=2 # S16
        attribute_descriptions = [
                                  ('Maximum Measureable Temperature 1/100 of C', 0, attribute_type*8, True),
                                  ]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Maximum Measureable Temperature', attribute_type=attribute_type, attribute_values=[0xf0,0xf0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Tolerance', 0, attribute_type*8),
                                  ]
        sa4 = HFServerAttribute(attribute_id=4, attribute_name='Tolerance', attribute_type=attribute_type, attribute_values=[0x64],
                                attribute_descriptions=attribute_descriptions)

        interface = HFInterface(intrf_id=0x0301, intrf_name='Simple Temperature',
                                server_attributes=[sa1,sa2,sa3,sa4], 
                                c2s_cmds=[],
                                s2c_cmds=[])
        self.add_interface(interface)
        
        # simple power meter
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code x/h', 32, attribute_type*8),
                                  [('Precision Code 0x00 1W/h ', 32, 38),
                                  ('Precision Code 0x10 milli', 32, 38),
                                  ('Precision Code 0x11 micro', 32, 38),
                                  ('Precision Code 0x12 nano ', 32, 38),
                                  ('Precision Code 0x13 pico ', 32, 38),
                                  ('Precision Code 0x20 kilo ', 32, 38),
                                  ('Precision Code 0x21 mega ', 32, 38),
                                  ('Precision Code 0x22 giga ', 32, 38),
                                  ('Precision Code 0x23 tera ', 32, 38)],
                                  ('Energy', 0, 32),
]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Energy', attribute_type=attribute_type, attribute_values=[0x23,0x0a,0x0a,0x0a,0x0a],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code 0x00 -', 32, attribute_type*8),
                                  ('Energy at Last Reset', 0, 32),
                                  ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Energy at Last Reset', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Time Code 0x00', 32, attribute_type*8),
                                  ('Uptime    0x00', 32, 33),
                                  ('UTC       0x01', 32, 33),
                                  ('Time at Last Reset', 0, 32),
                                  ]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Time at Last Reset', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code 0x00', 32, attribute_type*8),
                                  ('Instantaneous Power', 0, 32),
                                  ]
        sa4 = HFServerAttribute(attribute_id=4, attribute_name='Instantaneous Power', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code 0x00', 32, attribute_type*8),
                                  ('Average Power', 0, 32),
                                  ]
        sa5 = HFServerAttribute(attribute_id=5, attribute_name='Average Power', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Average Power Interval', 0, attribute_type*8),
                                  ]
        sa6 = HFServerAttribute(attribute_id=6, attribute_name='Average Power Interval', attribute_type=attribute_type, attribute_values=[0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [ ('Precision Code 0x00', 32, attribute_type*8),
                                  ('Voltage', 0, 32),
                                  ]
        sa7 = HFServerAttribute(attribute_id=7, attribute_name='Voltage', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code', 32, attribute_type*8),
                                  ('Current       ', 0, 32),
                                  ]
        sa8 = HFServerAttribute(attribute_id=8, attribute_name='Current', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=5 # U8 + U32
        attribute_descriptions = [('Precision Code', 32, attribute_type*8),
                                  ('Frequency       ', 0, 32),
                                  ]
        sa9 = HFServerAttribute(attribute_id=9, attribute_name='Frequency', attribute_type=attribute_type, attribute_values=[0,0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=1 # U8
        attribute_descriptions = [
                                  ('Power Factor', 0, attribute_type*8),
                                  ]
        sa10 = HFServerAttribute(attribute_id=10, attribute_name='Power Factor', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('Report Interval', 0, attribute_type*8),
                                  ]
        sa11 = HFServerAttribute(attribute_id=11, attribute_name='Report Interval', attribute_type=attribute_type, attribute_values=[0,0],
                                attribute_descriptions=attribute_descriptions)
       
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='Measurement Reset')
        
        # server report command has complex payload
        attribute_type=7 # U8 + U8 + U8+U32
        attribute_descriptions = [('Number of Attributes ', 48, attribute_type*8),
                                  ('Attribute ID   ', 40, 48),
                                  ('Attribute value', 0, 40),
                                  ('Sample Precision Code ', 32, 40),
                                  ('Sample Current        ', 0, 32),
                                  ]
        scma1 = HFServerAttribute(attribute_id=1, attribute_name='Report', attribute_type=attribute_type, attribute_values=[1,8,0x22,0x0a,0x0a,0x0a,0xff],
                                attribute_descriptions=attribute_descriptions)
    
        scmd1 = HFS2CCommand(cmd_id=1, cmd_name='Report', cmd_action='snom_handle_x0300_report(device_id, unit_id, interface_id, cmd_id, hl)', payload_descriptions=[scma1])

        interface = HFInterface(intrf_id=0x0300, intrf_name='Simple Power Metering',
                                server_attributes=[sa1,sa2,sa3,sa4,sa5,sa6,sa7,sa8,sa9,sa10,sa11], 
                                c2s_cmds=[cmd1],
                                s2c_cmds=[scmd1])
        self.add_interface(interface)


        # 5.1 Alert Interface
        '''
        Attribute ID 0x01 0x02
        Attribute Name State Enable
        Attribute Type U32 (bitmask) U32 (bitmask)
        Attribute Values 0x00000000 - 0xFFFFFFFF 0x00000000 - 0xFFFFFFFF
        Attribute Access M/O Read Only M Read / Write M
        '''
        attribute_type=4 # U32
        attribute_descriptions = [('State', 0, attribute_type*8),]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [('Enable', 0, attribute_type*8),]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Enable', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        # S2C Commands
        scmd1 = HFS2CCommand(cmd_id=1, cmd_name='Status', cmd_action='snom_handle_x0100_state(device_id, unit_id, interface_id, cmd_id, hl)')
        alert = HFInterface(intrf_id=256, intrf_name='Alert', server_attributes=[sa1, sa2], c2s_cmds=[], s2c_cmds=[scmd1])
        self.add_interface(alert)

        # Level Control Interface
        # U8 with percentage
        '''
        5.3.1 Server Attributes
        Table 8 - Level Control Interface Server, Attributes
        Attribute ID Attribute Name Attribute Type Attribute Values Attribute Access M/O
        0x01 Current Level U8 0x00 - 0xFF Read / Write M 5.3.1.1 Current Level
        Current Level attribute indicates the current value is a percentage of the maximum value allowed. The maximum value is device dependent, but Current Level is not. For example:
         a value of 0xFF (255) indicates 100% of maximum value.
         a value of 0x80 (128) indicates 50% of maximum value.
        '''
        attribute_type=1 # U8
        attribute_descriptions = [('Current Level', 0, 8),
                                 ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Current Level', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)

        attribute_type=1 # U8
        payload_descriptions = [
                                ('Level', 0, attribute_type*8),                                
                               ]
        
        pl1 = HFServerAttribute(attribute_id=1, attribute_name='Set Level', attribute_type=attribute_type, attribute_values=[0x7f],
                                attribute_descriptions=payload_descriptions)
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='Set Level', payload_descriptions=[pl1])
        
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Increase Level')
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='Decrease Level')
        # when COV reports happen, we want to fire an URL as well
        # COV sends a cmd from interface 6 with cmd_id=2
        scmd2 = HFS2CCommand(cmd_id=2, cmd_name='Level Report', cmd_action='action_on_report_level_changed(device_id, level)')

        lc = HFInterface(intrf_id=513, intrf_name='Level Control', server_attributes=[sa1], 
                         c2s_cmds=[cmd1, cmd2, cmd3],
                         s2c_cmds=[scmd1, scmd2])
        self.add_interface(lc)

        # On/Off Interface
        # U8 with only 1 bits used
        attribute_type=1 # U8
        attribute_descriptions = [('State', 0, 8),
                                 [('On  = 0x01', 0, 1),
                                  ('Off = 0x00', 0, 1)],
                                 ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='On')
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Off')
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='Toggle')
        onoff = HFInterface(intrf_id=512, intrf_name='On/Off', server_attributes=[sa1], c2s_cmds=[cmd1, cmd2, cmd3])
        for cmd in onoff.get_c2s_cmds() or []:
            print(cmd)
        self.add_interface(onoff)
        
        
        # OpenClose Interface
        attribute_type=1 # U8
        attribute_descriptions = [('Open/Close', 0,8),
                                  [('On  = 0x01', 0, 1),
                                   ('Off = 0x00', 0, 1)],
                                  [('in close direction = 0x00', 1, 2),
                                   ('in open direction  = 0x01', 1, 2)],
                                 ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='Open')
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Close')
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='Stop')
        openclose = HFInterface(intrf_id=516, intrf_name='OpenClose', server_attributes=[sa1], c2s_cmds=[cmd1, cmd2, cmd3])
        #print(openclose)
        self.add_interface(openclose)

        # 4.1	OpenClose Device Configuration
        attribute_type=1 # U8
        attribute_descriptions = [('Set End Position supported                ', 0, 1),
                                  ('Automatic End Position detection supported', 1, 2),
                                  ('Detect End Position Command supported     ', 2, 3),
                                 ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='End Position Setting Option', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [('Opened End Position Status', 0, 1),
                                  ('Closed End Position Status', 1, 2),
                                 ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='End Position State', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [[('Standard Direction=0', 0, 1),
                                  ('Reverse Direction =1', 0, 1),
                                 ]]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Motor Direction', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [('Fly Guard Protection', 0, 8),
                                 [('disabled=0', 0, 1),
                                  ('enabled =1', 0, 1)],
                                 ]
        sa4 = HFServerAttribute(attribute_id=4, attribute_name='Fly Guard Protection', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [('Freeze Protection', 0, 8),
                                  [('disabled=0', 0, 1),
                                  ('enabled =1', 0, 1)],
                                 ]
        sa5 = HFServerAttribute(attribute_id=5, attribute_name='Freeze Protection', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        # default open on loss
        attribute_descriptions = [('Behaviour on ULE Connection loss', 0, 8),
                                  [('Do Nothing=0', 0, 2),
                                  ('Open      =1', 0, 2),
                                  ('Close     =3', 0, 2)],
                                 ]
        sa6 = HFServerAttribute(attribute_id=6, attribute_name='Behaviour on ULE Connection loss', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_descriptions = [('Behaviour Rain', 0, 8),
                                  [('Do Nothing=0', 0, 2),
                                  ('Open      =1', 0, 2),
                                  ('Close     =3', 0, 2)],
                                 ]
        sa7 = HFServerAttribute(attribute_id=7, attribute_name='Behaviour Rain', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=4 # U32
        attribute_descriptions = [('Cycle Count', 0, 32),
                                 ]
        sa8 = HFServerAttribute(attribute_id=8, attribute_name='Cycle Count', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
        # C2S Commands
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='Reset')
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Detect End Positions')
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='Set End Position Opened')
        cmd4 = HFC2SCommand(cmd_id=4, cmd_name='Set End Position Closed')
        # S2C Commands
        scmd1 = HFS2CCommand(cmd_id=1, cmd_name='End Position Changed', cmd_action='action_on_end_position_changed(1)')
        opencloseconf = HFInterface(intrf_id=517, intrf_name='OpenClose Device Configuration', 
                                    server_attributes=[sa1, sa2, sa3, sa4, sa5, sa6, sa7, sa8], 
                                    c2s_cmds=[cmd1, cmd2, cmd3, cmd4], 
                                    s2c_cmds=[scmd1])
        #print(opencloseconf)
        self.add_interface(opencloseconf)


        # Color Control Interface
        attribute_type=1 # U8
        attribute_descriptions = [('Supported Colour Modes', 0, attribute_type*8),
                                  ('Mode H/S              ', 0, 1),
                                  ('CIE 1931              ', 1, 2),
                                  ('Colour Temperature    ', 2, 3),
                                 ]
        sa1 = HFServerAttribute(attribute_id=1, attribute_name='Supported Colour Modes', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=1 # U8
        attribute_descriptions = [('Current Colour Mode', 0, attribute_type*8),
                                  [('Mode H/S           = 0x01', 0, 1),
                                   ('CIE 1931           = 0x02', 1, 2),
                                   ('Colour Temperature = 0x04', 2, 3)],
                                 ]
        sa2 = HFServerAttribute(attribute_id=2, attribute_name='Current Colour Mode', attribute_type=attribute_type, attribute_values=[0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=3 # U16+U8
        attribute_descriptions = [
                                  ('Hue 0-359 ', 8, attribute_type*8),
                                  ('Saturation', 0, 8),
                                 ]
        sa3 = HFServerAttribute(attribute_id=3, attribute_name='Current Hue Saturation', attribute_type=attribute_type, attribute_values=[0,0,0],
                                attribute_descriptions=attribute_descriptions)
        attribute_type=4 # U16+U16
        attribute_descriptions = [
                                  ('X = Current X/65535', 16, attribute_type*8),
                                  ('Y = Current Y/65535', 0, 16),
                                 ]
        sa4 = HFServerAttribute(attribute_id=4, attribute_name='Current XY', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
      
        attribute_type=2 # U16
        attribute_descriptions = [
                                  ('CT=1,000,000/Mired value   ', 0, attribute_type*8),
                                 ]
        sa5 = HFServerAttribute(attribute_id=5, attribute_name='Current Colour Temperature', attribute_type=attribute_type, attribute_values=[0,0],
                                attribute_descriptions=attribute_descriptions)
        # C2S Commands
        # cmd with complex payload
        attribute_type=5 # U16+U8+U16 
        payload_descriptions = [('Hue 0-359                         ', 16+8, attribute_type*8),
                                ('Direction                         ', 16, 16+8),
                                [('Direction Up                = 0x01', 16, 19),
                                ('Direction Down              = 0x02', 16, 19),
                                ('Direction Shortest Distance = 0x03', 16, 19),
                                ('Direction Longest Distance  = 0x04', 16, 19)],
                                ('Transition Time 100ms             ', 0, 16),
                              ]
        pl1 = HFServerAttribute(attribute_id=1, attribute_name='MoveToHue', attribute_type=attribute_type, attribute_values=[0x00,0x00,0x03,0x0,0x1],
                                attribute_descriptions=payload_descriptions)
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='MoveToHue', payload_descriptions=[pl1])
        
        attribute_type=3 # U8+U16 
        payload_descriptions = [
                                ('Direction            ', 16, attribute_type*8),
                                [('Direction Up   = 0x01', 16, 17),
                                ('Direction Down = 0x02', 17, 18)],
                                ('Degrees/second 0-359 ', 0, 16),
                              ]
        pl2 = HFServerAttribute(attribute_id=2, attribute_name='MoveHue', attribute_type=attribute_type, attribute_values=[0x01,0x00,0x64],
                                attribute_descriptions=payload_descriptions)
        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='MoveHue', payload_descriptions=[pl2])
    
        attribute_type=3 # U8+U8+U8
        payload_descriptions = [
                                ('Step Size             ', 16, attribute_type*8),
                                ('Direction             ', 8, 16),
                                [('Direction Up    = 0x01', 8, 9),
                                ('Direction Down  = 0x02', 9, 10)],
                                ('Transition Time 100ms ', 0, 8),
                              ]
        pl3 = HFServerAttribute(attribute_id=3, attribute_name='StepHue', attribute_type=attribute_type, attribute_values=[0x0A,0x01,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='StepHue', payload_descriptions=[pl3])
      
        attribute_type=4 # U8+U8+U16
        payload_descriptions = [
                                ('Saturation            ', 24, attribute_type*8),
                                ('Direction             ', 16, 24),
                                [('Direction Up    = 0x01', 16, 17),
                                ('Direction Down  = 0x02', 17, 18)],
                                ('Transition Time 100ms ', 0, 16),
                              ]
        pl4 = HFServerAttribute(attribute_id=4, attribute_name='MoveToSaturation', attribute_type=attribute_type, attribute_values=[0xff,0x01,0x00,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd4 = HFC2SCommand(cmd_id=4, cmd_name='MoveToSaturation', payload_descriptions=[pl4])
    
        attribute_type=2 # U8+U8
        payload_descriptions = [
                                ('Direction             ', 8, attribute_type*8),
                                [('Direction Up    = 0x01', 8, 9),
                                ('Direction Down  = 0x02', 9, 10)],
                                ('Rate steps/s          ', 0, 8),
                              ]
        pl5 = HFServerAttribute(attribute_id=5, attribute_name='MoveSaturation', attribute_type=attribute_type, attribute_values=[0x01,0x0a],
                                attribute_descriptions=payload_descriptions)
        cmd5 = HFC2SCommand(cmd_id=5, cmd_name='MoveSaturation', payload_descriptions=[pl5])
    
        attribute_type=3 # U8+U8+U8
        payload_descriptions = [
                                ('Step Size             ', 16, attribute_type*8),
                                ('Direction             ', 8, 16),
                                [('Direction Up    = 0x01', 8, 10),
                                ('Direction Down  = 0x02', 8, 10)],
                                ('Transition Time 100ms ', 0, 8),
                              ]
        pl6 = HFServerAttribute(attribute_id=6, attribute_name='StepSaturation', attribute_type=attribute_type, attribute_values=[0x0a,0x01,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd6 = HFC2SCommand(cmd_id=6, cmd_name='StepSaturation', payload_descriptions=[pl6])
    
        attribute_type=6 # U16+U8+U8+U16
        payload_descriptions = [('Hue 0-359                         ', 32, attribute_type*8),
                                ('Saturation                        ', 24, 32),
                                ('Direction                         ', 16, 24),
                                [('Direction Up                = 0x01', 16, 19),
                                ('Direction Down              = 0x02', 16, 19),
                                ('Direction Shortest Distance = 0x03', 16, 19),
                                ('Direction Longest Distance  = 0x04', 16, 19)],  
                                ('Transition Time 100ms             ', 0, 16),
                              ]
        pl7 = HFServerAttribute(attribute_id=7, attribute_name='MoveToHueAndSaturation', attribute_type=attribute_type, attribute_values=[0x00,0x00,0xff,0x03,0x00,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd7 = HFC2SCommand(cmd_id=7, cmd_name='MoveToHueAndSaturation', payload_descriptions=[pl7])
    
        attribute_type=6 # U16+U16+U16
        payload_descriptions = [
                                  ('X = X/65535          ', 32, attribute_type*8),
                                  ('Y = Y/65535          ', 16, 32),
                                  ('Transition Time 100ms', 0, 16),
                                 ]        
        pl8 = HFServerAttribute(attribute_id=8, attribute_name='MoveToXY', attribute_type=attribute_type, attribute_values=[0x00,0x00,0x00,0x00,0x00,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd8 = HFC2SCommand(cmd_id=8, cmd_name='MoveToXY', payload_descriptions=[pl8])

        attribute_type=4 # S16+S16+U8
        payload_descriptions = [
                                  ('Rate of X +Sign', 16, 32, True),
                                  ('Rate of Y +Sign', 0, 16, True),
                                 ]        
        pl9 = HFServerAttribute(attribute_id=9, attribute_name='MoveXY', attribute_type=attribute_type, attribute_values=[0x03,0x00,0x03,0x00],
                                attribute_descriptions=payload_descriptions)
        cmd9 = HFC2SCommand(cmd_id=9, cmd_name='MoveXY', payload_descriptions=[pl9])

        attribute_type=5 # S16+S16+U8
        payload_descriptions = [
                                ('X Step Size +Sign    ', 24, attribute_type*8, True),
                                ('Y Step Size +Sign    ', 8, 24, True),
                                ('Transition Time 100ms', 0, 8),
                               ]        
        pl10 = HFServerAttribute(attribute_id=10, attribute_name='StepXY', attribute_type=attribute_type, attribute_values=[0x0a,0x0a,0x0a,0x0a,0x1],
                                attribute_descriptions=payload_descriptions)
        cmd10 = HFC2SCommand(cmd_id=10, cmd_name='StepXY', payload_descriptions=[pl10])

        attribute_type=4 # U16+U16
        payload_descriptions = [
                                ('Colour Temperature Mired', 16, attribute_type*8),
                                ('Transition Time 100ms   ', 0, 16),
                               ]        
        pl11 = HFServerAttribute(attribute_id=11, attribute_name='MoveToColourTemperature', attribute_type=attribute_type, attribute_values=[0x03,0xe8,0x00,0x01],
                                attribute_descriptions=payload_descriptions)
        cmd11 = HFC2SCommand(cmd_id=11, cmd_name='MoveToColourTemperature', payload_descriptions=[pl11])

        cmd12 = HFC2SCommand(cmd_id=12, cmd_name='Stop')

        colour = HFInterface(intrf_id=0x0202, intrf_name='Colour Control Interface', 
                                    server_attributes=[sa1, sa2, sa3, sa4, sa5], 
                                    c2s_cmds=[cmd1,cmd2,cmd3,cmd4,cmd5,cmd6,cmd7,cmd8,cmd9,cmd10,cmd11,cmd12])
        self.add_interface(colour)


        #Simple Visual Control Interface

        attribute_type=2 # U16
        payload_descriptions = [
                                ('Duration ms', 0, attribute_type*8),
                               ]        
        pl1 = HFServerAttribute(attribute_id=1, attribute_name='Duration', attribute_type=attribute_type, attribute_values=[0x01,0x02],
                                attribute_descriptions=payload_descriptions)
        cmd1 = HFC2SCommand(cmd_id=1, cmd_name='On', payload_descriptions=[pl1])

        cmd2 = HFC2SCommand(cmd_id=2, cmd_name='Off')

        attribute_type=6 # U16+U16+U16
        payload_descriptions = [
                                 ('On Duty Cycle   ', 32, attribute_type*8),
                                 ('Off Duty Cycle  ', 16, 32),
                                 ('Number of Cycles', 0, 16),
                               ]        
        pl3 = HFServerAttribute(attribute_id=3, attribute_name='Blink', attribute_type=attribute_type, attribute_values=[0x01,0x02,0x01,0x02,0x01,0x02],
                                attribute_descriptions=payload_descriptions)
        cmd3 = HFC2SCommand(cmd_id=3, cmd_name='Blink', payload_descriptions=[pl3])

        attribute_type=4 # U8+U8+U16
        payload_descriptions = [
                                 ('Starting Brightness', 24, attribute_type*8),
                                 ('Final Brightness   ', 16, 24),
                                 ('Fade Duration      ', 0, 16),
                               ]        
        pl4 = HFServerAttribute(attribute_id=4, attribute_name='Fade', attribute_type=attribute_type, attribute_values=[0x01,0x02,0x01,0x02],
                                attribute_descriptions=payload_descriptions)
        cmd4 = HFC2SCommand(cmd_id=4, cmd_name='Fade', payload_descriptions=[pl4])

        attribute_type=12 # U8+U16+U16+U8+U16+U16+U16
        payload_descriptions = [
                                 ('First Brightness             ', 88, attribute_type*8),
                                 ('First Brightness Duration    ', 72, 88),
                                 ('First to Second Fade Duration', 56, 72),
                                 ('Second Brightness            ', 48, 56),
                                 ('Second Brightness Duration   ', 32, 48),
                                 ('Second to First Fade Duration', 16, 32),
                                 ('Number of Cycles             ', 0, 16),
                               ]        
        pl5 = HFServerAttribute(attribute_id=5, attribute_name='Breath', attribute_type=attribute_type, 
                                attribute_values=[0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02],
                                attribute_descriptions=payload_descriptions)
        cmd5 = HFC2SCommand(cmd_id=5, cmd_name='Breath', payload_descriptions=[pl5])

        visualcontrol = HFInterface(intrf_id=0x0305, intrf_name='Simple Visual Control', 
                                    server_attributes=[], 
                                    c2s_cmds=[cmd1,cmd2,cmd3,cmd4,cmd5])
        self.add_interface(visualcontrol)

        '''
        print(f'######################################')
        print(f'Interface list:')
        print(f'{self.interfaces}')
        print(f'######################################')
        print(self.get_interface_by_name('Level Control'))
        print(self.get_interface_by_id(516))
        self.get_interface_by_id(517).print_server_attributes()
        '''

@dataclass
class HFProfile:
    profile_id : int = 0 
    profile_name : str = ''

@dataclass
class HFProfiles:
    profile_list_name : str = 'Current available Profiles'
    profiles : List[HFProfile] = field(default_factory=lambda: [])

    def __post_init__(self):
        self.create_known_profiles()

    def add_profile(self, profile: HFProfile):
        try:
            self.profiles.append(copy.deepcopy(profile))
        except:
            logging.exception(f'cannot add Profile {profile} to list.') 

    def get_profiles(self) -> List[HFProfile]:
        if len(self.profiles) > 0:
            return self.profiles
        else:
            print(f'no HFProfiles specified.') 

    def get_profile_by_id(self, profile_id) -> HFProfile:
        ## hex value as string should also be supported.
        if len(self.profiles) > 0:
            match = next((profile for profile in self.profiles if profile and profile.profile_id == profile_id), None)
            if match:
                return match
            else:
                print(f'cannot find Profile={profile_id}')
        else:
            print(f'cannot find profile={profile_id}, no HFProfile specified.') 
        return HFProfile(profile_id=profile_id, profile_name='unknown')

    def create_known_profiles(self):
        # service interfaces and profiles
        p1 = HFProfile(profile_id=int(0x0), profile_name='Reserved')
        self.add_profile(p1)
        # profiles
        p1 = HFProfile(profile_id=int(0x0100), profile_name='Simple OnOff Switchable')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0101), profile_name='Simple On-Off Switch')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0102), profile_name='Simple Level Controllable')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0103), profile_name='Simple Level Control')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0104), profile_name='Simple Level Controllable Switchable')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0105), profile_name='Simple Level Control switch')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0106), profile_name='AC Outlet')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0107), profile_name='AC Outlet with Simple Power Metering')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0108), profile_name='Simple Light')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0109), profile_name='Dimmable Light')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010A), profile_name='Dimmer Switch')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010B), profile_name='Simple Door Lock')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010C), profile_name='Simple Door Bell')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010D), profile_name='Simple Power Meter')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010E), profile_name='Simple Temperature Sensor')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x010F), profile_name='Simple Humidity Sensor')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0110), profile_name='Simple Air Pressure Sensor')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0111), profile_name='Simple Button')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0112), profile_name='Controllable Thermostat')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0113), profile_name='Simple Led')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0114), profile_name='Environment Monitor')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0115), profile_name='Colour Bulb')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0116), profile_name='Dimmable Colour Bulb')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0117), profile_name='Tracker')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0118), profile_name='Simple Keypad')
        self.add_profile(p1)
        # new 
        p1 = HFProfile(profile_id=int(0x0119), profile_name='Blind')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x011A), profile_name='Lamellar')
        self.add_profile(p1)
        
        p1 = HFProfile(profile_id=int(0x0200), profile_name='Simple Detector')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0201), profile_name='Door Open Close Detector')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0202), profile_name='Window Open/Close Detector')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0203), profile_name='Motion Detector')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0204), profile_name='Smoke Detector')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0205), profile_name='Gas Detector')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0206), profile_name='Flood Detector')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0207), profile_name='Glass Break Detector')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0208), profile_name='Vibration Detector')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0209), profile_name='Simple Light Sensor')
        self.add_profile(p1)

        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0280), profile_name='Siren')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0281), profile_name='Alertable')
        self.add_profile(p1)
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0300), profile_name='Simple Pendant')
        self.add_profile(p1)

        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0401), profile_name='UI Lock')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0410), profile_name='User Interface')
        self.add_profile(p1)
        p1 = HFProfile(profile_id=int(0x0411), profile_name='Generic Application Logic')
        self.add_profile(p1)

        # Proprietary 
        # 0xFF00 - 0xFFFF
        
@dataclass
class HFUnit:
    unit_id : int
    profile : HFProfile
    unit_name : str = ''
    interfaces : List[HFInterface] = field(default_factory=lambda: [])

    def __init__(self, unit_id: int, profile: HFProfile, unit_name: str = '', interfaces: List[HFInterface] = []):
        self.unit_id = unit_id
        self.profile = copy.deepcopy(profile)
        self.unit_name = unit_name
        self.interfaces = copy.deepcopy(interfaces)
        #print('deepcopy profile and interfaces')

    def get_interface_by_id(self, intrf_id) -> HFInterface:
        ## hex value as string should also be supported.
        if len(self.interfaces) > 0:
            match = next((interface for interface in self.interfaces if interface and interface.intrf_id == intrf_id), None)
            if match:
                return match
            else:
                print(f'cannot find Interface={intrf_id} in Unit={self.unit_id}')
        else:
            print(f'cannot find an interface, no HFInterfaces specified.') 
        return HFInterface(intrf_id=intrf_id, intrf_name="unknown" )

@dataclass
class HFUnitsa:
    unit_list_name : str = 'Current available Units'
    units : List[HFUnit] = field(default_factory=lambda: [])

    def add_unit(self, unit: HFUnit):
        try:
            self.units.append(copy.deepcopy(unit))
        except:
            logging.exception(f'cannot add Unit {unit} to list.') 

    def get_units(self) -> List[HFUnit]:
        if len(self.units) > 0:
            return self.units
        else:
            print(f'no HFUnits specified.') 

    def get_unit_by_id(self, unit_id) -> HFUnit:
        ## hex value as string should also be supported.
        if len(self.units) > 0:
            match = next((unit for unit in self.units if unit and unit.unit_id == unit_id), None)
            if match:
                return match
            else:
                print(f'cannot find Unit={unit_id}')
        else:
            print(f'cannot find a unit, no HFUnit specified.') 
        return None

@dataclass
class HFDevice:
    device_id : int
    device_ipui : str
    device_name : str
    units : List[HFUnit] = field(default_factory=lambda: [])

    def get_unit_by_id(self, unit_id: int) -> HFUnit:
        ## hex value as string should also be supported.
        if len(self.units) > 0:
            match = next((unit for unit in self.units if unit and unit.unit_id == unit_id), None)
            if match:
                return match
            else:
                print(f'cannot find Unit {unit_id} in device {self.device_id}')
        else:
            print(f'cannot find unit={unit_id}, no HFUnits specified.') 
        return None

@dataclass
class HFDevices:
    device_list_name : str = 'Currently available Devices'
    devices : List[HFDevice] = field(default_factory=lambda: [])

    def add_device(self, device: HFDevice) -> bool:
        try:
            self.devices.append(copy.deepcopy(device))
            return True
        except:
            logging.exception(f'cannot add Device {device} to list.') 
            return False

    def delete_device_by_id(self, device_id: int) -> bool:
        dev = self.get_device_by_id(device_id)
        if dev != None:
            try:
                self.devices.remove(dev)
            except:
                logging.exception(f'cannot remove device {device_id}: {dev}.')
                return False
        return True

    def update_device(self, device: HFDevice) -> bool:
        try:
            dev = self.get_device_by_id(device.device_id)
            if dev != None:
                self.delete_device_by_id(dev.device_id)
            self.devices.append(copy.deepcopy(device))
            return True
        except:
            logging.exception(f'cannot update Device {device}.')
            return False
    
    def get_devices(self) -> List[HFDevice]:
        if len(self.devices) > 0:
            return self.devices
        else:
            print(f'no HFDevices specified.') 

    def get_device_by_name(self, device_name) -> HFDevice:
        if len(self.devices) > 0:
            match = next((device for device in self.devices if device and device.device_name == device_name), None)
            if match:
                return match
            else:
                print(f'cannot find SDevice={device_name}')
        else:
            print(f'cannot find a device, no HFDevices specified.') 
        return None

    def get_device_by_id(self, device_id) -> HFDevice:
        ## hex value as string should also be supported.
        if len(self.devices) > 0:
            match = next((device for device in self.devices if device and device.device_id == device_id), None)
            if match:
                return match
            else:
                print(f'cannot find Device={device_id}')
        else:
            print(f'cannot find an device, no HFDevices specified.') 
        return None
    
def dev_update_to_becker_rolladen(dev: HFDevice) -> bool:
    dev.device_name = 'Becker Rolladenmotor'
    for u in dev.units:
        print(f'process unit {u.unit_name}')
        if u.profile.profile_id == 0x0119:
            attribute_type=4 # U32
            attribute_descriptions = [
                                      ('Wind Alarm ', 0, 1),
                                      ('Rain Alert ', 1, 2),
                                      ('Smoke Alert', 2, 3),
                                      ]
            sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                    attribute_descriptions=attribute_descriptions)

            attribute_descriptions = [
                                      ('Wind Alarm ', 0, 1),
                                      ('Rain Alert ', 1, 2),
                                      ('Smoke Alert', 2, 3),
                                      ]
            sa2 = HFServerAttribute(attribute_id=2, attribute_name='Enable', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
     
            # S2C Commands
            #scmd1 = HFS2CCommand(cmd_id=2, cmd_name='Status', cmd_action='snom_handle_x0100_state(device_id, unit_id, interface_id, cmd_id, hl)')
            #alert = HFInterface(intrf_id=256, intrf_name='Alert', server_attributes=[sa1, sa2], c2s_cmds=[], s2c_cmds=[scmd1])
            alert = HFInterface(intrf_id=256, intrf_name='Alert', server_attributes=[sa1, sa2], c2s_cmds=[])
            # search to replace
            for i in u.interfaces:
                if i.intrf_id == 0x0100:
                    u.interfaces.remove(i)
                    u.interfaces.append(copy.deepcopy(alert))
        # lamellar
        if u.profile.profile_id == 0x011A:
            dev.device_name = 'Becker Rolladen, Lamellensteuerung'

def dev_update_profile_changes(dev: HFDevice) -> bool:
    known_interfaces = HFInterfaces()
    known_interfaces.create_known_interfaces()

    for u in dev.units:
        if u.profile.profile_id == 0x0119 or u.profile.profile_id == 0x011A:   
            dev_update_to_becker_rolladen(dev)

        # AC Outlet with Simple Power Metering
        if u.profile.profile_id == 0x0107:     
            # just add on top. wrong.. should check each existing interface and add missing.       
            if u.interfaces == [] or True: # profile without interface happens as well..
                onoff = known_interfaces.get_interface_by_name('On/Off')
                temp = known_interfaces.get_interface_by_name('Simple Power Metering')
                u.interfaces.extend(copy.deepcopy([onoff,temp]))
            else: 
                return False       
    
        # Simple Door Bell
        if u.profile.profile_id == 0x010C:
            #alert_interface = 
            attribute_type=4 # U32
            attribute_descriptions = [
                                      ('Simple Door Bell', 0, 1),
                                      ('pressed    = 0x1', 0, 1),
                                      ]
            sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                    attribute_descriptions=attribute_descriptions)
            attribute_descriptions = [
                                      ('Simple Door Bell', 0, 1),
                                      ('pressed    = 0x1', 0, 1),
                                      ]
            sa2 = HFServerAttribute(attribute_id=2, attribute_name='Enable', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
            
            alert = HFInterface(intrf_id=0x0100, intrf_name='Alert', server_attributes=[sa1,sa2], c2s_cmds=[])
            # search to replace
            interfaces_to_add = []
            for i in u.interfaces:
                if i.intrf_id == 0x0100 :
                    u.interfaces.remove(i)
                    interfaces_to_add.append(copy.deepcopy(alert))

            u.interfaces.append(copy.deepcopy(interfaces_to_add))
        
        # Simple Detector
        if u.profile.profile_id == 0x0200:
            attribute_type=4 # U32
            attribute_descriptions = [
                                      ('Simple Detector', 0, 1),
                                      ('Detection = 0x1', 0, 1),
                                      ]
            sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                    attribute_descriptions=attribute_descriptions)
            attribute_descriptions = [
                                      ('Simple Detector', 0, 1),
                                      ('Detection  = 0x1', 0, 1),
                                      ]
            
            sa2 = HFServerAttribute(attribute_id=2, attribute_name='Enable', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                attribute_descriptions=attribute_descriptions)
                                    
            alert = HFInterface(intrf_id=0x0100, intrf_name='Alert', server_attributes=[sa1, sa2], c2s_cmds=[])
            # search to replace
            interfaces_to_add = []
            for i in u.interfaces:
                if i.intrf_id == 0x0100 :
                    u.interfaces.remove(i)
                    interfaces_to_add.append(copy.deepcopy(alert))

            u.interfaces.append(copy.deepcopy(interfaces_to_add))
      
        # Window Open/Close Detector
        if u.profile.profile_id == 0x0202:
            attribute_type=4 # U32
            attribute_descriptions = [
                                      ('Window Open/Close', 0, attribute_type*8),
                                      ('Close       = 0x0', 0, 1),
                                      ('Open        = 0x1', 0, 1),
                                      ]
            sa1 = HFServerAttribute(attribute_id=1, attribute_name='State', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                    attribute_descriptions=attribute_descriptions)
            sa2 = HFServerAttribute(attribute_id=1, attribute_name='Enable', attribute_type=attribute_type, attribute_values=[0,0,0,0],
                                    attribute_descriptions=attribute_descriptions)
            scmd1 = HFS2CCommand(cmd_id=1, cmd_name='Status', cmd_action='snom_handle_x0100_state(device_id, unit_id, interface_id, cmd_id, hl)')
            alert = HFInterface(intrf_id=0x0100, intrf_name='Alert', server_attributes=[sa1, sa2], c2s_cmds=[], s2c_cmds=[scmd1])
        
            #print(alert)
            # search to replace
            interfaces_to_add = []
            if u.interfaces == []: # profile without interface happens as well..
                interfaces_to_add.append(copy.deepcopy(alert))
            else: # search for existing and replace
                for i in u.interfaces:
                    if i.intrf_id == 0x0100:
                        u.interfaces.remove(i)
                        interfaces_to_add.append(copy.deepcopy(alert))

            u.interfaces.extend(copy.deepcopy(interfaces_to_add))

        # Colour Bulb
        if u.profile.profile_id == 0x0115:            
            if u.interfaces == []: # profile without interface happens as well..
                onoff = known_interfaces.get_interface_by_id(0x0200)
                colorcontrol = known_interfaces.get_interface_by_id(0x0202)
                u.interfaces.extend(copy.deepcopy([onoff, colorcontrol]))
            else: 
                return False       
    
        # Dimmable Colour Bulb
        if u.profile.profile_id == 0x0116:            
            if u.interfaces == []: # profile without interface happens as well..
                onoff = known_interfaces.get_interface_by_id(0x0200)
                colorcontrol = known_interfaces.get_interface_by_id(0x0202)
                levelcontrol = known_interfaces.get_interface_by_id(0x0201)
                u.interfaces.extend(copy.deepcopy([onoff, colorcontrol, levelcontrol]))
            else: 
                return False       


        # Simple Temperature Sensor 
        if u.profile.profile_id == 0x010E:            
            if u.interfaces == []: # profile without interface happens as well..
                temp = known_interfaces.get_interface_by_name('Simple Temperature')
                u.interfaces.extend(copy.deepcopy([temp]))
            else: 
                return False       
    
        # Simple Button 
        if u.profile.profile_id == 0x0111:     
            if u.interfaces == [] or True: # profile without interface happens as well..
                simplebutton = known_interfaces.get_interface_by_name('Simple Button')
                u.interfaces.extend(copy.deepcopy([simplebutton]))
            else: 
                return False       
        # Controllable Thermostat 
        if u.profile.profile_id == 0x0112:     
            # just add on top. wrong.. should check each existing interface and add missing.       
            if u.interfaces == [] or True: # profile without interface happens as well..
                onoff = known_interfaces.get_interface_by_name('On/Off')
                temp = known_interfaces.get_interface_by_name('Simple Thermostat')
                u.interfaces.extend(copy.deepcopy([onoff,temp]))
            else: 
                return False       
    
        # UI Lock  
        if u.profile.profile_id == 0x0401:     
            # just add on top. wrong.. should check each existing interface and add missing.       
            if u.interfaces == [] or True: # profile without interface happens as well..
                onoff = known_interfaces.get_interface_by_name('On/Off')
                u.interfaces.extend(copy.deepcopy([onoff]))
            else: 
                return False       

    return True


####
# sample action
####
def action_on_end_position_changed(num: int):
    print(f'fire -> action_on_end_position_changed with {num}')

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
    # blind
    profile1 = HFProfile(profile_id=281, profile_name='Becker Rolladenmotor')
    unit1 = HFUnit(unit_id=1, unit_name='one', profile=profile1,
                  interfaces=[interfaces.get_interface_by_id(256),
                              interfaces.get_interface_by_id(513),
                              interfaces.get_interface_by_id(516),
                              interfaces.get_interface_by_id(517),
                             ])
    # lamellar
    profile2 = HFProfile(profile_id=0x11A, profile_name='Becker Lamellen')
    unit2 = HFUnit(unit_id=2, unit_name='two', profile=profile2,
                  interfaces=[interfaces.get_interface_by_id(517),
                             ])
    new_device = HFDevice(device_id=12, device_ipui = "1234567890", device_name='Becker Rolladenmotor', units=[unit1, unit2])
    
    # update to known vendor 
    dev_update_to_becker_rolladen(new_device)
    dev_update_profile_changes(new_device)
    devices.add_device(new_device)
    
    # check options value 
    interface_t = interfaces.get_interface_by_id(517)
    val = interface_t.get_attribute_by_id(3).get_attribute_value_by_description('Standard Directi')
    print(f'value in optional list = {val}')

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

    '''
    profiles = HFProfiles()
    profile=profiles.get_profile_by_id(0x0115)
    unit = HFUnit(unit_id=1, unit_name='Color', profile=profile,
                  interfaces=[interfaces.get_interface_by_id(0x0202),
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
    '''
    
    # signed values in cmd payload description
    interface_t = interfaces.get_interface_by_id(0x0202)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', -200)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'signed={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Stepa', 0xf0ff)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'signed val given as unsigned={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', -200000)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'too big signed masked ={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', 0xf0ff)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'signed val given as unsigned={val}')
    interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].set_attribute_value_by_description('Y Step', 0xff0ff)
    val = interface_t.get_c2s_cmd_by_id(10).payload_descriptions[0].get_attribute_value_by_description('Y Step')
    print(f'too big signed val given as unsigned maske={val}')
    print(interface_t.get_c2s_cmd_by_id(10))

    print('apply known profile changes for interfaces')
    for dev in devices.get_devices():
        print(f'update dev {dev.device_id}:{dev.device_name}')
        dev_update_profile_changes(dev)

    # check pprint
    print(devices)

    _, device_id, unit_id, interface_id, attribute_id = ['', '12', '1', '517', '1']
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

    print('\nget and fire Server to Client Command')
    print('-------------------------')
    scmd = interface.get_s2c_cmd_by_ref(1)
    scmd2 = interface.get_s2c_cmd_by_ref(scmd.cmd_name)
    eval(scmd2.cmd_action)
   