# Made by: Bao Truong
# Last edit: 3/22/2024
# Comments: working
# Purpose: OHIO mav test and parrellel parking senario

from beamngpy import BeamNGpy, Scenario, ScenarioObject, Vehicle
from beamngpy.misc.colors import coerce_color
from beamngpy.misc.quat import angle_to_quat
from beamngpy.types import Float3
from beamngpy.api.beamng import UiApi
import math

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

# Create a scenario in west_coast_usa called 'example'
scenario = Scenario('LosInjurus', 'OHIO and Parallel Parking', description="Learn parallel parking and 9x20 cone test ")

# Create a driver car and set all initial parameters,
set_pcolor = (89/255, 203/255, 232/255) # color value is divided by 255
primary_color = coerce_color(set_pcolor, alpha=0)
vehicle = Vehicle('ego_vehicle', model='bastion', license='URS SIM', color=primary_color) # you can change car here

# Add it to our scenario at this position and rotations
scenario.add_vehicle(vehicle, pos=(6346.014, 1509.513, 337.037), rot_quat=(0, 0, -180, 1))

# ----- OHIO Maneuverability Test = 9 ft WIDE x 40/2 ft LONG, with tube object -----
# Create the first tube, back left
tube1 = ScenarioObject(oid='roadblock',
                      name='test',
                      otype='BeamNGVehicle',
                      pos=(6335.958, 1531.306, 337.037),
                      rot_quat=(0, 0, 0, 1),
                      scale=(1, 1, 1.2),
                      JBeam='tube',
                      datablock='default_vehicle')
scenario.add_object(tube1)

# Create the first tube, back right
tube2 = ScenarioObject(oid='roadblock2',
                       name='sawhorse',
                       otype='BeamNGVehicle',
                       pos=(6338.896, 1531.306, 337.037),
                       rot_quat=(0, 0, 0, 1),
                       scale=(1, 1, 1.2),
                       JBeam='tube',
                       datablock='default_vehicle')
scenario.add_object(tube2)

# Create the third tube, middle left
tube3 = ScenarioObject(oid='roadblock3',
                       name='test',
                       otype='BeamNGVehicle',
                       pos=(6335.958, 1537.267, 337.037),
                       rot_quat=(0, 0, 0, 1),
                       scale=(1, 1, 1.2),
                       JBeam='tube',
                       datablock='default_vehicle')
scenario.add_object(tube3)

# Create the fourth tube, middle right
tube4 = ScenarioObject(oid='roadblock4',
                       name='test',
                       otype='BeamNGVehicle',
                       pos=(6338.896, 1537.267, 337.037),
                       rot_quat=(0, 0, 0, 1),
                       scale=(1, 1, 1.2),
                       JBeam='tube',
                       datablock='default_vehicle')
scenario.add_object(tube4)

# Create the fifth tube, front
tube5 = ScenarioObject(oid='roadblock5',
                       name='test',
                       otype='BeamNGVehicle',
                       pos=(6337.427, 1543.228, 337.037),
                       rot_quat=(0, 0, 0, 1),
                       scale=(1, 1, 1.2),
                       JBeam='tube',
                       datablock='default_vehicle')
scenario.add_object(tube5)

# ----- Parallel parking test -----
set_ocolor = (255/255, 95/255, 31/255) # neon orange
safe_color = coerce_color(set_ocolor, alpha=0)

# Rotation example
left_degree = tuple((0, 0, 90))
right_degree = tuple((0, 0, -90))
rot_left = angle_to_quat(left_degree)
rot_right = angle_to_quat(right_degree)

vehicle = Vehicle('p_vehicle1', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6349.361, 1560.991, 337.209), rot_quat=rot_right, cling=True)

vehicle = Vehicle('p_vehicle2', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6361.536, 1560.991, 337.209), rot_quat=rot_right, cling=True)

vehicle = Vehicle('p_vehicle3', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6378.722, 1561.5, 337.209), rot_quat=rot_left , cling=True)

vehicle = Vehicle('p_vehicle4', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6390.734, 1561.5, 337.209), rot_quat=rot_left , cling=True)
# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()

# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()

# Commenting out the block starting from TrafficApi creation
'''
# Create TrafficApi instance
traffic_api = TrafficApi(bng)

# Spawn vehicles using TrafficApi
traffic_api.spawn(max_amount=10, police_ratio=0, extra_amount=5, parked_amount=2)

# Export OpenStreetMap (.osm).
OpenStreetMapExporter.export(r'C:\\Users\\URS\\Desktop\\OSM\\test2_osm', bng)
print("Current Vehicle Configuration:")
print(vehicle.get_part_config())

print("\nPossible Part Options:")
print(vehicle.get_part_options())

# Make the vehicle's AI span the map
# vehicle.ai.set_mode('span')
# input('Hit enter when done...')
input('press \'Enter\' to exit demo')
'''