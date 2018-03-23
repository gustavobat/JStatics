cmake_minimum_required(VERSION 2.8.0)

# Sets project name.
project("JStatics")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Location to search files or libraries.
set(SEARCH_DIRS ENV PATH /usr/local/opt/ /usr/include 	/usr/local/include /usr/lib /usr/lib64 /usr/local/lib .. )

find_package(PZ)
include_directories(${PZ_INCLUDE_DIRS})

# Enables Pthread library.
find_path(PTHREAD_INCLUDE
	NAMES PThread.h pthread.h 
	PATHS ${SEARCH_DIRS}
	${CMAKE_SOURCE_DIR}/pthread 
 	${CMAKE_SOURCE_DIR}/externallibs/pthread 
	${CMAKE_SOURCE_DIR}/externallibs/pthread/include)

find_library(PTHREAD_LIB 
	NAMES libpthread.dylib pthreadVC2.lib libPThread.so 			libPThread.a libpthread.so libpthread.a 
	PATHS ${SEARCH_DIRS} 
	${CMAKE_SOURCE_DIR}/pthread/lib/x86
	${CMAKE_SOURCE_DIR}/externallibs/pthread/lib/x86)

if(PTHREAD_INCLUDE-NOTFOUND)
	set(PTHREAD_INCLUDE "" CACHE PATH "Directory where 				Pthread header files can be found")
else()
	include_directories(${PTHREAD_INCLUDE})
endif()

if(PTHREAD_LIB-NOTFOUND)
	set(PTHREAD_LIB "" CACHE PATH "Directory where the 				Pthread library can be found")
endif()

# Adds CLI application subdirectory.
add_subdirectory(CLI)

# Adds GUI application subdirectory.
add_subdirectory(GUI)
