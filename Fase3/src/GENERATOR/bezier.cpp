#include "headers/bezier.hpp"

void multMatrixVector(float* m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j*4+k];
		}
	}

}

void getBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos){
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vetorU[4] = { u * u * u, u * u, u, 1 };
    float vetorV[4] = { v * v * v, v * v, v, 1 };

    float vetorAux[4];
    float px[4];
    float py[4];
    float pz[4];

    float mx[4];
    float my[4];
    float mz[4];

    multMatrixVector((float*)bezierMatrix, vetorV, vetorAux);
    multMatrixVector((float*)matrixX, vetorAux, px);
    multMatrixVector((float*)matrixY, vetorAux, py);
    multMatrixVector((float*)matrixZ, vetorAux, pz);

    multMatrixVector((float*)bezierMatrix, px, mx);
    multMatrixVector((float*)bezierMatrix, py, my);
    multMatrixVector((float*)bezierMatrix, pz, mz);


    pos[0] = (vetorU[0] * mx[0]) + (vetorU[1] * mx[1]) + (vetorU[2] * mx[2]) + (vetorU[3] * mx[3]);
    pos[1] = (vetorU[0] * my[0]) + (vetorU[1] * my[1]) + (vetorU[2] * my[2]) + (vetorU[3] * my[3]);
    pos[2] = (vetorU[0] * mz[0]) + (vetorU[1] * mz[1]) + (vetorU[2] * mz[2]) + (vetorU[3] * mz[3]);

}




string parseBezierPatches(vector<int> patchesIndices, vector<vector<float>> all_points, float tesselation) {

    string res;

    float matrixX[4][4];
    float matrixY[4][4];
    float matrixZ[4][4];

    float pos[4][3];
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

                getBezierPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[0]);
                getBezierPoint(u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[1]);
                getBezierPoint(next_u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[2]);
                getBezierPoint(next_u, next_v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[3]);



                res += std::to_string(pos[3][0]) + "," + std::to_string(pos[3][1]) + "," + std::to_string(pos[3][2]) + "\n";
                res += std::to_string(pos[2][0]) + "," + std::to_string(pos[2][1]) + "," + std::to_string(pos[2][2]) + "\n";
                res += std::to_string(pos[0][0]) + "," + std::to_string(pos[0][1]) + "," + std::to_string(pos[0][2]) + "\n";

                res += std::to_string(pos[0][0]) + "," + std::to_string(pos[0][1]) + "," + std::to_string(pos[0][2]) + "\n";
                res += std::to_string(pos[1][0]) + "," + std::to_string(pos[1][1]) + "," + std::to_string(pos[1][2]) + "\n";
                res += std::to_string(pos[3][0]) + "," + std::to_string(pos[3][1]) + "," + std::to_string(pos[3][2]) + "\n";
                
            }
              
        }
    }   
    return res;
}