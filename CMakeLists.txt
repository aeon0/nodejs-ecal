cmake_minimum_required(VERSION 3.10)
project(nodejsEcal)

find_package(eCAL REQUIRED)

# add_compile_definitions(SHOW_DEBUG_COMMENTS)

execute_process(
  COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
include_directories(
  ${CMAKE_JS_INC}
  ${NODE_ADDON_API_DIR}
)

file(GLOB SOURCE_FILES "src/*.cpp")
add_library(${PROJECT_NAME} SHARED
  ${SOURCE_FILES}
  ${CMAKE_JS_SRC}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  eCAL::core
  ${CMAKE_JS_LIB}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
