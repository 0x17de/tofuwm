cmake_minimum_required(VERSION 2.8.4)
project(minwm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(X11 REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/keygrabber.cpp src/keygrabber.h
    src/windowmanager.cpp src/windowmanager.h
    src/display.h
    src/wmwindow.cpp src/wmwindow.h
    src/workspace.cpp src/workspace.h)

add_executable(minwm ${SOURCE_FILES})
target_link_libraries(minwm X11)