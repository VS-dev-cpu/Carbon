#include <Carbon/debug/renderer.hpp>

#include <Carbon/debug/log.hpp>

// Forward-Declarations
extern const char *_raw_shader[2];
extern std::unordered_map<std::string, int> GLFW_STRING_SCANCODE;

vec3 normal(vec3 v[3]);

DebugRenderer::DebugRenderer() {
    // ---- Window Creation
    glfwInit();

    window = glfwCreateWindow(width = 720, height = 480,
                              "Carbon Debug Renderer", nullptr, nullptr);

    if (window == nullptr) {
        log_error("DebugRenderer", "Window Creation Failed");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log_error("DebugRenderer", "OpenGL Initialization Failed");
        return;
    }

    glfwSetWindowUserPointer(window, this);

    // Set Callbacks
    glfwSetFramebufferSizeCallback(window, callback_resize);
    glfwSetKeyCallback(window, callback_key);
    glfwSetMouseButtonCallback(window, callback_mouse);
    glfwSetCursorPosCallback(window, callback_cursor);
    glfwSetScrollCallback(window, callback_scroll);

    glfwSwapInterval(0);

    // ---- Shader Compilation

    int success;
    char infoLog[1024];

    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &_raw_shader[0], NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 1024, NULL, infoLog);
        log_error("DebugRenderer", "(V) Shader Compilation Failed: \n%s",
                  infoLog);
        return;
    }

    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &_raw_shader[1], NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 1024, NULL, infoLog);
        log_error("DebugRenderer", "(F) Shader Compilation Failed: \n%s",
                  infoLog);
        return;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vert);
    glAttachShader(shader, frag);

    glLinkProgram(shader);

    glDeleteShader(vert);
    glDeleteShader(frag);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        log_error("DebugRenderer", "Shader Linking Failed: \n%s", infoLog);
        return;
    }

    uniform[0] = glGetUniformLocation(shader, "model");
    uniform[1] = glGetUniformLocation(shader, "view");
    uniform[2] = glGetUniformLocation(shader, "projection");
    uniform[3] = glGetUniformLocation(shader, "color");

    // Set Up GL
    cameraPosition.z = -10.0f;
    cameraRotation.y = 90.0f;
}

DebugRenderer::~DebugRenderer() {
    glDeleteProgram(shader);

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool DebugRenderer::update() {
    // Draw Bodies
    for (int i = 0; i < (int)body.size(); i++) {
        draw(VAO[i], body[i]->mesh.tri.size() * 3,
             body[i]->position + body[i]->mesh.offset, body[i]->rotation);
    }

    // Update Window Buffer
    glfwSwapBuffers(window);

    glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Poll Events
    keypress.clear();
    glfwPollEvents();

    glfwGetFramebufferSize(window, &width, &height);

    return !glfwWindowShouldClose(window);
}

bool DebugRenderer::key(std::string k) {
    int scancode = GLFW_STRING_SCANCODE[k];

    if (scancode < 0)
        return keyboard.count(scancode);

    return keyboard.count(glfwGetKeyScancode(scancode)) > 0;
}

bool DebugRenderer::keyPress(std::string k) {
    int scancode = GLFW_STRING_SCANCODE[k];

    if (scancode < 0)
        return keypress.count(scancode);

    return keypress.count(glfwGetKeyScancode(scancode)) > 0;
}

// Draw

void DebugRenderer::line(vec3 start, vec3 end, vec3 color) {
    // Draw Line
}

void DebugRenderer::aabb(AABB b, vec3 color) {
    for (int i = 0; i < 2; i++) {
        b.x[i] += b.offset.x;
        b.y[i] += b.offset.y;
        b.z[i] += b.offset.z;
    }

    // Draw AABB
}

void DebugRenderer::mesh(Mesh m, vec3 color) {
    // Draw Mesh
}

void DebugRenderer::add(Body *b) {
    body.push_back(b);
    VAO.push_back(0);

    std::vector<float> vertices;

    for (int i = 0; i < (int)b->mesh.tri.size(); i++) {
        vec3 n = normal(b->mesh.tri[i].p);
        for (int j = 0; j < 3; j++) {
            vertices.push_back(b->mesh.tri[i].p[0].x);
            vertices.push_back(b->mesh.tri[i].p[0].y);
            vertices.push_back(b->mesh.tri[i].p[0].z);

            vertices.push_back(n.x);
            vertices.push_back(n.y);
            vertices.push_back(n.z);
        }
    }

    unsigned int *vao = &VAO[VAO.size() - 1];
    unsigned int vbo;

    // Load Into VRAM
    glGenVertexArrays(1, vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * b->mesh.tri.size(),
                 vertices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void *)(3 * sizeof(float)));
}

void DebugRenderer::draw(unsigned int VAO, unsigned int size, vec3 position,
                         vec3 rotation) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(shader);
    glEnable(GL_DEPTH_TEST);

    // Set Model Uniform
    glUniformMatrix4fv(
        uniform[0], 1, GL_FALSE, // Position, rotation
        (matrix::translation(vec3()) * matrix::rotation(rads(vec3()))).m[0]);

    // Set View Uniform
    glUniformMatrix4fv(uniform[1], 1, GL_FALSE, // rotation is lookdir
                       matrix::lookat(cameraPosition,
                                      cameraPosition + vec3(cameraRotation.x,
                                                            cameraRotation.y),
                                      vec3(0, 1, 0))
                           .m[0]);

    // Set Projection Uniform
    glUniformMatrix4fv(uniform[2], 1, GL_FALSE,
                       matrix::perspective(rads(60),
                                           (float)width / (float)height, 0.1f,
                                           1000.0f)
                           .m[0]);

    glUniform3f(uniform[3], 1.0f, 0.4f, 0.0f);

    // Draw Mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, size);
}

