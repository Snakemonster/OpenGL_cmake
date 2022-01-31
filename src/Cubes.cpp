#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_handler.h"
#include "Cubes.h"

Cubes::Cubes(Shader lightingShader, Shader lightCubeShader) : lightingShader(lightingShader), lightCubeShader(lightCubeShader) {
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

    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO_cube);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);


    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

}

void Cubes::draw(glm::mat4 projection, glm::mat4 view) {
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", 1.f, 1.f, 1.f);

    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    // world transformation
    glm::mat4 transform = glm::mat4(1.0f);
    lightingShader.setMat4("transform", transform);

    glBindVertexArray(VAO_cube);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(float));

    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(1.2f, 1.0f, 2.0f));
    transform = glm::scale(transform, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("transform", transform);

    glBindVertexArray(VAO_light);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(float));
}
