cmake_minimum_required(VERSION 2.8.4)
project(tofuwm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(X11 REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCE_FILES
    src/main.cpp
    src/windowmanager/windowmanager.cpp src/windowmanager/windowmanager.h
    src/windowmanager/windowmanager_debug.cpp
    src/windowmanager/windowmanager_init.cpp
    src/windowmanager/windowmanager_eventloop.cpp
    src/windowmanager/windowmanager_onRequest.cpp
    src/windowmanager/windowmanager_onInput.cpp
    src/windowmanager/windowmanager_onNotify.cpp
    src/keygrabber.cpp src/keygrabber.h
    src/display.h
    src/workspace.cpp src/workspace.h
    src/fonthelper.cpp src/fonthelper.h
    src/containers/wmwindow.cpp src/containers/wmwindow.h
    src/containers/wmsplitter.cpp src/containers/wmsplitter.h
    src/containers/wmframe.cpp src/containers/wmframe.h
    src/containers/wmcontainer.cpp src/containers/wmcontainer.h)

add_executable(tofuwm ${SOURCE_FILES})
target_link_libraries(tofuwm X11)
