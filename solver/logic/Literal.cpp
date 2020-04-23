#include "Literal.h"

Literal::LitPair Literal::CreateLitPair(int var_name)
{

    std::shared_ptr<Variable> var(new Variable(var_name));

    Literal::LitPair pair;
    pair.regular = new Literal(var, var_name);
    pair.notted = new Literal(var, -var_name, true);
    pair.regular->inverse = pair.notted;
    pair.notted->inverse = pair.regular;

    return pair;
}

Literal *Literal::GetInverse() const
{
    return inverse;
}

Literal::Literal(std::shared_ptr<Variable> var_, int name_)
    : var(var_),
      name(name_),
      notted(false)
{
}

Literal::Literal(std::shared_ptr<Variable> var_, int name_, bool notted_)
    : var(var_),
      name(name_),
      notted(notted_)
{
}

bool Literal::IsNot() const
{
    return notted;
}

bool Literal::IsAsserted() const
{
    return asserted;
}

void Literal::Assert()
{
    asserted = true;
    var->Set(!notted);
}

void Literal::UnAssert()
{
    asserted = false;
    var->UnSet();
}

bool Literal::EvaluatesToTrue() const
{
    return (var->IsTrue() && !notted) || (!var->IsTrue() && notted);
}

bool Literal::SatisfiesClause() const
{
    return var->IsSet() && EvaluatesToTrue();
}

bool Literal::PresentInClause() const
{
    return !(var->IsSet() && !EvaluatesToTrue());
}

bool Literal::operator()(const Literal *lhs, const Literal *rhs) const
{
    return lhs->name < rhs->name;
}

int Literal::GetName() const
{
    return name;
}