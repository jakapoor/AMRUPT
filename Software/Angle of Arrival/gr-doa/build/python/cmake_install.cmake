# Install script for directory: /home/pi/gr-doa/python

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/doa" TYPE FILE FILES
    "/home/pi/gr-doa/python/__init__.py"
    "/home/pi/gr-doa/python/phase_correct_hier.py"
    "/home/pi/gr-doa/python/twinrx_usrp_source.py"
    "/home/pi/gr-doa/python/average_and_save.py"
    "/home/pi/gr-doa/python/compass.py"
    "/home/pi/gr-doa/python/save_antenna_calib.py"
    "/home/pi/gr-doa/python/twinrx_phase_offset_est.py"
    "/home/pi/gr-doa/python/findmax_and_save.py"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/doa" TYPE FILE FILES
    "/home/pi/gr-doa/build/python/__init__.pyc"
    "/home/pi/gr-doa/build/python/phase_correct_hier.pyc"
    "/home/pi/gr-doa/build/python/twinrx_usrp_source.pyc"
    "/home/pi/gr-doa/build/python/average_and_save.pyc"
    "/home/pi/gr-doa/build/python/compass.pyc"
    "/home/pi/gr-doa/build/python/save_antenna_calib.pyc"
    "/home/pi/gr-doa/build/python/twinrx_phase_offset_est.pyc"
    "/home/pi/gr-doa/build/python/findmax_and_save.pyc"
    "/home/pi/gr-doa/build/python/__init__.pyo"
    "/home/pi/gr-doa/build/python/phase_correct_hier.pyo"
    "/home/pi/gr-doa/build/python/twinrx_usrp_source.pyo"
    "/home/pi/gr-doa/build/python/average_and_save.pyo"
    "/home/pi/gr-doa/build/python/compass.pyo"
    "/home/pi/gr-doa/build/python/save_antenna_calib.pyo"
    "/home/pi/gr-doa/build/python/twinrx_phase_offset_est.pyo"
    "/home/pi/gr-doa/build/python/findmax_and_save.pyo"
    )
endif()

