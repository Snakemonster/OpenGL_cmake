#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_handler.h"
#include "Spheres.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;
glm::vec3 lightPosSphere(1.2f, 1.0f, 2.0f);

Spheres::Spheres(float radius, int sectorCount, int stackCount, bool smooth) :
        lightingShader(Shader("shaders/vertex.glsl", "shaders/fragment.glsl")),
        lightCubeShader(Shader("shaders/light_cube_vertex.glsl","shaders/light_cube_fragment.glsl")),
        interleavedStride(6 * sizeof(float)) {
    this->radius = radius;
    this->sectorCount = sectorCount;
    if (sectorCount < MIN_SECTOR_COUNT) this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stackCount;
    if (sectorCount < MIN_STACK_COUNT) this->sectorCount = MIN_STACK_COUNT;

    if(smooth) buildVerticesSmooth();
    else buildVerticesFlat();

    glGenVertexArrays(1, &VAO_sphere);
    glBindVertexArray(VAO_sphere);

    glGenBuffers(1, &VBO_sphere);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sphere);
    glBufferData(GL_ARRAY_BUFFER, interleavedVertices.size() * sizeof(float), interleavedVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, interleavedStride, (void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, interleavedStride, (void *) (3 * sizeof(float)));


//    light cube
    verticesLight = {
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
    glGenVertexArrays(1, &VAO_cubelight);
    glBindVertexArray(VAO_cubelight);

    glGenBuffers(1, &VBO_cubelight);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cubelight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesLight.size(), verticesLight.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
}


void Spheres::draw(glm::mat4 projection, glm::mat4 view, double time) {

    //start of sphere
    lightingShader.use();
    lightingShader.setVec3("objectColor", 0.2f, 0.f, 0.f);
    lightingShader.setVec3("lightColor", 1.f, 1.f, 0.2f);
    lightingShader.setVec3("lightPos", lightPosSphere);

    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    glm::mat4 transform = glm::mat4(1.0f);
    lightingShader.setMat3("matrixNormals", glm::mat3(glm::transpose(glm::inverse(view * transform))));
    lightingShader.setMat4("transform", transform);

    glBindVertexArray(VAO_sphere);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, (void *) 0);
    //end of sphere

    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    lightPosSphere.x = static_cast<float>(sin(tan(time * 18)) * 2);
    lightPosSphere.y = static_cast<float>(sin(tan(time * 4.455345)) * cos(tan(time)) * 2);
    lightPosSphere.z = static_cast<float>(-cos(tan(time * 8))* 2);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, lightPosSphere);
    transform = glm::scale(transform, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("transform", transform);

    glBindVertexArray(VAO_cubelight);
    glDrawArrays(GL_TRIANGLES, 0, verticesLight.size() * sizeof(float));
}

void Spheres::buildVerticesSmooth() {
    const float PI = acos(-1);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
            }

            if (i != (stackCount - 1)) {
                addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void Spheres::buildVerticesFlat()  {
    const float PI = acos(-1);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex {
        float x, y, z;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);       // r * cos(u)
        float z = radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
    std::vector<float> n;                           // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for (i = 0; i < stackCount; ++i) {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2) {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for line (first stack requires only vertical line)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);

                index += 3;     // for next
            } else if (i == (stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for lines (last stack requires both vert/hori lines)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 3;     // for next
            } else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put indices of quad (2 triangles)
                addIndices(index, index + 1, index + 2);
                addIndices(index + 2, index + 1, index + 3);

                // indices for lines
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 4;     // for next
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

void Spheres::buildInterleavedVertices() {
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2) {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);
    }
}

void Spheres::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

void Spheres::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Spheres::addNormal(float x, float y, float z) {
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}

void Spheres::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

std::vector<float>
Spheres::computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON) {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
