#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/gtc/type_ptr.hpp>
using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f,0.1f,100.0f)
	{
		{
			Sphere sphere;
			sphere.Position = { 0.0f,0.0f,0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 0;
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Position = { 0.0f,-101.0f,0.0f };
			sphere.Radius = 100.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}
		
		{
			Material material;
			material.Albedo = { 1.0f,0.0f,1.0f };
			material.Roughness = 1.0f;
			material.Metallic = 0.0f;
			m_Scene.Materials.push_back(material);
		}
		{
			Material material;
			material.Albedo = { 0.0f,1.0f,1.0f };
			material.Roughness = 0.1f;
			material.Metallic = 0.0f;
			m_Scene.Materials.push_back(material);
		}
	}

	virtual void OnUpdate(float ts) override {
		if (m_Camera.OnUpdate(ts)) {
			m_Renderer.ResetFrameIndex();
		}
	}
	
	virtual void OnUIRender() override
	{
		// Menu window
		ImGui::Begin("Menu");
		ImGui::Checkbox("Render", &m_Render);
		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);
		if (ImGui::Button("Reset")) {
			m_Renderer.ResetFrameIndex();
		}
		ImGui::Text("Rendering Time : %.3fms (%.1f FPS)", m_LastRenderTime, 1000.0f / m_LastRenderTime);
		ImGui::Text("Right Click and drag to rotate camera");
		ImGui::Text("W, A, S, D, E, Q to move around");
		ImGui::End();

		ImGui::Begin("Scene");
		
		ImGui::DragFloat3("light Direction", glm::value_ptr(m_Renderer.lightDir),0.1f);
		ImGui::Separator();
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
			ImGui::PushID(i);
			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::Text("Sphere %d", i);
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f,0.0f,10.0f);
			ImGui::DragInt("Material ID", &sphere.MaterialIndex, 1.0f, 0, m_Scene.Materials.size() - 1);
			ImGui::Separator();

			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Materials");
		for (size_t i = 0; i < m_Scene.Materials.size(); i++) {
			Material& material = m_Scene.Materials[i];
			ImGui::PushID(i);
			ImGui::ColorEdit3("Color", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
			ImGui::Separator();

			ImGui::PopID();
		}
		ImGui::End();

		// Viewport window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		auto image = m_Renderer.GetFinalImage();
		if(image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(),(float)image->GetHeight() },
				ImVec2(0,1),ImVec2(1,0));

		ImGui::End();
		ImGui::PopStyleVar();

		if(m_Render)
			Render();
	}

	void Render() {
		Timer timer;

		m_Renderer.onResize(m_ViewportWidth,m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene,m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
	bool m_Render = true;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "JJT's Raytracing Practice";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("Yeah"))
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