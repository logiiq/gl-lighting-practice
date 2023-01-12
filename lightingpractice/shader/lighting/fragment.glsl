// Lighting fshader
#version 400 core

out vec4 FragColor;

in vec3 normPos;
in vec2 texPos;
in vec3 fragPos;
in vec3 lightPos;

uniform sampler2D txSampler;
uniform float ambientStrength;

void main()
{
    // Define lighting
    vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(lightPos - fragPos);
    float lightDistance = length(lightPos - fragPos);
    vec3 norm = normalize(normPos); // Normalize normals to unit vectors

    // Calculate ambient lighting
    vec3 ambient = lightColour * ambientStrength;

    // Calculate diffuse lighting
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = diff * lightColour;
    float attenuation = 1.0f / (1.0f + 0.09 * lightDistance + 0.032 * (lightDistance * lightDistance));
    diffuse *= attenuation;

    // Calculate specular lighting
    float specularIntensity = 0.5f;
    vec3 viewDir = normalize(-fragPos); // We just normalize the negated vector of the fragment because viewer is always (0, 0, 0);
    //vec3 reflectDir = reflect(-viewDir, norm);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
    vec3 specular = specularIntensity * spec * lightColour;

    vec3 allLighting = ambient + diffuse;

    //vec4 result = texture(txSampler, texPos) * vec4(allLighting, 1.0f);
    vec4 result = vec4(0.0f, 0.3f, 0.2f, 1.0f) * vec4(allLighting, 1.0f);

    FragColor = result;
}