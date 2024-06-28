#include "BoundlessEditor.h"

constexpr const char* EDITOR_TITLE = "Boundless Editor";
constexpr const char* EDITOR_META_FILE_LOCATION = "themes/default.json\0";

#include "imgui.h"

#include "views/console_view.h"
#include "views/scene_view.h"


namespace {
	
	I::IWindow* win;
	I::IInputHandler* input;
	I::IGraphicsAPI* surface;
	I::IGUI* gui;

}

BoundlessEditor* BoundlessEditor::_instance = nullptr;

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
#include "Windows.h"
#ifdef max 
#undef max
#endif
#ifdef min
#undef min
#endif


void OpenNativeFileDialog(char* outFilePath, size_t maxSize) {
    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = outFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = maxSize;
    ofn.lpstrFilter = "All Files\0*.json\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        //outFilePath = ofn.lpstrFile;
        printf("open file: %s success.\n", outFilePath);
    }


    // Restore the current working directory
    SetCurrentDirectoryA(currentDir);
}

void CreateNativeFileDialog(char* outFilePath, size_t maxSize) {
    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = outFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = maxSize;
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        //outFilePath = ofn.lpstrFile;
        printf("creat file: %s success.\n", outFilePath);
    }

    // Restore the current working directory
    SetCurrentDirectoryA(currentDir);
}

