cmake_minimum_required(VERSION 2.8)
project(mqtt C)

set (CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DKXVER=3")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DKXVER=3")

set(MY_LIBRARY_NAME mqtt)
file(GLOB SRC_FILES src/mqtt.c) # Do not add 'socket_pair.c' since it gets included
set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE C)

message(STATUS "Generator : ${CMAKE_GENERATOR}")
message(STATUS "Build Tool : ${CMAKE_BUILD_TOOL}")
message(STATUS "Paho MQTT API : $ENV{PAHO_HOME}")

include_directories (
    $ENV{PAHO_HOME}/include 
    ${CMAKE_BINARY_DIR} # For 'k.h', downloaded below
)

find_library(MQTT3A_LIBRARY
    NAMES paho-mqtt3a
    HINTS "$ENV{PAHO_HOME}/lib/"
)
find_library(MQTT3AS_LIBRARY
    NAMES paho-mqtt3as
    HINTS "$ENV{PAHO_HOME}/lib/"
)
find_library(MQTT3C_LIBRARY
    NAMES paho-mqtt3c
    HINTS "$ENV{PAHO_HOME}/lib/"
)
find_library(MQTT3CS_LIBRARY
    NAMES paho-mqtt3cs
    HINTS "$ENV{PAHO_HOME}/lib/"
)

file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/c/c/k.h" "${CMAKE_BINARY_DIR}/k.h" )

if (MSVC)
    file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/w64/q.lib" "${CMAKE_BINARY_DIR}/q.lib" )
    set(LINK_LIBS "${CMAKE_BINARY_DIR}/q.lib")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS") 
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS")
else()
    set(LINK_LIBS "")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-aliasing")
endif()
    
add_library (${MY_LIBRARY_NAME} SHARED ${SRC_FILES})

IF(APPLE)
   set_target_properties(${MY_LIBRARY_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
   set_target_properties(${MY_LIBRARY_NAME} PROPERTIES SUFFIX .so)
endif()

target_link_libraries(${MY_LIBRARY_NAME} ${MQTT3A_LIBRARY} ${MQTT3AS_LIBRARY} ${MQTT3C_LIBRARY} ${MQTT3CS_LIBRARY} ${LINK_LIBS})
set_target_properties(${MY_LIBRARY_NAME} PROPERTIES PREFIX "")

install(FILES README.md install.sh install.bat DESTINATION ${PROJECT_BINARY_DIR}/mqtt/)
install(DIRECTORY examples DESTINATION ${PROJECT_BINARY_DIR}/mqtt)
install(DIRECTORY script DESTINATION ${PROJECT_BINARY_DIR}/mqtt)
install(TARGETS ${MY_LIBRARY_NAME} DESTINATION ${PROJECT_BINARY_DIR}/mqtt/lib OPTIONAL)
