cmake_minimum_required(VERSION 3.10)

project(collog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

include_directories(
	include
	CoNCoL/include
) 

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(SOURCES ${SOURCE_DIR}/collog.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_subdirectory(CoNCoL)
add_subdirectory(test)
