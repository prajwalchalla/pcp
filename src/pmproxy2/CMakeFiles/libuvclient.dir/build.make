# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/prajwal/CLionProjects/pcp/src/pmproxy2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/prajwal/CLionProjects/pcp/src/pmproxy2

# Include any dependencies generated for this target.
include CMakeFiles/libuvclient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libuvclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libuvclient.dir/flags.make

CMakeFiles/libuvclient.dir/client.c.o: CMakeFiles/libuvclient.dir/flags.make
CMakeFiles/libuvclient.dir/client.c.o: client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/prajwal/CLionProjects/pcp/src/pmproxy2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/libuvclient.dir/client.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/libuvclient.dir/client.c.o   -c /home/prajwal/CLionProjects/pcp/src/pmproxy2/client.c

CMakeFiles/libuvclient.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libuvclient.dir/client.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/prajwal/CLionProjects/pcp/src/pmproxy2/client.c > CMakeFiles/libuvclient.dir/client.c.i

CMakeFiles/libuvclient.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libuvclient.dir/client.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/prajwal/CLionProjects/pcp/src/pmproxy2/client.c -o CMakeFiles/libuvclient.dir/client.c.s

CMakeFiles/libuvclient.dir/client.c.o.requires:

.PHONY : CMakeFiles/libuvclient.dir/client.c.o.requires

CMakeFiles/libuvclient.dir/client.c.o.provides: CMakeFiles/libuvclient.dir/client.c.o.requires
	$(MAKE) -f CMakeFiles/libuvclient.dir/build.make CMakeFiles/libuvclient.dir/client.c.o.provides.build
.PHONY : CMakeFiles/libuvclient.dir/client.c.o.provides

CMakeFiles/libuvclient.dir/client.c.o.provides.build: CMakeFiles/libuvclient.dir/client.c.o


# Object files for target libuvclient
libuvclient_OBJECTS = \
"CMakeFiles/libuvclient.dir/client.c.o"

# External object files for target libuvclient
libuvclient_EXTERNAL_OBJECTS =

libuvclient: CMakeFiles/libuvclient.dir/client.c.o
libuvclient: CMakeFiles/libuvclient.dir/build.make
libuvclient: CMakeFiles/libuvclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/prajwal/CLionProjects/pcp/src/pmproxy2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable libuvclient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libuvclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libuvclient.dir/build: libuvclient

.PHONY : CMakeFiles/libuvclient.dir/build

CMakeFiles/libuvclient.dir/requires: CMakeFiles/libuvclient.dir/client.c.o.requires

.PHONY : CMakeFiles/libuvclient.dir/requires

CMakeFiles/libuvclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libuvclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libuvclient.dir/clean

CMakeFiles/libuvclient.dir/depend:
	cd /home/prajwal/CLionProjects/pcp/src/pmproxy2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/prajwal/CLionProjects/pcp/src/pmproxy2 /home/prajwal/CLionProjects/pcp/src/pmproxy2 /home/prajwal/CLionProjects/pcp/src/pmproxy2 /home/prajwal/CLionProjects/pcp/src/pmproxy2 /home/prajwal/CLionProjects/pcp/src/pmproxy2/CMakeFiles/libuvclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libuvclient.dir/depend

