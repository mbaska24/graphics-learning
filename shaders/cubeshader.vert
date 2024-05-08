#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 st;

out vec4 color;

uniform mat4 MVP;
uniform vec4 lightPosition;
uniform vec4 lightColor;


void main() {
//float absX = abs(pos.x); float absY = abs(pos.y);
//colorOffset = vec4(absX, absY, 0.0f, 1.0f);
vec4 pos4 = MVP * vec4(pos, 1.0f);
gl_Position = pos4;
float distance = distance(pos4,lightPosition);
//color = (lightColor / distance) * vec4(pos, 1.0f);
color = vec4(vertexNormal, 1.0f);
}