#include "headers/bezier.hpp"

void multMatrixVector(float* m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j*4+k];
		}
	}

}

void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void getBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos){
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vectorU[4] = { u * u * u, u * u, u, 1 };
    float vectorV[4] = { v * v * v, v * v, v, 1 };

    float vectorAux[4];
    float px[4];
    float py[4];
    float pz[4];

    float mx[4];
    float my[4];
    float mz[4];

    multMatrixVector((float*)bezierMatrix, vectorV, vectorAux);
    multMatrixVector((float*)matrixX, vectorAux, px);
    multMatrixVector((float*)matrixY, vectorAux, py);
    multMatrixVector((float*)matrixZ, vectorAux, pz);

    multMatrixVector((float*)bezierMatrix, px, mx);
    multMatrixVector((float*)bezierMatrix, py, my);
    multMatrixVector((float*)bezierMatrix, pz, mz);


    pos[0] = (vectorU[0] * mx[0]) + (vectorU[1] * mx[1]) + (vectorU[2] * mx[2]) + (vectorU[3] * mx[3]);
    pos[1] = (vectorU[0] * my[0]) + (vectorU[1] * my[1]) + (vectorU[2] * my[2]) + (vectorU[3] * my[3]);
    pos[2] = (vectorU[0] * mz[0]) + (vectorU[1] * mz[1]) + (vectorU[2] * mz[2]) + (vectorU[3] * mz[3]);

}

void getNormalBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos){
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vectorU[4] = { u * u * u, u * u, u, 1 };
    float derivU[4] = { 3 * u * u, 2 * u, 1, 0 };

    float vectorV[4] = { v * v * v, v * v, v, 1 };
    float derivV[4] = { 3 * v * v, 2 * v, 1, 0 };

    // Calculate Derivate V
    float vectorAuxV[4];
    float pxV[4];
    float pyV[4];
    float pzV[4];

    float mxV[4];
    float myV[4];
    float mzV[4];

    float pos_V[3];

    multMatrixVector((float*)bezierMatrix, derivV, vectorAuxV);
    multMatrixVector((float*)matrixX, vectorAuxV, pxV);
    multMatrixVector((float*)matrixY, vectorAuxV, pyV);
    multMatrixVector((float*)matrixZ, vectorAuxV, pzV);

    multMatrixVector((float*)bezierMatrix, pxV, mxV);
    multMatrixVector((float*)bezierMatrix, pyV, myV);
    multMatrixVector((float*)bezierMatrix, pzV, mzV);


    pos_V[0] = (vectorU[0] * mxV[0]) + (vectorU[1] * mxV[1]) + (vectorU[2] * mxV[2]) + (vectorU[3] * mxV[3]); 
    pos_V[1] = (vectorU[0] * myV[0]) + (vectorU[1] * myV[1]) + (vectorU[2] * myV[2]) + (vectorU[3] * myV[3]); 
    pos_V[2] = (vectorU[0] * mzV[0]) + (vectorU[1] * mzV[1]) + (vectorU[2] * mzV[2]) + (vectorU[3] * mzV[3]); 


    // Calculate Derivate U
    float vectorAuxU[4];
    float pxU[4];
    float pyU[4];
    float pzU[4];

    float mxU[4];
    float myU[4];
    float mzU[4];

    float pos_U[3];

    multMatrixVector((float*)bezierMatrix, vectorV, vectorAuxU);
    multMatrixVector((float*)matrixX, vectorAuxU, pxU);
    multMatrixVector((float*)matrixY, vectorAuxU, pyU);
    multMatrixVector((float*)matrixZ, vectorAuxU, pzU);

    multMatrixVector((float*)bezierMatrix, pxU, mxU);
    multMatrixVector((float*)bezierMatrix, pyU, myU);
    multMatrixVector((float*)bezierMatrix, pzU, mzU);


    pos_U[0] = (derivU[0] * mxU[0]) + (derivU[1] * mxU[1]) + (derivU[2] * mxU[2]) + (derivU[3] * mxU[3]);
    pos_U[1] = (derivU[0] * myU[0]) + (derivU[1] * myU[1]) + (derivU[2] * myU[2]) + (derivU[3] * myU[3]);
    pos_U[2] = (derivU[0] * mzU[0]) + (derivU[1] * mzU[1]) + (derivU[2] * mzU[2]) + (derivU[3] * mzU[3]);

    cross(pos_V, pos_U, pos);

    normalize(pos);

}




