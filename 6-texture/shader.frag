#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D aTex;

void main()
{
  FragColor = texture(aTex, texCoord); // * vec4(color, 1.0); mix texture color and vertex color
}