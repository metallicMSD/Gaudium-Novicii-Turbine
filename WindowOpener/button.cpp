#include "button.h"
#include "bmp_loader.h"

void Button::Init(float x, float y, float size, const char* imagePath)
{
	GLfloat buttonmaker[] =
	{
		//triangle one positions					//texture cords
		-1.0f * size + x, -1.0f * size + y, 0.0f,	0.0f, 0.0f, //bottom left
		 1.0f * size + x, -1.0f * size + y, 0.0f,	1.0f, 0.0f, //botton right
		 1.0f * size + x,  1.0f * size + y, 0.0f,	1.0f, 1.0f, //top right

		 //triangle two
		-1.0f * size + x, -1.0f *size + y, 0.0f,	0.0f, 0.0f, //bottom left
		1.0f * size + x, 1.0f * size + y, 0.0f,		1.0f, 1.0f, //top right
		-1.0f * size + x, 1.0f * size + y, 0.0f,	0.0f, 1.0f, //top left


	};


	buttonVAO.Bind();
	buttonVBO = new VBO(buttonmaker, sizeof(buttonmaker));
	buttonVAO.LinkAttrib(*buttonVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	buttonVAO.LinkAttrib(*buttonVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float))); //TEXTURE
	buttonVAO.Unbind();
	buttonVBO->Unbind();

	//bounds for hit testing
	minX = - 1.0f * size + x;
	maxX = 1.0f * size + x;
	minY = -1.0f * size + y;
	maxY = 1.0f * size + y;

	//IMAGE TEXTURE
	int imgWidth, imgHeight;
	std::vector<uint8_t> pixels;
	loadBMP(imagePath, imgWidth, imgHeight, pixels);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Button::Update(GLFWwindow* window, int windowWidth, int windowHeight)
{
	if (!isVisible) { hovering = false; clicking = false; return; }
	

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float ndcX = (2.0f * (float)mouseX) / windowWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * (float)mouseY) / windowHeight;

	hovering = (ndcX >= minX && ndcX <= maxX && ndcY >= minY && ndcY <= maxY);
	clicking = hovering && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

void Button::Render(Shader& sharedShader)
{
	if (!isVisible) return;
	glm::vec4 color;
	if (clicking)
		color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	else if (hovering)
		color = glm::vec4(1.2f, 1.2f, 1.2f, 1.0f);
	else
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	GLint colorLoc = glGetUniformLocation(sharedShader.ID, "uColor");
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	//TEXTURE
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint texLoc = glGetUniformLocation(sharedShader.ID, "buttonTexture");
	glUniform1i(texLoc, 0);
	//TEXTURE

	buttonVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	buttonVAO.Unbind();
}

void Button::Cleanup()
	{
	buttonVAO.Delete();
	if (buttonVBO) { buttonVBO->Delete(); delete buttonVBO; buttonVBO = nullptr; }
	glDeleteTextures(1, &textureID);
	}
