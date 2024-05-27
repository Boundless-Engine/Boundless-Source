#pragma once

#include "imgui.h"

struct GuiTheme {

	struct {
		ImVec4 Default = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImVec4 Disabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		ImVec4 Selected = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	} Text;

	struct {
		ImVec4 Background = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		ImVec2 Padding = ImVec2(8, 8);
		float  Rounding = 5;
		float  BorderSize = 1;
		ImVec2 MinSize = ImVec2(1, 1);
		ImVec2 TitleAlign = ImVec2(0.5f, 0.5f);
		ImGuiDir MenuButtonPosition = ImGuiDir_Left;
	} Window;

	struct {
		ImVec4 Background = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		float Rounding = 5;
		float BorderSize = 1;
	} ChildWindows;

	struct {
		ImVec4 Background = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		float       Rounding = 5;
		float       BorderSize = 1;
	}Popups;

	struct {
		ImVec4 Color = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		ImVec4 ShadowColor = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	}Border;

	struct {
		ImVec4 Background = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
		ImVec4 Hovered = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		ImVec4 Active = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

		ImVec2 Padding = ImVec2(5, 5);
		float Rounding = 5;
		float BorderSize = 1;
	}Frame;

	struct {
		ImVec4 Background = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		ImVec4 Active = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		ImVec4 Collapsed = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	}Title;

	struct {
		ImVec4 Background = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	}Menubar;

	struct {
		ImVec4 Background = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		struct {
			ImVec4 Background = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			ImVec4 Hover = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			ImVec4 Active = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			float       MinSize = 1;
			float       Rounding = 5;
		} Grab;

		float       Size = 2;
		float       Rounding = 5;
	}Scrollbar;

	struct {
		struct {
			ImVec4 Foreground = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		}CheckMark;
		struct {
			ImVec4 Grab = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
			ImVec4 GrabActive = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		}Slider;
		struct {
			ImVec4 Background = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
			ImVec4 Hover = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			ImVec4 Active = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

		}Button;
	}Inputs;

	struct {
		ImVec4 Background = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		ImVec4 Hover = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		ImVec4 Active = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	}Header;

	struct {
		ImVec4 Background = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		ImVec4 Hover = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		ImVec4 Active = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		float  TextBorderSize = 0;
		ImVec2 TextAlign = ImVec2(0.5, 0.5);
		ImVec2 TextPadding = ImVec2(5, 5);
	}Seperator;

	struct {
		ImVec4 Background = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		ImVec4 Hover = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		ImVec4 Active = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	}ResizeGrip;

	struct {
		ImVec4 Background = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
		ImVec4 Hover = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		ImVec4 Active = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
		ImVec4 Unfocused = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		ImVec4 UnfocusedActive = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		float Rounding = 5;
		float BorderSize = 1;
		float MinWidthForCloseButton = 10;
		float BarBorderSize = 1;
	}Tab;

	struct {
		ImVec4 Preview = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		ImVec4 EmptyBG = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		float SeparatorSize = 5;

	}Dock;

	struct {
		struct {
			ImVec4 Background = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			ImVec4 Hover = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		}Lines;
		struct {
			ImVec4 Background = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			ImVec4 Hover = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		}Bars;
	}Graph;

	struct {
		ImVec4 HeaderBG = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		ImVec4 BorderStrong = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		ImVec4 BorderLight = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		ImVec4 RowBG = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		ImVec4 RowAltBG = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	} Table;

	struct {
		ImVec4 Highlight = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		ImVec4 WindowHighlight = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		ImVec4 WindowDim = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	}Navigation;

	struct {
		ImVec4 Target = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	}DragNDrop;

	struct {
		ImVec4 WindowDim = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	}Modal;

	struct {
		float       Alpha = 1.0f;
		float       DisabledAlpha = 0.5f;
		float       MouseCursorScale = 1.0f;
		bool        AntiAliasedLines = true;
		bool        AntiAliasedLinesUseTex = true;
		bool        AntiAliasedFill = true;
		float       CurveTessellationTol = 0.1f;
		float       CircleTessellationMaxError = 0.1f;
		float       LogSliderDeadzone = 0.01f;
		float       TableAngledHeadersAngle = 35.0f;
		ImGuiDir    ColorButtonPosition = ImGuiDir_Right;
		ImVec2      ButtonTextAlign = ImVec2(0.5f, 0.5f);
		ImVec2      SelectableTextAlign = ImVec2(0, 0);

