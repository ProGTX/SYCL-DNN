# Copyright 2018 Codeplay Software Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use these files except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# Helper functions to add targets to SYCL DNN
#
# Supported helper functions:
#   snn_test  - adds a test executable target and registers it with ctest
#   snn_bench - adds a benchmark executable and registers it with ctest
#
# Low level functions:
#   snn_executable - adds an executable target
#
# These helper functions all take named parameters, detais of which can be
# found below. Typically they take a `TARGET` and a list of `SOURCES`, along
# with a list of `PUBLIC_LIBRARIES` and `PUBLIC_INCLUDE_DIRS` to add to the
# target. As not all targets will need to be compiled with SYCL support, there
# is an option `WITH_SYCL` to specify that SYCL support is required. Any
# additional C++ compiler flags required can be added to the target with
# `CXX_OPTS`.
#
cmake_minimum_required(VERSION 3.2.2)
# snn_executable helper function
# Adds an executable target with the specified sources, libraries and include
# directories. If SYCL support is requested then that is added to the target as
# well.
#
# WITH_SYCL: whether to compile the executable for SYCL
# TARGET: name of binary for the target
# SOURCES: sources files for the binary
# PUBLIC_LIBRARIES: targets and flags for linking phase
# PRIVATE_LIBRARIES: targets and flags for linking phase
# PUBLIC_INCLUDE_DIRS: include directories for target
# PRIVATE_INCLUDE_DIRS: include directories for target
# CXX_OPTS: additional compile flags to add to the target
function(snn_executable)
  set(options
    WITH_SYCL
  )
  set(one_value_args
    TARGET
  )
  set(multi_value_args
    SOURCES
    PUBLIC_LIBRARIES
    PRIVATE_LIBRARIES
    PUBLIC_INCLUDE_DIRS
    PRIVATE_INCLUDE_DIRS
    CXX_OPTS
  )
  cmake_parse_arguments(SNN_EXEC
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )
  add_executable(${SNN_EXEC_TARGET} ${SNN_EXEC_SOURCES})
  target_link_libraries(${SNN_EXEC_TARGET}
    PUBLIC  ${SNN_EXEC_PUBLIC_LIBRARIES}
    PRIVATE ${SNN_EXEC_PRIVATE_LIBRARIES}
  )
  target_include_directories(${SNN_EXEC_TARGET}
    PUBLIC  ${SNN_EXEC_PUBLIC_INCLUDE_DIRS}
    PRIVATE ${SNN_EXEC_PRIVATE_INCLUDE_DIRS}
            ${sycldnn_SOURCE_DIR}/include
            ${sycldnn_SOURCE_DIR}
  )
  # Specify some C++11 features used widely across the library
  target_compile_features(${SNN_EXEC_TARGET} PUBLIC
    cxx_auto_type
    cxx_constexpr
    cxx_final
    cxx_lambdas
    cxx_static_assert
  )
  target_compile_options(${SNN_EXEC_TARGET} PRIVATE -Wall -Wextra)
  # The SYCL kernel names are only used when name mangling is turned on.
  # By default we keep kernel name compression off, so these variables are
  # typically not used.
  target_compile_options(${SNN_EXEC_TARGET} PRIVATE -Wno-unused-variable)
  if(SNN_EXEC_CXX_OPTS)
    target_compile_options(${SNN_EXEC_TARGET} PUBLIC ${SNN_EXEC_CXX_OPTS})
  endif()
  if(SNN_EXEC_WITH_SYCL)
    set_property(
      TARGET   ${SNN_EXEC_TARGET}
      PROPERTY COMPUTECPP_INCLUDE_AFTER
    )
    add_sycl_to_target(
      TARGET     ${SNN_EXEC_TARGET}
      BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${SNN_EXEC_TARGET}.dir
      SOURCES    ${SNN_EXEC_SOURCES}
    )
  endif()
