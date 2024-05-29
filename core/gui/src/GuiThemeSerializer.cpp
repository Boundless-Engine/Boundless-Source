#include "GuiThemeSerializer.h"

#include "pch.h"
#include "GUITheme.h"
#include "imgui.h"

#include <fstream>
#include <filesystem>

namespace Boundless {
	namespace Graphics {

		namespace Gui {

			BReturn Theme::Save(const std::string& filepath, GuiTheme* pTheme) {
				
				if (!std::filesystem::exists(filepath))
				{
					auto dir = filepath.substr(0, filepath.find_last_of("/") + 1);
					std::filesystem::create_directories(dir);
				}
				
				std::ofstream file(filepath);

				nlohmann::json j = *pTheme;

				if (file.is_open()) {
					file << j.dump(4);
					file.close();
				}
				else {
					printf("could not save to filepath: %s as the filepath or file does not exist...", filepath.c_str());
					return FAILURE;
				}

				return SUCCESS;
			}

			BReturn Theme::Save(const std::string& filepath, ImGuiStyle& style) {

				if (!std::filesystem::exists(filepath))
				{
					auto dir = filepath.substr(0, filepath.find_last_of("/") + 1);
					std::filesystem::create_directories(dir);
				}

				std::ofstream file(filepath);

				auto theme = GuiTheme(style);

				nlohmann::json j = theme;

				if (file.is_open()) {
					file << j.dump(4);
					file.close();
				}
				else {
					printf("could not save to filepath: %s as the filepath or file does not exist...", filepath.c_str());
					return FAILURE;
				}

				return SUCCESS;
			}


			BReturn Theme::Load(const std::string& filepath, GuiTheme* pTheme)
			{
				std::ifstream file(filepath);
				nlohmann::json j;

				if (file.is_open()) {
					file >> j;
					file.close();
				}
				else {
					printf("could not load from filepath: %s as the filepath or file does not exist...", filepath.c_str());
					return FAILURE;
				}

				*pTheme = j.get<GuiTheme>();

				return SUCCESS;
			}

