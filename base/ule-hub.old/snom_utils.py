import datetime
import logging

def log(str):
    print('snom_utils:' + " " + str)

def global_message_type(msg_type: int):
    if 0x00 >= msg_type >= 0x11:
        return 'Unknown MSGTYPE'
    elif msg_type == 1:
        return 'Command'
    elif msg_type == 2:
        return 'Command with Response Required'
    elif msg_type == 3:
        return 'Command Response'
    elif msg_type == 4:
        return 'Get Attribute Request'
    elif msg_type == 5:
        return 'Response to a Get Attribute Request'
    elif msg_type == 6:
        return 'Set Attribute Request'
    elif msg_type == 7:
        return 'Set Attribute Request with Response Required'
    elif msg_type == 8:
        return 'Response to a Set Attribute Request'
    elif msg_type == 9:
        return 'Get Attribute Pack Request'
    elif msg_type == 0x0a:
        return 'Response to a Get Attribute Pack Request'
    elif msg_type == 0x0b:
        return 'Set Attribute Pack Request'
    elif msg_type == 0x0c:
        return 'Set Attribute Pack Request with Response Required'
    elif msg_type == 0x0d:
        return 'Response to a Set Attribute Pack Request'
    elif msg_type == 0x0e:
        return 'Atomic Set Attribute Pack Request'
    elif msg_type == 0x0f:
        return 'Atomic Set Attribute Pack Request with Response Required'
    elif msg_type == 0x10:
        return 'Response to an Atomic Set Attribute Pack Request'
    else:
        return f'MSGTYPE unknown?, msg_type={hex(msg_type)}'

def global_response_code(error_code: int) -> str:
    '''
    HF Protocol
    8 HAN-FUNGeneralResponseFormat
    Table 24 - Data in the payload of a Default Response to any command or request.
    Response Code
    Value that indicates the state of the command reception/processing.
    U8
    0x00 - Ok
    0x03 - Fail: Not supported 
    0xFF - Fail: Unknown reason

    0x00
    Ok
    The request/command was correctly received and/or processed.
    0x01
    Fail: Not authorized
    The requesting device needs to authenticate itself or it is simply not authorized to perform that request.
    0x02
    Fail: Invalid argument
    One or more request/command arguments are invalid.
    0x03
    Fail: Not supported
    Some requested feature, command or attribute is not implemented on the destination device. The operation will permanently fail.
    0x04
    Fail: Read Only attribute
    The attribute you are trying to set is a read only attribute. The operation will permanently fail.
    0x20
    Fail: Read session not established
    The operation requires a read session to be correctly established with the destination device.
    0x21
    Fail: Entries table was modified
    The table, over which you were operating, changed. You should re-start the read session to avoid inconsistencies.
    0xFE
    Fail: Not enough resources
    The available resources on the destination device are not sufficient to handle the request/command. Try again.
    0xFF
    Fail: Unknown reason
    An unspecified error has occurred, the operation failed.
    and Interface specific
    '''
    if error_code == 0:
        return 'OK'
    elif error_code == 1:
        return 'Fail: Not authorized'
    elif error_code == 2:
        return 'Fail: Invalid argument'
    elif error_code == 3:
        return 'Fail: Not supported'
    elif error_code == 4:
        return 'Fail: Read Only attribute'
    elif error_code == 5:
        return 'Fail: Read session not established'
    elif error_code == 0x20:
        return 'Fail: Read session not established'
    elif error_code == 0x21:
        return 'Fail: Entries table was modified'
    elif error_code == 0xfe:
        return 'Fail: Not enough resources'
    elif error_code == 255:
        return 'Fail: Unknown reason'
    else:
        return f'OK/NOK?, error_code={hex(error_code)}'

