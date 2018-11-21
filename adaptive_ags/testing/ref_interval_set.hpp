#ifndef ADAPRIVE_AGS_TESTING_REF_INTERVAL_SET_HPP_
#define ADAPRIVE_AGS_TESTING_REF_INTERVAL_SET_HPP_

#include "adaptive_ags/testing/utils.hpp"

namespace adaptive_ags {
namespace testing {

inline scalar_t default_weight_function(scalar_t m, scalar_t l, scalar_t r,
                                        scalar_t lvalue, scalar_t rvalue) {
  scalar_t xdiff = r - l;
  scalar_t zdiff = rvalue - lvalue;
  return m * xdiff + (zdiff * zdiff) / (m * xdiff) - 2.0 * (rvalue + lvalue);
}

inline scalar_t default_next_function(scalar_t m, scalar_t l, scalar_t r,
                                      scalar_t lvalue, scalar_t rvalue) {
  return 0.5 * (r + l - (rvalue - lvalue) / m);
}

class RefIntervalSet {
public:
  explicit RefIntervalSet(scalar_t reliability,
                          size_t iterations)
      : t_(0),
        size_(0),
        r_(reliability),
        m_(1.0),
        tw_(utils::neg_infty()),
        xx_(iterations + 2),
        zz_(iterations + 2),
        min_z_(utils::infty()) { }

  void push_first(scalar_t l, scalar_t lvalue,
                  scalar_t r, scalar_t rvalue) {
    _ADAPTIVE_AGS_ASSERT_( size_ == 0 );
    _ADAPTIVE_AGS_ASSERT_( capacity() > 0 );

    xx_[0] = l; zz_[0] = lvalue;
    xx_[1] = r; zz_[1] = rvalue;
    m_ = r_ * utils::abs((rvalue - lvalue) / (r - l));
    t_ = 1;
    tw_ = default_weight_function(m_, xx_[0], xx_[1], zz_[0], zz_[1]);
    size_ = 2;
    min_z_ = utils::min(lvalue, rvalue);
  }

  scalar_t push(scalar_t x, scalar_t z) {
    _ADAPTIVE_AGS_ASSERT_( size_ < capacity() );
    size_++;

    for (size_t i = size_ - 1; i > 0; i--) {
      if (xx_[i - 1] < x) {
        xx_[i] = x;
        zz_[i] = z;
        break;
      }

      xx_[i] = xx_[i - 1];
      zz_[i] = zz_[i - 1];
    }

    scalar_t mslope = utils::neg_infty();
    for (size_t i = 1; i < size_; i++) {
      scalar_t s = utils::abs( (zz_[i] - zz_[i - 1]) /
                               (xx_[i] - xx_[i - 1]) );
      mslope = utils::max(mslope, s);
    }
    m_ = r_ * mslope;

    tw_ = utils::neg_infty();
    for (size_t i = 1; i < size_; i++) {
      scalar_t c = default_weight_function(m_, xx_[i - 1], xx_[i],
                                               zz_[i - 1], zz_[i]);
      if (c > tw_) {
        tw_ = c;
        t_ = i;
      }
    }

    min_z_ = utils::min(min_z_, z);
    return xx_[t_] - xx_[t_ - 1];
  }

  scalar_t next() const {
    return default_next_function(m_,
      xx_[t_ - 1], xx_[t_], zz_[t_ - 1], zz_[t_]);
  }

  scalar_t best_weight() const {
    return tw_;
  }

  vector_t arguments() const {
    return xx_;
  }

  vector_t values() const {
    return zz_;
  }

  scalar_t min_z() const {
    return min_z_;
  }

  size_t capacity() const {
    _ADAPTIVE_AGS_ASSERT_( xx_.size() == zz_.size() );
    return xx_.size();
  }

private:
  size_t t_;
  size_t size_;
  size_t capacity_;

  scalar_t r_;
  scalar_t m_;
  scalar_t tw_;
  vector_t xx_;
  vector_t zz_;
  scalar_t min_z_;
};

} // namespace testing
} // namespace adaptive_ags

#endif
