# readme.txt
# Created Time: 2016-04-23 14:20:56
# Last modified: 2017-03-09 08:55:00
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
   srcs/alpha-bet.cpp
   srcs/denoise.cpp
   srcs/dic-parser.cpp
   srcs/disk-dic.cpp
   srcs/format-later-stage.cpp
   srcs/format-parser.cpp
   srcs/formatParsing.cpp
   srcs/python-module.cpp
   srcs/regex_common.cpp
   srcs/rfileoprt.cpp
   srcs/rwfileoprt.cpp
   srcs/segmentation.cpp
   srcs/statistics.cpp
   srcs/stringTools.cpp
   srcs/timer.cpp
   srcs/scripts-manager.cpp
   srcs/socket-ipc.cpp
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

