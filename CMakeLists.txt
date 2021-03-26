cmake_minimum_required(VERSION 3.12)

project(OpenInjector CXX)

#including sandboxed api library
set(CMAKE_CXX_STANDARD 17)
set(SAPI_ROOT "${PROJECT_SOURCE_DIR}/sandboxed-api"
      CACHE PATH "Path to the Sandboxed API source tree")

  # Configure options and include Sandboxed API as a sub-directory.
  set(SAPI_ENABLE_TESTS OFF CACHE BOOL "")
  add_subdirectory("${SAPI_ROOT}"
                   "${CMAKE_BINARY_DIR}/sandboxed-api-build" EXCLUDE_FROM_ALL)

include_directories(include)

add_library(binary include/binary.h src/binary.cpp)
add_library(injector include/injector.h src/injector.cpp)
add_library(statistics include/statistics.h src/statistics.cpp)
add_library(tester include/tester.h src/tester.cpp)

target_link_libraries(tester PRIVATE
		absl::memory
		sandbox2::bpf_helper
		sapi::runfiles
		sandbox2::sandbox2
		sapi::base
		sapi::flags
		)
add_executable(openinjector ../main.cpp)
target_link_libraries(openinjector PRIVATE
		binary
		tester
		statistics
		injector
		)
