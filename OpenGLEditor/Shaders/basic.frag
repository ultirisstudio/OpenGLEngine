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

uniform vec3 uCameraPosition;

void main()
{
		VertexData vertex;
        vertex.position = fPosition;
        vertex.normal = fNormal;

		vec3 objectColor;
		vec3 objectSpecular;

		if (uMaterial.use_diffuse_texture == 1)
		{
			objectColor = texture(uMaterial.diffuse_texture, fTextureCoordinates).rgb;
		}
		else
		{
			objectColor = uMaterial.diffuse_color;
		}

		if (uMaterial.use_specular_texture == 1)
		{
			objectSpecular = texture(uMaterial.specular_texture, fTextureCoordinates).rgb;
		}
		else
		{
			objectSpecular = uMaterial.specular_color;
		}

		float ambientStrength = 0.1f;
		vec3 ambient = uLight.diffuse * objectColor;

		vec3 norm = normalize(vertex.normal);
		vec3 lightDir = normalize(uLight.position - vertex.position);

		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = uLight.diffuse * diff * objectColor;

		vec3 viewDir = normalize(uCameraPosition - vertex.position);
		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = spec * objectSpecular;
		
		vec3 result = (ambient + diffuse + specular);
		
        color = vec4(result, 1.0f);
};