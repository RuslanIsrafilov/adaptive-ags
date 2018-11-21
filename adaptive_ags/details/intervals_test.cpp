#include <random>
#include "adaptive_ags/details/intervals.hpp"
#include "adaptive_ags/testing/utils.hpp"
#include "adaptive_ags/testing/ref_interval_set.hpp"

namespace adaptive_ags {
namespace {

using details::IntervalSet;
using testing::RefIntervalSet;

class UnivariateFunction {
private:
  const scalar_t AMPLITUDE = 10.0;

public:
  explicit UnivariateFunction(int seed)
      : engine_(seed),
        distr_(-AMPLITUDE, AMPLITUDE) { }

  scalar_t compute(scalar_t x) {
    return distr_(engine_);
  }

private:
  std::default_random_engine engine_;
  std::uniform_real_distribution<scalar_t> distr_;
};

TEST(IntervalSet, IterationsAreEqualToReference) {
  const scalar_t r = 3.5;
  const size_t max_n = 100;

  UnivariateFunction f(7777);
  const scalar_t a = -2.0;
  const scalar_t b =  3.0;
  const scalar_t af = f.compute(a);
  const scalar_t bf = f.compute(b);

  IntervalSet iset(r);
  iset.push_first(a, af, b, bf);

  RefIntervalSet iset_ref(r, max_n);
  iset_ref.push_first(a, af, b, bf);

  for (size_t i = 0; i < max_n; i++) {
    const scalar_t x = iset.next();
    const scalar_t x_ref = iset_ref.next();
    ASSERT_DOUBLE_EQ(x, x_ref)
      << "x     = " << x     << "\n"
      << "x_ref = " << x_ref << "\n";

    const scalar_t z = f.compute(x);
    const scalar_t e = iset.push(x, x);
    const scalar_t e_ref = iset_ref.push(x, x);
    ASSERT_DOUBLE_EQ(x, x_ref)
      << "e     = " << e     << "\n"
      << "e_ref = " << e_ref << "\n";
  }
}

} // namespace
} // namespace adaptive_ags
