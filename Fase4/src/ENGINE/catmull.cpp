#include "headers/catmull.hpp"


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
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


float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float m[4][4], float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j][k];
		}
	}

}


void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};

	for(int i = 0; i < 3; i++){
		
		// Compute A = M * P
		float a[4];
		float v[4] = { p0[i], p1[i], p2[i], p3[i] };
		multMatrixVector(m, v, a);

		// Compute pos = T * A
		pos[i] = powf(t, 3.0) * a[0] + powf(t, 2.0) * a[1] + t * a[2] + a[3];

		// compute deriv = T' * A
		deriv[i] = 3.0 * powf(t, 2.0) * a[0] + 2.0 * t * a[1] + a[2];

	}
}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(std::vector<float> translate, float gt, float *pos, float *deriv) {
    int size = translate.size()/3;
	float t = gt * size; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

    float** p = (float**) malloc(sizeof(float*) * size);
    int c = 0;

    for(int i = 0; i < translate.size(); i+=3){
        p[c] = (float*) malloc(sizeof(float) * 3);
		p[c][0] = translate[i];
        p[c][1] = translate[i+1];
        p[c][2] = translate[i+2];
        c++;
    }

	// indices store the points
	int indices[4]; 
	indices[0] = (index + size-1)%size;	
	indices[1] = (indices[0]+1)%size;
	indices[2] = (indices[1]+1)%size; 
	indices[3] = (indices[2]+1)%size;

    getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);

	for(int i = 0; i < size; i++)
		free(p[i]);
	free(p);	
}


// Draws Curve
void renderCatmullRomCurve(std::vector<float> translate) {

// draw curve using line segments with GL_LINE_LOOP
	float pos[3], deriv[3];

	float gt = 0.00f;
	
	glBegin(GL_LINE_LOOP);
	while(gt < 1){
		getGlobalCatmullRomPoint(translate, gt, pos, deriv);	
		glVertex3f(pos[0], pos[1], pos[2]);
		gt += 0.01f;
	}	
	glEnd();
}