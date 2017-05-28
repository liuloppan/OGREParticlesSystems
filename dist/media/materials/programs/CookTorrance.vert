#version 130

// Input and output variable declarations
uniform mat4x4 mWorldViewProjMatrix;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;

// Per-vertex operations
void main()
{
	vPosition = gl_Vertex.xyz;
	gl_Position = mWorldViewProjMatrix * vec4(vPosition, gl_Vertex.w);
	
	vNormal = gl_Normal;
	vTexCoord = gl_MultiTexCoord0.st;
}
