#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>




#include"shaderClass.h"
#include"bmp_loader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};




int main()

{
	glfwInit(); //initializing the thing

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //a kind of package thingie, the core profile is modern.	

	GLFWwindow* window = glfwCreateWindow(width, height, "Novicii", NULL, NULL); //creates the acursed object. first "NULL" is for fullscreen.

	if (window == NULL)
	{
		std::cout << "FUCK MY LIFE IT DIDNT WORK" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //without this, the window is useless.

	gladLoadGL();

	glViewport(0, 0, width, height); //0.0 is the bottom left. 



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	//stratospshere.

	int width, height;
	std::vector<uint8_t> pixels;

	loadBMP("C:\\Users\\tallm\\source\\repos\\bmp reader\\bmp reader\\stratosphereBMP.bmp", width, height, pixels);

	// Variables that help the rotation of the stratospshere
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);


	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		width,
		height,
		0,			//legacy, unimportant.
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		

	//stratospshere.

	// Main while loop
		while (!glfwWindowShouldClose(window))
		{
			// Specify the color of the background
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Tell OpenGL which Shader Program we want to use
			shaderProgram.Activate();
			// Bind the VAO so OpenGL knows to use it
			VAO1.Bind();

			//simple timer
			//maybe move above VAO1.Bind(); ?
			double crntTime = glfwGetTime();
			if (crntTime - prevTime >= 1 / 60)
			{
				rotation += 0.5f;
				prevTime = crntTime;
			}



			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			GLint texUniform = glGetUniformLocation(shaderProgram.ID, "u_Texture");
			glUniform1i(texUniform, 0);
		
			glm::mat4 model = glm::mat4(1.0); //initializing.
			glm::mat4 view = glm::mat4(1.0); //initializing.
			glm::mat4 proj = glm::mat4(1.0); //initializing.

			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); //rotates the thing
			view = glm::translate(view, glm::vec3(0.0f, -0.5f,-2.0f )); //moves the camera.
			proj = glm::perspective(glm::radians(45.0f), (float) (width / height), 0.1f, 100.0f); //in essence, FOV.

			// Outputs the matrices into the Vertex Shader
			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			// Take care of all GLFW events
			glfwPollEvents();
		}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glDeleteTextures(1, &texture_id);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}