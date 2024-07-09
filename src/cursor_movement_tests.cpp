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
                    "}"
        );
    }

    void set_data_utf8(ImEdit::editor &ed) {
        ed.clear();
        ed.set_data("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                    "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᚻᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                    "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n"
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
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
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

        // going up while at first lines goes to first char
        ctx->KeyPress(ImGuiKey_UpArrow, 4);
        IM_CHECK(editor.has_cursor({0, 0, 0}));

        // going down while at last line goes to last char
        ctx->KeyPress(ImGuiKey_DownArrow, 6);
        IM_CHECK(editor.has_cursor({5, 0, 1}));

        // end file, beg file
        ctx->KeyPress(ImGuiKey_Home | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({0, 0, 0}));
        ctx->KeyPress(ImGuiKey_End | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({5, 0, 1}));

        // next line and previous line using right and left arrows
        editor.set_cursor({3, 0, 0});
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({2, 17, 1}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({3, 0, 0}));

        // end line / beg line
        ctx->KeyPress(ImGuiKey_End);
        IM_CHECK(editor.has_cursor({3, 22, 1}));
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({3, 1, 0})); // Home sets the cursor position at the beginning of the line, but after the leading spaces
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({3, 0, 0}));
    };

    test = IM_REGISTER_TEST(engine, scm_name, "arrow key navigation within UTF-8 text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_utf8(editor);
        editor.set_cursor({1, 6, 6});
        IM_CHECK(editor.has_cursor({1, 6, 6}));

        // basic moves
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({0, 6, 12}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({1, 6, 6}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({2, 8, 9}));
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({1, 6, 6}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({1, 6, 3}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({1, 6, 6}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({1, 6, 9}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({1, 6, 6}));

        // next token and previous token
        ctx->KeyPress(ImGuiKey_RightArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({1, 7, 0}));
        ctx->KeyPress(ImGuiKey_RightArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({1, 8, 0}));

        editor.set_cursor({1, 6, 6});
        ctx->KeyPress(ImGuiKey_LeftArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({1, 6, 0}));
        ctx->KeyPress(ImGuiKey_LeftArrow | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({1, 5, 0}));

        // next line and previous line using right and left arrows
        editor.set_cursor({1, 0, 0});
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({0, 8, 27}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({1, 0, 0}));

        // end line / beg line
        ctx->KeyPress(ImGuiKey_End);
        IM_CHECK(editor.has_cursor({1, 12, 15}));
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({1, 0, 0}));
    };

    test = IM_REGISTER_TEST(engine, scm_name, "click navigation on ASCII text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->WindowMove(window_name, ImVec2{0.f, 0.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_ascii(editor);
        ctx->MouseMoveToPos({146, 66});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({2, 6, 3}));
        ctx->MouseMoveToPos({243, 85});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({3, 14, 0}));
        ctx->MouseMoveToPos({23, 98});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({4, 0, 0}));
        ctx->MouseMoveToPos({301, 30});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({0, 4, 1}));

    };

    test = IM_REGISTER_TEST(engine, scm_name, "click navigation on UTF-8 text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->WindowMove(window_name, ImVec2{0.f, 0.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_utf8(editor);
        ctx->MouseMoveToPos({146, 66});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({2, 7, 1}));
        ctx->MouseMoveToPos({243, 85});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({3, 0, 0}));
        ctx->MouseMoveToPos({301, 30});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({0, 8, 27}));

    };

    test = IM_REGISTER_TEST(engine, mcm_name, "ctrl+click cursor creation on ASCII text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->WindowMove(window_name, ImVec2{0.f, 0.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_ascii(editor);
        ctx->MouseMoveToPos({146, 66});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({2, 6, 3}));

        ctx->KeyDown(ImGuiKey_LeftCtrl);
        ctx->MouseMoveToPos({243, 85});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({3, 14, 0}));
        IM_CHECK(editor.has_cursor({2, 6, 3}));

        ctx->MouseMoveToPos({23, 98});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({4, 0, 0}));
        IM_CHECK(editor.has_cursor({3, 14, 0}));
        IM_CHECK(editor.has_cursor({2, 6, 3}));

        ctx->KeyUp(ImGuiKey_LeftCtrl);
        ctx->MouseMoveToPos({301, 30});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({0, 4, 1}));
        IM_CHECK(!editor.has_cursor({4, 0, 0}));
        IM_CHECK(!editor.has_cursor({3, 14, 0}));
        IM_CHECK(!editor.has_cursor({2, 6, 3}));

    };

    test = IM_REGISTER_TEST(engine, mcm_name, "ctrl+click cursor creation on UTF-8 text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->WindowMove(window_name, ImVec2{0.f, 0.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_utf8(editor);

        ctx->MouseMoveToPos({146, 66});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({2, 7, 1}));

        ctx->KeyDown(ImGuiKey_LeftCtrl);
        ctx->MouseMoveToPos({243, 85});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({3, 0, 0}));
        IM_CHECK(editor.has_cursor({2, 7, 1}));

        ctx->KeyUp(ImGuiKey_LeftCtrl);
        ctx->MouseMoveToPos({301, 30});
        ctx->MouseClick(ImGuiMouseButton_Left);
        IM_CHECK(editor.has_cursor({0, 8, 27}));
        IM_CHECK(!editor.has_cursor({3, 0, 0}));
        IM_CHECK(!editor.has_cursor({2, 7, 1}));

    };

    test = IM_REGISTER_TEST(engine, mcm_name, "multiple cursor movement on ASCII text");
    test->GuiFunc = [&](ImGuiTestContext*) {
        if (ImGui::Begin(window_name)) {
            editor.render();
        }
        ImGui::End();
    };
    test->TestFunc = [&](ImGuiTestContext* ctx) {
        // taking focus
        ctx->WindowResize(window_name, ImVec2{500.f, 250.f});
        ctx->WindowMove(window_name, ImVec2{0.f, 0.f});
        ctx->SetRef(window_name);
        ctx->MouseMove(editor_name, ImGuiTestOpFlags_NoCheckHoveredId | ImGuiTestOpFlags_MoveToEdgeR | ImGuiTestOpFlags_MoveToEdgeU);
        ctx->MouseClick(ImGuiMouseButton_Left);

        // filling editor with starting data
        set_data_ascii(editor);
        editor.set_cursor({2, 6, 3});
        editor.add_cursor({3, 6, 1});


        // basic moves
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({1, 0, 0}));
        IM_CHECK(editor.has_cursor({2, 6, 2}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({2, 6, 3}));
        IM_CHECK(editor.has_cursor({3, 6, 1}));
        ctx->KeyPress(ImGuiKey_DownArrow);
        IM_CHECK(editor.has_cursor({3, 7, 1}));
        IM_CHECK(editor.has_cursor({4, 4, 1}));
        ctx->KeyPress(ImGuiKey_UpArrow);
        IM_CHECK(editor.has_cursor({2, 6, 3}));
        IM_CHECK(editor.has_cursor({3, 6, 1}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({2, 6, 2}));
        IM_CHECK(editor.has_cursor({3, 6, 0}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({2, 6, 3}));
        IM_CHECK(editor.has_cursor({3, 6, 1}));
        ctx->KeyPress(ImGuiKey_RightArrow);
        IM_CHECK(editor.has_cursor({2, 6, 4}));
        IM_CHECK(editor.has_cursor({3, 7, 1}));
        ctx->KeyPress(ImGuiKey_LeftArrow);
        IM_CHECK(editor.has_cursor({2, 6, 3}));
        IM_CHECK(editor.has_cursor({3, 6, 1}));

        ctx->KeyPress(ImGuiKey_End);
        IM_CHECK(editor.has_cursor({2, 17, 1}));
        IM_CHECK(editor.has_cursor({3, 22, 1}));
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({2, 0, 0}));
        IM_CHECK(editor.has_cursor({3, 1, 0}));
        ctx->KeyPress(ImGuiKey_Home);
        IM_CHECK(editor.has_cursor({2, 0, 0}));
        IM_CHECK(editor.has_cursor({3, 0, 0}));
    };



    // TODO cursor pos when inputting text ? (maybe test that elsewhere)
}