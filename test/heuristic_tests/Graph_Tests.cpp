#include <gtest/gtest.h>
#include "Graph.h"


TEST(Graph, graph)
{
    Graph g;

    AtomicProposition::PropPair a = AtomicProposition::CreatePropPair(1);
    AtomicProposition::PropPair b = AtomicProposition::CreatePropPair(2);
    AtomicProposition::PropPair c = AtomicProposition::CreatePropPair(3);
    AtomicProposition::PropPair e = AtomicProposition::CreatePropPair(4);

    g.AddNode(a.notted);
}