/*		compile with gcc -o :p main.c -lGL -lGLU -lglut		*/
/*		recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/*	    number of recursive steps given on command line */

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <list>
#include "loader.h"
#include "geom.h"
using namespace std;
using std::vector;

/* initial fractal */

#define TRUE 1
#define FALSE 0
#define VIEWING_DISTANCE_MIN  3.0

const char *objectLoaderG;
Trimesh triPtrG;
TrimeshLoader trimeshLoaderG;
Trimesh tG;
Trimesh displayTrimesh1G;
Trimesh displayTrimesh2G;
static bool pointMode = false;
static bool wireFrameMode = false;
static bool solidMode = false;
static bool shadedMode = false;
static bool faceNorms = false;
static bool vertexNorms = false;


enum modesAndNormals
{
	Point_Mode,     // Just draws the vertices.
	Wireframe_Mode, // Draws the edges of the mesh.
	Solid_Mode,     // Draws filled triangles.
	Shaded_Mode,    // Basic lightings, see below.
	Face_Normals,   // Adds a toggle to display the face normals.
	Vertex_Normals, // Adds a toggle for the vertex normals as well.
};

enum objects
{
    bu_head,
    cactus,
    cessna,
    feline4k,
    mannequin,
    sphere,
    Exit
};

//the left, right, bottom and top values are specified for the near plane and not the far plane
void glFrustum(	GLdouble left,
		GLdouble right,
		GLdouble bottom,
		GLdouble top,
		GLdouble nearVal,
		GLdouble farVal);

typedef int BOOL;
static int g_yClick = 0;
static int xMouse = 0;
static int yMouse = 0;
static GLfloat theta_x;
static GLfloat phi_y;
static GLfloat g_fViewDistance = 3 * VIEWING_DISTANCE_MIN;
static BOOL g_bButton1Down = FALSE;
static GLfloat angle = 45.0;
static int g_Width = 500;                          // Initial window width
static int g_Height = 500;

void drawVertices(Trimesh *trimesh)
{

		GLfloat v[3];

	      for(int i = 0 ; i < trimesh->faceVectors.size() ; i++)
	      {			
				v[0] = trimesh->faceVectors.at(i).x;
				v[1] = trimesh->faceVectors.at(i).y;
				v[2] = trimesh->faceVectors.at(i).z;
				
				glColor4ub(0, 127, 255, 255);
				glVertex3fv(v);
	      }
}		


void drawMesh(Trimesh *trimesh)
{

		GLfloat va[3];
		GLfloat vb[3];
		GLfloat vc[3];

	 for(int i = 0 ; i < trimesh->theSurface.size() ; i++)
	 { 
            va[0] = trimesh->theSurface.at(i).v1.x;
			va[1] = trimesh->theSurface.at(i).v1.y;
			va[2] = trimesh->theSurface.at(i).v1.z;

			vb[0] = trimesh->theSurface.at(i).v2.x;
			vb[1] = trimesh->theSurface.at(i).v2.y;
			vb[2] = trimesh->theSurface.at(i).v2.z;

		    vc[0] = trimesh->theSurface.at(i).v3.x;
			vc[1] = trimesh->theSurface.at(i).v3.y;
			vc[2] = trimesh->theSurface.at(i).v3.z;

        glBegin(GL_TRIANGLES);
		glColor4ub(0, 127, 255, 255);
		glVertex3fv(va);
		glColor4ub(0, 127, 255, 255);
		glVertex3fv(vb);
		glColor4ub(0, 127, 255, 255);
		glVertex3fv(vc);
		glEnd();
	
	 }
}



void MouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON)
  {
	      g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
	      g_yClick = y - 3 * g_fViewDistance;
	      xMouse = x;
	      yMouse = y;
  }
}


void MouseMotion(int x, int y) {
    if (g_bButton1Down)
    {
      g_fViewDistance = (y - g_yClick) / 3.0;

      if (g_fViewDistance < VIEWING_DISTANCE_MIN)
      {
         g_fViewDistance = VIEWING_DISTANCE_MIN;
         theta_x = x - xMouse;
         phi_y = y - yMouse;
      }

      glutPostRedisplay();
    }
}



void SelectFromMenu(int idCommand) 
{
	

     switch (idCommand)
     {
  	    case bu_head:
			objectLoaderG = "models/bu_head.obj";
		
  	  	break;
		/////////////////////////////////////

        case cactus:
			objectLoaderG = "models/cactus.obj";
		
  		break;
		/////////////////////////////////////

        case cessna:
				objectLoaderG = "models/cessna.obj";

		break;
		/////////////////////////////////////

        case feline4k:
		    objectLoaderG = "models/feline4k.obj";

		break;
		////////////////////////////////////

	    case mannequin:
     		 objectLoaderG = "models/mannequin.obj";

		break;
		////////////////////////////////////

	    case sphere:
    	      objectLoaderG = "models/sphere.obj";

		break;

	    case Exit:             // ESCAPE key
		  exit (0);
		  break;

		////////////////////////////////////
     }

       trimeshLoaderG.loadOBJ(objectLoaderG, &tG );
 
       glutPostRedisplay();
}


int BuildPopupMenu (void) 
{
	  int menu;
	  menu = glutCreateMenu (SelectFromMenu);
	  glutAddMenuEntry ("bu_head", bu_head);
	  glutAddMenuEntry ("cactus", cactus);
	  glutAddMenuEntry ("cessna", cessna);
	  glutAddMenuEntry ("feline4k", feline4k);
	  glutAddMenuEntry ("mannequin", mannequin);
	  glutAddMenuEntry ("sphere", sphere);
	  glutAddMenuEntry ("Exit", Exit);

	  return menu;

}




void display()
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
 
   gluLookAt(0, 0, -g_fViewDistance, 0, 0, 0, 0, 1, 0);
   glRotatef(theta_x, 1.0,0.0,0.0);
   glRotatef(phi_y, 0.0,0.0,1.0);

  drawMesh (&tG);


	if (pointMode)
	{ 
		glutSwapBuffers();
	        glEnd();
	        glFlush();
		glBegin(GL_POINTS);
		drawVertices(&tG);
	}

   	if (wireFrameMode)
	{ 
		glutSwapBuffers();
	        glEnd();
	        glFlush();
		glBegin(GL_LINE_LOOP);
		drawMesh(&tG);
	}

	if (solidMode)
	{ 
		glutSwapBuffers();
	        glEnd();
	        glFlush();
		glBegin(GL_TRIANGLE_FAN);
		drawMesh(&tG);
	}

	if (shadedMode)
	{ 
		//glBegin();
	}

	if(faceNorms)
	{ 
		//glBegin();
	}

	if(vertexNorms)
	{ 
		//glBegin();
	}

   glutSwapBuffers();
   glFlush();

}

void myReshape(int g_Width, int g_Height)
{
    glViewport(0, 0, g_Width, g_Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)g_Width / (GLfloat)g_Height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Raeeca Narimani | UTEID: rs47345");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc (MouseButton);
    glutMotionFunc (MouseMotion);
 
     BuildPopupMenu ();
    glutAttachMenu (GLUT_RIGHT_BUTTON);
 
    glEnable(GL_DEPTH_TEST);
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glutMainLoop();

	return 0;
}
