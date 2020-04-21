#include "Clause.h"


void Clause::AddProposition(AtomicProposition* prop)
{
    propositions.insert(prop);
}

bool Clause::ContainsProposition(AtomicProposition* prop) const
{
    return prop->PresentInClause() && std::find(propositions.begin(), propositions.end(), prop) != propositions.end();
}

size_t Clause::Size() const
{
    size_t size{0};
    for (auto p : propositions)
    {
        if (p->PresentInClause()) size++;
    }
    return size;
}

bool Clause::Satisfied() const
{
    for (auto p : propositions)
    {
        if (p->SatisfiesClause()) return true;
    }
    return false;
}

bool Clause::False() const
{
    bool clauses_exist = false;
    for (auto p : propositions)
    {
        clauses_exist |= p->PresentInClause();
    }
    return !clauses_exist;
}

std::set<int> Clause::GetPropsAsInts() const
{
    std::set<int> props_long_long;
    for (PropSetRaw_t::iterator iter = std::begin(propositions); iter != std::end(propositions); std::advance(iter, 1))
    {
        props_long_long.insert((*iter)->GetName());
    }
    return props_long_long;
}