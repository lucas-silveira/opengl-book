#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // if we'd calculate the light from the view+model coordinate, we wouldn't need this uniform, as the view position would be (0, 0, 0)

void main()
{
  float ambienteStrength = 0.1;
  vec3 ambientLight = ambienteStrength * lightColor;
  
  vec3 lightDir = normalize(lightPos - fragPos);
  vec3 norm = normalize(normal);

  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuseLight = diffuseStrength * lightColor;

  int shininess = 32; // the shininess of the object surface
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specularLight = specularStrength * spec * lightColor;

  vec3 light = (ambientLight + diffuseLight + specularLight) * objColor;
  FragColor = vec4(light, 1.0);
}