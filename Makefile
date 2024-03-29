# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/michael/Library/CloudStorage/OneDrive-WesternReserveAcademy/Pioneer Academics/wfc"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/michael/Library/CloudStorage/OneDrive-WesternReserveAcademy/Pioneer Academics/wfc"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/Users/michael/Library/CloudStorage/OneDrive-WesternReserveAcademy/Pioneer Academics/wfc/CMakeFiles" "/Users/michael/Library/CloudStorage/OneDrive-WesternReserveAcademy/Pioneer Academics/wfc//CMakeFiles/progress.marks"
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/Users/michael/Library/CloudStorage/OneDrive-WesternReserveAcademy/Pioneer Academics/wfc/CMakeFiles" 0
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
# Target rules for targets named mcts

# Build rule for target.
mcts: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 mcts
.PHONY : mcts

# fast build rule for target.
mcts/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mcts.dir/build.make CMakeFiles/mcts.dir/build
.PHONY : mcts/fast

#=============================================================================
# Target rules for targets named check

# Build rule for target.
check: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 check
.PHONY : check

# fast build rule for target.
check/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/build
.PHONY : check/fast

#=============================================================================
# Target rules for targets named rand

# Build rule for target.
rand: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 rand
.PHONY : rand

# fast build rule for target.
rand/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rand.dir/build.make CMakeFiles/rand.dir/build
.PHONY : rand/fast

#=============================================================================
# Target rules for targets named graph

# Build rule for target.
graph: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 graph
.PHONY : graph

# fast build rule for target.
graph/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/graph.dir/build.make CMakeFiles/graph.dir/build
.PHONY : graph/fast

#=============================================================================
# Target rules for targets named ua

# Build rule for target.
ua: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ua
.PHONY : ua

# fast build rule for target.
ua/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ua.dir/build.make CMakeFiles/ua.dir/build
.PHONY : ua/fast

checkPath.o: checkPath.cpp.o
.PHONY : checkPath.o

# target to build an object file
checkPath.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/checkPath.cpp.o
.PHONY : checkPath.cpp.o

checkPath.i: checkPath.cpp.i
.PHONY : checkPath.i

# target to preprocess a source file
checkPath.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/checkPath.cpp.i
.PHONY : checkPath.cpp.i

checkPath.s: checkPath.cpp.s
.PHONY : checkPath.s

# target to generate assembly for a file
checkPath.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/checkPath.cpp.s
.PHONY : checkPath.cpp.s

makeGraph.o: makeGraph.cpp.o
.PHONY : makeGraph.o

# target to build an object file
makeGraph.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/graph.dir/build.make CMakeFiles/graph.dir/makeGraph.cpp.o
.PHONY : makeGraph.cpp.o

makeGraph.i: makeGraph.cpp.i
.PHONY : makeGraph.i

# target to preprocess a source file
makeGraph.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/graph.dir/build.make CMakeFiles/graph.dir/makeGraph.cpp.i
.PHONY : makeGraph.cpp.i

makeGraph.s: makeGraph.cpp.s
.PHONY : makeGraph.s

# target to generate assembly for a file
makeGraph.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/graph.dir/build.make CMakeFiles/graph.dir/makeGraph.cpp.s
.PHONY : makeGraph.cpp.s

newPostman.o: newPostman.cpp.o
.PHONY : newPostman.o

# target to build an object file
newPostman.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mcts.dir/build.make CMakeFiles/mcts.dir/newPostman.cpp.o
.PHONY : newPostman.cpp.o

newPostman.i: newPostman.cpp.i
.PHONY : newPostman.i

# target to preprocess a source file
newPostman.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mcts.dir/build.make CMakeFiles/mcts.dir/newPostman.cpp.i
.PHONY : newPostman.cpp.i

newPostman.s: newPostman.cpp.s
.PHONY : newPostman.s

# target to generate assembly for a file
newPostman.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mcts.dir/build.make CMakeFiles/mcts.dir/newPostman.cpp.s
.PHONY : newPostman.cpp.s

randomPostman.o: randomPostman.cpp.o
.PHONY : randomPostman.o

# target to build an object file
randomPostman.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rand.dir/build.make CMakeFiles/rand.dir/randomPostman.cpp.o
.PHONY : randomPostman.cpp.o

randomPostman.i: randomPostman.cpp.i
.PHONY : randomPostman.i

# target to preprocess a source file
randomPostman.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rand.dir/build.make CMakeFiles/rand.dir/randomPostman.cpp.i
.PHONY : randomPostman.cpp.i

randomPostman.s: randomPostman.cpp.s
.PHONY : randomPostman.s

# target to generate assembly for a file
randomPostman.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rand.dir/build.make CMakeFiles/rand.dir/randomPostman.cpp.s
.PHONY : randomPostman.cpp.s

wfcPostman.o: wfcPostman.cpp.o
.PHONY : wfcPostman.o

# target to build an object file
wfcPostman.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ua.dir/build.make CMakeFiles/ua.dir/wfcPostman.cpp.o
.PHONY : wfcPostman.cpp.o

wfcPostman.i: wfcPostman.cpp.i
.PHONY : wfcPostman.i

# target to preprocess a source file
wfcPostman.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ua.dir/build.make CMakeFiles/ua.dir/wfcPostman.cpp.i
.PHONY : wfcPostman.cpp.i

wfcPostman.s: wfcPostman.cpp.s
.PHONY : wfcPostman.s

# target to generate assembly for a file
wfcPostman.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ua.dir/build.make CMakeFiles/ua.dir/wfcPostman.cpp.s
.PHONY : wfcPostman.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... check"
	@echo "... graph"
	@echo "... mcts"
	@echo "... rand"
	@echo "... ua"
	@echo "... checkPath.o"
	@echo "... checkPath.i"
	@echo "... checkPath.s"
	@echo "... makeGraph.o"
	@echo "... makeGraph.i"
	@echo "... makeGraph.s"
	@echo "... newPostman.o"
	@echo "... newPostman.i"
	@echo "... newPostman.s"
	@echo "... randomPostman.o"
	@echo "... randomPostman.i"
	@echo "... randomPostman.s"
	@echo "... wfcPostman.o"
	@echo "... wfcPostman.i"
	@echo "... wfcPostman.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

