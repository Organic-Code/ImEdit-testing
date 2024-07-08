
#include <imgui.h>
#include <imgui_te_engine.h>
#include <imgui_te_exporters.h>
#include <imgui_te_ui.h>
#include <imgui_te_context.h>
#include <iostream>

// imgui_app (this is a helper to wrap multiple backends)
#include "imgui_app.h"
#include "imedit/editor.h"

std::string as_string(ImEdit::editor& ed) {
    std::string str;
    std::copy(ed.begin(), ed.end(), std::back_inserter(str));
    return str;
}

int main(int, char*[])
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    constexpr const char * editor_name = "editor";
    constexpr const char * editor_window_name = "Editor";

    ImEdit::editor editor(editor_name);
    editor._width = editor._height = 800;

    ImGuiTestEngine* engine = ImGuiTestEngine_CreateContext();
    ImGuiApp* window = ImGuiApp_ImplDefault_Create();
    window->InitCreateWindow(window, "ImEdit testing", ImVec2(1440, 900));
    window->InitBackends(window);

    ImGuiTestEngine_GetIO(engine).ConfigRunSpeed = ImGuiTestRunSpeed_Cinematic;

    auto test = IM_REGISTER_TEST(engine, "test", "test1");
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        editor.clear();
        ctx->SetRef(editor_window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);
        ctx->KeyChars("auie");
        IM_CHECK_STR_EQ(as_string(editor).c_str(), "auie");
    };

    // Start engine
    ImGuiTestEngine_Start(engine, ImGui::GetCurrentContext());
    ImGuiTestEngine_InstallDefaultCrashHandler();
    while (window->NewFrame(window))
    {

        ImGui::NewFrame();
        ImGuiTestEngine_ShowTestEngineWindows(engine, nullptr);

        if (ImGui::Begin(editor_window_name)) {
            if (ImGui::Button("button_name")) {
                std::cout << "Button tested" << std::endl;
            }
            editor.render();
        }
        ImGui::End();
        ImGui::Render();

        window->ClearColor = window->ClearColor;
        window->Render(window);

        // Post-swap handler is REQUIRED in order to support screen capture
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
