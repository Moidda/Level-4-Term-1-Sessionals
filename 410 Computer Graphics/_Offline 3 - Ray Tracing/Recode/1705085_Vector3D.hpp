#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <bits/stdc++.h>

struct Vector3D
{
	double x,y,z;
	Vector3D(): x(0), y(0), z(0) {}
	Vector3D(double a, double b, double c): x(a), y(b), z(c) {}

	Vector3D operator+ (const Vector3D &p) {
		return Vector3D(x+p.x, y+p.y, z+p.z);
	}

    Vector3D add(const Vector3D &p) {
		return Vector3D(x+p.x, y+p.y, z+p.z);
	}

	Vector3D operator- (const Vector3D &p) {
		return Vector3D(x-p.x, y-p.y, z-p.z);
	}

	Vector3D operator* (double k) {
		return Vector3D(k*x, k*y, k*z);
	}

    Vector3D mul(double k) {
		return Vector3D(k*x, k*y, k*z);
	}

	double dot(const Vector3D &v) {
		return x*v.x + y*v.y + z*v.z;
	}

	Vector3D cross(const Vector3D &v) {
		Vector3D ret;
        ret.x =  y*v.z - z*v.y;
        ret.y = -x*v.z + z*v.x; 
        ret.z =  x*v.y - y*v.x;
        return ret;
	}

	Vector3D normalize() {
		double deno = sqrt(x*x + y*y + z*z);
		if(deno == 0) return Vector3D(0, 0, 0);
		return Vector3D(x/deno, y/deno, z/deno);
	}

	double getValue() {
		return sqrt(x*x + y*y + z*z);
	}

	double getDistance(const Vector3D &point) {
		return (x - point.x)*(x - point.x) + (y - point.y)*(y - point.y) + (z - point.z)*(z - point.z);
	}
};

#endif