/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */

#include <stdlib.h>
#include <GL/glut.h> 


/* initial fractal */

GLfloat v[8][3]={  
		   {-0.5, 0.5, 0.5}, //a
		   {-0.5, 0.5, -0.5}, //b
		   {-0.5, -0.5,-0.5}, //d
		   {-0.5, -0.5, 0.5}, //c
		   {0.5, 0.5, 0.5}, //e
		   {0.5, 0.5, -0.5}, //f
		   {0.5, -0.5, -0.5}, //h
       {0.5, -0.5, 0.5}, //g
		   
		};

                        // red, green, blue
GLfloat colors[6][3] = {
			{0.5, 0.0, 1.0}, 
			{0.0, 0.6, 0.9},
      {0.0, 0.0, 1.0},
			{0.2, 0.5, 0.9},
      {0.1, 0.4, 1.0},
			{0.0, 0.0, 0.0}
                       };

#define TRUE 1
#define FALSE 0
#define VIEWING_DISTANCE_MIN  3.0

enum 
{
	RECURSION_LEVEL_1,
	RECURSION_LEVEL_2,
	RECURSION_LEVEL_3,
	RECURSION_LEVEL_4,
        MENU_EXIT
};

typedef int BOOL;

int n;
static int g_yClick = 0;
static int xMouse = 0;
static int yMouse = 0;
static GLfloat theta_x;
static GLfloat phi_y;
static GLfloat g_fViewDistance = 3 * VIEWING_DISTANCE_MIN;
static BOOL g_bButton1Down = FALSE;
static GLfloat angle = 45.0;
void sqr(GLfloat *va, GLfloat *vb, GLfloat *vc, GLfloat *vd)
{
       glVertex3fv(va);
       glVertex3fv(vb);
       glVertex3fv(vc);
       glVertex3fv(vd);
}

void cube(GLfloat *a, GLfloat *b, GLfloat *d, GLfloat *c,
          GLfloat *e, GLfloat *f, GLfloat *h, GLfloat *g)
{
    glColor3fv(colors[0]);
    sqr(a, b, d, c);
    glColor3fv(colors[1]);
    sqr(a, e, g, c);
    glColor3fv(colors[2]);
    sqr(a, b, f, e);
    glColor3fv(colors[3]);
    sqr(c, d, h, g);
    glColor3fv(colors[4]);
    sqr(b, f, h, d);
    glColor3fv(colors[5]);
    sqr(e, f, h, g);
}

