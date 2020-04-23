#include "Clause.h"


void Clause::AddLiteral(Literal* lit)
{
    literals.insert(lit);
}

bool Clause::ContainsLiteral(Literal* lit) const
{
    return lit->PresentInClause() && std::find(literals.begin(), literals.end(), lit) != literals.end();
}

size_t Clause::Size() const
{
    size_t size{0};
    for (auto p : literals)
    {
        if (p->PresentInClause()) size++;
    }
    return size;
}

bool Clause::Satisfied() const
{
    for (auto p : literals)
    {
        if (p->SatisfiesClause()) return true;
    }
    return false;
}

bool Clause::False() const
{
    bool clauses_exist = false;
    for (auto p : literals)
    {
        clauses_exist |= p->PresentInClause();
    }
    return !clauses_exist;
}

std::set<int> Clause::GetLitsAsInts() const
{
    std::set<int> lits_long_long;
    for (LitSetRaw_t::iterator iter = std::begin(literals); iter != std::end(literals); std::advance(iter, 1))
    {
        if ((*iter)->PresentInClause())
        {
            lits_long_long.insert((*iter)->GetName());
        }
    }
    return lits_long_long;
}

LitSetRaw_t Clause::GetLitsAsPtrs() const
{
    LitSetRaw_t present_clauses;
    for (LitSetRaw_t::iterator iter = std::begin(literals); iter != std::end(literals); std::advance(iter, 1))
    {
        if ((*iter)->PresentInClause())
        {
            present_clauses.insert((*iter));
        }
    }
    return present_clauses;
}