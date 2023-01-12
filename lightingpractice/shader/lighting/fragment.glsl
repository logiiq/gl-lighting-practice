// Lighting fshader
#version 400 core

out vec4 FragColor;

in vec3 normPos;
in vec2 texPos;
in vec3 fragPos;
in vec3 vp;

uniform sampler2D txSampler;

uniform vec3 lightPos;
uniform float ambientStrength;

void main()
{
    // Define lighting
    vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);

    // Lighting calculations
    vec3 norm = normalize(normPos);
    vec3 lightDir = normalize(lightPos - fragPos);

    float lightDistance = length(lightPos - fragPos);
    float att = 1.0f / (1.0f + 0.09 * lightDistance + 0.032 * (lightDistance * lightDistance));

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = (diff * lightColour);
    diffuse *= att;

    vec3 ambient = ambientStrength * lightColour;

    vec4 result = (
        vec4(ambient, 0.0f) + 
        vec4(diffuse, 0.0f)
    ) * texture(txSampler, texPos);

    FragColor = result;
}