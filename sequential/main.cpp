#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unistd.h>
#include <fstream>
#include <string>

std::set<int> split_by_space_and_dedup(std::string s) {
	std::set<int> ret;
	int length, i, j;
	std::string number;
	
	length = s.length();
	i = 0;
	j = 0;
	while (j < length) {
		while (j < length && s[j] != ' ') j ++;
		number = s.substr(i, (j - i));
		ret.insert(stoi(number));
		j += 1;
		i = j;
	}
	return ret;
}

std::vector<std::set<int> > file_process(char *filename) {
	std::vector<std::set<int> > transactions;
	std::ifstream ifs;
	std::string buf;

	ifs.open(filename, std::ios::in);
	if (!ifs.is_open()) {
		std::cout << "fail to open file " << filename << std::endl;
		return transactions;
	}
	while (getline(ifs, buf)) {
		// std::cout << buf << std::endl;
		std::set<int> transaction = split_by_space_and_dedup(buf);
		transactions.push_back(transaction);
	}
	return transactions;
}

int main(int argc, char **argv) {
	int opt;
	extern char *optarg;
	extern int optind;
	char *filename;
	float min_support;
	std::vector<std::set<int> > transactions;
	
	filename = NULL;
	min_support = 0.15;

	while ((opt=getopt(argc, argv, "i:s:m:")) != EOF) {
		switch (opt) {
			case 'i': filename=optarg;
					  break;
			case 's': min_support=atof(optarg);
					  break;
		}
	}
	if (filename == 0) {
		std::cout << "error: use -i filename" << std::endl;
		return 0;
	}

	transactions = file_process(filename);
	sequential::apriori(transactions, min_support);
	return 0;
}