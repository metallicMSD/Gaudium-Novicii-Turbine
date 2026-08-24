#include "button.h"

void Button::Init(float x, float y, float size)
{
	GLfloat buttonmaker[] =
	{
		//triangle one
		-1.0f * size + x, -1.0f * size + y, 0.0f,
		 1.0f * size + x, -1.0f * size + y, 0.0f,
		 1.0f * size + x,  1.0f * size + y, 0.0f,

		 //triangle two
		-1.0f * size + x, -1.0f *size + y, 0.0f,
		1.0f * size + x, 1.0f * size + y, 0.0f,
		-1.0f * size + x, 1.0f * size + y, 0.0f,


	};


	buttonVAO.Bind();
	buttonVBO = new VBO(buttonmaker, sizeof(buttonmaker));
	buttonVAO.LinkAttrib(*buttonVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	buttonVAO.Unbind();
	buttonVBO->Unbind();

	minX = - 1.0f * size + x;
	maxX = 1.0f * size + x;
	minY = -1.0f * size + y;
	maxY = 1.0f * size + y;
}

void Button::Update(GLFWwindow* window, int windowWidth, int windowHeight)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float ndcX = (2.0f * (float)mouseX) / windowWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * (float)mouseY) / windowHeight;

	hovering = (ndcX >= minX && ndcX <= maxX && ndcY >= minY && ndcY <= maxY);
	clicking = hovering && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

void Button::Render(Shader& sharedShader)
{
	glm::vec4 color;
	if (clicking)
		color = glm::vec4(0.6f, 0.0f, 0.0f, 1.0f);
	else if (hovering)
		color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
	else
		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	GLint colorLoc = glGetUniformLocation(sharedShader.ID, "uColor");
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	buttonVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	buttonVAO.Unbind();
}

void Button::Cleanup()
	{
	buttonVAO.Delete();
	if (buttonVBO) { buttonVBO->Delete(); delete buttonVBO; buttonVBO = nullptr; }
	}
