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
	vector<char> bits;
	int bit1count;
	int bit01count;
	vector<int> minterms_idx;
	bool is_prime = 1;
	int size() const { return bits.size(); }
	char operator[](int n) const { return bits[n]; }

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

	Implicant(const Implicant &i1, const Implicant &i2) {
		bits.resize(i1.size());
		bit1count = min(i1.bit1count, i2.bit1count);
		bit01count = i1.bit01count - 1;	// i1.bit01count luôn bằng i2.bit01count
		minterms_idx = i1.minterms_idx;
		minterms_idx.insert(minterms_idx.end(), i2.minterms_idx.begin(), i2.minterms_idx.end());

		for (int i = 0; i < bits.size(); i++)
			bits[i] = i1.bits[i] == i2.bits[i] ? i1.bits[i] : -1;
	}
};

string ImplicantToString(const Implicant &imp, const vector<char> &vars_names);

void find_minterms(const string &postfix_expr, vector<VarsValue> &minterms, unordered_map<char, int> &vars_idx);

void find_prime_implicants(vector<Implicant> &implicants, vector<Implicant> &prime_implicants);

void print_implicants(const vector<Implicant> &implicants, const vector<char> &vars_names);