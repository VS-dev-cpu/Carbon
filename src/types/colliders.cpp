#include <Carbon/types/colliders.hpp>

// AABB

AABB AABB::operator+(vec3 position) {
    return {this->offset + position,
            this->x[0],
            this->x[1],
            this->y[0],
            this->y[1],
            this->z[0],
            this->z[1]};
}

// Sphere

Sphere Sphere::operator+(vec3 position) {
    return {this->offset + position, this->radius};
}