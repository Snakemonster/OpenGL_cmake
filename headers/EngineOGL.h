#ifndef OPENGL_CMAKE_ENGINEOGL_H
#define OPENGL_CMAKE_ENGINEOGL_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "camera.h"

class EngineOGL {
public:
    int start();

private:
    void mainLoop();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double yoffset);
    void processInput(GLFWwindow* window) const;
    void showFPS(GLFWwindow* window);

    static float lastX;
    static float lastY;
    static bool firstMouse;
    double deltaTime = 0, deltaTimeFPS = 0;
    double lastTime = 0, lastTimeFPS = 0;
    int nFrames = 0;
    GLFWwindow* window;
    static Camera camera;
};


#endif //OPENGL_CMAKE_ENGINEOGL_H
