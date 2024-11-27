#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Walnut/Random.h"
#include "glm/glm.hpp"

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

class Renderer
{
public:
	Renderer() = default;
	void Render(Scene* scene, const Camera &camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_Image; };

private:
	struct RayHitResult
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;
		uint32_t ObjectIndex;
	};

	glm::vec4 OnPixel(uint32_t x, uint32_t y);
	RayHitResult TraceRay(const Ray& ray);
	RayHitResult ClosestHit(const Ray& ray, float distance, uint32_t ObjIdx);
	RayHitResult Miss(const Ray& ray);
	
private:
	float AspectRatio = 1.0f;
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;

	const Scene* ActiveScene = nullptr;
	const Camera* ActiveCamera = nullptr;
};

