# Install script for directory: /home/pi/gr-doa/include/doa

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/doa" TYPE FILE FILES
    "/home/pi/gr-doa/include/doa/api.h"
    "/home/pi/gr-doa/include/doa/autocorrelate.h"
    "/home/pi/gr-doa/include/doa/MUSIC_lin_array.h"
    "/home/pi/gr-doa/include/doa/rootMUSIC_linear_array.h"
    "/home/pi/gr-doa/include/doa/antenna_correction.h"
    "/home/pi/gr-doa/include/doa/find_local_max.h"
    "/home/pi/gr-doa/include/doa/calibrate_lin_array.h"
    "/home/pi/gr-doa/include/doa/autocorrelate_smoothing.h"
    )
endif()

