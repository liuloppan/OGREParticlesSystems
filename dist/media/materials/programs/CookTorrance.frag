
// Input variable declarations
uniform vec3 eyePosition;
uniform vec3 lightPosition;

uniform float roughness, fresnel;

varying vec3 surfacePosition, surfaceNormal;

varying vec2 vTexCoord;



// Per-fragment operations
void main()
{
    float cookTorranceSpecular(
        vec3 lightDirection,
        vec3 viewDirection,
        vec3 surfaceNormal,
        float roughness,
        float fresnel) {

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
        float D = beckmannDistribution(NdotH, roughness);

        //Fresnel term
        float F = pow(1.0 - VdotN, fresnel);

        //Multiply terms and done
        return  G * F * D / max(3.14159265 * VdotN * LdotN, 0.000001);
    }

    //Light and view geometry
    vec3 viewDirection = normalize(eyePosition - surfacePosition);
    vec3 lightDirection = normalize(lightPosition - surfacePosition);

    //Surface properties
    vec3 normal = normalize(surfaceNormal);

    //Compute specular power
    float power = cookTorranceSpec(
                      lightDirection,
                      viewDirection,
                      normal,
                      roughness,
                      fresnel);

    gl_FragColor = vec4(power, power, power, 1.0);
}
