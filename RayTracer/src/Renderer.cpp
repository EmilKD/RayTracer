#include "Renderer.h"

namespace utils {
	static uint32_t ConvertRGBA(const glm::vec4 &InpColor) 
	{
		uint8_t r = (uint8_t)(InpColor.r * 255.0f);
		uint8_t g = (uint8_t)(InpColor.g * 255.0f);
		uint8_t b = (uint8_t)(InpColor.b * 255.0f);
		uint8_t a = (uint8_t)(InpColor.a * 255.0f);

		return a << 24 | b << 16 | g << 8 | r;
	}
}

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

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	//uint8_t r = (uint8_t)(coord.x * 255.0f);
	//uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 RayOrigin = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 RayDirection = glm::vec3(coord.x, coord.y, -1.0f);
	float SphereRadius = 0.5f;
	glm::vec3 SphereOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 LightDir{ -1.0f, -1.0f, -1.0f };

	// |d|t^2+2(a-c).d.t+|a-c|-r^2=0
	float a = glm::dot(RayDirection, RayDirection);
	float b = 2.0f * (glm::dot(RayOrigin, RayDirection) - glm::dot(SphereOrigin, RayDirection));
	float c = glm::dot(RayOrigin - SphereOrigin, RayOrigin - SphereOrigin) - SphereRadius * SphereRadius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0.0f)
	{
		float t0 = (-b + glm::sqrt(discriminant)) / (2.0 * a);
		float t1 = (-b - glm::sqrt(discriminant)) / (2.0 * a);

		glm::vec3 SphereNormal = glm::normalize(RayOrigin + RayDirection * t1) * 0.5f + 0.5f;

		float Specular = glm::max(glm::dot(SphereNormal, -LightDir), 0.0f);

		glm::vec4 color = glm::vec4(1*Specular, 0, 0, 1);

		return color;
	}
	else
		return glm::vec4(0, 0, 0, 1);

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

			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Image->GetWidth()] = utils::ConvertRGBA(color);
		}
	}
	m_Image->SetData(m_ImageData);
}
