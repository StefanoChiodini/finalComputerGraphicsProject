#version 450

// SPOT SHADER

layout(location = 0) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformSpot {
	vec3 lightPos;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

layout(set = 1, binding = 1) uniform sampler2D tex;

void main() {
	vec3 Norm = normalize(fragNorm);
    float distance = length(fragPos - gubo.lightPos);
	float attenuation = pow((g/distance), beta);
	float cosAngle = dot(lightDir, normalize(gubo.lightDir));
	float spot = clamp((cosAngle - cosout) / (cosin - cosout), 0.0f, 1.0f);
	vec3 lightColor = attenuation * gubo.lightColor.rgb * spot;