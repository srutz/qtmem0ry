cmake_minimum_required(VERSION 3.5)

project(QtMem0ry VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_C_FLAGS "-O0")


find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(CURL REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    util.h util.cpp
    downloadhandler.h downloadhandler.cpp
    memorypanel.h memorypanel.cpp
    memorycard.h memorycard.cpp
    animatedwidget.h animatedwidget.cpp
    aboutdialog.h aboutdialog.cpp aboutdialog.ui
    keyvaluetablemodel.h keyvaluetablemodel.cpp
    resources.qrc
)

qt_add_executable(QtMem0ry 
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(QtMem0ry PRIVATE Qt6::Widgets Qt6::Network ${CURL_LIBRARIES})

qt_finalize_executable(QtMem0ry)
