#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Camera.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f, 0.1f, 100.0f)
	{
		m_Scene.Materials.push_back(Material{ {0.5f, 0.5f, 0.5f} });
		m_Scene.Materials.push_back(Material{ {0.0f, 0.17f, 0.6f} });
		m_Scene.Materials.push_back(Material{ {1.0f, 0.7f, 0.0f} });

		m_Scene.Spheres.push_back(Sphere{ {0.0f, -100.0f, -4.0f}, 100.0f,  1});
		m_Scene.Spheres.push_back(Sphere{ {0.0f, 0.3f, 3.0f}, 0.5f,  2});
	}

	virtual void OnUpdate(float ts) override 
	{
		if (m_Camera.OnUpdate(ts)) 
		{
			m_renderer.ResetSampleCount();
		}
	}

	virtual void OnUIRender() override
	{

		ImGui::Begin("Settings");
		ImGui::Text("Render Time: %.3fms", LastRenderTime); // 1/fps
		if (ImGui::Button("Render"))
		{
			render();
		}
		ImGui::Checkbox("Accumulate", &m_renderer.GetSettings().Accumulate);
		ImGui::End();

		// Creating Object Settings GUI
		ImGui::Begin("Objects");
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::Text("Object Index: %i", i);
			ImGui::PushID(i);
			ImGui::DragFloat3("Position", glm::value_ptr(m_Scene.Spheres[i].Position), 0.1f);
			ImGui::DragFloat("Radius", &m_Scene.Spheres.at(i).Radius, 0.1f);
			ImGui::DragInt("Material Index", &m_Scene.Spheres.at(i).MatIdx, 1, (int)m_Scene.Materials.size() - 1);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		// Creating Material Settings GUI
		ImGui::Begin("Materials");
		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::Text("Material Index: %i", i);
			Material& mat = m_Scene.Materials.at(i);
			ImGui::PushID(i);
			ImGui::ColorEdit3("Albedo", glm::value_ptr(mat.Albedo));
			ImGui::DragFloat("Roughness", &mat.Roughness, 0.01, 0.0f, 1.0f);
			ImGui::DragFloat("Metallness", &mat.Metallness, 0.01, 0.0f, 1.0f);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Viewport");

		ViewportWidth = ImGui::GetContentRegionAvail().x;
		ViewportHeight = ImGui::GetContentRegionAvail().y;

		const auto image = m_renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		render();
	}

	void render()
	{
		Timer timer;
		m_renderer.OnResize(ViewportWidth, ViewportHeight);
		m_Camera.OnResize(ViewportWidth, ViewportHeight);
		m_renderer.Render(&m_Scene, m_Camera);
		LastRenderTime = timer.ElapsedMillis();
	}


private:
	Renderer m_renderer;
	uint32_t ViewportWidth = 0, ViewportHeight = 0;
	float LastRenderTime = 0.0f;
	Camera m_Camera;
	Scene m_Scene;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Emil's Ray Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}