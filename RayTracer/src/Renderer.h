#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Walnut/Random.h"
#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer() = default;
	void Render();
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_Image; };
	uint32_t PerPixel(glm::vec2 coord);

private:
	float AspectRatio = 1.0f;
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;

};

