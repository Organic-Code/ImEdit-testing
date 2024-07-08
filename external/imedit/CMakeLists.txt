set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download imedit
FetchContent_Populate(
        imedit
        SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imedit-src/imedit"
        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imedit-build/imedit"
        SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imedit-subbuild/imedit"
        GIT_REPOSITORY "https://github.com/Organic-Code/ImEdit"
        GIT_TAG "master"
        GIT_PROGRESS ON
        GIT_SHALLOW ON
)

# Declare imgui
add_library(imedit STATIC)
add_library(imedit::imedit ALIAS imedit)

# Add sources
file(
        GLOB sources
        "${imedit_SOURCE_DIR}/*.cpp"
        "${imedit_SOURCE_DIR}/*.h"
)
target_sources(imedit PRIVATE ${sources})

# Add includes
target_include_directories(
        imedit PUBLIC
        "${imedit_SOURCE_DIR}/.."
)

# Add dependencies
target_link_libraries(
        imedit PUBLIC
        imgui::imgui
)
