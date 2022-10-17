#pragma once
#include "ff.hpp"
#include <benchmark/benchmark.h>

// Benchmark Dilithium PQC suite implementation on CPU, using google-benchmark
namespace bench_dilithium {

// Benchmark addition over prime field Z_q | q = 2^23 - 2^13 + 1
void
ff_add(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = b + a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c = c + a;
  }

  assert(c == b);
}

// Benchmark subtraction over prime field Z_q | q = 2^23 - 2^13 + 1
void
ff_sub(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = b - a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  const size_t itr = state.iterations();

  ff::ff_t c{};
  for (size_t i = 0; i < itr; i++) {
    c = c - a;
  }

  assert(c == b);
}

// Benchmark negation over prime field Z_q | q = 2^23 - 2^13 + 1
void
ff_neg(benchmark::State& state)
{
  ff::ff_t a = ff::ff_t::random();
  ff::ff_t b{};

  for (auto _ : state) {
    b = -a;

    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(a);
    benchmark::ClobberMemory();
  }

  ff::ff_t c = -a;
  assert(c == b);
}

}
