#include "Carbon/debug/renderer.h"
#include <Carbon/Carbon.h>
#include <cmath>

int main() {
    Carbon C;
    printf("Init done\n");

    Body b;

    printf("Added Body %i\n", C.add(&b));

    DebugRenderer db;

    while (db.update()) {
        C.update();

        // printf("%f\n", C.world.b[0].position.y);
        printf("%f\n", C.world.b[0].position.y);

        db.draw(C.world.b[0], vec3(1, .4, 0));
    }

    return 0;
}