# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_SOURCE_DIR = /tmp/tmp.oPOmILp7xO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.oPOmILp7xO/cmake-build-arch_debug

# Include any dependencies generated for this target.
include CMakeFiles/debugger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/debugger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/debugger.dir/flags.make

CMakeFiles/debugger.dir/src/main.cpp.o: CMakeFiles/debugger.dir/flags.make
CMakeFiles/debugger.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.oPOmILp7xO/cmake-build-arch_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/debugger.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/debugger.dir/src/main.cpp.o -c /tmp/tmp.oPOmILp7xO/src/main.cpp

CMakeFiles/debugger.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/debugger.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.oPOmILp7xO/src/main.cpp > CMakeFiles/debugger.dir/src/main.cpp.i

CMakeFiles/debugger.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/debugger.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.oPOmILp7xO/src/main.cpp -o CMakeFiles/debugger.dir/src/main.cpp.s

CMakeFiles/debugger.dir/src/debugger.cpp.o: CMakeFiles/debugger.dir/flags.make
CMakeFiles/debugger.dir/src/debugger.cpp.o: ../src/debugger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.oPOmILp7xO/cmake-build-arch_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/debugger.dir/src/debugger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/debugger.dir/src/debugger.cpp.o -c /tmp/tmp.oPOmILp7xO/src/debugger.cpp

CMakeFiles/debugger.dir/src/debugger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/debugger.dir/src/debugger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.oPOmILp7xO/src/debugger.cpp > CMakeFiles/debugger.dir/src/debugger.cpp.i

CMakeFiles/debugger.dir/src/debugger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/debugger.dir/src/debugger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.oPOmILp7xO/src/debugger.cpp -o CMakeFiles/debugger.dir/src/debugger.cpp.s

CMakeFiles/debugger.dir/src/breakpoint.cpp.o: CMakeFiles/debugger.dir/flags.make
CMakeFiles/debugger.dir/src/breakpoint.cpp.o: ../src/breakpoint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.oPOmILp7xO/cmake-build-arch_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/debugger.dir/src/breakpoint.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/debugger.dir/src/breakpoint.cpp.o -c /tmp/tmp.oPOmILp7xO/src/breakpoint.cpp

CMakeFiles/debugger.dir/src/breakpoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/debugger.dir/src/breakpoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.oPOmILp7xO/src/breakpoint.cpp > CMakeFiles/debugger.dir/src/breakpoint.cpp.i

CMakeFiles/debugger.dir/src/breakpoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/debugger.dir/src/breakpoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.oPOmILp7xO/src/breakpoint.cpp -o CMakeFiles/debugger.dir/src/breakpoint.cpp.s

# Object files for target debugger
debugger_OBJECTS = \
"CMakeFiles/debugger.dir/src/main.cpp.o" \
"CMakeFiles/debugger.dir/src/debugger.cpp.o" \
"CMakeFiles/debugger.dir/src/breakpoint.cpp.o"

# External object files for target debugger
debugger_EXTERNAL_OBJECTS =

debugger: CMakeFiles/debugger.dir/src/main.cpp.o
debugger: CMakeFiles/debugger.dir/src/debugger.cpp.o
debugger: CMakeFiles/debugger.dir/src/breakpoint.cpp.o
debugger: CMakeFiles/debugger.dir/build.make
debugger: /usr/lib/liblinenoise.so
debugger: CMakeFiles/debugger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.oPOmILp7xO/cmake-build-arch_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable debugger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/debugger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/debugger.dir/build: debugger

.PHONY : CMakeFiles/debugger.dir/build

CMakeFiles/debugger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/debugger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/debugger.dir/clean

CMakeFiles/debugger.dir/depend:
	cd /tmp/tmp.oPOmILp7xO/cmake-build-arch_debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.oPOmILp7xO /tmp/tmp.oPOmILp7xO /tmp/tmp.oPOmILp7xO/cmake-build-arch_debug /tmp/tmp.oPOmILp7xO/cmake-build-arch_debug /tmp/tmp.oPOmILp7xO/cmake-build-arch_debug/CMakeFiles/debugger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/debugger.dir/depend
