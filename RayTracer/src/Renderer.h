#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Walnut/Random.h"
#include "glm/glm.hpp"

#include "Camera.h"
#include "Ray.h"

class Renderer
{
public:
	Renderer() = default;
	void Render(const Camera &camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_Image; };
	glm::vec4 TraceRay(const Ray& coord);

private:
	float AspectRatio = 1.0f;
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;

};

