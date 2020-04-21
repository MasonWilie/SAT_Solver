#ifndef ATOMIC_PROPOSITION_H
#define ATOMIC_PROPOSITION_H

#include "Variable.h"

#include <iostream>
#include <vector>
#include <set>
#include <limits>

class AtomicProposition
{
public:
    struct PropPair
    {
        AtomicProposition *regular;
        AtomicProposition *notted;
    };

    ~AtomicProposition() = default;

    bool IsNot() const;
    bool SatisfiesClause() const;
    bool PresentInClause() const;
    int GetName() const;
    AtomicProposition *GetInverse() const;
    bool IsAsserted() const;

    void Assert();
    void UnAssert();

    bool operator()(const AtomicProposition *lhs, const AtomicProposition *rhs) const;
    static AtomicProposition::PropPair CreatePropPair(int var_name);

private:
    AtomicProposition(std::shared_ptr<Variable> var_, int name_);
    AtomicProposition(std::shared_ptr<Variable> var_, int name_, bool notted_);
    bool EvaluatesToTrue() const;

    const std::shared_ptr<Variable> var;
    const int name;
    const bool notted;
    AtomicProposition *inverse;

    bool asserted = false;
};

typedef std::set<AtomicProposition *> PropSetRaw_t;
typedef std::set<std::unique_ptr<AtomicProposition>> PropSetUnique_t;

typedef std::vector<std::unique_ptr<AtomicProposition>> PropVecUnique_t;
typedef std::vector<AtomicProposition *> PropVecRaw_t;

typedef std::map<int, std::unique_ptr<AtomicProposition>> PropMapUnique_t;
typedef std::map<int, AtomicProposition *> PropMapRaw_t;

#endif