#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec4 fragTan;
layout(location = 3) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;
layout(binding = 3) uniform sampler2D normMap;
layout(binding = 4) uniform sampler2D matMap;

layout(binding = 2) uniform GlobalUniformBufferObject {
  vec3 lightDir;
  vec4 lightColor;
  vec3 eyePos;
} gubo;

vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, float F0, float metallic, float roughness) {
  //vec3 V  - direction of the viewer
  //vec3 N  - normal vector to the surface
  //vec3 L  - light vector (from the light model)
  //vec3 Md - main color of the surface
  //float F0 - Base color for the Fresnel term
  //float metallic - parameter that mixes the diffuse with the specular term.
  //                 in particular, parmeter K seen in the slides is: float K = 1.0f - metallic;
  //float roughness - Material roughness (parmeter rho in the slides).
  //specular color Ms is not passed, and implicitely considered white: vec3 Ms = vec3(1.0f);

  float K = 1.0f - metallic;
    vec3 Ms = vec3(1.0f);
    vec3 H = normalize(V + L);
    float roughSqr = roughness * roughness;
    float NdotH = dot(N, H);
    float NdotV = dot(N, V);
    float NdotL = dot(N, L);
    float HdotV = dot(H, V);
    float HdotL = dot(H, L);
    float VdotN = dot(V, N);
    float LdotN = dot(L, N);
    float VdotH = dot(V, H);
    float HdotN = dot(H, N);
    F0 = 1.0;
    
    //Fresnel Term F
    float F = F0 + (1-F0)*pow(1-clamp(VdotH, 0.0000001f, 1.0), 5.0);

    //Distribution D
    float D = (exp(-(1-pow(HdotN, 2.0))/(pow(HdotN,2.0)*roughSqr)))/(3.1415926535f*roughSqr*pow(HdotN, 4.0));
     
    //Geometric Term G
    float G1 = min(1.0, (((2*HdotN)*VdotN)/VdotH)); 
    float G = min(G1, (((2*HdotN)*LdotN)/VdotH));
    // Specular reflection
    vec3 specular = Ms*vec3((D * F * G) / (4.0 * clamp(VdotN, 0.0000001f, 1.0)));

    // Diffuse reflection
    vec3 diffuse = Md*clamp(LdotN, 0.0, 1.0);

    // Combine the diffuse and specular reflections
    vec3 BRDF = K*diffuse+(1-K)*specular;
    return BRDF;
  
  
}

void main() {
    vec3 Norm = normalize(fragNorm);
    vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));
    vec3 Bitan = cross(Norm, Tan) * fragTan.w;
    mat3 tbn = mat3(Tan, Bitan, Norm);
    vec4 nMap = texture(normMap, fragUV);
    vec3 N = normalize(tbn * (nMap.rgb * 2.0 - 1.0));

    vec3 albedo = texture(tex, fragUV).rgb;

    vec4 MRAO = texture(matMap, fragUV);
    float roughness = MRAO.g;
    float ao = MRAO.b;
    float metallic = MRAO.r;

    vec3 L = gubo.lightDir;
    vec3 lightColor = gubo.lightColor.rgb;

    vec3 V = normalize(gubo.eyePos - fragPos);

    vec3 DiffSpec = BRDF(V, N, L, albedo, 0.3f, metallic, roughness);
    vec3 Ambient = albedo * 0.05f * ao;

    outColor = vec4(clamp(0.95 * DiffSpec * lightColor.rgb + Ambient, 0.0, 1.0), 1.0f);
}