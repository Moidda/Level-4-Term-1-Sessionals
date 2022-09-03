#ifndef OBJECTS_H
#define OBJECTS_H

#include "1705085_Vector3D.hpp"
#include "1705085_Color.hpp"
#include "1705085_Lights.hpp"

#include <windows.h>
#include <glut.h>

using namespace std;


class Object {
public:
    Vector3D referencePoint;
    double height, width, length;
    Color color;
    double coefficient[4]; 
    int shine;

    Object() {}

    virtual void draw() {}
    virtual double intersect(Ray ray)           { cout<<"wtf draw";exit(0); return -1; }
    virtual Color getColor(Vector3D point)      { return color; }
    virtual Vector3D getNormal(Vector3D point)  { cout<<"wtf getNormal";exit(0); return Vector3D(0,0,0); }

    void intersectRecursive(Ray &ray, double t_min, Color &color, int level);
    bool isInShadow(Ray ray, Vector3D point);
};


// externs from 1705085_main.cpp
extern int recursionLevel;
extern Vector3D pos, u, r, l;
extern vector<shared_ptr<Object>> objects;
extern vector<shared_ptr<PointLight>> pointLights;
extern vector<shared_ptr<SpotLight>> spotLights;


bool Object::isInShadow(Ray ray, Vector3D point) {
    shared_ptr<Object> nearestObject = 0;
    double t_min = DBL_MAX;
    for(auto ob: objects) {
        double t = ob->intersect(ray);
        if(t>=0 and t<t_min) {
            t_min = t;
            nearestObject = ob;
        }
    }
    Vector3D ip = ray.start + ray.dir*t_min;
    // the following return results in dotted shadow effect
    return abs(ip.x-point.x)>EPS && abs(ip.y-point.y)>EPS && abs(ip.z-point.z)>EPS;
    // the follwoing return results in smoother image
    // return !(nearestObject.get() == this);
}


void Object::intersectRecursive(Ray &ray, double t_min, Color &color, int level) {
    Vector3D ip = ray.start + ray.dir*t_min;
    Color ipColor = getColor(ip);
    color = ipColor*coefficient[AMB];
    Vector3D normalAtip = getNormal(ip);
    normalAtip = normalAtip.normalize();

    for(auto pl: pointLights) {
        Ray rayL = Ray(pl->pos, ip - pl->pos);
        Vector3D reflectedDir = rayL.dir - normalAtip*(2.0*rayL.dir.dot(normalAtip));
        Ray rayR = Ray(ip, reflectedDir);
        if(isInShadow(rayL, ip)) continue;
        double lambert = rayL.dir.dot(normalAtip);
        double phong = rayR.dir.dot(ray.dir);
        color = color + ipColor * pl->color * coefficient[DIFF] * lambert;
        color = color + ipColor * pl->color * coefficient[SPEC] * pow(phong,shine);
    }

    for(auto sl: spotLights) {
        Ray rayL = Ray(sl->pos, ip - sl->pos);
        // check if angle betn rayL and sl->dir is smaller than cutoffangle
        // A*B = |A||B|cos(theta)
        // cos(theta) = (A*B)/(|A||B|)
        double costheta = rayL.dir.dot(sl->dir) / (rayL.dir.getValue()*sl->dir.getValue());
        costheta *= (PI/180.0);
        double theta = acos(costheta);
        if(theta > sl->cutoffAngle) continue;

        Vector3D reflectedDir = rayL.dir - normalAtip*(2.0*rayL.dir.dot(normalAtip));
        Ray rayR = Ray(ip, reflectedDir);
        if(isInShadow(rayL, ip)) continue;
        double lambert = rayL.dir.dot(normalAtip);
        double phong = rayR.dir.dot(ray.dir);
        color = color + ipColor * sl->color * coefficient[DIFF] * lambert;
        color = color + ipColor * sl->color * coefficient[SPEC] * pow(phong,shine);
    }

    /*******************Recursive Reflection***************************/
    if(level >= recursionLevel) return;
    
    Vector3D reflectionDir = ray.dir - normalAtip*(2.0*ray.dir.dot(normalAtip)) ;
    reflectionDir = reflectionDir.normalize();
    Ray reflectedRay(ip+reflectionDir*0.005, reflectionDir);
    t_min = DBL_MAX;
    shared_ptr<Object> nearestObject = 0;
    for(auto ob: objects) {
        double t = ob->intersect(reflectedRay);
        if(t>=0 && t<t_min) {
            t_min = t;
            nearestObject = ob;
        }
    }
    if(nearestObject) {
        Color reflectedColor;
        nearestObject->intersectRecursive(reflectedRay, t_min, reflectedColor, level+1);
        color = color + reflectedColor*coefficient[REFL];
    }
}


