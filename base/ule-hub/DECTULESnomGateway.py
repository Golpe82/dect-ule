# vi:si:et:sw=4:sts=4:ts=4
# -*- coding: UTF-8 -*-
# -*- Mode: Python -*-
#from gevent import monkey

#monkey.patch_all()
import gevent

import sys

import logging
import requests
import paho.mqtt.client as mqtt

import bottle
from bottle import app, template, request, url, FormsDict
from bottle import Jinja2Template
from beaker.middleware import SessionMiddleware


from bottle_utils.i18n import I18NPlugin
from bottle_utils.i18n import lazy_gettext as _


### DECT ULE app
from snom_HF_app import *
# ??? remove
from DECTULEMiniBrowser import *

template.settings = {
    "autoescape": True,
}

template.defaults = {
    "url": url,
    "site_name": "SnomLocationViewer",
}

LANGS = [
    ("de_DE", "Deutsch"),
    ("en_US", "English")
    # ('fr_FR', 'français'),
    # ('es_ES', 'español')
]

DEFAULT_LOCALE = "en_US"
LOCALES_DIR = "./locales"


session_opts = {
    "session.type": "file",
    "session.cookie_expires": 300,
    "session.data_dir": "/tmp/data",
    "session.auto": True,
    "session.encrypt_key": "invoipwetrust",
    "session.validate_key": "invoipwetrust",
}


bottle.debug(False)
# used for templates with multiple urls to download images etc.

bottle.TEMPLATE_PATH = ("./views", "./templates")

css_root = "/css/"
css_root_path = ".%s" % css_root
images_root = "/images/"
images_root_path = ".%s" % images_root
save_root = "/uploads/"
save_root_path = ".%s" % save_root

tapp = bottle.default_app()
wsgi_app = I18NPlugin(
    tapp,
    langs=LANGS,
    default_locale=DEFAULT_LOCALE,
    locale_dir=LOCALES_DIR,
    domain="base",
)

app = SessionMiddleware(wsgi_app, session_opts)

logger = logging.getLogger("DECTULESnomGateway")
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s  %(name)s  %(levelname)s: %(message)s")
ch.setFormatter(formatter)
logger.addHandler(ch)

## helper
class PrettyFormsDict(FormsDict):
    def __repr__(self):
        # Return a string that could be eval-ed to create this instance.
        args = ", ".join("{}={!r}".format(k, v) for (k, v) in sorted(self.items()))
        return "{}({})".format(self.__class__.__name__, args)

    def __str__(self):
        # Return a string that is a pretty representation of this instance.
        args = " ,\n".join(
            "\t{!r}: {!r}".format(k, v) for (k, v) in sorted(self.items())
        )
        return "{{\n{}\n}}".format(args)
## end helper


# receives full list of DEVICES in json format DEVICES
@bottle.route("/json_action", name="json_action", no_i18n=True, method=["GET", "POST"])
def run_json_action():

    # run action to device
    request_url = "http://10.245.0.136:12380/cm?cmnd=Power%20TOGGLE"
    r_json = None
    snom_xml = '<?xml version="1.0" encoding="UTF-8"?><SnomIPPhoneText><Title>Error</Title><Text>Cannot access sensor</Text></SnomIPPhoneText>'
    beacon_action = True
    if beacon_action:
        try:
            s = requests.Session()
            s.mount("http://", requests.adapters.HTTPAdapter(max_retries=3))

            r = s.get(request_url, timeout=1.0)
            r_json = r.json()
            logger.debug("fire_action: %s, response=%s", request_url, r)
        except:
            logger.debug("fire_action: cannot connect %s", request_url)
    else:
        logger.debug("fire_action: Dx disabled: %s", request_url)
    # we got a response
    if r_json:
        print(r_json)
        json_dict = r_json
        val1 = json_dict["POWER"]
        # if val1=='OFF':
        #    val1 = 'Bad airquality'
        #    qual_icon='virus_red.png'
        if val1 == "OFF":
            val1 = "Excellent airquality"
            qual_icon = "leaf-24px.png"
        if val1 == "ON":
            val1 = "Medium airquality"
            qual_icon = "virus_yellow.jpg"

        #   kIconTypeFkeyStats -- alternative Sensor Icon.
        snom_xml = f"""
        <?xml version="1.0" encoding="UTF-8"?>
<InfoBoxQueue>
 <InfoBox>
  <Line pos="1">
   <Icon>kIconTypeFkeyDispCode</Icon>
   <Text>Corona alert, airquality control </Text>
  </Line>
  <Line pos="2">
   <Icon>http://10.245.0.28/sensor/{qual_icon}</Icon>
   <Text>{val1}</Text>
  </Line>
 </InfoBox>
</InfoBoxQueue>
"""

    return snom_xml

