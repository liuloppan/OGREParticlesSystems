
// Input and output variable declarations
uniform mat4x4 mWorldViewProjMatrix;
uniform vec4 vPoseInfluence;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;

// Per-vertex operations
void main()
{
	mat4x3 mPoseMatrix = mat4x3(gl_MultiTexCoord1.xyz, gl_MultiTexCoord2.xyz, gl_MultiTexCoord3.xyz, gl_MultiTexCoord4.xyz);
	vPosition = gl_Vertex.xyz + mPoseMatrix * vPoseInfluence;
	gl_Position = mWorldViewProjMatrix * vec4(vPosition, gl_Vertex.w);
	
	vNormal = gl_Normal;
	vTexCoord = gl_MultiTexCoord0.st;
}
