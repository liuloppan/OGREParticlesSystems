
// Input and output variable declarations
uniform mat4x4 mWorldViewProjMatrix;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec4 vTexCoord;

// Per-vertex operations
void main()
{
    gl_Position = mWorldViewProjMatrix * gl_Vertex;

    vPosition = gl_Vertex.xyz;
    vNormal = gl_Normal;
    vTexCoord = gl_MultiTexCoord0.st;
}
