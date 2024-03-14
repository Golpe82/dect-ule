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

import redis

### DECT ULE app
from snom_HF_app import *
# ??? remove
from DECTULEMiniBrowser import *

#POOL = redis.ConnectionPool(host='127.0.0.1', port=6379, db=0)
POOL = None

def getVariable(variable_name):
    my_server = redis.Redis(connection_pool=POOL)
    response = my_server.get(variable_name)
    return response

def setVariable(variable_name, variable_value):
    my_server = redis.Redis(connection_pool=POOL)
    my_server.set(variable_name, variable_value)

def getKeys(variable_pattern):
    my_server = redis.Redis(connection_pool=POOL)
    keylist = []
    for key in my_server.scan_iter(variable_pattern):
           keylist.append(key)
    return keylist

def deleteKeys(variable_pattern):
    # deleteKeys('homeassistant/binary_sensor/MD/1/*')
    my_server = redis.Redis(connection_pool=POOL)
    keylist = []
    for key in my_server.scan_iter(variable_pattern):
           my_server.delete(key)
    return True

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


# define redis shared variables across threads from gunicorn
setVariable("WINDOWOPEN", "off")

TEMPERATURE = 0.0
IAQACC = 0
IAQ = 0
HUMIDITY = 0


@bottle.route("/window_open", method=["GET"], no_i18n=True)
def run_window_open():
    open_window()
    return 'Trying to open window...'


@bottle.route("/window_close", method=["GET"], no_i18n=True)
def run_window_close():
    close_window()
    return 'Trying to close window...'


@bottle.route("/window_off", method=["GET"], no_i18n=True)
def run_window_off():
    window_all_off()
    return 'Trying to close window...'


@bottle.route("/state", method=["GET"], no_i18n=True)
def run_state():
    global TEMPERATURE
    global IAQACC
    global IAQ
    global HUMIDITY
    
    return f'TEMPERATURE:{TEMPERATURE} IAQACC:{IAQACC} IAQ:{IAQ} HUMIDITY:{HUMIDITY} WINDOWOPEN:{getVariable("WINDOWOPEN").decode()}'


@bottle.route("/airquality", method=["GET", "POST"], no_i18n=True)
def run_airquality():
    global TEMPERATURE
    global IAQACC
    global IAQ
    global HUMIDITY

    if request.method == "POST":
        d = request.json
        logger.info("dict:%s", d)
        try:
            TEMPERATURE = float(d["TEMP"])
        except:
            pass
        try:
            IAQACC = int(d["IAQ-ACC"])
        except:
            pass
        try:
            IAQ = int(d["IAQ"])
        except:
            pass
        try:
            HUMIDITY = int(d["HUM"])
        except:
            pass
        try:
            setVariable("WINDOWOPEN", d["window"])
        except:
            pass
        return d

    else:
        logger.warning("GET request of the page, do nothing")
        return "GET request of the page, do nothing"


