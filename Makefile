# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/opt/homebrew/Cellar/cmake/3.30.4/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/CMakeFiles /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/Souhaila.B/Desktop/COURS/ENSEA_3A/Stratego_Berdij_Charret_Preys_Pillot/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named generate-headers

# Build rule for target.
generate-headers: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 generate-headers
.PHONY : generate-headers

# fast build rule for target.
generate-headers/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/generate-headers.dir/build.make CMakeFiles/generate-headers.dir/build
.PHONY : generate-headers/fast

#=============================================================================
# Target rules for targets named clean-headers

# Build rule for target.
clean-headers: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean-headers
.PHONY : clean-headers

# fast build rule for target.
clean-headers/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/clean-headers.dir/build.make CMakeFiles/clean-headers.dir/build
.PHONY : clean-headers/fast

#=============================================================================
# Target rules for targets named code-coverage

# Build rule for target.
code-coverage: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 code-coverage
.PHONY : code-coverage

# fast build rule for target.
code-coverage/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code-coverage.dir/build.make CMakeFiles/code-coverage.dir/build
.PHONY : code-coverage/fast

#=============================================================================
# Target rules for targets named code-coverage-gcov

# Build rule for target.
code-coverage-gcov: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 code-coverage-gcov
.PHONY : code-coverage-gcov

# fast build rule for target.
code-coverage-gcov/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code-coverage-gcov.dir/build.make CMakeFiles/code-coverage-gcov.dir/build
.PHONY : code-coverage-gcov/fast

#=============================================================================
# Target rules for targets named dia2code

# Build rule for target.
dia2code: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 dia2code
.PHONY : dia2code

# fast build rule for target.
dia2code/fast:
	$(MAKE) $(MAKESILENT) -f extern/dia2code/CMakeFiles/dia2code.dir/build.make extern/dia2code/CMakeFiles/dia2code.dir/build
.PHONY : dia2code/fast

#=============================================================================
# Target rules for targets named generate-header-state

# Build rule for target.
generate-header-state: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 generate-header-state
.PHONY : generate-header-state

# fast build rule for target.
generate-header-state/fast:
	$(MAKE) $(MAKESILENT) -f src/shared/CMakeFiles/generate-header-state.dir/build.make src/shared/CMakeFiles/generate-header-state.dir/build
.PHONY : generate-header-state/fast

#=============================================================================
# Target rules for targets named generate-header-engine

# Build rule for target.
generate-header-engine: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 generate-header-engine
.PHONY : generate-header-engine

# fast build rule for target.
generate-header-engine/fast:
	$(MAKE) $(MAKESILENT) -f src/shared/CMakeFiles/generate-header-engine.dir/build.make src/shared/CMakeFiles/generate-header-engine.dir/build
.PHONY : generate-header-engine/fast

#=============================================================================
# Target rules for targets named generate-header-ai

# Build rule for target.
generate-header-ai: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 generate-header-ai
.PHONY : generate-header-ai

# fast build rule for target.
generate-header-ai/fast:
	$(MAKE) $(MAKESILENT) -f src/shared/CMakeFiles/generate-header-ai.dir/build.make src/shared/CMakeFiles/generate-header-ai.dir/build
.PHONY : generate-header-ai/fast

#=============================================================================
# Target rules for targets named shared_static

# Build rule for target.
shared_static: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 shared_static
.PHONY : shared_static

# fast build rule for target.
shared_static/fast:
	$(MAKE) $(MAKESILENT) -f src/shared/CMakeFiles/shared_static.dir/build.make src/shared/CMakeFiles/shared_static.dir/build
.PHONY : shared_static/fast

#=============================================================================
# Target rules for targets named generate-header-client

# Build rule for target.
generate-header-client: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 generate-header-client
.PHONY : generate-header-client

# fast build rule for target.
generate-header-client/fast:
	$(MAKE) $(MAKESILENT) -f src/client/CMakeFiles/generate-header-client.dir/build.make src/client/CMakeFiles/generate-header-client.dir/build
.PHONY : generate-header-client/fast

#=============================================================================
# Target rules for targets named client_static

# Build rule for target.
client_static: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 client_static
.PHONY : client_static

# fast build rule for target.
client_static/fast:
	$(MAKE) $(MAKESILENT) -f src/client/CMakeFiles/client_static.dir/build.make src/client/CMakeFiles/client_static.dir/build
.PHONY : client_static/fast

#=============================================================================
# Target rules for targets named client

# Build rule for target.
client: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 client
.PHONY : client

# fast build rule for target.
client/fast:
	$(MAKE) $(MAKESILENT) -f src/client/CMakeFiles/client.dir/build.make src/client/CMakeFiles/client.dir/build
.PHONY : client/fast

#=============================================================================
# Target rules for targets named client_sfml

