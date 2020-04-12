#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

std::set<int> generate_set(int* list, int size) {
	std::set<int> ret;
	for (int i = 0; i < size; i ++) {
		ret.insert(list[i]);
	}
	return ret;
}

std::vector<std::set<int> > generate_transactions() {
	std::vector<std::set<int> > ret;
	int t1[4] = {1,2,3,4};
	int t2[3] = {1,2,3};
	int t3[2] = {1,2};
	int t4[2] = {1,5};
	int t5[4] = {6,2,3,4};
	int t6[3] = {6,2,3};
	int t7[2] = {6,2};
	int t8[2] = {6,5};
	ret.push_back(generate_set(t1, 4));
	ret.push_back(generate_set(t2, 3));
	ret.push_back(generate_set(t3, 2));
	ret.push_back(generate_set(t4, 2));
	ret.push_back(generate_set(t5, 4));
	ret.push_back(generate_set(t6, 3));
	ret.push_back(generate_set(t7, 2));
	ret.push_back(generate_set(t8, 2));
	return ret;
}

int main() {
	std::vector<std::set<int> > transactions = generate_transactions();
	float min_support = 0.15;
	sequential::apriori(transactions, min_support);
	return 0;
}