# receives full list of DEVICES in json format DEVICES
@bottle.route("/snomair", name="Snom Air", no_i18n=True, method=["GET"])
def run_snomair():
    # data has been collected in
    global TEMPERATURE
    global IAQ
    global IAQACC
    global HUMIDITY
    global IAQ

    open = False
    switch = False

    # get the state for this worker
    last_state = getVariable("last_state").decode()
    last_IAQ = int(getVariable("last_IAQ").decode())

    if IAQ < 100:
        qual_icon = "leaf-24px.png"
        iaq_text = "- good"
    if IAQ < 50:
        qual_icon = "leaf-24px.png"
        iaq_text = "- excellent"
    if IAQ >= 100:
        qual_icon = "virus_yellow.png"
        iaq_text = "- pause and leave the room"
        open = True
    if IAQ > 150:
        qual_icon = "virus_red.png"
        iaq_text = "- open windows shortly"
        open = True
    if IAQ > 200:
        qual_icon = "virus_red.png"
        iaq_text = "- open windows and leave"
        open = True
    if IAQ > 300:
        qual_icon = "virus_red.png"
        iaq_text = "- ! RUN !"
        open = True
    if IAQACC != 3:
        iaq_acc_text = "- calibrate"
        iaq_text = f'{iaq_text}{iaq_acc_text}'


    # we need a switching tolerance to avoid toggling
    if abs(IAQ - last_IAQ) >= 10:
        if last_state == "open":
            # do not close, tolerance not reached
            logger.info("last state=open, wish=%s", open)
        else:
            if last_state == "close":
                # respect the IAQ open threshhold
                logger.info("respect the current IAQ open threshhold=%s", open)

        # ok to switch, take next threshold
        last_IAQ = IAQ
        switch = True
    else:
        switch = False
        logger.info("tolerance not reached")

    
    logger.info("Final State: %s %s %s %s", IAQ, abs(IAQ - last_IAQ), open, last_state)
    logger.info("Window Open Sensor: %s", getVariable("WINDOWOPEN"))

    # check if we should open or close window.
    if switch and open and last_state == "close":
        open_window()
        last_state = "open"
        #logger.info("run_snomair: window opened")
    else:
        if switch and not open and last_state == "open":
            # we can close
            close_window()
            last_state = "close"
            #logger.info("run_snomair: window closed")

    setVariable("last_state", last_state)
    setVariable("last_IAQ", last_IAQ)

    # we got a response

    snom_xml = f"""<?xml version="1.0" encoding="UTF-8"?>
<InfoBoxQueue loop="true">
 <InfoBox duration="1">
  <Line pos="1">
   <Icon>kIconTypeFkeyDispCode</Icon>
   <Text>Corona alert, airquality control </Text>
  </Line>
   <Line pos="2">
   <Icon>http://10.110.16.63/sensor/{qual_icon}</Icon>
   <Text>{IAQ} / 300+ {iaq_text}</Text>
  </Line>
  </InfoBox>
  <InfoBox duration="1">
  <Line pos="1">
   <Icon>kIconTypeFkeyDispCode</Icon>
   <Text>Corona alert, airquality control, {TEMPERATURE:.1f} C</Text>
  </Line>
   </InfoBox>
  <InfoBox duration="1">
  <Line pos="1">
   <Icon>kIconTypeFkeyDispCode</Icon>
   <Text>Corona alert, airquality control, {HUMIDITY}% Humidity</Text>
  </Line>
 </InfoBox>
</InfoBoxQueue>
"""

    print(snom_xml)
    return snom_xml


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

@bottle.route("/htmlULE/<dev_id>/<unit_id>/<interface_id>", name='return_HTML_CMDs', method=['GET'], no_i18n = True)
def return_HTML_CMDs(dev_id, unit_id, interface_id):
    try: 
        dev = hf_devices.get_device_by_id(int(dev_id))
        unit = dev.get_unit_by_id(int(unit_id))

        interface_t = unit.get_interface_by_id(int(interface_id))
        answer_tuple = return_cmds(int(dev_id), int(unit_id), interface_t, unit.profile.profile_name)
        return bottle.jinja2_template('ulecmdsview', title=_(f"Interface {interface_id} Commands"), data=answer_tuple)

    except:
        return('return_HTML_Mini_CMDs failed')

@bottle.route("/htmlULE/<dev_id>/<unit_id>/<interface_id>/<cmd_id>", name='return_HTML_Mini_CMD', method=['GET','POST'], no_i18n = True)
def return_HTML_Mini_CMD(dev_id, unit_id, interface_id, cmd_id):
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
            label = descrs[i][0]
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

        answer_tuple = return_cmd_payload(int(dev_id), interface_t.intrf_name, int(interface_t.intrf_id), int(unit_id),
                                          command)
        return bottle.jinja2_template('ulecmdpayloadview', title=_(f"Set command value"), data=answer_tuple)
    else: # GET 
        # cmd with values
        try: 
            dev = hf_devices.get_device_by_id(int(dev_id))
            unit = dev.get_unit_by_id(int(unit_id))
            
            interface_t = unit.get_interface_by_id(int(interface_id))
            command = interface_t.get_c2s_cmd_by_id(int(cmd_id))
            answer_tuple = return_cmd_payload(int(dev_id), interface_t.intrf_name, int(interface_t.intrf_id), int(unit_id),
                                          command)
            return bottle.jinja2_template('ulecmdpayloadview', title=_(f"Set command value"), data=answer_tuple)

        except:
            logger.exception('Kaputt')
            return('return_HTML_Mini_CMD failed')

 
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

