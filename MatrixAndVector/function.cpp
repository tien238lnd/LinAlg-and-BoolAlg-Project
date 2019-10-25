#include "function.h"

Vector CreateVectorWithLast(int length, double last)
{
	vector<double> v;
	v.resize(length);
	for (int i = 0; i < length - 1; i++)
	{
		v[i] = 0;
	}
	v[length - 1] = last;
	return v;
}

void PrintRoot(Matrix x)
{
	for (int i = 0; i < x.NRows(); i++)
	{
		cout << "x" << i + 1 << " = ";
		if (FirstNumberInRow(x[i]) == -1)
			cout << "0";
		else
		{
			for (int j = 0; j < x.NCols(); j++)
			{
				if (x[i][j] != 0)
				{
					if (j != 0 && x[i][j - 1] != 0)
						cout << " + ";
					cout << x[i][j];
					if (j < x.NCols() - 1)
						cout << "*t" << j + 1;
				}
			}
		}
		cout << "\n";
	}
}

void Solve(Matrix A, Matrix b)
{
	Matrix B(A.NRows(), A.NCols() + 1);

	for (int i = 0; i < A.NRows(); i++)
	{
		for (int j = 0; j < A.NCols(); j++)
		{
			B[i][j] = A[i][j];
		}
		B[i][A.NCols()] = b[i][0];
	}

	int rankA = A.Rank();
	int rankB = B.Rank();
	
	if (rankA < rankB)
	{
		cout << "No solution.\n";
	}
	else
	{
		int row = A.NRows(), column = A.NCols();

		Matrix temp = B.EchelonMatrix();
		int freeVarCount = column - rankA; //So luong bien tu do

		Matrix x(column, freeVarCount + 1); // Ma tran nghiem

		for (int i = 0; i < column; i++)
		{
			for (int j = 0; j < freeVarCount + 1; j++)
			{
				x[i][j] = 0;
			}
		}

		int index1 = 0, index2 = 0;

		for (int i = 0; i < column - freeVarCount; i++) 
		{
			int first = FirstNumberInRow(temp[i]);
			if (first != index2)
			{
				for (; index2 < first; index2++)
				{
					x[index2][index1++] = 1;
				}
			}
			index2++;
		}

		for (; index2 < column; index2++)
		{
			x[index2][index1++] = 1;
		}

		for (int i = column - freeVarCount - 1; i >= 0; i--)
		{
			int index = FirstNumberInRow(temp[i]);
			Vector temp1 = CreateVectorWithLast(freeVarCount + 1, temp[i][column]);
			for (int j = index + 1; j < column; j++)
			{
				temp1 = temp1 - x[j] * temp[i][j];
			}
			temp1 = temp1 * (1.0/temp[i][index]);
			x[index] = temp1;
		}

		PrintRoot(x);
	}
}