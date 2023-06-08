#pragma once

#include <Carbon/types/vector.hpp>

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

struct Box {
    vec3 offset{};
    vec3 rotation{};

    vec3 size{};
};

struct Sphere {
    vec3 offset{};

    float radius{};

    Sphere operator+(vec3 position);
};

struct Capsule {
    vec3 offset;
    vec3 rotation{};

    vec3 a{}, b{};

    float radius{};
};

// Triangle
struct Triangle {
    vec3 offset;

    vec3 p[3];
};

// Mesh
struct Mesh {
    vec3 offset{};

    std::vector<Triangle> tri{};

    // Mesh operator+(vec3 position) { return {this->offset + position}; }
};

// Physics Collider
struct Collider {
    std::any collider{};
    unsigned int type{};

    vec3 *position{};
    vec3 *rotation{};
};