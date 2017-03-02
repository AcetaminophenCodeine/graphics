#ifndef __GEOM_H__
#define __GEOM_H__
#include <iostream>
#include <GL/glut.h>
#include <map>
#include <vector>
#include <math.h>
#include <list>
using namespace std;

class Trimesh
{ 
	public:

	typedef struct
	{
		float x;
		float y;
		float z;

	}structVertex;

	typedef struct 
	{
	
		float x;
		float y;
		float z;
	        int *faceArr;
		int arrSize;
	         structVertex vectorNorm;

	}structVector;

	typedef struct 
	{
		structVector v1;
	        structVector v2;
	        structVector v3;

		structVertex faceNorm;	
		structVertex midPoint;

	}structFace;	
	
	std:: vector <structVector> faceVectors; //vectors
        std:: vector <structFace> theSurface; //Faces

	
	void addVertex(float arr[]) 
	{

		
		structVector theVector;

		theVector.x = arr[0];
		theVector.y = arr[1];
		theVector.z = arr[2];
		
		faceVectors.push_back(theVector);
	}

	void addFace(int arr[]) 
	{

		structFace theFace;
		theFace.v1 = faceVectors.at(arr[0]);
		theFace.v2 = faceVectors.at(arr[1]);
		theFace.v3 = faceVectors.at(arr[2]);

	        //   i  j  k
		//   xa ya za
	        //   xb yb zb
 		
		float x1 = theFace.v1.x;
		float x2 = theFace.v2.x;
		float xa = x2 - x1;
		float x3 = theFace.v3.x;
		float xb = x3 - x1;

		float y1 = theFace.v1.y;
		float y2 = theFace.v2.y;
		float ya = y2 - y1;
		float y3 = theFace.v3.y;
		float yb = y3 - y1;

		float z1 = theFace.v1.z;
		float z2 = theFace.v2.z;
		float za = z2 - z1;
		float z3 = theFace.v3.z;
		float zb = z3 - z1; 

		theFace.faceNorm.x = (ya * zb) - (za * yb);     
		theFace.faceNorm.y = (za * xb) - (xa * zb);			     
		theFace.faceNorm.z = (xa * yb) - (ya * xb);

		float midLeg1x = (x1 + x2)/2;
		float midLeg1y = (y1 + y2)/2;
		float midLeg1z = (z1 + z2)/2;
	
		float midLeg2x = (x1 + x3)/2;
		float midLeg2y = (y1 + y3)/2;
		float midLeg2z = (z1 + z3)/2;
		
		float midTrianglex = (midLeg1x + midLeg2x)/2;
		float midTriangley = (midLeg1y + midLeg2y)/2;
		float midTrianglez = (midLeg1z + midLeg2z)/2;

		theFace.midPoint.x = midTrianglex;
		theFace.midPoint.y = midTriangley;
		theFace.midPoint.z = midTrianglez;

		theSurface.push_back(theFace);

		int ticket = theSurface.size() - 1;

		for ( int i = 0 ; i < 3 ; i++)
		{
			if (faceVectors.at(arr[i]).faceArr == NULL)
			{
				faceVectors.at(arr[i]).faceArr = new int [1];
				faceVectors.at(arr[i]).arrSize = 1;

				faceVectors.at(arr[i]).faceArr[0] = ticket;
			}
			else
			{
				faceVectors.at(arr[i]).arrSize += 1;
				faceVectors.at(arr[i]).faceArr = new int [faceVectors.at(arr[i]).arrSize];

				faceVectors.at(arr[i]).faceArr[faceVectors.at(arr[i]).arrSize - 1] = ticket;
			}
		}	
	}
};

#endif
