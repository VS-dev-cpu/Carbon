#pragma once

#include <Carbon/types/types.hpp>

namespace COLLISION {

// AABB - AABB
bool AABB_AABB(AABB a, AABB b);

// Sphere - Sphere
// bool SPHERE_SPHERE(Sphere a, Sphere b);

// Triangle
bool TRIANGLE_TRIANGLE(Triangle a, Triangle b);

// Body (Triangles)
bool BODY_BODY(Body a, Body b);

} // namespace COLLISION
