#version 330 core

out vec4 FragColor;

void main()
{
  // The source light cube stays the same and is not affected by other shader fragments
  FragColor = vec4(1.0); // White color
}