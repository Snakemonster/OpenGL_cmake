#ifndef OPENGL_CMAKE_LIGHT_H
#define OPENGL_CMAKE_LIGHT_H
#include <vector>

class Light {
public:
    glm::vec3 position;
    Light(glm::vec3 position, glm::vec3 ambient = glm::vec3(1), glm::vec3 diffuse = glm::vec3(1), glm::vec3 specular = glm::vec3(1));
    void draw(glm::mat4 projection, glm::mat4 view, double time);
    const glm::vec3 &getAmbient() const;
    const glm::vec3 &getDiffuse() const;
    const glm::vec3 &getSpecular() const;
private:
    Shader shader;
    glm::vec3 ambient, diffuse, specular;
    std::vector<float> vertices;
    unsigned int VAO, VBO;
};


#endif //OPENGL_CMAKE_LIGHT_H
