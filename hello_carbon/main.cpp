#include "Carbon/debug/renderer.h"
#include <Carbon/Carbon.h>
#include <cmath>

int main() {
    Carbon C;
    printf("Init done\n");

    Body a, b;

    a.position.y = -2.0f;
    b.position.y = 5.0f;

    C.add(&a, false, true);
    C.add(&b);

    DebugRenderer db;

    while (db.update()) {
        C.update();

        if (db.keyPress("space"))
            C.world.b[1].position.y = 5;

        // printf("%f\n", C.world.b[0].position.y);
        // printf("%f\n", C.world.b[0].position.y);

        db.aabb(C.world.b[0].aabb + C.world.b[0].position, vec3(1, 0, 0));
        db.aabb(C.world.b[1].aabb + C.world.b[1].position, vec3(1, 0, 1));

        db.line(C.world.b[1].position,
                C.world.b[1].position + C.world.b[1].velocity, vec3(0, 1, 0));
    }

    return 0;
}