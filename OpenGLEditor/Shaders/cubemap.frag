#version 330 core

in VS_OUT
{
	vec3 fTexCoords;
} fs_in;

out vec4 color;

uniform samplerCube uCubemap;

void main()
{
	color = vec4(texture(uCubemap, fs_in.fTexCoords).rgb, 1.0f);
}