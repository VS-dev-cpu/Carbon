#include <Carbon/types/types.h>

// AABB

AABB AABB::operator+(vec3 position) {
    return {this->offset,
            this->x[0] + position.x,
            this->x[1] + position.x,
            this->y[0] + position.y,
            this->y[1] + position.y,
            this->z[0] + position.z,
            this->z[1] + position.z};
}

// Sphere

Sphere Sphere::operator+(vec3 position) {
    return {this->offset + position, this->radius};
}

// Body

void Body::addVelocity(float amount, vec3 v) { this->velocity += v * amount; }

void Body::addAngularVelocity(float amount, vec3 av, vec3 center) {
    // TODO
    // no idea how to do it
}

// Partition

void Partition::get(vec3 position, ivec3 &v) {
    v[0] = position.x / size.x;
    v[1] = position.y / size.y;
    v[2] = position.z / size.z;
}

void Partition::add(ivec3 v, int id) { objs[{v[0], v[1], v[2]}].insert(id); }

std::vector<ivec3> Partition::add(ivec3 low, ivec3 high, int id) {
    std::vector<ivec3> ret;

    // bug ??? idk
    // TODO: check if actually bug
    for (int x = low[0]; x < high[0]; x++)
        for (int y = low[1]; y < high[1]; y++)
            for (int z = low[2]; z < high[2]; z++) {
                add((ivec3){x, y, z}, id);
                ret.push_back({x, y, z});
            }

    return ret;
}

std::vector<ivec3> Partition::add(AABB collider, int id) {
    ivec3 low, high;

    get(vec3(collider.x[0], collider.y[0], collider.z[0]), low);
    get(vec3(collider.x[1], collider.y[1], collider.z[1]), high);

    return add(low, high, id);
}

void Partition::del(ivec3 v, int id) { objs[{v[0], v[1], v[2]}].erase(id); }

std::set<int> Partition::get(ivec3 v) { return objs[{v[0], v[1], v[2]}]; }