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

    std::string to_string(const ImEdit::editor& ed) {
        std::string str;
        std::copy(ed.begin(), ed.end(), std::back_inserter(str));
        return str;
    }

    void set_data_utf8(ImEdit::editor &ed) {
        ed.clear();
        ed.set_data("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                    "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᚻᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                    "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n"
        );
    }

    constexpr const char* test_category = "Text Edition";
    constexpr const char* window_name = "text_edition_simple_tests";
    constexpr const char* editor_name = "editor";

    ImEdit::editor editor(editor_name);
}

void add_text_edition_tests(ImGuiTestEngine* engine) {

    auto test = IM_REGISTER_TEST(engine, test_category, "ASCII text deletion");
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
        editor.set_cursor({3, 11});
        IM_CHECK(editor.has_cursor({3, 11}));

        ctx->KeyPress(ImGuiKey_Backspace);
        IM_CHECK(editor.has_cursor({3, 10}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::cut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());
        ctx->KeyPress(ImGuiKey_Delete);
        IM_CHECK(editor.has_cursor({3, 10}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::ct << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());


        editor.set_cursor({3, 21});
        ctx->KeyPress(ImGuiKey_Backspace | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 16}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::ct << \" World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());

        editor.set_cursor({3, 17});
        ctx->KeyPress(ImGuiKey_Delete | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({3, 17}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::ct << \" \" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());


        editor.set_cursor({2, 0});
        ctx->KeyPress(ImGuiKey_Backspace);
        IM_CHECK(editor.has_cursor({1, 0}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::ct << \" \" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());

        editor.set_cursor({3, 13});
        ctx->KeyPress(ImGuiKey_Delete);
        IM_CHECK(editor.has_cursor({3, 13}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::ct << \" \" << std::endl;\n"
                        "    return 0;}", to_string(editor).c_str());
    };


    test = IM_REGISTER_TEST(engine, test_category, "UTF8 text deletion");
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
        editor.set_cursor({1, 54});
        IM_CHECK(editor.has_cursor({1, 54}));

        ctx->KeyPress(ImGuiKey_Backspace);
        IM_CHECK(editor.has_cursor({1, 51}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        ctx->KeyPress(ImGuiKey_Delete);
        IM_CHECK(editor.has_cursor({1, 51}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({2, 58});
        ctx->KeyPress(ImGuiKey_Backspace | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({2, 40}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({2, 47});
        ctx->KeyPress(ImGuiKey_Delete | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({2, 47}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛖ ᛞ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({2, 0});
        ctx->KeyPress(ImGuiKey_Backspace);
        IM_CHECK(editor.has_cursor({1, 105}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛖ ᛞ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({0, 79});
        ctx->KeyPress(ImGuiKey_Delete);
        IM_CHECK(editor.has_cursor({0, 79}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᚪ ᚷᛖᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛖ ᛞ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

    };


    test = IM_REGISTER_TEST(engine, test_category, "ASCII text input");
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
        editor.set_cursor({3, 11});
        IM_CHECK(editor.has_cursor({3, 11}));

        ctx->KeyChars("auie nrst");
        IM_CHECK(editor.has_cursor({3, 20}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::coauie nrstut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());

        editor.set_cursor({1, 0});
        ctx->KeyPress(ImGuiKey_Enter);
        IM_CHECK(editor.has_cursor({2, 0}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::coauie nrstut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());


        ctx->KeyChars("auie nrst");
        IM_CHECK(editor.has_cursor({2, 9}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "auie nrst\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::coauie nrstut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());

        editor.set_cursor({2, 4});
        ctx->KeyPress(ImGuiKey_Enter | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({2, 4}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "auie\n"
                        " nrst\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::coauie nrstut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());

        ctx->KeyPress(ImGuiKey_Enter);
        IM_CHECK(editor.has_cursor({3, 0}));
        IM_CHECK_STR_EQ("#include <iostream>\n"
                        "\n"
                        "auie\n"
                        "\n"
                        " nrst\n"
                        "int main(int argc, char* argv[]) {\n"
                        "    std::coauie nrstut << \"Hello World\" << std::endl;\n"
                        "    return 0;\n"
                        "}", to_string(editor).c_str());
    };


    test = IM_REGISTER_TEST(engine, test_category, "UTF8 text input");
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
        editor.set_cursor({1, 41});
        IM_CHECK(editor.has_cursor({1, 41}));

        ctx->KeyChars("ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ");
        IM_CHECK(editor.has_cursor({1, 69}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᛋᚳᛖᚪᛚ ᚦᛖᚪᚻᚪ ᚷᛖᚻᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({1, 0});
        ctx->KeyPress(ImGuiKey_Enter);
        IM_CHECK(editor.has_cursor({2, 0}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "\n"
                        "ᛋᚳᛖᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᛋᚳᛖᚪᛚ ᚦᛖᚪᚻᚪ ᚷᛖᚻᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());

        editor.set_cursor({2, 9});
        ctx->KeyPress(ImGuiKey_Enter | ImGuiMod_Ctrl);
        IM_CHECK(editor.has_cursor({2, 9}));
        IM_CHECK_STR_EQ("ᚠᛇᚻ ᛒᛦᚦ ᚠᚱᚩᚠᚢᚱ ᚠᛁᚱᚪ ᚷᛖᚻᚹᛦᛚᚳᚢᛗ\n"
                        "\n"
                        "ᛋᚳᛖ\n"
                        "ᚪᛚ ᚦᛖᚪᚻ ᛗᚪᚾᚾᛋᚳᛖᚪᛚ ᚦᛖᚪᚻᚪ ᚷᛖᚻᚹᛦᛚᚳ ᛗᛁᚳᛚᚢᚾ ᚻᛦᛏ ᛞᚫᛚᚪᚾ\n"
                        "ᚷᛁᚠ ᚻᛖ ᚹᛁᛚᛖ ᚠᚩᚱ ᛞᚱᛁᚻᛏᚾᛖ ᛞᚩᛗᛖᛋ ᚻᛚᛇᛏᚪᚾ᛬\n", to_string(editor).c_str());
    };


    // TODO selection suppression
    // TODO writing over a selection
    // TODO ascii/utf8, mono/multi-cursor pasting
}