#include "imgui/imgui.h"

#include "Application.h"
#include "UI/PlaneWindow.h"

PlaneWindow::PlaneWindow(Application* app) :
		app(app),
		size(10) {

}

void PlaneWindow::draw() {
	const ImVec2 size(170, 78);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(this->app->getScreenWidth() - size.x - 8, size.y + 8 + 8));
	ImGui::Begin("Plane", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::Checkbox("Enabled", &this->app->plane.show);
	if (ImGui::SliderInt("Size", &this->size, 1, PLANE_MAX_SIZE)) {
		this->app->plane.setSize(this->size);
	}

	ImGui::End();
}
