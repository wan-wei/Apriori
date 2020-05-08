#include "apriori.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace sequential {

	bool is_subset(std::vector<int> vec1, std::set<int> set2) {
		/* check if vec1 is the subset of set2 */
		for (unsigned int i = 0; i < vec1.size(); i ++) {
			if (set2.find(vec1[i]) == set2.end()) return false;
		}
		return true;
	}

	std::set<int> merge_set(std::vector<int> vec1, std::vector<int> vec2) {
		/* merge set1 with set2 */
		std::set<int> ret;
		ret.insert(vec1.begin(), vec1.end());
		ret.insert(vec2.begin(), vec2.end());
		return ret;
	}

	void print_freqset_info(std::map<std::vector<int>, float> freqset) {
		std::map<std::vector<int>, float>::iterator map_it;

		if (freqset.empty()) return;

		for (map_it = freqset.begin(); map_it != freqset.end(); map_it ++) {
			std::cout << "< ";
			for (unsigned int i = 0; i < map_it->first.size(); i ++) {
				std::cout << map_it->first[i] << " ";
			}
			std::cout << ">, support=" << map_it->second << std::endl;
		}
		return;
	}

	void print_candidates_info(std::vector<std::vector<int> > candidates) {
		int freq_k;

		if (candidates.empty()) return;
		freq_k = candidates.begin()->size();

		std::cout << "---- candidates of k=" << freq_k << " ----\n";
		for (unsigned int i = 0; i < candidates.size(); i ++) {
			std::cout << "< ";
			for (unsigned int j = 0; j < candidates[i].size(); j ++) {
				std::cout << candidates[i][j] << " ";
			}
		}
		return;
	}

	std::vector<std::vector<int> > generate_freqset(std::vector<std::vector<int> >candidates,
												  	std::vector<std::set<int> > transactions,
												  	float min_support,
												  	std::map<std::vector<int>, float>& ret)
	{
		std::vector<std::vector<int> > freqset;
		std::map<std::vector<int>, int>::iterator map_it;
		int t_size = transactions.size();
		int c_size = candidates.size();
		int *set_count = new int[c_size + 10];

		for (int i = 0; i < c_size; i ++) {
			set_count[i] = 0;
		}

		#pragma omp parallel for
		for (int i = 0; i < c_size; i ++) {
			std::vector<int> cand = candidates[i];
			#pragma omp parallel for
			for (int j = 0; j < t_size; j ++) {
				// if the candidate is the subset of transactions[i],
				// then update set_count by one.
				

				// ------------------- inline function --------------------
				bool f = true;
				for (unsigned int z = 0; z < cand.size(); z ++) {
					if (transactions[j].find(cand[z]) == transactions[j].end()) {
						f = false;
						break;
					}
				}
				if (f) {
				#pragma omp atomic
					set_count[i] += 1;
				}
				
				// ------------------- call function --------------------
				// if (is_subset(cand, transactions[j])) {
				// 	#pragma omp atomic
				// 	set_count[i] += 1;
				// }
			}
		}

		// calculate the support value and compare it with the min_support
		for (int i = 0; i < c_size; i ++) {
			float support_val = float(set_count[i]) / t_size;
			if (support_val >= min_support) {
				freqset.push_back(candidates[i]);
				ret[candidates[i]] = support_val;
			}
		}
		return freqset;
	}

	std::vector<std::vector<int> > generate_candidates(std::vector<std::vector<int> > freqset)
	{
		/* Generate candidate sets from freqset.
		   Go over freqset and merge any two sets from it together.
		   If the size of the merged set equals current freq_k + 1,
		   then consider it as the candidate.
		*/
		std::vector<std::vector<int> > candidates;
		// std::set<int> merged_set;
		std::set<std::set<int> > dup;
		unsigned int k, f_size;

		f_size = freqset.size();
		if (freqset.empty()) return candidates;
		k = freqset[0].size();

		// #pragma omp parallel for
		for (unsigned int i = 0; i < f_size; i ++) {
			for (unsigned int j = i + 1; j < f_size; j ++) {
				// ---------------- inline function -----------------
				// std::set<int> merged_set;
				// merged_set.insert(freqset[i].begin(), freqset[i].end());
				// merged_set.insert(freqset[j].begin(), freqset[j].end());

				std::set<int> merged_set = merge_set(freqset[i], freqset[j]);
				// #pragma omp critical
				if (merged_set.size() == k + 1 && dup.find(merged_set) == dup.end()) {
					candidates.push_back(std::vector<int>(merged_set.begin(), merged_set.end()));
					dup.insert(merged_set);
				}
			}
		}
		return candidates;
	}

	std::map<std::set<int>, float> apriori(std::vector<std::set<int> > transactions, float min_support)
	{

		std::map<std::set<int>, float> ret;
		std::map<std::vector<int>, float> ans;
		std::vector<std::vector<int> > candidates;
		std::set<int> unique_identifier;
		std::vector<std::vector<int> > freqset;
		int k;

		// ---- generate candidate set with one element ----
		for (unsigned int i = 0; i < transactions.size(); i ++) {
			std::set<int>::iterator it = transactions[i].begin();
			for (; it != transactions[i].end(); it ++) {
				int tmp[1] = {*it};
				if (unique_identifier.find(*it) == unique_identifier.end()) {
					unique_identifier.insert(*it);
					candidates.push_back(std::vector<int>(tmp, tmp + 1));
				}
			}
		}

		// ---- get one-freqset ----
		freqset = generate_freqset(candidates, transactions, min_support, ans);
		k = 1;

		// ---- generate frequent set ----
		while (!freqset.empty()) {
			printf("k=%d, freqset size=%d\n", k, int(freqset.size()));
			// print_freqset_info(freqset);
			candidates = generate_candidates(freqset);
			// print_candidates_info(candidates);
			freqset = generate_freqset(candidates, transactions, min_support, ans);
			k += 1;
		}
		printf("------- all freqset size=%d -------\n", int(ans.size()));
		print_freqset_info(ans);
		
		// move ans to ret
		for (std::map<std::vector<int>, float>::iterator it=ans.begin(); it != ans.end(); it ++) {
			std::set<int> key;
			key.insert(it->first.begin(), it->first.end());
			ret[key] = it->second;
		}
		return ret;
	}
}