#pragma once
#include "Vector.h"
#include <cmath>

class Matrix
{
private:
	vector<Vector> rows;
	int nrows, ncols;
public:
	int NRows();
	int NCols();
	void Input();
	double Det();
	Matrix EchelonMatrix();
	Matrix Invert();
	void SwapRows(int i1, int i2);
	int Rank();

	Matrix operator+(const Matrix& other);
	Matrix operator-(const Matrix& other);
	Matrix operator*(const Matrix& other);
	Vector& operator [](int index);
	friend ostream& operator <<(ostream& os, const Matrix& A);

	Matrix();
	Matrix(int m, int n);
	Matrix(const Matrix &other);
	~Matrix();
};

