#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
  float ambientStrength = 0.1;
  vec3 ambientLight = ambientStrength * lightColor;
  
  vec3 lightDir = normalize(lightPos - fragPos);
  vec3 norm = normalize(normal);

  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuseLight = diffuseStrength * lightColor;

  vec3 light = (ambientLight+diffuseLight) * objColor;
  FragColor = vec4(light, 1.0);
}