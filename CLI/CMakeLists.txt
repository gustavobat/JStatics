file(GLOB headers *.h *.hpp)
file(GLOB sources *.cpp)
file(GLOB data *.json)

set(main_SRCS ${sources} ${headers} ${data})
add_executable(CLI ${main_SRCS})
target_link_libraries(CLI pz ${PTHREAD_LIB})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
configure_file(InputJSON.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
