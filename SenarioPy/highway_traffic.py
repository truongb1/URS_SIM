# Made by: Bao Truong
# Last edit: 4/3/2024
# Comments: working, recommend only official map should have ai traffic, change car ammount if needed
# Purpose: highway driving with AI traffic, mutiple lanes

from beamngpy import BeamNGpy, Scenario, ScenarioObject, Vehicle
from beamngpy.misc.colors import coerce_color
from beamngpy.misc.quat import angle_to_quat
from beamngpy.types import Float3
from beamngpy.api.beamng import TrafficApi
import math
import keyboard
import json

# Instantiate BeamNGpy instance running the simulator from the given path,
# communicating over localhost:64256
bng = BeamNGpy('localhost', 64256, home=r'C:\Users\URS\Desktop\BeamNG.tech.v0.31.3.0')
# Launch BeamNG.tech
bng.open()

# Change Default setting
bng.settings.change('GraphicDisplayResolutions', '3440 1440')  # change this to match monitor, full rez = '3440 1440'
bng.settings.change('GraphicDisplayModes', 'Borderless')
bng.settings.change('defaultGearboxBehavior', 'realistic')
bng.settings.change('spawnVehicleIgnitionLevel', 0)  # start engine off
bng.settings.apply_graphics()

# Create a scenario
scenario = Scenario('west_coast_usa', 'highway traffic', description="Learn driving on highway with traffic, caution AI sometime have random behavior!")

# Create a driver car and set all initial parameters,
set_pcolor = (89/255, 203/255, 232/255) # color value is divided by 255
primary_color = coerce_color(set_pcolor, alpha=0)
vehicle = Vehicle('ego_vehicle', model='bastion', license='URS SIM', color=primary_color) # you can change car here

# Add it to our scenario at this position and rotations
car_angle = tuple((0, 0, -135.678))
car_rot = angle_to_quat(car_angle)
scenario.add_vehicle(vehicle, pos=(-942.868, -608.800, 106.456), rot_quat=car_rot)

# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()

# Car config from .pc file, this give it adaptive power steering
def parse_pc_file(pc_file_path):
    with open(pc_file_path, 'r') as file:
        pc_data = json.load(file)  # Assuming the .pc file is in JSON format
    return pc_data

pc_file_path = r'C:\Users\URS\AppData\Local\BeamNG.drive\0.31\vehicles\bastion\URS.pc'
pc_data = parse_pc_file(pc_file_path)
vehicle.set_part_config(pc_data)

# Create TrafficApi instance
traffic_api = TrafficApi(bng)

# Spawn vehicles using TrafficApi
traffic_api.spawn(max_amount=10, police_ratio=0, extra_amount=5, parked_amount=3)

# reset traffic function
def traffic_reset(event):
    if event.name == 'r':
        print("R key pressed! traffic reset")
        traffic_api.reset()

# quit sim funtion
def sim_quit(event):
    if event.name == 'end':
        print("End key pressed! quitting sim")
        bng.close()

# Register the callback function
keyboard.on_press(sim_quit)

# Keep the script running
keyboard.wait('caps lock')
