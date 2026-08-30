#include "GUI.h"

void GUI::Init()
{
	sharedShader = new Shader("guiShadervert.txt", "guiShaderfrag.txt");
}

int GUI::AddButton(float x, float y, float size, const char* imagePath)
{
	Button b;
	b.Init(x, y, size, imagePath);
	buttons.push_back(b);
	return (int)buttons.size() - 1; //button indexxxxxxx
}

int GUI::AddDropdown(int triggerButton, const std::vector<int>& ItemButtonIndices)
{
	Dropdown d;
	d.triggerIndex = triggerButton;
	d.itemIndices = ItemButtonIndices;
	d.open = false;

	for (int idx : ItemButtonIndices)
		buttons[idx].SetVisible(false);

	dropdowns.push_back(d);
	return (int)dropdowns.size() - 1;
}

void GUI::Update(GLFWwindow* window, int windowWidth, int windowHeight)
{
	for (auto& Button : buttons)
		Button.Update(window, windowWidth, windowHeight);

	for (auto& d : dropdowns)
	{
		if (buttons[d.triggerIndex].isClicked())
		{
			d.open = !d.open;
			for (int idx : d.itemIndices)
				buttons[idx].SetVisible(d.open);
		}
	}
}

void GUI::Render()
{
	glDisable(GL_DEPTH_TEST);

	sharedShader->Activate();
	for (auto& Button : buttons) //"Button" here might lead to syntax errors.
		Button.Render(*sharedShader);

	glEnable(GL_DEPTH_TEST);
}

//im listening to teen suicide as i write this
//its making me feel depressed.
// i hate living in such a shithole.
//8:39pm august 23rd 2026

bool GUI::IsButtonClicked(int index) const
{
	if (index < 0 || index >= (int)buttons.size()) return false;
	return buttons[index].isClicked();
}

bool GUI::IsButtonHovered(int index) const
{
	if (index < 0 || index >= (int)buttons.size()) return false;
	return buttons[index].isHovered();
}

bool GUI::IsButtonJustClicked(int index) const
{
	if (index < 0 || index >= (int)buttons.size()) return false;
	return buttons[index].isJustClicked();
}

void GUI::Cleanup()
{
	for (auto& Button : buttons)
		Button.Cleanup();

	if (sharedShader) { sharedShader->Delete(); delete sharedShader; sharedShader = nullptr;}
}
