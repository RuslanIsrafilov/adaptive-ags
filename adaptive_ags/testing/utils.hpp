#ifndef ADAPRIVE_AGS_TESTING_UTILS_HPP_
#define ADAPRIVE_AGS_TESTING_UTILS_HPP_

#include <list>
#include <cmath>
#include "gtest/gtest.h"
#include "adaptive_ags/utils/common.hpp"
#include "adaptive_ags/utils/zip_iterator.hpp"

namespace adaptive_ags {
namespace testing {

inline ::testing::Message message(const vector_t &vector) {
  ::testing::Message msg;
  msg << "[ ";
  for (size_t i = 0; i < vector.size(); i++) {
    msg << vector[i];
    if (i < vector.size() - 1)
    { msg << ", "; }
  }
  return msg << " ]";
}

inline void assert_points_are_equal(const std::list<vector_t> &points,
                                    const std::list<vector_t> &points_ref) {
  size_t index = 0;
  for (auto &&pair : utils::zip(points, points_ref)) {
    const vector_t &x = pair.first;
    const vector_t &x_ref = pair.second;

    for (auto &&xx : utils::zip(x, x_ref)) {
      ASSERT_FLOAT_EQ(xx.first, xx.second)
        << "Check for trials equality failed\n"
        << "x   = " << message(x)     << "\n"
        << "ref = " << message(x_ref) << "\n"
        << "idx = " << index          << "\n";
    }
    index++;
  }
}

inline void assert_vectors_are_close(const vector_t &actual,
                                     const vector_t &reference,
                                     scalar_t epsilon) {
  scalar_t sum = 0;
  for (auto &&xy : utils::zip(actual, reference)) {
    const scalar_t x = xy.first;
    const scalar_t y = xy.second;
    sum += (x - y) * (x - y);
  }
  ASSERT_LT(std::sqrt(sum), epsilon)
    << "Check for vectors closeness failed\n"
    << "x    = " << message(actual)    << "\n"
    << "ref  = " << message(reference) << "\n"
    << "diff = " << std::sqrt(sum)     << "\n";
}

} // namespace testing
} // namespace adaptive_ags

#endif