void BoundlessEditor::ShowStyleEditor()
{
 
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 size = viewport->Size;
    const ImVec2 topSize = ImVec2(size.x * 0.7f, size.y * 0.03f);
    const ImVec2 leftSize = ImVec2(size.x * 0.2f, size.y * 0.7f);
    const ImVec2 rightSize = ImVec2(size.x * 0.5f, size.y * 0.7f);

    // header..
    {

        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
        ImGui::BeginChild("###Header", topSize, true);
        ImGui::SetCursorPosX((topSize.x - ImGui::CalcTextSize("Preferences").x) * 0.5f); // Center text horizontally
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Preferences");

        // Position close button to the top right corner
        ImGui::SameLine(topSize.x - 20.0f - ImGui::GetTextLineHeight());
    
        ImGui::PushStyleColor(ImGuiCol_Button,        0x7F'00'00'FF);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFF'00'00'FF);

        if (ImGui::Button("###X", ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight())))
        {
            show_preferences = false; // Assuming 'show_preferences' is a variable controlling the visibility of the preferences window
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("close?");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        ImGui::PopStyleColor(2);
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    // sidebar..
    {
        ImGui::BeginChild("###Sidebar", leftSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::Button("Style");
        ImGui::EndChild();
        ImGui::SameLine();
    }
    // main-content region
    {
        ImGui::BeginChild("###Main-Content", rightSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGuiStyle& style = ImGui::GetStyle(); // get the current style..
        
        auto ref = &style;

        static bool customStyleLoaded = false;

        static char filepath[512]{'\0'};

        if (!customStyleLoaded) {
            const char* default_filepath = "themes/default.json";
            memcpy(filepath, default_filepath, strlen(default_filepath));
        }
        
        ImGui::TextColored(ImVec4(ImGui::ColorConvertU32ToFloat4(0xFFFFFFFF)), filepath);
        ImGui::SameLine();
        if (ImGui::Button("Open")) {
            OpenNativeFileDialog(filepath, 512);
            customStyleLoaded = true;
            Graphics::GuiTheme newTheme;
            Graphics::Gui::Theme::Load(filepath, &newTheme);
            Graphics::Gui::Theme::Apply(&newTheme);
        }
        ImGui::SameLine();
        if (ImGui::Button("Create")) {
            CreateNativeFileDialog(filepath, 512);
            customStyleLoaded = true;
            Graphics::GuiTheme newTheme;
            Graphics::Gui::Theme::Save(filepath, &newTheme);
            Graphics::Gui::Theme::Apply(&newTheme);
        }
        // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
        if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f")) style.GrabRounding = style.FrameRounding;

        { bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

        ImGui::Separator();

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {

            if (ImGui::BeginTabItem("Sizes"))
            {
                ImGui::SeparatorText("Main");
                ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
                ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");

                ImGui::SeparatorText("Borders");
                ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("TabBarBorderSize", &style.TabBarBorderSize, 0.0f, 2.0f, "%.0f");

                ImGui::SeparatorText("Rounding");
                ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");

                ImGui::SeparatorText("Tables");
                ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderAngle("TableAngledHeadersAngle", &style.TableAngledHeadersAngle, -50.0f, +50.0f);

                ImGui::SeparatorText("Widgets");
                ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                    style.WindowMenuButtonPosition = window_menu_button_position - 1;
                ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
                ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
                ImGui::SliderFloat("SeparatorTextBorderSize", &style.SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat2("SeparatorTextAlign", (float*)&style.SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat2("SeparatorTextPadding", (float*)&style.SeparatorTextPadding, 0.0f, 40.0f, "%.0f");
                ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

                ImGui::SeparatorText("Docking");
                ImGui::SliderFloat("DockingSplitterSize", &style.DockingSeparatorSize, 0.0f, 12.0f, "%.0f");

                ImGui::SeparatorText("Tooltips");
                for (int n = 0; n < 2; n++)
                    if (ImGui::TreeNodeEx(n == 0 ? "HoverFlagsForTooltipMouse" : "HoverFlagsForTooltipNav"))
                    {
                        ImGuiHoveredFlags* p = (n == 0) ? &style.HoverFlagsForTooltipMouse : &style.HoverFlagsForTooltipNav;
                        ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNone", p, ImGuiHoveredFlags_DelayNone);
                        ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayShort", p, ImGuiHoveredFlags_DelayShort);
                        ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNormal", p, ImGuiHoveredFlags_DelayNormal);
                        ImGui::CheckboxFlags("ImGuiHoveredFlags_Stationary", p, ImGuiHoveredFlags_Stationary);
                        ImGui::CheckboxFlags("ImGuiHoveredFlags_NoSharedDelay", p, ImGuiHoveredFlags_NoSharedDelay);
                        ImGui::TreePop();
                    }

                ImGui::SeparatorText("Misc");
                ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f"); ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {
                static ImGuiTextFilter filter;
                filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                static ImGuiColorEditFlags alpha_flags = 0;
                if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
                HelpMarker(
                    "In the color list:\n"
                    "Left-click on color square to open color picker,\n"
                    "Right-click to open edit options menu.");

                ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10), ImVec2(FLT_MAX, FLT_MAX));
                ImGui::BeginChild("##colors", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    if (ImGui::Button("?"))
                        ImGui::DebugFlashStyleColor((ImGuiCol)i);
                    ImGui::SetItemTooltip("Flash given color to identify places where it is used.");
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                    if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                    {
                        // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                        // so instead of "Save"/"Revert" you'd use icons!
                        // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { ref->Colors[i] = style.Colors[i]; }
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = ref->Colors[i]; }
                    }
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::TextUnformatted(name);
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Fonts"))
            {
                ImGuiIO& io = ImGui::GetIO();
                ImFontAtlas* atlas = io.Fonts;
                HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
                //ImGui::ShowFontAtlas(atlas);

                // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
                // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
                const float MIN_SCALE = 0.3f;
                const float MAX_SCALE = 2.0f;
                HelpMarker(
                    "Those are old settings provided for convenience.\n"
                    "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                    "rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
                    "Using those settings here will give you poor quality results.");
                static float window_scale = 1.0f;
                ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
                if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
                    ImGui::SetWindowFontScale(window_scale);
                ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rendering"))
            {
                ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
                ImGui::SameLine();
                HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

                ImGui::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
                ImGui::SameLine();
                HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

                ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
                ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
                ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
                if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

                // When editing the "Circle Segment Max Error" value, draw a preview of its effect on auto-tessellated circles.
                ImGui::DragFloat("Circle Tessellation Max Error", &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
                const bool show_samples = ImGui::IsItemActive();
                if (show_samples)
                    ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
                if (show_samples && ImGui::BeginTooltip())
                {
                    ImGui::TextUnformatted("(R = radius, N = number of segments)");
                    ImGui::Spacing();
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
                    for (int n = 0; n < 8; n++)
                    {
                        const float RAD_MIN = 5.0f;
                        const float RAD_MAX = 70.0f;
                        const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

                        ImGui::BeginGroup();

                        ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

                        const float canvas_width = std::max(min_widget_width, rad * 2.0f);
                        const float offset_x = floorf(canvas_width * 0.5f);
                        const float offset_y = floorf(RAD_MAX);

                        const ImVec2 p1 = ImGui::GetCursorScreenPos();
                        draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
                        ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));


                        ImGui::EndGroup();
                        ImGui::SameLine();
                    }
                    ImGui::EndTooltip();
                }
                ImGui::SameLine();
                HelpMarker("When drawing circle primitives with \"num_segments == 0\" tesselation will be calculated automatically.");

                ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
                ImGui::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        if (customStyleLoaded) {

            if (ImGui::Button("Save")) {

                auto newTheme = Graphics::GuiTheme(style);
                Graphics::Gui::Theme::Save(filepath, &newTheme);
                Graphics::Gui::Theme::Apply(&newTheme);
                
                metadata.theme = filepath;

                if (Serialize() == FAILURE) {
                    throw std::runtime_error("no metafile found...");
                }
                
            }
            if (ImGui::Button("Cancel")) {
                show_preferences = false;
                ImGui::CloseCurrentPopup();
            }

        }
        ImGui::EndChild();
    }

}

void BoundlessEditor::SaveDialogue(std::function<void(bool)> closeFn) {

	ImGuiIO& io = ImGui::GetIO();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 windowSize = viewport->Size;
	ImVec2 windowPos = viewport->Pos;

	// Calculate the center of the main viewport
	ImVec2 pos(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

	// Set the next window position to the center, adjusted by the window size
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	//ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_Modal | ImGuiWindowFlags_Popup
		| ImGuiWindowFlags_NoDocking;

	if (show_save_popup) {
		ImGui::OpenPopup("Save Progress");
	}
		

	if(ImGui::BeginPopupModal("Save Progress", &show_save_popup, flags)) {

		ImGui::Text("Save Progress?");

		if (ImGui::Button("Yes")) {
			closeFn(true);
			show_save_popup = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			closeFn(false);
			show_save_popup = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			show_save_popup = false;
		}

		ImGui::EndPopup();
	}

		// Draw a transparent overlay to block interaction with elements behind the dialogue
		
	
}

void BoundlessEditor::ShowPreferences()
{
    if (!show_preferences)
        return;

    ImGuiIO& io = ImGui::GetIO();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 windowSize = viewport->Size;
    ImVec2 windowPos = viewport->Pos;

    // Calculate the center of the main viewport
    ImVec2 pos(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

    // Set the next window position to the center, adjusted by the window size
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    //ImGui::SetNextWindowSize(ImVec2(windowSize.x / 2, windowSize.y - 100), ImGuiCond_Always);

    //ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        //| ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_Modal | ImGuiWindowFlags_Popup
        | ImGuiWindowFlags_NoDocking;

	constexpr const char* preferences_str = "Preferences";
	
    if (show_preferences) {
		ImGui::OpenPopup(preferences_str);
    }


	if (ImGui::BeginPopup(preferences_str, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_Modal )) {
		
        ShowStyleEditor();

		ImGui::EndPopup();
	}
}

BReturn BoundlessEditor::Initillize()
{
    // determine if a meta file exists.. if one doesnt create one
    // the meta file is used to persist user changes to the editor.

    // Try to open existing file for reading
    if (Deserialize() == FAILURE) {
        // If file doesn't exist, create it with default data
        std::cout << "Creating default metafile..." << std::endl;

        metadata.theme = EDITOR_META_FILE_LOCATION;

        if (Serialize() == FAILURE) {
            std::cerr << "Failed to create default metafile." << std::endl;
            return FAILURE;
        }
    }
    else {
        std::cout << "Metafile loaded successfully." << std::endl;
    }

    std::cout << "Theme: " << metadata.theme << std::endl;


	+Window::Create(prefs.Window.width, prefs.Window.height, EDITOR_TITLE, &win);
	+Input::Create(&input);
	+Graphics::GraphicsAPI::Create(win, &surface);

	+Graphics::Gui::Create(surface, &gui, metadata.theme, _instance->Menubar());
	
	PushLayer<Console>();
	PushLayer<SceneView>(surface);

	return SUCCESS;
}

BReturn BoundlessEditor::Shutdown()
{
    RemoveLayer<Console>();
    RemoveLayer<SceneView>();

	+Graphics::Gui::Destroy(&gui);
	+Graphics::GraphicsAPI::Destroy(&surface);
	+Input::Destroy(&input);
	+Window::Destroy(&win);

	return SUCCESS;
}

BReturn BoundlessEditor::Run()
{

	while (true) {
		
		if (!application_should_run)
		{	
			break;
		}

        // Define a global variable to store the time at the end of the previous frame
        static auto previousTime = std::chrono::high_resolution_clock::now();

        // At the beginning of each frame:
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate deltaTime (time elapsed since the last frame)
        std::chrono::duration<float> duration = currentTime - previousTime;
        float deltaTime = duration.count();

        // Update previousTime for the next frame
        previousTime = currentTime;
        for (auto& l : layers)
		{
			l->OnUpdate(deltaTime);
		}

        for (auto& l : layers)
        {
            l->OnRender();
        }


		{
			gui->Begin();
         
			for (auto& l : layers)
			{
				l->OnGUI();
			}

			ImGui::ShowDemoWindow();

			SaveDialogue([&](bool shouldSave) { if (shouldSave) { SavePrefs(); } application_should_run = false; });
			ShowPreferences();

			gui->End();
		}
		gui->Present();


		// update all layers...

		win->Update();
	}

	return SUCCESS;
}

void BoundlessEditor::Close()
{
	auto shutdown = Events::ApplicationClosedEvent(true);
	EventDispatcher::Dispatch(&shutdown);
}

BReturn BoundlessEditor::Serialize()
{
    std::ofstream file(metadata_filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << metadata_filepath << std::endl;
        return FAILURE;
    }

    // Write the size of the string, followed by the string itself
    int length = static_cast<int>(metadata.theme.size());
    std::cout << "Writing length: " << length << std::endl;

    // Ensure that the write operations are successful
    if (!(file << length << '\n')) {
        std::cerr << "Failed to write length to file." << std::endl;
        return FAILURE;
    }

    if (!(file << metadata.theme << '\n')) {
        std::cerr << "Failed to write theme data to file." << std::endl;
        return FAILURE;
    }

    file.close();
    if (file.fail()) {
        std::cerr << "Failed to close the file after writing." << std::endl;
        return FAILURE;
    }

    std::cout << "File write success: " << metadata_filepath << std::endl;
    return SUCCESS;
}

BReturn BoundlessEditor::Deserialize()
{
    std::ifstream file(metadata_filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << metadata_filepath << std::endl;
        return FAILURE;
    }

    int length;
    if (!(file >> length)) {
        std::cerr << "Failed to read length from file." << std::endl;
        return FAILURE;
    }
    std::cout << "Read length: " << length << std::endl;

    // Ignore the newline character after the length
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    metadata.theme.resize(length);
    file.read(&metadata.theme[0], length);

    if (file.fail()) {
        std::cerr << "Failed to read theme data from file." << std::endl;
        return FAILURE;
    }

    file.close();
    if (file.fail()) {
        std::cerr << "Failed to close the file after reading." << std::endl;
        return FAILURE;
    }

    std::cout << "File read success: " << metadata_filepath << std::endl;
    return SUCCESS;
}

std::function<void()> BoundlessEditor::Menubar()
{
	return [&]() {

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				SavePrefs();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				show_save_popup = true;

			}
			ImGui::EndMenu();
		}
		
		else if (ImGui::BeginMenu("Edit")) {
			ImGui::MenuItem("Preferences", NULL, &show_preferences);
			
			ImGui::EndMenu();
		}
	};
}

void BoundlessEditor::OnEvent(I::IEvent* event)
{
	if (event->type == Events::WindowClosed) {
		Events::ApplicationClosedEvent close(false);
		EventDispatcher::Dispatch(&close);
	}
	
	if (event->type == Events::ApplicationShutdown) {
		auto close = static_cast<Events::ApplicationClosedEvent>(event);
		
		if(close.save)
			SavePrefs();
		
		application_should_run = false;
		return;
	}

	if (event->type == Events::WindowResized) {
		auto resize = static_cast<Events::WindowResizedEvent*>(event);
		prefs.Window.width = resize->width;
		prefs.Window.height = resize->height;
	}

	if (event->type == Events::WindowMoved) {
		auto moved = static_cast<Events::WindowMovedEvent*>(event);
		prefs.Window.x = moved->posX;
		prefs.Window.y = moved->posY;
	}

}
