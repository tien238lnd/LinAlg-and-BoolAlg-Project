#include "function.h"

int main()
{
	int column, row;
	cout << "Nhap he phuong trinh: \n";
	cout << "Nhap so an: ";
	cin >> column;
	cout << "Nhap so phuong trinh: ";
	cin >> row;
	Matrix A(row, column);
	Matrix B(row, 1);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			cout << "a" << i + 1 << j + 1 << " * " << "x" << j + 1;
			if (j != column - 1)
				cout << " + ";
			else
				cout << " = ";
		}
		cout << "b" << i + 1 << "\n";
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			cout << "a" << i + 1 << j + 1 << ": ";
			cin >> A[i][j];

		}
		cout << "b" << i + 1 << ": ";
		cin >> B[i][0];
	}

	Solve(A, B);

	return 0;
}