project(SAT_tests)

set(TEST_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/logic_tests/Variable_Tests.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/logic_tests/Clause_Tests.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/logic_tests/Literal_Tests.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/heuristic_tests/BacktrackingHeuristics_Tests.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/heuristic_tests/BranchingHeuristics_Tests.cpp
                Solver_Tests.cpp)

enable_testing()
find_package(GTest REQUIRED)

add_executable(Test ${TEST_FILES})

target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/heuristic_tests/)
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/logic_tests/)

target_link_libraries(Test PRIVATE core_lib GTest::GTest GTest::Main)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data/ DESTINATION ${CMAKE_SOURCE_DIR}/build/test_data/)