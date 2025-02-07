#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
if(NOT global_generated_files_list_inited)
    set(global_generated_files_list_inited ON)
    set(global_generated_files_list "" CACHE INTERNAL "" FORCE)
endif()

cmake_minimum_required(VERSION 3.25)
project(
  ImEdit_testing
  DESCRIPTION "Unit testing for ImEdit"
  LANGUAGES CXX
)
include(FetchContent)

# Set CMake modules path
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

# Flags module
include(flags)

# setup sanitize build types
string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
if(build_type STREQUAL "ASAN")
    setup_asan_flags()
elseif(build_type STREQUAL "UBSAN")
    setup_ubsan_flags()
elseif(build_type STREQUAL "TSAN")
    setup_tsan_flags()
elseif(build_type STREQUAL "SANITIZE")
    setup_sanitize_flags()
endif()

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Use strict C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for external tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(imgui)
include(imedit)
include(imgui_test_engine)

# Declare target
add_executable(ImEdit_testing)

# Get sources
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Add sources
target_sources(
  ImEdit_testing PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  ImEdit_testing PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link dependencies
target_link_libraries(
  ImEdit_testing PRIVATE
  imgui::imgui
  imgui_te::imgui_te
  imedit::imedit
)

# Build in C++17
target_compile_features(ImEdit_testing PRIVATE cxx_std_17)

# Add warning flags
target_add_cxx_warning_flags(ImEdit_testing)
