//
// Created by QiuXu on 2024/4/16.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <vector>

#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Sphere
{
    glm::vec3 position{0.0f};
    float radius = 0.5f;

    glm::vec3 albedo{1.0f};
};

struct Scene
{
    std::vector<Sphere> spheres;
};

#endif //RAYTRACING_SCENE_H
