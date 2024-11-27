#pragma once

#include "glm/glm.hpp"
#include<vector>

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness{1.0f};
	float Metallness;
	glm::vec3 EmissionColor{ 0.0f };
	float EmissionValue{ 0.0f };

	glm::vec3 GetEmissive() const { return EmissionColor * EmissionValue; }
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
	glm::vec3 SkyColor{ 0.275f, 0.392f, 0.510f };
	float Gravity{ -9.83 };
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};

