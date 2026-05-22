################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 3.8)
project(turtlebot3_gazebo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC)
  add_compile_definitions(_USE_MATH_DEFINES)
endif()

################################################################################
# Find ament packages and libraries for ament and system dependencies
################################################################################
find_package(ament_cmake REQUIRED)
find_package(ros_gz_sim REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)

find_package(gz_math_vendor REQUIRED)
find_package(gz-math REQUIRED)
find_package(gz_sim_vendor REQUIRED)
find_package(gz-sim REQUIRED)
find_package(gz_plugin_vendor REQUIRED)
find_package(gz-plugin REQUIRED)

################################################################################
# Build
################################################################################
include_directories(
  include
)

set(EXEC_NAME "turtlebot3_drive")

add_executable(${EXEC_NAME} src/turtlebot3_drive.cpp)
ament_target_dependencies(${EXEC_NAME}
  geometry_msgs
  nav_msgs
  rclcpp
  sensor_msgs
  tf2
)

set(OBSTACLE_LIBS
  obstacles
  obstacle1
  obstacle2
)

foreach(lib ${OBSTACLE_LIBS})
  add_library(${lib} SHARED src/${lib}.cpp)
  target_link_libraries(${lib}
    PUBLIC
    gz-sim::gz-sim
    gz-math::gz-math
  )
endforeach()

# Traffic light system plugin for Gazebo Harmonic / gz-sim
add_library(traffic_light_plugin SHARED
  src/traffic_light_plugin.cpp
)

target_link_libraries(traffic_light_plugin
  PUBLIC
  gz-sim::gz-sim
  gz-plugin::gz-plugin
)

target_include_directories(traffic_light_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_library(traffic_bar_plugin SHARED
  src/traffic_bar_plugin.cpp
)

target_link_libraries(traffic_bar_plugin
  PUBLIC
  gz-sim::gz-sim
  gz-plugin::gz-plugin
)

target_include_directories(traffic_bar_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

################################################################################
# Install
################################################################################
install(TARGETS
  ${EXEC_NAME}
  obstacles
  obstacle1
  obstacle2
  traffic_light_plugin
  traffic_bar_plugin
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch models params rviz urdf worlds
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY include/
  DESTINATION include/
)

################################################################################
# Macro for ament package
################################################################################
ament_export_include_directories(include)
ament_export_dependencies(
  ros_gz_sim
  geometry_msgs
  nav_msgs
  rclcpp
  sensor_msgs
  tf2
  gz_math_vendor
  gz_sim_vendor
  gz_plugin_vendor
)

ament_package()