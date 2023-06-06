#pragma once

#include <Carbon/types/types.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

// Debug Renderer
class DebugRenderer {
  public:
    // Init Renderer & Window
    DebugRenderer();

    // Destroy Renderer & Window
    ~DebugRenderer();

    // Clear & Update Window
    bool update();

    // Get Key State
    bool key(std::string k);

    // Draw Physics Body
    void draw(Body b, vec3 color = vec3(-1, -1, -1), bool wireframe = false);

  public:
    int width, height;
    vec2 mouse, scroll;
    std::set<int> keyboard;

  private:
    GLFWwindow *window = nullptr;

  private:
    // Get DebugRenderer from Window
    static DebugRenderer *getRenderer(GLFWwindow *window) {
        return (DebugRenderer *)glfwGetWindowUserPointer(window);
    }

    // FrameBuffer Size Callback
    static void callback_resize(GLFWwindow *window, int w, int h) {
        DebugRenderer *self = getRenderer(window);
        self->width = w;
        self->height = h;

        glViewport(0, 0, w, h);
    }

    // Keyboard State Callback
    static void callback_key(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
        DebugRenderer *self = getRenderer(window);

        if (action == GLFW_PRESS)
            self->keyboard.insert(scancode);
        else
            self->keyboard.erase(scancode);
    }

    // Cursor Movement Callback
    static void callback_cursor(GLFWwindow *window, double x, double y) {
        DebugRenderer *self = getRenderer(window);

        self->mouse.x = (x * 2.0f) / (float)self->width - 1.0f;
        self->mouse.y = (y * -2.0f) / (float)self->height + 1.0f;

        // glTranslatef(self->mouse.u, self->mouse.x, 0);
    }

    // Scroll Movement Callback
    static void callback_scroll(GLFWwindow *window, double x, double y) {
        DebugRenderer *self = getRenderer(window);

        glTranslatef(0, 0, y / 10.0f);

        self->scroll.x = x;
        self->scroll.y = y;
    }
};