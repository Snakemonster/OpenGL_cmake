#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 matrixNormals;

void main(){
    gl_Position = projection * view * transform * vec4(position, 1.0);
    FragPos = vec3(transform * vec4(position, 1.0));
    Normal = matrixNormals * normals;
}