#pragma once

#include "glm/glm.hpp"
#include<vector>

struct Material
{
	glm::vec3 Albedo{ glm::vec3(1.0f) };
	float Roughness{1.0f};
	float Metallness;
};

struct Sphere
{
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	float Radius = 0.5f;
	int MatIdx{ 0 };

	// Physics
	glm::vec3 Velocity{ 0.0f, 0.0f, 0.0f };
	float Density{ 1.0f };
};
 
struct Scene
{
	glm::vec3 SkyColor{ 0.5f, 0.7f, 0.95f };
	float Gravity{ -9.83 };
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};

