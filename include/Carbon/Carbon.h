#pragma once

#include <chrono>
#include <iostream>
#include <pthread.h>

#include <Carbon/types/types.h>

#include <Carbon/collisions.h>

#include <Carbon/debug/renderer.h>

class Carbon {
  public:
    Carbon(float partition_size = 10.0f);
    ~Carbon();

    void update();

    int add(Body *b, bool gravity = true, bool isStatic = false);

    void blow(vec3 pos, float strength);

  public:
    World world;
    float deltaTime;

  private:
    // Timing
    float past, now;

    // Max DeltaTime Value (in seconds)
    const long maxDelta = 2;

  public:
    float time();
};