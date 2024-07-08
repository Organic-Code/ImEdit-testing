#include <imgui_te_engine.h>
#include <imgui_te_context.h>

#include <imedit/editor.h>

namespace {
    void set_data_ascii(ImEdit::editor &ed) {
        ed.clear();
        ed.set_data("#include <iostream>\n"
                    "\n"
                    "int main(int argc, char* argv[]) {\n"
                    "    std::cout << \"Hello World\" << std::endl;\n"
                    "    return 0;\n"
                    "}\n"
        );
    }

    constexpr const char* scm_name = "Single cursor move";
    constexpr const char* mcm_name = "Multi cursor move";
    constexpr const char* window_name = "cursor_movements_tests";
    constexpr const char* editor_name = "editor";

    ImEdit::editor editor(editor_name);
}


void add_cursor_movement_tests(ImGuiTestEngine* engine) {

    auto test = IM_REGISTER_TEST(engine, scm_name, "arrow key navigation within ASCII text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        ImGui::SetNextWindowSize({500.f, 250.f});
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_ascii(editor);
        editor.set_cursor({3, 4, 2});
        IM_CHECK(editor.has_cursor({3, 4, 2}));

        // basic moves
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({2, 4, 2}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({3, 4, 2}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({4, 2, 1}));
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({3, 4, 2}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({3, 4, 1}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({3, 4, 2}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({3, 4, 3}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({3, 4, 2}));

        // next token and previous token
        ctx->KeyPress(ImGuiKey_RightArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 5, 0}));
        ctx->KeyPress(ImGuiKey_RightArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 6, 0}));

        editor.set_cursor({3, 4, 2});
        ctx->KeyPress(ImGuiKey_LeftArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 4, 0}));
        ctx->KeyPress(ImGuiKey_LeftArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 3, 0}));

        // next line and previous line using right and left arrows
        editor.set_cursor({3, 0, 0});
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({2, 17, 1}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({3, 0, 0}));

        // end line / beg line
        ctx->KeyPress(ImGuiKey_End);
        IM_CHECK(editor.has_cursor({3, 20, 1}));
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({3, 1, 0})); // Home sets the cursor position at the beginning of the line, but after the leading spaces
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({3, 0, 0}));
    };
    // TODO same for UTF-8

    // TODO cursor pos when inputting text ? (maybe test that elsewhere)
    // TODO on cursor click
}