# receives full list of DEVICES in json format DEVICES
@bottle.route("/sss", name="snom sensor service", no_i18n=True, method=["GET", "POST"])
def run_sss():
    return render_sss()
    
import shlex

#########
# returns html / jinja template with data 
#########

@bottle.route("/htmlULE", name='return_HTML_Devs', method=['GET'], no_i18n = True)
def return_HTML_Devs():
    try: 
        answer_tuple = return_ULE(hf_devices)
        return bottle.jinja2_template('uledevicesview', title=_("DECT ULE Devices"), data=answer_tuple)

    except:
        logger.exception('Kaputt')
        return('return_HTML_Devs failed')

@bottle.route("/htmlULE/<dev_id>", name='return_HTML_Dev', method=['GET'], no_i18n = True)
def return_HTML_Dev(dev_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        answer_tuple = return_device(dev)
        return bottle.jinja2_template('uledeviceview', title=_("DECT ULE Device Interfaces"), data=answer_tuple)

    except:
        return('return_HTML_Dev failed')

@bottle.route("/htmlULE/<dev_id>/<unit_id>/<interface_id>/status", name='return_HTML_CMDs', method=['GET'], no_i18n = True)
def return_HTLM_interface_status(dev_id, unit_id, interface_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))

        interface_t = unit.get_interface_by_id(int(interface_id))

        # iterate over number of server attributes 
        interface_t.get_attribute_by_id(1)
        for sa_id,sa in enumerate(interface_t.server_attributes):
            # get server attributes from interface
            # e.g. get attribute 1 -> snom_attrib_info 5 2 512 1  
            url = f'{ULE_URL}/snom_ule_cmd_nr/snom_attrib_info'
            url += f' {dev_id} {unit_id}'
            url += f' {interface_id}'
            url += f' {sa_id}'
            try:
                s = requests.Session()
                s.mount("http://", requests.adapters.HTTPAdapter(max_retries=3))

                r = s.get(url, timeout=5.0)
                logger.debug("snom_attrib_info: %s, response=%s", url, r)
                s.close()
            except:
                logger.debug("snom_attrib_info: cannot connect %s", url)
                s.close()
            # sometimes we already have new data
            print(f'Attribute {sa.attribute_id}: {sa.attribute_name} {sa.attribute_descriptions}={sa.attribute_values}')

        # gather latest data and view 
        # wait a bit longer for the data to arrive from DCX81
        gevent.sleep(0.25)
        answer_tuple = return_server_attribute_values(int(dev_id), int(unit_id), interface_t, unit.profile.profile_name)
        return bottle.jinja2_template('ulecinterfacestatusview', title=_(f"Interface {interface_id} Status"), data=answer_tuple)
    except Exception as e:
        return(f'return_HTLM_interface_status failed: {e}')

@bottle.route("/htmlULE/<dev_id>/<unit_id>/<interface_id>", name='return_HTML_CMDs', method=['GET'], no_i18n = True)
def return_HTML_CMDs(dev_id, unit_id, interface_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))

        interface_t = unit.get_interface_by_id(int(interface_id))
        answer_tuple = return_cmds(int(dev_id), int(unit_id), interface_t, unit.profile.profile_name)
        return bottle.jinja2_template('ulecmdsview', title=_(f"Interface {interface_id} Commands"), data=answer_tuple)

    except:
        return('return_HTML_CMDs failed')

