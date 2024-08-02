cmake_minimum_required(VERSION 3.5)

project(QtMem0ry VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS_DEBUG "-O0")
#set(CMAKE_C_FLAGS_DEBUG "-O0")


set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_C_FLAGS "-O0")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
find_package(CURL REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtMem0ry
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        gamescene.h gamescene.cpp



        util.h util.cpp
        downloadhandler.h downloadhandler.cpp
        memorypanel.h memorypanel.cpp
        memorycard.h memorycard.cpp
        animatedwidget.h animatedwidget.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        keyvaluetablemodel.h keyvaluetablemodel.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtMem0ry APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtMem0ry SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtMem0ry
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QtMem0ry PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network ${CURL_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QtMem0ry)
endif()
set_target_properties(QtMem0ry PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QtMem0ry
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtMem0ry)
endif()
