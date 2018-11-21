#ifndef ADAPRIVE_AGS_METHOD_HPP_
#define ADAPRIVE_AGS_METHOD_HPP_

#include "adaptive_ags/result.hpp"
#include "adaptive_ags/problem.hpp"

namespace adaptive_ags {

class Parameters {
  _ADAPTIVE_AGS_REG_PARAM_( size_t,   max_iters,      1000  )
  _ADAPTIVE_AGS_REG_PARAM_( size_t,   init_max_iters, 7     )
  _ADAPTIVE_AGS_REG_PARAM_( scalar_t, epsilon,        0.01  )
  _ADAPTIVE_AGS_REG_PARAM_( scalar_t, init_epsilon,   0.1   )
  _ADAPTIVE_AGS_REG_PARAM_( scalar_t, reliability,    3.5   )
  _ADAPTIVE_AGS_REG_PARAM_( bool,     enable_updates, false )
};

class Method {
public:
  Result run(const Problem &problem,
             const Parameters &parameters);
};

} // namespace adaptive_ags

#endif
