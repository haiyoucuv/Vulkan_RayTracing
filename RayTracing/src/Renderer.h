//
// Created by QiuXu on 2024/4/12.
//

#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

class Renderer {

public:
    Renderer() = default;

    void OnResize(uint32_t width, uint32_t height);

    void Render();

    std::shared_ptr<Walnut::Image> GetImage() const { return m_Image; };

private:
    uint32_t PerPixel(glm::vec2 coord);

private:
    std::shared_ptr<Walnut::Image> m_Image;
    uint32_t *m_ImageData = nullptr;
};


#endif//RAYTRACING_RENDERER_H
