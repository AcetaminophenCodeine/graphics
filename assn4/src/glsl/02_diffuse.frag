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
varying vec3 light;
varying vec2 normalMapTexCoord;
varying vec3 lightDirection;
varying vec3 eyeDirection;
varying vec3 halfAngle;
varying vec3 c0, c1, c2;

float diffuseLightingContribution;
varying vec4 diffuseShading;  

void main()
{

	lightDirection = normalize(lightDirection);
	
	float max = (lightDirection.z >= 0 )? lightDirection.z: 0 ;

	clamp(lightDirection, 0.0 , max);

	
	//In order for the diffuse shading to reflect the light and material colors, you should modulate LMd by the diffuse coefficient and add LMa to output a final fragment color for this task.

	diffuseLightingContribution = dot(lightDirection, vec3(0,0,1));
	diffuseShading = diffuseLightingContribution * LMd + LMa;

	gl_FragColor = diffuseShading;  // XXX fix me
}

