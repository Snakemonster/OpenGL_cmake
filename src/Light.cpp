#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "shader_handler.h"
#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        shader(Shader("shaders/light_cube_vertex.glsl","shaders/light_cube_fragment.glsl")) {
    this->position = position;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;

    vertices = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
}

void Light::draw(glm::mat4 projection, glm::mat4 view, double time) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    position.x = static_cast<float>(sin(time)  * 8);
//    position.y += static_cast<float>(sin(time) * cos(time) * 2);
    position.z = static_cast<float>(-cos(time) * 2);

    glm::mat4 transform = glm::mat4(1.f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(0.2f)); // a smaller cube
    shader.setMat4("transform", transform);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(float));
}

const glm::vec3 &Light::getAmbient() const {
    return ambient;
}

const glm::vec3 &Light::getDiffuse() const {
    return diffuse;
}

const glm::vec3 &Light::getSpecular() const {
    return specular;
}