void divide_cube(GLfloat *a, GLfloat *b, GLfloat *d, GLfloat *c,
		 GLfloat *e, GLfloat *f, GLfloat *h, GLfloat *g, int m)
{

    GLfloat mid[56][3];
    int j;

    if( m > 0 )
    {
        /* compute six midpoints */

	// passing order : a , mid [ 0 ... 6]
        for(j=0; j<3; j++) mid[0][j]=(2*a[j]+b[j])/3;
	      for(j=0; j<3; j++) mid[1][j]=(2*a[j]+d[j])/3;
        for(j=0; j<3; j++) mid[2][j]=(2*a[j]+c[j])/3;
        for(j=0; j<3; j++) mid[3][j]=(2*a[j]+e[j])/3;
        for(j=0; j<3; j++) mid[4][j]=(2*a[j]+f[j])/3;
	      for(j=0; j<3; j++) mid[5][j]=(2*a[j]+h[j])/3;
        for(j=0; j<3; j++) mid[6][j]=(2*a[j]+g[j])/3;
		///////////////////////////////////////////
	// passing order : mid[7] , b , mid [8 ... 13]
        for(j=0; j<3; j++) mid[7][j]=(2*b[j]+a[j])/3;
        for(j=0; j<3; j++) mid[8][j]=(2*b[j]+d[j])/3;
        for(j=0; j<3; j++) mid[9][j]=(2*b[j]+c[j])/3;
	      for(j=0; j<3; j++) mid[10][j]=(2*b[j]+e[j])/3;
        for(j=0; j<3; j++) mid[11][j]=(2*b[j]+f[j])/3;
        for(j=0; j<3; j++) mid[12][j]=(2*b[j]+h[j])/3;
	      for(j=0; j<3; j++) mid[13][j]=(2*b[j]+g[j])/3;
		//////////////////////////////////////////
	// passing order: mid [14, 15], d, mid[16 ... 20]
        for(j=0; j<3; j++) mid[14][j]=(2*d[j]+a[j])/3;
        for(j=0; j<3; j++) mid[15][j]=(2*d[j]+b[j])/3;
	      for(j=0; j<3; j++) mid[16][j]=(2*d[j]+c[j])/3;
        for(j=0; j<3; j++) mid[17][j]=(2*d[j]+e[j])/3;
      	for(j=0; j<3; j++) mid[18][j]=(2*d[j]+f[j])/3;
      	for(j=0; j<3; j++) mid[19][j]=(2*d[j]+h[j])/3;
      	for(j=0; j<3; j++) mid[20][j]=(2*d[j]+g[j])/3;
		//////////////////////////////////////////
	// passing order : mid[21,22,23], c , mid[24 ... 27]
        for(j=0; j<3; j++) mid[21][j]=(2*c[j]+a[j])/3;
      	for(j=0; j<3; j++) mid[22][j]=(2*c[j]+b[j])/3;
        for(j=0; j<3; j++) mid[23][j]=(2*c[j]+d[j])/3;
      	for(j=0; j<3; j++) mid[24][j]=(2*c[j]+e[j])/3;
      	for(j=0; j<3; j++) mid[25][j]=(2*c[j]+f[j])/3;
        for(j=0; j<3; j++) mid[26][j]=(2*c[j]+h[j])/3;
        for(j=0; j<3; j++) mid[27][j]=(2*c[j]+g[j])/3;
		///////////////////////////////////////////
	// passing order : mid[28 ... 31], e , mid[32,33,34]
	      for(j=0; j<3; j++) mid[28][j]=(2*e[j]+a[j])/3;
        for(j=0; j<3; j++) mid[29][j]=(2*e[j]+b[j])/3;
        for(j=0; j<3; j++) mid[30][j]=(2*e[j]+d[j])/3;
      	for(j=0; j<3; j++) mid[31][j]=(2*e[j]+c[j])/3;
        for(j=0; j<3; j++) mid[32][j]=(2*e[j]+f[j])/3;
        for(j=0; j<3; j++) mid[33][j]=(2*e[j]+h[j])/3;
	      for(j=0; j<3; j++) mid[34][j]=(2*e[j]+g[j])/3;
		///////////////////////////////////////////
	// passing order : mid [35 ... 39], f , mid[40, 41]
        for(j=0; j<3; j++) mid[35][j]=(2*f[j]+a[j])/3;
        for(j=0; j<3; j++) mid[36][j]=(2*f[j]+b[j])/3;
        for(j=0; j<3; j++) mid[37][j]=(2*f[j]+d[j])/3;
	      for(j=0; j<3; j++) mid[38][j]=(2*f[j]+c[j])/3;
        for(j=0; j<3; j++) mid[39][j]=(2*f[j]+e[j])/3;
        for(j=0; j<3; j++) mid[40][j]=(2*f[j]+h[j])/3;
	      for(j=0; j<3; j++) mid[41][j]=(2*f[j]+g[j])/3;
		///////////////////////////////////////////
	// pasing order : mid[42, ... , 47] , h , mid[48]
        for(j=0; j<3; j++) mid[42][j]=(2*h[j]+a[j])/3;
        for(j=0; j<3; j++) mid[43][j]=(2*h[j]+b[j])/3;
        for(j=0; j<3; j++) mid[44][j]=(2*h[j]+d[j])/3;
	      for(j=0; j<3; j++) mid[45][j]=(2*h[j]+c[j])/3;
        for(j=0; j<3; j++) mid[46][j]=(2*h[j]+e[j])/3;
        for(j=0; j<3; j++) mid[47][j]=(2*h[j]+f[j])/3;
	      for(j=0; j<3; j++) mid[48][j]=(2*h[j]+g[j])/3;
		///////////////////////////////////////////
	// passing order : mid[49 ... 55] , g
	      for(j=0; j<3; j++) mid[49][j]=(2*g[j]+a[j])/3;
        for(j=0; j<3; j++) mid[50][j]=(2*g[j]+b[j])/3;
        for(j=0; j<3; j++) mid[51][j]=(2*g[j]+d[j])/3;
	      for(j=0; j<3; j++) mid[52][j]=(2*g[j]+c[j])/3;
        for(j=0; j<3; j++) mid[53][j]=(2*g[j]+e[j])/3;
        for(j=0; j<3; j++) mid[54][j]=(2*g[j]+f[j])/3;
	      for(j=0; j<3; j++) mid[55][j]=(2*g[j]+h[j])/3;
	       ////////////////////////////////////////////
	
        
        /* create 4 tetrahedrons by subdivision */
	
	
        divide_cube(a        , mid[0], mid[1], mid[2], mid[3], mid[4], mid[5], mid[6], m-1); // cube 1
        divide_cube(mid[7]   , b , mid[8], mid[9], mid[10], mid[11], mid[12], mid[13], m-1); // cube 3
        divide_cube(mid [14] , mid[15], d, mid[16] , mid[17], mid[18] , mid[19],  mid [20], m-1); // cube 9
        divide_cube(mid[21]  , mid[22],mid[23], c , mid[24] , mid[25] , mid[26] , mid[27], m-1); // cube 7
	      divide_cube(mid[28]  , mid[29], mid[30] , mid[31], e , mid[32] , mid[33],mid[34], m-1); // cube 19
        divide_cube(mid [35] , mid[36] , mid[37] , mid[38], mid[39], f , mid[40] , mid[41], m-1); // cube 21
        divide_cube(mid[42]  , mid[43], mid[44] , mid[45] , mid[46], mid[47] , h , mid[48], m-1); // cube 25
        divide_cube(mid[49]  , mid[50] , mid[51], mid[52] , mid[53] , mid[54] , mid[55] , g, m-1); // cube 27
      	divide_cube(mid[0]   , mid[7] , mid[9], mid[1] , mid[4] , mid[10] , mid[13] , mid[5], m-1); // cube 2
      	divide_cube(mid[9]   , mid[8] , mid[15], mid[14] , mid[13] , mid[12] , mid[18], mid[17], m-1); // cube 6 
      	divide_cube(mid[22]  , mid[14] , mid[16] , mid[23] , mid[25] , mid[17] , mid[20] , mid[26], m-1); // cube 8 
      	divide_cube(mid[2]   , mid[1] , mid[22], mid[21] , mid[6] , mid[5] , mid[25] , mid[24], m-1); // cube 4
      	divide_cube(mid[29]  , mid[35] , mid[38], mid[30] , mid[32] , mid[39] , mid[41] , mid[33], m-1); // cube 20
      	divide_cube(mid[38]  , mid[37] , mid[43], mid[42] , mid[41] , mid[40] , mid[47] , mid[46], m-1); // cube 24
      	divide_cube(mid[50]  , mid[42] , mid[45], mid[51] , mid[54] , mid[46] , mid[48] , mid[55], m-1); // cube 26
      	divide_cube(mid[31]  , mid[30] , mid[50], mid[49] , mid[34] , mid[33] , mid[54] , mid[53], m-1); // cube 22
      	divide_cube(mid[3]   , mid[4] , mid[5], mid[6] , mid[28] , mid[29] , mid[30] , mid[31], m-1); // cube 10
      	divide_cube(mid[10]  , mid[11] , mid[12], mid[13] , mid[35] , mid[36] , mid[37] , mid[38], m-1); // cube 12
      	divide_cube(mid[17]  , mid[18] , mid[19], mid[20] , mid[42] , mid[43] , mid[44] , mid[45], m-1); // cube 18
      	divide_cube(mid[24]  , mid[25] , mid[26], mid[27] , mid[49] , mid[50] , mid[51] , mid[52], m-1); // cube 16
    }

    else
        cube(a,b,d,c,e,f,h,g); /* draw tetrahedron at end of recursion */
}


