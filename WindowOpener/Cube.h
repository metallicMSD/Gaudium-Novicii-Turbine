#pragma once
#include <glad/glad.h>
#include <glm//glm.hpp>
#include <vector>
#include <string>
#include "Meshes.h"
#include <utility>

inline std::pair<std::vector<Vertex>, std::vector<GLuint>> MakeCube(glm::vec3 center, float halfSize)
{
	float h = halfSize;
	glm::vec3 c = center;

	std::vector<Vertex> verts = {
		//front
		{ {c.x - h, c.y - h, c.z + h}, {1,1,1}, {0,0} },
		{ {c.x + h, c.y - h, c.z + h}, {1,1,1}, {1,0} },
		{ {c.x + h, c.y + h, c.z + h}, {1,1,1}, {1,1} },
		{ {c.x - h, c.y + h, c.z + h}, {1,1,1}, {0,1} },
		//back
		{ {c.x + h, c.y - h, c.z - h}, {1,1,1}, {0,0} },
		{ {c.x - h, c.y - h, c.z - h}, {1,1,1}, {1,0} },
		{ {c.x - h, c.y + h, c.z - h}, {1,1,1}, {1,1} },
		{ {c.x + h, c.y + h, c.z - h}, {1,1,1}, {0,1} },
		//left
		{ {c.x - h, c.y - h, c.z - h}, {1,1,1}, {0,0} },
		{ {c.x - h, c.y - h, c.z + h}, {1,1,1}, {1,0} },
		{ {c.x - h, c.y + h, c.z + h}, {1,1,1}, {1,1} },
		{ {c.x - h, c.y + h, c.z - h}, {1,1,1}, {0,1} },
		//right
		{ {c.x + h, c.y - h, c.z + h}, {1,1,1}, {0,0} },
		{ {c.x + h, c.y - h, c.z - h}, {1,1,1}, {1,0} },
		{ {c.x + h, c.y + h, c.z - h}, {1,1,1}, {1,1} },
		{ {c.x + h, c.y + h, c.z + h}, {1,1,1}, {0,1} },
		//top
		{ {c.x - h, c.y + h, c.z + h}, {1,1,1}, {0,0} },
		{ {c.x + h, c.y + h, c.z + h}, {1,1,1}, {1,0} },
		{ {c.x + h, c.y + h, c.z - h}, {1,1,1}, {1,1} },
		{ {c.x - h, c.y + h, c.z - h}, {1,1,1}, {0,1} },
		//bottom
		{ {c.x - h, c.y - h, c.z - h}, {1,1,1}, {0,0} },
		{ {c.x + h, c.y - h, c.z - h}, {1,1,1}, {1,0} },
		{ {c.x + h, c.y - h, c.z + h}, {1,1,1}, {1,1} },
		{ {c.x - h, c.y - h, c.z + h}, {1,1,1}, {0,1} },
	};

	std::vector<GLuint> idx;
	for (GLuint face = 0; face < 6; face++)
	{
		GLuint base = face * 4;
		idx.insert(idx.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
	}

	return { verts, idx };
}