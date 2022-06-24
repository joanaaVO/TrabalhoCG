#ifndef bezier_h
#define bezier_h

#include <string>
#include <vector>
#include <math.h>

using namespace std;

void multMatrixVector(float* m, float *v, float *res);
void getBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos);
string parseBezierPatches(vector<int> patchesIndices, vector<vector<float>> all_points, float tesselation);


#endif