//
// Created by QiuXu on 2024/4/12.
//

#include "Renderer.h"


namespace Utils {
    uint32_t ConvertToRGBA(glm::vec4 color) {
        color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

        uint8_t r = uint8_t(color.r * 255.0f);
        uint8_t g = uint8_t(color.g * 255.0f);
        uint8_t b = uint8_t(color.b * 255.0f);
        uint8_t z = uint8_t(color.a * 255.0f);

        return r | g << 8 | b << 16 | z << 24;
    }
}

void Renderer::OnResize(uint32_t width, uint32_t height) {

    if (m_image) {
        if (m_image->GetWidth() == width && m_image->GetHeight() == height) {
            return;
        }
        m_image->Resize(width, height);
    } else {
        m_image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] m_imageData;
    m_imageData = new uint32_t[width * height * 4];
}

void Renderer::Render() {

    uint32_t width = m_image->GetWidth();
    uint32_t height = m_image->GetHeight();

    glm::vec2 resolution = glm::vec2(float(width), float(height));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            glm::vec2 coord = {float(x), float(y)};
            // coord = coord * 2.0f - 1.0f;
            coord = (coord * 2.0f - resolution) / float(height);
            glm::vec4 color = PerPixel(coord);
            m_imageData[x + y * width] = Utils::ConvertToRGBA(color);
        }
    }

    m_image->SetData(m_imageData);
}

float dir = 1;

void Renderer::Update(float dt) {

    m_lightPos.x += dt * dir;
    if (m_lightPos.x >= 2) {
        dir = -1;
    } else if (m_lightPos.x <= -2) {
        dir = 1;
    }

}

glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 1.0f);

glm::vec4 Renderer::PerPixel(glm::vec2 coord) {

    glm::vec3 rayDir = glm::vec3(coord.x, coord.y, -1.0f);
    // rayDir = glm::normalize(rayDir);

    // (bx^2 + by^2 + bz^2)t^2 + 2(axbx + ayby + azbz)t + (ax^2 + ay^2 + az^2 - r^2) = 0
    // a = bx^2 + by^2 + bz^2;
    // b = 2(axbx + ayby + azbz)
    // c = ax^2 + ay^2 + az^2 - r^2

    glm::vec3 ro = rayOrigin - m_sphereCenter;
    float a = glm::dot(rayDir, rayDir);;
    float b = 2.0f * glm::dot(ro, rayDir);
    float c = glm::dot(ro, ro) - m_radius * m_radius;

    // b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant >= 0) {

        float sqrtDiscriminant = glm::sqrt(discriminant);
        float a2 = 2.0f * a;

        float t0 = (sqrtDiscriminant - b) / a2;
        float t1 = (-b - sqrtDiscriminant) / a2;

        glm::vec3 hitPos = rayOrigin + rayDir * t1;

        glm::vec3 normal = glm::normalize(hitPos - m_sphereCenter);
        // normal = normal * 0.5f + 0.5f;

        glm::vec3 lightDir = glm::normalize(hitPos - m_lightPos);
        float d = glm::dot(normal, -lightDir);

        glm::vec3 color = glm::vec3(1.0f) * d;

        return glm::vec4(color, 1.0f);
    } else {
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}
