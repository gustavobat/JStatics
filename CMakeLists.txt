cmake_minimum_required(VERSION 2.8.0)

project("JStatics")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Location to search files or libraries.
set(SEARCH_DIRS ENV PATH /usr/local/opt/ /usr/include /usr/local/include
  /usr/lib /usr/lib64 /usr/local/lib .. )


# Finds NeoPZ library.
if(WIN32)
  # Prevents timespec redefinition problem with Visual Studio 2015
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_STRUCT_TIMESPEC")
  find_package( PZ PATHS "C:/Arquivos de Programas/PZ" REQUIRED NO_DEFAULT_PATH)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
  find_package( PZ REQUIRED )
endif()

if(APPLE)
  # Mac OS X specific code
  add_definitions(-DMACOSX)
  set(MACOSX MACOSX)
  FIND_LIBRARY(ACCELERATE_LIB Accelerate)
  link_libraries(${ACCELERATE_LIB})
  #   SET(OperatingSystem "Mac OS X")
endif()

include_directories(${PZ_INCLUDE_DIRS})

if(NEOPZ_LIB-NOTFOUND)
  set(NEOPZ_LIB "" CACHE PATH "Directory where NeoPZ library can be found")
endif()


# Finds Pthread library.
find_path(PTHREAD_INCLUDE
  NAMES PThread.h pthread.h
  PATHS ${SEARCH_DIRS}
  ${CMAKE_SOURCE_DIR}/pthread
  ${CMAKE_SOURCE_DIR}/externallibs/pthread
  ${CMAKE_SOURCE_DIR}/externallibs/pthread/include)

find_library(PTHREAD_LIB
  NAMES libpthread.dylib pthreadVC2.lib libPThread.so libPThread.a libpthread.so libpthread.a
  PATHS ${SEARCH_DIRS}
  ${CMAKE_SOURCE_DIR}/pthread/lib/x86
  ${CMAKE_SOURCE_DIR}/externallibs/pthread/lib/x86)

if(PTHREAD_INCLUDE-NOTFOUND)
  set(PTHREAD_INCLUDE "" CACHE PATH "Directory where Pthread header files can be found")
else()
  include_directories(${PTHREAD_INCLUDE})
endif()

if(PTHREAD_LIB-NOTFOUND)
  set(PTHREAD_LIB "" CACHE PATH "Directory where the Pthread library can be found")
endif()


add_subdirectory(lib)
add_subdirectory(CLI)
add_subdirectory(GUI)
