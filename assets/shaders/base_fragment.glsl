#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec3 FragNormal;
in vec2 TexCoord;

uniform struct Light {
    vec3 position;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
} light;

uniform struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float roughness;
} material;

uniform sampler2D albedoTexture;

void main()
{
    vec3 ambientColor = material.ambientColor * light.ambientColor;

    // Diffuse Lighting
    vec3 normal = normalize(FragNormal);
    vec3 lightDirection = normalize(light.position - FragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = diff * material.diffuseColor * light.diffuseColor;

    // Specular Lighting
    vec3 viewDirection = normalize(FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularIntensity = pow(max(dot(viewDirection, reflectionDirection), 0.0), 64);
    specularIntensity *= step(0.0, dot(normal, lightDirection));
    vec3 specularColor = (1.f - material.roughness) * specularIntensity * material.specularColor * light.specularColor;

    vec4 textureColor = texture(albedoTexture, TexCoord);
    vec3 finalColor = textureColor.rgb * (ambientColor + diffuseColor + specularColor);
    FragColor = vec4(finalColor, textureColor.a);
}