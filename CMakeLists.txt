# define the minimum cmake version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# define the project
project(cpp_project LANGUAGES CXX)

# set cpp version
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# CGAL performance warning
set(CMAKE_BUILD_TYPE "Release")

# enable sanitization
include(Sanitizers.cmake)

# define options
option(ENABLE_TEST "Enable the test files to build. Default: false" false)
option(ENABLE_COVERAGE "Enable coverage for tests. Default: false" false)

if (ENABLE_TEST)
  # find catch2 lib
  find_package(Catch2 REQUIRED)

  # test_maze files
  set(test_maze_headers ${CMAKE_SOURCE_DIR}/include/maze.hpp)
  set(test_maze_sources ${CMAKE_SOURCE_DIR}/test/test_maze.cpp)

  # test_generator files
  set(test_generator_headers ${CMAKE_SOURCE_DIR}/include/generator.hpp)
  set(test_generator_sources ${CMAKE_SOURCE_DIR}/test/test_generator.cpp)

  # test_solver files
  set(test_solver_headers ${CMAKE_SOURCE_DIR}/include/solver.hpp)
  set(test_solver_sources ${CMAKE_SOURCE_DIR}/test/test_solver.cpp)

  if(ENABLE_COVERAGE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type." FORCE)

    # include the code coverage cmake file
    include(CodeCoverage.cmake)

    # use the provided function to adjust compiler flags
    APPEND_COVERAGE_COMPILER_FLAGS()
  endif()

  add_executable(test_maze ${test_maze_headers} ${test_maze_sources})
  add_executable(test_generator ${test_generator_headers} ${test_generator_sources})
  add_executable(test_solver ${test_solver_headers} ${test_solver_sources})

  target_link_libraries(test_maze Catch2::Catch2)
  target_link_libraries(test_generator Catch2::Catch2)
  target_link_libraries(test_solver Catch2::Catch2)

  if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

    # create a new target for code coverage, inspired from slides provided by prof Adams
    SETUP_TARGET_FOR_COVERAGE_LCOV(
      NAME test_maze_coverage
      EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/test_maze
      LCOV_ARGS --rc lcov_branch_coverage=1
      GENHTML_ARGS --legend --branch-coverage
      DEPENDENCIES test_maze)

    # create a new target for code coverage, inspired from slides provided by prof Adams
    SETUP_TARGET_FOR_COVERAGE_LCOV(
      NAME test_generator_coverage
      EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/test_generator
      LCOV_ARGS --rc lcov_branch_coverage=1
      GENHTML_ARGS --legend --branch-coverage
      DEPENDENCIES test_generator)

    # create a new target for code coverage, inspired from slides provided by prof Adams
    SETUP_TARGET_FOR_COVERAGE_LCOV(
      NAME test_solver_coverage
      EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/test_solver
      LCOV_ARGS --rc lcov_branch_coverage=1
      GENHTML_ARGS --legend --branch-coverage
      DEPENDENCIES test_solver)
  endif()
endif()

# maze_gen files
set(maze_gen_headers ${CMAKE_SOURCE_DIR}/include/maze.hpp ${CMAKE_SOURCE_DIR}/include/generator.hpp)
set(maze_gen_sources ${CMAKE_SOURCE_DIR}/app/maze_gen.cpp)

# maze_solve files
set(maze_solve_headers ${CMAKE_SOURCE_DIR}/include/maze.hpp ${CMAKE_SOURCE_DIR}/include/solver.hpp)
set(maze_solve_sources ${CMAKE_SOURCE_DIR}/app/maze_solve.cpp)

# add the exexutables
add_executable(maze_gen ${maze_gen_headers} ${maze_gen_sources})
add_executable(maze_solve ${maze_solve_headers} ${maze_solve_sources})

# install the main executables
install(TARGETS maze_gen maze_solve DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

#install the demo script
install(PROGRAMS demo DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)