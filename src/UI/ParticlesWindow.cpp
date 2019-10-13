#include "imgui/imgui.h"

#include "UI/ParticlesWindow.h"
#include "Application.h"

ParticlesWindow::ParticlesWindow(Application* app) :
		selectedType(0),
		minParticles(10ul),
		maxParticles(N_MAX_PARTICLES),
		maxParticlesValue(0ul),
		spawnRate(-1.0f),
		app(app) {

	this->types.push_back("Confetti");
	this->types.push_back("Snowflakes");
	this->types.push_back("Fire");
}

void ParticlesWindow::draw() {
	const ImVec2 size(350, 100);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(8, 8));
	ImGui::Begin("Particles", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	std::string &currentItem = this->types[this->selectedType];
	if (ImGui::BeginCombo("##combo", currentItem.c_str())) {
		for (int n = 0; n < this->types.size(); n++) {
			bool isSelected = currentItem == this->types[n];
			if (ImGui::Selectable(this->types[n].c_str(), isSelected)) {
				this->selectedType = n;

				if (n == 0) {
					this->app->activeEmitter = &this->app->confettiEmitter;
				} else if (n == 1) {
					this->app->activeEmitter = &this->app->snowflakeEmitter;
				} else if (n == 2) {
					this->app->activeEmitter = &this->app->fireEmitter;
				}
				this->maxParticlesValue = this->app->activeEmitter->getMaxParticles();
				this->spawnRate = this->app->activeEmitter->getSpawnRate();
			}
		}
		ImGui::EndCombo();
	}

	if (this->maxParticlesValue == 0ul) {
		this->maxParticlesValue = this->app->activeEmitter->getMaxParticles();
	}
	if (ImGui::SliderScalar("Max particles", ImGuiDataType_U32, &this->maxParticlesValue, &this->minParticles, &this->maxParticles)) {
		this->app->activeEmitter->setMaxParticles(this->maxParticlesValue);
	}

	if (this->spawnRate < 0.0f) {
		this->spawnRate = this->app->activeEmitter->getSpawnRate();
	}
	if (ImGui::SliderFloat("Spawn rate", &this->spawnRate, 0.0f, static_cast<float>(this->maxParticlesValue / 5ul), "%.0f/second")) {
		this->app->activeEmitter->setSpawnRate(this->spawnRate);
	}

	ImGui::End();
}
