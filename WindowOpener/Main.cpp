#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()

{
	glfwInit(); //initializing the thing

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //a kind of package thingie, the core profile is modern.

	//now we make the window

	GLFWwindow* window = glfwCreateWindow(800, 600, "Novicii", NULL, NULL); //creates the acursed object. first "NULL" is for fullscreen, its null
																			//here because we dont want that. sencond null is unkown the guy on youtube didnt tell me what it is.
	if (window == NULL) 
	{
		std::cout << "FUCK MY LIFE IT DIDNT WORK" << std::endl;
		glfwTerminate(); //KILLS the window
		return -1; //never seen -1 before, waow. You should know i'm a huge novice.
	}

	glfwMakeContextCurrent(window); //without this, the window is useless.

	gladLoadGL();

	glViewport(0, 0, 800, 600); //0.0 is the bottom left. 

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //makes it dark blue, last one is transparency. 1.0 is no transparency.
	glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer.
	glfwSwapBuffers(window); //see's the colors.


	while (!glfwWindowShouldClose(window)) //windows doesnt close by itself unless we make it to or somehing else does
	{
		glfwSwapBuffers(window); //tells it to
		glfwPollEvents();		 //process the thing
	}

	glfwDestroyWindow(window);

	glfwTerminate(); //KILLS the window
	return 0;
}

