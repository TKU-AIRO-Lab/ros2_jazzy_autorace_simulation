# TurtleBot3 Autorace Simulation for ROS 2 Jazzy

This repository provides a modified **TurtleBot3 Autorace simulation** for **ROS 2 Jazzy** and **Gazebo Harmonic**.

It is based on the ROBOTIS TurtleBot3 simulation packages and has been adapted to run the Autorace-style track with a camera-enabled TurtleBot3 Burger model, ROS-Gazebo bridge topics, and simple animated traffic objects.

## Features

- TurtleBot3 Burger simulation for ROS 2 Jazzy
- Gazebo Harmonic / Gazebo Sim world for Autorace practice
- Autorace-style course model
- Camera-enabled TurtleBot3 Burger model
- ROS 2 bridge topics for camera, lidar, odometry, IMU, TF, joint states, and velocity command
- Traffic sign / traffic light / parking / tunnel models
- Custom Gazebo plugins for traffic light and traffic bar animation
- RViz configuration and map files included

## Dependencies

Clone turtlebot3 packages in the source directory:

```bash
git clone -b jazzy https://github.com/ROBOTIS-GIT/DynamixelSDK.git
git clone -b jazzy https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
git clone -b jazzy https://github.com/ROBOTIS-GIT/turtlebot3.git
```

clone this package in the the same directory:

```bash
git clone https://github.com/TKU-AIRO-Lab/ros2_jazzy_autorace_simulation.git
```

## Run the Autorace Simulation

Set the TurtleBot3 model:

```bash
export TURTLEBOT3_MODEL=burger
```

Launch the simulation:

```bash
ros2 launch turtlebot3_gazebo turtlebot3_autorace.launch.py
```

## Main ROS 2 Topics

| Topic | Type | Description |
|---|---|---|
| `/camera/image_raw` | `sensor_msgs/msg/Image` | Camera image from the simulated robot |
| `/camera/camera_info` | `sensor_msgs/msg/CameraInfo` | Camera intrinsic information |
| `/scan` | `sensor_msgs/msg/LaserScan` | 2D lidar scan |
| `/odom` | `nav_msgs/msg/Odometry` | Robot odometry |
| `/imu` | `sensor_msgs/msg/Imu` | IMU data |
| `/tf` | `tf2_msgs/msg/TFMessage` | Transform data |
| `/joint_states` | `sensor_msgs/msg/JointState` | Wheel joint states |
| `/cmd_vel` | `geometry_msgs/msg/TwistStamped` | Velocity command topic |
| `/clock` | `rosgraph_msgs/msg/Clock` | Simulation clock |


## View the Camera Image

Use `rqt_image_view`:

```bash
ros2 run rqt_image_view rqt_image_view
```

Then select:

```text
/camera/image_raw
```
