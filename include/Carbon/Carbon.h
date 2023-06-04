#pragma once

#include <iostream>
#include <pthread.h>

#include <Carbon/types/types.h>

class Carbon {
  private:
    World world;

    // std::vector<physics::body> toAdd;

    // Timing
    long past = 0;
    long now = 0;

  public:
    double deltaTime = 0.0f;

    Carbon(float partition_size = 10.0f);
    ~Carbon();

    void update();

    int add(Body *b, bool gravity = true, bool isStatic = false);

    void blow(vec3 pos, float strength);
};

// Init Physics Engine
Carbon::Carbon(float partition_size) {
    // Setup World
    world.part.size = vec3(partition_size, partition_size, partition_size);

    now = time(true);
}

// Destroy Physics Engine
Carbon::~Carbon() {}

// Update Physics Engine
void Carbon::update() {
    // Timing
    past = now;
    now = time(true);
    deltaTime = (double)(now - past) / 1000000.0;

    // Update Physics World

    // Reset Collisions
    world.collisions.clear();

    // For Every Object (In Motion)
    for (int i = 0; i < (int)world.b.size(); i++) {
        Body *A = &world.b[i];

        // Apply Forces
        if (A->gravity)
            A->velocity.y -= world.gravity * deltaTime;

        // Move Object A
        if (!A->isStatic) {
            *A->position += A->velocity * deltaTime;
            *A->rotation += A->angular_velocity * deltaTime;
        }

        // TODO: Update Partitions

        // For Every Partition it's in
        for (std::set<ivec3>::iterator j = A->partitions.begin();
             j != A->partitions.end(); j++) //< TODO: fix itarators
        {
            std::set<int> o = world.part.get((*j)[0], (*j)[1], (*j)[2]);

            // For Every Object in Partition
            for (std::set<int>::iterator k = o.begin(); k != o.end(); k++) {
                Body *B = &world.b[*k];

                // Check Collisions
                if (*k != i && !world.collisions[i].count(*k)) {
                    // Check Collisions
                    if (collision::detect(A->aabb, B->aabb)) {
                        vec3 norm;
                        if (collision::detect(*A, *B, norm)) {
                            // Collision!
                            world.collisions[i].insert(*k);
                        }
                    }
                }
            }
        }
    }
}

// Link Body to Physics Engine
int Carbon::add(Body *b, bool gravity, bool isStatic) {
    Body pb;

    pb.position = &b->position;
    pb.rotation = &b->rotation;

    pb.c = &b->m;

    // Get AABB
    for (int i = 0; i < (int)b->m.vertices.size(); i++) {
        vec3 p = b->m.vertices[i].position;

        if (p.x < pb.aabb.x[0])
            pb.aabb.x[0] = p.x;
        if (p.x > pb.aabb.x[1])
            pb.aabb.x[1] = p.x;

        if (p.y < pb.aabb.y[0])
            pb.aabb.y[0] = p.y;
        if (p.y > pb.aabb.y[1])
            pb.aabb.y[1] = p.y;

        if (p.z < pb.aabb.z[0])
            pb.aabb.z[0] = p.z;
        if (p.x > pb.aabb.z[1])
            pb.aabb.z[1] = p.z;
    }
    pb.aabb.position = pb.position;

    // Get "MASS"
    vec3 size;
    size.x = abs(pb.aabb.x[0] - pb.aabb.x[1]);
    size.y = abs(pb.aabb.y[0] - pb.aabb.y[1]);
    size.z = abs(pb.aabb.z[0] - pb.aabb.z[1]);
    pb.mass = size.x * size.y * size.z;

    // Create Trigger
    /*pb.t.position = &b->position;
    pb.t.offset = vector::avg((int)b->m.vertices.size(),
    &b->m.vertices[0].position);

    for (int i = 0; i < (int)b->m.vertices.size(); i++)
    {
        float dst = vector::distance(b->m.vertices[i].position, obj.t.offset);
        if (dst > obj.t.radius)
            obj.t.radius = dst;
    }*/

    // Set Gravity
    pb.gravity = gravity;
    pb.isStatic = isStatic;

    // Add Object
    int id = world.b.size();
    world.b.push_back(pb);
    std::vector<ivec3> v = world.part.add(pb.aabb, world.b.size() - 1);
    for (int i = 0; i < (int)v.size(); i++)
        world.b[world.b.size() - 1].partitions.insert(v[i]);

    return id;
}