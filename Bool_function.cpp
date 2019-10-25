#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef vector<bool> VarsValue;

class Implicant
{
public:
	vector<char> bits;
	int bit1count;
	vector<int> minterms_idx;
	bool is_prime = 1;
	int size() const { return bits.size(); }
	char operator[](int n) const { return bits[n]; }

	Implicant(const VarsValue &minterm) {
		bits.resize(minterm.size());
		bit1count = 0;
		minterms_idx.resize(1, 0);
		for (int i = 0; i < bits.size(); i++) {
			bits[i] = minterm[i];
			if (bits[i] == 1) {
				bit1count++;
				minterms_idx[0] += pow(2, bits.size() - 1 - i);
			}
		}
	}

	Implicant(const Implicant &i1, const Implicant &i2) {
		bits.resize(i1.size());
		bit1count = 0;
		minterms_idx = i1.minterms_idx;
		minterms_idx.insert(minterms_idx.end(), i2.minterms_idx.begin(), i2.minterms_idx.end());

		for (int i = 0; i < bits.size(); i++) {
			bits[i] = i1.bits[i] == i2.bits[i] ? i1.bits[i] : -1;
			if (bits[i] == 1)
				bit1count++;
		}
	}

};

int is_combinable_or_same(const Implicant &i1, const Implicant &i2)
{	// 0 = not combinable, 1 = combinable, 2 = same
	if (abs(i1.bit1count - i2.bit1count) > 1)
		return 0;
	int differ = 0;
	for (int i = 0; i < i1.size(); i++) {
		differ += i1[i] != i2[i];
		if (differ > 1)
			return 0;
	}
	if (differ == 1)
		return 1;
	else return 2;	// differ = 0
}

string ImplicantToString(const Implicant &imp, const vector<char> &vars_names)
{
	string result;
	for (int i = 0; i < imp.size(); i++)
	{
		if (imp.bits[i] == 1) {
			result += vars_names[i];
			result += ".";
		}
		else if (imp.bits[i] == 0) {
			result += "!";
			result += vars_names[i];
			result += ".";
		}
	}
	result.pop_back();
	return result;
}

bool preprocess(string &expression, vector<char> &variables_names)
{	// xoa cac khoang trang va chuyen cac phep toan co nhieu cach viet ve 1 cach viet
	// lay danh sach bien
	// kiem tra ky tu ko cho phep
    for (int i = 0; i < expression.length(); i++)	// để rảnh thử sửa lại for (char c : expression) xem sao
        switch (expression[i]) {
        case ' ':
			expression.erase(i, 1); i--; break;
		case '-': case '~':
            expression[i] = '!'; break;
		case 'V': case 'v': case '|':
			expression[i] = '+'; break;
		case '^': case '*': case '&':
			expression[i] = '.'; break;
		case '!': case '+': case '.': case '(': case ')': case '0': case '1':
			break;
        default:
			if ('a' <= expression[i] && expression[i] <= 'z' || 'A' <= expression[i] && expression[i] <= 'Z')
			{    // kiểm tra xem biến đã xuất hiện trước đó chưa
				if (find(variables_names.begin(), variables_names.end(), expression[i]) == variables_names.end())
					variables_names.push_back(expression[i]);
			}
            else return 0;	// invalid character(s) in the function
        }
	// find duplicated NOT '!!'
	for (int i = 0; i < expression.length() - 1; i++)
		if (expression[i] == '!' && expression[i + 1] == '!')
			expression.erase(i--, 2);
    return 1;
}

