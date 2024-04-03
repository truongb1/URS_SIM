# Made by: Bao Truong
# Last edit: x/x/x
# Comments: Reference = https://beamngpy.readthedocs.io/en/latest/ | Example = https://github.com/BeamNG/BeamNGpy/blob/master/examples/README.md
# Purpose: do all test here + code example, save in different file

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
scenario = Scenario('LosInjurus', 'TEST')

# Create a driver car and set all initial parameters
set_pcolor = (89/255, 203/255, 232/255) # color value is divided by 255
primary_color = coerce_color(set_pcolor, alpha=0)
vehicle = Vehicle('ego_vehicle', model='bastion', license='URS SIM', color=primary_color)

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

# ----- Various parking test -----
# right next to each other x diff = 2.5
# 1 empty space between x diff = 5.0
# estimate parking formula = x position - [2.5*(empty space + 1)]

back_degree = tuple((0, 0, 180))
rot_back = angle_to_quat(back_degree)
no_degree = tuple((0, 0, 0))
rot_no = angle_to_quat(no_degree)
# bottom list
vehicle = Vehicle('p_vehicle5', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6388.580, 1524.019, 337.257), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_vehicle6', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6383.58, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_vehicle7', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6376.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_vehicle8', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6371.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

vehicle = Vehicle('p_vehicle9', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6361.08, 1524.019, 337.288), rot_quat=rot_back, cling=True)

# top list
vehicle = Vehicle('p_vehicle10', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6385.5, 1528.5019, 337.257), cling=True)

vehicle = Vehicle('p_vehicle11', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6380.5, 1528.5019, 337.257), cling=True)

vehicle = Vehicle('p_vehicle12', model='etk800', color=safe_color, license='URS')
scenario.add_vehicle(vehicle, pos=(6378, 1528.5019, 337.257), cling=True)

# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()

#bng.show_hud()
#Make the vehicle's AI span the map
#vehicle.ai.set_mode('span')
#input('Hit enter when done...')
input('press \'Enter\' to exit demo')

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