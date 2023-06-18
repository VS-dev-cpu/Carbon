#pragma once

#include <mathutil/matrix.hpp>

#include <any>
#include <array>
#include <vector>

// Axis-Aligned Bounding-Box
struct AABB {
    vec3 offset{};

    vec1 x[2]{};
    vec1 y[2]{};
    vec1 z[2]{};

    AABB operator+(vec3 position);
};

// Triangle
struct Triangle {
    vec3 offset;

    vec3 p[3];

    Triangle operator+(vec3 position);
};

// Mesh
struct Mesh {
    vec3 offset{};

    std::vector<Triangle> tri{};

    Mesh operator+(vec3 position);
};