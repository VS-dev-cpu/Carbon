#pragma once

#include <algorithm>
#include <array>
#include <bits/stdc++.h>

// Physics Types
namespace physics {

typedef std::array<int, 3> ivec3;

// Axis-Aligned Bounding-Box
struct AABB {
    vec3 *position;

    vec1 x[2];
    vec1 y[2];
    vec1 z[2];
};

// Sphere
struct sphere {
    vec3 *position;

    vec3 offset;

    float radius = 1.0f;
};

// Triangle
typedef vec3 triangle[3];

// Physics Body
struct body {
    vec3 *position;
    vec3 *rotation;

    // Collider(s)
    mesh *c;
    AABB aabb;

    std::set<ivec3> partitions;

    // Properties
    bool isStatic =
        false; //< static bodies have a collider, but forces don't effect them
    bool collider = true; //< has collider?
    bool gravity = true;  //< effected by gravity

    float mass = 1.0f;

    // Physics Variables
    vec3 velocity;
    vec3 angular_velocity;
};

// Physics Partition
struct partition {
  private:
    std::map<ivec3, std::set<int>> objs; //< Spatial Partitions

  public:
    vec3 size = vec3(10.0f, 10.0f, 10.0f); //< The Size of the Partitions

    // Get Partition Coordinate's for position
    void get(vec3 position, int &x, int &y, int &z) {
        x = position.x / size.x;
        y = position.y / size.y;
        z = position.z / size.z;
    }

    // Add ID to partition
    void add(int x, int y, int z, int id) { objs[{x, y, z}].insert(id); }

    // Add ID to a volume of partitions
    std::vector<ivec3> add(int lx, int ly, int lz, int hx, int hy, int hz,
                           int id) {
        std::vector<ivec3> ret;

        // bug
        for (int x = lx; x < hx; x++)
            for (int y = ly; y < hy; y++)
                for (int z = lz; z < hz; z++) {
                    add(x, y, z, id);
                    ret.push_back({x, y, z});
                }

        return ret;
    }

    // AutoAdd a Body to partition(s) by AABB
    std::vector<ivec3> add(AABB collider, int id) {
        int lx, ly, lz, hx, hy, hz;

        get(vec3(collider.x[0], collider.y[0], collider.z[0]), lx, ly, lz);
        get(vec3(collider.x[1], collider.y[1], collider.z[1]), hx, hy, hz);

        return add(lx, ly, lz, hx, hy, hz, id);
    }

    // Delete ID from partition
    void del(int x, int y, int z, int id) { objs[{x, y, z}].erase(id); }

    // Get Partition's Objects
    std::set<int> get(int x, int y, int z) { return objs[{x, y, z}]; }
};

// Physics World
struct world {
    // Every Body in the Physics World
    std::vector<body> b;
    std::map<int, std::set<int>> collisions;

    partition part;

    // The Physics World's Gravitational Force
    float gravity = gravity::EARTH;

    bool active = true; //< is the world running?
};

}; // namespace physics