#include <iostream>
#include "GUI.h"

float scale = 0.2f;
float offsetX = 0.8f;
float offsetY = 0.8f;

GLfloat triangle[] =
{
    // Triangle 1
    -1.0f * scale + offsetX, -1.0f * scale + offsetY, 0.0f,   // bottom-left
     1.0f * scale + offsetX, -1.0f * scale + offsetY, 0.0f,   // bottom-right
     1.0f * scale + offsetX,  1.0f * scale + offsetY, 0.0f,   // top-right

     // Triangle 2
     -1.0f * scale + offsetX, -1.0f * scale + offsetY, 0.0f,   // bottom-left
      1.0f * scale + offsetX,  1.0f * scale + offsetY, 0.0f,   // top-right
     -1.0f * scale + offsetX,  1.0f * scale + offsetY, 0.0f,   // top-left
};



void GUI::Init()
{
    guiShader = new Shader("guiShadervert.txt", "guiShaderfrag.txt"); //IN THIS SPECIFIC ORDER, WONT WORK ELSEWISE. VERT THEN FRAG.

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    guiVAO.Bind();
    guiVBO = new VBO(triangle, sizeof(triangle));
    guiVAO.LinkAttrib(*guiVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    guiVAO.Unbind();
    guiVBO->Unbind();

    vertexCount = 6;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!
    //HOVER
    
    minX = -1.0f * scale + offsetX;
    maxX = 1.0f * scale + offsetX;
    minY = -1.0f * scale + offsetY;
    maxY = 1.0f * scale + offsetY;
    //HOVER
}

void GUI::Render()
{
    glDisable(GL_DEPTH_TEST); //makes the gui move with the camera

    guiShader->Activate();
    guiVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount); 
    guiVAO.Unbind();

    glEnable(GL_DEPTH_TEST);

    
    {
        glDisable(GL_DEPTH_TEST);

        guiShader->Activate();

        glm::vec4 color;
        if (isClicked)
            color = glm::vec4(0.6f, 0.0f, 0.0f, 1.0f);   //dark when click
        else if (isHovered)
            color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);   //light when hover
        else
            color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);   //neutral

        GLint colorLoc = glGetUniformLocation(guiShader->ID, "uColor");
        glUniform4fv(colorLoc, 1, glm::value_ptr(color));

        guiVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        guiVAO.Unbind();

        glEnable(GL_DEPTH_TEST);
    }

}

//HOVER
void GUI::Update(GLFWwindow* window, int windowWidth, int windowHeight)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    //square coordinates
    float ndcX = (2.0f * (float)mouseX) / windowWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)mouseY) / windowHeight;

    //checks if cursor is on the square
    isHovered = (ndcX >= minX && ndcX <= maxX && ndcY >= minY && ndcY <= maxY);

    isClicked = isHovered && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}
//HOVER


void GUI::Cleanup()
{
    guiVAO.Delete();
    if (guiVBO) { guiVBO->Delete(); delete guiVBO; guiVBO = nullptr; }
    if (guiShader) { guiShader->Delete(); delete guiShader; guiShader = nullptr; }
}