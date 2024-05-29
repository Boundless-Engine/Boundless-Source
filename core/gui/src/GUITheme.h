#pragma once

#include "pch.h"
#include "imgui.h"

namespace Boundless {
	namespace Graphics {

		struct GuiTheme {
			GuiTheme() = default;
			GuiTheme(ImGuiStyle& style) {

				Global.Alpha = style.Alpha;
				Global.DisabledAlpha = style.DisabledAlpha;
				Window.Padding = style.WindowPadding;
				Window.Rounding = style.WindowRounding;
				Window.BorderSize = style.WindowBorderSize;
				Window.MinSize = style.WindowMinSize;
				Window.TitleAlign = style.WindowTitleAlign;
				Window.MenuButtonPosition = style.WindowMenuButtonPosition;
				ChildWindows.Rounding = style.ChildRounding;
				ChildWindows.BorderSize = style.ChildBorderSize;
				Popups.BorderSize = style.PopupBorderSize;
				Popups.Rounding = style.PopupRounding;
				Frame.Padding = style.FramePadding;
				Frame.Rounding = style.FrameRounding;
				Frame.BorderSize = style.FrameBorderSize;
				Widget.ItemSpacing = style.ItemSpacing;
				Widget.ItemInnerSpacing = style.ItemInnerSpacing;
				Widget.CellPadding = style.CellPadding;
				Widget.TouchExtraPadding = style.TouchExtraPadding;
				Widget.IndentSpacing = style.IndentSpacing;
				Widget.ColumnsMinSpacing = style.ColumnsMinSpacing;
				Scrollbar.Size = style.ScrollbarSize;
				Scrollbar.Rounding = style.ScrollbarRounding;
				Scrollbar.Grab.MinSize = style.GrabMinSize;
				Scrollbar.Grab.Rounding = style.GrabRounding;
				Global.LogSliderDeadzone = style.LogSliderDeadzone;
				Tab.Rounding = style.TabRounding;
				Tab.BorderSize = style.TabBorderSize;
				Tab.MinWidthForCloseButton = style.TabMinWidthForCloseButton;
				Global.TableAngledHeadersAngle = style.TableAngledHeadersAngle;
				Global.ColorButtonPosition = style.ColorButtonPosition;
				Global.ButtonTextAlign = style.ButtonTextAlign;
				Global.SelectableTextAlign = style.SelectableTextAlign;
				Seperator.TextBorderSize = style.SeparatorTextBorderSize;
				Seperator.TextAlign = style.SeparatorTextAlign;
				Seperator.TextPadding = style.SeparatorTextPadding;
				Global.DisplayWindowPadding = style.DisplayWindowPadding;
				Global.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding;
				Dock.SeparatorSize = style.DockingSeparatorSize;
				Global.MouseCursorScale = style.MouseCursorScale;
				Global.AntiAliasedLines = style.AntiAliasedLines;
				Global.AntiAliasedFill = style.AntiAliasedFill;
				Global.AntiAliasedLinesUseTex = style.AntiAliasedLinesUseTex;
				Global.CurveTessellationTol = style.CurveTessellationTol;
				Global.CircleTessellationMaxError = style.CircleTessellationMaxError;

				ImVec4* colors = style.Colors;

				Text.Default = colors[ImGuiCol_Text];
				Text.Disabled = colors[ImGuiCol_TextDisabled];
				Window.Background = colors[ImGuiCol_WindowBg];
				ChildWindows.Background = colors[ImGuiCol_ChildBg];
				Popups.Background = colors[ImGuiCol_PopupBg];
				Border.Color = colors[ImGuiCol_Border];
				Border.ShadowColor = colors[ImGuiCol_BorderShadow];
				Frame.Background = colors[ImGuiCol_FrameBg];
				Frame.Hover = colors[ImGuiCol_FrameBgHovered];
				Frame.Active = colors[ImGuiCol_FrameBgActive];
				Title.Background = colors[ImGuiCol_TitleBg];
				Title.Active = colors[ImGuiCol_TitleBgActive];
				Title.Collapsed = colors[ImGuiCol_TitleBgCollapsed];
				Menubar.Background = colors[ImGuiCol_MenuBarBg];
				Scrollbar.Background = colors[ImGuiCol_ScrollbarBg];
				Scrollbar.Grab.Background = colors[ImGuiCol_ScrollbarGrab];
				Scrollbar.Grab.Hover = colors[ImGuiCol_ScrollbarGrabHovered];
				Scrollbar.Grab.Active = colors[ImGuiCol_ScrollbarGrabActive];
				Inputs.CheckMark.Foreground = colors[ImGuiCol_CheckMark];
				Inputs.Slider.Grab = colors[ImGuiCol_SliderGrab];
				Inputs.Slider.GrabActive = colors[ImGuiCol_SliderGrabActive];
				Inputs.Button.Background = colors[ImGuiCol_Button];
				Inputs.Button.Hover = colors[ImGuiCol_ButtonHovered];
				Inputs.Button.Active = colors[ImGuiCol_ButtonActive];
				Header.Background = colors[ImGuiCol_Header];
				Header.Hover = colors[ImGuiCol_HeaderHovered];
				Header.Active = colors[ImGuiCol_HeaderActive];
				Seperator.Background = colors[ImGuiCol_Separator];
				Seperator.Hover = colors[ImGuiCol_SeparatorHovered];
				Seperator.Active = colors[ImGuiCol_SeparatorActive];
				ResizeGrip.Background = colors[ImGuiCol_ResizeGrip];
				ResizeGrip.Hover = colors[ImGuiCol_ResizeGripHovered];
				ResizeGrip.Active = colors[ImGuiCol_ResizeGripActive];
				Tab.Background = colors[ImGuiCol_Tab];
				Tab.Hover = colors[ImGuiCol_TabHovered];
				Tab.Active = colors[ImGuiCol_TabActive];
				Tab.Unfocused = colors[ImGuiCol_TabUnfocused];
				Tab.UnfocusedActive = colors[ImGuiCol_TabUnfocusedActive];
				Dock.Preview = colors[ImGuiCol_DockingPreview];
				Dock.EmptyBG = colors[ImGuiCol_DockingEmptyBg];
				Graph.Lines.Background = colors[ImGuiCol_PlotLines];
				Graph.Lines.Hover = colors[ImGuiCol_PlotLinesHovered];
				Graph.Bars.Background = colors[ImGuiCol_PlotHistogram];
				Graph.Bars.Hover = colors[ImGuiCol_PlotHistogramHovered];
				Table.HeaderBG = colors[ImGuiCol_TableHeaderBg];
				Table.BorderStrong = colors[ImGuiCol_TableBorderStrong];
				Table.BorderLight = colors[ImGuiCol_TableBorderLight];
				Table.RowBG = colors[ImGuiCol_TableRowBg];
				Table.RowAltBG = colors[ImGuiCol_TableRowBgAlt];
				Text.Selected = colors[ImGuiCol_TextSelectedBg];
				DragNDrop.Target = colors[ImGuiCol_DragDropTarget];
				Navigation.Highlight = colors[ImGuiCol_NavHighlight];
				Navigation.WindowHighlight = colors[ImGuiCol_NavWindowingHighlight];
				Navigation.WindowDim = colors[ImGuiCol_NavWindowingDimBg];
				Modal.WindowDim = colors[ImGuiCol_ModalWindowDimBg];

			}

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
				ImVec4 Hover = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
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
				float       Size = 2;
				float       Rounding = 5;

