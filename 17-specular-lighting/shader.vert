#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  fragPos = vec3(model * vec4(aPos, 1.0)); // we want the fragment position in world space
  normal = mat3(normalMatrix) * aNormal;
}