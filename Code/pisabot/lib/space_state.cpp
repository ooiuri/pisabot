#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Filters/Butterworth.hpp>
#include <Filters.h>

#define mtx_type	double

// Sampling frequency
const double f_s = 25; // Hz
// Cut-off frequency (-3 dB)
const double f_c = 7.84; // Hz
// Normalized cut-off frequency
const double f_n = 2 * f_c / f_s;

const float Ta = 40; // Tempo de Amostragem em ms

unsigned long lastMillis;
// Sixth-order Butterworth filter
auto filter = butter<6>(f_n);

#define MPU_SDA 23
#define MPU_SCL 22
MPU6050 mpu6050(Wire);
float angulofiltrado;

///////////////////////////////////////////////////////////////////
mtx_type rss = 0; //angulo de referência desejado
mtx_type Nx[2][1] = {{1}, {0}};
mtx_type Nu = -0.1957;
mtx_type phi_i[3][3] = {{0.9632, 0.0395, -0.1878}, {-1.8247, 0.9632, -9.3231}, {0, 0, 1.0000}};
mtx_type gama_i[3][1] = {{-0.1875}, {-9.3157}, {0}};
mtx_type C_i[3][1] = {{1}, {0}, {0}};
mtx_type D_i = 0; 

void setup() {
  Serial.begin(115200);
  Wire.begin(MPU_SDA, MPU_SCL, f_s);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {

  if(millis() - lastMillis > Ta){
    mpu6050.update();
    angulofiltrado = filter(mpu6050.getAngleX());
    
    Serial.print("angulofiltrado: ");
    Serial.println(angulofiltrado);
    
    lastMillis = millis();
  }  
}



void observador(float u, float y, float* x_chapeu, float* d_chapeu){
  // multiplicando gama_i*u
  mtx_type gama_i_u[3][1];
  MatrixMathScale(&gama_i[0][0], 3, 1, u, &gama_i_u[0][0]);
  mtx_type C_[3][1];
  MatrixMathScale(&C_i[0][0], 3, 1, u, &gama_i_u[0][0]);

}


// Matrix helper functions

void MatrixMathMultiply(mtx_type* A, mtx_type* B, int m, int p, int n, mtx_type* C)
{
	// A = input matrix (m x p)
	// B = input matrix (p x n)
	// m = number of rows in A
	// p = number of columns in A = number of rows in B
	// n = number of columns in B
	// C = output matrix = A*B (m x n)
	int i, j, k;
	for (i = 0; i < m; i++)
		for(j = 0; j < n; j++)
		{
			C[n * i + j] = 0;
			for (k = 0; k < p; k++)
				C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
		}
}


//Matrix Addition Routine
void MatrixMathAdd(mtx_type* A, mtx_type* B, int m, int n, mtx_type* C)
{
	// A = input matrix (m x n)
	// B = input matrix (m x n)
	// m = number of rows in A = number of rows in B
	// n = number of columns in A = number of columns in B
	// C = output matrix = A+B (m x n)
	int i, j;
	for (i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			C[n * i + j] = A[n * i + j] + B[n * i + j];
}

//Matrix Subtraction Routine
void MatrixMathSubtract(mtx_type* A, mtx_type* B, int m, int n, mtx_type* C)
{
	// A = input matrix (m x n)
	// B = input matrix (m x n)
	// m = number of rows in A = number of rows in B
	// n = number of columns in A = number of columns in B
	// C = output matrix = A-B (m x n)
	int i, j;
	for (i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			C[n * i + j] = A[n * i + j] - B[n * i + j];
}

void MatrixMathScale(mtx_type* A, int m, int n, mtx_type k, mtx_type* Result)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			Result[n * i + j] = A[n * i + j] * k;
}