#version 330 core
uniform sampler2D u_Texture;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    FragColor = texture(u_Texture, TexCoord); //this is what loads the texture.
}