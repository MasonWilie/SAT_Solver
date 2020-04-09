#include "Variable.h"

Variable::Variable(long long name_) : name(name_)
{}

long long Variable::GetName() const
{
    return name;
}

bool Variable::IsTrue() const
{
    return truth_value;
}

void Variable::Set(bool value)
{
    is_set = true;
    truth_value = value;
}

bool Variable::IsSet() const
{
    return is_set;
}

void Variable::UnSet()
{
    is_set = false;
}
