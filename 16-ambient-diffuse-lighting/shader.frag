#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
  vec3 ambientLight = 0.1 * lightColor;
  
  vec3 lightDir = normalize(lightPos - fragPos);
  vec3 norm = normalize(normal);

  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuseLight = diffuseStrength * lightColor;

  FragColor = vec4((ambientLight+diffuseLight) * objColor, 1.0);
}