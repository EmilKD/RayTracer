#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{

		ImGui::Begin("Settings");
		ImGui::Text("Render Time: %.3fms", LastRenderTime); // 1/fps
		if (ImGui::Button("Render"))
		{
			render();
		}
		ImGui::End();

		ImGui::Begin("Viewport");

		ViewportWidth = ImGui::GetContentRegionAvail().x;
		ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		render();
	}

	void render()
	{
		Timer timer;
		m_renderer.OnResize(ViewportWidth, ViewportHeight);
		m_renderer.Render();
		LastRenderTime = timer.ElapsedMillis();
	}


private:
	Renderer m_renderer;
	uint32_t ViewportWidth = 0, ViewportHeight = 0;
	float LastRenderTime = 0.0f;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracer";

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