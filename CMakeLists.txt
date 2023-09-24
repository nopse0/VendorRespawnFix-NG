cmake_minimum_required (VERSION 3.8)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

project (
		"VendorRespawnFix-NG"
		VERSION 0.0.0
        DESCRIPTION "VendorRespawnFix built with CommonLibSSE-NG."
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)		

include(GNUInstallDirs)

set(headers
        include/main.h)
		
set(sources
        src/config.cpp
        src/main.cpp)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${headers}
        ${sources})


find_package(CommonLibSSE CONFIG REQUIRED)
find_package(ryml CONFIG REQUIRED)
find_path(ARTICUNO_INCLUDE_DIRS "articuno/articuno.h")
find_library(Detours_LIBRARIES NAMES detours.lib)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ryml::ryml
        ${Detours_LIBRARIES}
)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        src/PCH.h)
