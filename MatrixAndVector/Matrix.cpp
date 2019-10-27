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
		cout << "Matrix's not square. Cannot calculate determinant";
		return 0;
	}
	Matrix temp(*this);
	double det = 1;
	for (int i = 1; i < nrows; i++)
	{
		for (int j = 0; j < i; j++)
		{
			int f1 = LeadingEntry(temp[j]);
			int f2 = LeadingEntry(temp[i]);
			if (f1 == f2 && f1 != ncols)
			{
				double factor = temp[i][f1] / temp[j][f1];
				for (int k = f1; k < ncols; k++)
				{
					temp[i][k] -= factor * temp[j][k];
				}
			}
			else if (f1 > f2)
			{
				det *= -1;
				temp.SwapRows(i, j);
				break;
			}
		}
	}
	for(int i  = 0; i < nrows; i++)
	{
		int index = LeadingEntry(temp[i]);
		if (index == ncols)
			return 0;
		det *= temp[i][index];

	}
	return det;
}

Matrix Matrix::EchelonMatrix()
{
	//Tạo bản sao của ma trận gốc để không thay đổi ma trận gốc
	Matrix temp(*this);
	for (int i = 1; i < nrows; i++)
	{
		for (int j = 0; j < i; j++)
		{
			//Tìm vị trí phần tử khác 0 đầu tiên
			int f1 = LeadingEntry(temp[j]);
			int f2 = LeadingEntry(temp[i]);
			//Nếu vị trí khác 0 của 2 hàng bằng nhau thì trừ cho hàng đang xét factor lần hàng j
			if (f1 == f2 && f1 != ncols)
			{
				//factor là tỉ lệ cần trừ để vị trí f2 của hàng i = 0
				double factor = temp[i][f1] / temp[j][f1];
				//Trừ cả hàng cho factor lần hàng j
				for (int k = f1; k < ncols; k++)
				{
					temp[i][k] -= factor * temp[j][k];
				}
			}
			//Nếu f1 > f2 thì ta phải hoán vị 2 hàng cho nhau
			else if(f1 > f2)
			{
				temp.SwapRows(i, j);
				break;
			}
		}
	}
	return temp;
}

Matrix Matrix::Invert()
{
	if(ncols != nrows || Det() == 0)
	{
		cout << "Cannot Invert!\n";
		return *this;
	}
	//Tạo bản sao của ma trận gốc để không thay đổi ma trận gốc
	Matrix temp(*this);
	//Tạo ma trận đơn vị I
	Matrix I(ncols, ncols);

	//Điền vào ma trận đơn vị I tại đường chéo là 1, và những điểm khác là 0
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			if (i == j)
				I[i][i] = 1;
			else
				I[i][j] = 0;
		}
	}

	//Chuyển về ma trận bậc thang
	for (int i = 1; i < nrows; i++)
	{
		for (int j = 0; j < i; j++)
		{
			//Tìm vị trí phần tử khác 0 đầu tiên
			int f1 = LeadingEntry(temp[j]);
			int f2 = LeadingEntry(temp[i]);

			//Nếu vị trí khác 0 đầu tiên của 2 hàng bằng nhau thì trừ cho hàng đang xét factor lần hàng j
			if (f1 == f2 && f1 != ncols)
			{
				//factor là tỉ lệ cần trừ để vị trí f2 của hàng i = 0
				double factor = temp[i][f1] / temp[j][f1];

				//Trừ cả hàng cho factor lần hàng j
				for (int k = 0; k < ncols; k++)
				{
					temp[i][k] -= factor * temp[j][k];
					I[i][k] -= factor * I[j][k];
				}
			}

			//Nếu f1 > f2 và f2 khác -1 tức hàng thứ i có phần tử khác 0, thì ta phải hoán vị 2 hàng cho nhau
			else if (f1 > f2)
			{
				temp.SwapRows(i, j);
				I.SwapRows(i, j);
				break;
			}
		}
	}

	//Chuyển về ma trận đơn vị

	//Duyệt từ hàng thứ 2 từ dưới lên
	for (int i = nrows - 2; i >= 0; i--)
	{
		for (int j = nrows - 1; j > i; j--)
		{
			//Tìm tỉ lệ cần trừ để vị trí j của hàng i = 0
			double factor = temp[i][j] / temp[j][j];
			for (int k = 0; k < ncols; k++)
			{
				temp[i][k] -= factor * temp[j][k];
				I[i][k] -= factor * I[j][k];
			}
		}
	}

	//Chia các phần tử của hàng i cho temp[i][i] đưa temp[i][i] về 1, tức ma trận đơn vị
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			I[i][j] /= temp[i][i];
		}
	}
	return I;
}

//Hàm hoán vị 2 dòng trong ma trận
//i1, i2 là vị trí của 2 dòng cần hoán vị
void Matrix::SwapRows(int i1, int i2)
{
	Vector t = rows[i1];
	rows[i1] = rows[i2];
	rows[i2] = t;
}

//Hàm tính hạng của ma trận
int Matrix::Rank()
{
	//Tạo ma trận mới là ma trận bậc thang
	Matrix temp = EchelonMatrix();
	int rank;
	for (rank = 0; rank < nrows; rank++)
	{
		//Nếu hàng i có phần tử khác 0, thì tăng rank lên 1
		if (LeadingEntry(temp[rank]) == ncols)
			break;
	}
	return rank;
}

Matrix Matrix::operator+(const Matrix& other)
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

Matrix Matrix::operator-(const Matrix& other)
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

//Toán tử nhân 2 ma trận
Matrix Matrix::operator*(const Matrix& other)
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

ostream& operator <<(ostream& os,const Matrix& A)
{
	for (int i = 0; i < A.nrows; i++)
	{
		for (int j = 0; j < A.ncols; j++)
		{
			os << A.rows[i][j] << " ";
		}
		os << "\n";
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

Matrix::Matrix(const Matrix &other)
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
