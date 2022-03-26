cmake_minimum_required(VERSION 3.20.2)
project(mmn14 C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -ansi -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


add_executable(assembler
        common.c
        common.h
        code.h
        compilation.c
        compilation.h
        test_funcs.h
        test_funcs.c
        main.c
        code.c
        artifacts.c
        precompilation.c
        precompilation.h structs.h)

#set_target_properties(assembler PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

