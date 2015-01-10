cmake_minimum_required(VERSION 2.8.4)
project(minwm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(X11 REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/keygrabber.cpp src/keygrabber.h
    src/windowmanager.cpp src/windowmanager_onEvent.cpp src/windowmanager_onNotify.cpp src/windowmanager.h
    src/display.h
    src/workspace.cpp src/workspace.h
    src/fonthelper.cpp src/fonthelper.h
    src/containers/wmwindow.cpp src/containers/wmwindow.h
    src/containers/wmsplitter.cpp src/containers/wmsplitter.h
    src/containers/wmframe.cpp src/containers/wmframe.h
    src/containers/wmcontainer.cpp src/containers/wmcontainer.h)

add_executable(minwm ${SOURCE_FILES})
target_link_libraries(minwm X11)