@bottle.route("/htmlULE/<dev_id>/<unit_id>/<interface_id>/<cmd_id>", name='return_HTML_CMD', method=['GET','POST'], no_i18n = True)
def return_HTML_CMD(dev_id, unit_id, interface_id, cmd_id):
    if request.method == 'POST':
        vals = request.forms.dict
        ###
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))
        
        interface_t = unit.get_interface_by_id(int(interface_id))
        command = interface_t.get_c2s_cmd_by_id(int(cmd_id))

        payload_descrs = descrs = interface_t.get_c2s_cmd_by_id(int(cmd_id)).payload_descriptions[0]

        descrs = interface_t.get_c2s_cmd_by_id(int(cmd_id)).payload_descriptions[0].attribute_descriptions
      
        for i in range(0,len(vals)):
            value = request.forms.get(f'val{i}')
            print(descrs[i])
            if type(descrs[i]) is list:
                # take only the first entry - use the first option element 
                tmp_descrs = descrs[i][0]
            else:
                tmp_descrs = descrs[i]
            # a single element 
            label = tmp_descrs[0]
            label = label.strip()
            payload_descrs.set_attribute_value_by_description(label, int(value))
            

        # combine all values and send as a cmd with payload 
        url = f'{ULE_URL}/snom_ule_cmd_nr/snom_send_cmd'
        url += f' {dev_id} {unit_id}'
        url += f' {interface_id} 1 {cmd_id}'
        # values 
        value_string = " ".join(str(x) for x in payload_descrs.attribute_values)
        url += f' {value_string}'
        try:
            s = requests.Session()
            s.mount("http://", requests.adapters.HTTPAdapter(max_retries=3))

            r = s.get(url, timeout=5.0)
            logger.debug("snom_set_cmd_attribute_value: %s, response=%s", url, r)
            s.close()
        except:
            logger.debug("snom_set_cmd_attribute_value: cannot connect %s", url)
            s.close()

        # get the changed values in case DECT ULE answered already
        answer_tuple = return_cmd_options_payload(int(dev_id), interface_t.intrf_name, int(interface_t.intrf_id), int(unit_id),
                                          command)
        return bottle.jinja2_template('ulecmdpayloadview', title=_(f"Set command value"), data=answer_tuple, url=url)
    else: # GET 
        # cmd with values
        try: 
            dev = hf_devices.get_device_by_id(int(dev_id))
            unit = dev.get_unit_by_id(int(unit_id))
            
            interface_t = unit.get_interface_by_id(int(interface_id))
            command = interface_t.get_c2s_cmd_by_id(int(cmd_id))
            answer_tuple = return_cmd_options_payload(int(dev_id), interface_t.intrf_name, int(interface_t.intrf_id), int(unit_id),
                                          command)         
            return bottle.jinja2_template('ulecmdpayloadview', title=_(f"Set command value"), data=answer_tuple)

        except:
            '''answer_tuple = ({'DeviceID': 2, 'InterfaceName': 'Colour Control Interface', 'InterfaceId': 514, 'UnitID': 1, 'CName': 'MoveToHueAndSaturation', 'BackUrl': 'http://192.168.188.185:8881/htmlULE/2/1/514'}, [[(0, 'Hue 0-359', 32, 48, False, 0, 999)], [(0, 'Saturation', 24, 32, False, 255, 999)], [(0, 'Direction', 16, 24, False, 3, 999)], [(0, 'Direction Up = 0x01', 16, 18, False, 3, 1), (1, 'Direction Down = 0x02', 16, 18, False, 3, 2), (2, 'Direction Shortest Distance = 0x03', 16, 18, False, 3, 3), (3, 'Direction Longest Distance = 0x04', 16, 18, False, 3, 4)], [(0, 'Transition Time 100ms', 0, 16, False, 1, 999)]])

            return bottle.jinja2_template('ulecmdpayloadview', title=_(f"Set command value"), data=answer_tuple)
            '''
            logger.exception('return_HTML_CMD failed, devices etc. None')
            return('return_HTML_CMD failed')

 
