# Install script for directory: /home/pi/gr-xcorr/include/xcorr

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/xcorr" TYPE FILE FILES
    "/home/pi/gr-xcorr/include/xcorr/api.h"
    "/home/pi/gr-xcorr/include/xcorr/capon_ccf.h"
    "/home/pi/gr-xcorr/include/xcorr/unwrap_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/linearslope_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/mode_ii.h"
    "/home/pi/gr-xcorr/include/xcorr/lin_delay_cc.h"
    "/home/pi/gr-xcorr/include/xcorr/peak_estimator_cif.h"
    "/home/pi/gr-xcorr/include/xcorr/cub_delay_cc.h"
    "/home/pi/gr-xcorr/include/xcorr/cumulative_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/variance_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/zero_pad_cc.h"
    "/home/pi/gr-xcorr/include/xcorr/sample_offset_cci.h"
    "/home/pi/gr-xcorr/include/xcorr/hold_ii.h"
    "/home/pi/gr-xcorr/include/xcorr/hold_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/full_capon_ccf.h"
    "/home/pi/gr-xcorr/include/xcorr/full_capon3_ccf.h"
    "/home/pi/gr-xcorr/include/xcorr/multiply_exp_cc.h"
    "/home/pi/gr-xcorr/include/xcorr/pi2pi_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/delay.h"
    "/home/pi/gr-xcorr/include/xcorr/message_sink_i.h"
    "/home/pi/gr-xcorr/include/xcorr/message_sink_f.h"
    "/home/pi/gr-xcorr/include/xcorr/add_const_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/phase2xcorr_ff.h"
    "/home/pi/gr-xcorr/include/xcorr/hold_state_ff.h"
    )
endif()

