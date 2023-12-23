cmake_minimum_required(VERSION 3.23)

project(c-basics)

set(CMAKE_CXX_STANDARD 98)

add_executable(c-basics c-grammar/c-study.cpp include/c-grammar/c-study.h)

#将子目录下的项目反馈回主项目
target_include_directories(c-basics PUBLIC include)