#include <Carbon/Carbon.hpp>

int main() {
    Carbon C;
    printf("Init done\n");

    Body a, b;

    a.position.y = -2.0f;
    b.position.y = 5.0f;

    Mesh m;
    m.tri.push_back((Triangle){vec3(0.0f), vec3(1), vec3(-1), vec3(.0)});

    C.add(m, false, true);
    C.add(m);

    log_warning("test", "hello logging");

    DebugRenderer db;

    while (db.update()) {
        C.update();

        if (db.keyPress("space"))
            C.world.b[1].position.y = 5;

        // printf("%f\n", C.world.b[0].position.y);
        // printf("%f\n", C.world.b[0].position.y);

        db.aabb(C.world.b[0].trigger + C.world.b[0].position, vec3(1, 0, 0));
        db.aabb(C.world.b[1].trigger + C.world.b[1].position, vec3(1, 0, 1));

        // Draw Velocity Line
        db.line(C.world.b[1].position,
                C.world.b[1].position + C.world.b[1].velocity / 5.0f,
                vec3(0, 1, 0));
    }

    return 0;
}