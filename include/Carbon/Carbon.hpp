#pragma once

#include <Carbon/debug/renderer.hpp>
#include <Carbon/types/types.hpp>
#include <Carbon/utility/collisions.hpp>

#include <Carbon/debug/log.hpp>

class Carbon {
  public:
    Carbon(float partition_size = 10.0f);
    ~Carbon();

    void update();

    // Add Mesh
    int add(Mesh m, bool gravity = true, bool isStatic = false);

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