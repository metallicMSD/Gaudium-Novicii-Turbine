
#version 330 core
layout (location = 0) in vec3 aPos;     //unsure
layout (location = 2) in vec2 aTexCoord; //tells it about the texture.
out vec2 TexCoord; //output variable to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   TexCoord = aTexCoord;	
}