string parseBezierPatches(vector<int> patchesIndices, vector<vector<float>> all_points, float tesselation) {

    string res;

    float matrixX[4][4];
    float matrixY[4][4];
    float matrixZ[4][4];

    float pos[4][3];
    float n_pos[4][3];
    float u = 0;
    float v = 0;
    float t = 1 / tesselation;

    int counter = 0;

    int number_points = (patchesIndices.size() / 16) * 6 * tesselation * tesselation;
    
    res += std::to_string(number_points) + "\n";

    for (int s = 0; s < patchesIndices.size(); s+=16){
        
        for (size_t a = 0; a < 4; a++) {
                    
            for (size_t b = 0; b < 4; b++) {
                    matrixX[a][b] = all_points.at(patchesIndices.at(s + a * 4 + b))[0];
                    matrixY[a][b] = all_points.at(patchesIndices.at(s + a * 4 + b))[1];
                    matrixZ[a][b] = all_points.at(patchesIndices.at(s + a * 4 + b))[2];
            }
        }

        for (int i = 0; i < tesselation; i++){

            for (int j = 0; j < tesselation; j++){
                counter++;
                u = t * i;
                v = t * j;
                float next_u = t * (i+1);
                float next_v = t * (j+1);

                // Calculate Points
                getBezierPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[0]);
                getBezierPoint(u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[1]);
                getBezierPoint(next_u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[2]);
                getBezierPoint(next_u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[3]);

                // Calculate Normals
                getNormalBezierPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, n_pos[0]);
                getNormalBezierPoint(u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, n_pos[1]);
                getNormalBezierPoint(next_u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, n_pos[2]);
                getNormalBezierPoint(next_u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, n_pos[3]);

                // Draw Points, Draw Normals, Draw Textures
                res += std::to_string(pos[3][0]) + "," + std::to_string(pos[3][1]) + "," + std::to_string(pos[3][2]) + "\n";
                res += std::to_string(n_pos[3][0]) + "," + std::to_string(n_pos[3][1]) + "," + std::to_string(n_pos[3][2]) + "\n";
                res += std::to_string(1.0f - next_v) + "," + std::to_string(1.0f - next_u) + "," + std::to_string(0) + "\n";

                res += std::to_string(pos[2][0]) + "," + std::to_string(pos[2][1]) + "," + std::to_string(pos[2][2]) + "\n";
                res += std::to_string(n_pos[2][0]) + "," + std::to_string(n_pos[2][1]) + "," + std::to_string(n_pos[2][2]) + "\n";
                res += std::to_string(1.0f - v) + "," + std::to_string(1.0f - next_u) + "," + std::to_string(0) + "\n";

                res += std::to_string(pos[0][0]) + "," + std::to_string(pos[0][1]) + "," + std::to_string(pos[0][2]) + "\n";
                res += std::to_string(n_pos[0][0]) + "," + std::to_string(n_pos[0][1]) + "," + std::to_string(n_pos[0][2]) + "\n";
                res += std::to_string(1.0f - v) + "," + std::to_string(1.0f - u) + "," + std::to_string(0) + "\n";

                res += std::to_string(pos[0][0]) + "," + std::to_string(pos[0][1]) + "," + std::to_string(pos[0][2]) + "\n";
                res += std::to_string(n_pos[0][0]) + "," + std::to_string(n_pos[0][1]) + "," + std::to_string(n_pos[0][2]) + "\n";
                res += std::to_string(1.0f - v) + "," + std::to_string(1.0f - u) + "," + std::to_string(0) + "\n";

                res += std::to_string(pos[1][0]) + "," + std::to_string(pos[1][1]) + "," + std::to_string(pos[1][2]) + "\n";
                res += std::to_string(n_pos[1][0]) + "," + std::to_string(n_pos[1][1]) + "," + std::to_string(n_pos[1][2]) + "\n";
                res += std::to_string(1.0f - next_v) + "," + std::to_string(1.0f - u) + "," + std::to_string(0) + "\n";

                res += std::to_string(pos[3][0]) + "," + std::to_string(pos[3][1]) + "," + std::to_string(pos[3][2]) + "\n";
                res += std::to_string(n_pos[3][0]) + "," + std::to_string(n_pos[3][1]) + "," + std::to_string(n_pos[3][2]) + "\n";
                res += std::to_string(1.0f - next_v) + "," + std::to_string(1.0f - next_u) + "," + std::to_string(0) + "\n";
                
                
            }
              
        }
    }   
    return res;
}