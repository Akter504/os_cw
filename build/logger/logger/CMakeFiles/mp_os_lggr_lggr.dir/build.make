# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\labs mai\mp_os"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\labs mai\mp_os\build"

# Include any dependencies generated for this target.
include logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/compiler_depend.make

# Include the progress variables for this target.
include logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/progress.make

# Include the compile flags for this target's objects.
include logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/flags.make

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/includes_CXX.rsp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj: C:/labs\ mai/mp_os/logger/logger/src/logger.cpp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\labs mai\mp_os\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj -MF CMakeFiles\mp_os_lggr_lggr.dir\src\logger.cpp.obj.d -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger.cpp.obj -c "C:\labs mai\mp_os\logger\logger\src\logger.cpp"

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.i"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\labs mai\mp_os\logger\logger\src\logger.cpp" > CMakeFiles\mp_os_lggr_lggr.dir\src\logger.cpp.i

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.s"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\labs mai\mp_os\logger\logger\src\logger.cpp" -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger.cpp.s

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/includes_CXX.rsp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj: C:/labs\ mai/mp_os/logger/logger/src/logger_guardant.cpp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\labs mai\mp_os\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj -MF CMakeFiles\mp_os_lggr_lggr.dir\src\logger_guardant.cpp.obj.d -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger_guardant.cpp.obj -c "C:\labs mai\mp_os\logger\logger\src\logger_guardant.cpp"

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.i"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\labs mai\mp_os\logger\logger\src\logger_guardant.cpp" > CMakeFiles\mp_os_lggr_lggr.dir\src\logger_guardant.cpp.i

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.s"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\labs mai\mp_os\logger\logger\src\logger_guardant.cpp" -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger_guardant.cpp.s

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/includes_CXX.rsp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj: C:/labs\ mai/mp_os/logger/logger/src/logger_builder.cpp
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\labs mai\mp_os\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj -MF CMakeFiles\mp_os_lggr_lggr.dir\src\logger_builder.cpp.obj.d -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger_builder.cpp.obj -c "C:\labs mai\mp_os\logger\logger\src\logger_builder.cpp"

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.i"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\labs mai\mp_os\logger\logger\src\logger_builder.cpp" > CMakeFiles\mp_os_lggr_lggr.dir\src\logger_builder.cpp.i

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.s"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && C:\LABSMA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\labs mai\mp_os\logger\logger\src\logger_builder.cpp" -o CMakeFiles\mp_os_lggr_lggr.dir\src\logger_builder.cpp.s

# Object files for target mp_os_lggr_lggr
mp_os_lggr_lggr_OBJECTS = \
"CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj" \
"CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj" \
"CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj"

# External object files for target mp_os_lggr_lggr
mp_os_lggr_lggr_EXTERNAL_OBJECTS =

logger/logger/libmp_os_lggr_lggr.a: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger.cpp.obj
logger/logger/libmp_os_lggr_lggr.a: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_guardant.cpp.obj
logger/logger/libmp_os_lggr_lggr.a: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/src/logger_builder.cpp.obj
logger/logger/libmp_os_lggr_lggr.a: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/build.make
logger/logger/libmp_os_lggr_lggr.a: logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\labs mai\mp_os\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libmp_os_lggr_lggr.a"
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && $(CMAKE_COMMAND) -P CMakeFiles\mp_os_lggr_lggr.dir\cmake_clean_target.cmake
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\mp_os_lggr_lggr.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/build: logger/logger/libmp_os_lggr_lggr.a
.PHONY : logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/build

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/clean:
	cd /d C:\LABSMA~1\mp_os\build\logger\logger && $(CMAKE_COMMAND) -P CMakeFiles\mp_os_lggr_lggr.dir\cmake_clean.cmake
.PHONY : logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/clean

logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\labs mai\mp_os" "C:\labs mai\mp_os\logger\logger" "C:\labs mai\mp_os\build" "C:\labs mai\mp_os\build\logger\logger" "C:\labs mai\mp_os\build\logger\logger\CMakeFiles\mp_os_lggr_lggr.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : logger/logger/CMakeFiles/mp_os_lggr_lggr.dir/depend

