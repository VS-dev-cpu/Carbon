#include <Carbon/Carbon.hpp>

#include <cstring>
#include <string>

Mesh load_obj(std::string path, float scale = 1.0f);

int main() {
    Carbon C;
    printf("Init done\n");

    C.add(load_obj("cube.obj"), false, true);
    C.add(load_obj("cube.obj"));

    float now = C.time(), then = now;
    while (true) {
        then = now;
        now = C.time();
        float deltaTime = now - then;

        C.update(deltaTime);
        printf("%f\n", C.world.b[0].position.y);
    }

    return 0;
}

Mesh load_obj(std::string path, float scale) {
    Mesh out;

    FILE *f = fopen(path.c_str(), "r");

    if (f) {
        std::vector<vec3> vertices;

        char line[512];
        while (fgets(line, sizeof(line), f)) {
            char index[8];
            char data[256];

            sscanf(line, "%7s %[^\n]", index, data);

            if (!strcmp(index, "v")) {
                float x, y, z, w = 1.0f;
                sscanf(data, "%f %f %f %f", &x, &y, &z, &w);
                vertices.push_back(vec3(x, y, z));
            } else if (!strcmp(index, "f")) {
                float v[4], t[4], n[4];
                char part[4][128];
                int parts = sscanf(data, "%s %s %s %s", part[0], part[1],
                                   part[2], part[3]);

                for (int i = 0; i < parts; i++) {
                    if (sscanf(part[i], "%f/%f/%f", &v[i], &t[i], &n[i]) != 3)
                        if (sscanf(part[i], "%f//%f", &v[i], &n[i]) != 2)
                            if (sscanf(part[i], "%f/%f", &v[i], &t[i]) != 2)
                                if (sscanf(part[i], "%f", &v[i]) != 1) {
                                    log_error("load_obj", "failed to read line",
                                              path.c_str());
                                    parts = 0;
                                }
                }

                Triangle tri;

                for (int i = 0; i < 3; i++)
                    tri.p[i] = vertices[v[i]];

                out.tri.push_back(tri);

            } else {
                // TODO: Something
            }
        }

        fclose(f);
    } else
        log_error("load_obj", "failed to open file: %s", path.c_str());

    // Return Loaded
    log_system("load_obj", "loaded file: %s", path.c_str());
    return out;
}