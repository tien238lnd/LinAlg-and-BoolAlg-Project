#include "Matrix.h"

int Matrix::NRows()
{
	return nrows;
}

int Matrix::NCols()
{
	return ncols;
}

void Matrix::Input()
{
	cout << "NRows: ";
	cin >> nrows;
	cout << "NCols: ";
	cin >> ncols;
	rows.resize(nrows);
	for (int i = 0; i < nrows; i++)
	{
		rows[i].resize(ncols);
		for (int j = 0; j < ncols; j++)
		{
			cout << i << "_" << j << ": ";
			cin >> rows[i][j];
		}
	}
}

double Matrix::Det()
{
	if (nrows != ncols)
	{
		cout << "Matrix is not square, returning 0!\n";
		return 0;
	}
	Matrix temp = EchelonMatrix();
	double det = 1;
	for (int i = 0; i < nrows; i++)
	{
		det *= temp[i][i];
	}
	return det;
}

Matrix& Matrix::EchelonMatrix()
{
	Matrix* temp = new Matrix(*this);
	for (int i = 1; i < nrows; i++)
	{
		for (int j = 0; j < i; j++)
		{
			int f1 = FirstNumberInRow(temp->rows[j]);
			int f2 = FirstNumberInRow(temp->rows[i]);
			if (f1 == f2 && f1 != -1)
			{
				double factor = temp->rows[i][f1] / temp->rows[j][f1];
				for (int k = f1; k < ncols; k++)
				{
					temp->rows[i][k] -= factor * temp->rows[j][k];
				}
				cout << "\n" << *temp << "\n";
			}
			else if(f1 > f2)
			{
				temp->SwapRows(i, j);
				cout << "\n" << *temp << "\n";
				break;
			}
		}
	}
	return *temp;
}

Matrix& Matrix::Invert()
{
	if(ncols != nrows || Det() == 0)
	{
		cout << "Cannot Invert!\n";
		return *this;
	}
	Matrix temp(*this);
	Matrix *I = new Matrix(ncols, ncols);
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			if (i == j)
				I->rows[i][i] = 1;
			else
				I->rows[i][j] = 0;
		}
	}

	for (int i = 1; i < nrows; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (temp[j][j] != 0)
			{
				double factor = temp[i][j] / temp[j][j];
				for (int k = j; k < ncols; k++)
				{
					temp[i][k] -= factor * temp[j][k];
					I->rows[i][k] -= factor * I->rows[j][k];
				}
			}
			else
			{
				temp.SwapRows(i, j);
				I->SwapRows(i, j);
				break;
			}
		}
	}

	for (int i = nrows - 2; i >= 0; i--)
	{
		for (int j = nrows - 1; j > i; j--)
		{
			double factor = temp[i][j] / temp[j][j];
			for (int k = 0; k < ncols; k++)
			{
				temp[i][k] -= factor * temp[j][k];
				I->rows[i][k] -= factor * I->rows[j][k];
			}
		}
	}

	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			I->rows[i][j] /= temp[i][i];
		}
		temp[i][i] = 1;
	}
	return *I;
}

void Matrix::SwapRows(int i1, int i2)
{
	Vector t = rows[i1];
	rows[i1] = rows[i2];
	rows[i2] = t;
}

int Matrix::Rank()
{
	Matrix temp = EchelonMatrix();
	int rank = 0;
	for (int i = 0; i < nrows; i++)
	{
		if (FirstNumberInRow(temp[i]) != -1)
			rank++;
	}
	return rank;
}

Matrix& Matrix::operator+(Matrix& other)
{
	if (nrows != other.nrows || ncols != other.ncols)
	{
		cout << "Invalid dimesion!\n";
		return *this;
	}
	Matrix* matrix = new Matrix(nrows, ncols);
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			(*matrix)[i][j] = rows[i][j] + other.rows[i][j];
		}
	}
	return *matrix;
}

Matrix& Matrix::operator-(Matrix& other)
{
	if (nrows != other.nrows || ncols != other.ncols)
	{
		cout << "Invalid dimesion!\n";
		return *this;
	}
	Matrix* matrix = new Matrix(nrows, ncols);
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			(*matrix)[i][j] = rows[i][j] - other.rows[i][j];
		}
	}
	return *matrix;
}

Matrix& Matrix::operator*(Matrix& other)
{
	if (ncols != other.nrows)
	{
		cout << "Invalid dimesion!\n";
		return *this;
	}
	Matrix* result = new Matrix(nrows, other.ncols);

	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < other.ncols; j++)
		{
			double S = 0;
			for (int k = 0; k < ncols; k++)
			{
				S += rows[i][k] * other.rows[k][j];
			}
			result->rows[i][j] = S;
		}
	}
	return *result;
}

Vector& Matrix::operator [](int index)
{
	return rows[index];
}

ostream& operator <<(ostream& os, Matrix& A)
{
	for (int i = 0; i < A.nrows; i++)
	{
		for (int j = 0; j < A.ncols; j++)
		{
			cout << A.rows[i][j] << " ";
		}
		cout << "\n";
	}
	return os;
}

Matrix::Matrix()
{
	nrows = ncols = 0;
}

Matrix::Matrix(int m, int n)
{
	nrows = m;
	ncols = n;
	rows.resize(nrows);
	for (int i = 0; i < nrows; i++)
	{
		rows[i].resize(ncols);
	}
}

Matrix::Matrix(Matrix &other)
{
	nrows = other.nrows;
	ncols = other.ncols;
	rows.resize(nrows);
	for (int i = 0; i < nrows; i++)
	{
		rows[i].resize(ncols);
		for (int j = 0; j < ncols; j++)
		{
			rows[i][j] = other.rows[i][j];
		}
	}
}

Matrix::~Matrix()
{
}
