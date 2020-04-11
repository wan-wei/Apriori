/*
    apriori.h: header file for sequential version of apriori algorithm
*/
#include <algorithm>
#include <vector>
#include <set>

#ifndef APRIORI_H
#define APRIORI_H

namespace sequential
{

/**
 * @Simple implementation of the Apriori Algorithm
 * @param transactions List of transactions
 * @param min_support Minimum support value
 * @param min_confidence Minimum confidence value
 */
  std::vector<std::set<int> > apriori(std::vector<std::set<int> > transactions, float min_support, float min_confidence);
}

#endif