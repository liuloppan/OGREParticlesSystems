precision highp float;

// Input variable declarations
uniform vec3 eyePosition;
//uniform vec3 lightPosition;

uniform vec3 lightPos;

uniform float roughnessValue;

uniform float fresnel;

//uniform vec4 warna;

varying vec3 surfacePosition, surfaceNormal;

varying vec2 vTexCoord;


// Per-fragment operations
void main()
{
    //Light and view geometry
    vec3 lightPosition = lightPos;
    vec3 viewDirection = normalize(eyePosition - surfacePosition);
    vec3 lightDirection = normalize(lightPosition - surfacePosition);

    //Surface properties
    vec3 normal = normalize(surfaceNormal);
    float specular = 0.0;
    float F0 = 0.8; // fresnel reflectance at normal incidence
    float k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)
    vec3 lightColor = vec3(0.9, 0.1, 0.1);

    float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);
    float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);

    //Half angle vector
    vec3 H = normalize(lightDirection + viewDirection);

    //Geometric term
    float NdotH = max(dot(surfaceNormal, H), 0.0);
    float VdotH = max(dot(viewDirection, H), 0.000001);
    float x = 2.0 * NdotH / VdotH;
    float G = min(1.0, min(x * VdotN, x * LdotN));

    //Distribution term
    //float D = beckmannDist(NdotH, roughness);
    // roughness (or: microfacet distribution function)
    // beckmann distribution function
    float mSquared = roughnessValue * roughnessValue ;
    float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
    float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
    float D = r1 * exp(r2);

    //Fresnel Schlick approxmation
    float F = pow(1.0 - VdotN, fresnel);
    F *= (1.0 - F0);
    F += F0;

    //Multiply terms and done
    specular = G * F * D / max(3.14 * VdotN * LdotN, 0.000001);

    vec3 finalValue = lightColor * LdotN * (k + specular * (1.0 - k));
    gl_FragColor = vec4(finalValue, 1.0 * texture(vTexCoord));
    //gl_FragColor = vec4(lightPosition, 1.0);
}
