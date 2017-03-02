//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

// who the hell cares if my identifiers are longer than 255 characters:
#pragma warning(disable : 4786)

#include <string>
#include <thread>
#include <vector>
using std::string;

class RayTracer;

class TraceUI {
public:
	TraceUI() : m_nDepth(0), m_nSize(512), m_displayRayCount(false), m_nRayCount(1), m_displayThreader(false), m_nThread(1)
			  , m_displayDebuggingInfo(false), m_shadows(true), m_smoothshade(true), raytracer(0), m_usingCubeMap(false), m_gotCubeMap(false), m_nFilterWidth(1)
              {}

	virtual int	run() = 0;

	// Send an alert to the user in some manner
	virtual void alert(const string& msg) = 0;

	// setters
	virtual void setRayTracer( RayTracer* r ) { raytracer = r; }

	//***************************************************
	void setCubeMap(bool b) { m_gotCubeMap = b; }
	void useCubeMap(bool b) { m_usingCubeMap = b; }
	//***************************************************

	// accessors:
	int	getSize() const { return m_nSize; }
	int getRayCount() const { return m_nRayCount; }

	int getThreads() const { return m_nThread; }

	int	getDepth() const { return m_nDepth; }
	int	getFilterWidth() const { return m_nFilterWidth; }

	bool isThreading() const {  return m_displayThreader; }
    bool isRayCounting() const { return m_displayRayCount; }
	bool shadowSw() const { return m_shadows; }
	bool smShadSw() const { return m_smoothshade; }

	static bool m_raycount;
	static bool m_thread;
	static bool m_debug;

protected:
	RayTracer*	raytracer;

	int	m_nSize;	// Size of the traced image
	int	m_nDepth;	// Max depth of recursion
	int m_nRayCount; //counts the number of rays per pixel
	int m_nThread;   
	                    
	// Determines whether or not to show debugging information
	// for individual rays.  Disabled by default for efficiency
	// reasons.

 	bool m_displayRayCount;
 	bool m_displayThreader;
	bool m_displayDebuggingInfo;
	bool m_shadows;  // compute shadows?
	bool m_smoothshade;  // turn on/off smoothshading?
	//*********************************************************
	bool m_usingCubeMap;  // render with cubemap
	bool m_gotCubeMap;  // cubemap defined
	//*********************************************************
	int  m_nFilterWidth;  // width of cubemap filter
};

#endif
