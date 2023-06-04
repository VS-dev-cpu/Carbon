#pragma once

#include <iostream>
#include <pthread.h>

#include <Carbon/collision.h>

/* The Main Physics Engine */
class Physics {
  private:
    physics::world WORLD; // The "Shared" Physics World
    bool writing = false;
    bool reading = false;

    // std::vector<physics::body> toAdd;

    // Threading
    pthread_t thread;
    bool threadState = false;

    static void *process(void *p);
    static void updatePhysicsWorld(Physics *p, physics::world *world);

    // Timing
    long past = 0;
    long now = 0;

  public:
    double deltaTime = 0.0f;

    Physics(float partition_size = 10.0f);
    ~Physics();

    void update();

    int add(body *b, bool gravity = true, bool isStatic = false);

    bool start();
    bool stop();

    void blow(vec3 pos, float strength);
};

// Init Physics Engine
Physics::Physics(float partition_size) {
    // Setup World
    WORLD.part.size = vec3(partition_size, partition_size, partition_size);

    // Launch Thread
    threadState = true;
    int code = pthread_create(&thread, NULL, &process, (void *)this);

    if (code != 0) {
        threadState = false;
        LOG::WARNING("physics()", "failed to start thread");
    }

    now = time(true);
}

// Destroy Physics Engine
Physics::~Physics() {
    // Stop Physics Thread(s)
    if (threadState == true) {
        threadState = false;
        pthread_join(thread, NULL);
    }
}

// Update Physics Engine
void Physics::update() {
    if (threadState) {
        // TODO: Fix Update
        /*
         // Update World Information
         reading = true;
         while (writing)
             ;

         if (WORLD.fresh)
             world = WORLD;
         WORLD.fresh = false;

         reading = false;
         */
    } else {
        // Calculate Changes on Main Thread
        updatePhysicsWorld(this, &WORLD);
    }
}

// Link Body to Physics Engine
int Physics::add(body *b, bool gravity, bool isStatic) {
    physics::body pb;

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

    // Add Body
    reading = true;
    while (writing)
        ;

    int id = WORLD.b.size();
    WORLD.b.push_back(pb);
    std::vector<physics::ivec3> v = WORLD.part.add(pb.aabb, WORLD.b.size() - 1);
    for (int i = 0; i < (int)v.size(); i++) {
        WORLD.b[WORLD.b.size() - 1].partitions.insert(v[i]);
    }

    reading = false;

    return id;
}

// Start Physics World
bool Physics::start() {
    if (WORLD.active)
        return false;
    WORLD.active = true;
    return true;
}

// Stop Physics World
bool Physics::stop() {
    if (!WORLD.active)
        return false;
    WORLD.active = false;
    return true;
}

// Update a Physics World
void Physics::updatePhysicsWorld(Physics *p, physics::world *world) {
    // Reset Collisions
    world->collisions.clear();

    // For Every Object (In Motion)
    for (int i = 0; i < (int)world->b.size(); i++) {
        physics::body *A = &world->b[i];

        // Apply Forces
        if (A->gravity)
            A->velocity.y -= world->gravity * p->deltaTime;

        // Move Object A
        if (!A->isStatic) {
            *A->position += A->velocity * p->deltaTime;
            *A->rotation += A->angular_velocity * p->deltaTime;
        }

        // TODO: Update Partitions

        // For Every Partition it's in
        for (std::set<physics::ivec3>::iterator j = A->partitions.begin();
             j != A->partitions.end(); j++) //< TODO: fix itarators
        {
            std::set<int> o = world->part.get((*j)[0], (*j)[1], (*j)[2]);

            // For Every Object in Partition
            for (std::set<int>::iterator k = o.begin(); k != o.end(); k++) {
                physics::body *B = &world->b[*k];

                // Check Collisions
                if (*k != i && !world->collisions[i].count(*k)) {
                    // Check Collisions
                    if (physics::collision::detect(A->aabb, B->aabb)) {
                        vec3 norm;
                        if (physics::collision::detect(*A, *B, norm)) {
                            // Collision!
                            world->collisions[i].insert(*k);
                        }
                    }
                }
            }
        }
    }
}

// Simulate a Physics World
void *Physics::process(void *arg) {
    Physics *p = (Physics *)arg;
    pthread_detach(pthread_self());

    // TODO: Local (thread) World

    p->now = time(true);
    while (p->threadState) {
        // Timing
        p->past = p->now;
        p->now = time(true);
        p->deltaTime = (double)(p->now - p->past) / 1000000;

        // Calculate Changes
        // Update "Shared" Variable
        while (p->reading)
            ;
        p->writing = true;

        if (p->WORLD.active)
            updatePhysicsWorld(p, &p->WORLD);

        p->writing = false;
    }

    // Exit Thread
    p->threadState = false;
    pthread_exit(NULL);
    return NULL;
}