include_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(CLI main.cpp InputJSON.json)
target_link_libraries(CLI jstatics pz)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
configure_file(InputJSON.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
