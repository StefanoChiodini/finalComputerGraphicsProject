﻿#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;


layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
 vec3 lightPos; // position of the spotlight
 vec3 lightDir; // direction of the spotlight
 vec4 lightColor;// color of the spotlight 
 vec3 eyePos; // position of the viewer
} gubo;

layout(binding = 3) uniform sampler2D texEmit;

const float beta = 2.0f; // decay exponent of the spotlight
const float g = 160;   // target distance of the spotlight
const float cosout = 0.5; // cosine of the outer angle of the spotlight
const float cosin  = 0.95; // cosine of the inner angle of the spotlight

// coefficients for the spehrical harmonics ambient light term
const vec3 C00  = vec3( .38f, .43f, .45f)/8.0f;
const vec3 C1m1 = vec3( .29f, .36f, .41f)/8.0f;
const vec3 C10  = vec3( .04f, .03f, .01f)/8.0f;
const vec3 C11  = vec3(-.10f,-.10f,-.09f)/8.0f;
const vec3 C2m2 = vec3(-.06f,-.06f,-.04f)/8.0f;
const vec3 C2m1 = vec3( .01f,-.01f,-.05f)/8.0f;
const vec3 C20  = vec3(-.09f,-.13f,-.15f)/8.0f;
const vec3 C21  = vec3(-.06f,-.05f,-.04f)/8.0f;
const vec3 C22  = vec3( .02f, .00f,-.05f)/8.0f;

void main() {
 vec3 N = normalize(fragNorm);    // surface normal
 vec3 V = normalize(gubo.eyePos - fragPos); // viewer direction
 vec3 MD = texture(tex, fragUV).rgb;   // diffuse color
 vec3 MA = MD;        // ambient color
 vec3 MS = vec3(1);       // specular color
 vec3 ME = texture(texEmit, fragUV).rgb;  // emission color

 // Write the shader here
    // Compute the ambient lighting using spherical harmonics
    vec3 ambientSH = C00 + C1m1 * N.y + 
      C10 * N.z +  C11 * N.x +
      C2m2 * (N.x * N.y) + C2m1 * (N.y * N.z) + 
      C21 * (N.x * N.z) + C22 * (N.x * N.x - N.y * N.y) + 
      C20 * (3.0 * N.z * N.z - 1.0) ;

    vec3 ambient = ambientSH * MA * 0.25;  // Adjust the coefficient for darkness

    // Compute the diffuse lighting
    vec3 L = gubo.lightDir;
    float diffuse = clamp(dot(N,L), 0.0, 1.0);
    vec3 diffuseColor = diffuse * MD * 0.25;  // Adjust the coefficient for darkness

    vec3 finalColor = ambient + diffuseColor + ME;

    outColor = vec4(finalColor, 1.0);
}