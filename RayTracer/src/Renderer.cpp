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

glm::vec4 Renderer::TraceRay(Scene* scene, const Ray& ray)
{
	float a{ 0.0f }, b{ 0.0f }, c{ 0.0f }, discriminant{ 0.0f }, t1{ 0.0f };
	glm::vec3 LightDir{ -1.0f }, RayOrigin{ 0.0f }, SphereNormal{ 0.0f };
	float ShortestDistance{ std::numeric_limits<float>::max() };
	Sphere* sphere{ nullptr }, *ClosestSphere{ nullptr };

	if (scene->Spheres.size() == 0)
	{
		return glm::vec4(0, 0, 0, 1);
	}

	for (int i = 0; i < scene->Spheres.size(); i++)
	{
		sphere = &scene->Spheres.at(i);

		RayOrigin = ray.Origin - sphere->Position;
		// |d|t^2+2(a-c).d.t+|a-c|-r^2=0
		a = glm::dot(ray.Direction, ray.Direction);
		b = 2.0f * glm::dot(RayOrigin, ray.Direction);
		c = glm::dot(RayOrigin, RayOrigin) - sphere->Radius * sphere->Radius;

		discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0)
			continue;

		t1 = (-b - glm::sqrt(discriminant)) / (2.0 * a);

		if (t1 < ShortestDistance)
		{
			ShortestDistance = t1;
			ClosestSphere = sphere;
		}
	}
	
	if (ClosestSphere == nullptr)
		return glm::vec4(0, 0, 0, 1);
	else
	{
		RayOrigin = ray.Origin - ClosestSphere->Position;
		SphereNormal = glm::normalize(RayOrigin + ray.Direction * ShortestDistance);
		float Specular = glm::max(glm::dot(SphereNormal, -LightDir), 0.0f);
		return glm::vec4(ClosestSphere->Albedo * Specular, 1);
	}

}

void Renderer::Render(Scene* scene, const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	
	for (uint32_t y = 0; y < m_Image->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Image->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_Image->GetWidth()];
			
			glm::vec4 color = TraceRay(scene, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Image->GetWidth()] = utils::ConvertRGBA(color);
		}
	}
	m_Image->SetData(m_ImageData);
}
