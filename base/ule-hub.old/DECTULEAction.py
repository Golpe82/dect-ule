# vi:si:et:sw=4:sts=4:ts=4
# -*- coding: UTF-8 -*-
# -*- Mode: Python -*-

import sys
import requests
import time
import logging
import threaded

# scenes are device_id based. 
hf_scenes = []

logger = logging.getLogger("DECTULEAction")
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s  %(name)s  %(levelname)s: %(message)s")
ch.setFormatter(formatter)
logger.addHandler(ch)

from DECTMessagingConfig import *
 
HTTP_ULE_ROOT = f'http://{XML_SERVER_IP}:8881'


from dataclasses import dataclass, field
from typing import List

# f'{HTTP_ULE_ROOT}/snom_ule_cmd/snom_set_cmd_attribute_value%209%201%20514%201%201%20%22Hue%200-359%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%22%200',
@dataclass
class HFAction:
    action_url : str = ''
    actions_duration : float = 0     

#{"1": [HFAction]
@dataclass
class HFStateAction:
    state : str = 'else'
    actions : List[HFAction] = field(default_factory=lambda: [])
    next_action : HFAction = None

@dataclass
class HFScene:
    name: str = 'unnamed'
    device_id : int = -1
    state_actions : List[HFStateAction] = field(default_factory=lambda: [])

    def get_actions_by_state(self, state: str) -> List[HFAction]:
        if len(state) > 0:
            match = next((action for action in self.state_actions if action and action.state == state), None)
            if match:
                return match
            else:
                print(f'cannot get HFAction with state={state}')
        else:
            print(f'no HFAction(s) specified for empty state') 


def send_action_to_web_server(url):
    # save xml to file
    # send to phone
    request = url
    logger.debug("send to phone: {}".format(request))
    try:
        _response = requests.get(request, timeout=5)
        logger.debug(_response)        
    except:
        logger.exception("Error send_action_to_web_server:")
        pass
    
actions1 = [HFAction(f'{KNX_GATEWAY_URL}/5/1/18-an',
                     actions_duration = 0.0),
]
actions2 = [HFAction(f'{KNX_GATEWAY_URL}/5/1/18-aus',
                     actions_duration = 0.0),
]
actions3 = [HFAction(f'{HTTP_ULE_ROOT}/snom_ule_cmd/snom_set_cmd_attribute_value%209%201%20514%201%201%20%22Hue%200-359%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%22%20240',
                        actions_duration = 5.0),
            ]
            
state_actions = [HFStateAction("1", actions1),
                HFStateAction("0", actions2),
                HFStateAction("else", actions3),
                ]                  
scene = HFScene(name='KNXOpenCloseSend', device_id=3, state_actions=state_actions)
# add to global scenes
hf_scenes.append(scene)

def snom_handle_window_open_close(device_id, proximity):
    global hf_scenes
    if device_id == 3:
        # thread start here
        thread = snom_handle_window_open_close_t(proximity, hf_scenes[0]) 
        thread.start()
        return True
    if device_id == 10:
        # thread start here
        thread = snom_handle_window_open_close_t(proximity, hf_scenes[0]) 
        thread.start()
        return True
    return False

@threaded.Threaded
def snom_handle_window_open_close_t(state: str, scene: HFScene) -> bool:
    global hf_scenes
    logger.debug("snom_handle_window_open_close: run scene {}, state={}".format(scene.name, state))

    try:
        action = 'not specified'
        # action_list = device_action_dict[device_id]
        state_actions = scene.get_actions_by_state(str(state))
        #action_list_for_state = action_list[str(state)]
        for action in state_actions.actions:
            logger.debug("snom_handle_window_open_close action={}".format(action))
            try:
                send_action_to_web_server(action.action_url)
                time.sleep(action.actions_duration)
            except:
                logger.debug(f'action for {str(state)} not found, only have {state_actions.actions}')
            
        return True
    except:
        logger.debug("snom_handle_window_open_close failed: {}->{}".format(state, scene))
        return False

    # shoot mqtt message
    #mqttc.send_sensor_data('WOCD', device_id, int(proximity))    

##
# while moving 20%-80%
actions1 = [HFAction(f'{KNX_GATEWAY_URL}/2/1/70-an',
                        actions_duration = 0.0),
            # turn window background on 
            HFAction(f'{KNX_GATEWAY_URL}/3/1/10-an',
                        actions_duration = 0.0),
            ]
actionsclosed = [HFAction(f'{KNX_GATEWAY_URL}/2/1/70-aus',
                        actions_duration = 0.0),
            HFAction(f'{KNX_GATEWAY_URL}/2/1/60-an',
                        actions_duration = 0.0),
            HFAction(f'{KNX_GATEWAY_URL}/3/1/10-aus',
                        actions_duration = 0.0),
            ]
