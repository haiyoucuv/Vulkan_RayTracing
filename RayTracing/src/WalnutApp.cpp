#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

class RayTracing : public Walnut::Layer {

public:
    RayTracing()
        : m_camera(45.0f, 0.1f, 100.0f) {

        // sphere
        {
            Sphere sphere;
            sphere.position = glm::vec3(0.0f, 0.0f, 10.0f);
            sphere.radius = 0.5f;
            sphere.albedo = glm::vec3(1.0f, 0.0f, 0.0f);
            m_scene.spheres.push_back(sphere);
        }

        // sphere 2
        // {
        //     Sphere sphere;
        //     sphere.position = glm::vec3(0.2f, 0.0f, -2.0f);
        //     sphere.radius = 0.5f;
        //     sphere.albedo = glm::vec3(0.0f, 1.0f, 0.0f);
        //     m_scene.spheres.push_back(sphere);
        // }

    }

    void OnUIRender() override {

        ImGui::Begin("Settings");
        ImGui::Text("Fps: %.0ffps", 1000 / m_dt);
        ImGui::Text("Logic Time: %.2fms", m_logicTime);
        ImGui::Text("Render Time: %.2fms", m_renderTime);
        ImGui::Text("Total Time: %.2fms", m_logicTime + m_renderTime);

        if (ImGui::Button("Render")) {
            Render();
        }

        ImGui::End();

        ImGui::Begin("Scene");
        for (size_t i = 0; i < m_scene.spheres.size(); i++) {
            ImGui::PushID(i);

            Sphere &sphere = m_scene.spheres[i];
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
            ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.albedo));

            ImGui::Separator();

            ImGui::PopID();
        }
        ImGui::End();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::Begin("Viewport");

        m_vWidth = uint32_t(ImGui::GetContentRegionAvail().x);
        m_vHeight = uint32_t(ImGui::GetContentRegionAvail().y);

        std::shared_ptr<Walnut::Image> image = m_renderer.GetImage();

        if (m_renderer.GetImage()) {
            ImGui::Image(
                image->GetDescriptorSet(),
                ImVec2(float(image->GetWidth()), float(image->GetHeight())),
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }

        ImGui::End();
        ImGui::PopStyleVar();


        float now = m_timer.ElapsedMillis();
        m_dt = now - m_lastT;
        m_lastT = now;

        Update(m_dt / 1000);
        m_logicTime = m_timer.ElapsedMillis() - now;

        Render();
        m_renderTime = m_timer.ElapsedMillis() - now - m_logicTime;

    }

    void Update(float dt) {
        m_camera.OnUpdate(dt);
        m_renderer.Update(dt);
    }

    void Render() {
        m_camera.OnResize(m_vWidth, m_vHeight);
        m_renderer.OnResize(m_vWidth, m_vHeight);
        m_renderer.Render(m_scene, m_camera);
    }

private:
    Renderer m_renderer;

    Camera m_camera;
    Scene m_scene;

    uint32_t m_vWidth = 0;
    uint32_t m_vHeight = 0;

    Timer m_timer;
    float m_lastT = 0.0f;
    float m_dt = 0.0f;
    float m_logicTime = 0.0f;
    float m_renderTime = 0.0f;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
    Walnut::ApplicationSpecification spec;
    spec.Name = "RayTracing";

    auto *app = new Application(spec);
    app->PushLayer<RayTracing>();
    app->SetMenubarCallback([app]() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}