project(Core)

# All sources that also need to be tested in unit tests go into a static library
add_library(core_lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/logic/AtomicProposition.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/logic/AtomicProposition.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/logic/Variable.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/logic/Variable.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/logic/Clause.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/logic/Clause.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/solver/Solver.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/solver/Solver.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/ProgramOptions.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/heuristics/BacktrackingHeuristics.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/heuristics/BacktrackingHeuristics.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/heuristics/BranchingHeuristics.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/heuristics/BranchingHeuristics.cpp)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/solver)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/logic)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/heuristics)

find_package(Boost REQUIRED COMPONENTS program_options)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# The main program
add_executable(SAT main.cpp)
target_link_libraries(SAT PRIVATE core_lib ${Boost_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)