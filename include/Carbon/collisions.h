#pragma once

#include <Carbon/types/types.h>

namespace COLLISION {

// 1D Interval
bool DETECT(vec1 a1, vec1 a2, vec1 b1, vec1 b2);

// AABB - AABB
bool AABB_AABB(AABB a, AABB b);

// Sphere - Sphere
bool SPHERE_SPHERE(Sphere a, Sphere b);

// Triangle
bool TRIANGLE_TRIANGLE(Triangle a, Triangle b);

// Body (Triangles)
bool BODY_BODY(Body a, Body b, vec3 &norm);

} // namespace COLLISION
