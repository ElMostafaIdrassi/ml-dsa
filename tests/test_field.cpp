#include "field.hpp"
#include <gtest/gtest.h>

// Test functional correctness of Dilithium prime field operations, by running
// through fairly large number of rounds of execution of field operations on
// randomly generated field element.
TEST(Dilithium, ArithmeticOverZq)
{
  constexpr size_t itr_cnt = 1ul << 20;
  prng::prng_t prng;

  for (size_t i = 0; i < itr_cnt; i++) {
    const auto a = field::zq_t::random(prng);
    const auto b = field::zq_t::random(prng);

    // Addition, Subtraction and Negation
    const auto c = a + b;
    const auto d = c - b;
    const auto e = c - a;

    EXPECT_EQ(d, a);
    EXPECT_EQ(e, b);

    // Multiplication, Exponentiation, Inversion and Division
    const auto f = a * b;
    const auto g = f / b;
    const auto h = f / a;

    if (b != field::zq_t()) {
      EXPECT_EQ(g, a);
    } else {
      EXPECT_EQ(g, field::zq_t());
    }

    if (a != field::zq_t()) {
      EXPECT_EQ(h, b);
    } else {
      EXPECT_EQ(h, field::zq_t());
    }
  }
}
