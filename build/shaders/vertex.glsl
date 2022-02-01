#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 matrixNormals;
uniform vec3 lightPos;


void main(){
    gl_Position = projection * view * transform * vec4(position, 1.0);
    FragPos = vec3(view * transform * vec4(position, 1.0));
    Normal = matrixNormals * normals;
    LightPos = vec3(view * vec4(lightPos, 1.0));
}