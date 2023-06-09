#pragma once

#include <Carbon/types/types.hpp>
#include <Carbon/utility/collisions.hpp>

#include <Carbon/debug/log.hpp>

#ifdef DEBUG_RENDERER
#include <Carbon/debug/renderer.hpp>
#endif

class Carbon {
  public:
    Carbon(float partition_size = 10.0f);
    ~Carbon();

    bool update();

    // Add Mesh
    int add(Mesh m, bool gravity = true, bool isStatic = false);

    void blow(vec3 pos, float strength);

  public:
    World world;
    float deltaTime;

#ifdef DEBUG_RENDERER
    DebugRenderer db;
#endif

  private:
    // Timing
    float past, now;

    // Max DeltaTime Value (in seconds)
    const long maxDelta = 2;

  public:
    float time();
};