#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"

using namespace Walnut;

class RayTracing : public Walnut::Layer {
public:
    void OnUIRender() override {

        ImGui::Begin("Settings");
        ImGui::Text("Last Render Time: %.2fms", m_LastRenderTime);

        if (ImGui::Button("Render")) {
            Render();
        }

        ImGui::End();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::Begin("Viewport");

        m_vWidth = uint32_t(ImGui::GetContentRegionAvail().x);
        m_vHeight = uint32_t(ImGui::GetContentRegionAvail().y);

        std::shared_ptr<Walnut::Image> image = m_Renderer.GetImage();

        if (m_Renderer.GetImage()) {
            ImGui::Image(
                image->GetDescriptorSet(),
                ImVec2(float(image->GetWidth()), float(image->GetHeight())),
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }

        ImGui::End();
        ImGui::PopStyleVar();

        Timer timer;
        Render();
        m_LastRenderTime = timer.ElapsedMillis();

    }

    void Render() {
        m_Renderer.OnResize(m_vWidth, m_vHeight);
        m_Renderer.Render();
    }

private:
    Renderer m_Renderer;

    uint32_t m_vWidth = 0;
    uint32_t m_vHeight = 0;

    float m_LastRenderTime = 0.0f;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
    Walnut::ApplicationSpecification spec;
    spec.Name = "RayTracing";

    Walnut::Application *app = new Application(spec);
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