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
varying vec3 view;
float diffuseLightingContribution;
float blinnSpecularContribution;

void main()
{
	
  blinnSpecularContribution = dot(normalize(halfAngle), c1);
  blinnSpecularContribution = pow( blinnSpecularContribution ,  shininess);

  ///if (diffuseLightingContribution != 0)
	// blinnSpecularContribution = 0;
  // else if (blinnSpecularContribution < 0)
	// blinnSpecularContribution = 0;
   

  gl_FragColor = blinnSpecularContribution * LMs +  LMa;  // XXX fix me
}
