#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 LightPosition;
out vec3 FragPosition;
out vec3 FragNormal;
out vec2 TexCoord;

uniform vec3 lightPosition;
uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    LightPosition = vec3(view * vec4(lightPosition, 1.0)); // Light Position in View Space!
    FragPosition = (view * model * vec4(position, 1.0)).xyz;
    FragNormal = mat3(normalMatrix) * normal;
    TexCoord = uv;

    gl_Position = projection * view * model * vec4(position, 1.0);
}