#########
# returns minibrowser 
#########

# minibrowser of next page in the menu structure will be returned 
@bottle.route("/miniULE", name='return_XML_Devs', method=['GET'], no_i18n = True)
def return_XML_Devs():
    try: 
        answer = return_minibrowser_ULE(hf_devices)
        return answer
    except:
        return('return_XML_Devs failed')

@bottle.route("/miniULE/<dev_id>", name='return_XML_Dev', method=['GET'], no_i18n = True)
def return_XML_Dev(dev_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        answer = return_minibrowser_device(dev)
        return answer
    except:
        return('return_XML_Dev failed')

@bottle.route("/miniULE/<dev_id>/<unit_id>/<interface_id>", name='return_XML_CMDs', method=['GET'], no_i18n = True)
def return_XML_Mini_CMDs(dev_id, unit_id, interface_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))

        interface_t = unit.get_interface_by_id(int(interface_id))
        answer = return_minibrowser_cmds(int(dev_id), int(unit_id), interface_t)
        return answer
    except:
        return('return_XML_Mini_CMDs failed')

@bottle.route("/miniULE/<dev_id>/<unit_id>/<interface_id>/<cmd_id>", name='return_XML_Mini_CMD', method=['GET'], no_i18n = True)
def return_XML_Mini_CMD(dev_id, unit_id, interface_id, cmd_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))
        
        interface_t = unit.get_interface_by_id(int(interface_id))
        command = interface_t.get_c2s_cmd_by_id(int(cmd_id))
        answer = return_minibrowser_cmd_payload(int(dev_id), int(interface_id), int(unit_id), command)
        return answer
    except:
        return('return_XML_Mini_CMD failed')

@bottle.route("/miniULE/<dev_id>/<unit_id>/<interface_id>/<cmd_id>/<idx>", name='return_XML_Mini_Input', method=['GET'], no_i18n = True)
def return_XML_Mini_Input(dev_id, unit_id, interface_id, cmd_id, idx):
    try:
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))
        
        interface_t = unit.get_interface_by_id(int(interface_id))
        command = interface_t.get_c2s_cmd_by_id(int(cmd_id))
        payload = command.payload_descriptions[0].attribute_descriptions[int(idx)]
        label = payload[0]
        #def return_minibrowser_cmd_payload_change(dev_id: int, i_id: int, u_id: int, c: HFC2SCommand, label: str, idx: int) -> str:
        answer = return_minibrowser_cmd_payload_change(int(dev_id), int(interface_id), int(unit_id), command, label, idx)
        return answer
    except:
        return('return_XML_Mini_Input failed')

#####
# direct ule command 
#####
@bottle.route("/snom_ule_cmd/<cmd>", name='snom_ule_cmd', method=['GET'], no_i18n = True)
def run_snom_ule_cmd(cmd):
    global client_handle
    try:
        #argv = cmd.split(" ") # does not preserve 10 11 "peter der grosse" 1
        argv = shlex.split(cmd)
        #print(argv)
        
        back_url = request.url
        back_url = shlex.split(request.url_args['cmd'])

        # send any other ULE command via cmbs 
        answer = snom_send_generic_cmd(argv)
         # url to go back to is:['snom_set_cmd_attribute_value', '9', '1', '513', '1', '1', 'Level', '100']
        if len(back_url) >= 6 and back_url[0] in 'snom_set_cmd_attribute_value' or back_url[0] in 'snom_send_cmd':
            back_url=back_url[1:4]
            
            dev_id = int(back_url[0])
            unit_id = int(back_url[1])
            interface_id = int(back_url[2])
            dev = hf_devices.get_device_by_id(int(dev_id))
            unit = dev.get_unit_by_id(int(unit_id))
            interface_t = unit.get_interface_by_id(int(interface_id))
      
            answer = return_minibrowser_cmds(dev_id, unit_id, interface_t)
        return answer
    except: 
        logger.log('DECT_ULE: cannot send %s', cmd)
        return f'DECT_ULE: cannot send {cmd}'

