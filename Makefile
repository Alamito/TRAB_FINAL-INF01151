# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/caues/Documents/Cadeiras_UFRGS/Sistemas_Operacionais_2/TRAB_FINAL-INF01151

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/caues/Documents/Cadeiras_UFRGS/Sistemas_Operacionais_2/TRAB_FINAL-INF01151

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/caues/Documents/Cadeiras_UFRGS/Sistemas_Operacionais_2/TRAB_FINAL-INF01151/CMakeFiles /mnt/c/Users/caues/Documents/Cadeiras_UFRGS/Sistemas_Operacionais_2/TRAB_FINAL-INF01151//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/caues/Documents/Cadeiras_UFRGS/Sistemas_Operacionais_2/TRAB_FINAL-INF01151/CMakeFiles 0
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
# Target rules for targets named Client

# Build rule for target.
Client: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Client
.PHONY : Client

# fast build rule for target.
Client/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/build
.PHONY : Client/fast

client/src/classes/Client.o: client/src/classes/Client.cpp.o
.PHONY : client/src/classes/Client.o

# target to build an object file
client/src/classes/Client.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/classes/Client.cpp.o
.PHONY : client/src/classes/Client.cpp.o

client/src/classes/Client.i: client/src/classes/Client.cpp.i
.PHONY : client/src/classes/Client.i

# target to preprocess a source file
client/src/classes/Client.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/classes/Client.cpp.i
.PHONY : client/src/classes/Client.cpp.i

client/src/classes/Client.s: client/src/classes/Client.cpp.s
.PHONY : client/src/classes/Client.s

# target to generate assembly for a file
client/src/classes/Client.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/classes/Client.cpp.s
.PHONY : client/src/classes/Client.cpp.s

client/src/mainClient.o: client/src/mainClient.cpp.o
.PHONY : client/src/mainClient.o

# target to build an object file
client/src/mainClient.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/mainClient.cpp.o
.PHONY : client/src/mainClient.cpp.o

client/src/mainClient.i: client/src/mainClient.cpp.i
.PHONY : client/src/mainClient.i

# target to preprocess a source file
client/src/mainClient.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/mainClient.cpp.i
.PHONY : client/src/mainClient.cpp.i

client/src/mainClient.s: client/src/mainClient.cpp.s
.PHONY : client/src/mainClient.s

# target to generate assembly for a file
client/src/mainClient.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/client/src/mainClient.cpp.s
.PHONY : client/src/mainClient.cpp.s

shared/src/classes/Message.o: shared/src/classes/Message.cpp.o
.PHONY : shared/src/classes/Message.o

# target to build an object file
shared/src/classes/Message.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Message.cpp.o
.PHONY : shared/src/classes/Message.cpp.o

shared/src/classes/Message.i: shared/src/classes/Message.cpp.i
.PHONY : shared/src/classes/Message.i

# target to preprocess a source file
shared/src/classes/Message.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Message.cpp.i
.PHONY : shared/src/classes/Message.cpp.i

shared/src/classes/Message.s: shared/src/classes/Message.cpp.s
.PHONY : shared/src/classes/Message.s

# target to generate assembly for a file
shared/src/classes/Message.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Message.cpp.s
.PHONY : shared/src/classes/Message.cpp.s

shared/src/classes/Socket.o: shared/src/classes/Socket.cpp.o
.PHONY : shared/src/classes/Socket.o

# target to build an object file
shared/src/classes/Socket.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Socket.cpp.o
.PHONY : shared/src/classes/Socket.cpp.o

shared/src/classes/Socket.i: shared/src/classes/Socket.cpp.i
.PHONY : shared/src/classes/Socket.i

# target to preprocess a source file
shared/src/classes/Socket.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Socket.cpp.i
.PHONY : shared/src/classes/Socket.cpp.i

shared/src/classes/Socket.s: shared/src/classes/Socket.cpp.s
.PHONY : shared/src/classes/Socket.s

# target to generate assembly for a file
shared/src/classes/Socket.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Client.dir/build.make CMakeFiles/Client.dir/shared/src/classes/Socket.cpp.s
.PHONY : shared/src/classes/Socket.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Client"
	@echo "... client/src/classes/Client.o"
	@echo "... client/src/classes/Client.i"
	@echo "... client/src/classes/Client.s"
	@echo "... client/src/mainClient.o"
	@echo "... client/src/mainClient.i"
	@echo "... client/src/mainClient.s"
	@echo "... shared/src/classes/Message.o"
	@echo "... shared/src/classes/Message.i"
	@echo "... shared/src/classes/Message.s"
	@echo "... shared/src/classes/Socket.o"
	@echo "... shared/src/classes/Socket.i"
	@echo "... shared/src/classes/Socket.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