// Local Variables

vec3 normal(vec3 v[3]) {
    vec3 ret;
    double ux = v[1].x - v[0].x;
    double uy = v[1].y - v[0].y;
    double uz = v[1].z - v[0].z;
    double vx = v[2].x - v[0].x;
    double vy = v[2].y - v[0].y;
    double vz = v[2].z - v[0].z;
    ret.x = uy * vz - uz * vy;
    ret.y = uz * vx - ux * vz;
    ret.z = ux * vy - uy * vx;
    return ret;
}

// OpenGL Shaders
const char *_raw_shader[2] = {
    // Vertex Shader
    "#version 300 es\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aNormal;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"

    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"

    "void main() {\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}",

    // Fragment Shader
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 FragColor;\n"

    "uniform vec3 color;\n"

    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"

    "void main(){\n"
    "   vec3 lightDir = normalize(-vec3(-0.2, -1.0, -0.3));\n"
    "   float diff = max(dot(Normal, lightDir), 0.0);\n"
    "   FragColor = vec4(color *diff, 1.0);\n"
    "}"};

// KeyCodes
std::unordered_map<std::string, int> GLFW_STRING_SCANCODE = { // Letters
    {"a", GLFW_KEY_A},
    {"b", GLFW_KEY_B},
    {"c", GLFW_KEY_C},
    {"d", GLFW_KEY_D},
    {"e", GLFW_KEY_E},
    {"f", GLFW_KEY_F},
    {"g", GLFW_KEY_G},
    {"h", GLFW_KEY_H},
    {"i", GLFW_KEY_I},
    {"j", GLFW_KEY_J},
    {"k", GLFW_KEY_K},
    {"l", GLFW_KEY_L},
    {"m", GLFW_KEY_M},
    {"n", GLFW_KEY_N},
    {"o", GLFW_KEY_O},
    {"p", GLFW_KEY_P},
    {"q", GLFW_KEY_Q},
    {"r", GLFW_KEY_R},
    {"s", GLFW_KEY_S},
    {"t", GLFW_KEY_T},
    {"u", GLFW_KEY_U},
    {"v", GLFW_KEY_V},
    {"w", GLFW_KEY_W},
    {"x", GLFW_KEY_X},
    {"y", GLFW_KEY_Y},
    {"z", GLFW_KEY_Z},

    // Numbers
    {"0", GLFW_KEY_0},
    {"1", GLFW_KEY_1},
    {"2", GLFW_KEY_2},
    {"3", GLFW_KEY_3},
    {"4", GLFW_KEY_4},
    {"5", GLFW_KEY_5},
    {"6", GLFW_KEY_6},
    {"7", GLFW_KEY_7},
    {"8", GLFW_KEY_8},
    {"9", GLFW_KEY_9},

    // Enter
    {"return", GLFW_KEY_ENTER},
    {"enter", GLFW_KEY_ENTER},
    {"\n", GLFW_KEY_ENTER},

    // Escape
    {"escape", GLFW_KEY_ESCAPE},
    {"esc", GLFW_KEY_ESCAPE},

    // Backspace
    {"backspace", GLFW_KEY_BACKSPACE},
    {"\b", GLFW_KEY_BACKSPACE},

    // Tab
    {"tab", GLFW_KEY_TAB},
    {"\t", GLFW_KEY_TAB},

    // Space
    {"space", GLFW_KEY_SPACE},
    {" ", GLFW_KEY_SPACE},

    // Control Keys
    {"ctrl", GLFW_KEY_LEFT_CONTROL},
    {"control", GLFW_KEY_LEFT_CONTROL},
    {"shift", GLFW_KEY_LEFT_SHIFT},
    {"alt", GLFW_KEY_LEFT_ALT},
    {"super", GLFW_KEY_LEFT_SUPER},

    {"lctrl", GLFW_KEY_LEFT_CONTROL},
    {"lshift", GLFW_KEY_LEFT_SHIFT},
    {"lalt", GLFW_KEY_LEFT_ALT},
    {"lgui", GLFW_KEY_LEFT_SUPER},
    {"lsuper", GLFW_KEY_LEFT_SUPER},

    {"rctrl", GLFW_KEY_RIGHT_CONTROL},
    {"rshift", GLFW_KEY_RIGHT_SHIFT},
    {"ralt", GLFW_KEY_RIGHT_ALT},
    {"rgui", GLFW_KEY_RIGHT_SUPER},
    {"rsuper", GLFW_KEY_RIGHT_SUPER},

    // Function Keys
    {"f1", GLFW_KEY_F1},
    {"f2", GLFW_KEY_F2},
    {"f3", GLFW_KEY_F3},
    {"f4", GLFW_KEY_F4},
    {"f5", GLFW_KEY_F5},
    {"f6", GLFW_KEY_F6},
    {"f7", GLFW_KEY_F7},
    {"f8", GLFW_KEY_F8},
    {"f9", GLFW_KEY_F9},
    {"f10", GLFW_KEY_F10},
    {"f11", GLFW_KEY_F11},
    {"f12", GLFW_KEY_F12},

    // Mouse Buttons
    {"left", -1},
    {"right", -2},
    {"middle", -3}};