		ImVec2      DisplayWindowPadding = ImVec2(8, 8);
		ImVec2      DisplaySafeAreaPadding = ImVec2(5, 5);
	} Global;

	struct {
		ImVec2      ItemSpacing = ImVec2(5, 5);
		ImVec2      ItemInnerSpacing = ImVec2(5, 5);
		ImVec2      CellPadding = ImVec2(5, 5);
		ImVec2      TouchExtraPadding = ImVec2(5, 5);
		float       IndentSpacing = 5;
		float       ColumnsMinSpacing = 5;
	}Widget;


	void Apply() {
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = Global.Alpha;
		style.DisabledAlpha = Global.DisabledAlpha;
		style.WindowPadding = Window.Padding;
		style.WindowRounding = Window.Rounding;
		style.WindowBorderSize = Window.BorderSize;
		style.WindowMinSize = Window.MinSize;
		style.WindowTitleAlign = Window.TitleAlign;
		style.WindowMenuButtonPosition = Window.MenuButtonPosition;
		style.ChildRounding = ChildWindows.Rounding;
		style.ChildBorderSize = ChildWindows.BorderSize;
		style.PopupBorderSize = Popups.BorderSize;
		style.PopupRounding = Popups.Rounding;
		style.FramePadding = Frame.Padding;
		style.FrameRounding = Frame.Rounding;
		style.FrameBorderSize = Frame.BorderSize;
		style.ItemSpacing = Widget.ItemSpacing;
		style.ItemInnerSpacing = Widget.ItemInnerSpacing;
		style.CellPadding = Widget.CellPadding;
		style.TouchExtraPadding = Widget.TouchExtraPadding;
		style.IndentSpacing = Widget.IndentSpacing;
		style.ColumnsMinSpacing = Widget.ColumnsMinSpacing;
		style.ScrollbarSize = Scrollbar.Size;
		style.ScrollbarRounding = Scrollbar.Rounding;
		style.GrabMinSize = Scrollbar.Grab.MinSize;
		style.GrabRounding = Scrollbar.Grab.Rounding;
		style.LogSliderDeadzone = Global.LogSliderDeadzone;
		style.TabRounding = Tab.Rounding;
		style.TabBorderSize = Tab.BorderSize;
		style.TabMinWidthForCloseButton = Tab.MinWidthForCloseButton;
		style.TableAngledHeadersAngle = Global.TableAngledHeadersAngle;
		style.ColorButtonPosition = Global.ColorButtonPosition;
		style.ButtonTextAlign = Global.ButtonTextAlign;
		style.SelectableTextAlign = Global.SelectableTextAlign;
		style.SeparatorTextBorderSize = Seperator.TextBorderSize;
		style.SeparatorTextAlign = Seperator.TextAlign;
		style.SeparatorTextPadding = Seperator.TextPadding;
		style.DisplayWindowPadding = Global.DisplayWindowPadding;
		style.DisplaySafeAreaPadding = Global.DisplaySafeAreaPadding;
		style.DockingSeparatorSize = Dock.SeparatorSize;
		style.MouseCursorScale = Global.MouseCursorScale;
		style.AntiAliasedLines = Global.AntiAliasedLines;
		style.AntiAliasedFill = Global.AntiAliasedFill;
		style.AntiAliasedLinesUseTex = Global.AntiAliasedLinesUseTex;
		style.CurveTessellationTol = Global.CurveTessellationTol;
		style.CircleTessellationMaxError = Global.CircleTessellationMaxError;

		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Text] = Text.Default;
		colors[ImGuiCol_TextDisabled] = Text.Disabled;
		colors[ImGuiCol_WindowBg] = Window.Background;
		colors[ImGuiCol_ChildBg] = ChildWindows.Background;
		colors[ImGuiCol_PopupBg] = Popups.Background;
		colors[ImGuiCol_Border] = Border.Color;
		colors[ImGuiCol_BorderShadow] = Border.ShadowColor;
		colors[ImGuiCol_FrameBg] = Frame.Background;
		colors[ImGuiCol_FrameBgHovered] = Frame.Hovered;
		colors[ImGuiCol_FrameBgActive] = Frame.Active;
		colors[ImGuiCol_TitleBg] = Title.Background;
		colors[ImGuiCol_TitleBgActive] = Title.Active;
		colors[ImGuiCol_TitleBgCollapsed] = Title.Collapsed;
		colors[ImGuiCol_MenuBarBg] = Menubar.Background;
		colors[ImGuiCol_ScrollbarBg] = Scrollbar.Background;
		colors[ImGuiCol_ScrollbarGrab] = Scrollbar.Grab.Background;
		colors[ImGuiCol_ScrollbarGrabHovered] = Scrollbar.Grab.Hover;
		colors[ImGuiCol_ScrollbarGrabActive] = Scrollbar.Grab.Active;
		colors[ImGuiCol_CheckMark] = Inputs.CheckMark.Foreground;
		colors[ImGuiCol_SliderGrab] = Inputs.Slider.Grab;
		colors[ImGuiCol_SliderGrabActive] = Inputs.Slider.GrabActive;
		colors[ImGuiCol_Button] = Inputs.Button.Background;
		colors[ImGuiCol_ButtonHovered] = Inputs.Button.Hover;
		colors[ImGuiCol_ButtonActive] = Inputs.Button.Active;
		colors[ImGuiCol_Header] = Header.Background;
		colors[ImGuiCol_HeaderHovered] = Header.Hover;
		colors[ImGuiCol_HeaderActive] = Header.Active;
		colors[ImGuiCol_Separator] = Seperator.Background;
		colors[ImGuiCol_SeparatorHovered] = Seperator.Hover;
		colors[ImGuiCol_SeparatorActive] = Seperator.Active;
		colors[ImGuiCol_ResizeGrip] = ResizeGrip.Background;
		colors[ImGuiCol_ResizeGripHovered] = ResizeGrip.Hover;
		colors[ImGuiCol_ResizeGripActive] = ResizeGrip.Active;
		colors[ImGuiCol_Tab] = Tab.Background;
		colors[ImGuiCol_TabHovered] = Tab.Hover;
		colors[ImGuiCol_TabActive] = Tab.Active;
		colors[ImGuiCol_TabUnfocused] = Tab.Unfocused;
		colors[ImGuiCol_TabUnfocusedActive] = Tab.UnfocusedActive;
		colors[ImGuiCol_DockingPreview] = Dock.Preview;
		colors[ImGuiCol_DockingEmptyBg] = Dock.EmptyBG;
		colors[ImGuiCol_PlotLines] = Graph.Lines.Background;
		colors[ImGuiCol_PlotLinesHovered] = Graph.Lines.Hover;
		colors[ImGuiCol_PlotHistogram] = Graph.Bars.Background;
		colors[ImGuiCol_PlotHistogramHovered] = Graph.Bars.Hover;
		colors[ImGuiCol_TableHeaderBg] = Table.HeaderBG;
		colors[ImGuiCol_TableBorderStrong] = Table.BorderStrong;
		colors[ImGuiCol_TableBorderLight] = Table.BorderLight;
		colors[ImGuiCol_TableRowBg] = Table.RowBG;
		colors[ImGuiCol_TableRowBgAlt] = Table.RowAltBG;
		colors[ImGuiCol_TextSelectedBg] = Text.Selected;
		colors[ImGuiCol_DragDropTarget] = DragNDrop.Target;
		colors[ImGuiCol_NavHighlight] = Navigation.Highlight;
		colors[ImGuiCol_NavWindowingHighlight] = Navigation.WindowHighlight;
		colors[ImGuiCol_NavWindowingDimBg] = Navigation.WindowDim;
		colors[ImGuiCol_ModalWindowDimBg] = Modal.WindowDim;


		
	}

};
