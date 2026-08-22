#include "GUI.h"

float scale = 0.2f;
float offsetX = 0.7f;
float offsetY = 0.7f;

GLfloat triangle[] =
{
    -0.5f * scale + offsetX, (-0.5f * float(sqrt(3)) / 3) * scale + offsetY, 0.0f,
     0.5f * scale + offsetX, (-0.5f * float(sqrt(3)) / 3) * scale + offsetY, 0.0f,
     0.0f * scale + offsetX, (0.5f * float(sqrt(3)) * 2 / 3) * scale + offsetY, 0.0f
};
void GUI::Init()
{
    guiShader = new Shader("GUIShader.vert", "GUIShader.frag");

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    guiVAO.Bind();
    guiVBO = new VBO(triangle, sizeof(triangle));
    guiVAO.LinkAttrib(*guiVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    guiVAO.Unbind();
    guiVBO->Unbind();

    vertexCount = 3;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void GUI::Render()
{
    glDisable(GL_DEPTH_TEST); //makes the gui move with the camera

    guiShader->Activate();
    guiVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    guiVAO.Unbind();

    glEnable(GL_DEPTH_TEST);
}

void GUI::Cleanup()
{
    guiVAO.Delete();
    if (guiVBO) { guiVBO->Delete(); delete guiVBO; guiVBO = nullptr; }
    if (guiShader) { guiShader->Delete(); delete guiShader; guiShader = nullptr; }
}