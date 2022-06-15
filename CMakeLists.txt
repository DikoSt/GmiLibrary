cmake_minimum_required(VERSION 3.16)
project(GmiToHtmlConverter)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

add_executable(GmiToHtmlConverter main.cpp 
                                  src/gmi.cpp)
