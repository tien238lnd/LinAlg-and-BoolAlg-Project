#include "LinAlg/Linear_Algebra.h"
#include "Bool/Bool_Minimize.h"

unordered_map<char, Token> Vars_names;
char var_assign = '\0';

void help();
void credit();

int main()
{
	cout << "Type '?' for help using program. Type'credits' for more information about authors. Type 'exit()' to exit.\n";
	while (true)
	{

		cout << "\n>>> ";
		string expression;
		getline(cin, expression);

		erase_space(expression);

		if (expression.length() == 0)
			continue;
		if (expression == "?") {
			help(); continue;
		}
		else if (expression == "credits") {
			credit(); continue;
		}
		else if (expression == "exit()")
			break;
		
		if (expression.substr(0, 5) == "bool=") {
			bool_minimize(expression.substr(5, expression.length() - 5));
			continue;
		}
		else if (expression.substr(0, 6) == "solve(" && expression.back() == ')') {
			solve_sysEquation(expression.substr(6, expression.length() - 7));
			continue;
		}

		string r_Express;
		if (seperate_lvalue_rvalue(expression, r_Express) == 0) {
			cout << "Wrong expression!\n";
			continue;
		}

		if (r_Express.substr(0, 7) == "invert(" && r_Express.back() == ')')
			invert(r_Express.substr(7, r_Express.length() - 8));
			
		else if (r_Express.substr(0, 10) == "transpose(" && r_Express.back() == ')')
			transpose(r_Express.substr(10, r_Express.length() - 11));
			
		else if (r_Express.substr(0, 8) == "echelon(" && r_Express.back() == ')')
			echelon(r_Express.substr(8, r_Express.length() - 9));

		else if (r_Express.substr(0, 4) == "det(" && r_Express.back() == ')')
			det(r_Express.substr(4, r_Express.length() - 5));

		else if (r_Express.substr(0, 5) == "rank(" && r_Express.back() == ')')
			rank_matrix(r_Express.substr(5, r_Express.length() - 6));

		else algebra(r_Express);
	}
	return 0;
}

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
	cout << "Arithmetic with matrix is allowed inside these function.\n";
	cout << "Ex: rank(A*-A) is fine, or invert([2,3][5,6][0, -1] * [0,-1,9][6,0,-7]) is ok too.\n";
	cout << "However, if you want to solve system equations with solve(A, b), you must firstly assign them to 2 variables. Directly type [] inside solve() is not allowed.\n";
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
	cout << "HCMUS - 10/2019";
	cout << "18TT5B - Data Structures and Algorithms\n";
	cout << "Lecturer: Nguyen Thanh An\n\n";
	cout << "Members:\n";
	cout << "Nguyen Dang Trung Tien - 18120591\n";
	cout << "Vo Minh Trieu - 18120615\n";
}

// nên thống nhất 0 là đúng còn khác 0 là sai