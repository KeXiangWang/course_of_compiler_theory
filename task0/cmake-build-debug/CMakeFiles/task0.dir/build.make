# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\OneDrive\CLionProjects\course_of_compiler_theory\task0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/task0.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/task0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/task0.dir/flags.make

CMakeFiles/task0.dir/main.cpp.obj: CMakeFiles/task0.dir/flags.make
CMakeFiles/task0.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/task0.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\task0.dir\main.cpp.obj -c D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\main.cpp

CMakeFiles/task0.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/task0.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\main.cpp > CMakeFiles\task0.dir\main.cpp.i

CMakeFiles/task0.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/task0.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\main.cpp -o CMakeFiles\task0.dir\main.cpp.s

# Object files for target task0
task0_OBJECTS = \
"CMakeFiles/task0.dir/main.cpp.obj"

# External object files for target task0
task0_EXTERNAL_OBJECTS =

task0.exe: CMakeFiles/task0.dir/main.cpp.obj
task0.exe: CMakeFiles/task0.dir/build.make
task0.exe: CMakeFiles/task0.dir/linklibs.rsp
task0.exe: CMakeFiles/task0.dir/objects1.rsp
task0.exe: CMakeFiles/task0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable task0.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\task0.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/task0.dir/build: task0.exe

.PHONY : CMakeFiles/task0.dir/build

CMakeFiles/task0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\task0.dir\cmake_clean.cmake
.PHONY : CMakeFiles/task0.dir/clean

CMakeFiles/task0.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\OneDrive\CLionProjects\course_of_compiler_theory\task0 D:\OneDrive\CLionProjects\course_of_compiler_theory\task0 D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug D:\OneDrive\CLionProjects\course_of_compiler_theory\task0\cmake-build-debug\CMakeFiles\task0.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/task0.dir/depend

