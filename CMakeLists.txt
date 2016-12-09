project(mqttClient_x86)

cmake_minimum_required(VERSION 2.8)

include_directories(mqttClient_x86/external/fifo/inc)
include_directories(mqttClient_x86/external/socket/inc)
include_directories(mqttClient_x86/external/mqtt_c/inc)
include_directories(mqttClient_x86/external/ServiceRGB/inc)
include_directories(mqttClient_x86/external/ServiceDispatcher/inc)
include_directories(mqttClient_x86/client/inc)

add_subdirectory(mqttClient_x86/external/fifo)
add_subdirectory(mqttClient_x86/external/socket)
add_subdirectory(mqttClient_x86/external/mqtt_c)
add_subdirectory(mqttClient_x86/external/ServiceRGB)
add_subdirectory(mqttClient_x86/external/ServiceDispatcher)
add_subdirectory(mqttClient_x86/client)
add_subdirectory(mqttClient_x86/src)