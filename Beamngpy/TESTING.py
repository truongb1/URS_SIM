from beamngpy import BeamNGpy, Scenario, Vehicle, ScenarioObject

# Instantiate BeamNGpy instance running the simulator from the given path,
# communicating over localhost:64256
bng = BeamNGpy('localhost', 64256, home=r'D:\BeamNG.tech.v0.30.6.0', user=r'C:\Users\bobab\AppData\Local\BeamNG.tech\0.30')
# Launch BeamNG.tech
bng.open()

# Create a scenario in west_coast_usa called 'example'
scenario = Scenario('LosInjurus', 'example')
# Create an ETK800 with the licence plate 'PYTHON'
vehicle = Vehicle('ego_vehicle', model='bastion', license='PYTHON',color='0.3490 0.7960 0.9098') #RGB/255
# Add it to our scenario at this position and rotations
scenario.add_vehicle(vehicle, pos=(6346.014, 1509.513, 337.037), rot_quat=(0, 0, -180, 1))

# OHIO Maneuverability Test = 9 ft WIDE x 40/2 ft LONG, with tube object
# Create the first tube, back left
tube = ScenarioObject(oid='roadblock2',
                      name='test',
                      otype='BeamNGVehicle',
                      pos=(6335.958, 1531.306, 337.037),
                      rot_quat=(0, 0, 0, 1),
                      scale=(1, 1, 1.2),
                      JBeam='tube',
                      datablock='default_vehicle')
scenario.add_object(tube)

# Create the first tube, back right
tube1 = ScenarioObject(oid='roadblock',
                       name='sawhorse',
                       otype='BeamNGVehicle',
                       pos=(6338.896, 1531.306, 337.037),
                       rot_quat=(0, 0, 0, 1),
                       scale=(1, 1, 1.2),
                       JBeam='tube',
                       datablock='default_vehicle')
scenario.add_object(tube1)

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

# Place files defining our scenario for the simulator to read
scenario.make(bng)

# Load and start our scenario
bng.scenario.load(scenario)
bng.scenario.start()

'''
# Loop for user input
while True:
    user_input = input('Press Enter to close the simulator...')
    if user_input == '':
        bng.close()
        break
'''