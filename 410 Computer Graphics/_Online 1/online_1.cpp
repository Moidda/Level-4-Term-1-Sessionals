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

#define WHEEL_STEP_ANGLE 5

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

double wheelRadius, wheelWidth, wheelSlices;
double wheelSidewaysAngle, wheelRotateAngle;
point wheelCenter, wheelFrontVector, wheelSideVector;

double R, G, B;

double gridInclineAngle;
double gridInclineDist;

void setRGB(int r, int g, int b) {
	R = r/255.0;
	G = g/255.0;
	B = b/255.0;
}

/**
 * @brief rotate theta degrees in the forward direction and move forwards
 * 
 * @param theta 
 */
void wheelMove(double theta) {
    double r = wheelRadius;
    double s = r*theta * (pi/180.0);
    wheelCenter = wheelCenter.add( wheelFrontVector.mul(s) );
    wheelRotateAngle -= theta;
}

/**
 * @brief turn the wheel around z-axis theta degrees,
 * which basically means, turn wheelFrontVector and wheelSideVector
 * theta degrees around z-axis
 * 
 * @param theta 
 */
void wheelTurn(double theta) {
    double cosTheta = cos(theta*pi/180.0);
    double sinTheta = sin(theta*pi/180.0);
    point f = wheelFrontVector.mul(cosTheta).add( wheelSideVector.mul(sinTheta) );
    point s = wheelSideVector.mul(cosTheta).add( wheelFrontVector.mul(-sinTheta) );
    wheelFrontVector = f;
    wheelSideVector = s;
    wheelSidewaysAngle -= theta;
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
			for(int i=-10;i<=10;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes
                    
				//lines parallel to Y-axis
				glVertex3f(i*10, -100, 0);
				glVertex3f(i*10,  100, 0);

				//lines parallel to X-axis
				glVertex3f(-100, i*10, 0);
				glVertex3f( 100, i*10, 0);
			}

            for(int i = 1; i <= 30; i++) {
                //lines parallel to Y-axis
                double d = i*10;
                double z = d*tan(gridInclineAngle*pi/180.0);
				glVertex3f(100 + i*10, -100, z);
				glVertex3f(100 + i*10,  100, z);

                //lines parallel to X-axis
                if(i <= 10) {
                    d = 30*10;
                    z = d*tan(gridInclineAngle*pi/180.0);
                    glVertex3f(100        , i*10, 0);
                    glVertex3f(100 + 30*10, i*10, z);
                }
                else {
                    int j = i-10;
                    d = 30*10;
                    z = d*tan(gridInclineAngle*pi/180.0);
                    glVertex3f(100        , -j*10, 0);
                    glVertex3f(100 + 30*10, -j*10, z);
                }
            }
		}glEnd();
	}
}


/**
 * @brief draw a wheel
 * 
 * @param radius radius of the wheel 
 * @param width width of the wheel (height of the cylinder)
 */
void drawWheel(double radius, double width, int slices) {
	point points[100];
    //generate points 
    for(int i=0; i<=slices; i++) {
        points[i].x = width/2;
        points[i].y = radius*cos(((double)i/(double)slices)*2*pi);
		points[i].z = radius*sin(((double)i/(double)slices)*2*pi);
	}
	// draw quads
	for(int i = 0; i < slices; i++) {
		double intensity = 2*min(i, abs(slices - i));
		double frac = intensity/slices; 
		glColor3f(R*frac, G*frac, B*frac);
		glBegin(GL_QUADS); {
			glVertex3f( points[i].x, 		points[i].y,		points[i].z   );
			glVertex3f( points[i+1].x, 		points[i+1].y,  	points[i+1].z );
			glVertex3f( -points[i+1].x, 	points[i+1].y,  	points[i+1].z);
			glVertex3f( -points[i].x, 		points[i].y,		points[i].z  );
		}
		glEnd();
	}
	// draw diagonals
	point p1 = point(width/4, 0,  radius);
	point p2 = point(width/4, 0, -radius);
	point q1 = point(width/4,  radius, 0);
	point q2 = point(width/4, -radius, 0);
	glColor3f(R/2, G/2, B/2);
	glBegin(GL_QUADS); {
		glVertex3f( p1.x, p1.y, p1.z);
		glVertex3f( p2.x, p2.y, p2.z);
		glVertex3f(-p2.x, p2.y, p2.z);
		glVertex3f(-p1.x, p1.y, p1.z);

		glVertex3f( q1.x, q1.y, q1.z);
		glVertex3f( q2.x, q2.y, q2.z);
		glVertex3f(-q2.x, q2.y, q2.z);
		glVertex3f(-q1.x, q1.y, q1.z);
	}
	glEnd();
}

void drawWheelVectors() {
    // wheelSideVector
    glColor3f(R, G, B);
    glBegin(GL_LINES);{
        glVertex3f(
            wheelCenter.x, 
            wheelCenter.y, 
            wheelCenter.z
        );
        glVertex3f(
            wheelCenter.x + 50*wheelSideVector.x, 
            wheelCenter.y + 50*wheelSideVector.y, 
            wheelCenter.z + 50*wheelSideVector.z
        );

        glVertex3f(
            wheelCenter.x, 
            wheelCenter.y, 
            wheelCenter.z
        );
        glVertex3f(
            wheelCenter.x + 50*wheelFrontVector.x, 
            wheelCenter.y + 50*wheelFrontVector.y, 
            wheelCenter.z + 50*wheelFrontVector.z
        );
    }glEnd();
    // wheelFrontVector
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
        case 'w':
			wheelMove(WHEEL_STEP_ANGLE);
			break;
        case 's':
			wheelMove(-WHEEL_STEP_ANGLE);
			break;
		case 'a':
            wheelTurn(-WHEEL_STEP_ANGLE);
            break;
		case 'd':
            wheelTurn(WHEEL_STEP_ANGLE);
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
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	printf("pos: %.2f, %.2f, %.2f\n", pos.x, pos.y, pos.z);
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
    drawWheelVectors();

    setRGB(255, 255, 255); 
    // move the wheel up onto the grid
	glTranslatef(wheelCenter.x, wheelCenter.y, wheelCenter.z);  
    // rotate the wheel, based on moving forward/backward
    glRotatef(wheelRotateAngle, wheelSideVector.x, wheelSideVector.y, wheelSideVector.z);
	// rotate the wheel sideways, based on turning
    glRotatef(wheelSidewaysAngle, 0, 0, 1);
    drawWheel(wheelRadius, wheelWidth, wheelSlices); 
    glRotatef(-wheelSidewaysAngle, 0, 0, 1);
    glRotatef(-wheelRotateAngle, wheelSideVector.x, wheelSideVector.y, wheelSideVector.z);
    glTranslatef(wheelCenter.x, wheelCenter.y, -wheelCenter.z);

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

    // radius of the wheel
    wheelRadius = 30;
    // width of the wheel
    wheelWidth = 15;
    // no of slices in the circle that makes the wheel
    wheelSlices = 50;
    // center of the wheel
    wheelCenter = point(0, 0, wheelRadius);
    // the angle of the wheel, to be changed when the wheel is turning
    wheelSidewaysAngle = 0;
    // the angle of the wheel, to be changed when the wheel is moving forward/backward
    wheelRotateAngle = 0;
    // unit vector denoting the front-facing direction of the wheel
    wheelFrontVector = point(0, 1, 0);
    // unit vector denoting the side of the wheel. It is perpendicular to the wheelFrontVector
    // and parallel to the XY plane
    wheelSideVector = point(1, 0, 0);


    gridInclineAngle = 60;
    gridInclineDist = 50;

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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing
	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	// glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
