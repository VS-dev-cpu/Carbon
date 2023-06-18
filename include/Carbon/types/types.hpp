#pragma once

#include <Carbon/types/colliders.hpp>
#include <mathutil/matrix.hpp>

#include <map>
#include <set>
#include <unordered_map>

// Integer Vectors
typedef int ivec1;
typedef std::array<int, 2> ivec2;
typedef std::array<int, 3> ivec3;
typedef std::array<int, 4> ivec4;

// Physics Body
struct Body {
  public:
    // Transform
    vec3 position{};
    vec3 rotation{};

    // Collider(s)
    Mesh mesh{};
    AABB trigger{};

    // Partitions containing it
    // TODO: something about it
    // std::set<ivec3> partitions;

    // Properties
    bool isStatic = false;   // has collider, forces doesn't have effect
    bool hasCollider = true; // has collider
    bool hasGravity = true;  // effected by gravity

    // Physical Density (kg/m3)
    float mass = 1.0f;

    // Physical Weight (kg)
    float weight = 1.0f;

    // Forces
    vec3 velocity{};
    vec3 angular_velocity{};

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
    // Bodies
    std::vector<Body> b;

    // Collisions
    std::unordered_map<int, std::set<int>> collisions;

    // Spacial Partition
    Partition part;

    // Gravitational Force
    float gravity = 9.807f;
};