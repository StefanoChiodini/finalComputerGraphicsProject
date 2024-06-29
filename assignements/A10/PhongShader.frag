#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform GlobalUniformBufferObject {
	float time;
} gubo;

void main() {
	vec3 Norm = fragNorm;
	
	float y = fragPos.y;
	float t = gubo.time;
	float r = 0.0f, g = 0.0f, b = 0.0f;
	float h = mod(t/3.0 + y, 6.0);
	
	if(h < 1.0){
	r = 1.0;
	g = h;
	b = 0.0;
	} else if(1.0 <= h && h < 2.0){
	r = 2.0 - h;
	g = 1.0;
	b = 0.0;
	} else if(2.0 <= h && h < 3.0){
	r = 0.0;
	g = 1.0;
	b = h - 2.0;
	} else if (3.0 <= h && h < 4.0){
	r = 0.0;
	g = 4.0 - h;
	b = 1.0;
	} else if(4.0 <= h && h < 5.0){
	r = h - 4.0;
	g = 0.0;
	b = 1.0;
	} else if(h >= 5.0){
	r = 1.0;
	g = 0.0;
	b = 6.0 - h;
	}
	
	outColor = vec4(r, g, b, 1.0);
}