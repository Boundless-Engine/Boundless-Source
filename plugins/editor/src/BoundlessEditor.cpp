#include "BoundlessEditor.h"

constexpr const char* EDITOR_TITLE = "Boundless Editor";

#include "imgui.h"

#include "views/console.h"
#include <GUITheme.h>

namespace {
	
	I::IWindow* win;
	I::IInputHandler* input;
	I::IRasterSurface* surface;
	I::IGUI* gui;

}


static bool show_save_popup = false;

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

BReturn BoundlessEditor::Initillize()
{

	PushLayer<Console>();

	+Window::Create(prefs.Window.width, prefs.Window.height, EDITOR_TITLE, &win);
	+Input::Create(&input);
	+Graphics::RasterSurface::Create(win, &surface);

	GuiTheme theme;

	+Graphics::Gui::Create(surface, &gui, &theme, [&]() {

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
		});
	

	return SUCCESS;
}

BReturn BoundlessEditor::Shutdown()
{
	+Graphics::Gui::Destroy(&gui);
	+Graphics::RasterSurface::Destroy(&surface);
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

		for (auto& l : layers)
		{
			l->OnUpdate();
		}

		surface->BeginFrame();
		gui->Begin();
		for (auto& l : layers)
		{
			l->OnGUI();
		}

		ImGui::ShowDemoWindow();

		SaveDialogue([&](bool shouldSave) { if (shouldSave) { SavePrefs(); } application_should_run = false; });

		gui->End();
		surface->EndFrame();

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
