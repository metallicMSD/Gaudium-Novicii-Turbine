#ifndef BUTTON_CLASS_H
#define BUTTON_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"

class Button
{

public:
	//call once 
	void Init(float x, float y, float size, const char* imagePath); //allows customization

	//call every frame. updated when hovering
	void Update(GLFWwindow* window, int windowWidth, int windowheight);

	//call every frame, draws the accursed object.
	void Render(Shader& sharedShader); //with this, you can share a shader.

	void SetVisible(bool visible) { isVisible = visible; }
	bool Isvisible() const { return isVisible; }

	void Cleanup();

	bool isClicked() const { return clicking; }
	bool isHovered() const { return hovering; }

private:
	VAO buttonVAO;
	VBO* buttonVBO = nullptr;
	GLuint textureID = 0; //FOR TEXTURES

	float minX = 0.0f, maxX = 0.0f, minY = 0.0f, maxY = 0.0f;

	bool hovering = false;
	bool clicking = false;
		bool isVisible = true;

};

#endif 

