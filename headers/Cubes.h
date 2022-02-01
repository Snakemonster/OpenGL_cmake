#ifndef OPENGL_CMAKE_CUBES_H
#define OPENGL_CMAKE_CUBES_H
#include <vector>

class Cubes {
public:
    Cubes(Shader lightingShader, Shader lightCubeShader);
    void draw(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos);
private:
    Shader lightingShader, lightCubeShader;
    unsigned int VAO_cube, VAO_light, VBO;
    std::vector<float> vertices;
};
#endif //OPENGL_CMAKE_CUBES_H
