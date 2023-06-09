#include <Carbon/utility/collisions.hpp>

// Include Triangle Intersection Test "Library"
extern "C" {
#include <Carbon/utility/tri_intersect.h>
}

namespace COLLISION {

// Interval - Interval
bool INTERVAL(vec1 a1, vec1 a2, vec1 b1, vec1 b2) {
    if (a1 > a2)
        std::swap(a1, a2);

    if (b1 > b2)
        std::swap(b1, b2);

    // |-a1----a2-|  |---b---|

    // ("a1" or "a2" is inside "b")
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
    if (INTERVAL(a.x[0], a.x[1], b.x[0], b.x[1]))
        // X axis collides
        if (INTERVAL(a.y[0], a.y[1], b.y[0], b.y[1]))
            // Y axis collides too
            if (INTERVAL(a.z[0], a.z[1], b.z[0], b.z[1]))
                // Z axis collides as well, collision!
                return true;

    // no collisions
    return false;
}

// Sphere - Sphere
// bool SPHERE_SPHERE(Sphere a, Sphere b) {
//     // Check Collision
//     return a.offset.distance(b.offset) <= (a.radius + b.radius);
// }

// Triangle - Triangle
bool TRIANGLE_TRIANGLE(Triangle a, Triangle b) {
    for (int i = 0; i < 3; i++) {
        a.p[i] += a.offset;
        b.p[i] += b.offset;
    }

    return NoDivTriTriIsect(a.p[0].data(), a.p[1].data(), a.p[2].data(),
                            b.p[0].data(), b.p[1].data(), b.p[2].data());
}

// Mesh - Mesh (Triangles)
bool MESH_MESH(Mesh a, Mesh b) {
    int la = a.tri.size();
    int lb = b.tri.size();

    // Then Calculate Triangle-by-Triangle
    for (int i = 0; i < la; i++)
        for (int j = 0; j < lb; j++)
            if (TRIANGLE_TRIANGLE(a.tri[i] + a.offset, b.tri[j] + a.offset))
                return true;

    return false;
}

// Body - Body (Meshes)
bool BODY_BODY(Body a, Body b) {
    if (!AABB_AABB(a.trigger + a.position, b.trigger + b.position))
        return false;

    return MESH_MESH(a.mesh + a.position, b.mesh + b.position);

    // This is the worst case...
    return false;
}

} // namespace COLLISION
