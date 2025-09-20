# cmake_minimum_required(VERSION 3.10)
# project(lab_1)
# find_program(CLANG_FORMAT "clang-format")
# if(CLANG_FORMAT)
#     file(GLOB_RECURSE SOURCE_FILES ".cpp" ".h")
#     if(SOURCE_FILES)
#         add_custom_target(clang-format
#             COMMAND ${CLANG_FORMAT} -i --style=file ${SOURCE_FILES}
#             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#             COMMENT "Formatting code with clang-format"
#         )
#     else()
#         add_custom_target(clang-format
#             COMMAND echo "No source files found for formatting"
#             COMMENT "clang-format: no files to format"
#         )
#     endif()
# else()
#     add_custom_target(clang-format
#         COMMAND echo "clang-format not found"
#         COMMENT "clang-format: tool not available"
#     )
# endif()

# cmake_minimum_required(VERSION 3.20)

# project(Example)

# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# set(SOURCES
#     ${Example}/main.cpp

# )


# add_executable(${Example} ${SOURCES})
# set_property(TARGET ${Example} PROPERTY CXX_STANDARD 20)
# target_compile_options(${Example} PRIVATE -Wall -Wextra -Werror -Wpedantic)

# if(ENABLE_DEBUG)
#     target_compile_options(${Example} PRIVATE -g)
# endif()

# add_custom_target(
#     clang-format
#     COMMAND clang-format
#     --dry-run
#     -Werror
#     ${SOURCES}
# )


cmake_minimum_required(VERSION 3.20)

project(lab_1)

# ПЕРЕМЕСТИТЕ эту строку ПОСЛЕ project()
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lab_1.cpp
)

# Убедитесь, что HEADERS определен или удалите его
set(HEADERS "")  # добавьте эту строку если HEADERS не определен

add_executable(lab_1 ${SOURCES})
set_property(TARGET lab_1 PROPERTY CXX_STANDARD 20)
target_compile_options(lab_1 PRIVATE -Wall -Wextra -Werror -Wpedantic)

if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()



add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${SOURCES}

)