void MouseButton(int button, int state, int x, int y) {
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
  if (button == GLUT_LEFT_BUTTON)
    {
      g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
      g_yClick = y - 3 * g_fViewDistance;
      xMouse = x;
      yMouse = y;
    }
}

void MouseMotion(int x, int y) {
  // If button 1 pressed, zoom in/out if mouse is moved up/down.
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
  	    case RECURSION_LEVEL_1:
  			n = 0;
  	  	break;

        case RECURSION_LEVEL_2:
  			n = 1;
  		  break;

        case RECURSION_LEVEL_3:
  			n = 2;
  		  break;

        case RECURSION_LEVEL_4:
  			n = 3;
  	    break;

  	    case MENU_EXIT:
  	    exit (0);
  	    break;

     }
      // Almost any menu selection requires a redraw
     glutPostRedisplay();
}

int BuildPopupMenu (void) 
{
	  int menu;
	  menu = glutCreateMenu (SelectFromMenu);
	  glutAddMenuEntry ("Recursion Level 1", RECURSION_LEVEL_1);
	  glutAddMenuEntry ("Recursion Level 2", RECURSION_LEVEL_2);
	  glutAddMenuEntry ("Recursion Level 3", RECURSION_LEVEL_3);
	  glutAddMenuEntry ("Recursion Level 4", RECURSION_LEVEL_4);
	  glutAddMenuEntry ("Exit", MENU_EXIT);

	  return menu;

}

void display()
{
    // Clear frame buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set up viewing transformation, looking down -Z axis
    glLoadIdentity();
 
   gluLookAt(0, 0, -g_fViewDistance, 0, 0, 0, 0, 1, 0);
   glRotatef(theta_x, 1.0,0.0,0.0);
   glRotatef(phi_y, 0.0,0.0,1.0);
   glBegin(GL_QUADS);
   divide_cube(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], n);
   
  
   glutSwapBuffers();
   glEnd();
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
 /* enter number of subdivision steps here */
	if (argc > 1) 
	{
		n=atoi(argv[1]);
	}
	else
		n = 3;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Raeeca Narimani");
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
