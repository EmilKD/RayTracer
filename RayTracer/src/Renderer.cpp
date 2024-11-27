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

glm::vec4 Renderer::OnPixel(uint32_t x, uint32_t y)
{
	glm::vec3 LightDir = glm::normalize(glm::vec3(-1.0f)), SphereColor{ 0.0f }, color{0.0f};

	Ray ray;
	ray.Origin = ActiveCamera->GetPosition();
	ray.Direction = ActiveCamera->GetRayDirections()[x + y * m_Image->GetWidth()];

	int bounces = 2;
	float Specular{ 0.0f }, c{ 1.0f };

	for (size_t i = 0; i < bounces; i++)
	{
		Renderer::RayHitResult hit = TraceRay(ray);

		if (hit.HitDistance < 0.0f)
		{
			break;
		}

		const Sphere& sphere = ActiveScene->Spheres.at(hit.ObjectIndex);
		SphereColor = sphere.Albedo;
		Specular = glm::max(glm::dot(hit.WorldNormal, -LightDir), 0.0f);
		SphereColor *= Specular;
		color += SphereColor * c;
		
		ray.Origin = hit.WorldPosition + hit.WorldNormal * 0.0001f;
		ray.Direction = glm::reflect(ray.Direction, hit.WorldNormal);

		c *= 0.7;
	}
	
	return glm::vec4(color, 1.0f);
}

Renderer::RayHitResult Renderer::TraceRay(const Ray& ray)
{
	float a{ 0.0f }, b{ 0.0f }, c{ 0.0f }, discriminant{ 0.0f }, t1{ 0.0f };
	glm::vec3 RayOrigin{ 0.0f };
	float ShortestDistance{ std::numeric_limits<float>::max() };
	int ClosestId{ -1 };

	if (ActiveScene->Spheres.size() == 0)
	{
		return Miss(ray);
	}

	for (int i = 0; i < ActiveScene->Spheres.size(); i++)
	{
		const Sphere* sphere = &ActiveScene->Spheres[i];

		RayOrigin = ray.Origin - sphere->Position;
		// |d|t^2+2(a-c).d.t+|a-c|-r^2=0
		a = glm::dot(ray.Direction, ray.Direction);
		b = 2.0f * glm::dot(RayOrigin, ray.Direction);
		c = glm::dot(RayOrigin, RayOrigin) - sphere->Radius * sphere->Radius;

		discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0)
			continue;

		t1 = (-b - glm::sqrt(discriminant)) / (2.0 * a);

		if (t1 < ShortestDistance && t1 > 0)
		{
			ShortestDistance = t1;
			ClosestId = i;
		}
	}

	if (ClosestId < 0)
		return Miss(ray);
	else
		return ClosestHit(ray, ShortestDistance, ClosestId);
		
}

Renderer::RayHitResult Renderer::ClosestHit(const Ray& ray, float distance, uint32_t ObjIdx)
{
	glm::vec3 LightDir{ -1.0f }, RayOrigin{ 0.0f }, SphereNormal{ 0.0f };
	const Sphere ClosestSphere = ActiveScene->Spheres[ObjIdx];

	RayHitResult hit;

	hit.HitDistance = distance;
	RayOrigin = ray.Origin - ClosestSphere.Position;
	hit.WorldPosition = RayOrigin + ray.Direction * distance;
	hit.WorldNormal = glm::normalize(hit.WorldPosition);
	hit.WorldPosition += ClosestSphere.Position;
	hit.ObjectIndex = ObjIdx;

	return hit;
}

Renderer::RayHitResult Renderer::Miss(const Ray& ray)
{
	RayHitResult hit;
	hit.HitDistance = -1;
	return hit;
}

void Renderer::Render(Scene* scene, const Camera& camera)
{
	glm::vec4 color{ 0.0f };

	ActiveScene = scene;
	ActiveCamera = &camera;

	for (uint32_t y = 0; y < m_Image->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Image->GetWidth(); x++)
		{
			color = OnPixel(x, y);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_Image->GetWidth()] = utils::ConvertRGBA(color);
		}
	}
	m_Image->SetData(m_ImageData);
}

