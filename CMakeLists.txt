cmake_minimum_required(VERSION 3.12)

project(OpenInjector CXX)
include_directories(include)
set(SAPI_ROOT "${PROJECT_SOURCE_DIR}/sandboxed-api/")
add_subdirectory("${SAPI_ROOT}"
		"${CMAKE_BINARY_DIR}/sandboxed-api-build" EXCLUDE_FROM_ALL)
set(SAPI_ENABLE_EXAMPLES OFF CACHE BOOL "")
set(SAPI_ENABLE_TESTS OFF CACHE BOOL "")
add_library(binary include/binary.h src/binary.cpp)
add_library(injector include/injector.h src/injector.cpp)
add_library(statistics include/statistics.h src/statistics.cpp)
add_library(tester STATIC include/tester.h src/tester.cpp)
target_link_libraries(tester PRIVATE
		absl::memory
		sandbox2::bpf_helper
		sandbox2::comms
		sandbox2::runfiles
		sandbox2::sandbox2
		sapi::base
		sapi::flags)
add_sapi_library(tester-sapi
		SOURCES include/tester.h
		FUNCTIONS test
		INPUTS src/tester.cpp
		LIBRARY tester
		LIBRARY_NAME Tester
		NAMESPACE ""
		)
add_executable(openinjector main.cpp)
target_link_libraries(openinjector PRIVATE
		tester-sapi
		binary
		tester
		statistics
		injector
		sapi::flags
		)
