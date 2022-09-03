#ifndef COLOR_H
#define COLOR_H

struct Color {
    double r, g, b;

    Color() {
        r = g = b = 0;
    }

    Color(double r, double g, double b) {
        if(r>1) r = 1;
        if(g>1) g = 1;
        if(b>1) b = 1;
        if(r<0) r = 0;
        if(g<0) g = 0;
        if(b<0) b = 0;
        this->r = r;
        this->g = g;
        this->b = b;
    }

    Color operator+ (Color c) {
		return Color(r+c.r, g+c.g, b+c.b);
	}

    Color operator* (Color c) {
        return Color(r*c.r, g*c.g, b*c.b);
    }

    Color operator* (double k) {
        return Color(r*k, g*k, b*k);
    }
};

#endif