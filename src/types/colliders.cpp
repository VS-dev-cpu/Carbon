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

// Triangle

Triangle Triangle::operator+(vec3 position) {
    return {this->offset + position, this->p[0], this->p[1], this->p[2]};
}

// Mesh

Mesh Mesh::operator+(vec3 position) {
    return {this->offset + position, this->tri};
}