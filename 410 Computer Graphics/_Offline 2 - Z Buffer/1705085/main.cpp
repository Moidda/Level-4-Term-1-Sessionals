#include <bits/stdc++.h>
#include "../bmp_generation_codes/bitmap_image.hpp"
using namespace std;



#define PI      acos(-1.0)
#define MATRIX  vector<vector<double>>
#define INF     100000
#define EPS     1e-9


const int testcase = 4;


bool equals(double x, double y) {
    return abs(x-y) < EPS;
}


double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


struct Point {
    double x, y, z, w;

    Point(): x(0), y(0), z(0), w(1) {}
    Point(double a, double b, double c): x(a), y(b), z(c), w(1) {}

    Point normalize() {
        double deno = sqrt(x*x + y*y + z*z);
        if(deno == 0) return Point(0, 0, 0);
        return Point(x/deno, y/deno, z/deno);
    }

    double dot(Point p) {
        return x*p.x + y*p.y + z*p.z;
    }

    Point add(Point p) {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    Point mul(double k) {
        return Point(k*x, k*y, k*z);
    }

    Point cross(Point p) {
        Point ret;
        ret.x = y*p.z - z*p.y;
        ret.y = -x*p.z + z*p.x; 
        ret.z = x*p.y - y*p.x;
        return ret;
    }

    Point transformPoint(MATRIX m) {
        Point p;
        p.x = x*m[0][0] + y*m[0][1] + z*m[0][2] + w*m[0][3];
        p.y = x*m[1][0] + y*m[1][1] + z*m[1][2] + w*m[1][3];
        p.z = x*m[2][0] + y*m[2][1] + z*m[2][2] + w*m[2][3];
        p.w = x*m[3][0] + y*m[3][1] + z*m[3][2] + w*m[3][3];

        if(p.w > 1) {
            p.x /= p.w;
            p.y /= p.w;
            p.z /= p.w;
            p.w /= p.w;
        }

        return p;
    }

    string toString() {
        string ret, temp;
        stringstream ss; 
        ss << fixed << setprecision(7) << x;
        ss >> temp;
        ret += temp + " ";
        ss.clear();
        ss << fixed << setprecision(7) << y;
        ss >> temp;
        ret += temp + " ";
        ss.clear();
        ss << fixed << setprecision(7) << z;
        ss >> temp;
        ret += temp;
        return ret;
    }
};


struct Color {
    int r, g, b;
    Color() { r = g = b = 0; }
    Color(int r, int g, int b) { this->r = r; this->g = g; this->b = b; }
    Color getRandom() {
        Color c;
        c.r = rand()%256; 
        c.g = rand()%256; 
        c.b = rand()%256;
        return c;
    }
};


struct Triangle {
    Point p[3];
    Color color;

    Triangle() { color = color.getRandom(); }
    
    Triangle(Point x, Point y, Point z) {
        p[0] = x; p[1] = y; p[2] = z;
        color = color.getRandom();
    }

    vector<Point> getEdgeIntersection(double ys) {
        int invalidCount = 0;
        // 0-1
        double x01 =  (p[0].x - p[1].x)*(ys - p[0].y)/(p[0].y - p[1].y) + p[0].x;
        if(!equals(dist(p[0].x, p[0].y, x01, ys) + dist(x01, ys, p[1].x, p[1].y), dist(p[0].x, p[0].y, p[1].x, p[1].y))) {
            x01 = INF;
            invalidCount++;
        }
        // 1-2
        double x12 =  (p[1].x - p[2].x)*(ys - p[1].y)/(p[1].y - p[2].y) + p[1].x;
        if(!equals(dist(p[1].x, p[1].y, x12, ys) + dist(x12, ys, p[2].x, p[2].y), dist(p[1].x, p[1].y, p[2].x, p[2].y))) {
            x12 = INF;
            invalidCount++;
        }
        // 2-0
        double x20 =  (p[2].x - p[0].x)*(ys - p[2].y)/(p[2].y - p[0].y) + p[2].x;
        if(!equals(dist(p[2].x, p[2].y, x20, ys) + dist(x20, ys, p[0].x, p[0].y), dist(p[2].x, p[2].y, p[0].x, p[0].y))) {
            x20 = INF;
            invalidCount++;
        }

        if(invalidCount != 1) return vector<Point>{};
        // assert(invalidCount == 1);

        if(equals(x01, INF)) {
            // return vector<Point> {p[2], p[0], p[1]};
            if(p[0].x < p[1].x) return vector<Point> {p[2], p[0], p[1]};
            else                return vector<Point> {p[2], p[1], p[0]};
        }
        else if(equals(x12, INF)) {
            // return vector<Point> {p[0], p[1], p[2]};
            if(p[1].x < p[2].x) return vector<Point> {p[0], p[1], p[2]};
            else                return vector<Point> {p[0], p[2], p[1]};
        }
        else {
            // return vector<Point> {p[1], p[2], p[0]};
            if(p[2].x < p[0].x) return vector<Point> {p[1], p[2], p[0]};
            else                return vector<Point> {p[1], p[0], p[2]};
        }
    }

