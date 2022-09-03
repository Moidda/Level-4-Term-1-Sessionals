#include <bits/stdc++.h>

#include <windows.h>
#include <glut.h>

using namespace std;

#include "1705085_defines.hpp"
#include "1705085_Vector3D.hpp"
#include "1705085_Ray.hpp"
#include "1705085_Objects.hpp"
#include "1705085_Lights.hpp"
#include "bitmap_image.hpp"



// camera
Vector3D pos, u, r, l;
// reflection
int recursionLevel;
// window
int windowWidth = 600, windowHeight = 600;
double viewAngle = 80;
// image
int imageWidth, imageHeight;
int imageCount;
// floor
double floorWidth, tileWidth;
// count of things
int objectCount;
int pointLightCount;
int spotLightCount;
// vectors
vector<shared_ptr<Object>> objects;
vector<shared_ptr<PointLight>> pointLights;
vector<shared_ptr<SpotLight>> spotLights;


void capture() {
    bitmap_image image(imageWidth, imageHeight);
	for(int i = 0; i < imageHeight; i++) 
		for(int j = 0; j < imageWidth; j++) 
			image.set_pixel(i, j, 0, 0, 0);

	double planeDistance = (windowHeight/2.0) / tan((viewAngle/2.0) * (PI/180.0));
	Vector3D topLeft = pos + l*planeDistance - r*(windowWidth*0.5) + u*(windowHeight*0.5);
	double du = (double)windowWidth/imageWidth;
	double dv = (double)windowHeight/imageHeight;
	topLeft = topLeft + r*(0.5*du) - u*(0.5*dv);

	for(int i = 0; i < imageWidth; i++) {
		for(int j = 0; j < imageHeight; j++) {
			Vector3D worldCoord = topLeft + r*(i*du) - u*(j*dv);
			Ray ray(pos, worldCoord - pos);
			double t_min = DBL_MAX;
			shared_ptr<Object> nearestObject = 0;
			for(auto ob: objects) {
				double t = ob->intersect(ray);
				if(t>=0 and t<t_min) {
					t_min = t;
					nearestObject = ob;
				}
			}
			if(nearestObject) {
				Color color;
				nearestObject->intersectRecursive(ray, t_min, color, 1);
				image.set_pixel(i, j, color.r*255, color.g*255, color.b*255);
			}	
		}
	}

	string imageName = "output_" + to_string(imageCount++) + ".bmp";
	image.save_image(imageName);
	cout << imageName << "\n";
}


void clearMemory() {
    
	glutDestroyWindow(glutGetWindow());
}


void look(int dir) {
	double cosTheta, sinTheta;
	Vector3D l_, r_, u_;
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


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':
			capture();
			break;
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
		case 'd':
            clearMemory();
            break;
        case 'p':
            cout << "pos: " << pos.x << "," << pos.y << "," << pos.z << endl;
            cout << "u: " << u.x << "," << u.y << "," << u.z << endl;
            cout << "r: " << r.x << "," << r.y << "," << r.z << endl;
            cout << "l: " << l.x << "," << l.y << "," << l.z << endl;
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
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				
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
    for(auto ob: objects) ob->draw();
	for(auto pl: pointLights) pl->draw();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void loadData() {
	ifstream scene;
	scene.open("scene.txt");
    scene >> recursionLevel;
    scene >> imageWidth;
    imageHeight = imageWidth;
    scene >> objectCount;
    for(int i = 0; i < objectCount; i++) {
        shared_ptr<Object> ob;
        string objectType;
        scene >> objectType;
        if(objectType == "sphere") {
            double centerX, centerY, centerZ;
            double radius;
            scene >> centerX >> centerY >> centerZ;
            scene >> radius;
            ob = make_shared<Sphere>(Vector3D(centerX, centerY, centerZ), radius);
        }
        else if(objectType == "triangle") {
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            scene >> x1 >> y1 >> z1;
            scene >> x2 >> y2 >> z2;
            scene >> x3 >> y3 >> z3;
            ob = make_shared<Triangle>(Vector3D(x1,y1,z1), Vector3D(x2,y2,z2), Vector3D(x3,y3,z3));
        }
        else if(objectType == "general") {
            double A, B, C, D, E, F, G, H, I, J;
            double referenceX, referenceY, referenceZ;
            double length, width, height;
            scene >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            scene >> referenceX >> referenceY >> referenceZ;
            scene >> length >> width >> height;
			shared_ptr<GeneralQuadratic> gen = make_shared<GeneralQuadratic>(A,B,C,D,E,F,G,H,I,J);
			gen->referencePoint = Vector3D(referenceX, referenceY, referenceZ);
			gen->length = length;
			gen->width = width;
			gen->height = height;
        }
        double r, g, b;
        double ambient, diffuse, specular, reflection;
        int shine;
        scene >> r >> g >> b;
        scene >> ambient >> diffuse >> specular >> reflection;
        scene >> shine;
        ob->color = Color(r, g, b);
        ob->coefficient[AMB] = ambient;
        ob->coefficient[DIFF] = diffuse;
        ob->coefficient[SPEC] = specular;
        ob->coefficient[REFL] = reflection;
        ob->shine = shine;
        objects.push_back(ob);
    }
    scene >> pointLightCount;
    for(int i = 0; i < pointLightCount; i++) {
        double x, y, z;
        double r, g, b;
        scene >> x >> y >> z;
        scene >> r >> g >> b;
		shared_ptr<PointLight> light = make_shared<PointLight>(Vector3D(x,y,z), Color(r,g,b));
		pointLights.push_back(light);
	} 
    scene >> spotLightCount;
    for(int i = 0; i < spotLightCount; i++) {
        double x, y, z;
        double r, g, b;
        double dirX, dirY, dirZ;
        double cutoffAngle;
        scene >> x >> y >> z;
        scene >> r >> g >> b;
        scene >> dirX >> dirY >> dirZ;
        scene >> cutoffAngle;
		shared_ptr<SpotLight> light = make_shared<SpotLight>(Vector3D(x,y,z), Vector3D(dirX,dirY,dirZ), Color(r,g,b), cutoffAngle);
		spotLights.push_back(light);
	}
    cout << "input taken" << endl;
}


void animate(){
	
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){

	// camera/eye
	// pos = Vector3D(0, 20, 0);
    // u = Vector3D(0, 0, 1);
    // r = Vector3D(-cos(PI/4), cos(PI/4), 0);
	// l = Vector3D(-cos(PI/4), -cos(PI/4), 0);

	pos = Vector3D(-6.83486,-111.764,50);
	u = Vector3D(0, 0, 1);
    r = Vector3D(0.931235,0.364419,0);
    l = Vector3D(-0.364419,0.931235,0);

	// perspective
	viewAngle = 80;

    floorWidth = 1000;
    tileWidth = 20;
    shared_ptr<Object> flor = make_shared<Floor>(floorWidth, tileWidth);
	flor->coefficient[0] = 0.3;
	flor->coefficient[1] = 1;
	flor->coefficient[2] = 1;
	flor->coefficient[3] = 1;
	flor->shine = 5;
    objects.push_back(flor);

	// load other objects and lights
	loadData();


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
	glutInitWindowSize(windowWidth, windowHeight);
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