class Sphere : public Object {
public:
    Sphere(Vector3D center, double radius) {
        referencePoint = center;
        height = width = length = radius;
    }

    void draw() override {
        Vector3D center = referencePoint;
        glTranslated(center.x, center.y, center.z);

        int stacks = 50;
        int slices = 100;
        Vector3D points[stacks + 10][slices + 10];
        //generate points
        for(int i=0; i<=stacks; i++) {
            double h = length * sin(((double)i/(double)stacks)*(PI/2));
            double r = length * cos(((double)i/(double)stacks)*(PI/2));
            for(int j=0; j<=slices; j++) {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*PI);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*PI);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(int i=0; i<stacks; i++) {
            for(int j=0; j<slices; j++) {
                glColor3f(color.r, color.g, color.b);
                glBegin(GL_QUADS); {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,     points[i][j].y,     points[i][j].z    );
                    glVertex3f(points[i][j+1].x,   points[i][j+1].y,   points[i][j+1].z  );
                    glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,   points[i+1][j].y,   points[i+1][j].z  );
                    //lower hemisphere
                    glVertex3f(points[i][j].x,     points[i][j].y,     -points[i][j].z    );
                    glVertex3f(points[i][j+1].x,   points[i][j+1].y,   -points[i][j+1].z  );
                    glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,   points[i+1][j].y,   -points[i+1][j].z  );
                }
                glEnd();
            }
        }
        // re-translate back
        glTranslated(-center.x, -center.y, -center.z);
    }

    double intersect(Ray ray) override {
        ray.start = ray.start - referencePoint;
        double a = ray.dir.dot(ray.dir);
        double b = 2*ray.dir.dot(ray.start);
        double c = ray.start.dot(ray.start) - length*length;
        double d = b*b - 4*a*c;
        if(d < 0) return -1;        
        d = sqrt(d);
        double t_neg = (-b - d) / 2;
        double t_pos = (-b + d) / 2;
        if(t_neg>0 and t_pos>0) return t_neg;       
        if(abs(t_neg-t_pos) < EPS) return t_neg;    
        if(t_neg<0 and 0<t_pos) return t_pos;       
        if(t_pos<0 and 0<t_neg) return t_neg;       
        return -1; 
    }

    Vector3D getNormal(Vector3D point) override {
        Vector3D normal = point - referencePoint;
        return normal.normalize();
    }
};


class Triangle : public Object {
public:
    Vector3D points[3];

    Triangle(Vector3D p0, Vector3D p1, Vector3D p2) {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
    }

    void draw() override {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES); {
            glVertex3f(points[0].x, points[0].y, points[0].z);
            glVertex3f(points[1].x, points[1].y, points[1].z);
            glVertex3f(points[2].x, points[2].y, points[2].z);
        }
        glEnd();
    }

    double intersect(Ray ray) override {
        Vector3D edge01 = points[1] - points[0];
        Vector3D edge02 = points[2] - points[0];
        Vector3D h = ray.dir.cross(edge02);
        double a = edge01.dot(h);
        if (a > -EPS && a < EPS) return -1;    
        double f = 1.0/a;
        Vector3D s = ray.start - points[0];
        double u = f*s.dot(h);
        if (u < 0.0 || u > 1.0) return -1;
        Vector3D q = s.cross(edge01);
        double v = f*ray.dir.dot(q);
        if (v < 0.0 || u + v > 1.0) return -1;
        double t = f*edge02.dot(q);
        if (t > EPS) return t;
        else return -1;
    }

    Vector3D getNormal(Vector3D point) override {
        Vector3D edge01 = points[1] - points[0];
        Vector3D edge02 = points[2] - points[0];
        Vector3D normal = edge01.cross(edge02);
        if(l.dot(normal) > 0) normal = normal*(-1);
        return normal.normalize();
    }
};