bool invalid_check(const string &expression)
{
	if (expression[0] == '.' || expression[0] == '+' || expression[0] == ')')
		return 0;
	if (expression.back() == '.' || expression.back() == '+' || expression.back() == '(' || expression.back() == '!')
		return 0;
	int open_parenthesis_count = 0;
	for (int i = 0; i < expression.length() - 1; i++)
		switch (expression[i]) {
		case '.': case '+': case '!':
			switch (expression[i + 1]) {
			case ')': case '+': case '.':
				return 0;
			}
			break;
		case '(':
			switch (expression[i + 1]) {
			case '+': case '.':
				return 0;
			}
			open_parenthesis_count++; break;
		case ')':
			if (open_parenthesis_count == 0)
				return 0;
			switch (expression[i + 1]) {
			case '.': case '+': case ')':
				open_parenthesis_count--; break;
			default:
				return 0;
			}
			break;
		default:
			switch (expression[i + 1]) {
			case '.': case '+': case ')':
				break;
			default:
				return 0;
			}
		}
	if (open_parenthesis_count > 1 || (open_parenthesis_count == 0) == (expression.back() == ')'))
		return 0;
	return 1;
}

int priority(char my_operator)
{
	switch (my_operator) {
	case '!':
		return 3;
	case '.':
		return 2;
	case '+':
		return 1;
	case '(':
		return 0;
	}
}

