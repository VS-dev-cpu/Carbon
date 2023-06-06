#pragma once

#include <Carbon/types/vector.h>

#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <unordered_map>

// (low effort) vec3, but integer
typedef std::array<int, 3> ivec3;

// Axis-Aligned Bounding-Box
struct AABB {
    vec3 *position;

    vec1 x[2];
    vec1 y[2];
    vec1 z[2];
};

// Sphere
struct Sphere {
    vec3 *position;

    vec3 offset;

    float radius = 1.0f;
};

// Mesh
struct Mesh {};

// Triangle
typedef vec3 Triangle[3];

// Physics Body
struct Body {
    // Transform
    vec3 position;
    vec3 rotation;

    // Collider(s)
    Mesh m;
    AABB aabb;

    // Partitions containing it
    // TODO: something about it
    std::set<ivec3> partitions;

    // Properties
    bool isStatic = false;   // has collider, forces doesn't have effect
    bool hasCollider = true; // has collider
    bool hasGravity = true;  // effected by gravity

    // Physical Density (kg/m3)
    float mass = 1.0f;

    // Physical Weight (kg)
    float weight = 1.0f;

    // Forces
    vec3 velocity;
    vec3 angular_velocity;
};

// Physics Partition
struct Partition {
  private:
    std::map<ivec3, std::set<int>> objs; // Spatial Partitions

  public:
    vec3 size = vec3(10.0f, 10.0f, 10.0f); // The Size of the Partitions

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
struct World {
    // Every Body in the Physics World
    std::vector<Body> b;
    std::unordered_map<int, std::set<int>> collisions;

    // World Partitioning
    Partition part;

    // The Physics World's Gravitational Force
    float gravity = 9.807f; // Earth's Gravity
};