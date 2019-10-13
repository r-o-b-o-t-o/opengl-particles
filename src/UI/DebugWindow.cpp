#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

#include "Application.h"
#include "UI/DebugWindow.h"

DebugWindow::DebugWindow(Application* app) :
		app(app),
		framerate(0),
		vsync(false),
		wasVSyncEnabled(false) {

	this->applyVSyncValue();
}

void DebugWindow::draw() {
	const ImVec2 size(170, 78);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(this->app->getScreenWidth() - size.x - 8, 8));
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize);

	ImGui::Text("Framerate: %d fps", this->framerate);
	ImGui::Checkbox("VSync", &this->vsync);
	if (this->wasVSyncEnabled != this->vsync) {
		this->applyVSyncValue();
		this->wasVSyncEnabled = this->vsync;
	}

	ImGui::End();
}

void DebugWindow::applyVSyncValue() const {
	glfwSwapInterval(this->vsync ? 1 : 0);
}
