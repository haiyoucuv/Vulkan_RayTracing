//
// Created by QiuXu on 2024/4/12.
//

#include "Renderer.h"


void Renderer::OnResize(uint32_t width, uint32_t height) {

    if (m_Image) {
        if (m_Image->GetWidth() == width || m_Image->GetHeight() == height) {
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
            m_ImageData[x + y * width] = PerPixel(coord);
        }
    }

    m_Image->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord) {

    uint8_t r = coord.x * 255.0f;
    uint8_t g = coord.y * 255.0f;

    return 0xff000000 | (g << 8) | r;
}
