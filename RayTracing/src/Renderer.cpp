//
// Created by QiuXu on 2024/4/12.
//

#include "Renderer.h"


void Renderer::OnResize(uint32_t width, uint32_t height) {

    if (m_Image) {
        if (m_Image->GetWidth() == width && m_Image->GetHeight() == height) {
            return;
        }
        m_Image->Resize(width, height);
    } else {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] m_ImageData;
    m_ImageData = new uint32_t[width * height * 4];
}

void Renderer::Render() {

    uint32_t width = m_Image->GetWidth();
    uint32_t height = m_Image->GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            glm::vec2 coord = {float(x) / float(width), float(y) / float(height)};
            coord = coord * 2.0f - 1.0f;
            m_ImageData[x + y * width] = PerPixel(coord);
        }
    }

    m_Image->SetData(m_ImageData);
}


glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 1.0f);
float radius = 0.5f;

uint32_t Renderer::PerPixel(glm::vec2 coord) {

    glm::vec3 rayDir = glm::vec3(coord.x, coord.y, -1.0f);
    rayDir = glm::normalize(rayDir);

    // (bx^2 + by^2 + bz^2)t^2 + 2(axbx + ayby + azbz)t + (ax^2 + ay^2 + az^2 - r^2) = 0
    // a = bx^2 + by^2 + bz^2;
    // b = 2(axbx + ayby + azbz)
    // c = ax^2 + ay^2 + az^2 - r^2

    float a = glm::dot(rayDir, rayDir);;
    float b = 2.0f * glm::dot(rayOrigin, rayDir);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    // b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant >= 0) {
        return 0xff0000ff;
    } else {
        return 0;
    }

    // uint8_t r = coord.x * 255.0f;
    // uint8_t g = coord.y * 255.0f;
    // return 0xff000000 | (g << 8) | r;
}
