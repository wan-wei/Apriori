#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

namespace sequential {

	std::vector<std::set<int> > apriori(std::vector<std::set<int> > transactions, float min_support, float min_confidence) {
		std::vector<std::set<int> > ret;

		for (int i = 0; i < transactions.size(); i ++) {
			std::cout <<"i=" <<i << std::endl;
			for (int j = 0; j < transactions[i].size(); j ++) {
				std::cout << j << " ";
			}
			std::cout << std::endl;
		}

		return ret;
	}
}