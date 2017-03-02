/*
Raeeca Narimani, rs47345
*/

uniform vec4 LMa; // Light-Material ambient
uniform vec4 LMd; // Light-Material diffuse
uniform vec4 LMs; // Light-Material specular
uniform float shininess;

uniform sampler2D normalMap;
uniform sampler2D decal;
uniform sampler2D heightField;
uniform samplerCube envmap;

uniform mat3 objectToWorld;


varying vec2 normalMapTexCoord;
varying vec3 lightDirection;
varying vec3 eyeDirection;
varying vec3 halfAngle;
varying vec3 c0, c1, c2;

varying vec3 light;

float diffuseLightingContribution;
float blinnSpecularContribution;
 
void main()
{

	lightDirection = normalize(lightDirection);
	
	float max = (lightDirection.z >= 0 )? lightDirection.z : (-1) * lightDirection.z;

	clamp(lightDirection, 0.0 , max);

	diffuseLightingContribution = dot(lightDirection, vec3(0,0,1));

  	blinnSpecularContribution = dot(normalize(halfAngle), c1);

  	blinnSpecularContribution = pow( blinnSpecularContribution ,  shininess);

	gl_FragColor = (diffuseLightingContribution * LMd) + (blinnSpecularContribution * LMs) + LMa;  // XXX fix me
}
