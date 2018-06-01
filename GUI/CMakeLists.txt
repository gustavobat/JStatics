# Sets prefix path on different OS.
if(WIN32)
  set(CMAKE_PREFIX_PATH "C:/Qt/5.9.1/msvc2015" ${CMAKE_PREFIX_PATH})
elseif(APPLE)
  set(CMAKE_PREFIX_PATH /usr/local/opt/qt ${CMAKE_PREFIX_PATH})
elseif(UNIX)
  set(CMAKE_PREFIX_PATH /opt/Qt/5.9.1 ${CMAKE_PREFIX_PATH})
endif()
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Find the QtWidgets library
if(WIN32)
	include_directories(BEFORE SYSTEM "C:/Qt/5.9.1/msvc2015/include")
elseif(APPLE)
	include_directories(BEFORE SYSTEM /usr/local/opt/qt/include)
elseif(UNIX)
	include_directories(BEFORE SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
endif()

# UI Files
QT5_WRAP_UI (UISrcs  TMainWindow.ui)
QT5_WRAP_CPP(MOCSrcs TMainWindow.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(../CLI)

file(GLOB headers *.h *.hpp)
file(GLOB sources *.cpp)
file(GLOB data *.json)

file (GLOB to_remove_from_sources main.cpp)
list (REMOVE_ITEM sources ${to_remove_from_sources})

file(GLOB commonHeaders ../*.h)
file(GLOB commonSources ../*.cpp)

set(main_SRCS main.cpp ${sources} ${headers} ${commonHeaders}
    ${commonSources} ${data})

ADD_EXECUTABLE(GUI ${main_SRCS} ${MOCSrcs} ${UISrcs} ${RSrcs} ${QMFiles})

TARGET_LINK_LIBRARIES(GUI Qt5::Core Qt5::Widgets Qt5::Gui ${QT_QTMAIN_LIBRARY}
    pz ${PTHREAD_LIB})

if (WIN32)
    add_custom_command(
	TARGET GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:GUI>
    )
    add_custom_command(
        TARGET GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:GUI>
    )
    add_custom_command(
	TARGET GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:GUI>
    )
endif(WIN32)
