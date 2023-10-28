#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 frag_pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal_matrix;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection * view * model * vec4(a_pos, 1.0);
  frag_pos = vec3(model * vec4(a_pos, 1.0)); // we want the fragment position in world space
  normal = mat3(normal_matrix) * a_normal;
}