import json
import time
import logging
import schedule

from mqtt.snomM900MqttClient import snomM900MqttClient as hassiomqtt

class snomSSSMqttHasssioClient(hassiomqtt):
    def __init__(self, enable=True):
        hassiomqtt.__init__(self)

        self.enable = enable
       

    def publish_SSS_motion_detector_config_ha_disabled(self, ipui, device_id, toggle):
        if self.enable:
            component = 'sensor'
            #object_id = beacon_gateway
            object_id = device_id
            # leave out
            node_id = ipui

            # /config for best proximity
            attribute='P'
            device_name = "homeassistant-%s" % ipui
            # send a new device with multiple sensors
            device_topic = "homeassistant/%s/%s/%s/config" % (component, node_id, "%s_%s" % (object_id, attribute))
            payload = {"name": "homeassistant-%s-%s" % (object_id, attribute),
            "unique_id": "homeassistant-%s-%s" % (object_id, attribute),
            "device_class": "motion",
            "state_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
            "json_attributes_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
            "json_attributes_template": "{{ value_json.data | tojson}}",
            "unit_of_measurement": "proximity",
            "value_template": "{{ value_json.data.md_proximity | int }}",
            "device": {"identifiers": ["%s-%s" % (object_id, attribute)],
                        "name": device_name,
                        "model": "SnomSSS",
                        "manufacturer": "Snom Technology GmbH"}
            }
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_light_bulp_config_ha(self, ipui, device_id):
        ''' Light Bulp
        '''
        if self.enable:
            device_class = 'None'
            if ipui == "LB":
                device_class = 'light'
          
            component = 'sensor'
            object_id = device_id
            # abreviaiton for the used service, we have no ipei at this point.
            node_id = ipui

            # /config for energy
            attribute='S' 
            device_name = "snomSSS-%s" % ipui
            # send a new device with multiple sensors
            device_topic = "homeassistant/%s/%s/%s/config" % (component, node_id, "%s_%s" % (object_id, attribute))
            payload = {
                "name": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "unique_id": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "device_class": "%s" % device_class,
                "state_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
                "command_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
                "brightness": True,
                "color_mode": True,
                "json_attributes_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
                "unit_of_measurement": "schema",
                #"value_template": "{{ value_json.data }}",
            }
            
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_light_bulp_state_ha(self, ipui, device_id, h, s, v):
        if self.enable:
            component = 'sensor'
            object_id = device_id
            # leave out
            node_id = ipui

            # send payload to existing device
            payload = {  
                        "brightness": v,
                        "color_mode": "hs",
                        "color": {
                            "h": h,
                            "s": s
                        },
                        "effect": "colorloop",
                        "state": "ON",
                        "transition": 2,
                        }
            device_topic = "homeassistant/%s/%s/%s/state" % (component, node_id, object_id)
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_light_bulp_set_ha(self, ipui, device_id, h, s, v):
        if self.enable:
            component = 'sensor'
            object_id = device_id
            # leave out
            node_id = ipui

            # send payload to existing device
            payload = {  
                        "brightness": v,
                        "color_mode": "hs",
                        "color": {
                            "h": h,
                            "s": s
                        },
                        "effect": "colorloop",
                        "state": "ON",
                        "transition": 2,
                        }
            device_topic = "homeassistant/%s/%s/%s/state" % (component, node_id, object_id)
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_simple_power_meter_config_ha(self, ipui, device_id, energy):
        ''' 5.6 Simple Power Metering Interface
        '''
        if self.enable:
            device_class = 'None'
            if ipui == "SPMI":
                device_class = 'energy'
          
            component = 'sensor'
            object_id = device_id
            # abreviaiton for the used service, we have no ipei at this point.
            node_id = ipui

            # /config for energy
            attribute='E' 
            device_name = "snomSSS-%s" % ipui
            # send a new device with multiple sensors
            device_topic = "homeassistant/%s/%s/%s/config" % (component, node_id, "%s_%s" % (object_id, attribute))
            payload = {
                "name": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "unique_id": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "device_class": "%s" % device_class,
                "state_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
                "json_attributes_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
                "unit_of_measurement": "W/h",
                "value_template": "{{ value_json.energy }}",
                "payload_available": "online",
                "payload_not_available": "offline",
            }
            
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_simple_power_meter_state_ha(self, ipui, device_id, energy, energy_lreset, time_lreset,
                                        inst_power, average_power, average_power_int,
                                        voltage, current, 
                                        frequency, power_factor, 
                                        report_interval):
        if self.enable:
            component = 'sensor'
            object_id = device_id
            # leave out
            node_id = ipui

            # send payload to existing device
            payload = {  "energy" : energy,
                         "energy_lreset" : energy,
                         "time_lreset" : time_lreset,
                         "inst_power": inst_power,
                         "average_power": average_power,
                         "average_power_int": average_power_int,
                         "voltage": voltage,
                         "current": current,
                         "frequency": frequency,
                         "power_factor": power_factor,
                         "report_interval": report_interval,
                      }
            device_topic = "homeassistant/%s/%s/%s/state" % (component, node_id, object_id)
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_motion_detector_config_ha(self, ipui, device_id, toggle):
        if self.enable:
            device_class = 'None'
            if ipui == "WOCD":
                device_class = 'door'
            if ipui == "MD":
                device_class = 'motion'
            if ipui == "SB":
                device_class = 'motion'

            component = 'binary_sensor'
            object_id = device_id
            # abreviaiton for the used service, we have no ipei at this point.
            node_id = ipui

            # /config for best proximity
            attribute='P'
            device_name = "snomSSS-%s" % ipui
            # send a new device with multiple sensors
            device_topic = "homeassistant/%s/%s/%s/config" % (component, node_id, "%s_%s" % (object_id, attribute))
            payload = {
                "name": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "unique_id": "snomSSS-%s-%s-%s" % (node_id, object_id, attribute),
                "device_class": "%s" % device_class,
                "state_topic": "homeassistant/%s/%s/%s/state" % (component, node_id, object_id),
            }
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))


    def publish_SSS_motion_detector_state_ha_disabled(self, ipui, device_id, proximity):
        if self.enable:
            component = 'sensor'
            object_id = device_id
            # leave out
            node_id = ipui

            # send payload to existing device
            payload = { "data" : {  "ipui" : ipui,
                                    "md_proximity" : int(proximity),
                                 }
                        }
            device_topic = "homeassistant/%s/%s/%s/state" % (component, node_id, object_id)
            # encode object to JSON
            self.publish("%s" % device_topic, json.dumps(payload))

    def publish_SSS_motion_detector_state_ha(self, ipui, device_id, prox_msg):
        if self.enable:
            component = 'binary_sensor'
            object_id = device_id
            # leave out
            node_id = ipui

            # send payload to existing device
            payload = prox_msg
            device_topic = "homeassistant/%s/%s/%s/state" % (component, node_id, object_id)
            # encode object to JSON
            self.publish("%s" % device_topic, payload)

    
    def send_sensor_data(self, ipui, device_id, proximity):
        prox_msg = 'ON' if proximity == 1 else 'OFF'

        # publish config
        self.publish_SSS_motion_detector_config_ha(ipui, device_id, prox_msg)
        # publish state
        self.publish_SSS_motion_detector_state_ha(ipui, device_id, prox_msg)

    def send_simple_button_data(self, ipui, device_id, press_type):
        # publish config
        self.publish_SSS_motion_detector_config_ha(ipui, device_id, press_type)
        # publish state
        self.publish_SSS_motion_detector_state_ha(ipui, device_id, press_type)
    
    def send_simple_power_meter_data(self, ipui, device_id, energy, energy_lreset, time_lreset,
                                        inst_power, average_power, average_power_int,
                                        voltage, current, 
                                        frequency, power_factor, 
                                        report_interval):

        # publish config
        self.publish_SSS_simple_power_meter_config_ha(ipui, device_id, energy)
        # publish state
        self.publish_SSS_simple_power_meter_state_ha(ipui, device_id, energy, energy_lreset, time_lreset,
                                        inst_power, average_power, average_power_int,
                                        voltage, current, 
                                        frequency, power_factor, 
                                        report_interval)

    def send_light_bulb_data(self, ipui, device_id, h, s, v):
        # publish config
        self.publish_SSS_light_bulp_config_ha(ipui, device_id)
        # publish state
        self.publish_SSS_light_bulp_state_ha(ipui, device_id, h, s, v)
   
