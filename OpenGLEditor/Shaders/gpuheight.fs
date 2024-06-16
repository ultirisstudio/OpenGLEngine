#version 440 core

in float fHeight;
in vec3 fNormal;

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform int uTextureScale;

void main()
{
    //float h = (fHeight + 16)/64.0f;
    //FragColor = vec4(h, h, h, 1.0);
    //vec3 color = normalize(fNormal) * 0.5 + 0.5;
    //FragColor = vec4(color, 1.0);

    FragColor = vec4(texture(uTexture, vec2(TexCoord.x * uTextureScale, TexCoord.y * uTextureScale)).rgb, 1.0);
}