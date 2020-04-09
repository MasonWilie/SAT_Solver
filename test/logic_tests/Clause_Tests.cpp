#include "gtest/gtest.h"
#include "Clause.h"


TEST(Clause, General)
{
    AtomicProposition::PropPair pair = AtomicProposition::CreatePropPair(1);
    std::unique_ptr<AtomicProposition> a_prop = std::unique_ptr<AtomicProposition>(pair.regular);
    std::unique_ptr<AtomicProposition> a_not_prop = std::unique_ptr<AtomicProposition>(pair.notted);

    pair = AtomicProposition::CreatePropPair(2);
    std::unique_ptr<AtomicProposition> b_prop = std::unique_ptr<AtomicProposition>(pair.regular);
    std::unique_ptr<AtomicProposition> b_not_prop = std::unique_ptr<AtomicProposition>(pair.notted);

    pair = AtomicProposition::CreatePropPair(3);
    std::unique_ptr<AtomicProposition> c_prop = std::unique_ptr<AtomicProposition>(pair.regular);
    std::unique_ptr<AtomicProposition> c_not_prop = std::unique_ptr<AtomicProposition>(pair.notted);

    pair = AtomicProposition::CreatePropPair(4);
    std::unique_ptr<AtomicProposition> d_prop = std::unique_ptr<AtomicProposition>(pair.regular);
    std::unique_ptr<AtomicProposition> d_not_prop = std::unique_ptr<AtomicProposition>(pair.notted);

    Clause c1, c2, c3;
    
    // C1 = a + b + c'
    c1.AddProposition(a_prop.get());
    c1.AddProposition(b_prop.get());
    c1.AddProposition(c_not_prop.get());

    // C2 = a' + c + d'
    c2.AddProposition(a_not_prop.get());
    c2.AddProposition(c_prop.get());
    c2.AddProposition(d_not_prop.get());

    // C3 = b + d
    c3.AddProposition(b_prop.get());
    c3.AddProposition(d_prop.get());

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