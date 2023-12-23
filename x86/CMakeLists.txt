project(x86)

set(CMAKE_CXX_STANDARD 98)

add_executable(x86 main.cpp)

#将子目录下的项目反馈回主项目
target_include_directories(x86 PUBLIC include)