			BReturn Theme::Apply(const GuiTheme* theme)
			{
				ImGuiStyle& style = ImGui::GetStyle();

				style.Alpha = theme->Global.Alpha;
				style.DisabledAlpha = theme->Global.DisabledAlpha;
				style.WindowPadding = theme->Window.Padding;
				style.WindowRounding = theme->Window.Rounding;
				style.WindowBorderSize = theme->Window.BorderSize;
				style.WindowMinSize = theme->Window.MinSize;
				style.WindowTitleAlign = theme->Window.TitleAlign;
				style.WindowMenuButtonPosition = theme->Window.MenuButtonPosition;
				style.ChildRounding = theme->ChildWindows.Rounding;
				style.ChildBorderSize = theme->ChildWindows.BorderSize;
				style.PopupBorderSize = theme->Popups.BorderSize;
				style.PopupRounding = theme->Popups.Rounding;
				style.FramePadding = theme->Frame.Padding;
				style.FrameRounding = theme->Frame.Rounding;
				style.FrameBorderSize = theme->Frame.BorderSize;
				style.ItemSpacing = theme->Widget.ItemSpacing;
				style.ItemInnerSpacing = theme->Widget.ItemInnerSpacing;
				style.CellPadding = theme->Widget.CellPadding;
				style.TouchExtraPadding = theme->Widget.TouchExtraPadding;
				style.IndentSpacing = theme->Widget.IndentSpacing;
				style.ColumnsMinSpacing = theme->Widget.ColumnsMinSpacing;
				style.ScrollbarSize = theme->Scrollbar.Size;
				style.ScrollbarRounding = theme->Scrollbar.Rounding;
				style.GrabMinSize = theme->Scrollbar.Grab.MinSize;
				style.GrabRounding = theme->Scrollbar.Grab.Rounding;
				style.LogSliderDeadzone = theme->Global.LogSliderDeadzone;
				style.TabRounding = theme->Tab.Rounding;
				style.TabBorderSize = theme->Tab.BorderSize;
				style.TabMinWidthForCloseButton = theme->Tab.MinWidthForCloseButton;
				style.TableAngledHeadersAngle = theme->Global.TableAngledHeadersAngle;
				style.ColorButtonPosition = theme->Global.ColorButtonPosition;
				style.ButtonTextAlign = theme->Global.ButtonTextAlign;
				style.SelectableTextAlign = theme->Global.SelectableTextAlign;
				style.SeparatorTextBorderSize = theme->Seperator.TextBorderSize;
				style.SeparatorTextAlign = theme->Seperator.TextAlign;
				style.SeparatorTextPadding = theme->Seperator.TextPadding;
				style.DisplayWindowPadding = theme->Global.DisplayWindowPadding;
				style.DisplaySafeAreaPadding = theme->Global.DisplaySafeAreaPadding;
				style.DockingSeparatorSize = theme->Dock.SeparatorSize;
				style.MouseCursorScale = theme->Global.MouseCursorScale;
				style.AntiAliasedLines = theme->Global.AntiAliasedLines;
				style.AntiAliasedFill = theme->Global.AntiAliasedFill;
				style.AntiAliasedLinesUseTex = theme->Global.AntiAliasedLinesUseTex;
				style.CurveTessellationTol = theme->Global.CurveTessellationTol;
				style.CircleTessellationMaxError = theme->Global.CircleTessellationMaxError;

				ImVec4* colors = style.Colors;

				colors[ImGuiCol_Text] = theme->Text.Default;
				colors[ImGuiCol_TextDisabled] = theme->Text.Disabled;
				colors[ImGuiCol_WindowBg] = theme->Window.Background;
				colors[ImGuiCol_ChildBg] = theme->ChildWindows.Background;
				colors[ImGuiCol_PopupBg] = theme->Popups.Background;
				colors[ImGuiCol_Border] = theme->Border.Color;
				colors[ImGuiCol_BorderShadow] = theme->Border.ShadowColor;
				colors[ImGuiCol_FrameBg] = theme->Frame.Background;
				colors[ImGuiCol_FrameBgHovered] = theme->Frame.Hover;
				colors[ImGuiCol_FrameBgActive] = theme->Frame.Active;
				colors[ImGuiCol_TitleBg] = theme->Title.Background;
				colors[ImGuiCol_TitleBgActive] = theme->Title.Active;
				colors[ImGuiCol_TitleBgCollapsed] = theme->Title.Collapsed;
				colors[ImGuiCol_MenuBarBg] = theme->Menubar.Background;
				colors[ImGuiCol_ScrollbarBg] = theme->Scrollbar.Background;
				colors[ImGuiCol_ScrollbarGrab] = theme->Scrollbar.Grab.Background;
				colors[ImGuiCol_ScrollbarGrabHovered] = theme->Scrollbar.Grab.Hover;
				colors[ImGuiCol_ScrollbarGrabActive] = theme->Scrollbar.Grab.Active;
				colors[ImGuiCol_CheckMark] = theme->Inputs.CheckMark.Foreground;
				colors[ImGuiCol_SliderGrab] = theme->Inputs.Slider.Grab;
				colors[ImGuiCol_SliderGrabActive] = theme->Inputs.Slider.GrabActive;
				colors[ImGuiCol_Button] = theme->Inputs.Button.Background;
				colors[ImGuiCol_ButtonHovered] = theme->Inputs.Button.Hover;
				colors[ImGuiCol_ButtonActive] = theme->Inputs.Button.Active;
				colors[ImGuiCol_Header] = theme->Header.Background;
				colors[ImGuiCol_HeaderHovered] = theme->Header.Hover;
				colors[ImGuiCol_HeaderActive] = theme->Header.Active;
				colors[ImGuiCol_Separator] = theme->Seperator.Background;
				colors[ImGuiCol_SeparatorHovered] = theme->Seperator.Hover;
				colors[ImGuiCol_SeparatorActive] = theme->Seperator.Active;
				colors[ImGuiCol_ResizeGrip] = theme->ResizeGrip.Background;
				colors[ImGuiCol_ResizeGripHovered] = theme->ResizeGrip.Hover;
				colors[ImGuiCol_ResizeGripActive] = theme->ResizeGrip.Active;
				colors[ImGuiCol_Tab] = theme->Tab.Background;
				colors[ImGuiCol_TabHovered] = theme->Tab.Hover;
				colors[ImGuiCol_TabActive] = theme->Tab.Active;
				colors[ImGuiCol_TabUnfocused] = theme->Tab.Unfocused;
				colors[ImGuiCol_TabUnfocusedActive] = theme->Tab.UnfocusedActive;
				colors[ImGuiCol_DockingPreview] = theme->Dock.Preview;
				colors[ImGuiCol_DockingEmptyBg] = theme->Dock.EmptyBG;
				colors[ImGuiCol_PlotLines] = theme->Graph.Lines.Background;
				colors[ImGuiCol_PlotLinesHovered] = theme->Graph.Lines.Hover;
				colors[ImGuiCol_PlotHistogram] = theme->Graph.Bars.Background;
				colors[ImGuiCol_PlotHistogramHovered] = theme->Graph.Bars.Hover;
				colors[ImGuiCol_TableHeaderBg] = theme->Table.HeaderBG;
				colors[ImGuiCol_TableBorderStrong] = theme->Table.BorderStrong;
				colors[ImGuiCol_TableBorderLight] = theme->Table.BorderLight;
				colors[ImGuiCol_TableRowBg] = theme->Table.RowBG;
				colors[ImGuiCol_TableRowBgAlt] = theme->Table.RowAltBG;
				colors[ImGuiCol_TextSelectedBg] = theme->Text.Selected;
				colors[ImGuiCol_DragDropTarget] = theme->DragNDrop.Target;
				colors[ImGuiCol_NavHighlight] = theme->Navigation.Highlight;
				colors[ImGuiCol_NavWindowingHighlight] = theme->Navigation.WindowHighlight;
				colors[ImGuiCol_NavWindowingDimBg] = theme->Navigation.WindowDim;
				colors[ImGuiCol_ModalWindowDimBg] = theme->Modal.WindowDim;


				return SUCCESS;
			}
		}

	}
}
