#version 450 core
out vec4 FragColor;

in vec2 texturePos;

uniform sampler2D textures;

void main() {
    FragColor = texture(textures, texturePos);
}