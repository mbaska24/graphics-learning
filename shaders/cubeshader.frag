#version 460 core

in vec3 normal;
in vec3 worldPos;

out vec4 FragColor;

uniform vec4 lightPosition;
uniform vec4 lightColor;
uniform vec4 viewPos;


void main() {
float strength = 0.1f;
vec3 ambient = strength * vec3(lightColor);
vec3 colorful = vec3(0.849785, 0.062088, 0.086608);
vec3 worldNormal = normalize(normal);
vec4 pos4 = vec4(worldPos, 1.0f);
float distance = distance(pos4,lightPosition);
vec3 lightDirection = normalize(vec3(lightPosition - pos4));
float diffuse = max(dot(lightDirection, worldNormal), 0.0f);
vec3 diffuseComponent = diffuse * vec3(lightColor);


float specularStrength = 0.5f;
vec3 viewDirection = normalize(vec3(viewPos) - worldPos);
vec3 reflection = reflect(-viewDirection, worldNormal);
float spec = pow(max(dot(viewDirection, reflection), 0.0), 128);
vec3 specular = specularStrength * spec * vec3(lightColor);
//color = (lightColor / distance) * vec4(pos, 1.0f);
vec3 result = (ambient + diffuseComponent + specular) * colorful;
FragColor = vec4(result, 1.0f);
}