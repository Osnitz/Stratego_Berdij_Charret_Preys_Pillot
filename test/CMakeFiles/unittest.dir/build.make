# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot

# Utility rule file for unittest.

# Include any custom commands dependencies for this target.
include test/CMakeFiles/unittest.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/unittest.dir/progress.make

test/CMakeFiles/unittest:

unittest: test/CMakeFiles/unittest
unittest: test/CMakeFiles/unittest.dir/build.make
.PHONY : unittest

# Rule to build all files generated by this target.
test/CMakeFiles/unittest.dir/build: unittest
.PHONY : test/CMakeFiles/unittest.dir/build

test/CMakeFiles/unittest.dir/clean:
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test && $(CMAKE_COMMAND) -P CMakeFiles/unittest.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/unittest.dir/clean

test/CMakeFiles/unittest.dir/depend:
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/CMakeFiles/unittest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/CMakeFiles/unittest.dir/depend

