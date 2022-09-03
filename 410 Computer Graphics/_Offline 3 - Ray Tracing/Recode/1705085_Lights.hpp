#ifndef LIGHTS_H
#define LIGHTS_H

#include "1705085_Vector3D.hpp"
#include "1705085_Color.hpp"

class PointLight {
public:
    Vector3D pos;
    Color color;

    PointLight() {}

    PointLight(Vector3D pos, Color color) {
        this->pos = pos;
        this->color = color;
    }

    void draw() {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS); {
            glVertex3f(pos.x, pos.y,     pos.z     );
            glVertex3f(pos.x, pos.y,     pos.z + 2 );
            glVertex3f(pos.x, pos.y + 2, pos.z + 2 );
            glVertex3f(pos.x, pos.y + 2, pos.z     );
        }
        glEnd();
    }
};


class SpotLight {
public:
    Vector3D pos, dir;
    Color color;
    double cutoffAngle;

    SpotLight(Vector3D pos, Vector3D dir, Color color, double cutoffAngle) {
        this->pos = pos;
        this->dir = dir;
        this->color = color;
        this->cutoffAngle = cutoffAngle;
    }
};

#endif