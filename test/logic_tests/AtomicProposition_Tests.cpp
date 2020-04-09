#include "gtest/gtest.h"
#include "AtomicProposition.h"

TEST(AtomicProposition, CreatingPair)
{
    AtomicProposition::PropPair pair;
    pair = AtomicProposition::CreatePropPair(1);

    std::unique_ptr<AtomicProposition> a = std::unique_ptr<AtomicProposition>(pair.regular);
    std::unique_ptr<AtomicProposition> a_not = std::unique_ptr<AtomicProposition>(pair.notted);
}

TEST(AtomicProposition, Asserting)
{    
    AtomicProposition::PropPair pair = AtomicProposition::CreatePropPair(1);
    std::unique_ptr<AtomicProposition> a(pair.regular);
    std::unique_ptr<AtomicProposition> a_not(pair.notted);

    pair = AtomicProposition::CreatePropPair(2);
    std::unique_ptr<AtomicProposition> b(pair.regular);
    std::unique_ptr<AtomicProposition> b_not(pair.notted);

    EXPECT_EQ(a->GetName(), 1);
    EXPECT_EQ(a_not->GetName(), -1);
    EXPECT_EQ(b->GetName(), 2);
    EXPECT_EQ(b_not->GetName(), -2);

    EXPECT_EQ(a->SatisfiesClause(), false);
    EXPECT_EQ(a->PresentInClause(), true);
    a->Assert();
    EXPECT_EQ(a->SatisfiesClause(), true);

    EXPECT_EQ(a_not->IsNot(), true);
    EXPECT_EQ(a_not->PresentInClause(), false);
    EXPECT_EQ(a_not->SatisfiesClause(), false);

    a_not->UnAssert();
    EXPECT_EQ(a->SatisfiesClause(), false);
    EXPECT_EQ(a->PresentInClause(), true);
}