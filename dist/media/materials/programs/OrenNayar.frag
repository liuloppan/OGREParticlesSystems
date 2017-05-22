precision highp float;

uniform vec3 diffuseColor;
uniform float roughness;
uniform vec3 lightPosition;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec4 vTexCoord;

void main()
{
    const float PI = 3.14159;
    vec3 normal = normalize(vNormal);
    vec3 eyeDir = normalize(vPosition);

    float NdotL = dot(normal, lightPosition);
    float NdotV = dot(normal, vPosition);

    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);

    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = dot(vPosition - normal * dot(vPosition, normal), lightPosition - normal * dot(lightPosition, normal));

    float roughnessSquared = roughness * roughness;

    float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));

    float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));

    float C = sin(alpha) * tan(beta);

    float L1 = max(0.0, NdotL) * (A + B * max(0.0, gamma) * C);

    vec3 finalValue = diffuseColor * L1;
    gl_FragColor = vec4(finalValue, 1.0 * texture(vTexCoord));

}