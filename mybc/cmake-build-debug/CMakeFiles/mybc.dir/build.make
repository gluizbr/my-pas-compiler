# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /snap/clion/88/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/88/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gabriel/Faculdade/Compiladores/my-pas-compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/mybc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mybc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mybc.dir/flags.make

CMakeFiles/mybc.dir/main.o: CMakeFiles/mybc.dir/flags.make
CMakeFiles/mybc.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mybc.dir/main.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mybc.dir/main.o   -c /home/gabriel/Faculdade/Compiladores/my-pas-compiler/main.c

CMakeFiles/mybc.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mybc.dir/main.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gabriel/Faculdade/Compiladores/my-pas-compiler/main.c > CMakeFiles/mybc.dir/main.i

CMakeFiles/mybc.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mybc.dir/main.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gabriel/Faculdade/Compiladores/my-pas-compiler/main.c -o CMakeFiles/mybc.dir/main.s

CMakeFiles/mybc.dir/lexer.o: CMakeFiles/mybc.dir/flags.make
CMakeFiles/mybc.dir/lexer.o: ../lexer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/mybc.dir/lexer.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mybc.dir/lexer.o   -c /home/gabriel/Faculdade/Compiladores/my-pas-compiler/lexer.c

CMakeFiles/mybc.dir/lexer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mybc.dir/lexer.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gabriel/Faculdade/Compiladores/my-pas-compiler/lexer.c > CMakeFiles/mybc.dir/lexer.i

CMakeFiles/mybc.dir/lexer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mybc.dir/lexer.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gabriel/Faculdade/Compiladores/my-pas-compiler/lexer.c -o CMakeFiles/mybc.dir/lexer.s

CMakeFiles/mybc.dir/parser-sem.o: CMakeFiles/mybc.dir/flags.make
CMakeFiles/mybc.dir/parser-sem.o: ../parser-sem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/mybc.dir/parser-sem.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mybc.dir/parser-sem.o   -c /home/gabriel/Faculdade/Compiladores/my-pas-compiler/parser-sem.c

CMakeFiles/mybc.dir/parser-sem.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mybc.dir/parser-sem.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gabriel/Faculdade/Compiladores/my-pas-compiler/parser-sem.c > CMakeFiles/mybc.dir/parser-sem.i

CMakeFiles/mybc.dir/parser-sem.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mybc.dir/parser-sem.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gabriel/Faculdade/Compiladores/my-pas-compiler/parser-sem.c -o CMakeFiles/mybc.dir/parser-sem.s

CMakeFiles/mybc.dir/vmachine.o: CMakeFiles/mybc.dir/flags.make
CMakeFiles/mybc.dir/vmachine.o: ../vmachine.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/mybc.dir/vmachine.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mybc.dir/vmachine.o   -c /home/gabriel/Faculdade/Compiladores/my-pas-compiler/vmachine.c

CMakeFiles/mybc.dir/vmachine.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mybc.dir/vmachine.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gabriel/Faculdade/Compiladores/my-pas-compiler/vmachine.c > CMakeFiles/mybc.dir/vmachine.i

CMakeFiles/mybc.dir/vmachine.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mybc.dir/vmachine.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gabriel/Faculdade/Compiladores/my-pas-compiler/vmachine.c -o CMakeFiles/mybc.dir/vmachine.s

# Object files for target mybc
mybc_OBJECTS = \
"CMakeFiles/mybc.dir/main.o" \
"CMakeFiles/mybc.dir/lexer.o" \
"CMakeFiles/mybc.dir/parser-sem.o" \
"CMakeFiles/mybc.dir/vmachine.o"

# External object files for target mybc
mybc_EXTERNAL_OBJECTS =

mybc: CMakeFiles/mybc.dir/main.o
mybc: CMakeFiles/mybc.dir/lexer.o
mybc: CMakeFiles/mybc.dir/parser-sem.o
mybc: CMakeFiles/mybc.dir/vmachine.o
mybc: CMakeFiles/mybc.dir/build.make
mybc: CMakeFiles/mybc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable mybc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mybc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mybc.dir/build: mybc

.PHONY : CMakeFiles/mybc.dir/build

CMakeFiles/mybc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mybc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mybc.dir/clean

CMakeFiles/mybc.dir/depend:
	cd /home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/Faculdade/Compiladores/my-pas-compiler /home/gabriel/Faculdade/Compiladores/my-pas-compiler /home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug /home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug /home/gabriel/Faculdade/Compiladores/my-pas-compiler/cmake-build-debug/CMakeFiles/mybc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mybc.dir/depend
