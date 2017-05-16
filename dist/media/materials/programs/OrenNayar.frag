
// Input variable declarations
uniform vec4 vLightPosition;
uniform vec4 vCameraPosition;

uniform vec4 vLightAmbient;
uniform vec4 vLightDiffuse;
uniform vec4 vLightSpecular;
uniform vec4 vLightAttenuation;

uniform vec4 vSurfEmissive;
uniform vec4 vSurfAmbient;
uniform vec4 vSurfDiffuse;
uniform vec4 vSurfSpecular;
uniform float fSurfShininess;

uniform sampler2D sFacialTex;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;

// Phong reflection model for one point light source
vec4 PhongReflect(vec3 vLightDir, float fLightDist, vec3 vViewDir, vec3 vNormal)
{
	// Compute light attenuation term
	float fLightAtt = 0.0;
	if(fLightDist <= vLightAttenuation.x)
		fLightAtt = 1.0 / (vLightAttenuation.y + vLightAttenuation.z * fLightDist + vLightAttenuation.w * fLightDist * fLightDist);

	// Compute diffuse and specular terms
	vec4 vDiffuse = max(0.0, dot(vLightDir, vNormal)) * vLightDiffuse * vSurfDiffuse;
	vec4 vSpecular = pow(max(0.0, dot(normalize(vLightDir + vViewDir), vNormal)), fSurfShininess) * vLightSpecular * vSurfSpecular;
	
	// Compute final reflected color
	vec4 vColor = vSurfEmissive + vLightAmbient * vSurfAmbient + fLightAtt * (vDiffuse + vSpecular);
	return vColor;
}

// Per-fragment operations
void main()
{
	// Obtain light direction and distance
	vec3 vLightDir = vLightPosition.xyz - vPosition;
	float fLightDist = length(vLightDir);
	vLightDir = vLightDir / fLightDist;
	
	// Compute lighting color based on Phong reflection model
	vec3 vViewDir = normalize(vCameraPosition.xyz - vPosition);
	vec3 vNormalDir = normalize(vNormal);
	vec4 vColor = PhongReflect(vLightDir, fLightDist, vViewDir, vNormalDir);
	
	// Multiply lighting color with texture colors to obtain final result
	//gl_FragColor = vColor * texture(sFacialTex, vTexCoord);
	gl_FragColor = vec4(1.0, 0, 0, 1.0);
}
