#pragma once

#include <Carbon/types/types.hpp>
#include <Carbon/utility/collisions.hpp>

#include <mathutil/common.hpp>

class Carbon {
  public:
    Carbon(float partition_size = 10.0f);
    ~Carbon();

    void update(const float &deltaTime);

    // Add Mesh
    int add(Mesh m, bool gravity = true, bool isStatic = false);

  public:
    World world;

  private:
};