    bool isPointOnLine(Point p, int p1, int p2) {
        return equals(
            dist(this->p[p1].x, this->p[p1].y, p.x, p.y) + dist(p.x, p.y, this->p[p2].x, this->p[p2].y),
            dist(this->p[p1].x, this->p[p1].y, this->p[p2].x, this->p[p2].y)
        );
    }

    string toString() {
        return  p[0].toString() + "\n" + p[1].toString() + "\n" + p[2].toString();
    }
};


Point Rodrigues(Point x, Point a, double theta) {
    theta = theta*PI/180.0;     // theta is in degree, convert to radian
    double aDotx = a.dot(x);
    Point aCrossx = a.cross(x);
    
    return Point
    (
        cos(theta)*x.x + (1-cos(theta))*aDotx*a.x + sin(theta)*aCrossx.x,
        cos(theta)*x.y + (1-cos(theta))*aDotx*a.y + sin(theta)*aCrossx.y,
        cos(theta)*x.z + (1-cos(theta))*aDotx*a.z + sin(theta)*aCrossx.z
    );
}


MATRIX getIdentityMatrix() {
    MATRIX ret
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    return ret;
}

MATRIX getTranslateMatrix(Point t) {
    MATRIX ret
    {
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, t.z},
        {0, 0, 0, 1  }
    };
    return ret;
}

MATRIX getScaleMatrix(Point s) {
    MATRIX ret
    {
        {s.x, 0,   0,   0},
        {0,   s.y, 0,   0},
        {0,   0,   s.z, 0},
        {0,   0,   0,   1}
    };
    return ret;
}

MATRIX getRotateMatrix(double angle, Point a) {
    a = a.normalize();
    Point c1 = Rodrigues(Point(1, 0, 0), a, angle);
    Point c2 = Rodrigues(Point(0, 1, 0), a, angle);
    Point c3 = Rodrigues(Point(0, 0, 1), a, angle);
    MATRIX ret
    {
        {c1.x, c2.x, c3.x, 0},
        {c1.y, c2.y, c3.y, 0},
        {c1.z, c2.z, c3.z, 0},
        {0,    0,    0,    1}
    };
    return ret;
}

MATRIX addMatrix(MATRIX m1, MATRIX m2) {
    MATRIX ret(4);
    for(int i = 0; i < m1.size(); i++) 
        for(int j = 0; j < m1.size(); j++) 
            ret[i].push_back( m1[i][j] + m2[i][j] );

    return ret;
}

MATRIX multMatrix(MATRIX m1, MATRIX m2) {
    MATRIX ret(4);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            double x = 0;
            for(int k = 0; k < 4; k++) 
                x += m1[i][k]*m2[k][j];
        
            ret[i].push_back(x);
        }
    }
    return ret;
}


string matrixToString(MATRIX m) {
    string ret;
    stringstream ss;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) { 
            string temp;
            stringstream ss;
            ss << fixed << setprecision(7) << m[i][j];
            ss >> temp;
            ret += temp + " ";
        }
        ret += "\n";
    }
    return ret;
}

