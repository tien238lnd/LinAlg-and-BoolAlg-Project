#include "Vector.h"

int Approximate(double a, double b)
{
	return (abs(a - b) < epsilon);
}

Vector& operator + (Vector& A, Vector& B)
{
	if (A.size() != B.size())
	{
		cout << "Invalid dimesional!";
		return A;
	}
	Vector *C = new Vector;
	int n = (int)A.size();
	C->resize(A.size());
	for (int i = 0; i < n; i++)
	{
		(*C)[i] = A[i] + B[i];
	}
	return *C;
}

Vector& operator - (Vector& A, Vector& B)
{
	if (A.size() != B.size())
	{
		cout << "Invalid dimesion!";
		return A;
	}
	Vector *C = new Vector;
	int n = (int)A.size();
	C->resize(A.size());
	for (int i = 0; i < n; i++)
	{
		(*C)[i] = A[i] - B[i];
	}
	return *C;
}

Vector& operator * (Vector& A, double B)
{
	Vector* Result = new Vector();
	int n = (int)A.size();
	Result->resize(n);
	for (int i = 0; i < n; i++)
	{
		(*Result)[i] = A[i] * B;
	}
	return *Result;
}

ostream& operator << (ostream& os, Vector& v)
{
	int n = (int)v.size();
	for (int i = 0; i < n; i++)
	{
		os << v[i] << " ";
	}
	return os;
}

int FirstNumberInRow(Vector v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (!Approximate(v[i], 0))
			return i;
	}
	return -1;
}