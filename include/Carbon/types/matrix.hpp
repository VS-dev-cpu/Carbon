#pragma once

#include <Carbon/types/vector.hpp>

// Undef "near" and "far" on windows
#ifdef __windows
#undef near
#undef far
#endif

// 4 by 4 Matrix
struct mat4 {
    // Data
    float m[4][4] = {};

    // ---- Constructors

    // mat4(v) Constructor
    mat4(float v = 0);

    // ---- Operator Overloading (mat4)

    // [=] Operator (mat4)
    void operator=(const mat4 m);

    // [==] Operator (mat4)
    bool operator==(const mat4 m);

    // [*] Operator (mat4)
    mat4 operator*(const mat4 m);

    // ---- Operator Overloading (vec3)

    // [*] Operator (vec3)
    vec3 operator*(const vec3 v);
};

// Functions for Matrices
namespace matrix {

// Create an Identity Matrix
mat4 identity();

// Inverse a Matrix
mat4 inverse(mat4 m);

// Create a Perspective Matrix
mat4 perspective(float fovrads, float aspect, float near, float far);

// Create an Orthographic Projection Matrix
mat4 ortho(float left, float right, float bottom, float top, float near,
           float far);

// Create an X Rotation Matrix
mat4 rotationX(float rad);

// Create a Y Rotation Matrix
mat4 rotationY(float rad);

// Create a Z Rotation Matrix
mat4 rotationZ(float rad);

// Create a Rotation Matrix
mat4 rotation(vec3 rad);

// Create a Translation Matrix
mat4 translation(vec3 v);

mat4 scale(vec3 v);

// Create a Look-At Matrix
mat4 lookat(vec3 pos, vec3 target, vec3 up);

}; // namespace matrix