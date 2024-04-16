//
// Created by QiuXu on 2024/4/12.
//

#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

class Renderer {

public:
    Renderer() = default;

    glm::vec3 m_lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

    void OnResize(uint32_t width, uint32_t height);

    void Update(float dt);

    void Render(const Scene &scene, const Camera &camera);

    std::shared_ptr<Walnut::Image> GetImage() const { return m_image; };

private:
    std::shared_ptr<Walnut::Image> m_image;
    uint32_t *m_imageData = nullptr;

    glm::vec4 TraceRay(const Ray &ray, const Scene &scene);
};


#endif//RAYTRACING_RENDERER_H
