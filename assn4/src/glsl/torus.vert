/*
Raeeca Narimani, rs47345
*/

// the incoming (u,v) attribute
attribute vec2 parametric;



uniform vec3 lightPosition;  // Object-space *****************************
uniform vec3 eyePosition;    // Object-space *****************************

uniform vec2 torusInfo; 

// Varying variables provide an interface between Vertex and Fragment Shader


varying vec2 normalMapTexCoord; //

// vertex position to light position (lightPosition - vPos)
varying vec3 lightDirection;
varying vec3 halfAngle;
varying vec3 eyeDirection;
varying vec3 c0, c1, c2;

mat3 M;

varying vec3 light;
varying vec3 view;
float blinnSpecularContribution;

float PI = 3.14159265359;
varying vec3 vPos;

/*
	x(u,v) = [R + rcos(u)]cos(v)	0 <= u <= Pi
	y(u,v) = [R + rcos(u)]cos(v)	0 <= v <= 2Pi	
	z(u,v) = rsin(u)


	A = 1 = 4Rr(Pi)^2
	R = 1/[4r(Pi)^2]
	r = 1/[4R(Pi)^2]

	[R - rad(x^2 + y^2)]^2 + z^2 = r^2

	r = y/2Pi
	R = y/2Pi + x/2Pi 
*/


void xyzInitializer()
{
	
	float x , y , z;

	x = (torusInfo[0] + (torusInfo[1] * cos( 2 * PI * parametric[1]))) * cos(2 * PI * parametric[0]);
	y = (torusInfo[0] + (torusInfo[1] * cos( 2 * PI * parametric[1]))) * sin(2 * PI * parametric[0]);
	z = torusInfo[1] * sin(2 * PI * parametric[1]);

	vPos = vec3(x,y,z);
}

void gradient_du()
{
	float gradientX, gradientY, gradientZ;

	gradientX = (torusInfo[0] + (torusInfo[1] * cos( 2 * PI * parametric[1]))) * 2 * PI * (-1) * sin(2 * PI * parametric[0]);
	gradientY = (torusInfo[0] + (torusInfo[1] * cos( 2 * PI * parametric[1]))) * 2 * PI * cos(2 * PI * parametric[0]);
	gradientZ = 0;
	
	// the tangent ( vector parallel to tangent plane to F(u,v))
        c0 =  normalize( vec3(gradientX, gradientY, gradientZ));

	
}

void gradient_dv()
{
	float gradientX, gradientY, gradientZ;
	
	gradientX = torusInfo[1] * cos(2 * PI * parametric[0]) * 2 * PI * (-1) * sin( 2 * PI * parametric[1]);
	gradientY = torusInfo[1] * sin(2 * PI * parametric[0]) * 2 * PI * (-1) * sin( 2 * PI * parametric[1]);
	gradientZ = 2 * PI * torusInfo[1] * cos(2 * PI * parametric[1]);

 	// the normal (vector perpendicular to tangent plane to F(u,v))
	c1 = cross( c0, normalize(vec3( gradientX, gradientY, gradientZ)));

	// the binormal ( mutually orthogonal to the tangent-plane and to the tangent-plane`s-normal)
	c2 = cross(c0, c1);
} 

void main()
{

	  xyzInitializer();
	  gradient_du();
	  gradient_dv();

	  normalMapTexCoord = vec2(6 * parametric[0], 2 * parametric[1]);  // XXX fix me
	  
	  gl_Position = gl_ModelViewProjectionMatrix * vec4(vPos, 1);  // XXX fix me

	  light= lightPosition - vPos;  // XXX fix me **************************

	  //halfAngle = vec3(0);  // XXX fix me ************************

	  M = mat3 (c0,c2,c1);
 
	  lightDirection = light * M;

	  eyeDirection = eyePosition - vPos;
	  
            halfAngle = vec3((light.x + eyeDirection.x)/2, (light.y + eyeDirection.y)/2, (light.z + eyeDirection.z)/2);	
             
	  //halfAngle =  halfAngle * M;
	
          
	
}

