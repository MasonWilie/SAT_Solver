#include <map>
#include <memory>

#ifndef VARIABLE_H
#define VARIABLE_H

class Variable
{
public:
    Variable(long long name_);

    long long GetName() const;
    bool IsSet() const;
    bool IsTrue() const;

    void Set(bool value);
    void UnSet();

private:
    long long name;
    bool is_set{false};
    bool truth_value;
};

typedef std::map<long long, std::unique_ptr<Variable>> VariableMapUnique_t;
#endif