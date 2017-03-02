// The main ray tracer.

#pragma warning (disable: 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "scene/CubeMap.h"
#include "SceneObjects/trimesh.h"
#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through pixel(i,j), i.e. normalized window coordinates (x,y),
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.

Vec3d RayTracer::trace(double x, double y)
{
  // Clear out the ray cache in the scene for debugging purposes,
  if (TraceUI::m_debug) scene->intersectCache.clear();
  ray r(Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY);
  scene->getCamera().rayThrough(x,y,r);
  Vec3d ret = traceRay(r, traceUI->getDepth());
  ret.clamp();
  return ret;
}

Vec3d RayTracer::tracePixel(int i, int j)
{
	Vec3d col(0,0,0);

	if( ! sceneLoaded() ) 
		return col;

	double x;
	double y;
	int count = 0;
	//col = trace(x,y);

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;
	
	int aa = traceUI -> getRayCount();

    aa = sqrt(aa);

	double step = 1.0 / (double) aa;

	for (double row = i ; row < i + 1 ; row = row + step)
	{
		for (double column = j ; column < j + 1 ; column = column + step)
		{
				x = double(row)/double(buffer_width);
				y = double(column)/double(buffer_height);
				col += trace(x,y);
				count++;
		}
	}

	col = col / count;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);

	return col;
}


// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
Vec3d RayTracer::traceRay(ray& r, int depth)
{
	isect i;
	Vec3d colorC;
	double n_i, n_t, n;
	double cosT, cosI;

	if(scene->intersect(r, i)) 
	{
			// YOUR CODE HERE

			// An intersection occurred!  We've got work to do.  For now,
			// this code gets the material for the surface that was intersected,
			// and asks that material to provide a color for the ray.  

			// This is a great place to insert code for recursive ray tracing.
			// Instead of just returning the result of shade(), add some
			// more steps: add in the contributions from reflected and refracted
			// rays.

	    if (depth != 0)
	    {
			  const Material& m = i.getMaterial();
			  colorC = m.shade(scene, r, i);
		      Vec3d Q =  r.at(i.t);
			  Vec3d l = (-1) * r.getDirection();
		      Vec3d N = i.N;
		      // scene get camera get I - vi
		      double Ndotl = N * l; 
		      Vec3d R =  (2.0 * Ndotl) * N - l;
		      R.normalize();
		      int depth2 = depth - 1;
		      ray newRay(Vec3d(0,0,0), Vec3d(0,0,0), ray::REFLECTION);
		      if (!m.kr(i).iszero())
		      {
		      		newRay = ray(Q, R, ray::REFLECTION);
		      		colorC = colorC + prod(m.kr(i) , traceRay(newRay, depth2));
		      }
		      isect new_i;
		      if (scene->intersect(newRay, new_i) && new_i.obj == i.obj)
			  {
			  		n_i = m.index(i);
			  		n_t = 1.0;
			  		N = (-1) * N;	  		
			  }
			  else
			  {
			  	    n_i = 1.0;
			  		n_t = m.index(i);
			  }

			  n = n_i/n_t;
			  Vec3d V = (-1) * r.getDirection();
			  cosI = N * V;
			  cosT = 1 - n * n * (1 - cosI * cosI);
			  
			  //Q = newRay.at(new_i.t);

			  if (!m.kt(i).iszero() && cosT >= 0)  // check m.kt(i) > 0
			  {
			  		cosT = sqrt(cosT);
			  		Vec3d T = (n*cosI - cosT) * N - n * V;
			  		T.normalize();
			  		ray newRay2(Q, T, ray::REFRACTION);
			  		colorC =  colorC + prod( m.kt(i) , traceRay(newRay2, depth2) );
			  }
		}	
	}
	else
	{
		colorC = Vec3d(0.0, 0.0, 0.0);
	}
	
	return colorC;
}

RayTracer::RayTracer()
	: scene(0), buffer(0), buffer_width(256), buffer_height(256), m_bBufferReady(false)
{}

RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( char* fn ) {
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos ) path = ".";
	else path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
    Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}

	if( !sceneLoaded() ) return false;

	//trimesh -> generateNormals();

	return true;
}

void RayTracer::traceSetup(int w, int h)
{
	if (buffer_width != w || buffer_height != h)
	{
		buffer_width = w;
		buffer_height = h;
		bufferSize = buffer_width * buffer_height * 3;
		delete[] buffer;
		buffer = new unsigned char[bufferSize];
	}
	memset(buffer, 0, w*h*3);
	m_bBufferReady = true;
}

