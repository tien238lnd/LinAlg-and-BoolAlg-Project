#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

typedef vector<bool> VarsValue;

class Implicant
{
public:
	vector<char> bits;			// các bits của implicant, không phải là vector<bool> vì bit có thể là _ chứ không phải chỉ 0/1
	int bit1count;				
	int bit01count;
	vector<int> minterms_idx;	// danh sách các minterm được cover bởi implicant đó
	bool is_prime = 1;
	//int size() const { return bits.size(); }
	//char operator[](int &n) const { return bits[n]; }

	// khởi tạo implicant bậc 0 từ minterm
	Implicant(const VarsValue &minterm) {
		bits.resize(minterm.size());
		bit1count = 0; bit01count = minterm.size();
		minterms_idx.resize(1, 0);
		for (int i = 0; i < bits.size(); i++) {
			bits[i] = minterm[i];
			if (bits[i] == 1) {
				bit1count++;
				minterms_idx[0] += pow(2, bits.size() - 1 - i);
			}
		}
	}
	// khởi tạo 1 implicant bậc cao hơn từ 2 implicants i1 và i2 (chúng đã được đảm bảo là ghép đôi được)
	Implicant(const Implicant &i1, const Implicant &i2) {
		bits.resize(i1.bits.size());
		bit1count = min(i1.bit1count, i2.bit1count);
		bit01count = i1.bit01count - 1;	// i1.bit01count luôn bằng i2.bit01count
		minterms_idx = i1.minterms_idx;
		minterms_idx.insert(minterms_idx.end(), i2.minterms_idx.begin(), i2.minterms_idx.end());

		for (int i = 0; i < bits.size(); i++)
			bits[i] = i1.bits[i] == i2.bits[i] ? i1.bits[i] : -1;
	}
};

// hàm trả về dạng đơn thức của implicant
string ImplicantToString(const Implicant &imp, const vector<char> &vars_names);

// hàm tìm minterm từ biểu thức hàm bool (dạng hậu tố)
void find_minterms(const string &postfix_expr, vector<VarsValue> &minterms, unordered_map<char, int> &vars_idx);

// hàm tìm prime implicants từ các implicant bậc 0
void find_prime_implicants(vector<Implicant> &implicants, vector<Implicant> &prime_implicants);

// hàm in ra các implicant kèm theo dạng đơn thức của nó
void print_implicants(const vector<Implicant> &implicants, const vector<char> &vars_names);

