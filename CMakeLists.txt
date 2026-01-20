cmake_minimum_required(VERSION 3.20)

project(lab-5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    lab_5/main.cpp
    lab_5/cipher_Caesar.cpp
)

set(HEADERS
    lab_5/cipher_Caesar.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O2)
endif()

find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(
        clang-format
        COMMAND ${CLANG_FORMAT_EXE}
        --dry-run
        -Werror
        ${SOURCES}
        ${HEADERS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Проверка форматирования кода"
    )
endif()
