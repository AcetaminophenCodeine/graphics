
#include <cmath>
#include <float.h>
#include <algorithm>
#include <assert.h>
#include "trimesh.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

using namespace std;

Trimesh::~Trimesh()
{
    for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
        delete *i;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt ) 
      return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    
    if (!newFace->degen) 
      faces.push_back( newFace );


    // Don't add faces to the scene's object list so we can cull by bounding box
    // scene->add(newFace);
    return true;
}

char* Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

bool Trimesh::intersectLocal(ray& r, isect& i) const
{
    double tmin = 0.0;
    double tmax = 0.0;
    typedef Faces::const_iterator iter;
    bool have_one = false;
    for( iter j = faces.begin(); j != faces.end(); ++j )
      {
        isect cur;
        if( (*j)->intersectLocal( r, cur ) )
          {
        if( !have_one || (cur.t < i.t) )
          {
            i = cur;
            have_one = true;
          }
          }
      }
    if( !have_one ) i.setT(1000.0);
    return have_one;
}

bool TrimeshFace::intersect(ray& r, isect& i) const {
  return intersectLocal(r, i);
}

// Intersect ray r with the triangle abc.  If it hits returns true,
// and put the parameter in t and the barycentric coordinates of the
// intersection in u (alpha) and v (beta).

bool TrimeshFace::intersectLocal(ray& r, isect& i) const
{
    // parent is a trimesh in TrimeshFriend Class, 
    // ids[3] is an int array, 
    // vector<Vec3d> Vertices;
    // Vertices vertices;

    const Vec3d& a = parent->vertices[ids[0]];
    const Vec3d& b = parent->vertices[ids[1]];
    const Vec3d& c = parent->vertices[ids[2]];

    // YOUR CODE HERE
    
    //Vec3d normalAtC = (a - c) ^ (b - c);
    //normalAtC.normalize();

   // Vec3d normalAtB = (c - b) ^ (a - b);
    //normalAtB.normalize();

    Vec3d normalAtA = (b - a) ^ (c - a);
    Vec3d b_a_cross_c_a = normalAtA;
    normalAtA.normalize();

    // Q = R(t) = P + td
    // N.(Rt) = D **** N.(p + t.d) = D **** N.p + t.d.N = D ***   t = (D - N.p) / N.d
    // D

    double plane_d = normalAtA * a;
    // N.p
    double Ndotp = normalAtA * r.p;
    // N.d
    double Ndotd = normalAtA * r.d;

    if ( Ndotd != 0)
    {
         const double newIsect_t = (plane_d - Ndotp)/ Ndotd;

         if (newIsect_t <= RAY_EPSILON)
            return false;
          // Q = R(t) = P + td
          // N.(Rt) = D **** N.(p + t.d) = D **** N.p + t.d.N = D 
          // t = (D - N.p) / N.d

        Vec3d Q = r.at(newIsect_t);
        Vec3d a_c_cross_Q_c = (a-c)^(Q-c);
        Vec3d b_a_cross_Q_a = (b-a)^(Q-a);
        Vec3d c_b_cross_Q_b = (c-b)^(Q-b);

        double dot_productAtC = a_c_cross_Q_c * normalAtA;

        if (dot_productAtC >= 0)
        {
            double dot_productAtA = b_a_cross_Q_a * normalAtA;

            if (dot_productAtA >= 0)
            {
                  double dot_productAtB = c_b_cross_Q_b * normalAtA;

                   if ( dot_productAtB >= 0)
                   {
                          //********************************************
                          //  isect's barycentric coordinate computation
                          //*******************************************                

                          // area (ABC)
                          double denominatorDeterminant = b_a_cross_c_a * normalAtA; 
                          // area (BQC)
                          const double alpha = dot_productAtA / denominatorDeterminant;
                          // area (AQC)
                          const double beta = dot_productAtB / denominatorDeterminant;
                          const double gama = dot_productAtC / denominatorDeterminant; 


                          //*******************************************
                          //  isect's barycentric coordinate assigmnet
                          //*******************************************
                          i.setBary(alpha , beta , gama);

                          //*******************************************
                          //  isect N assigned:
                          //*******************************************
                         // parent -> generateNormals();

                          if(!parent->vertNorms)
                          { 
                              i.setN(normalAtA);
                          } 
                          else
                          {
                                const Vec3d& N1 = parent -> normals[ids[0]];
                                const Vec3d& N2 = parent -> normals[ids[1]];
                                const Vec3d& N3 = parent -> normals[ids[2]];

                                Vec3d norm = alpha * N1 + beta * N2 + gama * N3;
                                norm.normalize();
                                const Vec3d n = norm;
                                i.setN(norm);
                          }
                          //*******************************************
                          //  isect members assigned
                          //*******************************************
                          i.setT(newIsect_t);
                          const SceneObject *objct = this;
                          i.setObject (this);
                          //const Material m = objct -> getMaterial();
                          i.setMaterial(*material);
                          //const Vec2d uv = Vec2d (alpha,beta);
                          i.setUVCoordinates(Vec2d (alpha,beta));

                          return true; 
                }
            }
        }
    }
     
    return false;
}


void Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
        Vec3d faceNormal = (**fi).getNormal();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
            normals[i]  /= numFaces[i];
    }

    delete [] numFaces;
    vertNorms = true;
}
