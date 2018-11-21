#ifndef ADAPRIVE_AGS_UTILS_ZIP_ITERATOR_HPP_
#define ADAPRIVE_AGS_UTILS_ZIP_ITERATOR_HPP_

#include <utility>
#include <iterator>
#include <algorithm>

namespace adaptive_ags {
namespace utils {

template<class I1_Type, class I2_Type>
class ZipIterator {
private:
  typedef typename I1_Type::value_type I1_ValueType;
  typedef typename I2_Type::value_type I2_ValueType;
  typedef std::pair<I1_ValueType, I2_ValueType> ValuePair;

public:
  typedef ValuePair value_type;

  explicit ZipIterator(I1_Type iterator1, I2_Type iterator2)
      : _iterator1(iterator1),
        _iterator2(iterator2) { }

  ZipIterator<I1_Type, I2_Type>& operator++() {
    _iterator1++;
    _iterator2++;
    return *this;
  }

  ZipIterator<I1_Type, I2_Type> operator++(int) {
    ZipIterator<I1_Type, I2_Type> retval = *this;
    ++(*this);
    return retval;
  }

  bool operator==(const ZipIterator<I1_Type, I2_Type> &other) const {
    return _iterator1 == other._iterator1 &&
           _iterator2 == other._iterator2;
  }

  bool operator!=(const ZipIterator<I1_Type, I2_Type> &other) const {
    return _iterator1 != other._iterator1 &&
           _iterator2 != other._iterator2;
  }

  std::pair<I1_ValueType, I2_ValueType> operator*() const {
    return std::pair<I1_ValueType, I2_ValueType>(*_iterator1, *_iterator2);
  }

private:
  I1_Type _iterator1;
  I2_Type _iterator2;
};

template<class T1, class T2>
class ZipIteratorHolder {
private:
  typedef typename std::remove_reference<T1>::type T1_RemoveRef;
  typedef typename std::remove_reference<T2>::type T2_RemoveRef;
  typedef typename T1_RemoveRef::const_iterator T1_IteratorType;
  typedef typename T2_RemoveRef::const_iterator T2_IteratorType;

public:
  typedef ZipIterator<T1_IteratorType, T2_IteratorType> iterator;

  explicit ZipIteratorHolder(T1 &&collection1, T2 &&collection2)
      : _begin(collection1.begin(), collection2.begin()),
        _end(collection1.end(), collection2.end()) { }

  iterator begin() { return _begin; }
  iterator end() { return _end; }

private:
  iterator _begin;
  iterator _end;
};

template<class T1, class T2>
inline auto zip(T1 &&collection1, T2 &&collection2) {
  return ZipIteratorHolder<T1, T2>(std::forward<T1>(collection1),
                                   std::forward<T2>(collection2));
}

} // namespace utils
} // namespace adaptive_ags

#endif
