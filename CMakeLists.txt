cmake_minimum_required(VERSION 2.8.0)

# Sets project name.
project("JStatics")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Location to search files or libraries.
set(SEARCH_DIRS ENV PATH /usr/local/opt/ /usr/include 	/usr/local/include /usr/lib /usr/lib64 /usr/local/lib .. )

# Enables NeoPZ library.
find_path(NEOPZ_INCLUDE pzmatrix.h 
	PATHS ${SEARCH_DIRS} 
	${CMAKE_SOURCE_DIR}/externallibs/pz/include)
find_library(NEOPZ_LIB 
	NAMES libpz.a pz.lib 
	PATHS ${SEARCH_DIRS} 	${CMAKE_SOURCE_DIR}/externallibs/pz/lib)

if(NEOPZ_INCLUDE-NOTFOUND)
	set(NEOPZ_INCLUDE "" CACHE PATH "Directory where NeoPZ 			header files can be found")
else()
    include_directories(
	"/home/gustavo/Documents/pzlib/include/"
	"/home/gustavo/Documents/pzlib/include/Python"
	"/home/gustavo/Documents/pzlib/include/Util"
	"/home/gustavo/Documents/pzlib/include/PerfUtil"
	"/home/gustavo/Documents/pzlib/include/Common"
	"/home/gustavo/Documents/pzlib/include/Save"
	"/home/gustavo/Documents/pzlib/include/Matrix"
	"/home/gustavo/Documents/pzlib/include/Topology"
	"/home/gustavo/Documents/pzlib/include/Geom"
	"/home/gustavo/Documents/pzlib/include/SpecialMaps"
	"/home/gustavo/Documents/pzlib/include/Refine"
	"/home/gustavo/Documents/pzlib/include/Shape"
	"/home/gustavo/Documents/pzlib/include/Material"
	"/home/gustavo/Documents/pzlib/include/Material/REAL"
	"/home/gustavo/Documents/pzlib/include/Material/REAL/Plasticity"
	"/home/gustavo/Documents/pzlib/include/Material/Complex"
	"/home/gustavo/Documents/pzlib/include/Multigrid"
	"/home/gustavo/Documents/pzlib/include/Mesh"
	"/home/gustavo/Documents/pzlib/include/Mesh/REAL"
	"/home/gustavo/Documents/pzlib/include/StrMatrix"
	"/home/gustavo/Documents/pzlib/include/Integral"
	"/home/gustavo/Documents/pzlib/include/Frontal"
	"/home/gustavo/Documents/pzlib/include/Optimization"
	"/home/gustavo/Documents/pzlib/include/Pre"
	"/home/gustavo/Documents/pzlib/include/Post"
	"/home/gustavo/Documents/pzlib/include/Random"
	"/home/gustavo/Documents/pzlib/include/Analysis"
	"/home/gustavo/Documents/pzlib/include/SubStruct"
	"/home/gustavo/Documents/pzlib/include/LinearSolvers"
	"/home/gustavo/Documents/pzlib/include/External"
	"/home/gustavo/Documents/pzlib/include/External/sloan"
	"/home/gustavo/Documents/pzlib/include/Publications"
)
endif()

if(NEOPZ_LIB-NOTFOUND)
	set(NEOPZ_LIB "" CACHE PATH "Directory where NeoPZ 				library can be found")
endif()

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
