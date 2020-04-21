#ifndef VARIABLE_H
#define VARIABLE_H

#include <map>
#include <memory>

class Variable
{
public:
    Variable(int name_);

    int GetName() const;
    bool IsSet() const;
    bool IsTrue() const;

    void Set(bool value);
    void UnSet();

private:
    int name;
    bool is_set{false};
    bool truth_value;
};

typedef std::map<int, std::unique_ptr<Variable>> VariableMapUnique_t;
#endif