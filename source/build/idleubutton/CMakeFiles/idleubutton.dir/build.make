# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/arthus/Documents/GitHub/GMU/source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/arthus/Documents/GitHub/GMU/source/build

# Include any dependencies generated for this target.
include idleubutton/CMakeFiles/idleubutton.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include idleubutton/CMakeFiles/idleubutton.dir/compiler_depend.make

# Include the progress variables for this target.
include idleubutton/CMakeFiles/idleubutton.dir/progress.make

# Include the compile flags for this target's objects.
include idleubutton/CMakeFiles/idleubutton.dir/flags.make

idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o: idleubutton/CMakeFiles/idleubutton.dir/flags.make
idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o: ../idleubutton/idleubutton.c
idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o: idleubutton/CMakeFiles/idleubutton.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arthus/Documents/GitHub/GMU/source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o"
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o -MF CMakeFiles/idleubutton.dir/idleubutton.c.o.d -o CMakeFiles/idleubutton.dir/idleubutton.c.o -c /Users/arthus/Documents/GitHub/GMU/source/idleubutton/idleubutton.c

idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/idleubutton.dir/idleubutton.c.i"
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/arthus/Documents/GitHub/GMU/source/idleubutton/idleubutton.c > CMakeFiles/idleubutton.dir/idleubutton.c.i

idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/idleubutton.dir/idleubutton.c.s"
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/arthus/Documents/GitHub/GMU/source/idleubutton/idleubutton.c -o CMakeFiles/idleubutton.dir/idleubutton.c.s

# Object files for target idleubutton
idleubutton_OBJECTS = \
"CMakeFiles/idleubutton.dir/idleubutton.c.o"

# External object files for target idleubutton
idleubutton_EXTERNAL_OBJECTS =

/Users/arthus/Documents/GitHub/GMU/externals/idleubutton.mxo/Contents/MacOS/idleubutton: idleubutton/CMakeFiles/idleubutton.dir/idleubutton.c.o
/Users/arthus/Documents/GitHub/GMU/externals/idleubutton.mxo/Contents/MacOS/idleubutton: idleubutton/CMakeFiles/idleubutton.dir/build.make
/Users/arthus/Documents/GitHub/GMU/externals/idleubutton.mxo/Contents/MacOS/idleubutton: idleubutton/CMakeFiles/idleubutton.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/arthus/Documents/GitHub/GMU/source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C CFBundle shared module /Users/arthus/Documents/GitHub/GMU/externals/idleubutton.mxo/Contents/MacOS/idleubutton"
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/idleubutton.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copy PkgInfo"
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && cp /Users/arthus/SDKs/max-sdk/source/max-sdk-base/script/PkgInfo /Users/arthus/Documents/GitHub/GMU/source/../externals/idleubutton.mxo/Contents/PkgInfo

# Rule to build all files generated by this target.
idleubutton/CMakeFiles/idleubutton.dir/build: /Users/arthus/Documents/GitHub/GMU/externals/idleubutton.mxo/Contents/MacOS/idleubutton
.PHONY : idleubutton/CMakeFiles/idleubutton.dir/build

idleubutton/CMakeFiles/idleubutton.dir/clean:
	cd /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton && $(CMAKE_COMMAND) -P CMakeFiles/idleubutton.dir/cmake_clean.cmake
.PHONY : idleubutton/CMakeFiles/idleubutton.dir/clean

idleubutton/CMakeFiles/idleubutton.dir/depend:
	cd /Users/arthus/Documents/GitHub/GMU/source/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/arthus/Documents/GitHub/GMU/source /Users/arthus/Documents/GitHub/GMU/source/idleubutton /Users/arthus/Documents/GitHub/GMU/source/build /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton /Users/arthus/Documents/GitHub/GMU/source/build/idleubutton/CMakeFiles/idleubutton.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : idleubutton/CMakeFiles/idleubutton.dir/depend

