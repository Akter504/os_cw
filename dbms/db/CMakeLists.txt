cmake_minimum_required(VERSION 3.21)
project(os_cw_dbms_db_strg)

add_library(
        os_cw_dbms_db_strg
        src/db.cpp)
target_include_directories(
        os_cw_dbms_db_strg
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_lggr_srvr_lggr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr_srch_tr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_bndr_tgs)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_bdds_sstm)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_glbl_hp)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_rb_tr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_srtd_lst)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_flwght_str_pool)
        
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_dbms_my_types)
set_target_properties(
        os_cw_dbms_db_strg PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "database storage library")