#version 330 core

struct VertexData
{
        vec3 position;
        vec3 normal;
};

struct Material
{
        vec3 ambient_color;
        vec3 diffuse_color;
        vec3 specular_color;
		
		sampler2D diffuse_texture;
		sampler2D specular_texture;
		
		int use_diffuse_texture;
		int use_specular_texture;
		
        float shininess;
};

struct Light
{
		vec3 position;
		vec3 diffuse;
};

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTextureCoordinates;

out vec4 color;

uniform Material uMaterial;
uniform Light uLight;

void main()
{
		VertexData vertex;
        vertex.position = fPosition;
        vertex.normal = fNormal;

		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * uLight.diffuse;
		vec3 diffuse;
		vec3 specular;

		if (uMaterial.use_diffuse_texture == 1)
		{
			diffuse = texture(uMaterial.diffuse_texture, fTextureCoordinates).xyz;
		}
		else
		{
			diffuse = uMaterial.diffuse_color;
		}
		
		if (uMaterial.use_specular_texture == 1)
		{
			specular = texture(uMaterial.specular_texture, fTextureCoordinates).xyz;
		}
		else
		{
			specular = uMaterial.specular_color;
		}
		
		vec3 result = ambientStrength * diffuse; // + specular
		
        color = vec4(result, 1.0f);
};