				struct {
					ImVec4 Background = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
					ImVec4 Hover = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
					ImVec4 Active = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
					float       MinSize = 1;
					float       Rounding = 5;
				} Grab;
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
			} Widget;


		};

	}
}

/* DEFINE SERIALIZE/DESERIALIZE METHODS */
#include "json.hpp"

namespace nlohmann {
	template<>
	struct adl_serializer<ImVec2> {
		static void to_json(json& j, const ImVec2& v)
		{
			j = json{ {"x", v.x}, {"y", v.y} };
		}
		static void from_json(const json& j, ImVec2& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
		}
	};
	template<>
	struct adl_serializer<ImVec4> {
		static void to_json(json& j, const ImVec4& v)
		{
			j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z},{ "w", v.w } };
		}
		static void from_json(const json& j, ImVec4& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
			j.at("z").get_to(v.z);
			j.at("w").get_to(v.w);
		}
	};

	template<>
	struct adl_serializer<Boundless::Graphics::GuiTheme>
	{
		static void to_json(json& j, const Boundless::Graphics::GuiTheme& t) {
			j = json
			{
				{ "Text", {
						{ "Default",	t.Text.Default },
						{ "Disabled",	t.Text.Disabled },
						{ "Selected",	t.Text.Selected }
				} },
				{ "Window", {
						{ "Background",			t.Window.Background },
						{ "Padding",			t.Window.Padding },
						{ "Rounding",			t.Window.Rounding },
						{ "BorderSize",			t.Window.BorderSize},
						{ "MinSize",			t.Window.MinSize},
						{ "TitleAlign",			t.Window.TitleAlign},
						{ "MenuButtonPosition", t.Window.MenuButtonPosition }
				} },
				{ "ChildWindows", {
						{"Background",	t.ChildWindows.Background },
						{"Rounding",	t.ChildWindows.Rounding },
						{"BorderSize",	t.ChildWindows.BorderSize },
				} },
				{ "Popups", {
					{ "Background", t.Popups.Background },
					{ "Rounding",	t.Popups.Rounding },
					{ "BorderSize", t.Popups.BorderSize},
				} },
				{ "Border", {
					{ "Color",			t.Border.Color },
					{ "ShadowColor",	t.Border.ShadowColor },
				}},
				{ "Frame", {
					{ "Background", t.Frame.Background },
					{ "Hover",	t.Frame.Hover },
					{ "Active",		t.Frame.Active },
					{ "Padding",	t.Frame.Padding },
					{ "Rounding",	t.Frame.Rounding },
					{ "BorderSize", t.Frame.BorderSize },
				} },
				{ "Title", {
					{"Background",	t.Title.Background },
					{"Active",		t.Title.Active	},
					{"Collapsed",	t.Title.Collapsed }
				} },
				{ "Menubar", {
					{"Background", t.Menubar.Background}
				} },
				{ "Scrollbar", {
					{"Background",	t.Scrollbar.Background},
					{"Size",		t.Scrollbar.Size},
					{"Rounding",	t.Scrollbar.Rounding},
					{"Grab", {
						{ "Background", t.Scrollbar.Grab.Background },
						{ "Hover",		t.Scrollbar.Grab.Hover },
						{ "Active",		t.Scrollbar.Grab.Active },
						{ "MinSize",	t.Scrollbar.Grab.MinSize },
						{ "Rounding",	t.Scrollbar.Grab.Rounding },
					}}
				} },
				{ "Inputs", {
					{"CheckMark", {
						{"Foreground", t.Inputs.CheckMark.Foreground }
					}},
					{"Slider", {
						{"Grab",		t.Inputs.Slider.Grab },
						{"GrabActive",	t.Inputs.Slider.GrabActive },
					}},
					{"Button", {
						{"Background",	t.Inputs.Button.Background },
						{"Hover",		t.Inputs.Button.Hover },
						{"Active",		t.Inputs.Button.Active }
					}}
				} },
				{ "Header", {
					{"Background",	t.Header.Background },
					{"Hover",		t.Header.Hover },
					{"Active",		t.Header.Active },
				} },
				{ "Seperator", {
					{"Background",		t.Seperator.Background },
					{"Hover",			t.Seperator.Background },
					{"Active",			t.Seperator.Background },
					{"TextBorderSize",	t.Seperator.TextBorderSize },
					{"TextAlign",		t.Seperator.TextAlign },
					{"TextPadding",		t.Seperator.TextPadding },
					} },
				{ "ResizeGrip", {
					{ "Background", t.ResizeGrip.Background },
					{ "Hover",		t.ResizeGrip.Hover},
					{ "Active",		t.ResizeGrip.Active },
				} },
				{ "Tab", {
					{ "Background",					t.Tab.Background },
					{ "Hover",						t.Tab.Hover},
					{ "Active",						t.Tab.Active },
					{ "Unfocused",					t.Tab.Unfocused },
					{ "UnfocusedActive",			t.Tab.UnfocusedActive },
					{ "Rounding",					t.Tab.Rounding },
					{ "BorderSize",				t.Tab.BorderSize },
					{ "MinWidthForCloseButton",	t.Tab.MinWidthForCloseButton },
					{ "BarBorderSize",				t.Tab.BarBorderSize },
				} },
				{ "Dock", {
					{ "Preview",		t.Dock.Preview },
					{ "EmptyBG",		t.Dock.EmptyBG },
					{ "SeperatorSize",	t.Dock.SeparatorSize },

				} },
				{ "Graph", {
					{"Lines", {
						{"Background",	t.Graph.Lines.Background},
						{"Hover",		t.Graph.Lines.Hover},
					}},
					{"Bars", {
						{"Background",	t.Graph.Bars.Background},
						{"Hover",		t.Graph.Bars.Hover},
					}}
				} },
				{ "Table", {
					{"HeaderBG",		t.Table.HeaderBG },
					{"BorderStrong",	t.Table.BorderStrong },
					{"BorderLight",		t.Table.BorderLight },
					{"RowBG",			t.Table.RowBG },
					{"RowAltBG",		t.Table.RowAltBG },
				} },
				{ "Navigation", {
					{"Highlight",		t.Navigation.Highlight},
					{"WindowHighlight", t.Navigation.WindowHighlight},
					{"WindowDim",		t.Navigation.WindowDim},
				} },
				{ "DragNDrop", {
					{"Target", t.DragNDrop.Target}
				} },
				{ "Modal", {
					{"WindowDim", t.Modal.WindowDim }
				}},
				{ "Global", {
					{"Alpha",					t.Global.Alpha},
					{"DisabledAlpha",			t.Global.DisabledAlpha},
					{"MouseCursorScale",		t.Global.MouseCursorScale},
					{"AntiAliasedLines",		t.Global.AntiAliasedLines},
					{"AntiAliasedLinesUseTex",	t.Global.AntiAliasedLinesUseTex},
					{"AntiAliasedFill",			t.Global.AntiAliasedFill},
					{"CurveTessellationTol",	t.Global.CurveTessellationTol},
					{"CircleTessellationMaxError",	t.Global.CircleTessellationMaxError},
					{"LogSliderDeadzone",	t.Global.LogSliderDeadzone},
					{"TableAngledHeadersAngle",	t.Global.TableAngledHeadersAngle},
					{"ColorButtonPosition",	t.Global.ColorButtonPosition},
					{"ButtonTextAlign",	t.Global.ButtonTextAlign},
					{"SelectableTextAlign",	t.Global.SelectableTextAlign},
					{"DisplayWindowPadding",	t.Global.DisplayWindowPadding},
					{"DisplaySafeAreaPadding",	t.Global.DisplaySafeAreaPadding},
				}},
				{ "Widgit", {
					{"ItemSpacing", t.Widget.ItemSpacing },
					{"ItemInnerSpacing", t.Widget.ItemInnerSpacing },
					{"CellPadding", t.Widget.CellPadding},
					{"TouchExtraPadding", t.Widget.TouchExtraPadding},
					{"IndentSpacing", t.Widget.IndentSpacing},
					{"ColumnsMinSpacing", t.Widget.ColumnsMinSpacing},
				}},

			};
		}

		static void from_json(const json& j, Boundless::Graphics::GuiTheme& t) {
			// text
			j.at("Text").at("Default").get_to(t.Text.Default);
			j.at("Text").at("Disabled").get_to(t.Text.Disabled);
			j.at("Text").at("Selected").get_to(t.Text.Selected);
			// window
			j.at("Window").at("Background").get_to(t.Window.Background);
			j.at("Window").at("Padding").get_to(t.Window.Padding);
			j.at("Window").at("Rounding").get_to(t.Window.Rounding);
			j.at("Window").at("BorderSize").get_to(t.Window.BorderSize);
			j.at("Window").at("MinSize").get_to(t.Window.MinSize);
			j.at("Window").at("TitleAlign").get_to(t.Window.TitleAlign);
			j.at("Window").at("MenuButtonPosition").get_to(t.Window.MenuButtonPosition);
			// child windows
			j.at("ChildWindows").at("Background").get_to(t.ChildWindows.Background);
			j.at("ChildWindows").at("BorderSize").get_to(t.ChildWindows.BorderSize);
			j.at("ChildWindows").at("Rounding").get_to(t.ChildWindows.Rounding);
			// popups
			j.at("Popups").at("Background").get_to(t.Popups.Background);
			j.at("Popups").at("Rounding").get_to(t.Popups.Rounding);
			j.at("Popups").at("BorderSize").get_to(t.Popups.BorderSize);

			// border
			j.at("Border").at("Color").get_to(t.Border.Color);
			j.at("Border").at("ShadowColor").get_to(t.Border.ShadowColor);
			// frame
			j.at("Frame").at("Background").get_to(t.Frame.Background);
			j.at("Frame").at("Hover").get_to(t.Frame.Hover);
			j.at("Frame").at("Active").get_to(t.Frame.Active);
			j.at("Frame").at("Padding").get_to(t.Frame.Padding);
			j.at("Frame").at("Rounding").get_to(t.Frame.Rounding);
			j.at("Frame").at("BorderSize").get_to(t.Frame.BorderSize);
			// title
			j.at("Title").at("Background").get_to(t.Title.Background);
			j.at("Title").at("Active").get_to(t.Title.Active);
			j.at("Title").at("Collapsed").get_to(t.Title.Collapsed);
			// menubar
			j.at("Menubar").at("Background").get_to(t.Menubar.Background);

			// scrollbar
			j.at("Scrollbar").at("Background").get_to(t.Scrollbar.Background);
			j.at("Scrollbar").at("Size").get_to(t.Scrollbar.Size);
			j.at("Scrollbar").at("Rounding").get_to(t.Scrollbar.Rounding);

			j.at("Scrollbar").at("Grab").at("Background").get_to(t.Scrollbar.Grab.Background);
			j.at("Scrollbar").at("Grab").at("Hover").get_to(t.Scrollbar.Grab.Hover);
			j.at("Scrollbar").at("Grab").at("Active").get_to(t.Scrollbar.Grab.Active);
			j.at("Scrollbar").at("Grab").at("MinSize").get_to(t.Scrollbar.Grab.MinSize);
			j.at("Scrollbar").at("Grab").at("Rounding").get_to(t.Scrollbar.Grab.Rounding);

			// inputs
			j.at("Inputs").at("CheckMark").at("Foreground").get_to(t.Inputs.CheckMark.Foreground);

			j.at("Inputs").at("Slider").at("Grab").get_to(t.Inputs.Slider.Grab);
			j.at("Inputs").at("Slider").at("GrabActive").get_to(t.Inputs.Slider.GrabActive);

			j.at("Inputs").at("Button").at("Background").get_to(t.Inputs.Button.Background);
			j.at("Inputs").at("Button").at("Hover").get_to(t.Inputs.Button.Hover);
			j.at("Inputs").at("Button").at("Active").get_to(t.Inputs.Button.Active);
			// header
			j.at("Header").at("Background").get_to(t.Header.Background);
			j.at("Header").at("Hover").get_to(t.Header.Hover);
			j.at("Header").at("Active").get_to(t.Header.Active);
			// seperator 
			j.at("Seperator").at("Background").get_to(t.Seperator.Background);
			j.at("Seperator").at("Hover").get_to(t.Seperator.Hover);
			j.at("Seperator").at("Active").get_to(t.Seperator.Active);
			j.at("Seperator").at("TextBorderSize").get_to(t.Seperator.TextBorderSize);
			j.at("Seperator").at("TextAlign").get_to(t.Seperator.TextAlign);
			j.at("Seperator").at("TextPadding").get_to(t.Seperator.TextPadding);
			// resizegrip
			j.at("ResizeGrip").at("Background").get_to(t.ResizeGrip.Background);
			j.at("ResizeGrip").at("Hover").get_to(t.ResizeGrip.Hover);
			j.at("ResizeGrip").at("Active").get_to(t.ResizeGrip.Active);
			// tab
			j.at("Tab").at("Background").get_to(t.Tab.Background);
			j.at("Tab").at("Hover").get_to(t.Tab.Hover);
			j.at("Tab").at("Active").get_to(t.Tab.Active);
			j.at("Tab").at("Unfocused").get_to(t.Tab.Unfocused);
			j.at("Tab").at("UnfocusedActive").get_to(t.Tab.UnfocusedActive);
			j.at("Tab").at("Rounding").get_to(t.Tab.Rounding);
			j.at("Tab").at("BorderSize").get_to(t.Tab.BorderSize);
			j.at("Tab").at("MinWidthForCloseButton").get_to(t.Tab.MinWidthForCloseButton);
			j.at("Tab").at("BarBorderSize").get_to(t.Tab.BarBorderSize);
			// dock
			j.at("Dock").at("EmptyBG").get_to(t.Dock.EmptyBG);
			j.at("Dock").at("Preview").get_to(t.Dock.Preview);
			j.at("Dock").at("SeperatorSize").get_to(t.Dock.SeparatorSize);
			// graph
			// graph::lines
			j.at("Graph").at("Lines").at("Background").get_to(t.Graph.Lines.Background);
			j.at("Graph").at("Lines").at("Hover").get_to(t.Graph.Lines.Hover);
			// graph::bars
			j.at("Graph").at("Bars").at("Background").get_to(t.Graph.Bars.Background);
			j.at("Graph").at("Bars").at("Hover").get_to(t.Graph.Bars.Hover);
			// table
			j.at("Table").at("HeaderBG").get_to(t.Table.HeaderBG);
			j.at("Table").at("BorderStrong").get_to(t.Table.BorderStrong);
			j.at("Table").at("BorderLight").get_to(t.Table.BorderLight);
			j.at("Table").at("RowBG").get_to(t.Table.RowBG);
			j.at("Table").at("RowAltBG").get_to(t.Table.RowAltBG);
			// navigation
			j.at("Navigation").at("Highlight").get_to(t.Navigation.Highlight);
			j.at("Navigation").at("WindowHighlight").get_to(t.Navigation.WindowHighlight);
			j.at("Navigation").at("WindowDim").get_to(t.Navigation.WindowDim);
			// dragndrop
			j.at("DragNDrop").at("Target").get_to(t.DragNDrop.Target);
			// modal
			j.at("Modal").at("WindowDim").get_to(t.Modal.WindowDim);
			// global
			j.at("Global").at("Alpha").get_to(t.Global.Alpha);
			j.at("Global").at("DisabledAlpha").get_to(t.Global.DisabledAlpha);
			j.at("Global").at("MouseCursorScale").get_to(t.Global.MouseCursorScale);
			j.at("Global").at("AntiAliasedLines").get_to(t.Global.AntiAliasedLines);
			j.at("Global").at("AntiAliasedLinesUseTex").get_to(t.Global.AntiAliasedLinesUseTex);
			j.at("Global").at("AntiAliasedFill").get_to(t.Global.AntiAliasedFill);
			j.at("Global").at("CurveTessellationTol").get_to(t.Global.CurveTessellationTol);
			j.at("Global").at("CircleTessellationMaxError").get_to(t.Global.CircleTessellationMaxError);
			j.at("Global").at("LogSliderDeadzone").get_to(t.Global.LogSliderDeadzone);
			j.at("Global").at("TableAngledHeadersAngle").get_to(t.Global.TableAngledHeadersAngle);
			j.at("Global").at("ColorButtonPosition").get_to(t.Global.ColorButtonPosition);
			j.at("Global").at("ButtonTextAlign").get_to(t.Global.ButtonTextAlign);
			j.at("Global").at("SelectableTextAlign").get_to(t.Global.SelectableTextAlign);
			j.at("Global").at("DisplaySafeAreaPadding").get_to(t.Global.DisplaySafeAreaPadding);
			j.at("Global").at("DisplayWindowPadding").get_to(t.Global.DisplayWindowPadding);

			// widgit
			j.at("Widgit").at("ItemSpacing").get_to(t.Widget.ItemSpacing);
			j.at("Widgit").at("ItemInnerSpacing").get_to(t.Widget.ItemInnerSpacing);
			j.at("Widgit").at("CellPadding").get_to(t.Widget.CellPadding);
			j.at("Widgit").at("TouchExtraPadding").get_to(t.Widget.TouchExtraPadding);
			j.at("Widgit").at("IndentSpacing").get_to(t.Widget.IndentSpacing);
			j.at("Widgit").at("ColumnsMinSpacing").get_to(t.Widget.ColumnsMinSpacing);

		}
	};
}
