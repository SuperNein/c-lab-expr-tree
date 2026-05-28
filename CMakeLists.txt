cmake_minimum_required(VERSION 3.20)
project(expr C)

set(CMAKE_C_STANDARD 23)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
        src/*.c
)

add_executable(expr ${SRC_FILES})

target_include_directories(expr PRIVATE include)

target_link_libraries(expr PRIVATE m)

target_compile_options(expr PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Debug>:-O0>
)
