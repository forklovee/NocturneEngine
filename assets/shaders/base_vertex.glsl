#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 camPosition;
out vec3 vertPosition;
out vec3 vertNormal;
out vec2 TexCoord;

uniform vec3 viewPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vertPosition = (model * vec4(position, 1.0)).xyz;
    vertNormal = mat3(transpose(inverse(model))) * normal;
    TexCoord = uv;

    gl_Position = projection * view * model * vec4(position, 1.0);
}