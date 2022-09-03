#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <bits/stdc++.h>

#define Pi acos(-1.0)

using namespace std;

struct matrix
{
    double mat[4][4];

    matrix(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                mat[i][j] = 0;
            }
        }
    }

    matrix makeIdentityMatrix(int dim){
        matrix identity;
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(i == j){
                    identity.mat[i][j] = 1;
                }
                else
                    identity.mat[i][j] = 0;
            }
        }
        return identity;
    }

    matrix makeTranslateMatrix(double tx,double ty,double tz){
        matrix trans;
        trans = makeIdentityMatrix(4);
        trans.mat[0][3] = tx;
        trans.mat[1][3] = ty;
        trans.mat[2][3] = tz;

        return trans;
    }

    matrix makeScaleMatrix(double sx,double sy,double sz){
        matrix scale;
        scale = makeIdentityMatrix(4);
        scale.mat[0][0] = sx;
        scale.mat[1][1] = sy;
        scale.mat[2][2] = sz;

        return scale;
    }



    matrix multiply(matrix mat1,matrix mat2){
        matrix mult;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    mult.mat[i][j] += mat1.mat[i][k] * mat2.mat[k][j];
                }
            }
        }
        return mult;
    }

};


struct point
{
	double x,y,z,w;

    point(){
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }

    point(double x_,double y_,double z_){
        x = x_;
        y = y_;
        z = z_;
        w = 1;
    }

    void transformPoint(matrix trans){
        point p;
        p.x = trans.mat[0][0]*x + trans.mat[0][1]*y + trans.mat[0][2]*z + trans.mat[0][3]*w;
        p.y = trans.mat[1][0]*x + trans.mat[1][1]*y + trans.mat[1][2]*z + trans.mat[1][3]*w;
        p.z = trans.mat[2][0]*x + trans.mat[2][1]*y + trans.mat[2][2]*z + trans.mat[2][3]*w;
        p.w = trans.mat[3][0]*x + trans.mat[3][1]*y + trans.mat[3][2]*z + trans.mat[3][3]*w;

        p.x = p.x/w;
        p.y = p.y/w;
        p.z = p.z/w;
        p.w = 1;
        
        x = p.x;
        y = p.y;
        z = p.z;
        w = p.w;
    }

    double scalarMult(point p){
        double value;
        value = x * p.x + y * p.y + z * p.z;
        return value;
    }

    point vectorMult(point p){
        point ret;
        ret.x = y*p.z - z*p.y;
        ret.y = -x*p.z + z*p.x; 
        ret.z = x*p.y - y*p.x;
        return ret;
    }

    point Rodrigues(point x,double angle){
        //convert in radian
        double theta = (angle*Pi)/180;
        double scalar = this->scalarMult(x);
        point vector = this->vectorMult(x);

        point result;
        result.x = cos(theta) * x.x + (1- cos(theta)) * scalar * this->x + sin(theta) * vector.x;
        result.y = cos(theta) * x.y + (1- cos(theta)) * scalar * this->y + sin(theta) * vector.y;
        result.z = cos(theta) * x.z + (1- cos(theta)) * scalar * this->z + sin(theta) * vector.z;

        return result;
    }

    void normalize(){
        double magnitude = sqrt(x*x + y*y + z*z);
        if(magnitude == 0)
            return;

        x = x/magnitude;
        y = y/magnitude;
        z = z/magnitude;
    }

    point subtract(point p){
        point result;
        result.x = x - p.x;
        result.y = y - p.y;
        result.z = z - p.z;

        return result;
    }

};

struct Triangle
{
    point vertex[3];
};

matrix makeRotationMatrix(point c1,point c2,point c3){
        matrix rotation;
        rotation = matrix().makeIdentityMatrix(4);

        rotation.mat[0][0] = c1.x;
        rotation.mat[1][0] = c1.y;
        rotation.mat[2][0] = c1.z;

        rotation.mat[0][1] = c2.x;
        rotation.mat[1][1] = c2.y;
        rotation.mat[2][1] = c2.z;

        rotation.mat[0][2] = c3.x;
        rotation.mat[1][2] = c3.y;
        rotation.mat[2][2] = c3.z;

        return rotation;
}

