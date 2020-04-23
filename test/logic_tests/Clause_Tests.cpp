#include "gtest/gtest.h"
#include "Clause.h"


TEST(Clause, General)
{
    Literal::LitPair pair = Literal::CreateLitPair(1);
    std::unique_ptr<Literal> a_prop = std::unique_ptr<Literal>(pair.regular);
    std::unique_ptr<Literal> a_not_prop = std::unique_ptr<Literal>(pair.notted);

    pair = Literal::CreateLitPair(2);
    std::unique_ptr<Literal> b_prop = std::unique_ptr<Literal>(pair.regular);
    std::unique_ptr<Literal> b_not_prop = std::unique_ptr<Literal>(pair.notted);

    pair = Literal::CreateLitPair(3);
    std::unique_ptr<Literal> c_prop = std::unique_ptr<Literal>(pair.regular);
    std::unique_ptr<Literal> c_not_prop = std::unique_ptr<Literal>(pair.notted);

    pair = Literal::CreateLitPair(4);
    std::unique_ptr<Literal> d_prop = std::unique_ptr<Literal>(pair.regular);
    std::unique_ptr<Literal> d_not_prop = std::unique_ptr<Literal>(pair.notted);

    Clause c1, c2, c3;
    
    // C1 = a + b + c'
    c1.AddLiteral(a_prop.get());
    c1.AddLiteral(b_prop.get());
    c1.AddLiteral(c_not_prop.get());

    // C2 = a' + c + d'
    c2.AddLiteral(a_not_prop.get());
    c2.AddLiteral(c_prop.get());
    c2.AddLiteral(d_not_prop.get());

    // C3 = b + d
    c3.AddLiteral(b_prop.get());
    c3.AddLiteral(d_prop.get());

    EXPECT_EQ(c1.Size(), 3);
    EXPECT_EQ(c2.Size(), 3);
    EXPECT_EQ(c3.Size(), 2);

    a_prop->Assert();
    EXPECT_EQ(c1.Satisfied(), true);
    a_prop->UnAssert();
    EXPECT_EQ(c1.Satisfied(), false);

    a_prop->Assert();
    EXPECT_EQ(c1.Satisfied(), true);
    a_not_prop->Assert();
    EXPECT_EQ(c1.Satisfied(), false);

    a_prop->Assert();
    EXPECT_EQ(c2.Size(), 2);
    a_not_prop->Assert();
    EXPECT_EQ(c2.Size(), 3);
    EXPECT_EQ(c2.Satisfied(), true);

}