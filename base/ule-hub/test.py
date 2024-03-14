p = ({'DeviceID': 2, 'InterfaceName': 'Colour Control Interface', 'InterfaceId': 514, 'UnitID': 1, 'CName': 'MoveToHue', 'BackUrl': 'http://192.168.188.185:8881/htmlULE/2/1/514'}, [[(0, 'Hue 0-359', 24, 40, False, 0)], [(0, 'Direction', 16, 24, False, 3)], [(0, 'Direction Up = 01x11', 16, 19, False, 3), (1, 'Direction Down = 0x02', 16, 19, False, 3), (2, 'Direction Shortest Distance = 0x03', 16, 19, False, 3), (3, 'Direction Longest Distance = 0x04', 16, 19, False, 3)], [(0, 'Transition Time 100ms', 0, 16, False, 1)]])
ol = (p[1][2])
olt_label = ol[0][1]
print(olt_label)



val = ([x for x in olt_label.split() if x.startswith('0x')])

value = int(val[0],0)
print(value)
