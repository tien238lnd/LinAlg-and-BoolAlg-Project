#include "Find_EPIs.h"

bool check_cover(const vector<Implicant*> &subSet_of_remainImps, const unordered_set<int> &uncovered_minterms)
{	// hàm kiểm tra một tập con có cover hết các uncovered_minterms không
	unordered_set<int> set_minterms_of_remainImps;
	for (const auto &im : subSet_of_remainImps)
		if (im != NULL)
			set_minterms_of_remainImps.insert(im->minterms_idx.begin(), im->minterms_idx.end());

	if (set_minterms_of_remainImps == uncovered_minterms)
		return 1;
	else return 0;
}

bool filter(vector<Implicant*> &full_cover_EPIs, const unordered_set<int> &uncovered_minterms, vector<vector<Implicant*>> &multi_remain_EPIs)
{	// hàm đệ quy duyệt qua các tập con của full_cover_EPIs
	// tập con của full_cover_EPIs sinh bởi việc loại/ko loại ptử [k]
	// return 0 nghĩa là ko tìm được tập con nào (ko kể chính tập đó) cover được, return 1 nghĩa là tìm được
	static int k = 0;
	static int size = full_cover_EPIs.size();	// size này là size của tập con, vì ta ko xóa mà chỉ gán ptử bằng NULL nên phải lưu size lại
	static int min_size = size;	// cờ min_size để ghi nhớ lại tập con nhỏ nhất đã tìm được
	if (k == full_cover_EPIs.size())
		return 0;

	bool found = 0;
	Implicant* imp = full_cover_EPIs[k];
	full_cover_EPIs[k] = NULL; size--;	// thay vì loại ptử [k], ta sẽ gán nó bằng NULL
	k++;
	// nếu cover được thì đi xuống duyệt tiếp các tập con của tập đó
	if (check_cover(full_cover_EPIs, uncovered_minterms)) {
		// nếu ở dưới không còn tập con nào phủ được thì lấy tập hiện tại
		if (size == 1 || filter(full_cover_EPIs, uncovered_minterms, multi_remain_EPIs) == 0)
			if (size <= min_size) {
				if (size < min_size) {
					multi_remain_EPIs.clear();
					min_size = size;
				}
				multi_remain_EPIs.push_back(full_cover_EPIs);
			}
		found = 1;
	}
	full_cover_EPIs[k - 1] = imp; size++;	// điền ptử [k] vào lại và duyệt tiếp
	found = filter(full_cover_EPIs, uncovered_minterms, multi_remain_EPIs) || found;
	k--;
	return found;
	// cuối cùng thì full_cover_EPIs vẫn ko bị thay đổi, và ko bao giờ check_cover chính nó
}

int total_bitcount(const vector<Implicant*> &imps)
{
	int result = 0;
	for (const auto &imp : imps)
		if (imp != NULL)
			result += imp->bit01count;
	return result;
}

