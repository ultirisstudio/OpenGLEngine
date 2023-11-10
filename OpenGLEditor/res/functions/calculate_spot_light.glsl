vec3 spotLight(SpotLight light, LightMaterial material, VertexData vertex, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - vertex.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float diffuseImpact = max(dot(lightDir, vertex.normal), 0.0f);
	float specularImpact = pow(max(dot(halfwayDir, vertex.normal), 0.0f), material.shininess * 2.0f);

	vec3 ambient = material.ambient; // * light.ambient
	vec3 diffuse = material.diffuse * diffuseImpact; // * light.diffuse
	vec3 specular = material.specular * specularImpact; // * light.specular

	float dist = length(light.position - vertex.position);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2.0f));

	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutoff - light.outerCutoff);
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	return ambient + (diffuse + specular) * intensity; // * attenuation
}
