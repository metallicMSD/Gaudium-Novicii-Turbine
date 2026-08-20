#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

//no z axis at the moment.
//x leftmost window is -1, rightmost is 1. y lowermost is -1, uppermost is 1.

int main()

{
	glfwInit(); //initializing the thing

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //a kind of package thingie, the core profile is modern.


	GLfloat vertices[] = {
		-0.5f , -0.5f * float(sqrt(3)) / 3, 0.0f, //left
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //right
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f //top

		//all cordinates are between -1 and 1.
	};

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


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates the shader. GLuint is an unsigned int.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //1 is the number of shaders we are passing in. NULL is for length.
	glCompileShader(vertexShader); 

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
	glCompileShader(fragmentShader); 

	GLuint shaderProgram = glCreateProgram(); 

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);		//something to do with wrapping the shaders.
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VBO
	GLuint VBO;

	glGenBuffers(1, &VBO); //generates the buffer. 1 is the number of buffers we want to generate. &VBO is the address of the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds the buffer. GL_ARRAY_BUFFER is the type of buffer we are binding.

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copies the data to the buffer.
	//theres static, drawn once, dynamic, draw many times, and stream, draw once and discard!
	// theres also draw, read and copy.
	
	 
	
	//VBO

	//VAO

	GLuint VAO; 

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //0 is the index of the vertex attribute. 3 is how many values per vertex 
																				  //GL_FLOAT is the type of values. GL_FALSE is whether to normalize the data. 
																				  // 3 * sizeof(float) is the stride(amount of data between each vertex). (void*)0 is the offset.
	glEnableVertexAttribArray(0); //enables the vertex attribute.

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinds the buffer.
	glBindVertexArray(0); //unbinds the vertex array.

	//order here is extremely important.

	//VAO

	//GLuint textureID;																							
	//glGenTextures(1, &textureID);

	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.data());


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //makes it dark blue, last one is transparency. 1.0 is no transparency.
	glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer.
	glfwSwapBuffers(window); //see's the colors.


	while (!glfwWindowShouldClose(window)) //windows doesnt close by itself unless we make it to or somehing else does
	{

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); //draws the triangle. 0 is the starting index. 3 is the number of vertices to draw.
		
		glfwSwapBuffers(window); //tells it to
		glfwPollEvents();		 //process the thing
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate(); //KILLS the window
	return 0;
}