void find_EPIs(vector<Implicant> &prime_implicants, vector<vector<Implicant*>> &final_minimum_function)
{

	unordered_set<Implicant*> primeImps_set; // set này chỉ chứa địa chỉ của các PI trong vector prime_implicants
	for (Implicant &i : prime_implicants)
		primeImps_set.insert(&i);


	unordered_map<int, Implicant*> covered_minterms;
	unordered_set<int> uncovered_minterms;

	// tìm covered_minterms ở bước này chính là tìm các cột chỉ được cover bằng 1 imp 
	for (Implicant &i : prime_implicants)
		for (int minterm_idx : i.minterms_idx)
			if (covered_minterms.count(minterm_idx) == 0)
				covered_minterms[minterm_idx] = &i;
			else {
				covered_minterms[minterm_idx] = NULL;
				uncovered_minterms.insert(minterm_idx);
			}

	// lọc ra EPIs lần đầu, và xóa chúng khỏi primeImps_set: EPI chính là các PI cover được các cột minterms vừa tìm ở trên
	unordered_set<Implicant*> first_filter_EPIs;
	for (const auto &covered_minterm : covered_minterms)
		if (covered_minterm.second != NULL) {
			primeImps_set.erase(covered_minterm.second);
			first_filter_EPIs.insert(covered_minterm.second);
		}

	// xóa các cột sẽ được cover bởi EPIs vừa được lọc ra
	for (auto &epi : first_filter_EPIs)
		for (auto k : epi->minterms_idx)
			uncovered_minterms.erase(k);	// nếu ko có để xóa thì cũng ko sao

	if (uncovered_minterms.size() == 0)	// tất cả các minterm đã  đều được cover hết
	{
		final_minimum_function.push_back(vector<Implicant*>(first_filter_EPIs.begin(), first_filter_EPIs.end()));
		return;
	}

	// reduce: xóa các phần tử trong mảng imp.minterms_idx tương ứng với cột đã bị xóa
	for (auto im = primeImps_set.begin(); im != primeImps_set.end(); )
	{
		for (int i = 0; i < (*im)->minterms_idx.size(); )
			if (uncovered_minterms.count((*im)->minterms_idx[i]) == 0)	// ko có trong uncovered_minterms, tức đã bị xóa
				(*im)->minterms_idx.erase((*im)->minterms_idx.begin() + i);
			else i++;
		if ((*im)->minterms_idx.size() == 0)	// nếu PI nào có mảng minterms_idx đã bị xóa hết thì loại bỏ PI đó luôn
			primeImps_set.erase(im++);
		else im++;
	}
	
	// Lọc ra EPIs lần 2: ý tưởng là duyệt qua các tập con của primeImps_set, kiểm tra xem các tập con đó có cover hết uncovered_minterms ko, nếu được thì lấy.
	// Ở lần lọc này, có thể sẽ có thể kết quả thỏa mãn. Vì đề yêu cầu trả về tất cả đa thức tối tiểu khả dĩ, nên phải lưu lại hết các kết quả
		

	vector<vector<Implicant*>> multi_remain_EPIs;	// mỗi vector<Implicant*> là một tập con các PI của primeImps_set
													
	vector<Implicant*> full_cover_remain_EPIs(primeImps_set.begin(), primeImps_set.end());	// cái này ko bao h có size = 1 hết, hoặc 0 (đã return), hoặc 2 trở lên

	filter(full_cover_remain_EPIs, uncovered_minterms, multi_remain_EPIs);

	if (multi_remain_EPIs.size() == 0)	// ko tìm được tập con nào cover được hết, bèn phải đem tất cả primeImps_set vào final_minimum_function
	{
		final_minimum_function.push_back(vector<Implicant*>(first_filter_EPIs.begin(), first_filter_EPIs.end()));
		final_minimum_function[0].insert(final_minimum_function[0].end(), primeImps_set.begin(), primeImps_set.end());
		return;
	}

	// xóa NULL đi
	for (auto &EPIs : multi_remain_EPIs)
		while (true) {
			auto where_NULL = find(EPIs.begin(), EPIs.end(), (Implicant*)NULL);
			if (where_NULL != EPIs.end())
				EPIs.erase(where_NULL);
			else break;
		}
	// lọc lại lần cuối tìm các tập con có tổng số từ đơn (số bits) nhỏ nhất
	vector<vector<Implicant*>> multi_remain_EPIs_last_filter;
	int min_total_bit_count = INT32_MAX;
	for (const auto &EPIs : multi_remain_EPIs)
	{
		int it_min = total_bitcount(EPIs);
		if (it_min <= min_total_bit_count) {
			if (it_min < min_total_bit_count) {
				multi_remain_EPIs_last_filter.clear();
				min_total_bit_count = it_min;
			}
			multi_remain_EPIs_last_filter.push_back(EPIs);
		}
	}
	// ghép với EPIs tìm được ở trên để tạo thành đa thức cuối cùng
	for (int i = 0; i < multi_remain_EPIs_last_filter.size(); i++)
	{
		final_minimum_function.push_back(vector<Implicant*>(first_filter_EPIs.begin(), first_filter_EPIs.end()));
		final_minimum_function[i].insert(final_minimum_function[i].end(),
			multi_remain_EPIs_last_filter[i].begin(), multi_remain_EPIs_last_filter[i].end());
	}
	return;
}