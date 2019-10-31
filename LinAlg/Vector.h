#pragma once

#include <iostream>
#include <vector>
using namespace std;

#define Vector vector<double>
#define epsilon 10e-8

int Approximate(double a, double b);

Vector operator + (const Vector& A, const Vector& B);
Vector operator - (const Vector& A, const Vector& B);
Vector operator * (double A, const Vector& B);
double operator * (const Vector& A, const Vector& B);

ostream& operator << (ostream& os, const Vector& v);
int LeadingEntry(Vector v);