#include "function.h"

//Tạo một vector mới với phần tử cuối là last, còn lại là 0
Vector CreateVectorWithLast(int length, double last)
{
	Vector v;
	v.resize(length);
	for (int i = 0; i < length - 1; i++)
	{
		v[i] = 0;
	}
	v[length - 1] = last;
	return v;
}

//In ra nghiệm theo ma trận x
void PrintRoot(Matrix x)
{
	for (int i = 0; i < x.NRows(); i++)
	{
		cout << "x" << i + 1 << " = ";
		if (LeadingEntry(x[i]) == x.NCols())
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

//Hàm giải hệ phương trình tuyến tính, với A là ma trận bên trái dấu bằng, b là ma trận bên phải dấu bằng
void Solve(Matrix A, Matrix b)
{
	//Tạo ma trận B, là ma trận nối giữa A và b
	Matrix B(A.NRows(), A.NCols() + 1);

	//Nối A với B
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

		//Tạo ma trận temp là ma trận bậc thang của B
		Matrix temp = B.EchelonMatrix();

		//Số lượng biến tự do chính là số cột của A - túc số biến - trừ cho rank của A
		int freeVarCount = column - rankA;

		//Ma trận nghiệm x chứa các nghiệm theo dạng vector
		//Vị trí đầu tiên trong từng vector là hệ số của t1, vị trí cuối cùng là hệ số tự do
		Matrix x(column, freeVarCount + 1);

		//Gán tất cả phần tử của ma trận nghiệm bằng 0
		for (int i = 0; i < column; i++)
		{
			for (int j = 0; j < freeVarCount + 1; j++)
			{
				x[i][j] = 0;
			}
		}

		//Index1 là vị trí cột đang xét trong ma trận nghiệm
		//Index2 là vị trí dòng đang xét trong ma trận nghiệm
		int index1 = 0, index2 = 0;

		if (freeVarCount > 0)
		{
			//Gán các vị trí có ẩn tự do là 1
			for (int i = 0; i < column - freeVarCount; i++)
			{
				//Tìm vị trí đầu tiên trong vector, nếu nó bằng index2 nghĩa là vị trí index2 ko có ẩn tự do
				int first = LeadingEntry(temp[i]);
				if (first != index2)
				{
					for (; index2 < first; index2++)
					{
						x[index2][index1++] = 1;
					}
				}
				index2++;
			}

			//Tất cả các nghiệm còn lại đều là ẩn tự do
			for (; index2 < column; index2++)
			{
				x[index2][index1++] = 1;
			}
		}

		for (int i = column - freeVarCount - 1; i >= 0; i--)
		{
			int index = LeadingEntry(temp[i]);

			//Tạo một vector mới với phần tử cuối cùng là temp[i][column]
			Vector temp1 = CreateVectorWithLast(freeVarCount + 1, temp[i][column]);

			//Trừ vector ở vế phải cho các vector ở vế trái rồi chia cho hệ số để tìm ra nghiệm tại x[index]
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