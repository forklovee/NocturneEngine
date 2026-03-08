#version 330 core
out vec4 FragColor;

in vec3 LightPosition;
in vec3 FragPosition;
in vec3 FragNormal;
in vec2 TexCoord;

// Lighting
uniform float ambientLightStrength;
uniform vec3 ambientLightColor;
uniform vec3 lightStrength;
uniform vec3 lightColor;
uniform float specularStrength = 1.0f;

uniform sampler2D albedoTexture;


void main()
{
    vec3 ambientLight = ambientLightColor * ambientLightStrength;

    // Diffuse Lighting
    vec3 normal = normalize(FragNormal);
    vec3 lightDirection = normalize(LightPosition - FragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * lightColor;

    // Specular Lighting
    vec3 viewDirection = normalize(FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularIntensity = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
    specularIntensity *= step(0.0, dot(normal, lightDirection));
    vec3 specularLight = specularStrength * specularIntensity * lightColor;

    vec4 textureColor = texture(albedoTexture, TexCoord);
    vec3 finalColor = textureColor.rgb * (ambientLight + diffuseLight + specularLight);
    FragColor = vec4(finalColor, textureColor.a);
}