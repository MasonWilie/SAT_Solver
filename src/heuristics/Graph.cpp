#include "Graph.h"


void Graph::AddNode(AtomicProposition* prop)
{
    prop_to_node.insert(std::make_pair(prop, std::make_unique<Node>(prop)));
}

void Graph::AddNode(AtomicProposition* from, AtomicProposition* to)
{
    prop_to_node.insert(std::make_pair(to, std::make_unique<Node>(to)));

    prop_to_node.at(to)->backward.insert(prop_to_node.at(from).get());
    prop_to_node.at(from)->forward.insert(prop_to_node.at(to).get());
}

void Graph::RemoveNode(AtomicProposition* prop)
{
    Node* node = prop_to_node.at(prop).get();

    for (Node* to : node->forward)
    {
        to->backward.erase(node);
    }

    for (Node* from : node->backward)
    {
        from->forward.erase(node);
    }

    prop_to_node.erase(prop);
}

void Graph::AddEdge(AtomicProposition* from, AtomicProposition* to)
{
    Node* to_node = prop_to_node.at(to).get();
    Node* from_node = prop_to_node.at(from).get();

    to_node->backward.insert(from_node);
    from_node->forward.insert(to_node);
}