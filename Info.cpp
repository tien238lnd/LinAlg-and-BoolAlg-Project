#include "Info.h"

void help()
{
	cout << "Variable name is allowed with just one letter. \n";
	cout << "Ex: a = 4 is ok but Foo = 4 is not.\n";
	cout << "Format for vector is: [a, b, c, ...]\n";
	cout << "Ex: [3, -2, 0.5, 3.6e2] is a vector in R4.\n";
	cout << "Format for matrix is: [a1, b1, c1, ...][a2, b2, c2, ...]...[...]\n";
	cout << "Ex: [3, -2, 0.5, 3.6e2][-4.2, 0, 0.7, 3.0] is a matrix 2x4.\n";
	cout << "Arithmetic and variable name are not allowed inside vector or matrix.\n";
	cout << "Ex: [3+2, 4*7] will be error.\n";
	cout << "However, you can do arithmetic as you want outside [] (just + - * /, we didn't have ^ yet).\n";
	cout << "Some expamles:\n";
	cout << ">>> A = [1, 2][3, 4] \n";
	cout << ">>> B = [5][6] \n";
	cout << ">>> c = -7 \n";
	cout << ">>> B*(c*c - 2*c + 1) \n";
	cout << ">>> (-A + 5.3*A)*B \n";
	cout << ">>> D = [3, 4] \n";
	cout << ">>> D*A : D will be cast to a matrix 1x2 for multiplicate with matrix A 2x2, and result will be a matrix 1x2, not a vector anymore.\n";
	cout << ">>> -D*D : this will perform as a dot product.\n";
	cout << "You can also do arithmetic with the [] format directly.\n";
	cout << "Ex: [0, -1, 6][3, -4, 0.7] * [1][2][3] : it will be an 2x1 matrix.\n";
	cout << "Some functions with matrix: invert(), rank(), det(), transpose(), echelon(), and solve(A, b) with b is a vector or a matrix mx1.\n";
	cout << "Functions cannot be used with other functions or operators(Ex: invert(A)*A is not allowed).\n";
	cout << "Arithmetic with matrix is allowed inside these function.\n";
	cout << "Ex: rank(A*-A) is fine, or invert([2,3][5,6][0, -1] * [0,-1,9][6,0,-7]) is ok too.\n";
	cout << "SPECIAL FEATURE: If you want to get the last answer, use '_' (credit to python).\n";
	cout << "=======\n";
	cout << "For boolean function minimization, the format is:\n";
	cout << "bool = <type your function here>\n";
	cout << "We have 3 operators (with priority as order below):\n";
	cout << "\tNOT: denoted as '-' or '~' or '!'\n";
	cout << "\tAND: denoted as '^' or '*' or '&' or '.'\n";
	cout << "\tOR: denoted as 'V' or 'v' or '|' or '+'\n";
	cout << "Variable name is also allowed with just one letter. And leave out 'v' and 'V', we will treat them as operator.\n";
	cout << "Example:\n";
	cout << ">>> bool = a.(b+c.~a) + b.(~d.(~c + e) + ~(f.b.-g))\n";
	cout << "Your function can have up to 25 variables, but for 7 variable or more, we won't show the truth table and prime implicants.\n";
}
void credit()
{
	cout << "HCMUS - 10/2019\n";
	cout << "18TT5B - Data Structures and Algorithms\n";
	cout << "Lecturer: Nguyen Thanh An\n\n";
	cout << "Members:\n";
	cout << "Nguyen Dang Trung Tien - 18120591\n";
	cout << "Vo Minh Trieu - 18120615\n";
}