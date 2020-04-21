#include "AtomicProposition.h"

AtomicProposition::PropPair AtomicProposition::CreatePropPair(int var_name)
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

AtomicProposition::AtomicProposition(std::shared_ptr<Variable> var_, int name_)
    : var(var_),
      name(name_),
      notted(false)
{
}

AtomicProposition::AtomicProposition(std::shared_ptr<Variable> var_, int name_, bool notted_)
    : var(var_),
      name(name_),
      notted(notted_)
{
}

bool AtomicProposition::IsNot() const
{
    return notted;
}

bool AtomicProposition::IsAsserted() const
{
    return asserted;
}

void AtomicProposition::Assert()
{
    asserted = true;
    var->Set(!notted);
}

void AtomicProposition::UnAssert()
{
    asserted = false;
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

int AtomicProposition::GetName() const
{
    return name;
}