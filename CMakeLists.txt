cmake_minimum_required(VERSION 3.6)

# The project name
project(SAT_Solver)

add_subdirectory(solver)
add_subdirectory(verifier)
add_subdirectory(test)

file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_SOURCE_DIR}/build/data/)