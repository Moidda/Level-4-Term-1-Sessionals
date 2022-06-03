#include <algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

using namespace std;

#define pi (2*acos(0.0))

#define XY_PLANE 0
#define XZ_PLANE 1
#define YZ_PLANE 2

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define TILT_CLOCKWISE 4
#define TILT_ANTICLOCKWISE 5

#define MOVE_STEP 2
#define LOOK_ANGLE 0.05

struct point
{
	double x,y,z;
	point(): x(0), y(0), z(0) {}
	point(double a, double b, double c): x(a), y(b), z(c) {}

	point add(point p) {
		return point(x+p.x, y+p.y, z+p.z);
	}

	point mul(double k) {
		return point(k*x, k*y, k*z);
	}
};


point pos, u, r, l;

int drawgrid;
int drawaxes;
double angle;

double R, G, B;

int bostuCubePercent, bostuTransformStep;
double bostuRadius;


void setRGB(int r, int g, int b) {
	R = r/255.0;
	G = g/255.0;
	B = b/255.0;
}


void look(int dir) {
	double cosTheta, sinTheta;
	point l_, r_, u_;
	if(dir == RIGHT) {
		cosTheta = cos(LOOK_ANGLE);
		sinTheta = sin(LOOK_ANGLE);
		l_ = l.mul(cosTheta).add( r.mul(sinTheta) );
		r_ = r.mul(cosTheta).add( l.mul(-1*sinTheta) );
		l = l_;
		r = r_;
	}
	else if(dir == LEFT) {
		cosTheta = cos(-1*LOOK_ANGLE);
		sinTheta = sin(-1*LOOK_ANGLE);
		l_ = l.mul(cosTheta).add( r.mul(sinTheta) );
		r_ = r.mul(cosTheta).add( l.mul(-1*sinTheta) );
		l = l_;
		r = r_;
	}
	else if(dir == UP) {
		cosTheta = cos(LOOK_ANGLE);
		sinTheta = sin(LOOK_ANGLE);
		l_ = l.mul(cosTheta).add( u.mul(sinTheta) );
		u_ = u.mul(cosTheta).add( l.mul(-1*sinTheta) );
		l = l_;
		u = u_;
	}
	else if(dir == DOWN) {
		cosTheta = cos(-1*LOOK_ANGLE);
		sinTheta = sin(-1*LOOK_ANGLE);
		l_ = l.mul(cosTheta).add( u.mul(sinTheta) );
		u_ = u.mul(cosTheta).add( l.mul(-1*sinTheta) );
		l = l_;
		u = u_;
	}
	else if(dir == TILT_CLOCKWISE) {
		cosTheta = cos(LOOK_ANGLE);
		sinTheta = sin(LOOK_ANGLE);
		u_ = u.mul(cosTheta).add( r.mul(sinTheta) );
		r_ = r.mul(cosTheta).add( u.mul(-1*sinTheta) );
		r = r_;
		u = u_;
	}
	else if(dir == TILT_ANTICLOCKWISE) {
		cosTheta = cos(-1*LOOK_ANGLE);
		sinTheta = sin(-1*LOOK_ANGLE);
		u_ = u.mul(cosTheta).add( r.mul(sinTheta) );
		r_ = r.mul(cosTheta).add( u.mul(-1*sinTheta) );
		r = r_;
		u = u_;
	}
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawGrid()
{
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i=-8;i<=8;i++){
				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(R, G, B);
	glBegin(GL_QUADS);{
		glVertex3f( a/2, a/2,0);
		glVertex3f( a/2,-a/2,0);
		glVertex3f(-a/2,-a/2,0);
		glVertex3f(-a/2, a/2,0);
	}glEnd();
}

void drawCube(double a) {
	glColor3f(R, G, B);
	glBegin(GL_QUADS); {
		// north surface
		glVertex3f(  a/2,  a/2,  a/2 );
		glVertex3f(  a/2, -a/2,  a/2 );
		glVertex3f( -a/2, -a/2,  a/2 );
		glVertex3f( -a/2,  a/2,  a/2 );
		// south surface 
		glVertex3f(  a/2,  a/2, -a/2 );
		glVertex3f(  a/2, -a/2, -a/2 );
		glVertex3f( -a/2, -a/2, -a/2 );
		glVertex3f( -a/2,  a/2, -a/2 );
		// east surface 
		glVertex3f(  a/2,  a/2,  a/2 );
		glVertex3f(  a/2,  a/2, -a/2 );
		glVertex3f(  a/2, -a/2, -a/2 );
		glVertex3f(  a/2, -a/2,  a/2 );
		// west surface 
		glVertex3f( -a/2,  a/2,  a/2 );
		glVertex3f( -a/2,  a/2, -a/2 );
		glVertex3f( -a/2, -a/2, -a/2 );
		glVertex3f( -a/2, -a/2,  a/2 );
		//front surface
		glVertex3f(  a/2, -a/2,  a/2 );
		glVertex3f( -a/2, -a/2,  a/2 );
		glVertex3f(  a/2, -a/2, -a/2 );
		glVertex3f( -a/2, -a/2, -a/2 );
		//behind surface
		glVertex3f(  a/2,  a/2,  a/2 );
		glVertex3f(  a/2,  a/2, -a/2 );
		glVertex3f( -a/2,  a/2, -a/2 );
		glVertex3f( -a/2,  a/2,  a/2 );
	}
	glEnd();
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[segments + 10];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        // if(i<segments/2)shade=2*(double)i/(double)segments;
        // else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(R, G, B);

        glBegin(GL_TRIANGLES); {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere1_8th(double radius, int slices, int stacks) {
	point points[stacks + 1][slices + 1];
	for(int i = 0; i <= stacks; i++) {
		double h = radius*sin(((double)i/(double)stacks)*(pi/2.0));
		double r = radius*cos(((double)i/(double)stacks)*(pi/2.0));	
		for(int j = 0; j <= slices; j++) {
			points[i][j].x=r*cos(((double)j/(double)slices)*pi/2.0);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi/2.0);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(int i=0; i < stacks;i++) {
		glColor3f(R, G, B);
		for(int j=0; j < slices; j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSphere(double radius,int slices, int stacks, bool color3d)
{
	struct point points[stacks + 10][slices + 10];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		double frac = color3d ? (double)i/stacks : 1 ;
        glColor3f(R*frac, G*frac, B*frac);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int stacks, int slices) {
	point points[stacks + 10][slices + 10];
	for(int i=0;i<=stacks;i++) {
		double h = i*height/(2.0*stacks);
		double r = radius;
		for(int j=0; j<=slices; j++) {
			points[i][j].x = r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y = r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z = h;
		}
	}
	for(int i=0; i<stacks; i++) {
        glColor3f(R, G, B);
		for(int j=0; j<slices; j++) {
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder1_4th(double radius, double height, int stacks, int slices) {
	point points[stacks + 10][slices + 10];
	for(int i=0;i<=stacks;i++) {
		double h = i*height/(2.0*stacks);
		double r = radius;
		for(int j=0; j<=slices; j++) {
			points[i][j].x = r*cos(((double)j/(double)slices)*pi/2.0);
			points[i][j].y = r*sin(((double)j/(double)slices)*pi/2.0);
			points[i][j].z = h;
		}
	}
	for(int i=0; i<stacks; i++) {
        glColor3f(R, G, B);
		for(int j=0; j<slices; j++) {
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinders(double cylinderRadius, double squareLength) {
	glTranslatef(squareLength/2, squareLength/2, 0);
	drawCylinder1_4th(cylinderRadius, squareLength, 100, 50);
	glTranslatef(-squareLength/2, -squareLength/2, 0);

	glRotatef(90, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, 0);
	drawCylinder1_4th(cylinderRadius, squareLength, 100, 50);
	glTranslatef(-squareLength/2, -squareLength/2, 0);
	glRotatef(-90, 0,0,1);

	glRotatef(180, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, 0);
	drawCylinder1_4th(cylinderRadius, squareLength, 100, 50);
	glTranslatef(-squareLength/2, -squareLength/2, 0);
	glRotatef(-180, 0,0,1);

	glRotatef(270, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, 0);
	drawCylinder1_4th(cylinderRadius, squareLength, 100, 50);
	glTranslatef(-squareLength/2, -squareLength/2, 0);
	glRotatef(-270, 0,0,1);

}

void drawSquares(double radius, double squareLength) {
	glTranslatef(-radius, 0, 0);
	glRotatef(90, 0,1,0);
	drawSquare(squareLength);
	glRotatef(-90, 0,1,0);
	glTranslatef(radius, 0, 0);

	glTranslatef(radius, 0, 0);
	glRotatef(90, 0,1,0);
	drawSquare(squareLength);
	glRotatef(-90, 0,1,0);
	glTranslatef(-radius, 0, 0);

	glTranslatef(0, -radius, 0);
	glRotatef(90, 1,0,0);
	drawSquare(squareLength);
	glRotatef(-90, 1,0,0);
	glTranslatef(0, radius, 0);

	glTranslatef(0, radius, 0);
	glRotatef(90, 1,0,0);
	drawSquare(squareLength);
	glRotatef(-90, 1,0,0);
	glTranslatef(0, -radius, 0);

	glTranslatef(0, 0, radius);
	drawSquare(squareLength);
	glTranslatef(0, 0, -radius);

	glTranslatef(0, 0, -radius);
	drawSquare(squareLength);
	glTranslatef(0, 0, radius);
}

void drawSpheres(double cylinderRadius, double squareLength) {
	// draw a single 1/8^{th} of a sphere
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	// now rotate that same above drawing by 90 degree
	glRotatef(90, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-90, 0,0,1);
	// rotate another 90 degree
	glRotatef(180, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-180, 0,0,1);
	// rotate another 90 degree
	glRotatef(270, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-270, 0,0,1);
	// repeat the above 4 1/8^{th} spheres, but this time, do it on the opposite side
	// rotate along x-axis 180 degree
	glRotatef(180, 1,0,0);
	// draw a single 1/8^{th} of a sphere
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	// now rotate that same above drawing by 90 degree
	glRotatef(90, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-90, 0,0,1);
	// rotate another 90 degree
	glRotatef(180, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-180, 0,0,1);
	// rotate another 90 degree
	glRotatef(270, 0,0,1);
	glTranslatef(squareLength/2, squareLength/2, squareLength/2);
	drawSphere1_8th(cylinderRadius, 50, 100);
	glTranslatef(-squareLength/2, -squareLength/2, -squareLength/2);
	glRotatef(-270, 0,0,1);
	glRotatef(-180, 1,0,0);
}

void drawBostu(double radius, int cubePercent) {
	double squareLength = 2*radius*cubePercent/100.0;
	double cylinderRadius = radius - squareLength/2;

	setRGB(255, 255, 255);
	drawSquares(radius, squareLength);

	setRGB(0, 255, 0);
	drawCylinders(cylinderRadius, squareLength);

	glRotatef(90, 0,1,0);
	drawCylinders(cylinderRadius, squareLength);
	glRotatef(-90, 0,1,0);

	glRotatef(90, 1,0,0);
	drawCylinders(cylinderRadius, squareLength);
	glRotatef(-90, 1,0,0);

	setRGB(255, 0, 0);
	drawSpheres(cylinderRadius, squareLength);
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
			look(LEFT);
			break;
		case '2':
			look(RIGHT);
			break;
		case '3':
			look(UP);
			break;
		case '4':
			look(DOWN);
			break;
		case '5':
			look(TILT_CLOCKWISE);
			break;
		case '6':
			look(TILT_ANTICLOCKWISE);
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			pos = pos.add(l.mul(-MOVE_STEP));
			break;
		case GLUT_KEY_UP:
			pos = pos.add(l.mul(MOVE_STEP));
			break;

		case GLUT_KEY_RIGHT:
			pos = pos.add(r.mul(MOVE_STEP));
			break;
		case GLUT_KEY_LEFT:
			pos = pos.add(r.mul(-MOVE_STEP));
			break;

		case GLUT_KEY_PAGE_UP:
			pos = pos.add(u.mul(MOVE_STEP));
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos.add(u.mul(-MOVE_STEP));
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			bostuCubePercent = max(bostuCubePercent - bostuTransformStep, 0);
			break;
		case GLUT_KEY_END:
			bostuCubePercent = min(bostuCubePercent + bostuTransformStep, 100);
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);
	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?
	gluLookAt(
		pos.x, pos.y, pos.z,
		pos.x + l.x, pos.y + l.y, pos.z + l.z, 
		u.x, u.y, u.z
	);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	drawAxes();
	drawGrid();
	
	setRGB(255, 0, 0);
	drawBostu(bostuRadius, bostuCubePercent);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid = 1;
	drawaxes = 1;
	
	pos = point(100, 100, 20);
	u = point(0, 0, 1);
	r = point(-cos(pi/4), cos(pi/4), 0);
	l = point(-cos(pi/4), -cos(pi/4), 0);

	angle=0;

	R = G = B = 1;

	bostuCubePercent = 70;
	bostuRadius = 20;
	bostuTransformStep = 2;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("Fully Controlled Camera + Sphere<->Cube");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing
	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