# page will not return minibrowser next page
@bottle.route("/snom_ule_cmd_nr/<cmd>", name='snom_ule_cmd', method=['GET'], no_i18n = True)
def run_snom_ule_cmd(cmd):
    global client_handle
    try:
        #argv = cmd.split(" ") # does not preserve 10 11 "peter der grosse" 1
        argv = shlex.split(cmd)
        
        # send any other ULE command via cmbs 
        answer = snom_send_generic_cmd(argv)
        #return answer
    except: 
        logger.log('DECT_ULE: cannot send %s', cmd)
        return f'DECT_ULE: cannot send {cmd}'
    
# page will return HAN/FUN client info 
@bottle.route("/snom_ule_cmd_hf/<cmd>", name='snom_ule_cmd', method=['GET'], no_i18n = True)
def run_snom_ule_cmd(cmd):
    global client_handle
    try:
        #argv = cmd.split(" ") # does not preserve 10 11 "peter der grosse" 1
        argv = shlex.split(cmd)
        
        # send any other ULE command via cmbs 
        answer = snom_send_generic_cmd(argv)
        return answer
    except: 
        logger.log('DECT_ULE: cannot send %s', cmd)
        return f'DECT_ULE: cannot send {cmd}'
    
@bottle.route("/", name="main", method="GET")
def run_main():
    #request.session["test"] = request.session.get("test", 0) + 1
    #request.session.save()
    #logger.debug("Session: %d", request.session["test"])

    #request.session["profile_firstname"] = "NA"
    #request.session["profile_lastname"] = "NA"

    return "nothing here."


def on_connect(client, userdata, flags, rc):
    print('MQTT connected', rc)

def on_subscribe(client, userdata, mid, granted_qos):
    print('topic subscribed', mid)

def on_hanfun_message(client, obj, msg):
    #print(client)
    print('HAN-FUN Message:', msg.topic+" "+
        str(msg.qos)+" "+
        msg.payload.decode("utf-8") )
    payload_str = msg.payload.decode("utf-8") 


def on_log(client, userdata, level, buff):  # mqtt logs function
    print(buff)
    
def subsribe_mqtt(client):
    client.on_message = on_hanfun_message
    client.on_connect = on_connect
    client.on_subscribe = on_subscribe
    client.on_log = on_log


    #mqttc.message_callback_add('homeassistant/binary_sensor/MD/1/state', on_hanfun_message)
    #mqttc.message_callback_add('homeassistant/binary_sensor/MD/1/state', on_message)

import asyncio
from functools import wraps
def background(f):
    try:
        @wraps(f)
        def wrapped(*args, **kwargs):
            loop = asyncio.get_event_loop()
            if callable(f):
                logger.debug("function %s started in background.", str(f))
                return loop.run_in_executor(None, f, *args, **kwargs)
            else:
                raise TypeError('Task must be a callable')   
        return wrapped
    except:
        logger.exception('background task %s failed!', str(f))

@background
def run_ULE():
    # DECT ULE as its own thread - unblock main
    main_bottle()

if __name__ == "__main__":
    # run mqtt client
    #mqttc = mqtt.Client()
    # on_xx before connect
    #subsribe_mqtt(mqttc)

    #mqttc.loop_start()
    #mqttc.username_pw_set('mqtt_user', 'mqtt_user')
    #rc = mqttc.connect('10.245.0.28', 1883)
    #if rc != 0:
    #    print('MQTT could not be started')
    #    sys.exit(0)
    
    run_ULE()

    # wait for CONNACK
    gevent.sleep(5.0)
    #mqttc.subscribe('homeassistant/#')
    

    # run web server
    HOST = "0.0.0.0"
    
    bottle.run(app=app, server='waitress', threads=8, host=HOST, port=8881, reloader=False, debug=True, quiet=True)

    while True:
        gevent.sleep(0.1)
