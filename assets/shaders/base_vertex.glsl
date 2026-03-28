#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragPosition;
out vec3 FragNormal;
out vec2 TexCoord;

uniform struct PVM{
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} pvm;

void main(){
    FragPosition = (pvm.view * pvm.model * vec4(position, 1.0)).xyz;
    FragNormal = mat3(pvm.normal) * normal;
    TexCoord = uv;

    gl_Position = pvm.projection * vec4(FragPosition, 1.0);
}