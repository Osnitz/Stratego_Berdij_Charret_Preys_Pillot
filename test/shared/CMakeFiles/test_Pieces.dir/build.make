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

# Include any dependencies generated for this target.
include test/shared/CMakeFiles/test_Pieces.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/shared/CMakeFiles/test_Pieces.dir/compiler_depend.make

# Include the progress variables for this target.
include test/shared/CMakeFiles/test_Pieces.dir/progress.make

# Include the compile flags for this target's objects.
include test/shared/CMakeFiles/test_Pieces.dir/flags.make

test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o: test/shared/CMakeFiles/test_Pieces.dir/flags.make
test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o: test/shared/test_Pieces.cpp
test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o: test/shared/CMakeFiles/test_Pieces.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o"
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o -MF CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o.d -o CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o -c /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared/test_Pieces.cpp

test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_Pieces.dir/test_Pieces.cpp.i"
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared/test_Pieces.cpp > CMakeFiles/test_Pieces.dir/test_Pieces.cpp.i

test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_Pieces.dir/test_Pieces.cpp.s"
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared/test_Pieces.cpp -o CMakeFiles/test_Pieces.dir/test_Pieces.cpp.s

# Object files for target test_Pieces
test_Pieces_OBJECTS = \
"CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o"

# External object files for target test_Pieces
test_Pieces_EXTERNAL_OBJECTS =

test/shared/test_Pieces: test/shared/CMakeFiles/test_Pieces.dir/test_Pieces.cpp.o
test/shared/test_Pieces: test/shared/CMakeFiles/test_Pieces.dir/build.make
test/shared/test_Pieces: /opt/homebrew/lib/libboost_unit_test_framework-mt.dylib
test/shared/test_Pieces: src/shared/libshared_static.a
test/shared/test_Pieces: src/client/libclient_static.a
test/shared/test_Pieces: src/shared/libshared_static.a
test/shared/test_Pieces: /opt/homebrew/lib/libsfml-graphics.2.6.2.dylib
test/shared/test_Pieces: /opt/homebrew/lib/libsfml-network.2.6.2.dylib
test/shared/test_Pieces: /opt/homebrew/lib/libsfml-window.2.6.2.dylib
test/shared/test_Pieces: /opt/homebrew/lib/libsfml-system.2.6.2.dylib
test/shared/test_Pieces: test/shared/CMakeFiles/test_Pieces.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_Pieces"
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_Pieces.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/shared/CMakeFiles/test_Pieces.dir/build: test/shared/test_Pieces
.PHONY : test/shared/CMakeFiles/test_Pieces.dir/build

test/shared/CMakeFiles/test_Pieces.dir/clean:
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared && $(CMAKE_COMMAND) -P CMakeFiles/test_Pieces.dir/cmake_clean.cmake
.PHONY : test/shared/CMakeFiles/test_Pieces.dir/clean

test/shared/CMakeFiles/test_Pieces.dir/depend:
	cd /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/test/shared/CMakeFiles/test_Pieces.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/shared/CMakeFiles/test_Pieces.dir/depend

