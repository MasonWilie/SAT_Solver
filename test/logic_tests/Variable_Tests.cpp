#include "gtest/gtest.h"
#include "Variable.h"

TEST(Variable_tests, Constructor)
{
    const long long name_1{1};
    const long long name_2{2};
    const long long name_3{3};
    const long long name_4{4};

    Variable a(name_1);
    Variable b(name_2);
    Variable c(name_3);
    Variable d(name_4);

    EXPECT_EQ(name_1, a.GetName());
    EXPECT_EQ(false, a.IsSet());

    EXPECT_EQ(name_2, b.GetName());
    EXPECT_EQ(false, b.IsSet());

    EXPECT_EQ(name_3, c.GetName());
    EXPECT_EQ(false, c.IsSet());

    EXPECT_EQ(name_4, d.GetName());
    EXPECT_EQ(false, d.IsSet());
}

TEST(Variable_tests, Setting)
{
    const long long name_1{1};
    const long long name_2{2};

    Variable a(name_1);
    Variable b(name_2);

    EXPECT_EQ(name_1, a.GetName());
    EXPECT_EQ(false, a.IsSet());

    EXPECT_EQ(name_2, b.GetName());
    EXPECT_EQ(false, b.IsSet());
    a.Set(true);
    EXPECT_EQ(true, a.IsSet());
    EXPECT_EQ(true, a.IsTrue());

    b.Set(false);
    EXPECT_EQ(true, b.IsSet());
    EXPECT_EQ(false, b.IsTrue());
}