endfunction()
# snn_test helper function
# Adds a test target with the specified sources, libraries and include
# directories. If SYCL support is requested then that is added to the target as
# well.
#
# parameters
# WITH_SYCL: whether to compile the test for SYCL
# TARGET: target name prefix
# SOURCES: sources files for the tests
# PUBLIC_LIBRARIES: targets and flags for linking phase
# PUBLIC_INCLUDE_DIRS: include directories for target
# CXX_OPTS: additional compile flags to add to the target
function(snn_test)
  set(options
    WITH_SYCL
  )
  set(one_value_args
    TARGET
  )
  set(multi_value_args
    SOURCES
    PUBLIC_LIBRARIES
    PUBLIC_INCLUDE_DIRS
    CXX_OPTS
  )
  cmake_parse_arguments(SNN_TEST
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )
  if(${SNN_TEST_WITH_SYCL})
    set(_WITH_SYCL WITH_SYCL)
  endif()
  message(STATUS "Test target: ${SNN_TEST_TARGET}")
  set(_NAME ${SNN_TEST_TARGET}_test)

  snn_executable(
    ${_WITH_SYCL}
    TARGET               ${_NAME}_bin
    SOURCES              ${SNN_TEST_SOURCES}
    PUBLIC_LIBRARIES     ${SNN_TEST_PUBLIC_LIBRARIES}
    PRIVATE_LIBRARIES    GTest::GTest GTest::Main
    PUBLIC_INCLUDE_DIRS  ${SNN_TEST_PUBLIC_INCLUDE_DIRS}
    PRIVATE_INCLUDE_DIRS ${sycldnn_SOURCE_DIR}/test
    CXX_OPTS             ${SNN_TEST_CXX_OPTS}
  )
  add_test(NAME ${_NAME} COMMAND ${_NAME}_bin --gtest_output=xml:output/)
endfunction()
# snn_bench helper function
# Adds a benchmark target with the specified sources, libraries and include
# directories. If SYCL support is requested then that is added to the target as
# well.
#
# parameters
# WITH_SYCL: whether to compile the benchmark for SYCL
# TARGET: target name prefix
# SOURCES: sources files for the tests
# PUBLIC_LIBRARIES: targets and flags for linking phase
# PUBLIC_INCLUDE_DIRS: include directories for target
# CXX_OPTS: additional compile flags to add to the target
function(snn_bench)
  set(options
    WITH_SYCL
  )
  set(one_value_args
    TARGET
  )
  set(multi_value_args
    SOURCES
    PUBLIC_LIBRARIES
    PUBLIC_INCLUDE_DIRS
    CXX_OPTS
  )
  cmake_parse_arguments(SNN_BENCH
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )
  message(STATUS "Bench target: ${SNN_BENCH_TARGET}")
  set(_NAME ${SNN_BENCH_TARGET}_bench)
  if(${SNN_BENCH_WITH_SYCL})
    set(_WITH_SYCL WITH_SYCL)
  endif()
  snn_executable(
    ${_WITH_SYCL}
    TARGET               ${_NAME}_bin
    SOURCES              ${SNN_BENCH_SOURCES}
    PUBLIC_LIBRARIES     ${SNN_BENCH_PUBLIC_LIBRARIES}
    PRIVATE_LIBRARIES    benchmark::benchmark
    PUBLIC_INCLUDE_DIRS  ${SNN_BENCH_PUBLIC_INCLUDE_DIRS}
    PRIVATE_INCLUDE_DIRS ${sycldnn_SOURCE_DIR}/bench
    CXX_OPTS             ${SNN_TEST_CXX_OPTS}
  )
  add_test(
    NAME           ${_NAME}
    COMMAND        ${_NAME}_bin --benchmark_out=output/${_NAME}.csv
                                --benchmark_out_format=csv
    CONFIGURATIONS Benchmark
  )
  # Ensure that the benchmark output directory is made
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
endfunction()