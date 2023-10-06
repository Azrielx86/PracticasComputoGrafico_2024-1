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
	struct SpotLightVariables
	{
		glm::vec3 rgb;
		GLfloat   aIntensity;
		GLfloat   dIntensity;
		glm::vec3 pos;
		glm::vec3 dir;
		GLfloat   con;
		GLfloat   lin;
		GLfloat   exp;
		GLfloat   edg;
	};

  public:
	Gui();
	void Init(Window &window);
	void StartLoop();
	void EndLoop();
	void UpdatePointLightVariables(SpotLight &l);
	virtual ~Gui();
	
  private:
	Gui::SpotLightVariables varLight1{};
};

#endif // PREVIO_05_GUI_H
