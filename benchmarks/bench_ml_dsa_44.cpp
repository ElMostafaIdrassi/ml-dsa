#include "bench_helper.hpp"
#include "ml_dsa/ml_dsa_44.hpp"
#include <benchmark/benchmark.h>

// Benchmark performance of ML-DSA-44 key generation algorithm.
void
ml_dsa_44_keygen(benchmark::State& state)
{
  std::array<uint8_t, ml_dsa_44::KeygenSeedByteLen> seed{};
  std::array<uint8_t, ml_dsa_44::PubKeyByteLen> pubkey{};
  std::array<uint8_t, ml_dsa_44::SecKeyByteLen> seckey{};

  randomshake::randomshake_t<128> csprng;
  csprng.generate(seed);

  for (auto _ : state) {
    benchmark::DoNotOptimize(seed);
    benchmark::DoNotOptimize(pubkey);
    benchmark::DoNotOptimize(seckey);

    ml_dsa_44::keygen(seed, pubkey, seckey);

    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations());
}

// Benchmark performance of ML-DSA-44 signing algorithm.
void
ml_dsa_44_sign(benchmark::State& state)
{
  const size_t mlen = state.range(0);

  std::vector<uint8_t> msg(mlen, 0);
  auto msg_span = std::span(msg);

  std::array<uint8_t, ml_dsa_44::KeygenSeedByteLen> seed{};
  std::array<uint8_t, ml_dsa_44::PubKeyByteLen> pubkey{};
  std::array<uint8_t, ml_dsa_44::SecKeyByteLen> seckey{};
  std::array<uint8_t, ml_dsa_44::SigningSeedByteLen> rnd{};
  std::array<uint8_t, ml_dsa_44::SigByteLen> sig{};

  randomshake::randomshake_t<128> csprng;
  csprng.generate(seed);
  csprng.generate(rnd);
  csprng.generate(msg_span);

  ml_dsa_44::keygen(seed, pubkey, seckey);

  bool has_signed = true;
  for (auto _ : state) {
    benchmark::DoNotOptimize(has_signed);
    benchmark::DoNotOptimize(rnd);
    benchmark::DoNotOptimize(seckey);
    benchmark::DoNotOptimize(msg_span);
    benchmark::DoNotOptimize(sig);

    has_signed &= ml_dsa_44::sign(rnd, seckey, msg_span, {}, sig);

    benchmark::ClobberMemory();
  }

  assert(has_signed);
  assert(ml_dsa_44::verify(pubkey, msg_span, {}, sig));

  state.SetItemsProcessed(state.iterations());
}

// Benchmark performance of ML-DSA-44 signature verification algorithm.
void
ml_dsa_44_verify(benchmark::State& state)
{
  const size_t mlen = state.range(0);

  std::vector<uint8_t> msg(mlen, 0);
  auto msg_span = std::span(msg);

  std::array<uint8_t, ml_dsa_44::KeygenSeedByteLen> seed{};
  std::array<uint8_t, ml_dsa_44::PubKeyByteLen> pubkey{};
  std::array<uint8_t, ml_dsa_44::SecKeyByteLen> seckey{};
  std::array<uint8_t, ml_dsa_44::SigningSeedByteLen> rnd{};
  std::array<uint8_t, ml_dsa_44::SigByteLen> sig{};

  randomshake::randomshake_t<128> csprng;
  csprng.generate(seed);
  csprng.generate(rnd);
  csprng.generate(msg_span);

  ml_dsa_44::keygen(seed, pubkey, seckey);
  const bool has_signed = ml_dsa_44::sign(rnd, seckey, msg_span, {}, sig);
  assert(has_signed);

  bool has_verified = true;
  for (auto _ : state) {
    benchmark::DoNotOptimize(has_verified);
    benchmark::DoNotOptimize(pubkey);
    benchmark::DoNotOptimize(msg_span);
    benchmark::DoNotOptimize(sig);

    has_verified &= ml_dsa_44::verify(pubkey, msg_span, {}, sig);

    benchmark::ClobberMemory();
  }

  assert(has_verified);

  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(ml_dsa_44_keygen)->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(ml_dsa_44_sign)->Arg(32)->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(ml_dsa_44_verify)->Arg(32)->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
