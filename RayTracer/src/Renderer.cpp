#include "Renderer.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_Image)
	{
		m_Image->Resize(width, height);
	}
	else
	{
		m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
	AspectRatio = (float)width / (float)height;
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 RayOrigin = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 RayDirection = glm::vec3(coord.x, coord.y, -1.0f);
	float SphereRadius = 0.5f;
	glm::vec3 SphereOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

	// |d|t^2+2(a-c).d.t+|a-c|-r^2=0
	float a = glm::dot(RayDirection, RayDirection);
	float b = 2 * (glm::dot(RayOrigin, RayDirection) - glm::dot(SphereOrigin, RayDirection));
	float c = glm::dot(RayOrigin - SphereOrigin, RayOrigin - SphereOrigin) - SphereRadius * SphereRadius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0)
	{
		float t = glm::min((-b + glm::sqrt(discriminant)) / (-2 * a), (-b - glm::sqrt(discriminant)) / (-2 * a));
		uint8_t redGrad = (uint8_t)(256.0f / t);
		return 0xff000000 | redGrad;
	}
	else
		return 0xff000000;

}

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_Image->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Image->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_Image->GetWidth(), (float)y / (float)m_Image->GetHeight() };
			coord = 2.0f * coord - 1.0f;
			coord.y = coord.y / AspectRatio;
			m_ImageData[x + y * m_Image->GetWidth()] = PerPixel(coord);
		}
	}
	m_Image->SetData(m_ImageData);
}
