vec3 dirLight(DirLight light, LightMaterial material, VertexData vertex, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float diffuseImpact = max(dot(lightDir, vertex.normal), 0.0f);
	float specularImpact = pow(max(dot(halfwayDir, vertex.normal), 0.0f), material.shininess * 2.0f);

	vec3 ambient = material.ambient * light.ambient;
	vec3 diffuse = material.diffuse * light.diffuse * diffuseImpact;
	vec3 specular = material.specular * light.specular * specularImpact;

	return ambient + diffuse + specular;
}
