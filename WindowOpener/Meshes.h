#pragma once
#include <glad/glad.h>
#include <glm//glm.hpp>
#include <vector>
#include <string>

struct Vertex {
	glm::vec3 position;
	glm::vec3 Color;
	glm::vec2 TexCoords;

};

struct Texture {
	unsigned int id;
	std::string type; //diffuse, specular, etc.
	std::string path;
};

class Shader; 

class Mesh {

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures);

	void Draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};