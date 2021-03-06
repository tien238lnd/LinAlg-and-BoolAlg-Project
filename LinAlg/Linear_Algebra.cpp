#include "Linear_Algebra.h"

extern unordered_map<char, Token> Vars_names;
extern char var_assign;


void solve_sysEquation(const string& s)
{
	string str = s;
	Lexer lex;
	init(lex, str);
	Token tok1, tok2;

	get_token(lex, tok1);
	if (tok1.type != TOK_MATRIX) {
		cout << "Wrong format!\n";
		return;
	}
	lex.pos++;
	get_token(lex, tok2);
	if (tok2.type != TOK_MATRIX && tok2.type != TOK_VECTOR) {
		cout << "Wrong format!\n";
		return;
	}

	Matrix mat;
	if (tok2.type == TOK_VECTOR)
		mat = Matrix(*(Vector*)tok2.valptr).Transpose();
	else mat = *(Matrix*)tok2.valptr;

	if (mat.NCols() != 1 || mat.NRows() != ((Matrix*)tok1.valptr)->NRows()) {
		cout << "Wrong matrix or vector!\n";
		return;
	}

	Solve(*(Matrix*)tok1.valptr, mat);
}

void invert(const string &str)
{
	Matrix mat = evaluate_string_2_matrix(str);
	if (mat.NRows() == 0) {
		cout << "This is not a matrix!\n";
		return;
	}
	mat = mat.Invert();
	if (mat.NRows() == 0) {
		cout << "Matrix is non invertable!\n";
		return;
	}
	Token tok;
	tok.type = TOK_MATRIX;
	tok.valptr = new Matrix(mat);
	Vars_names['_'] = tok;
	print_ans(tok);
	if (var_assign != '\0')
		Vars_names[var_assign] = tok;
}

void transpose(const string &str)
{
	Matrix mat = evaluate_string_2_matrix(str);
	if (mat.NRows() == 0) {
		cout << "This is not a matrix!\n";
		return;
	}
	mat = mat.Transpose();
	Token tok;
	tok.type = TOK_MATRIX;
	tok.valptr = new Matrix(mat);
	Vars_names['_'] = tok;
	print_ans(tok);
	if (var_assign != '\0')
		Vars_names[var_assign] = tok;
}

void echelon(const string &str)
{
	Matrix mat = evaluate_string_2_matrix(str);
	if (mat.NRows() == 0) {
		cout << "This is not a matrix!\n";
		return;
	}
	mat = mat.EchelonMatrix();
	Token tok;
	tok.type = TOK_MATRIX;
	tok.valptr = new Matrix(mat);
	Vars_names['_'] = tok;
	print_ans(tok);
	if (var_assign != '\0')
		Vars_names[var_assign] = tok;
}

void det(const string &str)
{
	Matrix mat = evaluate_string_2_matrix(str);
	if (mat.NRows() == 0) {
		cout << "This is not a matrix!\n";
		return;
	}
	double det = mat.Det();
	if (det == DBL_MAX) {
		cout << "Matrix is non square!\n";
		return;
	}
	Token tok;
	tok.type = TOK_NUM;
	tok.valptr = new double(det);
	Vars_names['_'] = tok;
	print_ans(tok);
	if (var_assign != '\0')
		Vars_names[var_assign] = tok;
}

void rank_matrix(const string &str)
{
	Matrix mat = evaluate_string_2_matrix(str);
	if (mat.NRows() == 0) {
		cout << "This is not a matrix!\n";
		return;
	}
	int rank = mat.Rank();
	Token tok;
	tok.type = TOK_NUM;
	tok.valptr = new double(rank);
	Vars_names['_'] = tok;
	print_ans(tok);
	if (var_assign != '\0')
		Vars_names[var_assign] = tok;
}

void algebra(const string &str)
{
	string s = str;
	Lexer lexer_expression;
	init(lexer_expression, s);
	int check_ans = calculate_infix(Vars_names['_'], lexer_expression);

	switch (check_ans)
	{
	case 1:
		cout << "Invalid character or misspelling!\n"; break;
	case 2:
		cout << "Variable not found!\n"; break;
	case 3:
		cout << "Operator error!\n"; break;
	case 4:
		cout << "Dimension error!\n"; break;
	case 5:
		cout << "Syntax error!\n"; break;
	case 6:
		cout << "Division by zero!\n"; break;
	default:
	{
		print_ans(Vars_names['_']);
		if (var_assign != '\0')
			Vars_names[var_assign] = Vars_names['_'];
	}
	}
}
