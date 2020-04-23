#ifndef CLAUSE_H
#define CLAUSE_H

#include "Literal.h"

#include <algorithm>


class Clause
{
public:
    Clause()=default;

    void AddLiteral(Literal* lit);
    bool ContainsLiteral(Literal* lit) const;

    size_t Size() const;
    bool Satisfied() const;
    bool False() const;

    std::set<int> GetLitsAsInts() const;
    LitSetRaw_t GetLitsAsPtrs() const;

private:
    LitSetRaw_t literals;
};

// Types

typedef std::set<std::unique_ptr<Clause>> ClauseSetUnique_t;
typedef std::set<Clause*> ClauseSetRaw_t;
#endif