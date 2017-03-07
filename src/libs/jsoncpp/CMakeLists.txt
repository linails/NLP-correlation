cmake_minimum_required(VERSION 2.8)
include_directories(
    ./include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/jutils/jsoncpp
)

link_directories(
    ${SYS_PATH}/lib/
    ${SYS_PATH}/usr/lib/
    ${SYS_PATH}/local/lib
)

set(JCONFIG_JSON_JSONCPP_LIB_SRC_LIST 
    src/json_batchallocator.h
    src/json_reader.cpp
    src/json_tool.h
    src/json_value.cpp
    src/json_writer.cpp
)

add_library(jutil-json-jsoncpp-lib${JDEBUG_FLAG}-static  STATIC ${JCONFIG_JSON_JSONCPP_LIB_SRC_LIST})

set_target_properties(jutil-json-jsoncpp-lib${JDEBUG_FLAG}-static PROPERTIES OUTPUT_NAME jutil-json-jsoncpp-lib${JDEBUG_FLAG})
set_target_properties(jutil-json-jsoncpp-lib${JDEBUG_FLAG}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

install( FILES 
         ${PROJECT_BINARY_DIR}/libjutil-json-jsoncpp-lib${JDEBUG_FLAG}.a
         DESTINATION
         ${CMAKE_INSTALL_PREFIX}/lib
         PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ
    )
