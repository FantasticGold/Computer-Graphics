#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec3 aNormal;

out VS_OUT {
  vec3 FragPos;
  vec2 TexCoord;
  vec3 Color;
  vec3 Normal;	
  vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpace;

void main() {
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
  vs_out.TexCoord = aTexCoord;
  vs_out.Color = aColor;
  vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
  vs_out.FragPosLightSpace = lightSpace * vec4(vs_out.FragPos, 1.0f);
}