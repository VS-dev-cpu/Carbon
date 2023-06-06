#pragma once

#include <Carbon/types/vector.h>

#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <unordered_map>

// (low effort) 3D Intiger Vector (for Partition Indexing)
typedef std::array<int, 3> ivec3;

// Axis-Aligned Bounding-Box
struct AABB {
    vec3 offset;

    vec1 x[2];
    vec1 y[2];
    vec1 z[2];

    AABB operator+(vec3 position);
};

struct Sphere {
    vec3 offset;

    float radius = 1.0f;

    Sphere operator+(vec3 position);
};

// Triangle
typedef vec3 Triangle[3];

// Mesh
struct Mesh {
    vec3 offset;

    // std::vector<Triangle> tri;

    // Mesh operator+(vec3 position) { return {this->offset + position}; }
};

// Physics Body
struct Body {
  public:
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

    // Add Velocity [Force]
    void addVelocity(float amount, vec3 v);

    // Add Angular Velocity [Force]
    void addAngularVelocity(float amount, vec3 av, vec3 center = vec3());

  private:
};

// Physics Partition
struct Partition {
  public:
    // The Size of the Partitions
    vec3 size = vec3(10.0f, 10.0f, 10.0f);

    // Get Partition Coordinate's for position
    void get(vec3 position, ivec3 &v);

    // Add ID to partition
    void add(ivec3 pos, int id);

    // Add ID to a volume of partitions
    std::vector<ivec3> add(ivec3 low, ivec3 high, int id);

    // AutoAdd a Body to partition(s) by AABB
    std::vector<ivec3> add(AABB collider, int id);

    // Delete ID from partition
    void del(ivec3 pos, int id);

    // Get Partition's Objects
    std::set<int> get(ivec3 pos);

  private:
    // Spatial Partitions
    std::map<ivec3, std::set<int>> objs;
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