# Build rule for target.
client_sfml: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 client_sfml
.PHONY : client_sfml

# fast build rule for target.
client_sfml/fast:
	$(MAKE) $(MAKESILENT) -f src/client/CMakeFiles/client_sfml.dir/build.make src/client/CMakeFiles/client_sfml.dir/build
.PHONY : client_sfml/fast

#=============================================================================
# Target rules for targets named rapport

# Build rule for target.
rapport: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 rapport
.PHONY : rapport

# fast build rule for target.
rapport/fast:
	$(MAKE) $(MAKESILENT) -f rapport/CMakeFiles/rapport.dir/build.make rapport/CMakeFiles/rapport.dir/build
.PHONY : rapport/fast

#=============================================================================
# Target rules for targets named rapport-diapdf-state

# Build rule for target.
rapport-diapdf-state: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 rapport-diapdf-state
.PHONY : rapport-diapdf-state

# fast build rule for target.
rapport-diapdf-state/fast:
	$(MAKE) $(MAKESILENT) -f rapport/CMakeFiles/rapport-diapdf-state.dir/build.make rapport/CMakeFiles/rapport-diapdf-state.dir/build
.PHONY : rapport-diapdf-state/fast

#=============================================================================
# Target rules for targets named check

# Build rule for target.
check: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 check
.PHONY : check

# fast build rule for target.
check/fast:
	$(MAKE) $(MAKESILENT) -f test/CMakeFiles/check.dir/build.make test/CMakeFiles/check.dir/build
.PHONY : check/fast

#=============================================================================
# Target rules for targets named unittest

# Build rule for target.
unittest: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 unittest
.PHONY : unittest

# fast build rule for target.
unittest/fast:
	$(MAKE) $(MAKESILENT) -f test/CMakeFiles/unittest.dir/build.make test/CMakeFiles/unittest.dir/build
.PHONY : unittest/fast

#=============================================================================
# Target rules for targets named test_Engine

# Build rule for target.
test_Engine: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_Engine
.PHONY : test_Engine

# fast build rule for target.
test_Engine/fast:
	$(MAKE) $(MAKESILENT) -f test/shared/CMakeFiles/test_Engine.dir/build.make test/shared/CMakeFiles/test_Engine.dir/build
.PHONY : test_Engine/fast

#=============================================================================
# Target rules for targets named test_RandomAI

# Build rule for target.
test_RandomAI: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_RandomAI
.PHONY : test_RandomAI

# fast build rule for target.
test_RandomAI/fast:
	$(MAKE) $(MAKESILENT) -f test/shared/CMakeFiles/test_RandomAI.dir/build.make test/shared/CMakeFiles/test_RandomAI.dir/build
.PHONY : test_RandomAI/fast

#=============================================================================
# Target rules for targets named test_ScenarioManager

# Build rule for target.
test_ScenarioManager: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_ScenarioManager
.PHONY : test_ScenarioManager

# fast build rule for target.
test_ScenarioManager/fast:
	$(MAKE) $(MAKESILENT) -f test/client/CMakeFiles/test_ScenarioManager.dir/build.make test/client/CMakeFiles/test_ScenarioManager.dir/build
.PHONY : test_ScenarioManager/fast

#=============================================================================
# Target rules for targets named test_HumanController

# Build rule for target.
test_HumanController: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_HumanController
.PHONY : test_HumanController

# fast build rule for target.
test_HumanController/fast:
	$(MAKE) $(MAKESILENT) -f test/client/CMakeFiles/test_HumanController.dir/build.make test/client/CMakeFiles/test_HumanController.dir/build
.PHONY : test_HumanController/fast

#=============================================================================
# Target rules for targets named test_AIController

# Build rule for target.
test_AIController: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_AIController
.PHONY : test_AIController

# fast build rule for target.
test_AIController/fast:
	$(MAKE) $(MAKESILENT) -f test/client/CMakeFiles/test_AIController.dir/build.make test/client/CMakeFiles/test_AIController.dir/build
.PHONY : test_AIController/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... check"
	@echo "... clean-headers"
	@echo "... code-coverage"
	@echo "... code-coverage-gcov"
	@echo "... generate-header-ai"
	@echo "... generate-header-client"
	@echo "... generate-header-engine"
	@echo "... generate-header-state"
	@echo "... generate-headers"
	@echo "... rapport"
	@echo "... rapport-diapdf-state"
	@echo "... unittest"
	@echo "... client"
	@echo "... client_sfml"
	@echo "... client_static"
	@echo "... dia2code"
	@echo "... shared_static"
	@echo "... test_AIController"
	@echo "... test_Engine"
	@echo "... test_HumanController"
	@echo "... test_RandomAI"
	@echo "... test_ScenarioManager"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

