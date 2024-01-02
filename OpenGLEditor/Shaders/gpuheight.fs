#version 410 core

in float Height;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    float h = (Height + 16)/64.0f;
    FragColor = vec4(h, h, h, 1.0);
}