add_executable(JStaticsCLI main.cpp)
target_link_libraries(JStaticsCLI jstatics)

configure_file(InputJSON.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
