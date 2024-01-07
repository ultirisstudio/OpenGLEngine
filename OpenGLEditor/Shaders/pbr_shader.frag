#version 330 core

in vec2 fTextureCoordinates;
in vec3 fWorldPos;
in vec3 fNormal;

out vec4 color;

// material parameters
struct Material
{
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

    vec3 albedoColor;
    float metallic;
    float roughness;
    float ao;

    bool use_albedo_texture;
    bool use_normal_texture;
    bool use_metallic_texture;
    bool use_roughness_texture;
    bool use_ao_texture;
};

// lights
struct PointLight {    
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4

uniform int uUsePointLight;
uniform PointLight uPointLights[NR_POINT_LIGHTS];

uniform Material uMaterial;

uniform vec3 uCameraPosition;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(uMaterial.normalMap, fTextureCoordinates).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fWorldPos);
    vec3 Q2  = dFdy(fWorldPos);
    vec2 st1 = dFdx(fTextureCoordinates);
    vec2 st2 = dFdy(fTextureCoordinates);

    vec3 N   = normalize(fNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 calculateReflectance(PointLight light, vec3 V, vec3 N, vec3 F0, vec3 albedo, float roughness, float metallic)
{
    // calculate per-light radiance
    vec3 L = normalize(light.position - fWorldPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fWorldPos);
    float attenuation = 1.0 / (distance * distance); //(light.constant + light.linear * distance + light.quadratic * (distance * distance))
    vec3 radiance = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * radiance * NdotL; //  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}
// ----------------------------------------------------------------------------
void main()
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 N;

    if (!uMaterial.use_albedo_texture)
		albedo = uMaterial.albedoColor;
    else
        albedo = pow(texture(uMaterial.albedoMap, fTextureCoordinates).rgb, vec3(2.2));

    if (!uMaterial.use_normal_texture)
        N = normalize(fNormal);
    else
        N = getNormalFromMap();

    if (!uMaterial.use_metallic_texture)
        metallic = uMaterial.metallic;
    else
        metallic = texture(uMaterial.metallicMap, fTextureCoordinates).r;

    if (!uMaterial.use_roughness_texture)
        roughness = uMaterial.roughness;
    else
        roughness = texture(uMaterial.roughnessMap, fTextureCoordinates).r;

    if (!uMaterial.use_ao_texture)
        ao = uMaterial.ao;
    else
        ao = texture(uMaterial.aoMap, fTextureCoordinates).r;

    vec3 V = normalize(uCameraPosition - fWorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < uUsePointLight; ++i)
    {
        Lo += calculateReflectance(uPointLights[i], V, N, F0, albedo, roughness, metallic);
    }
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.6) * albedo * ao;
    
    vec3 result = ambient + Lo;

    // HDR tonemapping
    result = result / (result + vec3(1.0));
    // gamma correct
    result = pow(result, vec3(1.0/2.2)); 

    color = vec4(result, 1.0);
}