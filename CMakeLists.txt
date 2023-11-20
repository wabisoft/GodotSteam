cmake_minimum_required(VERSION 3.21)
project (godotsteam VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX _d)

if(DEFINED ENV{STEAMWORKS_SDK})
    set(STEAMWORKS_SDK_DIR $ENV{STEAMWORKS_SDK})
    message(STATUS "STEAMWORKS_SDK_DIR is ${STEAMWORKS_SDK_DIR}")
else()
    message(FATAL_ERROR "STEAMWORKS_SDK is not defined by user environment. Please specify the location of the Steamworks SDK by setting STEAMWORKS_SDK to that path in your environment")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/godotsteam/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/godotsteam/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/godotsteam/src/*.cpp")


add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(STEAM_INCLUDES_DIR ${STEAMWORKS_SDK_DIR}/public)
set(STEAM_BIN_DIR ${STEAMWORKS_SDK_DIR}/redistributable_bin)

if(NOT EXISTS ${STEAM_INCLUDES_DIR})
    message(FATAL_ERROR "Could not locate steam public includes directory at ${STEAM_INCLUDES_DIR}. Please insure you have steamworks sdk installed and it's location specified by STEAMWORKS_SDK environment var.")
endif()
if(NOT EXISTS ${STEAM_BIN_DIR})
    message(FATAL_ERROR "Could not locate steam binaries directory at ${STEAM_BIN_DIR}. Please insure you have steamworks sdk installed and it's location specified by STEAMWORKS_SDK environment var.")
endif()
target_include_directories(${PROJECT_NAME} PRIVATE "${STEAM_INCLUDES_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC wabisoft::cpp)

add_dependencies(${PROJECT_NAME} godot::cpp wabisoft::cpp)

set(STEAM_LIB_PATH ${STEAM_BIN_DIR}/win64/steam_api64.lib)
if(NOT EXISTS ${STEAM_LIB_PATH})
    message(FATAL_ERROR "Could not find steam library at ${STEAM_LIB_PATH}")
else()
    message(STATUS "Found Steam lib at ${STEAM_LIB_PATH}")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC ${STEAM_LIB_PATH})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/godotsteam/src" PREFIX src FILES ${SOURCES})

set(ADDONS_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "The directory to copy the full gdextension deps to, i.e. the resulting dll for this ${PROJECT_NAME} and the .gdextension file and supplementary dlls")

message(STATUS "ADDONS_DIR is ${ADDONS_DIR}")

set(GODOTSTEAM_OUTPUT_DIRECTORY ${ADDONS_DIR}/${PROJECT_NAME})
set(GODOTSTEAM_BINARY_OUTPUT_DIRECTORY ${GODOTSTEAM_OUTPUT_DIRECTORY}/${CMAKE_SYSTEM_NAME})
message(STATUS "Setting output for ${PROJECT_NAME} to ${GODOTSTEAM_OUTPUT_DIRECTORY}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GODOTSTEAM_BINARY_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GODOTSTEAM_BINARY_OUTPUT_DIRECTORY}
)
#TODO: don't hard code the win64 path?
add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${STEAM_BIN_DIR}/win64/steam_api64.dll
                ${GODOTSTEAM_BINARY_OUTPUT_DIRECTORY}/steam_api64.dll
                )

add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/godotsteam.gdextension
                ${GODOTSTEAM_OUTPUT_DIRECTORY}/godotsteam.gdextension
                )
