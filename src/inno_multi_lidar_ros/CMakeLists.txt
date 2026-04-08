cmake_minimum_required(VERSION 3.8)
#cmake_policy(SET CMP0048 NEW)
project(inno_multi_lidar_ros C CXX)
# 明确设置CMP0057策略
if(POLICY CMP0057)
  cmake_policy(SET CMP0069 NEW)
endif()
#=======================================
# Custom Point Type (XYZI, XYZI_TIME,XYZI_SOURCE)
#=======================================
set(POINT_TYPE XYZI_TIME)

#=======================================
# Compile setup (ORIGINAL, CATKIN, COLCON)
#=======================================
set(COMPILE_METHOD CATKIN)
set(CMAKE_C_STANDARD 99)
if(COMPILE_METHOD STREQUAL "COLCON")
option(ENABLE_ROS1 "Enable ROS1" OFF)
option(ENABLE_ROS2 "Enable ROS2" ON)
else()
option(ENABLE_ROS1 "Enable ROS1" ON)
option(ENABLE_ROS2 "Enable ROS2" OFF)
endif()
#========================
set(PROJECT_NAME inno_multi_lidar_ros)

add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")
option(ENABLE_IMU_MSG_PARSE "Enable IMU MSG PARSE" ON)
if(${ENABLE_IMU_MSG_PARSE})
  add_definitions(-DENABLE_IMU_MSG_PARSE)
  message(STATUS "Enable IMU MSG PARSE is on")
endif(${ENABLE_IMU_MSG_PARSE})

set(CMAKE_BUILD_TYPE Release)
add_definitions(-O3)
if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUQUIETILD_TYPE Release)
  add_definitions(-O3)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party/inno_driver)
set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/third_party/inno_driver/lib)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  set(SYS_FRAMEWORK "aarch64")
else()
  set(SYS_FRAMEWORK "x86_64")
endif()

link_directories("${LIB_DIR}/${SYS_FRAMEWORK}")

if($ENV{ROS_DISTRO} STREQUAL "humble")  # the ros2 humble requires c++17
add_definitions(-std=c++17)
else()
add_definitions(-std=c++14)
endif()

add_compile_options(-Wall)

#========================
# Point Type Definition
#========================
if(${POINT_TYPE} STREQUAL "XYZI")
  add_definitions(-DPOINT_TYPE_XYZI)
elseif(${POINT_TYPE} STREQUAL "XYZI_TIME")
  add_definitions(-DPOINT_TYPE_TIME)
else()
  add_definitions(-DPOINT_TYPE_SOURCE)
endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -g")
message(=============================================================)
message("-- POINT_TYPE is ${POINT_TYPE}")
message(=============================================================)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)
list(APPEND EXTERNAL_LIBS pcap)
#========================
# Dependencies Setup
#========================
# 配置选项

#ROS#
if(${ENABLE_ROS1})
find_package(roscpp 1.12 QUIET)
if(roscpp_FOUND)

  message(=============================================================)
  message("-- ROS Found. ROS Support is turned On.")
  message(=============================================================)

  add_definitions(-DROS_FOUND=1)

  find_package(roslib QUIET)
  include_directories(${roscpp_INCLUDE_DIRS} ${roslib_INCLUDE_DIRS})
  set(ROS_LIBS ${roscpp_LIBRARIES} ${roslib_LIBRARIES})

  #Catkin#
  if(${COMPILE_METHOD} STREQUAL "CATKIN")

    add_definitions(-DRUN_IN_ROS_WORKSPACE)

    find_package(catkin REQUIRED COMPONENTS
      roscpp
      sensor_msgs
      roslib)
    
    include_directories(${catkin_INCLUDE_DIRS})

    catkin_package(CATKIN_DEPENDS 
      sensor_msgs 
      roslib)

  endif(${COMPILE_METHOD} STREQUAL "CATKIN")

else(roscpp_FOUND)

  message(=============================================================)
  message("-- ROS Not Found. ROS Support is turned Off.")
  message(=============================================================)

endif(roscpp_FOUND)
endif(${ENABLE_ROS1})
if(${ENABLE_ROS2})
#ROS2#
find_package(rclcpp QUIET)

if(rclcpp_FOUND AND ${COMPILE_METHOD} STREQUAL "COLCON")

  message(=============================================================)
  message("-- ROS2 Found. ROS2 Support is turned On.")
  message(=============================================================)

  add_definitions(-DROS_FOUND=2)
  include_directories(${rclcpp_INCLUDE_DIRS})
  set(CMAKE_CXX_STANDARD 14)

  find_package(ament_cmake REQUIRED)
  find_package(sensor_msgs REQUIRED)
  find_package(std_msgs REQUIRED)                      

else(rclcpp_FOUND AND ${COMPILE_METHOD} STREQUAL "COLCON")

  message(=============================================================)
  message("-- ROS2 Not Found. ROS2 Support is turned Off.")
  message(=============================================================)

endif(rclcpp_FOUND AND ${COMPILE_METHOD} STREQUAL "COLCON")
endif(${ENABLE_ROS2})

#Others#
find_package(yaml-cpp REQUIRED)


#Include directory#
include_directories(${PROJECT_SOURCE_DIR}/src)

#========================
# Build Setup
#========================
add_executable(inno_lidar_node
               node/inno_lidar_node.cpp
               src/manager/node_manager.cpp
               src/source/source_driver.cpp
               src/source/publish_manager.cpp)


target_compile_options(inno_lidar_node PRIVATE
  -O3
#  -march=native    # 生成当前CPU专属指令集
  -flto            # 链接时优化
)
set_target_properties(inno_lidar_node PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE  # 启用LTO
)



target_link_libraries(inno_lidar_node
                      ${YAML_CPP_LIBRARIES}
                      ${inno_driver_LIBRARIES}
                      ${EXTERNAL_LIBS}
                      multi_driver)


#Ros#
if(roscpp_FOUND)

  target_link_libraries(inno_lidar_node 
    ${ROS_LIBS})

  if(${COMPILE_METHOD} STREQUAL "CATKIN")

    install(TARGETS inno_lidar_node
            RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
            LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
            ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

  endif()

endif(roscpp_FOUND)

#Ros2#
if(rclcpp_FOUND AND ${COMPILE_METHOD} STREQUAL "COLCON")

  message("==============================+++++++++++++++++++======")
  ament_target_dependencies(inno_lidar_node 
    rclcpp 
    std_msgs 
    sensor_msgs
    )

  install(TARGETS
    inno_lidar_node
    DESTINATION lib/${PROJECT_NAME})
	
  install(DIRECTORY
    launch
    rviz
    DESTINATION share/${PROJECT_NAME})
    
  install(FILES "${LIB_DIR}/${SYS_FRAMEWORK}/libmulti_driver.so" DESTINATION lib)

  ament_package()

endif(rclcpp_FOUND AND ${COMPILE_METHOD} STREQUAL "COLCON")

#target_link_libraries(inno_lidar_node Boost::filesystem)
