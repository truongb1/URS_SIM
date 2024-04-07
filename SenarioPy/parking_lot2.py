# Made by: Bao Truong
# Last edit: 4/3/2024
# Comments: working, but long load time bc a lot of car
# Purpose: left or right pull in parking

from beamngpy import BeamNGpy, Scenario, ScenarioObject, Vehicle
from beamngpy.misc.colors import coerce_color
from beamngpy.misc.quat import angle_to_quat
from beamngpy.types import Float3
from beamngpy.api.beamng import UiApi
import math
import keyboard

# Instantiate BeamNGpy instance running the simulator from the given path,
# communicating over localhost:64256
bng = BeamNGpy('localhost', 64256, home=r'C:\Users\URS\Desktop\BeamNG.tech.v0.31.3.0',user=r'C:\Users\URS\AppData\Local\BeamNG.tech')
# Launch BeamNG.tech
bng.open()

# Change Default setting
bng.settings.change('GraphicDisplayResolutions', '3440 1440')  # change this to match monitor, full rez = '3440 1440'
bng.settings.change('GraphicDisplayModes', 'Borderless')
bng.settings.change('defaultGearboxBehavior', 'realistic')
bng.settings.change('spawnVehicleIgnitionLevel', 0)  # start engine off
bng.settings.apply_graphics()

# Create a scenario
scenario = Scenario('LosInjurus', 'Various Parking',description="Learn pull in parking with different spaces between cars, choose left or right turn")

# Create a driver car and set all initial parameters,
set_pcolor = (89/255, 203/255, 232/255) # color value is divided by 255
primary_color = coerce_color(set_pcolor, alpha=0)
vehicle = Vehicle('ego_vehicle', model='bastion', license='URS SIM', color=primary_color) # you can change car here

# Add it to our scenario at this position and rotations
scenario.add_vehicle(vehicle, pos=(6346.014, 1509.513, 337.037), rot_quat=(0, 0, -180, 1))


# ----- Various parking test -----
# right next to each other x diff = 2.5
# 1 empty space between x diff = 5.0
# estimate parking formula = x position - [2.5*(empty space + 1)]

set_ocolor = (255/255, 95/255, 31/255) # neon orange
safe_color = coerce_color(set_ocolor, alpha=0)

back_degree = tuple((0, 0, 180))
rot_back = angle_to_quat(back_degree)
no_degree = tuple((0, 0, 0))
rot_no = angle_to_quat(no_degree)

# Top list
vehicle = Vehicle('pv_1', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6388.580, 1524.019, 337.257), rot_quat=rot_back, cling=True)

vehicle = Vehicle('pv_2', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6383.58, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('pv_3', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6376.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('pv_4', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6371.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('pv_5', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6361.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('pv_6', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6385.5, 1528.5019, 337.257), cling=True)

vehicle = Vehicle('pv_7', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6380.5, 1528.5019, 337.257), cling=True)

vehicle = Vehicle('pv_8', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6378, 1528.5019, 337.257), cling=True)

# Bottom list
vehicle = Vehicle('pv_9', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6387.936, 1508.800, 337.149), rot_quat=rot_no, cling=True)

vehicle = Vehicle('pv_10', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6382.936, 1508.800, 337.288), rot_quat=rot_no, cling=True)

vehicle = Vehicle('p_v11', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6375.436, 1508.800, 337.288), rot_quat=rot_no, cling=True)

vehicle = Vehicle('p_v12', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6370.436, 1508.800, 337.288), rot_quat=rot_no, cling=True)

vehicle = Vehicle('p_v13', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6360.436, 1508.800, 337.288), rot_quat=rot_no, cling=True)

vehicle = Vehicle('p_v14', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6386.097, 1504.042, 337.152), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_v15', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6381.097, 1504.042, 337.257), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_v16', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6378.597, 1504.042, 337.257), rot_quat=rot_back, cling=True)

# quit sim funtion
def sim_quit(event):
    if event.name == 'end':
        print("End key pressed! quitting sim")
        bng.close()

# Register the callback function
keyboard.on_press(sim_quit)

# Keep the script running
keyboard.wait('caps lock')

# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()


