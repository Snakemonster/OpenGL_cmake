#ifndef OPENGL_CMAKE_SPHERES_H
#define OPENGL_CMAKE_SPHERES_H
#include <vector>
#include "Light.h"

class Spheres {
public:
    Spheres(glm::vec3 position, float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);
    void draw(glm::mat4 projection, glm::mat4 view, Light lightCube);

    void setShininess(float shininess);
    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);
    void setAllPhongLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

private:
    void buildVerticesSmooth();
    void buildVerticesFlat();
    void buildInterleavedVertices();
    void clearArrays();

    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);

    float radius, shininess;
    int sectorCount;
    int stackCount;
    unsigned int VAO, VBO, EBO;
    Shader lightingShader;
    glm::vec3 position, ambient, diffuse, specular;
    std::vector <float> vertices, normals;
    std::vector <unsigned int> indices, lineIndices;


    std::vector<float> interleavedVertices;
    int interleavedStride;
};


#endif //OPENGL_CMAKE_SPHERES_H
