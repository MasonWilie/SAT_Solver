#ifndef CLAUSE_H
#define CLAUSE_H

#include "AtomicProposition.h"

#include <algorithm>


class Clause
{
public:
    Clause()=default;

    void AddProposition(AtomicProposition* prop);
    bool ContainsProposition(AtomicProposition* prop) const;

    size_t Size() const;
    bool Satisfied() const;
    bool False() const;

    std::set<int> GetPropsAsInts() const;

private:
    PropSetRaw_t propositions;
};

// Types

typedef std::set<std::unique_ptr<Clause>> ClauseSetUnique_t;
typedef std::set<Clause*> ClauseSetRaw_t;
#endif