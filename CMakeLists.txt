cmake_minimum_required(VERSION 3.8)

project(3dfc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/FileConverter.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/ReadObj.cpp
    ${PROJECT_SOURCE_DIR}/src/WriteStl.cpp)

set(TEST_FILES
    ${PROJECT_SOURCE_DIR}/src/FileConverter.cpp
    ${PROJECT_SOURCE_DIR}/src/ReadObj.cpp
    ${PROJECT_SOURCE_DIR}/src/WriteStl.cpp)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/Core.h
    ${PROJECT_SOURCE_DIR}/include/FileConverter.h
    ${PROJECT_SOURCE_DIR}/include/Reader.h
    ${PROJECT_SOURCE_DIR}/include/ReadObj.h
    ${PROJECT_SOURCE_DIR}/include/Utils.h
    ${PROJECT_SOURCE_DIR}/include/Writer.h
    ${PROJECT_SOURCE_DIR}/include/WriteStl.h)

set(GLM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/glm)
include_directories (${GLM_INCLUDE_DIR})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories (${INCLUDE_DIR})


ENABLE_TESTING()
ADD_SUBDIRECTORY(test)

add_executable(3dfc ${SOURCE_FILES} ${HEADER_FILES})
