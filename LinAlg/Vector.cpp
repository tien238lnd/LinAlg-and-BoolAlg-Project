#include "Vector.h"

int Approximate(double a, double b)
{
	return (abs(a - b) < epsilon);
}

Vector operator + (const Vector& A, const Vector& B)
{
	Vector C;
	if (A.size() == B.size())
	{
		int n = (int)A.size();
		C.resize(A.size());
		for (int i = 0; i < n; i++)
			C[i] = A[i] + B[i];
	}
	return C;
}

Vector operator - (const Vector& A, const Vector& B)
{
	Vector C;
	if (A.size() == B.size())
	{
		int n = (int)A.size();
		C.resize(A.size());
		for (int i = 0; i < n; i++)
			C[i] = A[i] - B[i];
	}
	return C;
}

Vector operator * (double A, const Vector& B)
{
	Vector Result;
	int n = (int)B.size();
	Result.resize(n);
	for (int i = 0; i < n; i++)
	{
		Result[i] = B[i] * A;
	}
	return Result;
}

double operator*(const Vector & A, const Vector & B)
{
	if (A.size() != B.size())
		return DBL_MAX;
	double result = 0;
	for (int i = 0; i < A.size(); i++)
		result += A[i] * B[i];
	return result;
}

ostream& operator << (ostream& os, const Vector& v)
{
	int n = (int)v.size();
	os << "[";
	for (int i = 0; i < n; i++)
	{
		os << v[i] << " ";
	}
	os << "]\n";
	return os;
}

int LeadingEntry(Vector v)
{
	int i;
	for (i = 0; i < v.size(); i++)
	{
		if (!Approximate(v[i], 0))
			return i;
	}
	return i;
}