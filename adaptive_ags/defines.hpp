#ifndef ADAPRIVE_AGS_DEFINES_HPP_
#define ADAPRIVE_AGS_DEFINES_HPP_

#include <stddef.h> // size_t definition

#include <vector>
#include <cstdint>
#include <cassert>

namespace adaptive_ags {

typedef double scalar_t;
typedef std::uint8_t dim_t;
typedef std::vector<scalar_t> vector_t;

#define _ADAPTIVE_AGS_ASSERT_(expr) \
  assert(expr)

#define _ADAPTIVE_AGS_REG_PARAM_(type, name, default_value) \
  public:                                                   \
    const auto &name() const {                              \
      return name##_;                                       \
    }                                                       \
    auto &name(const type &value) {                         \
      name##_ = value; return *this;                        \
    }                                                       \
    type name##_ = default_value;

} // namespace adaptive_ags

#endif
