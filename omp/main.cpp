#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

double wtime(void) 
{
    double          now_time;
    struct timeval  etstart;
    struct timezone tzp;

    if (gettimeofday(&etstart, &tzp) == -1)
        perror("Error: calling gettimeofday() not successful.\n");

    now_time = ((double)etstart.tv_sec) +              /* in seconds */
               ((double)etstart.tv_usec) / 1000000.0;  /* in microseconds */
    return now_time;
}

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

std::vector<std::set<int> > file_process(char *filename, int cut_size) {
	std::vector<std::set<int> > transactions;
	std::ifstream ifs;
	std::string buf;

	ifs.open(filename, std::ios::in);
	if (!ifs.is_open()) {
		printf("fail to open file %s\n", filename);
		return transactions;
	}
	int cnt = 0;
	while (getline(ifs, buf)) {
		if (cnt == cut_size) break;
		cnt += 1;
		// std::cout << buf << std::endl;
		std::set<int> transaction = split_by_space_and_dedup(buf);
		transactions.push_back(transaction);
		// for (auto x : transaction) {
		// 	std::cout << x << " ";
		// }
		// std::cout << std::endl;
	}
	return transactions;
}

int main(int argc, char **argv) {
	int opt, cut_size;
	extern char *optarg;
	extern int optind;
	double timing, run_timing;
	char *filename;
	float min_support;
	std::vector<std::set<int> > transactions;
	
	filename = NULL;
	min_support = 0.15;
	cut_size = 100000000;

	while ((opt=getopt(argc, argv, "i:s:c:")) != EOF) {
		switch (opt) {
			case 'i': filename=optarg;
					  break;
			case 's': min_support=atof(optarg);
					  break;
			case 'c': cut_size=atoi(optarg);
					  break;
		}
	}
	if (filename == 0) {
		printf("error: use -i filename\n");
		return 0;
	}

	transactions = file_process(filename, cut_size);
	if (transactions.size() == 0) {
		return 0;
	} else {
		printf("start processing %d transactions...\n", int(transactions.size()));
		timing = wtime();
		sequential::apriori(transactions, min_support);
		run_timing = wtime() - timing;
		printf("Computation timing = %10.4f sec\n", run_timing);
	}
	return 0;
}