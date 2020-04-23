#include "gtest/gtest.h"
#include "Literal.h"

TEST(Literal, CreatingPair)
{
    Literal::LitPair pair;
    pair = Literal::CreateLitPair(1);

    std::unique_ptr<Literal> a = std::unique_ptr<Literal>(pair.regular);
    std::unique_ptr<Literal> a_not = std::unique_ptr<Literal>(pair.notted);
}

TEST(Literal, Asserting)
{    
    Literal::LitPair pair = Literal::CreateLitPair(1);
    std::unique_ptr<Literal> a(pair.regular);
    std::unique_ptr<Literal> a_not(pair.notted);

    pair = Literal::CreateLitPair(2);
    std::unique_ptr<Literal> b(pair.regular);
    std::unique_ptr<Literal> b_not(pair.notted);

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