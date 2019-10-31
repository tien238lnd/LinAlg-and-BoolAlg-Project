#include "Bool_Minimize.h"

void print_final_result(const vector<vector<Implicant*>> &final_result, const vector<char> &vars_names)
{
	for (const auto &an_minimumFunc : final_result)
	{
		for (int i = 0; i < an_minimumFunc.size() - 1; i++)
			cout << ImplicantToString(*an_minimumFunc[i], vars_names) << " + ";
		cout << ImplicantToString(*an_minimumFunc.back(), vars_names) << endl;
	}
}

void bool_minimize(const string &expression)
{
	string tempExpression = expression;
	vector<char> variables_names;
	
	if (!preprocess(tempExpression, variables_names))
	{
		cout << "Invalid character(s) in the function!\n";
		return;
	}
	if (variables_names.size() == 0)
	{
		cout << "Function has no variable!\n";
		return;
	}
	if (variables_names.size() > 25)
	{
		cout << "Function must have not over 25 variables.\n";
		return;
	}
	if (!invalid_check(tempExpression))
	{
		cout << "Invalid expression!\n";
		return;
	}
	
	sort(variables_names.begin(), variables_names.end());
	unordered_map<char, int> variables_idx;
	for (int i = 0; i < variables_names.size(); i++)
		variables_idx[variables_names[i]] = i;

	string postfix_expr;
	postfix_convert(tempExpression, postfix_expr);
	
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
	if (variables_names.size() <= 7)
		print_implicants(prime_implicants, variables_names);

	vector<vector<Implicant*>> final_minimum_function;
	find_EPIs(prime_implicants, final_minimum_function);
	cout << endl << endl << "=== Final result ===" << endl;
	print_final_result(final_minimum_function, variables_names);

	return;
}


