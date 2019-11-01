#pragma once

#include <unordered_set>
#include "Find_Prime_Implicants.h"

// hàm tìm Essential Prime Implicants từ Prime Implicants
void find_EPIs(vector<Implicant> &prime_implicants, vector<vector<Implicant*>> &final_minimum_function);
