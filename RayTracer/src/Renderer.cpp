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

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	float SphereRadius = 0.5f;
	glm::vec3 SphereOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 LightDir{ -1.0f, -1.0f, -1.0f };

	// |d|t^2+2(a-c).d.t+|a-c|-r^2=0
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * (glm::dot(ray.Origin, ray.Direction) - glm::dot(SphereOrigin, ray.Direction));
	float c = glm::dot(ray.Origin - SphereOrigin, ray.Origin - SphereOrigin) - SphereRadius * SphereRadius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0.0f)
	{
		float t0 = (-b + glm::sqrt(discriminant)) / (2.0 * a);
		float t1 = (-b - glm::sqrt(discriminant)) / (2.0 * a);

		glm::vec3 SphereNormal = glm::normalize(ray.Origin + ray.Direction * t1);

		float Specular = glm::max(glm::dot(SphereNormal, -LightDir), 0.0f);

		glm::vec4 color = glm::vec4(Specular, 0, 0, 1);

		return color;
	}
	else
		return glm::vec4(0, 0, 0, 1);

}

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	
	for (uint32_t y = 0; y < m_Image->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Image->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_Image->GetWidth()];
			
			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Image->GetWidth()] = utils::ConvertRGBA(color);
		}
	}
	m_Image->SetData(m_ImageData);
}
