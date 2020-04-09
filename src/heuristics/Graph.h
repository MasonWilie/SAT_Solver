#ifndef GRAPH_H
#define GRAPH_H

#include "AtomicProposition.h"

#include <vector>
#include <memory>

struct Node
{
    AtomicProposition const *proposition;

    std::set<Node*> forward;
    std::set<Node*> backward;

    Node(AtomicProposition* prop)
        : proposition(prop)
    {}

    Node()
        : proposition(nullptr)
    {}
};

class Graph
{
public:
    Graph()=default;
    ~Graph()=default;

    void AddNode(AtomicProposition* prop);
    void AddNode(AtomicProposition* from, AtomicProposition* to);

    void AddEdge(AtomicProposition* from, AtomicProposition* to);

    void RemoveNode(AtomicProposition* prop);

private:
    std::map<AtomicProposition*, std::unique_ptr<Node>> prop_to_node;
};

#endif