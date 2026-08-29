#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include <vector>
#include "Button.h"
#include "shaderClass.h"

class GUI
{
public:
	void Init();
	int AddButton(float x, float y, float size, const char* imagePath);
	int AddDropdown(int triggerButton, const std::vector<int>& itemButtonIndices);
	void Update(GLFWwindow* window, int windowWidth, int windowHeight);
	void Render();
	void Cleanup();

	bool IsButtonClicked(int index) const;
	bool IsButtonHovered(int index) const;

private:
	std::vector<Button> buttons;
	Shader* sharedShader = nullptr;

	struct Dropdown {
		int triggerIndex;
		std::vector<int>itemIndices;
		bool open = false;
	};
	std::vector<Dropdown> dropdowns;
};
#endif