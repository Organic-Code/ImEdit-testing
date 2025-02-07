
#include <imgui.h>
#include <imgui_te_engine.h>
#include <imgui_te_exporters.h>
#include <imgui_te_ui.h>
#include <imgui_te_context.h>
#include <imgui_te_utils.h>
#include <iostream>

#include "imgui_app.h"
#include "imedit/editor.h"

void add_cursor_movement_tests(ImGuiTestEngine* engine);
void add_text_edition_tests(ImGuiTestEngine* engine);

void add_tests(ImGuiTestEngine* engine) {
    add_cursor_movement_tests(engine);
    add_text_edition_tests(engine);
}

int main(int, char*[])
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiTestEngine* engine = ImGuiTestEngine_CreateContext();
    ImGuiApp* window = ImGuiApp_ImplDefault_Create();
    window->InitCreateWindow(window, "ImEdit testing", ImVec2(1440, 900));
    window->InitBackends(window);

    ImGuiTestEngine_GetIO(engine).ConfigRunSpeed = ImGuiTestRunSpeed_Normal;
    ImGuiTestEngine_GetIO(engine).ConfigBreakOnError = ImOsIsDebuggerPresent();
    ImGuiTestEngine_GetIO(engine).ScreenCaptureFunc = ImGuiApp_ScreenCaptureFunc;
    ImGuiTestEngine_GetIO(engine).ScreenCaptureUserData = window;

    add_tests(engine);

    // Start engine
    ImGuiTestEngine_Start(engine, ImGui::GetCurrentContext());
    ImGuiTestEngine_InstallDefaultCrashHandler();

    while (window->NewFrame(window))
    {

        ImGui::NewFrame();
        ImGuiTestEngine_ShowTestEngineWindows(engine, nullptr);
        ImGui::Render();

        window->Vsync = !ImGuiTestEngine_GetIO(engine).IsRequestingMaxAppSpeed;
        window->ClearColor = window->ClearColor;
        window->Render(window);

        ImGuiTestEngine_PostSwap(engine);
    }

    ImGuiTestEngine_Stop(engine);

    // Print results (command-line mode)

    int count_tested = 0;
    int count_success = 0;
    ImGuiTestEngine_GetResult(engine, count_tested, count_success);
    ImGuiTestEngine_PrintResultSummary(engine);

    // Shutdown Application Window
    window->ShutdownBackends(window);
    window->ShutdownCloseWindow(window);

    // Shutdown
    // IMPORTANT: we need to destroy the Dear ImGui context BEFORE the test engine context, so .ini data may be saved.
    ImGui::DestroyContext();
    ImGuiTestEngine_DestroyContext(engine);
    window->Destroy(window);
}
