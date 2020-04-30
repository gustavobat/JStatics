add_library(jstatics
    JSONIntegration.cpp
    TDistributedLoad.cpp
    TElement.cpp
    TElementEndMoment.cpp
    TMaterial.cpp
    TNodalLoad.cpp
    TNode.cpp
    TStructure.cpp
    TSupport.cpp
    )

# Finds NeoPZ library
find_package(PZ REQUIRED)

target_link_libraries(jstatics pz)
target_include_directories(jstatics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PZ_INCLUDE_DIRS})
