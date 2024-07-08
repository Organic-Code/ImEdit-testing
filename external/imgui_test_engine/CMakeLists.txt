set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download imgui
FetchContent_Populate(
  imgui_te
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui_te-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui_te-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui_te-subbuild"
  GIT_REPOSITORY "https://github.com/ocornut/imgui_test_engine"
  GIT_TAG "v1.90.8"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Declare imgui
add_library(imgui_te STATIC)
add_library(imgui_te::imgui_te ALIAS imgui_te)

# Add sources
file(
  GLOB sources
  "${imgui_te_SOURCE_DIR}/imgui_test_engine/*.cpp"
  "${imgui_te_SOURCE_DIR}/imgui_test_engine/*.h"
  "${imgui_te_SOURCE_DIR}/imgui_test_engine/thirdparty/Str/*.h"
  "${imgui_te_SOURCE_DIR}/imgui_test_engine/thirparty/stb/*.h"
  "${imgui_te_SOURCE_DIR}/shared/*.cpp"
  "${imgui_te_SOURCE_DIR}/shared/*.h"
)
target_sources(imgui_te PRIVATE ${sources})

# Add includes
target_include_directories(
  imgui_te SYSTEM PUBLIC
  "${imgui_te_SOURCE_DIR}/imgui_test_engine"
  "${imgui_te_SOURCE_DIR}/shared"
)

# Add dependencies
target_link_libraries(
  imgui_te PUBLIC
  imgui::imgui
)

find_package(SDL2 REQUIRED)
target_include_directories(
  imgui_te SYSTEM PRIVATE
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(imgui_te PRIVATE ${SDL2_LIBRARIES})

# Add definitions
target_compile_definitions(
  imgui_te PUBLIC
  IMGUI_TEST_ENGINE_ENABLE_STD_FUNCTION=1
  IMGUI_TEST_ENGINE_ENABLE_IMPLOT=0
  IMGUI_TEST_ENGINE_ENABLE_COROUTINE_STDTHREAD_IMPL=1
  IMGUI_TEST_ENGINE_ENABLE_CAPTURE=0
  IMGUI_APP_SDL2_GL3
)
