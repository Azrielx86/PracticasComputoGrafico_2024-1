//
// Created by edgar on 10/5/2023.
//

#ifndef PREVIO_05_GUI_H
#define PREVIO_05_GUI_H

#include "SpotLight.h"
#include "Window.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm.hpp>

class Gui
{
	struct PointLightVariables
	{
		glm::vec3 rgb;
		GLfloat   aIntensity;
		GLfloat   dIntensity;
		glm::vec3 pos;
		GLfloat   constant, linear, exponent;
	};

	struct SpotLightVariables : PointLightVariables
	{
		GLfloat   aIntensity;
		GLfloat   dIntensity;
		glm::vec3 dir;
		GLfloat   edge;
	};

  public:
	Gui();
	void Init(Window &window);
	void StartLoop();
	void EndLoop();
	void UpdateSpotLightVariables(SpotLight &light);
	void UpdatePointLightVariables(PointLight &light);
	void SetSpotLightVars(SpotLight &light);
	void SetPointLightVars(PointLight &light);
	virtual ~Gui();

  private:
	Gui::SpotLightVariables  varSpotL{};
	Gui::PointLightVariables varPointL{};
};

#endif // PREVIO_05_GUI_H
