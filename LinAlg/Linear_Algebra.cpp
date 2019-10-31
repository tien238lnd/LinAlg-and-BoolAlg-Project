#include "Linear_Algebra.h"

extern unordered_map<char, Token> Vars_names;
extern char var_assign;


void solve_sysEquation(string &str)
{
	str.erase(str.find(","), 1);
	Lexer lex;
	init(lex, str);
	vector<Token> tokens;
	while (true)
	{
		Token tok;
		get_token(lex, tok);
		if (tok.type == TOK_EOL)
			break;
		if (tok.type == TOK_ERROR1) {
			cout << "Invalid character or misspelling!\n"; break;
		}
		if (tok.type == TOK_ERROR2) {
			cout << "Variable not found!\n"; break;
		}
		tokens.push_back(tok);
	}
	if (tokens.size() != 2 || tokens[0].type != TOK_MATRIX || (tokens[1].type != TOK_VECTOR && tokens[1].type != TOK_MATRIX)) {
		cout << "Wrong expression!\n";
		return;
	}
	if (tokens[1].type == TOK_MATRIX)
		Solve(*(Matrix*)tokens[0].valptr, *(Matrix*)tokens[1].valptr);
	else {
		Matrix mat = Matrix(*(Vector*)tokens[1].valptr).Transpose();
		Solve(*(Matrix*)tokens[0].valptr, mat);
	}
}

void invert(string &str)
{
	Matrix mat(str);
	if (mat.NRows() < 2) {
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

void transpose(string &str)
{
	Matrix mat(str);
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

void echelon(string &str)
{
	Matrix mat(str);
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

void det(string &str)
{
	Matrix mat(str);
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

void rank_matrix(string &str)
{
	Matrix mat(str);
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

void algebra(string &str)
{
	Lexer lexer_expression;
	init(lexer_expression, str);
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
