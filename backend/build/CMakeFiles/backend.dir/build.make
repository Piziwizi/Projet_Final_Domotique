# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/pi/Documents/Projet_Final_Domotique/backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Documents/Projet_Final_Domotique/backend/build

# Include any dependencies generated for this target.
include CMakeFiles/backend.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/backend.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/backend.dir/flags.make

CMakeFiles/backend.dir/src/main.c.o: CMakeFiles/backend.dir/flags.make
CMakeFiles/backend.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/backend.dir/src/main.c.o"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/backend.dir/src/main.c.o   -c /home/pi/Documents/Projet_Final_Domotique/backend/src/main.c

CMakeFiles/backend.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/backend.dir/src/main.c.i"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Projet_Final_Domotique/backend/src/main.c > CMakeFiles/backend.dir/src/main.c.i

CMakeFiles/backend.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/backend.dir/src/main.c.s"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Projet_Final_Domotique/backend/src/main.c -o CMakeFiles/backend.dir/src/main.c.s

CMakeFiles/backend.dir/src/sensor.c.o: CMakeFiles/backend.dir/flags.make
CMakeFiles/backend.dir/src/sensor.c.o: ../src/sensor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/backend.dir/src/sensor.c.o"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/backend.dir/src/sensor.c.o   -c /home/pi/Documents/Projet_Final_Domotique/backend/src/sensor.c

CMakeFiles/backend.dir/src/sensor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/backend.dir/src/sensor.c.i"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Projet_Final_Domotique/backend/src/sensor.c > CMakeFiles/backend.dir/src/sensor.c.i

CMakeFiles/backend.dir/src/sensor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/backend.dir/src/sensor.c.s"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Projet_Final_Domotique/backend/src/sensor.c -o CMakeFiles/backend.dir/src/sensor.c.s

CMakeFiles/backend.dir/src/control.c.o: CMakeFiles/backend.dir/flags.make
CMakeFiles/backend.dir/src/control.c.o: ../src/control.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/backend.dir/src/control.c.o"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/backend.dir/src/control.c.o   -c /home/pi/Documents/Projet_Final_Domotique/backend/src/control.c

CMakeFiles/backend.dir/src/control.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/backend.dir/src/control.c.i"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Projet_Final_Domotique/backend/src/control.c > CMakeFiles/backend.dir/src/control.c.i

CMakeFiles/backend.dir/src/control.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/backend.dir/src/control.c.s"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Projet_Final_Domotique/backend/src/control.c -o CMakeFiles/backend.dir/src/control.c.s

CMakeFiles/backend.dir/src/logging.c.o: CMakeFiles/backend.dir/flags.make
CMakeFiles/backend.dir/src/logging.c.o: ../src/logging.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/backend.dir/src/logging.c.o"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/backend.dir/src/logging.c.o   -c /home/pi/Documents/Projet_Final_Domotique/backend/src/logging.c

CMakeFiles/backend.dir/src/logging.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/backend.dir/src/logging.c.i"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Projet_Final_Domotique/backend/src/logging.c > CMakeFiles/backend.dir/src/logging.c.i

CMakeFiles/backend.dir/src/logging.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/backend.dir/src/logging.c.s"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Projet_Final_Domotique/backend/src/logging.c -o CMakeFiles/backend.dir/src/logging.c.s

CMakeFiles/backend.dir/src/interface.c.o: CMakeFiles/backend.dir/flags.make
CMakeFiles/backend.dir/src/interface.c.o: ../src/interface.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/backend.dir/src/interface.c.o"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/backend.dir/src/interface.c.o   -c /home/pi/Documents/Projet_Final_Domotique/backend/src/interface.c

CMakeFiles/backend.dir/src/interface.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/backend.dir/src/interface.c.i"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Projet_Final_Domotique/backend/src/interface.c > CMakeFiles/backend.dir/src/interface.c.i

CMakeFiles/backend.dir/src/interface.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/backend.dir/src/interface.c.s"
	/usr/bin/gcc-8 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Projet_Final_Domotique/backend/src/interface.c -o CMakeFiles/backend.dir/src/interface.c.s

# Object files for target backend
backend_OBJECTS = \
"CMakeFiles/backend.dir/src/main.c.o" \
"CMakeFiles/backend.dir/src/sensor.c.o" \
"CMakeFiles/backend.dir/src/control.c.o" \
"CMakeFiles/backend.dir/src/logging.c.o" \
"CMakeFiles/backend.dir/src/interface.c.o"

# External object files for target backend
backend_EXTERNAL_OBJECTS =

backend: CMakeFiles/backend.dir/src/main.c.o
backend: CMakeFiles/backend.dir/src/sensor.c.o
backend: CMakeFiles/backend.dir/src/control.c.o
backend: CMakeFiles/backend.dir/src/logging.c.o
backend: CMakeFiles/backend.dir/src/interface.c.o
backend: CMakeFiles/backend.dir/build.make
backend: CMakeFiles/backend.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable backend"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/backend.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/backend.dir/build: backend

.PHONY : CMakeFiles/backend.dir/build

CMakeFiles/backend.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/backend.dir/cmake_clean.cmake
.PHONY : CMakeFiles/backend.dir/clean

CMakeFiles/backend.dir/depend:
	cd /home/pi/Documents/Projet_Final_Domotique/backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Documents/Projet_Final_Domotique/backend /home/pi/Documents/Projet_Final_Domotique/backend /home/pi/Documents/Projet_Final_Domotique/backend/build /home/pi/Documents/Projet_Final_Domotique/backend/build /home/pi/Documents/Projet_Final_Domotique/backend/build/CMakeFiles/backend.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/backend.dir/depend

