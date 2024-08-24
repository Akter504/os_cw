cmake_minimum_required(VERSION 3.21)
project(os_cw_allctr_allctr_rb_tr)

add_subdirectory(tests)
add_library(
        os_cw_allctr_allctr_rb_tr
        src/allocator_red_black_tree.cpp)
target_include_directories(
        os_cw_allctr_allctr_rb_tr
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_allctr_allctr_rb_tr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_allctr_allctr_rb_tr
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_allctr_allctr_rb_tr
        PUBLIC
        os_cw_allctr_allctr)
set_target_properties(
        os_cw_allctr_allctr_rb_tr PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "red-black tree allocator implementation library")