void printMatrix(MATRIX m) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            cout << m[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


MATRIX calcViewTransformation(Point look, Point eye, Point up) {
    Point l = look.add(eye.mul(-1));
    l = l.normalize();
    Point r = l.cross(up);
    r = r.normalize();
    Point u = r.cross(l);

    MATRIX T = getTranslateMatrix(eye.mul(-1));
    MATRIX R {
        { r.x,  r.y,  r.z,   0},
        { u.x,  u.y,  u.z,   0},
        {-l.x, -l.y, -l.z,   0},
        {   0,    0,    0,   1},
    };

    return multMatrix(R, T);
}


MATRIX calcProjectionTransformation(double fovY, double aspectRatio, double near, double far) {
    fovY = fovY*PI/180.0;
    double fovX = fovY*aspectRatio;
    double t = near*tan(fovY/(2));
    double r = near*tan(fovX/(2));
    MATRIX P {
        {near/r,   0,             0,                       0              },
        {  0,    near/t,          0,                       0              },
        {  0,      0,    -(far+near)/(far-near), -(2*far*near)/(far-near) },
        {  0,      0,            -1,                       0              }  
    };
    return P;
}


int main() {

    srand(time(0));

    ofstream stage1, stage2, stage3;
    ifstream sceneFile;

    stage1.open("stage1.txt");
    stage2.open("stage2.txt");
    stage3.open("stage3.txt");
    sceneFile.open("../Test Cases/"+ to_string(testcase) + "/scene.txt");

    Point eye, look, up;
    double FOVY, aspectRatio, near, far;
    
    sceneFile >> eye.x >> eye.y >> eye.z;
    sceneFile >> look.x >> look.y >> look.z;
    sceneFile >> up.x >> up.y >> up.z;
    sceneFile >> FOVY >> aspectRatio >> near >> far;

    MATRIX V = calcViewTransformation(look, eye, up);
    MATRIX P = calcProjectionTransformation(FOVY, aspectRatio, near, far);

    string command;
    stack<MATRIX> S;
    S.push(getIdentityMatrix());

    while(sceneFile >> command) {
        if(command == "triangle") {
            Triangle tri;
            for(int i = 0; i < 3; i++) sceneFile >> tri.p[i].x >> tri.p[i].y >> tri.p[i].z;

            MATRIX currentState = S.top();
            for(int i = 0; i < 3; i++) tri.p[i] = tri.p[i].transformPoint(currentState);
            stage1 << tri.toString() << endl << endl;

            for(int i = 0; i < 3; i++) tri.p[i] = tri.p[i].transformPoint(V);
            stage2 << tri.toString() << endl << endl;

            for(int i = 0; i < 3; i++) tri.p[i] = tri.p[i].transformPoint(P);
            stage3 << tri.toString() << endl << endl;
        }
        else if(command == "translate") {
            MATRIX currentState = S.top();
            S.pop();
            Point t;
            sceneFile >> t.x >> t.y >> t.z;
            currentState = multMatrix(currentState, getTranslateMatrix(t));           
            S.push(currentState);
        }
        else if(command == "scale") {
            MATRIX currentState = S.top();
            S.pop();
            Point s;
            sceneFile >> s.x >> s.y >> s.z;
            currentState = multMatrix(currentState, getScaleMatrix(s));
            S.push(currentState);
        }
        else if(command == "rotate") {
            MATRIX currentState = S.top();
            S.pop();
            double angle;
            Point a;
            sceneFile >> angle >> a.x >> a.y >> a.z;
            currentState = multMatrix(currentState, getRotateMatrix(angle, a));
            S.push(currentState);
        }
        else if(command == "push") {
            S.push(S.top());
        }
        else if(command == "pop") {
            S.pop();
        }
        else if(command == "end") {
            break;
        }
        else {
            stage1 << "Invalid command: " << command << endl;
        }
    }

    stage1.close();
    stage2.close();
    stage3.close();
    sceneFile.close();

/************************** z buffer calculations **************************/ 
    ifstream configFile;
    ofstream bufferFile;
    int screenWidth, screenHeight;
    pair<double, double> limitX, limitY, limitZ;

    configFile.open("../Test Cases/4/config1.txt");
    configFile >> screenWidth >> screenHeight;   
    configFile >> limitX.first;
    configFile >> limitY.first;
    configFile >> limitZ.first >> limitZ.second;

    limitX.second = -1*limitX.first;
    limitY.second = -1*limitY.first;

    ifstream stage3In;
    stage3In.open("stage3.txt");   

    vector<Triangle> triangles;
    while(stage3In) {
        Triangle t;
        for(int i = 0; i < 3; i++) stage3In >> t.p[i].x >> t.p[i].y >> t.p[i].z;
        triangles.push_back(t);
    }
    triangles.pop_back();

    double dx = (limitX.second - limitX.first) / screenWidth;
    double dy = (limitY.second - limitY.first) / screenHeight; 
    double topY    = limitY.second - dy/2;      // top scan line
    double bottomY = limitY.first  + dy/2;      // bottom scan line
    double leftX   = limitX.first  + dx/2;      // left starting point for each scan line
    double rightX  = limitX.second - dx/2;      // right end point for each scan line

    // initializing zBuffer with max limit along z-axis
    MATRIX zBuffer(screenHeight, vector<double>(screenWidth, limitZ.second));
    vector<vector<Color>> colorBuffer(screenHeight, vector<Color>(screenWidth));

    for(Triangle t: triangles) {
        double maxY = -INF, minY = INF;
        for(Point p: t.p) maxY = max(maxY, p.y);
        for(Point p: t.p) minY = min(minY, p.y);
        maxY = min(maxY, limitY.second);            // top scan line for this triangle
        minY = max(minY, limitY.first );            // bottom scan line for this triangle
        int startRow = abs(maxY - limitY.second)/dy;
        int endRow   = abs(minY - limitY.second)/dy;
        startRow     = max(startRow, 0);
        endRow       = min(endRow, screenHeight-1);

        cout << "startRow: " << startRow << " endRow: " << endRow << endl;
        cout << endl;
        for(int i = startRow; i <= endRow; i++) {
            double ys = topY- i*dy;                         // current scan line
            vector<Point> p = t.getEdgeIntersection(ys);    // scan line intersects with 0-1, 0-2 segment
            if(p.size() != 3) continue;
            
            double za = (p[1].z - p[0].z)*(ys - p[0].y)/(p[1].y - p[0].y) + p[0].z;
            double zb = (p[2].z - p[0].z)*(ys - p[0].y)/(p[2].y - p[0].y) + p[0].z;
            double xa = (p[1].x - p[0].x)*(ys - p[0].y)/(p[1].y - p[0].y) + p[0].x;
            double xb = (p[2].x - p[0].x)*(ys - p[0].y)/(p[2].y - p[0].y) + p[0].x;
            
            double zp = za;
            double C = (zb-za)/(xb-xa);

            xa = max(xa, limitX.first);
            xb = min(xb, limitX.second);
            int startCol = abs(xa - limitX.first)/dx;
            int endCol   = abs(xb - limitX.first)/dx;
            startCol     = max(startCol, 0);
            endCol       = min(endCol, screenWidth-1);

            for(int j = startCol; j <= endCol; j++) {
                double x = leftX + j*dx;
                double delX = x - xa;
                zp += delX * C;
                if(limitZ.first <= zp and zp < zBuffer[i][j]) {
                    zBuffer[i][j] = zp;    
                    colorBuffer[i][j] = t.color;
                }
            }
        }
    }

    bufferFile.open("z_buffer.txt", ios::out | ios::trunc);
    for(int i = 0; i < screenHeight; i++) {
        for(int j = 0; j < screenWidth; j++) {
            if(equals(zBuffer[i][j], limitZ.second)) bufferFile << "INF     ";
            else bufferFile << fixed << setprecision(6) << zBuffer[i][j];
            bufferFile << "    ";
        }
        bufferFile << "\n";
    }


    cout << screenWidth << ", " << screenHeight << endl;
    bitmap_image image(screenWidth, screenHeight);
    for(int j = 0; j < screenWidth; j++) 
        for(int i = 0; i < screenHeight; i++) 
            image.set_pixel(j, i, colorBuffer[i][j].r, colorBuffer[i][j].g, colorBuffer[i][j].b);

    image.save_image("output.bmp");

    stage3In.close();
    bufferFile.close();
    configFile.close();

    return 0;
}