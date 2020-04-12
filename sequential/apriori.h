/*
    apriori.h: header file for sequential version of apriori algorithm
*/
#include <algorithm>
#include <vector>
#include <set>
#include <map>

#ifndef APRIORI_H
#define APRIORI_H

namespace sequential
{

/**
 * @Simple implementation of the Apriori Algorithm
 * @param transactions List of transactions, each transaction contains unique indexes
 * @param min_support Minimum support value
 * @param min_confidence Minimum confidence value
 */
  std::map<std::set<int>, float> apriori(std::vector<std::set<int> > transactions, float min_support);
}

#endif