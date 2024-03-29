# vi:si:et:sw=4:sts=4:ts=4
# -*- coding: UTF-8 -*-
# -*- Mode: Python -*-
import io, socket

def get_local_ip():
    local_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        local_socket.connect(('10.255.255.255', 1))
        local_ip = local_socket.getsockname()[0]
    except Exception:
        local_ip = '127.0.0.1'
    finally:
        local_socket.close()
    return local_ip

# disbale actions entirely. 
ACTIONS = True
# actions only for TAGs
KNX_ACTION = True


#PHONE_IP = '192.168.178.20'
PHONE_IP = '10.110.16.88'

#XML_SERVER_IP = '192.168.178.25'
#XML_SERVER_IP = '10.110.16.101'

# phone keys will not work on local host! 
XML_SERVER_IP = get_local_ip()

# knx ip
KNX_SERVER_IP = '192.168.178.99'
#KNX_SERVER_IP = '10.110.16.53'

# ULE server IP
ULE_SERVER_IP = XML_SERVER_IP 
#### testing
#ULE_SERVER_IP = '192.168.178.25'


DECT_MESSAGING_VIEWER_IP_AND_PORT = '127.0.0.1:8081'
DECT_MESSAGING_VIEWER_URL = f'http://{DECT_MESSAGING_VIEWER_IP_AND_PORT}/en_US'

LED_OFFSET = 37 # snomD735
OLD_TAG_STATE = ['holding', 'holding', 'holding', 'holding', 'holding', 
                    'holding', 'holding', 'holding', 'holding', 'holding',
                    'holding', 'holding', 'holding', 'holding', 'holding',
                    'holding', 'holding', 'holding', 'holding', 'holding',
                    ]
TAG_NAME_DICT = { "000413BA0029" : "Kaffeemuehle",
                    "000413BA0059" : "Bild",
                    "000413BA00E4" : "Laptop",
                    "000413BA0021" : "Defi",
                    "000413BA001F" : "Oma", 
                }
WAVE_URL = f'http://{XML_SERVER_IP}/IO/test1.wav'

HTTP_D7DIR = 'D7C_XML'
HTTP_ROOT = f'/var/www/html/{HTTP_D7DIR}'   

KNX_GATEWAY_URL = f'http://{KNX_SERVER_IP}:1234'
# its now symcon snom key action url / web hooks 
KNX_GATEWAY_URL = f'http://192.168.178.26:3777'

GATEWAY_URL = f'http://{KNX_SERVER_IP}:8000'
ULE_GATEWAY_URL = f'http://{ULE_SERVER_IP}:8881'
