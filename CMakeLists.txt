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

set(headers
        include/main.h)
		
set(sources
        src/main.cpp

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

