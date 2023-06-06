#include <Carbon/debug/renderer.h>
#include <GL/gl.h>

// KeyCodes
std::unordered_map<std::string, int> GLFW_STRING_SCANCODE = {
    // Letters
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

DebugRenderer::DebugRenderer() {
    glfwInit();

    window = glfwCreateWindow(width = 720, height = 480,
                              "Carbon Debug Renderer", nullptr, nullptr);

    if (window == nullptr) {
        printf("DebugRenderer: Failed to create Window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    // Set Callbacks
    glfwSetFramebufferSizeCallback(window, callback_resize);
    glfwSetKeyCallback(window, callback_key);
    glfwSetMouseButtonCallback(window, callback_mouse);
    glfwSetCursorPosCallback(window, callback_cursor);
    glfwSetScrollCallback(window, callback_scroll);

    glfwSwapInterval(0);

    // Set Up GL
    cameraPosition.z = -10.0f;
    cameraRotation.y = 90.0f;
}

DebugRenderer::~DebugRenderer() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool DebugRenderer::update() {
    // Poll Events
    glfwPollEvents();

    // Update Window Buffer
    glfwSwapBuffers(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetFramebufferSize(window, &width, &height);

    // Update Camera
    // glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setPerspectiveProjection(45.0f, (float)width / (float)height, 0.1f,
                             10000.0f);

    glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glRotatef(cameraRotation.x, 1, 0, 0);
    glRotatef(cameraRotation.y, 0, 1, 0);

    // glPushMatrix();

    return !glfwWindowShouldClose(window);
}

bool DebugRenderer::key(std::string k) {
    int scancode = GLFW_STRING_SCANCODE[k];

    if (scancode < 0)
        return keyboard.count(scancode);

    return keyboard.count(glfwGetKeyScancode(scancode)) > 0;
}

void DebugRenderer::draw(Body b, vec3 color, bool wireframe) {
    // Draw AABB
    glColor3f(color.r, color.g, color.b);

    AABB aabb = b.aabb + b.position + b.aabb.offset;

    // for (int i = 0; i < 2; i++) {
    //     aabb.x[i] += aabb.position->x;
    //     aabb.y[i] += aabb.position->y;
    //     aabb.z[i] += aabb.position->z;
    // }

    glBegin(GL_LINES);
    // Bottom face
    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[0]);
    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[0]);

    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[0]);
    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[1]);

    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[1]);
    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[1]);

    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[1]);
    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[0]);

    // Top face
    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[0]);
    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[0]);

    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[0]);
    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[1]);

    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[1]);
    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[1]);

    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[1]);
    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[0]);

    // Vertical edges
    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[0]);
    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[0]);

    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[0]);
    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[0]);

    glVertex3f(aabb.x[1], aabb.y[0], aabb.z[1]);
    glVertex3f(aabb.x[1], aabb.y[1], aabb.z[1]);

    glVertex3f(aabb.x[0], aabb.y[0], aabb.z[1]);
    glVertex3f(aabb.x[0], aabb.y[1], aabb.z[1]);
    glEnd();
}