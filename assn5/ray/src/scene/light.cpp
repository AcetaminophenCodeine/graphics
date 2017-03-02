#include <cmath>
#include "../vecmath/vec.h"
#include "../vecmath/mat.h"
#include "light.h"

using namespace std;

double DirectionalLight::distanceAttenuation(const Vec3d& P) const
{
  // distance to light is infinite, so f(di) goes to 0.  Return 1.
  return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.
  ray theRay ( p , getDirection(p), ray::SHADOW);
  isect i;
  if (scene -> intersect(theRay,i))
  {
        return Vec3d(0,0,0);
  }
      return Vec3d (1,1,1);
}


Vec3d DirectionalLight::getColor() const
{
  return color;
}

Vec3d DirectionalLight::getDirection(const Vec3d& P) const
{
  // for directional light, direction doesn't depend on P
  return -orientation;
}

double PointLight::distanceAttenuation(const Vec3d& P) const
{

  // YOUR CODE HERE

  // You'll need to modify this method to attenuate the intensity 
  // of the light based on the distance between the source and the 
  // point P.  For now, we assume no attenuation and just return 1.0

  //  f(d) = min( 1, 1/( a + b d + c d^2 ) )
  //  float constantTerm;   // a
  //  float linearTerm;     // b
  //  float quadraticTerm;  // c

  double d  = (position - P).length();
  double parabolaReciprocal = 1.0 / ( constantTerm + linearTerm * d + quadraticTerm * d * d) ;
  float minimum = min(1.0 , parabolaReciprocal);

  return minimum;
}

Vec3d PointLight::getColor() const
{
  return color;
}

Vec3d PointLight::getDirection(const Vec3d& P) const
{
  Vec3d ret = position - P;
  ret.normalize();
  return ret;
}


Vec3d PointLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{

  // YOUR CODE HERE:
  // You should implement shadow-handling code here.

  /*

    You are casting a ray from the intersection point in the direction of the light source.
    A point light will be at a distance "t" from the intersection point, so if you intersect an 
    object with a lower value "t", it means that it was before the light, so there's something 
    blocking the light source, and a shadow should be cast.

  */

  // 'p' is the position of the intersection between your original ray 
  // and the the object which will cast a shadow ray.

  ray theRay ( p , getDirection(p), ray::SHADOW);
  isect i;
  if (scene -> intersect(theRay,i))
  {
      if ((position - p).length() > i.t )
      {
        return Vec3d(0,0,0);
      }
  }

      return Vec3d (1,1,1);

}
