#pragma once

#include <Carbon/types/types.h>

#include <Carbon/tri_intersect.h>

namespace physics {
// Collision Detection
namespace collision {
// 1D Interval
bool detect(vec1 a1, vec1 a2, vec1 b1, vec1 b2) {
    if (a1 > a2)
        std::swap(a1, a2);

    if (b1 > b2)
        std::swap(b1, b2);

    // |-a1----a2-|  |-b1----b2-|

    if ((a1 >= b1 && b2 >= a1) ||
        (a2 >= b1 && b2 >= a2)) // ("a1" is inside "b") OR ("a2" is inside "b")
        return true;

    return false;
}

// AABB - AABB
bool detect(AABB a, AABB b) {
    // Calculate Real World Positions
    for (int i = 0; i < 2; i++) {
        a.x[i] += a.position->x;
        a.y[i] += a.position->y;
        a.z[i] += a.position->z;

        b.x[i] += b.position->x;
        b.y[i] += b.position->y;
        b.z[i] += b.position->z;
    }

    // Check Collision
    if (detect(a.x[0], a.x[1], b.x[0], b.x[1]))
        // the X axis is good!
        if (detect(a.y[0], a.y[1], b.y[0], b.y[1]))
            // the Y axis is good!
            if (detect(a.z[0], a.z[1], b.z[0], b.z[1]))
                // the Z axis is good, collision!
                return true;
    // no collisions detected
    return false;
}

// Sphere - Sphere
bool detect(sphere a, sphere b) {
    // Calculate Real World Center
    vec3 center[2];
    center[0] = *a.position + a.offset;
    center[1] = *b.position + b.offset;

    // Check Collision
    return center[0].distance(center[1]) <= (a.radius + b.radius);
}

// Triangle
bool detect(triangle a, triangle b) {
    return NoDivTriTriIsect(a[0].p, a[1].p, a[2].p, b[0].p, b[1].p, b[2].p);
}

// Body (Triangles)
bool detect(body a, body b, vec3 &norm) {
    int la = a.c->vertices.size();
    int lb = b.c->vertices.size();
    // Then Calculate Triangle-by-Triangle
    for (int i = 0; i < la / 3; i++) {
        for (int j = 0; j < lb / 3; j++) {
            vec3 normal = a.c->vertices[i].normal;
            triangle ta;
            ta[0] = a.c->vertices[i].position + *a.position;
            ta[1] = a.c->vertices[i + 1].position + *a.position;
            ta[2] = a.c->vertices[i + 2].position + *a.position;

            triangle tb;
            tb[0] = b.c->vertices[j].position + *b.position;
            tb[1] = b.c->vertices[j + 1].position + *b.position;
            tb[2] = b.c->vertices[j + 2].position + *b.position;

            if (detect(ta, tb)) {
                norm = normal;
                return true;
            }
        }
    }

    // This is the worst case...
    return false;
}

}; // namespace collision
}; // namespace physics