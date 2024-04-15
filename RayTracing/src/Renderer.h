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

    glm::vec3 m_sphereCenter = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_radius = 0.5f;

    glm::vec3 m_lightPos = glm::vec3(0.0f, 0.2f, 0.0f);

    glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    void OnResize(uint32_t width, uint32_t height);

    void Update(float dt);

    void Render();

    std::shared_ptr<Walnut::Image> GetImage() const { return m_image; };

private:
    glm::vec4 PerPixel(glm::vec2 coord);

private:
    std::shared_ptr<Walnut::Image> m_image;
    uint32_t *m_imageData = nullptr;
};


#endif//RAYTRACING_RENDERER_H
