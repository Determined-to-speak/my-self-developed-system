project(x86_64)

set(CMAKE_CXX_STANDARD 98)

add_executable(x86_64 main.cpp)

#将子目录下的项目反馈回主项目
target_include_directories(x86_64 PUBLIC include)