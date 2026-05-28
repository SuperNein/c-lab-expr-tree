cmake_minimum_required(VERSION 3.20)
project(expr C)

set(CMAKE_C_STANDARD 23)

include_directories(include)

add_executable(expr
        src/main.c
        src/expr_queue.c
        src/tok_stack.c
        src/expr_tree.c
        src/tok_vector.c
        src/expr_stack.c
        src/printer.c
        src/parser.c
        src/token.c
        src/tokenizer.c
)

target_compile_options(expr PRIVATE
        -Wall
        -Wextra
        -Werror
)

add_custom_target(run_file
        COMMAND expr tests/input.txt
        DEPENDS expr
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
