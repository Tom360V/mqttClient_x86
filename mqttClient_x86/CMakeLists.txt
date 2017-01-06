cmake_minimum_required(VERSION 2.8)

project(mqttClient_x86)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wall -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O0 -g3")


enable_testing()

set(externals
  externals/mqtt_c
  externals/ServiceDispatcher
  externals/ServiceRGB
  externals/socket
  externals/fifo
)


#Add include directories
include_directories(./)
include_directories(client/inc)

foreach(ext ${externals})
    message("*** " ${ext}/inc)
    include_directories(${ext}/inc)
endforeach(ext)

#Add related projects
foreach(ext ${externals})
    add_subdirectory(${ext})
endforeach(ext)


#Add sources
set(srcs main.c
         x86.c
    )

#Add EXEcutable
add_executable(mqttClient_x86 ${srcs})
target_link_libraries(mqttClient_x86 mqtt_c ServiceDispatcher client fifo ServiceRGB)

#add_test(TEST_mqttClient_x86 test/test_ServiceDispatcherRouter)
