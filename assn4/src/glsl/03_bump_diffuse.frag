
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

void main()
{
  
  //vec3 n = normalMap * M;
  //vec3 n = texture2D(normalMap, normalMapTexCoord) * 2 - 1;
  //float dotProduct = dot(n , normalize(lightDirection));
  //float clmp = max(dotProduct, 0);
  //gl_FragColor = clmp * LMd + LMa;
  //gl_FragColor = clmp *LMd + LMa;
  gl_FragColor = vec4(0,0,1);
}
