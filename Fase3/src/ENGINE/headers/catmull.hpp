#ifndef catmull_h
#define catmull_h

#include <string>
#include <vector>
#include <math.h>
#include <tgmath.h>
#include <GL/glew.h>
#include <GL/glut.h>

void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void multMatrixVector(float m[4][4], float *v, float *res);
void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);
void renderCatmullRomCurve(std::vector<float> translate);
void getGlobalCatmullRomPoint(std::vector<float> translate, float gt, float *pos, float *deriv);


#endif