#ifndef ADAPRIVE_AGS_UTILS_COMMON_HPP_
#define ADAPRIVE_AGS_UTILS_COMMON_HPP_

#include <cmath>
#include <limits>

#include "adaptive_ags/defines.hpp"

namespace adaptive_ags {
namespace utils {

inline scalar_t infty() {
  return std::numeric_limits<scalar_t>::infinity();
}

inline scalar_t neg_infty() {
  return -infty();
}

inline vector_t empty_vec() {
  return vector_t();
}

inline scalar_t min(scalar_t x, scalar_t y) {
  return (x > y) ? y : x;
}

inline scalar_t max(scalar_t x, scalar_t y) {
  return (x > y) ? x : y;
}

inline scalar_t abs(scalar_t x) {
  return std::abs(x);
}

inline scalar_t l2_norm(const vector_t &x, const vector_t &y) {
  _ADAPTIVE_AGS_ASSERT_( x.size() == y.size() );
  scalar_t sum = 0;
  for (size_t i = 0; i < x.size(); i++) {
    sum += (x[i] - y[i]) * (x[i] - y[i]);
  }
  return std::sqrt(sum);
}

} // namespace utils
} // namespace adaptive_ags

#endif
