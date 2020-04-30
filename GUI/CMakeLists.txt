set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(JStaticsGUI
    main.cpp
    TMainWindow.ui
    TMainWindow.cpp
    TLoadGraphics.cpp
    TMainWindow.cpp
    TStructureGraphics.cpp
    TSupportGraphics.cpp
    )

target_link_libraries(JStaticsGUI jstatics Qt5::Widgets)
target_include_directories(JStaticsGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
