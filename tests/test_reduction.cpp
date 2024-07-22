#include "ml_dsa/internals/math/reduction.hpp"
#include <gtest/gtest.h>
#include <tuple>

// Given a random element ∈ Z_q, this routine tests whether extracting high and low order bits & then reconstructing
// original element using components of smaller bit width, work as expected or not.
TEST(ML_DSA, Power2Round)
{
  constexpr size_t itr_cnt = 1ul << 16;

  constexpr size_t d = 13;
  constexpr ml_dsa_field::zq_t two_to_d(1u << d);

  ml_dsa_prng::prng_t<256> prng;

  for (size_t i = 0; i < itr_cnt; i++) {
    const ml_dsa_field::zq_t r = ml_dsa_field::zq_t::random(prng);

    ml_dsa_field::zq_t high, low;
    std::tie(high, low) = ml_dsa_reduction::power2round<d>(r);

    EXPECT_EQ(r, high * two_to_d + low);
  }
}

// Given any arbitrary element r ∈ Z_q and a small element z ∈ Z_q, this routine tries to test correctness of decompose
// routine, which is used for computing a hint bit ( say h ) s.t. one doesn't need to store r, z both, but they can
// still recover high order bits of r + z.
template<uint32_t alpha, uint32_t z, size_t rounds = 65536ul>
static void
test_decompose()
{
  ml_dsa_prng::prng_t<256> prng;

  for (size_t i = 0; i < rounds; i++) {
    const ml_dsa_field::zq_t r = ml_dsa_field::zq_t::random(prng);
    constexpr ml_dsa_field::zq_t elem_z{ z };

    const ml_dsa_field::zq_t h = ml_dsa_reduction::make_hint<alpha>(elem_z, r);
    const ml_dsa_field::zq_t rz = ml_dsa_reduction::use_hint<alpha>(h, r);

    const ml_dsa_field::zq_t rz_ = ml_dsa_reduction::highbits<alpha>(r + elem_z);

    EXPECT_EQ(rz, rz_);
  }
}

TEST(ML_DSA, MakingAndUsingOfHintBits)
{
  test_decompose<((ml_dsa_field::Q - 1u) / 88u) << 1, 77u>();
  test_decompose<((ml_dsa_field::Q - 1u) / 88u) << 1, 1321u>();
  test_decompose<((ml_dsa_field::Q - 1u) / 32u) << 1, 997u>();
  test_decompose<((ml_dsa_field::Q - 1u) / 32u) << 1, 1981u>();
}
