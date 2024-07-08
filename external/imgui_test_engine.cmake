set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET imgui_te::imgui_te)
    return()
endif()

message(CHECK_START "external: configuring imgui test engine")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui.cmake)

# Custom CMake project
add_subdirectory(
        "${CMAKE_CURRENT_LIST_DIR}/imgui_test_engine"
        "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui_te-build"
        EXCLUDE_FROM_ALL
)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
