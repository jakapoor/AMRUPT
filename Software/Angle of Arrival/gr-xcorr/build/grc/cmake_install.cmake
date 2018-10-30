# Install script for directory: /home/pi/gr-xcorr/grc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/pi/gr-xcorr/grc/xcorr_capon_ccf.xml"
    "/home/pi/gr-xcorr/grc/xcorr_timestamp_ccf.xml"
    "/home/pi/gr-xcorr/grc/xcorr_unwrap_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_linearslope_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_mode_ii.xml"
    "/home/pi/gr-xcorr/grc/xcorr_lin_delay_cc.xml"
    "/home/pi/gr-xcorr/grc/xcorr_peak_estimator_cif.xml"
    "/home/pi/gr-xcorr/grc/xcorr_cub_delay_cc.xml"
    "/home/pi/gr-xcorr/grc/xcorr_cumulative_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_variance_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_zero_pad_cc.xml"
    "/home/pi/gr-xcorr/grc/xcorr_sample_offset_cci.xml"
    "/home/pi/gr-xcorr/grc/xcorr_hold_ii.xml"
    "/home/pi/gr-xcorr/grc/xcorr_hold_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_full_capon_ccf.xml"
    "/home/pi/gr-xcorr/grc/xcorr_full_capon3_ccf.xml"
    "/home/pi/gr-xcorr/grc/xcorr_multiply_exp_cc.xml"
    "/home/pi/gr-xcorr/grc/xcorr_pi2pi_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_arrow_f.xml"
    "/home/pi/gr-xcorr/grc/xcorr_delay.xml"
    "/home/pi/gr-xcorr/grc/xcorr_message_sink_i.xml"
    "/home/pi/gr-xcorr/grc/xcorr_message_sink_f.xml"
    "/home/pi/gr-xcorr/grc/xcorr_add_const_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_phase2xcorr_ff.xml"
    "/home/pi/gr-xcorr/grc/xcorr_hold_state_ff.xml"
    )
endif()

