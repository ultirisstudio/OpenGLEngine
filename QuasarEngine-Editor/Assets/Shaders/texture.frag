#version 330 core

in VS_OUT
{
	vec2 fTexCoords;
} fs_in;

out vec4 color;

uniform sampler2D uTexture;

void main()
{
	color = vec4(texture(uTexture, fs_in.fTexCoords).rgb, 1.0f);
}