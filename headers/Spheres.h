#ifndef OPENGL_CMAKE_SPHERES_H
#define OPENGL_CMAKE_SPHERES_H
#include <vector>

class Spheres {
public:
    Spheres(float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);
    void draw(glm::mat4 projection, glm::mat4 view, double time);

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

    float radius;
    int sectorCount;
    int stackCount;
    Shader lightingShader, lightCubeShader;
    unsigned int VAO_sphere, VAO_cubelight, VBO_sphere, VBO_cubelight, EBO;
    std::vector <float> vertices, normals, verticesLight;
    std::vector <unsigned int> indices, lineIndices;


    std::vector<float> interleavedVertices;
    int interleavedStride;
};


#endif //OPENGL_CMAKE_SPHERES_H
