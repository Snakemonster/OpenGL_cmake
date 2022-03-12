#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader_handler.h"
#include "camera.h"
#include "Spheres.h"
#include "Light.h"

#include <iostream>
#include "EngineOGL.h"

const unsigned int SRC_WIDTH = 1280;
const unsigned int SRC_HEIGHT = 800;

bool EngineOGL::firstMouse = true;
float EngineOGL::lastX = SRC_WIDTH / 2.f;
float EngineOGL::lastY = SRC_HEIGHT / 2.f;
Camera EngineOGL::camera = Camera(glm::vec3(0.f, 0.f, 3.f));

int EngineOGL::start() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to crate GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    mainLoop();
    return 0;
}

void EngineOGL::mainLoop() {

    Spheres sphere(glm::vec3(-1.f, 0.f, 0.f), 1.f, 144, 72);
    sphere.setAllPhongLight(glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::vec3(1.0f, 1.0f, 1.0f),
                            16.f);

    Spheres sphere2(glm::vec3(5.f, 0.f, 0.f), 1.f, 144, 72);
    sphere2.setAllPhongLight(glm::vec3(1.0f, 1.0f, 1.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f),
                             16.f);


    Light light(glm::vec3(1.2f, 1.0f, 2.0f),
                glm::vec3(0.2f, 0.f, 0.f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(1.0f, 1.0f, 1.0f));

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//main loop code
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SRC_WIDTH / (float) SRC_HEIGHT, 0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        light.draw(projection, view, glfwGetTime());
        sphere2.draw(projection, view, light);
        sphere.draw(projection, view, light);
//end of main loop code

        showFPS(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
}


//--------------------------------------------------------------------------------------
//callbacks
void EngineOGL::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void EngineOGL::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void EngineOGL::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
//end of callbacks
//--------------------------------------------------------------------------------------

void EngineOGL::processInput(GLFWwindow *window) const {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void EngineOGL::showFPS(GLFWwindow *window) {
    double currentTimeFPS = glfwGetTime();
    deltaTimeFPS = currentTimeFPS - lastTimeFPS;
    nFrames++;
    if (deltaTimeFPS >= 1) {
        double fps = double(nFrames) / deltaTimeFPS;
        std::stringstream ss;
        ss << "OpenGL FPS is: " << fps << " fps";
        glfwSetWindowTitle(window, ss.str().c_str());
        nFrames = 0;
        lastTimeFPS = currentTimeFPS;
    }
}
