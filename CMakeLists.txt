cmake_minimum_required (VERSION 2.6)
project (AC_DataConcentrator)

# The Version Number
set (AC_DataConcentrator_VERSION_MAJOR 1)
set (AC_DataConcentrator_VERSION_MINOR 0)

# configure a header file to pass some of the Cmake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/ac_dataconcentratorconfig.h.in"
  "${PROJECT_BINARY_DIR}/ac_dataconcentratorconfig.h"
)

# add the binary tree to the search path for include files
# so that we will find ElementaEngineConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(AC_DataConcentrator
src/tinyxml2/tinyxml2.cpp
src/main.cpp
src/framework.cpp
src/input/sensorinput.cpp
src/output/networkoutput.cpp
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AC_DataConcentrator)

source_group("input" FILES
src/input/sensorinput.cpp
)

source_group("output" FILES
src/output/networkoutput.cpp
)

source_group("tinyxml2" FILES
src/tinyxml2/tinyxml2.cpp
)