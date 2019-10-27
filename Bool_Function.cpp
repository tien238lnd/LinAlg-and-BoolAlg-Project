#include <iostream>

#include "Expression_processing.h"
#include "Find_Prime_Implicants.h"
#include "Find_EPIs.h"

void print_final_result(const vector<vector<Implicant*>> &final_result, const vector<char> &vars_names)
{
	for (const auto &an_minimumFunc : final_result)
	{
		for (int i = 0; i < an_minimumFunc.size() - 1; i++)
			cout << ImplicantToString(*an_minimumFunc[i], vars_names) << " + ";
		cout << ImplicantToString(*an_minimumFunc.back(), vars_names) << endl;
	}
}

void Boolean()
{
	cout << "Operators:\n";
	cout << "\tNOT: denoted as '-' or '~' or '!'\n";
	cout << "\tAND: denoted as '^' or '*' or '&' or '.'\n";
	cout << "\tOR: denoted as 'V' or 'v' or '|' or '+'\n";
	string expression;

	//cin.ignore(INT_MAX, '\n');
	getline(cin, expression);
	vector<char> variables_names;
	
	if (!preprocess(expression, variables_names))
	{
		cout << "Invalid character(s) in the function!\n";
		return;
	}
	if (variables_names.size() == 0)
	{
		cout << "Function has no variable!\n";
		return;
	}
	if (!invalid_check(expression))
	{
		cout << "Invalid expression!\n";
		return;
	}
	
	sort(variables_names.begin(), variables_names.end());
	unordered_map<char, int> variables_idx;
	for (int i = 0; i < variables_names.size(); i++)
		variables_idx[variables_names[i]] = i;

	string postfix_expr;
	postfix_convert(expression, postfix_expr);
	
	vector<VarsValue> minterms;
	find_minterms(postfix_expr, minterms, variables_idx);

	if (minterms.size() == pow(2, variables_names.size())) {
		cout << endl << endl << "=== Final result ===" << endl;
		cout << 1 << endl;
		return;
	}
	if (minterms.size() == 0) {
		cout << endl << endl << "=== Final result ===" << endl;
		cout << 0 << endl;
		return;
	}

	vector<Implicant> implicants(minterms.begin(), minterms.end());

	vector<Implicant> prime_implicants;
	find_prime_implicants(implicants, prime_implicants);
	print_implicants(prime_implicants, variables_names);

	vector<vector<Implicant*>> final_minimum_function;
	find_EPIs(prime_implicants, final_minimum_function);
	cout << endl << endl << "=== Final result ===" << endl;
	print_final_result(final_minimum_function, variables_names);

	return;
}



int main()
{
	Boolean();
	
	getchar();
	return 0;
}

