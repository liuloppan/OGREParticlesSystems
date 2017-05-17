
// Input variable declarations
uniform vec4 vLightAmbient;
uniform vec4 vLightDiffuse;
uniform vec4 vLightSpecular;

uniform vec4 vSurfAmbient;
uniform vec4 vSurfDiffuse;
uniform vec4 vSurfSpecular;

varying vec2 vTexCoord;

// Per-fragment operations
void main()
{
	// Compute light attenuation term
	float fLightAtt = 0.0;
	if(fLightDist <= vLightAttenuation.x)
		fLightAtt = 1.0 / (vLightAttenuation.y + vLightAttenuation.z * fLightDist + vLightAttenuation.w * fLightDist * fLightDist);

	// Compute diffuse and specular terms
	vec4 vDiffuse = vLightDiffuse * vSurfDiffuse;
	vec4 vSpecular = vLightSpecular * vSurfSpecular;
	vec4 vColor = vLightAmbient * vSurfAmbient + fLightAtt * (vDiffuse + vSpecular);
	gl_FragColor = vColor * texture(sSkinTex, vTexCoord);
	//gl_FragColor = vec4(1.0,0,0,1.0);
}