def open_window():
    logger.debug("ow: LOCK: %s", getVariable("LOCK").decode())

    if getVariable("LOCK").decode() != "locked":
        setVariable("LOCK", "locked")
        logger.debug("ow: window opening")

        if getVariable("WINDOWOPEN").decode() != "on":
            if actors is not None:
                # make sure close is powerless
                actors.set_expert_pc("2", "0")

                actors.set_expert_pc("1", "1")
                gevent.sleep(6.0)
                actors.set_expert_pc("1", "0")
        else:
            # to make sure we turn all off
            window_all_off()
            logger.debug("ow: WINDOWOPEN sensor was %s, we didnt do anything", getVariable("WINDOWOPEN").decode())

        setVariable("LOCK", "unlocked")
        logger.debug("ow window now opened LOCK: %s", getVariable("LOCK").decode())
    else:
        logger.debug("ow: another worker is running: %s", getVariable("LOCK").decode())
     

def close_window():
    logger.debug("cw: LOCK: %s", getVariable("LOCK").decode())

    if getVariable("LOCK").decode() != "locked":
        setVariable("LOCK", "locked")
        logger.debug("oc: window closing")

        if getVariable("WINDOWOPEN").decode() != "off":
            if actors is not None:
                # make sure open is powerless
                actors.set_expert_pc("1", "0")

                actors.set_expert_pc("2", "1")
                gevent.sleep(6.0)
                actors.set_expert_pc("2", "0")
        else:
            # to make sure we turn all off
            window_all_off()
            logger.debug("cw: WINDOWOPEN sensor was %s, we didnt do anything", getVariable("WINDOWOPEN").decode())

        setVariable("LOCK", "unlocked")
        logger.debug("cw window now closed LOCK: %s", getVariable("LOCK").decode())
    else:
        logger.debug("cw: another worker is running: %s", getVariable("LOCK").decode())

def window_all_off():
    if actors is not None:
        actors.set_expert_pc("1", "0")
        actors.set_expert_pc("2", "0")

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
    setVariable(msg.topic, payload_str)

'''
<SnomIPPhoneMenu>
    <Menu name="name attr menu">
        <Title>2nd layer title tag</Title>
        <MenuItem name="2nd,1st menuitem"/>
        <MenuItem name="2nd,2nd menuitem"/>
    </Menu>
    <MenuItem name="name attr menuitem"/>
    <MenuItem name="name tag menuitem"/>
</SnomIPPhoneMenu>
'''
def render_sss():
    keys = getKeys('homeassistant/binary_sensor/*')
    snom_xml = f'''<?xml version="1.0" encoding="UTF-8"?>
    <SnomIPPhoneMenu>
        <Title>Snom Sensor Service</Title>
        '''
    for key in keys:
        key = key.decode()
        h_, s_, stype, sname, c_ = str(key).split('/')
        if c_ == 'state':
            key_val = getVariable(key).decode()
            entry = f'''<Menu name="{str(stype)}:{str(sname)}={str(key_val)}">
            </Menu>
            '''
            snom_xml = snom_xml + entry
    snom_xml = snom_xml + '</SnomIPPhoneMenu>'
    return snom_xml

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
    
    setVariable("last_state", "close")
    #open = False
    setVariable("last_IAQ", 0)
    setVariable("LOCK", "unlocked")

    # run web server
    HOST = "0.0.0.0"
    
    bottle.run(app=app, server='waitress', threads=8, host=HOST, port=8881, reloader=False, debug=True, quiet=True)

    while True:
        gevent.sleep(0.1)
