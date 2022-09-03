#ifndef RAY_H
#define RAY_H

#include "1705085_Vector3D.hpp"

class Ray {
public:
    Vector3D start, dir;

    Ray(Vector3D start, Vector3D dir) {
        this->start = start;
        this->dir = dir.normalize();
    }
};

#endif 