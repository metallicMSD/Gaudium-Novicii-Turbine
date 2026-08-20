
#version 330 core
layout (location = 0) in vec3 aPos;     //unsure
layout (location = 2) in vec2 aTexCoord; //tells it about the texture.
out vec2 TexCoord; //output variable to the fragment shader
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   TexCoord = aTexCoord;	
}
