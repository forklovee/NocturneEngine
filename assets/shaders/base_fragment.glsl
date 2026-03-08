#version 330 core
out vec4 FragColor;

in vec3 camPosition;
in vec3 vertPosition;
in vec3 vertNormal;
in vec2 TexCoord;

// Lighting
uniform float ambientLightStrength;
uniform vec3 ambientLightColor;

uniform vec3 lightStrength;
uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform float specularStrength = 0.5f;


uniform sampler2D albedoTexture;

void main()
{
    vec3 ambientLight = ambientLightColor * ambientLightStrength;

    vec3 normal = normalize(vertNormal);
    vec3 lightDirection = normalize(lightPosition - vertPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * lightColor;

    vec3 camDirection = normalize(camPosition - vertPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularIntensity = pow(max(dot(camDirection, reflectionDirection), 0.0), 32);
    vec3 specularLight = specularStrength * specularIntensity * lightColor;

    vec4 textureColor = texture(albedoTexture, TexCoord);
    vec3 finalColor = textureColor.rgb * (ambientLight + diffuseLight + specularLight);
    FragColor = vec4(finalColor, textureColor.a);
}