#--------------------------

def send_SSS():
    mqttc.send_sensor_data('0123456789', '1', 1)


if __name__ == "__main__":

    # prepare logger
    logger = logging.getLogger('snomSSSMqttHasssio')
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    formatter = logging.Formatter('%(asctime)s  %(name)s  %(levelname)s: %(message)s')
    ch.setFormatter(formatter)
    logger.addHandler(ch)

   
    # get a mqtt instance sending data in hass.io form
    mqttc = snomSSSMqttHasssioClient()

    # fire data with scheduler
    logger.debug("main: schedule.every(1).minutes.do(amsg.request_keepalive)")
    schedule.every(2).seconds.do(send_SSS)

    #configuration.yaml
    #mqtt:
    #broker: 192.168.188.201
    #port: 1883
    #discovery: True
    #discovery_prefix: snomSSS


    #rc = mqttc.connect_and_subscribe('10.245.0.28', 1883)
    #rc = mqttc.connect_and_subscribe('10.110.11.63', 1883, 'homeassistant', 'Quowoo3kaeco7jaiNg0Sheexief6aethod7AeKeesh9haegiethu6zoghie2ahC2')
    rc = mqttc.connect_and_subscribe('10.110.11.63', 1883, 'mqtt_user', 'mqtt_user')

    rc = 0
    while rc == 0:
        # check and execute scheduled task
        schedule.run_pending()

        # motivate mqtt scheduler
        rc = mqttc.run()
        time.sleep(2)


    print("rc: "+str(rc))
