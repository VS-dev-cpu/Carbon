#include "Carbon/debug/renderer.hpp"
#include <Carbon/Carbon.hpp>

#include <time.h>

Carbon::Carbon(float partition_size) {
    // Setup World
    world.part.size = vec3(partition_size, partition_size, partition_size);
    now = time();
}

// Destroy Physics Engine
Carbon::~Carbon() {}

// Update Physics Engine
bool Carbon::update() {
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
                    if (COLLISION::BODY_BODY(*A, *B)) {
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

#ifdef DEBUG_RENDERER
    if (!db.update())
        return false;
#endif

    return true;
}

// Add Mesh
int Carbon::add(Mesh mesh, bool gravity, bool isStatic) {
    // Add Body
    int id = world.b.size();
    world.b.push_back(Body());
    Body *b = &world.b[id];

    // Set Up Collider
    b->mesh = mesh;

    // Create Trigger (AABB)
    for (int i = 0; i < (int)mesh.tri.size(); i++) {
        for (int j = 0; j < 3; j++) {
            vec3 p = mesh.tri[i].p[j];

            b->trigger.x[0] = fmin(p.x, b->trigger.x[0]);
            b->trigger.x[1] = fmax(p.x, b->trigger.x[1]);

            b->trigger.y[0] = fmin(p.y, b->trigger.y[0]);
            b->trigger.y[1] = fmax(p.y, b->trigger.y[1]);

            b->trigger.z[0] = fmin(p.z, b->trigger.z[0]);
            b->trigger.z[1] = fmax(p.z, b->trigger.z[1]);
        }
    }

    // Calculate Weight
    vec3 size;
    size.x = abs(b->trigger.x[0] - b->trigger.x[1]);
    size.y = abs(b->trigger.y[0] - b->trigger.y[1]);
    size.z = abs(b->trigger.z[0] - b->trigger.z[1]);
    b->weight = size.x * size.y * size.z * b->mass;

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
    b->hasGravity = gravity;
    b->isStatic = isStatic;

    // TODO: fix partitioning
    // std::vector<ivec3> v = world.part.add(pb.aabb, world.b.size() - 1);
    // printf("%i\n", v.size());
    // for (int i = 0; i < (int)v.size(); i++)
    //     world.b[world.b.size() - 1].partitions.insert(v[i]);

#ifdef DEBUG_RENDERER
    // Add to DebugRenderer
    db.add(b);
#endif

    return id;
}

float Carbon::time() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return (1000.0f * res.tv_sec + (double)res.tv_nsec / 1e6) / 1000.0f;
}