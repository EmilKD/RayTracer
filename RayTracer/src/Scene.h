#pragma once

#include "glm/glm.hpp"
#include<vector>

struct Sphere
{
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	float Radius = 0.5f;
	glm::vec3 Albedo{ 1.0f };
	glm::vec3 Velocity{ 0.0f, 0.0f, 0.0f };
	float Density{ 1.0f };
};
 
struct Scene
{
	float Gravity{ -9.83 };
	std::vector<Sphere> Spheres;
};

