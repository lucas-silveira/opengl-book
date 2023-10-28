#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  // we read the multiplication from right to left
  gl_Position = projection * view * model * vec4(a_pos, 1.0);
}