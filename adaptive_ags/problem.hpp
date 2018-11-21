#ifndef ADAPRIVE_AGS_PROBLEM_HPP_
#define ADAPRIVE_AGS_PROBLEM_HPP_

#include "adaptive_ags/utils/common.hpp"

namespace adaptive_ags {

class BoundingBox {
public:
  explicit BoundingBox(dim_t dimension)
      : left_(dimension, 0.0),
        right_(dimension, 1.0) { }

  explicit BoundingBox(dim_t dimension,
                       scalar_t left,
                       scalar_t right)
      : left_(dimension, left),
        right_(dimension, right) { }

  explicit BoundingBox(const vector_t &left,
                       const vector_t &right)
      : left_(left), right_(right) {
    _ADAPTIVE_AGS_ASSERT_( left.size() == right.size() );
  }

  dim_t dimension() const {
    _ADAPTIVE_AGS_ASSERT_( left().size() == right().size() );
    return dim_t(left().size());
  }

  const vector_t &left() const {
    return left_;
  }

  const vector_t &right() const {
    return right_;
  }

private:
  vector_t left_;
  vector_t right_;
};

class Minimizer {
public:
  Minimizer() :
    available_(false),
    minimum_(utils::infty()),
    minimizer_(utils::empty_vec()) { }

  explicit Minimizer(scalar_t minimum,
                     const vector_t &minimizer) :
    available_(true),
    minimum_(minimum),
    minimizer_(minimizer) { }

  explicit Minimizer(scalar_t minimum,
                     vector_t &&minimizer) :
    available_(true),
    minimum_(minimum),
    minimizer_(minimizer) { }

  bool available() const {
    return available_;
  }

  scalar_t value() const {
    return minimum_;
  }

  const vector_t &argument() const {
    return minimizer_;
  }

  void update(scalar_t minimum,
              const vector_t &minimizer) {
    available_ = true;
    if (minimum < minimum_) {
      minimum_ = minimum;
      minimizer_ = minimizer;
    }
  }

private:
  bool available_;
  scalar_t minimum_;
  vector_t minimizer_;
};

class Problem {
public:
  virtual ~Problem() { }
  virtual const BoundingBox &bbox() const = 0;
  virtual const Minimizer &ground_truth() const = 0;
  virtual scalar_t compute(const vector_t &x) const = 0;
};

} // namespace adaptive_ags

#endif
