//
// Created by edgar on 10/5/2023.
//

#include "Gui.h"
Gui::Gui()
{
	varSpotL.dir        = {0.0f, -5.0f, 0.0f};
	varSpotL.constant   = 1.0f;
	varSpotL.linear     = 0.1f;
	varSpotL.exponent   = 0.0f;
	varSpotL.edge       = 15.0f;
	varSpotL.aIntensity = 1.0f;
	varSpotL.dIntensity = 2.0f;
	varSpotL.rgb        = {1.0f, 1.0f, 0.0f};

	varPointL.rgb        = {0.0f, 1.0f, 0.0f};
	varPointL.aIntensity = 0.0f;
	varPointL.dIntensity = 1.0f;
	varPointL.pos        = {-6.0f, 1.5f, -5.5f};
	varPointL.constant   = 0.3f;
	varPointL.linear     = 0.2f;
	varPointL.exponent   = 0.001f;
}
void Gui::Init(Window &window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindowPointer(), true);
	ImGui_ImplOpenGL3_Init();
}

void Gui::StartLoop()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//	ImGui::ShowDemoWindow();
}

void Gui::UpdateSpotLightVariables(SpotLight &light)
{
	if (ImGui::CollapsingHeader("Spot Light"))
	{
		ImGui::ColorEdit3("Light Color", reinterpret_cast<float *>(&varSpotL.rgb), ImGuiColorEditFlags_Float);
		ImGui::InputFloat("Exponent", &varSpotL.exponent, 0.001f, 0.01f);
		ImGui::InputFloat("Linear", &varSpotL.linear, 0.001f, 0.01f);
		ImGui::InputFloat("Constant", &varSpotL.constant, 0.01f, 0.1f);
		ImGui::InputFloat("Edge", &varSpotL.edge, 0.1f, 5.0f);
		ImGui::InputFloat("Direction x", &varSpotL.dir.x, 0.1f, 1.0f);
		ImGui::InputFloat("Direction y", &varSpotL.dir.y, 0.1f, 1.0f);
		ImGui::InputFloat("Direction z", &varSpotL.dir.z, 0.1f, 1.0f);
		ImGui::InputFloat("Position x", &varSpotL.pos.x, 0.1f, 1.0f);
		ImGui::InputFloat("Position y", &varSpotL.pos.y, 0.1f, 1.0f);
		ImGui::InputFloat("Position z", &varSpotL.pos.z, 0.1f, 1.0f);

		light.SetExp(varSpotL.exponent);
		light.SetLinear(varSpotL.linear);
		light.SetConstant(varSpotL.constant);
		light.SetDirection(varSpotL.dir);
		light.SetEdge(varSpotL.edge);
		light.SetColor(varSpotL.rgb);
		light.SetPos(varSpotL.pos);
	}

}

void Gui::UpdatePointLightVariables(PointLight &light)
{
	if (ImGui::CollapsingHeader("Point Light")) {
		ImGui::ColorEdit3("Light Color", reinterpret_cast<float *>(&varPointL.rgb), ImGuiColorEditFlags_Float);
		ImGui::InputFloat("Exponent", &varPointL.exponent, 0.001f, 0.01f);
		ImGui::InputFloat("Linear", &varPointL.linear, 0.001f, 0.01f);
		ImGui::InputFloat("Constant", &varPointL.constant, 0.01f, 0.1f);
		ImGui::InputFloat("Position x", &varPointL.pos.x, 0.1f, 1.0f);
		ImGui::InputFloat("Position y", &varPointL.pos.y, 0.1f, 1.0f);
		ImGui::InputFloat("Position z", &varPointL.pos.z, 0.1f, 1.0f);
		
		light.SetExp(varPointL.exponent);
		light.SetLinear(varPointL.linear);
		light.SetConstant(varPointL.constant);
		light.SetColor(varPointL.rgb);
		light.SetPos(varPointL.pos);
	}
}


void Gui::EndLoop()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::SetSpotLightVars(SpotLight &light)
{
	this->varSpotL.dir = light.getDirection();
	this->varSpotL.edge = light.getEdge();
	this->varSpotL.pos = light.getPosition();
	this->varSpotL.exponent = light.getExponent();
	this->varSpotL.linear = light.getLinear();
	this->varSpotL.constant = light.getConstant();
	this->varSpotL.rgb = light.getColor();
}

void Gui::SetPointLightVars(PointLight &light)
{
	this->varPointL.pos = light.getPosition();
	this->varPointL.exponent = light.getExponent();
	this->varPointL.linear = light.getLinear();
	this->varPointL.constant = light.getConstant();
	this->varPointL.rgb = light.getColor();
}
