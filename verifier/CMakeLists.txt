project(Core)

set ( CMAKE_CXX_FLAGS "-g -Wall")

find_package(Boost REQUIRED COMPONENTS program_options)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# The main program
add_executable(Verify main.cpp)
target_link_libraries(Verify PRIVATE ${Boost_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)