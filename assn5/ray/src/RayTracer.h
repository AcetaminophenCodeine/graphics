#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.


#include <time.h>
#include <queue>
#include "SceneObjects/trimesh.h"
#include "scene/ray.h"
#include "scene/CubeMap.h"

class Scene;


class RayTracer
{
	public:
		RayTracer();
	        ~RayTracer();

		Vec3d tracePixel(int i, int j);
		Vec3d trace(double x, double y);
		Vec3d traceRay(ray& r, int depth);

		void getBuffer(unsigned char *&buf, int &w, int &h);
		double aspectRatio();

		void traceSetup( int w, int h );

		bool loadScene(char* fn);
		bool sceneLoaded() { return scene != 0; }

		void setReady(bool ready) { m_bBufferReady = ready; }
		bool isReady() const { return m_bBufferReady; }

		const Scene& getScene() { return *scene; }

		//******************** Cube Map *********************
		void setCubeMap(CubeMap* cm)  {  cubemap = cm;	}
		const CubeMap& getCubeMap() {	return *cubemap;  }
		bool haveCubeMap() {	return m_haveCubeMap;	}

		

		//***************************************************

	public:
	        unsigned char *buffer;
	        int buffer_width, buffer_height;
	        int bufferSize;
	        Scene* scene;

	        //*********************
	        CubeMap* cubemap;
	        //*********************

	        Trimesh* trimesh;
	        bool m_bBufferReady;
	        bool m_haveCubeMap;
};

#endif // __RAYTRACER_H__
