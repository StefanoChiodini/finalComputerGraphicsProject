#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformPoint {
    vec3 lightPos[12];
    vec4 lightColor;
    vec3 eyePos;
} gubo;

layout(set = 1, binding = 1) uniform sampler2D tex;

const float beta = 1.0f;
const float g = 1.5;

void main() {
    vec3 Norm = normalize(fragNorm);
    vec3 EyeDir = normalize(gubo.eyePos - fragPos);
    vec3 finalColor = vec3(0.0);

    for (int i = 0; i < 12; i++) {
        vec3 lightDir = normalize(gubo.lightPos[i] - fragPos);
        float distance = length(fragPos - gubo.lightPos[i]);
        float attenuation = pow((g / distance), beta);
        vec3 lightColor = gubo.lightColor.rgb * attenuation;

        vec3 Diffuse = texture(tex, fragUV).rgb * 0.99f * clamp(dot(Norm, lightDir), 0.0, 1.0);
        vec3 Specular = vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)), 0.0, 1.0), 160.0f));
        vec3 Ambient = texture(tex, fragUV).rgb * 0.01f;

        finalColor += clamp((Diffuse + Specular) * lightColor.rgb + Ambient, 0.0, 1.0);
    }

    outColor = vec4(finalColor / 12.0, 1.0);  // Dividi per il numero di luci per evitare sovraesposizione
}