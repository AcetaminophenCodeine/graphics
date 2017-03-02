#include <GL/glut.h>  // includes necessary OpenGL headers


void display() {
    glShadeModel(GL_SMOOTH);  // smooth color interpolation
    glEnable(GL_DEPTH_TEST);  // enable hidden surface removal
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.9f,0.5f,0.0f,1.0f);
    glBegin(GL_TRIANGLES);  // every 3 vertexes makes a triangle

        glColor4ub(255, 102, 0, 255);    
        glVertex3f(-0.8,  0.8,  0.0); 
        glColor4ub(255, 102, 0, 255);    
        glVertex3f( -0.5,  0.8, 0.0);  
        glColor4ub(255, 102, 0, 255);  
        glVertex3f(-0.8, -0.8, 0.0);  

	glColor4ub(255, 102, 0, 255); 
        glVertex3f(-0.8, -0.8, 0.0);
	glColor4ub(255, 102, 0, 255);   
        glVertex3f(-0.50, 0.8, 0.0); 
	glColor4ub(255, 102, 0, 255);  
        glVertex3f(-0.5, -0.8, 0.0);  

	glColor4ub(255, 102, 0, 255);  
        glVertex3f(-0.50, 0.8, 0.0);
	glColor4ub(255, 102, 0, 255);  
        glVertex3f(-0.50, 0.4, 0.0); 
	glColor4ub(255, 102, 0, 255);    
        glVertex3f(0.35,-0.8, 0.0); 

	glColor4ub(255, 102, 0, 255);  
        glVertex3f(0.35, -0.8, 0.0);  
	glColor4ub(255, 102, 0, 255);  
        glVertex3f(0.35, -0.4, 0.0);  
	glColor4ub(255, 102, 0, 255);   
        glVertex3f(-0.50, 0.8, 0.0);

	glColor4ub(255, 102, 0, 255);  
        glVertex3f(0.65, -0.8, 0.0);  
        glColor4ub(255, 102, 0, 255);   
        glVertex3f(0.35, -0.8, 0.0);  
        glColor4ub(255, 102, 0, 255);    
        glVertex3f(0.35, 0.8, 0.0); 

	glColor4ub(255, 102, 0, 255);    
        glVertex3f(0.65, 0.8, 0.0);  
        glColor4ub(255, 102, 0, 255);   
        glVertex3f(0.65,-0.8, 0.0);  
        glColor4ub(255, 102, 0, 255);   
        glVertex3f(0.35, 0.8, 0.0);  

    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv) {  // request double-buffered color window with depth buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInit(&argc, argv);
    glutCreateWindow("Raeeca Narimani");
    glutDisplayFunc(display); // function to render window
    glutMainLoop();
}







