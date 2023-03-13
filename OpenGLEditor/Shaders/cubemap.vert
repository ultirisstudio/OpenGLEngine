#version 330 core

layout(location = 0) in vec3 vPosition;

out VS_OUT
{
    vec3 fTexCoords;
} vs_out;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vs_out.fTexCoords = vPosition;
    vec4 pos = uProjection * uView * vec4(vPosition, 1.0f);
    gl_Position = pos.xyww;
}