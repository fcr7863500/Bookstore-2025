cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cleanup src/cleanup.cpp)
target_include_directories(cleanup PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(code src/bookmanager.cpp src/cmdparser.cpp src/logmanager.cpp src/usermanager.cpp src/main.cpp)
target_include_directories(code PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
