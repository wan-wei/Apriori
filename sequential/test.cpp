#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

int main() {
	std::vector<std::set<int> > transactions;
	for (int i = 0; i < 5; i ++) {
		std::set<int> tmp;
		for (int j = 0; j < 3; j ++) {
			tmp.insert(j);
		}
		transactions.push_back(tmp);
	}
	sequential::apriori(transactions, 0.1, 0.2);
	return 0;
}