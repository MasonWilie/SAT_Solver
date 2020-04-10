project(Core)

set ( CMAKE_CXX_FLAGS "-g -Wall")

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

                            ${CMAKE_CURRENT_SOURCE_DIR}/backtracking_heuristics/BacktrackingHeuristics.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/backtracking_heuristics/BacktrackingHeuristics.cpp

                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/BranchingHeuristic.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/JeroslowWang.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/JeroslowWang.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/MOM.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/MOM.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/MaxMinClause.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/MaxMinClause.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/Random.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/Random.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/VSIDS.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/VSIDS.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/BOHM.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics/BOHM.h)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/solver)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/logic)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backtracking_heuristics)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/branching_heuristics)

find_package(Boost REQUIRED COMPONENTS program_options)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# The main program
add_executable(Solve main.cpp)
target_link_libraries(Solve PRIVATE core_lib ${Boost_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)