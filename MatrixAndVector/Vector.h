#pragma once
#include <iostream>
#include <vector>
using namespace std;


#define Vector vector<double>
#define epsilon 10e-8

int Approximate(double a, double b);

Vector& operator + (Vector& A, Vector& B);
Vector& operator - (Vector& A, Vector& B);
Vector& operator * (Vector& A, double B);
ostream& operator << (ostream& os, Vector& v);
int FirstNumberInRow(Vector v);