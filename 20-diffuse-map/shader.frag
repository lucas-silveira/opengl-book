#version 330 core

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;

out vec4 FragColor;

uniform vec3 view_pos; // if we'd calculate the light from the view+model coordinate, we wouldn't need this uniform, as the view position would be (0, 0, 0)

struct Material
{
  sampler2D diffuse; // ambient has the same value as diffuse, so we can discart the first one
  vec3 specular;
  float shininess;
};
uniform Material material;

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

void main()
{
  vec3 ambient_light = light.ambient * vec3(texture(material.diffuse, tex_coord));
  
  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff_strength = max(dot(norm, light_dir), 0.0);
  vec3 diffuse_light = light.diffuse * diff_strength * vec3(texture(material.diffuse, tex_coord));

  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular_light = light.specular * spec * material.specular;

  vec3 light = ambient_light + diffuse_light + specular_light;
  FragColor = vec4(light, 1.0);
}