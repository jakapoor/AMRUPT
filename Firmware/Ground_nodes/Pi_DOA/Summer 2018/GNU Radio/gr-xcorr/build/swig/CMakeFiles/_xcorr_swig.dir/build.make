# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/gr-xcorr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/gr-xcorr/build

# Include any dependencies generated for this target.
include swig/CMakeFiles/_xcorr_swig.dir/depend.make

# Include the progress variables for this target.
include swig/CMakeFiles/_xcorr_swig.dir/progress.make

# Include the compile flags for this target's objects.
include swig/CMakeFiles/_xcorr_swig.dir/flags.make

swig/xcorr_swigPYTHON_wrap.cxx: swig/xcorr_swig_swig_2d0df


swig/xcorr_swig.py: swig/xcorr_swig_swig_2d0df


swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/_xcorr_swig.dir/flags.make
swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o: swig/xcorr_swigPYTHON_wrap.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/gr-xcorr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o"
	cd /home/pi/gr-xcorr/build/swig && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Wno-unused-but-set-variable -o CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o -c /home/pi/gr-xcorr/build/swig/xcorr_swigPYTHON_wrap.cxx

swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.i"
	cd /home/pi/gr-xcorr/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Wno-unused-but-set-variable -E /home/pi/gr-xcorr/build/swig/xcorr_swigPYTHON_wrap.cxx > CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.i

swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.s"
	cd /home/pi/gr-xcorr/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Wno-unused-but-set-variable -S /home/pi/gr-xcorr/build/swig/xcorr_swigPYTHON_wrap.cxx -o CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.s

swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.requires:

.PHONY : swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.requires

swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.provides: swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.requires
	$(MAKE) -f swig/CMakeFiles/_xcorr_swig.dir/build.make swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.provides.build
.PHONY : swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.provides

swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.provides.build: swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o


# Object files for target _xcorr_swig
_xcorr_swig_OBJECTS = \
"CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o"

# External object files for target _xcorr_swig
_xcorr_swig_EXTERNAL_OBJECTS =

swig/_xcorr_swig.so: swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o
swig/_xcorr_swig.so: swig/CMakeFiles/_xcorr_swig.dir/build.make
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libpython2.7.so
swig/_xcorr_swig.so: lib/libgnuradio-xcorr-1.0.0git.so.0.0.0
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libboost_filesystem.so
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libboost_system.so
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libgnuradio-runtime.so
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libgnuradio-pmt.so
swig/_xcorr_swig.so: /usr/lib/arm-linux-gnueabihf/libgnuradio-fft.so
swig/_xcorr_swig.so: swig/CMakeFiles/_xcorr_swig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/gr-xcorr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module _xcorr_swig.so"
	cd /home/pi/gr-xcorr/build/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_xcorr_swig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/CMakeFiles/_xcorr_swig.dir/build: swig/_xcorr_swig.so

.PHONY : swig/CMakeFiles/_xcorr_swig.dir/build

swig/CMakeFiles/_xcorr_swig.dir/requires: swig/CMakeFiles/_xcorr_swig.dir/xcorr_swigPYTHON_wrap.cxx.o.requires

.PHONY : swig/CMakeFiles/_xcorr_swig.dir/requires

swig/CMakeFiles/_xcorr_swig.dir/clean:
	cd /home/pi/gr-xcorr/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/_xcorr_swig.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/_xcorr_swig.dir/clean

swig/CMakeFiles/_xcorr_swig.dir/depend: swig/xcorr_swigPYTHON_wrap.cxx
swig/CMakeFiles/_xcorr_swig.dir/depend: swig/xcorr_swig.py
	cd /home/pi/gr-xcorr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/gr-xcorr /home/pi/gr-xcorr/swig /home/pi/gr-xcorr/build /home/pi/gr-xcorr/build/swig /home/pi/gr-xcorr/build/swig/CMakeFiles/_xcorr_swig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/_xcorr_swig.dir/depend