void postfix_convert(const string &expression, string &postfix_expr)
{	// Su dung thuat toan Shunting-yard algorithm
	stack<char> operator_stack;
	for (char c: expression)
		switch (c)
		{
		case '(':
			operator_stack.push(c); break;
		case ')':
			while (true) {
				char x = operator_stack.top();
				operator_stack.pop();
				if (x != '(')
					postfix_expr += x;
				else break;
			}
			break;
		case '!': case '.': case '+':
			while (!operator_stack.empty() && priority(c) < priority(operator_stack.top())) {
				postfix_expr += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.push(c);
			break;
		default: // variables(also operands)
			postfix_expr += c;
		}
	while (!operator_stack.empty()) {
		postfix_expr += operator_stack.top();
		operator_stack.pop();
	}
}

bool calculate(const string &postfix_expr, const VarsValue &vars_value, unordered_map<char, int> &vars_idx)
{	// hàm này cần được tối ưu kỹ vì sẽ được gọi 2^n lần
	stack<bool> value;
	bool x, y;
	for (char c : postfix_expr)
		switch (c) {
		case '!':
			value.top() = !value.top(); break;
		case '.':
			x = value.top(); value.pop();
			y = value.top(); value.pop();
			value.push(x && y); break;
		case '+':
			x = value.top(); value.pop();
			y = value.top(); value.pop();
			value.push(x || y); break;
		case '0': case '1':
			value.push(c == '1'); break;
		default:
			value.push(vars_value[vars_idx[c]]);	// lấy bit thứ vars_idx[c] của int?
		}
	//if (value.size() != 1)
	//	return -1;
	return value.top();
}

void print_minterm(const VarsValue &minterm)
{
	for (bool x : minterm)
		cout << x;
}

void print_minterms(const vector<VarsValue> &minterms)
{
	for (VarsValue minterm : minterms)
	{
		for (bool x : minterm)
			cout << x;
		cout << endl;
	}
}

void generate_evaluate(VarsValue &vars_value, int n, const string &postfix_expr, vector<VarsValue> &minterms, unordered_map<char, int> &vars_idx)
{	// hàm đệ quy này ko có biến cục bộ, giảm thiểu nguy cơ stackoverflow
	if (n == vars_value.size()) {
		//print_minterm(vars_value);
		//cout << "\t:\t" << calculate(postfix_expr, vars_value, vars_idx) << endl;
		if (calculate(postfix_expr, vars_value, vars_idx) == 1)
			minterms.push_back(vars_value);
		return;
	}
	vars_value[n] = 0;
	generate_evaluate(vars_value, n + 1, postfix_expr, minterms, vars_idx);
	vars_value[n] = 1;
	generate_evaluate(vars_value, n + 1, postfix_expr, minterms, vars_idx);
}

void find_minterms(const string &postfix_expr, vector<VarsValue> &minterms, unordered_map<char, int> &vars_idx)
{
	VarsValue vars_value(vars_idx.size());
	generate_evaluate(vars_value, 0, postfix_expr, minterms, vars_idx);
}

void find_prime_implicants(vector<Implicant> &implicants, vector<Implicant> &prime_implicants)
{
	vector<Implicant> higher_order_implicants;
	while (true)
	{
		for (int i = 0; i < implicants.size() - 1; i++)
			for (int j = i + 1; j < implicants.size(); j++)
			{
				int check = is_combinable_or_same(implicants[i], implicants[j]);
				switch (check) {
				case 1:
					higher_order_implicants.push_back(Implicant(implicants[i], implicants[j]));
					implicants[i].is_prime = implicants[j].is_prime = 0;

					break;
				case 2:
					implicants.erase(implicants.begin() + j);
					j--; break;
				}
			}
		if (higher_order_implicants.size() != 0) {
			for (auto i : implicants)
				if (i.is_prime == 1)
					prime_implicants.push_back(i);
			implicants = higher_order_implicants;
			higher_order_implicants.clear();
		}
		else {
			prime_implicants.insert(prime_implicants.end(), implicants.begin(), implicants.end());
			break;
		}
	}
}

void print_implicants(const vector<Implicant> &implicants, const vector<char> &vars_names)
{
	for (auto im : implicants)
	{
		for (int x : im.minterms_idx)
			cout << x << ", ";
		cout << "\t:\t";
		for (char x : im.bits)
			if (x == -1)
				cout << "_";
			else cout << (int)x;
		cout << "\t:\t" << ImplicantToString(im, vars_names) <<endl;
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
	cout << postfix_expr << endl;
	vector<VarsValue> minterms;
	find_minterms(postfix_expr, minterms, variables_idx);
	print_minterms(minterms);

	
	vector<Implicant> implicants(minterms.begin(), minterms.end());
	vector<int> all_minterms_idx;
	for (auto imp : implicants)
		all_minterms_idx.push_back(imp.minterms_idx[0]);

	vector<Implicant> prime_implicants;
	find_prime_implicants(implicants, prime_implicants);
	print_implicants(prime_implicants, variables_names);
	// các minterms đang ở dạng vector bool
	// B1: chia các minterms lại theo nhóm. Mà ko cần chia cũng được.	DONE
	//		đếm số bit 1, 
	// B2: ghép lại thành implicants bậc cao hơn						DONE
	//		for lồng for qua các cặp nhóm, tìm cặp minterms chỉ sai khác 1 bit
	// B3: tới khi ko ghép đc nữa thì chúng chính là prime implicants	DONE
	// B4: lập bảng để chọn được essential prime implicants (EPI)
	//		
	//	    chuyển EPI thành biểu thức biến, - là ko có, 1 là x, 0 là !x
	unordered_set<int> allMintermsid_set(all_minterms_idx.begin(), all_minterms_idx.end());
	//unordered_set<Implicant> primeImps_set(prime_implicants.begin(), prime_implicants.end());
	return;


}



void LinAlg()
{
    
}

int main()
{
    /*cout << "\t1: Minimize Boolean function.\n";
    cout << "\t2: Linear algebra.\n";
    cout << "\tAny key else: Exit.\n";
    cout << "Choose a function of program: ";
	int choose;*/
	vector<int> a{ 1,2,3,4,5,6,7,8,9,0 };
	unordered_set<int> b(a.begin(), a.end());
	auto x = b.begin();
	while (x != b.end())
	{
		if (*x % 2 == 0)
			b.erase(x++);	// x++ trước để qua được next iter đã, rồi mới xóa x cũ
		else x++;
	}
	/*for (auto x = b.begin(); x != b.end(); x++)
	{
		if (*x % 2 == 0)
			b.erase(x);
	}*/

	Boolean();
    /*if (choose == 1)
    {
        Boolean();
    }
    else if (choose == 2)
    {
        LinAlg();
    }*/
	return 0;
}