matrix generateT(point eye){
    matrix T;
    T = matrix().makeIdentityMatrix(4);

    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;

    return T;
}

matrix generateR(point u,point r,point l){
    matrix R;
    R = matrix().makeIdentityMatrix(4);

    R.mat[0][0] = r.x;
    R.mat[0][1] = r.y;
    R.mat[0][2] = r.z;

    R.mat[1][0] = u.x;
    R.mat[1][1] = u.y;
    R.mat[1][2] = u.z;

    R.mat[2][0] = -l.x;
    R.mat[2][1] = -l.y;
    R.mat[2][2] = -l.z;

    return R;
}


int main(){
    ifstream scene;
    ofstream stage1,stage2;

    scene.open("scene.txt");
    stage1.open("bhesbhes1.txt");
    stage2.open("bhesbhes2.txt");

    point eye,look,up;

    scene >> eye.x >> eye.y >> eye.z;
    scene >> look.x >> look.y >> look.z;
    scene >> up.x >> up.y >> up.z;

    double fovY,aspectRatio,near,far;

    scene >> fovY >> aspectRatio >> near >> far;

    //calculate u,r,l
    point l,u,r;
    l = look.subtract(eye);
    l.normalize();
    r = l.vectorMult(up);
    r.normalize();
    u = r.vectorMult(l);

    matrix R,T,V;
    T = generateT(eye);
    R = generateR(u,r,l);

    V = matrix().multiply(R,T);

    stack<matrix> command_stack;
    command_stack.push(matrix().makeIdentityMatrix(4));

    while(true){
        string command;
        scene >> command;
        if (command == "triangle"){
            Triangle t;
            for(int i = 0; i < 3;i++){
                scene >> t.vertex[i].x >> t.vertex[i].y >> t.vertex[i].z;
            }

            for(int i = 0; i < 3;i++){
                t.vertex[i].transformPoint(command_stack.top());
                stage1 << fixed << setprecision(6) << t.vertex[i].x << " " << t.vertex[i].y << " " << t.vertex[i].z << endl;
                t.vertex[i].transformPoint(V);
                stage2 << fixed << setprecision(6) << t.vertex[i].x << " " << t.vertex[i].y << " " << t.vertex[i].z << endl;
            }
            stage1 << endl;
            stage2 << endl;
        }
        else if(command == "translate"){
            double tx,ty,tz;
            scene >> tx >> ty >> tz;
            matrix trans;

            trans = matrix().makeTranslateMatrix(tx,ty,tz);
            trans = matrix().multiply(command_stack.top(),trans);
            command_stack.pop();
            command_stack.push(trans);
        }

        else if(command == "scale"){
            double sx,sy,sz;
            scene >> sx >> sy >> sz;
            matrix scale;

            scale = matrix().makeScaleMatrix(sx,sy,sz);
            scale = matrix().multiply(command_stack.top(),scale);
            command_stack.pop();
            command_stack.push(scale);
        }

        else if(command == "rotate"){
            point a;
            double angle;
            scene >> angle >> a.x >> a.y >> a.z;

            a.normalize();
            point c1,c2,c3;
            c1 = a.Rodrigues(point(1,0,0),angle);
            c2 = a.Rodrigues(point(0,1,0),angle);
            c3 = a.Rodrigues(point(0,0,1),angle);

            matrix rotation;

            rotation = makeRotationMatrix(c1,c2,c3);
            rotation = matrix().multiply(command_stack.top(),rotation);
            command_stack.pop();
            command_stack.push(rotation);
        }

        else if(command == "push"){
            command_stack.push(command_stack.top());
        }

        else if(command == "pop"){
            command_stack.pop();
        }

        else if(command == "end"){
            break;
        }

        else{
            cout << "Error!" << endl;
        }
    }
}