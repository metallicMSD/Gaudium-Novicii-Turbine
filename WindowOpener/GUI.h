#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include <vector>
#include "Button.h"
#include "shaderClass.h"

class GUI
{
public:
	void Init();
	void AddButton(float x, float y, float size);
	void Update(GLFWwindow* window, int windowWidth, int windowHeight);
	void Render();
	void Cleanup();

private:
	std::vector<Button> buttons;
	Shader* sharedShader = nullptr;
};
#endif