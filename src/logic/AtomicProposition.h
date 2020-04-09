#include "Variable.h"

#include <iostream>
#include <vector>
#include <set>
#include <limits>


#ifndef ATOMIC_PROPOSITION_H
#define ATOMIC_PROPOSITION_H


class AtomicProposition
{
public:
    struct PropPair
    {
        AtomicProposition* regular;
        AtomicProposition* notted;
    };

    ~AtomicProposition()=default;

    bool IsNot() const;
    bool SatisfiesClause() const;
    bool PresentInClause() const;
    long long GetName() const;
    AtomicProposition* GetInverse() const;
    bool VarIsSet() const;

    void Assert();
    void UnAssert();
    
    bool operator()(const AtomicProposition* lhs, const AtomicProposition* rhs) const;
    static AtomicProposition::PropPair CreatePropPair(long long var_name);
private:
    AtomicProposition(std::shared_ptr<Variable> var_, long long name_);
    AtomicProposition(std::shared_ptr<Variable> var_, long long name_, bool notted_);
    bool EvaluatesToTrue() const;

    const long long name;
    const std::shared_ptr<Variable> var;
    const bool notted;
    AtomicProposition* inverse;
};

typedef std::set<AtomicProposition*> PropSetRaw_t;
typedef std::set<std::unique_ptr<AtomicProposition>> PropSetUnique_t;

typedef std::vector<std::unique_ptr<AtomicProposition>> PropVecUnique_t;
typedef std::vector<AtomicProposition*> PropVecRaw_t;

typedef std::map<long long, std::unique_ptr<AtomicProposition>> PropMapUnique_t;
typedef std::map<long long, AtomicProposition*> PropMapRaw_t;

#endif