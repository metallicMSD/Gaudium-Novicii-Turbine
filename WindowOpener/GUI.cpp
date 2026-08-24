#include "GUI.h"

void GUI::Init()
{
	sharedShader = new Shader("guiShadervert.txt", "guiShaderfrag.txt");
}

void GUI::AddButton(float x, float y, float size)
{
	Button b;
	b.Init(x, y, size);
	buttons.push_back(b);
}

void GUI::Update(GLFWwindow* window, int windowWidth, int windowHeight)
{
	for (auto& Button : buttons)
		Button.Update(window, windowWidth, windowHeight);
}

void GUI::Render()
{
	glDisable(GL_DEPTH_TEST);

	sharedShader->Activate();
	for (auto& Button : buttons)
		Button.Render(*sharedShader);

	glEnable(GL_DEPTH_TEST);
}

//im listening to teen suicide as i write this
//its making me feel depressed.
// i hate living in such a shithole.
//8:39pm august 23rd 2026

void GUI::Cleanup()
{
	for (auto& Button : buttons)
		Button.Cleanup();

	if (sharedShader) { sharedShader->Delete(); delete sharedShader; sharedShader = nullptr;}
}
