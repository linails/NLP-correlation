# readme.txt
# Created Time: 2016-04-23 14:20:56
# Last modified: 2017-03-07 21:55:31
#
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/libs
    ${PROJECT_SOURCE_DIR}/src/libs/rapidjson
    ${PROJECT_SOURCE_DIR}/src/libs/cppjieba
    ${PROJECT_SOURCE_DIR}/src/libs/jsoncpp/src
    ${PROJECT_SOURCE_DIR}/src/libs/cppjieba/limonp
    ${PROJECT_SOURCE_DIR}/src/libs/x86_64/include
    ${PROJECT_SOURCE_DIR}/src/srcs
)

set(SRC_LIST
   main.cpp 
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
)

add_executable(nlp-correlation ${SRC_LIST})

if ("${SYSTEM_TYPE}" STREQUAL "i686")
set(libs_path ${PROJECT_SOURCE_DIR}/src/libs/i686)
endif()

if ("${SYSTEM_TYPE}" STREQUAL "x86_64")
set(libs_path ${PROJECT_SOURCE_DIR}/src/libs/x86_64)
endif()

set(link_lib
    ${libs_path}/libsqlite3.a
    ${libs_path}/libjson.a
    ${libs_path}/lib/libssl.a
    ${libs_path}/lib/libcrypto.a
    ${libs_path}/lib/libpqxx.a
    ${libs_path}/lib/libgtest.a
    ${libs_path}/lib/libgtest_main.a
    pthread
    dl
    pq
)

target_link_libraries(nlp-correlation ${link_lib})

