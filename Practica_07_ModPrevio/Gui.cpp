//
// Created by edgar on 10/5/2023.
//

#include "Gui.h"
Gui::Gui()
{
	varLight1.dir        = {0.0f, -5.0f, 0.0f};
	varLight1.con        = 1.0f;
	varLight1.lin        = 0.1f;
	varLight1.exp        = 0.0f;
	varLight1.edg        = 15.0f;
	varLight1.aIntensity = 1.0f;
	varLight1.dIntensity = 2.0f;
	varLight1.rgb        = {1.0f, 1.0f, 0.0f};
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

	ImGui::Text("Spot Light");
}

void Gui::UpdatePointLightVariables(SpotLight &l)
{
	ImGui::ColorEdit3("Light Color", reinterpret_cast<float *>(&varLight1.rgb), ImGuiColorEditFlags_Float);
	ImGui::InputFloat("Exponent", &varLight1.exp, 0.001f, 0.01f);
	ImGui::InputFloat("Linear", &varLight1.lin, 0.001f, 0.01f);
	ImGui::InputFloat("Constant", &varLight1.con, 0.01f, 0.1f);
	ImGui::InputFloat("Edge", &varLight1.edg, 0.1f, 5.0f);
	ImGui::InputFloat("Ambient", &varLight1.aIntensity, 0.1f, 1.0f);
	ImGui::InputFloat("Diffuse", &varLight1.dIntensity, 0.1f, 1.0f);
	ImGui::InputFloat("Direction x", &varLight1.dir.x, 0.1f, 1.0f);
	ImGui::InputFloat("Direction y", &varLight1.dir.y, 0.1f, 1.0f);
	ImGui::InputFloat("Direction z", &varLight1.dir.z, 0.1f, 1.0f);
	ImGui::InputFloat("Position x", &varLight1.pos.x, 0.1f, 1.0f);
	ImGui::InputFloat("Position y", &varLight1.pos.y, 0.1f, 1.0f);
	ImGui::InputFloat("Position z", &varLight1.pos.z, 0.1f, 1.0f);

	l.SetExp(varLight1.exp);
	l.SetLinear(varLight1.lin);
	l.SetConstant(varLight1.con);
	l.SetDirection(varLight1.dir);
	l.SetEdg(varLight1.edg);
	l.SetColor(varLight1.rgb);
	l.SetPos(varLight1.pos);
	//	l.SetIntensity(varLight1.aIntensity, varLight1.dIntensity);
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
