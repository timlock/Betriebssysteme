# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/vm/Downloads/CLion-2021.2.3/clion-2021.2.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/vm/Downloads/CLion-2021.2.3/clion-2021.2.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vm/Downloads/Praktikum_4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vm/Downloads/Praktikum_4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Praktikum_4.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Praktikum_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Praktikum_4.dir/flags.make

CMakeFiles/Praktikum_4.dir/main.cpp.o: CMakeFiles/Praktikum_4.dir/flags.make
CMakeFiles/Praktikum_4.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vm/Downloads/Praktikum_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Praktikum_4.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Praktikum_4.dir/main.cpp.o -c /home/vm/Downloads/Praktikum_4/main.cpp

CMakeFiles/Praktikum_4.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Praktikum_4.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vm/Downloads/Praktikum_4/main.cpp > CMakeFiles/Praktikum_4.dir/main.cpp.i

CMakeFiles/Praktikum_4.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Praktikum_4.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vm/Downloads/Praktikum_4/main.cpp -o CMakeFiles/Praktikum_4.dir/main.cpp.s

# Object files for target Praktikum_4
Praktikum_4_OBJECTS = \
"CMakeFiles/Praktikum_4.dir/main.cpp.o"

# External object files for target Praktikum_4
Praktikum_4_EXTERNAL_OBJECTS =

Praktikum_4: CMakeFiles/Praktikum_4.dir/main.cpp.o
Praktikum_4: CMakeFiles/Praktikum_4.dir/build.make
Praktikum_4: CMakeFiles/Praktikum_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vm/Downloads/Praktikum_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Praktikum_4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Praktikum_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Praktikum_4.dir/build: Praktikum_4
.PHONY : CMakeFiles/Praktikum_4.dir/build

CMakeFiles/Praktikum_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Praktikum_4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Praktikum_4.dir/clean

CMakeFiles/Praktikum_4.dir/depend:
	cd /home/vm/Downloads/Praktikum_4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vm/Downloads/Praktikum_4 /home/vm/Downloads/Praktikum_4 /home/vm/Downloads/Praktikum_4/cmake-build-debug /home/vm/Downloads/Praktikum_4/cmake-build-debug /home/vm/Downloads/Praktikum_4/cmake-build-debug/CMakeFiles/Praktikum_4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Praktikum_4.dir/depend

