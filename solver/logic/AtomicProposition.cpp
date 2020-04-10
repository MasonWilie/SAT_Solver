#include "AtomicProposition.h"

AtomicProposition::PropPair AtomicProposition::CreatePropPair(long long var_name)
{

    std::shared_ptr<Variable> var(new Variable(var_name));

    AtomicProposition::PropPair pair;
    pair.regular = new AtomicProposition(var, var_name);
    pair.notted = new AtomicProposition(var, -var_name, true);
    pair.regular->inverse = pair.notted;
    pair.notted->inverse = pair.regular;

    return pair;
}

AtomicProposition *AtomicProposition::GetInverse() const
{
    return inverse;
}

AtomicProposition::AtomicProposition(std::shared_ptr<Variable> var_, long long name_) : var(var_),
                                                                                        name(name_),
                                                                                        notted(false)
{
}

AtomicProposition::AtomicProposition(std::shared_ptr<Variable> var_, long long name_, bool notted_) : var(var_),
                                                                                                      name(name_),
                                                                                                      notted(notted_)
{
}

bool AtomicProposition::IsNot() const
{
    return notted;
}

bool AtomicProposition::VarIsSet() const
{
    return var->IsSet();
}

void AtomicProposition::Assert()
{
    var->Set(!notted);
}

void AtomicProposition::UnAssert()
{
    var->UnSet();
}

bool AtomicProposition::EvaluatesToTrue() const
{
    return (var->IsTrue() && !notted) || (!var->IsTrue() && notted);
}

bool AtomicProposition::SatisfiesClause() const
{
    return var->IsSet() && EvaluatesToTrue();
}

bool AtomicProposition::PresentInClause() const
{
    return !(var->IsSet() && !EvaluatesToTrue());
}

bool AtomicProposition::operator()(const AtomicProposition *lhs, const AtomicProposition *rhs) const
{
    return lhs->name < rhs->name;
}

long long AtomicProposition::GetName() const
{
    return name;
}