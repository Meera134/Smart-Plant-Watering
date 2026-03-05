import pybullet as p
import pybullet_data
import time
import numpy as np

# -----------------------------
# Start PyBullet
# -----------------------------
physicsClient = p.connect(p.GUI)
p.setAdditionalSearchPath(pybullet_data.getDataPath())
p.setGravity(0,0,-9.8)

plane = p.loadURDF("plane.urdf")

robot = p.loadURDF("kuka_iiwa/model.urdf",[0,0,0],useFixedBase=True)

# end effector index for KUKA
end_effector = 6

# -----------------------------
# Plant positions
# -----------------------------
plant_positions = [
    [0.6,0.4,0.2],
    [0.6,-0.4,0.2],
    [1.0,0.4,0.2],
    [1.0,-0.4,0.2]
]

plants = []

for pos in plant_positions:
    plant = p.loadURDF("cube_small.urdf",pos)
    plants.append(plant)

# -----------------------------
# Soil Moisture Sensors
# -----------------------------
soil_moisture = [50,35,60,47]

threshold = 45

current_target = None

# -----------------------------
# Trajectory Planning
# -----------------------------
def move_robot(target_position):

    joint_angles = p.calculateInverseKinematics(
        robot,
        end_effector,
        target_position
    )

    for i in range(7):
        p.setJointMotorControl2(
            robot,
            i,
            p.POSITION_CONTROL,
            joint_angles[i]
        )

    for _ in range(240):
        p.stepSimulation()
        time.sleep(1/240)


# -----------------------------
# Water Spray
# -----------------------------
def spray_water(position):

    for _ in range(15):

        drop = p.loadURDF(
            "sphere_small.urdf",
            [position[0],position[1],0.4]
        )

        for _ in range(30):
            p.stepSimulation()
            time.sleep(1/240)

        p.removeBody(drop)


# -----------------------------
# Water Plant
# -----------------------------
def water_plant(index):

    global soil_moisture

    print("Pump ON")

    while soil_moisture[index] < threshold:

        spray_water(plant_positions[index])

        soil_moisture[index] += 3

        print("Moisture:",round(soil_moisture[index],2))

    print("Pump OFF")


# -----------------------------
# Optimization Scheduling
# -----------------------------
def choose_priority_plant():

    dry_plants = []

    for i in range(len(soil_moisture)):
        if soil_moisture[i] < threshold:
            dry_plants.append(i)

    if not dry_plants:
        return None

    # choose driest plant
    driest = min(dry_plants, key=lambda x: soil_moisture[x])

    return driest


# -----------------------------
# Continuous Control Loop
# -----------------------------
while True:

    # soil drying simulation
    for i in range(len(soil_moisture)):
        soil_moisture[i] = max(soil_moisture[i] - 0.2,0)

    print("\nCurrent Moisture:",
          [round(m,2) for m in soil_moisture])

    target = choose_priority_plant()

    if target is not None:

        if current_target != target:

            print("Moving to Plant",target+1)

            move_robot(plant_positions[target])

            current_target = target

        water_plant(target)

    else:

        print("All plants healthy, monitoring...")

    time.sleep(1)