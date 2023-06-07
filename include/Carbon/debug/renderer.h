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
    void draw(AABB b, vec3 color = vec3(-1, -1, -1), bool wireframe = false);

    // True when Key is Released
    bool keyPress(std::string k);

  public:
    int width, height;
    vec2 mouse, scroll;

  private:
    GLFWwindow *window = nullptr;

    vec2 cameraRotation;
    vec3 cameraPosition;

    std::set<int> keyboard;
    std::set<int> keyrelease;

  private:
    void setPerspectiveProjection(float fovY, float aspectRatio,
                                  float nearPlane, float farPlane) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-aspectRatio * nearPlane * tan(fovY / 2.0f),
                  aspectRatio * nearPlane * tan(fovY / 2.0f),
                  -nearPlane * tan(fovY / 2.0f), nearPlane * tan(fovY / 2.0f),
                  nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

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
        else {
            self->keyboard.erase(scancode);
            self->keyrelease.insert(scancode);
        }
    }

    // Mouse Button Callback
    static void callback_mouse(GLFWwindow *window, int button, int action,
                               int mods) {
        DebugRenderer *self = getRenderer(window);

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                self->keyboard.insert(-1);
            } else {
                self->keyboard.erase(-1);
                self->keyrelease.insert(-1);
            }
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS)
                self->keyboard.insert(-2);
            else {
                self->keyboard.erase(-2);
                self->keyrelease.insert(-2);
            }
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            if (action == GLFW_PRESS)
                self->keyboard.insert(-3);
            else {
                self->keyboard.erase(-3);
                self->keyrelease.insert(-3);
            }
        }
    }

    // Cursor Movement Callback
    static void callback_cursor(GLFWwindow *window, double x, double y) {
        DebugRenderer *self = getRenderer(window);

        x = x / self->width * 2 - 1;
        y = y / self->height * 2 - 1;

        float xx = x, yy = y;

        x -= self->mouse.x;
        y -= self->mouse.y;

        self->mouse.x = xx;
        self->mouse.y = yy;

        if (self->key("right"))
            self->cameraPosition += vec3(x, -y, 0);
        if (self->key("left")) {
            self->cameraRotation += vec2(y, x) * 40;
        }
    }

    // Scroll Movement Callback
    static void callback_scroll(GLFWwindow *window, double x, double y) {
        DebugRenderer *self = getRenderer(window);

        self->scroll.x = x;
        self->scroll.y = y;

        self->cameraPosition.z += y / 10.0f;
    }
};