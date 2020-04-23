#ifndef LITERAL_H
#define LITERAL_H

#include "Variable.h"

#include <iostream>
#include <vector>
#include <set>
#include <limits>

class Literal
{
public:
    struct LitPair
    {
        Literal *regular;
        Literal *notted;
    };

    ~Literal() = default;

    bool IsNot() const;
    bool SatisfiesClause() const;
    bool PresentInClause() const;
    int GetName() const;
    Literal *GetInverse() const;
    bool IsAsserted() const;

    void Assert();
    void UnAssert();

    bool operator()(const Literal *lhs, const Literal *rhs) const;
    static Literal::LitPair CreateLitPair(int var_name);

private:
    Literal(std::shared_ptr<Variable> var_, int name_);
    Literal(std::shared_ptr<Variable> var_, int name_, bool notted_);
    bool EvaluatesToTrue() const;

    const std::shared_ptr<Variable> var;
    const int name;
    const bool notted;
    Literal *inverse;

    bool asserted = false;
};

typedef std::set<Literal *> LitSetRaw_t;
typedef std::set<std::unique_ptr<Literal>> LitSetUnique_t;

typedef std::map<int, std::unique_ptr<Literal>> LitMapUnique_t;
typedef std::map<int, Literal *> LitMapRaw_t;

#endif