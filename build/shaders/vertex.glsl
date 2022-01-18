#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_pos;

out vec2 texturePos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * transform * vec4(position, 1.0);
    texturePos = vec2(texture_pos.x, 1.0 - texture_pos.y);
}