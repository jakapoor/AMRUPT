INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_XCORR xcorr)

FIND_PATH(
    XCORR_INCLUDE_DIRS
    NAMES xcorr/api.h
    HINTS $ENV{XCORR_DIR}/include
        ${PC_XCORR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    XCORR_LIBRARIES
    NAMES gnuradio-xcorr
    HINTS $ENV{XCORR_DIR}/lib
        ${PC_XCORR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XCORR DEFAULT_MSG XCORR_LIBRARIES XCORR_INCLUDE_DIRS)
MARK_AS_ADVANCED(XCORR_LIBRARIES XCORR_INCLUDE_DIRS)

