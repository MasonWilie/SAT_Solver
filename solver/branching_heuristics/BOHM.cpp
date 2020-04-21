#include "BOHM.h"

/**
 * @brief Return the maximum value between two ints
 * 
 * @param a 
 * @param b 
 * @return int Maximum of a and b
 */
int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/**
 * @brief Return the minimum value between two ints
 * 
 * @param a 
 * @param b 
 * @return int Minimum of a and b
 */
int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

BohmsBranching::BohmsBranching(int num_vars,
                               PropMapUnique_t &prop_map_unique)
    : MaxMinClauseHeuristic(num_vars, prop_map_unique)
{
}

int BohmsBranching::Score(std::pair<int, int> prop_pair) const
{
    return alpha * max(prop_pair.first, prop_pair.second) + beta * min(prop_pair.first, prop_pair.second);
}