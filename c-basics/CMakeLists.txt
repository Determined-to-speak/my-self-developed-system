cmake_minimum_required(VERSION 3.23)

project(c-basics)

set(CMAKE_CXX_STANDARD 98)

add_executable(c-basics c-grammar/c-study.cpp c-grammar/c-study.h ../lib/string.c ../include/string.h ../include/asm/io.h ../include/asm/system.h ../include/linux/types.h ../x86/init/main.c)