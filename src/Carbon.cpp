#include "Carbon/collisions.h"
#include <Carbon/Carbon.h>

Carbon::Carbon(float partition_size) {
    // Setup World
    world.part.size = vec3(partition_size, partition_size, partition_size);
    now = time();
}

// Destroy Physics Engine
Carbon::~Carbon() {}

// Update Physics Engine
void Carbon::update() {
    // Timing
    past = now;
    now = time();
    deltaTime = (float)(now - past);

    // Update Physics World

    // Reset Collisions
    world.collisions.clear();

    // For Every Object (In Motion)
    for (int i = 0; i < (int)world.b.size(); i++) {
        Body *A = &world.b[i];

        // Apply Forces
        if (A->hasGravity)
            A->velocity.y -= world.gravity * deltaTime;

        // TODO: Update Partitions

        for (int j = 0; j < (int)world.b.size(); j++) {
            if (i != j) {
                Body *B = &world.b[j];

                // Check Collisions
                if (!world.collisions[i].count(j)) {
                    world.collisions[i].erase(j);

                    // Check Collisions
                    if (COLLISION::AABB_AABB(A->aabb + A->position,
                                             B->aabb + B->position)) {

                        A->velocity = vec3();
                        A->angular_velocity = vec3();
                    }
                }
            }
        }

        // Move Object A
        if (!A->isStatic) {
            A->position += A->velocity * deltaTime;
            A->rotation += A->angular_velocity * deltaTime;

            // A->velocity = 0;
        }

        // For Every Partition it's in
        // for (auto j : A->partitions) //< TODO: fix itarators
        // {
        //     std::set<int> o = world.part.get(j);

        //     // For Every Object in Partition
        //     for (auto k : o) {
        //         Body *B = &world.b[k];

        //         // Check Collisions
        //         if (k != i && !world.collisions[i].count(k)) {
        //             // Check Collisions
        //             if (COLLISION::AABB_AABB(A->aabb + A->position,
        //                                      B->aabb + B->position)) {
        //                 vec3 norm;
        //                 if (COLLISION::BODY_BODY(*A, *B, norm)) {
        //                     // Collision!
        //                     world.collisions[i].insert(k);
        //                 }
        //             }
        //         }
        //     }
        // }
    }
}

// Link Body to Physics Engine
int Carbon::add(Body *b, bool gravity, bool isStatic) {
    Body pb;

    pb.position = b->position;
    pb.rotation = b->rotation;

    // pb.c = b->c;

    std::vector<vec3> vertices = {vec3(-0.5f, -0.5f, -0.5f),
                                  vec3(0.5f, 0.5f, 0.5f)};

    // Get AABB
    for (int i = 0; i < (int)vertices.size(); i++) {
        vec3 p = vertices[i];

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
    pb.hasGravity = gravity;
    pb.isStatic = isStatic;

    // Add Object
    int id = world.b.size();
    world.b.push_back(pb);
    // TODO: fix partitioning
    // std::vector<ivec3> v = world.part.add(pb.aabb, world.b.size() - 1);
    // printf("%i\n", v.size());
    // for (int i = 0; i < (int)v.size(); i++)
    //     world.b[world.b.size() - 1].partitions.insert(v[i]);

    return id;
}

float Carbon::time() {
    static const auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> duration = currentTime - startTime;
    return duration.count();
}