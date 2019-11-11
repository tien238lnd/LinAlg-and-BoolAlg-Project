#pragma once

#include "Vector.h"
#include <cmath>
#include <sstream>
#include <iomanip>

class Matrix
{
private:
	vector<Vector> rows;
	int nrows, ncols;
public:
	int NRows();
	int NCols();
	double Det();
	Matrix EchelonMatrix();
	Matrix Invert();
	void SwapRows(int i1, int i2);
	int Rank();

	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;
	Matrix Transpose();
	Vector& operator [](int index);
	friend ostream& operator <<(ostream& os, const Matrix& A);
	friend Matrix operator*(double k, const Matrix &A);
	friend Matrix operator*(const Vector &vec, const Matrix &A);
	Matrix();
	Matrix(int m, int n);
	Matrix(const Vector &vec);
	Matrix(const string &str);
	Matrix(const vector<Vector> &vecs);
	Matrix(const Matrix &other);
	~Matrix();
};