# turn off window backlight 
actionsopened = [HFAction(f'{KNX_GATEWAY_URL}/2/1/70-aus',
                        actions_duration = 0.0),
            HFAction(f'{KNX_GATEWAY_URL}/2/1/60-an',
                        actions_duration = 0.0),
            HFAction(f'{KNX_GATEWAY_URL}/3/1/10-an',
                        actions_duration = 0.0),
            ]
            
state_actions = [HFStateAction("255", actionsclosed),
                HFStateAction("204", actions1),
                HFStateAction("51", actions1),
                HFStateAction("0", actionsopened),
                ]            
      
scene = HFScene(name='KNXLEDEffectBlind', device_id=13, state_actions=state_actions)
# add to global scenes
hf_scenes.append(scene)

# action handler for device 13 / Level Control
def action_on_report_level_changed(device_id: int, level:int) -> bool:
    global hf_scenes
    if device_id == 13:
        # thread start here
        thread = action_on_report_level_changed_t(level, hf_scenes[1])
        thread.start()
    return True

@threaded.Threaded
def action_on_report_level_changed_t(level:int, scene: HFScene) -> bool:
    logger.debug("action_on_report_level_changed: run scene={}, level={}".format(scene.name, level))
    #if device_id == 13:
    try:
        action = 'not specified'
        # action_list = device_action_dict[device_id]
        if abs(level - 51) < 10:
            state_actions = scene.get_actions_by_state(str(51))
        elif abs(level - 204) < 10:
            state_actions = scene.get_actions_by_state(str(204))
        else:
            state_actions = scene.get_actions_by_state(str(level))
        
        #action_list_for_state = action_list[str(state)]
        for action in state_actions.actions:
            logger.debug("action_on_report_level_changed level={} -> action={}".format(level, action))
            try:
                send_action_to_web_server(action.action_url)
                time.sleep(action.actions_duration)
            except:
                logger.debug(f'action for {str(level)} not found, only have {state_actions.actions}')
        return True
    except:
        logger.debug("action_on_report_level_changed failed: {}->{}".format(level, scene))
        return False


# 2 simple button used as airquality and temperature triggers
# close blind
actionsclosed = [ 
                  HFAction(f'{HTTP_ULE_ROOT}/snom_ule_cmd/snom_send_cmd%2012%201%20516%201%202%2022',
                        actions_duration = 0.0),
                ]
# open blind 50%
actionsopen50 = [HFAction(f'{HTTP_ULE_ROOT}/snom_ule_cmd/snom_set_cmd_attribute_value%2012%201%20513%201%201%20%22Level%22%20127',
                        actions_duration = 10.0),
                HFAction(f'{HTTP_ULE_ROOT}/snom_ule_cmd/snom_send_cmd%2012%201%20516%201%202%2022',
                        actions_duration = 0.0),
                ]
# airquality 1 = bad -> open window
# airquality 0 = good -> close window
# same for high temperature
### or open for 30s...
state_actions = [HFStateAction("air0", actionsclosed),
                 HFStateAction("air1", actionsopen50),
                 HFStateAction("temp0", actionsclosed),
                 HFStateAction("temp1", actionsopen50),
                ]            
      
scene = HFScene(name='AirqualityTempTrigger', device_id=14, state_actions=state_actions)
# add to global scenes
hf_scenes.append(scene)

def snom_handle_airtemp(device_id, unit_id, cmd_id: int, cmd_name: str):
    global hf_scenes

    if device_id == 14:
        logger.debug('Owango sends button {}={} pressed'.format(cmd_id, cmd_name))
        # air quality to bad
        if unit_id == 1 and cmd_id == 1: 
            button = 'air1'
            # thread start here
            thread = snom_handle_airtemp_t(button, hf_scenes[2]) 
            thread.start()
            return True
        # temperature too high
        if unit_id == 2 and cmd_id == 1:
            button = "temp1"
             # thread start here
            thread = snom_handle_airtemp_t(button, hf_scenes[2]) 
            thread.start()
            return True
    else:
        logger.debug('button {}={} pressed'.format(cmd_id, cmd_name))

@threaded.Threaded
def snom_handle_airtemp_t(button: int, scene: HFScene):
    logger.debug("snom_handle_airtemp_t: run scene={}, button={}".format(scene.name, button))
    try:
        action = 'not specified'
        # action_list = device_action_dict[device_id]
        state_actions = scene.get_actions_by_state(button)

        for action in state_actions.actions:
            logger.debug("snom_handle_airtemp_t button={} -> action={}".format(button, action))
            try:
                send_action_to_web_server(action.action_url)
                time.sleep(action.actions_duration)
            except:
                logger.debug(f'action for {str(button)} not found, only have {state_actions.actions}')
        return True
    except:
        logger.debug("snom_handle_airtemp_t failed: {}->{}".format(button, scene))
        return False


    
if __name__ == "__main__":
    sys.stdout = sys.__stdout__

    snom_handle_window_open_close(3, 1)
    snom_handle_window_open_close(3, 0)
    # wait until threads are done
    time.sleep(20.0)
    print('done')
    print(HTTP_ULE_ROOT)
    