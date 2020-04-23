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
                               LitMapUnique_t &lit_map_unique)
    : MaxMinClauseHeuristic(num_vars, lit_map_unique)
{
}

int BohmsBranching::Score(std::pair<int, int> lit_pair) const
{
    return alpha * max(lit_pair.first, lit_pair.second) + beta * min(lit_pair.first, lit_pair.second);
}