#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"

class GUI
{
public:
    //call once
    void Init();

    //calls it evert frane
    void Render();

    void Cleanup();

private:
    VAO guiVAO;
    VBO* guiVBO = nullptr;
    Shader* guiShader = nullptr;
    unsigned int vertexCount = 0;
};

#endif