class Floor : public Object {
public:
    Floor(double floorWidth, double tileWidth) {
        referencePoint = Vector3D(-floorWidth/2, -floorWidth/2, 0);
        height = 0;
        length = width = tileWidth;
    }

    void draw() override {
        Vector3D bottomLeftPoint = referencePoint;
        double floorWidth = -2*referencePoint.x;
        bool rowToggle = false;
        for(double y = bottomLeftPoint.y; y < floorWidth/2; y += length) {
            bool colToggle = rowToggle; 
            for(double x = bottomLeftPoint.x; x < floorWidth/2; x += length) {
                if(colToggle)   glColor3f(0, 0, 0);
                else            glColor3f(1, 1, 1);
                glBegin(GL_QUADS); {
                    glVertex3f(        x         ,        y          ,     0);
                    glVertex3f(     x + length   ,        y          ,     0);
                    glVertex3f(     x + length   ,    y + length     ,     0);
                    glVertex3f(        x         ,    y + length     ,     0);
                }
                glEnd();
                colToggle ^= 1;
            }
            rowToggle ^= 1;
        }
    }

    double intersect(Ray ray) override {
        Vector3D n = Vector3D(0, 0, 1);
        double deno = n.dot(ray.dir);
        if(abs(deno) < EPS) return -1;
        double t = -n.dot(ray.start) / n.dot(ray.dir);
        return t;
    }

    Color getColor(Vector3D point) override {
        Vector3D bottomLeftPoint = referencePoint;
        double floorWidth = -2*referencePoint.x;
        bool rowToggle = false;
        for(double y = bottomLeftPoint.y; y < floorWidth/2; y += length) {
            bool colToggle = rowToggle; 
            for(double x = bottomLeftPoint.x; x < floorWidth/2; x += length) {
                bool isBlack = false;
                if(colToggle) isBlack = true;
                if(x <= point.x && point.x < x+length && y <= point.y && point.y < y+length) 
                    return isBlack ? Color(0,0,0) : Color(1,1,1);

                colToggle ^= 1;
            }
            rowToggle ^= 1;
        }
        return Color(0, 0, 0);
    }

    Vector3D getNormal(Vector3D point) override {
        return Vector3D(0,0,1);
    }
};


class GeneralQuadratic : public Object {
public:
    double A, B, C, D, E, F, G, H, I, J;

    GeneralQuadratic(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J) {
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;
    }

    double intersect(Ray ray) {
        double a, b, c;
        a =  A*ray.dir.x*ray.dir.x + B*ray.dir.y*ray.dir.y + C*ray.dir.z*ray.dir.z;
        a += D*ray.dir.x*ray.dir.y + E*ray.dir.x*ray.dir.z + F*ray.dir.y*ray.dir.z;

        b = 2.0*A*ray.start.x*ray.dir.x + 2.0*B*ray.start.y*ray.dir.y + 2.0*C*ray.start.z*ray.dir.z;
        b += D*(ray.start.x*ray.dir.y + ray.dir.x*ray.start.y);
        b += E*(ray.start.x*ray.dir.z + ray.dir.x*ray.start.z);
        b += F*(ray.start.y*ray.dir.z + ray.dir.y*ray.start.z);
        b += G*ray.dir.x + H*ray.dir.y + I*ray.dir.z;

        c = A*ray.start.x*ray.start.x + B*ray.start.y*ray.start.y + C*ray.start.z*ray.start.z;
        c += D*ray.start.x*ray.start.y + E*ray.start.x*ray.start.z + F*ray.start.y*ray.start.z;
        c += G*ray.start.x + H*ray.start.y + I*ray.start.z + J;
    
        double t_min, t_max;
        if(abs(a) <= EPS) {
            t_min = (abs(b) <= EPS)? DBL_MAX : (-c/b);
            t_max = DBL_MAX;
        } 
        else {
            double discriminant = b*b-4.0*a*c;
            if(discriminant < 0.0) {
                t_min = t_max = DBL_MAX;
            } 
            else if(discriminant > 0.0) {
                t_max = -b/(2.0*a)+sqrt(discriminant)/(2.0*a);
                t_min = -b/(2.0*a)-sqrt(discriminant)/(2.0*a);
            } 
            else {
                t_min = -b/(2.0*a);
                t_max = DBL_MAX;
            }
        }

        return -1;  
    }
};

#endif