#include <Carbon/utility/collisions.hpp>

extern "C" {
#include <Carbon/utility/tri_intersect.h>
}

namespace COLLISION {

// 1D Interval
bool DETECT(vec1 a1, vec1 a2, vec1 b1, vec1 b2) {
    if (a1 > a2)
        std::swap(a1, a2);

    if (b1 > b2)
        std::swap(b1, b2);

    // |-a1----a2-|  |---b---|

    // ("a1" is inside "b") OR ("a2" is inside "b")
    if ((a1 >= b1 && b2 >= a1) || (a2 >= b1 && b2 >= a2))
        return true;

    return false;
}

// AABB - AABB
bool AABB_AABB(AABB a, AABB b) {
    // Calculate Real World Positions
    for (int i = 0; i < 2; i++) {
        a.x[i] += a.offset.x;
        a.y[i] += a.offset.y;
        a.z[i] += a.offset.z;

        b.x[i] += b.offset.x;
        b.y[i] += b.offset.y;
        b.z[i] += b.offset.z;
    }

    // Check Collision
    if (DETECT(a.x[0], a.x[1], b.x[0], b.x[1]))
        // the X axis is good!
        if (DETECT(a.y[0], a.y[1], b.y[0], b.y[1]))
            // the Y axis is good!
            if (DETECT(a.z[0], a.z[1], b.z[0], b.z[1]))
                // the Z axis is good, collision!
                return true;

    // no collisions detected
    return false;
}

// Sphere - Sphere
bool SPHERE_SPHERE(Sphere a, Sphere b) {
    // Check Collision
    return a.offset.distance(b.offset) <= (a.radius + b.radius);
}

// Triangle
bool TRIANGLE_TRIANGLE(Triangle a, Triangle b) {
    return NoDivTriTriIsect(a.p[0].data(), a.p[1].data(), a.p[2].data(),
                            b.p[0].data(), b.p[1].data(), b.p[2].data());
}

bool MESH_MESH(Mesh a, Mesh b) {
    int la = a.tri.size();
    int lb = b.tri.size();

    // Then Calculate Triangle-by-Triangle
    for (int i = 0; i < la; i++)
        for (int j = 0; j < lb; j++)
            if (TRIANGLE_TRIANGLE(a.tri[i], b.tri[j]))
                return true;

    return false;
}

// Body (Triangles)
bool BODY_BODY(Body a, Body b, vec3 &norm) {
    switch (a.collider.type) {};

    // This is the worst case...
    return false;
}

} // namespace COLLISION