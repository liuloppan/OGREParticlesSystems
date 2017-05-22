
precision highp float;
// Input variable declarations
uniform vec3 eyePosition;
uniform float roughness, albedo;

uniform vec3 lightPos;

varying vec3 surfacePosition, surfaceNormal;
varying vec2 vTexCoord;


// Per-fragment operations
void main()
{


    //Light and view geometry
    vec3 lightPosition = lightPos;
    vec3 lightDirection = normalize(lightPosition - surfacePosition);
    vec3 viewDirection = normalize(eyePosition - surfacePosition);

    //Surface properties
    vec3 normal = normalize(surfaceNormal);

    float LdotV = dot(lightDirection, viewDirection);
    float NdotL = dot(lightDirection, surfaceNormal);
    float NdotV = dot(surfaceNormal, viewDirection);

    float s = LdotV - NdotL * NdotV;
    float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

    float sigma2 = roughness * roughness;
    float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
    float B = 0.45 * sigma2 / (sigma2 + 0.09);

    return albedo * max(0.0, NdotL) * (A + B * s / t) / 3.14159265;


    gl_FragColor = vec4(power, power, power, 1.0 * texture(vTexCoord));
}
