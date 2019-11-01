#include "LinAlg/Linear_Algebra.h"
#include "Bool/Bool_Minimize.h"
#include "Info.h"

unordered_map<char, Token> Vars_names;
char var_assign = '\0';



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

// nên thống nhất 0 là đúng còn khác 0 là sai