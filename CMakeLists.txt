##%% General Settings %%##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv#

# CMake Version
cmake_minimum_required(VERSION 3.1)

# Name of package
project(mqttkdb C)

# Name of shared object
set(MY_LIBRARY_NAME mqttkdb)
set(MY_SSL_LIBRARY_NAME mqttkdb_ssl)

# Dispay settings
message(STATUS "Generator : ${CMAKE_GENERATOR}")
message(STATUS "Build Tool : ${CMAKE_BUILD_TOOL}")
message(STATUS "MQTT API : $ENV{MQTT_INSTALL_DIR}")

# Error if MQTT_INSTALL_DIR is not set
if(NOT DEFINED ENV{MQTT_INSTALL_DIR})
	message(FATAL_ERROR "MQTT_INSTALL_DIR must be set to the location of your installed C api repository")
endif()

##%% Compilation %%##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv#

# Set default build type Release
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set target shared object
add_library(${MY_LIBRARY_NAME} SHARED
    src/mqtt.c
    src/socketpair.c
)
add_library(${MY_SSL_LIBRARY_NAME} SHARED
    src/mqtt.c
    src/socketpair.c
)

# Specify include directories
target_include_directories (${MY_LIBRARY_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    $ENV{MQTT_INSTALL_DIR}/include
)
target_include_directories (${MY_SSL_LIBRARY_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    $ENV{MQTT_INSTALL_DIR}/include
)

# Basic compile flag
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DKXVER=3")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DKXVER=3")

# Download k.h
file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/c/c/k.h" "${PROJECT_SOURCE_DIR}/include/k.h")

# Find dependencies
find_library(MQTT3C_LIBRARY
    NAMES paho-mqtt3c
    HINTS "$ENV{MQTT_INSTALL_DIR}/lib/"
)
find_library(MQTT3CS_LIBRARY
    NAMES paho-mqtt3cs
    HINTS "$ENV{MQTT_INSTALL_DIR}/lib/"
)

if (MSVC)
    # Get q dependency
    file(DOWNLOAD "https://github.com/KxSystems/kdb/raw/master/w64/q.lib" "${CMAKE_BINARY_DIR}/q.lib" )
    set(Q_LIBRARY "${CMAKE_BINARY_DIR}/q.lib")
    # Additional cmpile flags
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS") 
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /D WIN32_LEAN_AND_MEAN /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS")
else()
    # Additional cmpile flags
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-aliasing")
endif()
    
# Shared library prefix and suffix
# ex.) mqttkdb.so for linux
# Suffix must be `.so` for Mac
if(APPLE)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()
set_target_properties(${MY_LIBRARY_NAME} PROPERTIES PREFIX "")
set_target_properties(${MY_LIBRARY_NAME} PROPERTIES SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(${MY_SSL_LIBRARY_NAME} PROPERTIES PREFIX "")
set_target_properties(${MY_SSL_LIBRARY_NAME} PROPERTIES SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
target_compile_definitions(${MY_SSL_LIBRARY_NAME} PRIVATE -DMQTT_SSL)

# Link flag
IF(APPLE)
   set_target_properties(${MY_LIBRARY_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
   set_target_properties(${MY_SSL_LIBRARY_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()

# Link dependent libraries
target_link_libraries(${MY_LIBRARY_NAME} ${MQTT3C_LIBRARY} ${Q_LIBRARY})
target_link_libraries(${MY_SSL_LIBRARY_NAME} ${MQTT3CS_LIBRARY} ${Q_LIBRARY})

##%% Installation %%##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv#

# Set install script name to be packaged
if(WIN32)
    # Windows
    set(INSTALL_SCRIPT "install.bat")
    set(OSFLAG w)
else()
    set(INSTALL_SCRIPT "install.sh")
    if(APPLE)
        # MacOSX
        set(OSFLAG m)
    else()
        # Linux
        set(OSFLAG l)
    endif()
endif()

# Check if 32-bit/64-bit machine
set(BITNESS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BITNESS 64)
endif()

# Always build package
file(COPY README.md LICENSE ${INSTALL_SCRIPT} DESTINATION ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/)
file(COPY examples/ DESTINATION ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/examples/)
file(COPY q/ DESTINATION ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/q/)

# Copy built shared object into package directory after build instead of during installation
add_custom_command(TARGET ${MY_LIBRARY_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${MY_LIBRARY_NAME}>" ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/lib/${MY_LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
    DEPENDS ${MY_LIBRARY_NAME}
)
add_custom_command(TARGET ${MY_SSL_LIBRARY_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${MY_SSL_LIBRARY_NAME}>" ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}/lib/${MY_SSL_LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
    DEPENDS ${MY_SSL_LIBRARY_NAME}
)



# Install at Release
install(TARGETS ${MY_LIBRARY_NAME} DESTINATION "$ENV{QHOME}/${OSFLAG}${BITNESS}/" CONFIGURATIONS Release)
install(TARGETS ${MY_SSL_LIBRARY_NAME} DESTINATION "$ENV{QHOME}/${OSFLAG}${BITNESS}/" CONFIGURATIONS Release)
install(DIRECTORY "q/" DESTINATION "$ENV{QHOME}" CONFIGURATIONS Release)
