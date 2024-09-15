cmake_minimum_required(VERSION 3.21)
project(os_cw_hndlr_files)

add_library(
        os_cw_hndlr_files
        ./include/commands_realization.h
        ./include/parser_file.h
        ./include/request_handler.h
        ./include/file_validation.h)

target_include_directories(
        os_cw_hndlr_files
        PUBLIC
	./include)
target_link_libraries(
	os_cw_hndlr_files
	PUBLIC
	os_cw_dbms_db_strg)
set_target_properties(
        os_cw_hndlr_files PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "handler files")