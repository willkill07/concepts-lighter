#ifndef RAJA_CONCEPTS_HPP_
#define RAJA_CONCEPTS_HPP_

#include "metalib.hpp"
#include "concepts.hpp"

namespace concepts_raja {

using namespace concepts;

template <typename T>
using LessThanComparable =
    decltype(valid_expr(convertible_to<bool>(val<T>() < val<T>())));

template <typename T>
using GreaterThanComparable =
    decltype(valid_expr(convertible_to<bool>(val<T>() > val<T>())));

template <typename T>
using LessEqualComparable =
    decltype(valid_expr(convertible_to<bool>(val<T>() <= val<T>())));

template <typename T>
using GreaterEqualComparable =
    decltype(valid_expr(convertible_to<bool>(val<T>() >= val<T>())));

template <typename T>
using EqualityComparable =
    decltype(valid_expr(convertible_to<bool>(val<T>() == val<T>())));

template <typename T, typename U>
using ComparableTo = decltype(valid_expr(
    convertible_to<bool>(val<U>() <  val<T>()),
    convertible_to<bool>(val<T>() <  val<U>()),
    convertible_to<bool>(val<U>() <= val<T>()),
    convertible_to<bool>(val<T>() <= val<U>()),
    convertible_to<bool>(val<U>() >  val<T>()),
    convertible_to<bool>(val<T>() >  val<U>()),
    convertible_to<bool>(val<U>() >= val<T>()),
    convertible_to<bool>(val<T>() >= val<U>()),
    convertible_to<bool>(val<U>() == val<T>()),
    convertible_to<bool>(val<T>() == val<U>()),
    convertible_to<bool>(val<U>() != val<T>()),
    convertible_to<bool>(val<T>() != val<U>())));

template <typename T> using Comparable = ComparableTo<T, T>;

template <typename T>
using Iterator = decltype(valid_expr(*ref<T>(),
                                     has_type<T &>(++ref<T>())));

template <typename T>
using ForwardIterator =
    decltype(valid_expr(models<Iterator<T>>(),
                        ref<T>()++,
                        *ref<T>()++));

template <typename T>
using BidirectionalIterator =
    decltype(valid_expr(models<ForwardIterator<T>>(),
                        has_type<T &>(--ref<T>()),
                        convertible_to<T const &>(ref<T>()--),
                        *ref<T>()--));

  template <typename T>
  using _DifferenceT = decltype(val<T>() - val<T>());

template <typename T>
using RandomAccessIterator =
  decltype(valid_expr(models<BidirectionalIterator<T>>(),
                      has_type<T &>(ref<T>() += val<_DifferenceT<T>>()),
                      has_type<T>(val<T>() + val<_DifferenceT<T>>()),
                      has_type<T>(val<_DifferenceT<T>>() + val<T>()),
                      has_type<T &>(ref<T>() -= val<_DifferenceT<T>>()),
                      has_type<T>(val<T>() - val<_DifferenceT<T>>()),
                      val<T>()[val<_DifferenceT<T>>()]));

} // end namespace concepts_raja

namespace RAJA {
namespace concepts {

  using concepts_raja::EqualityComparable;
  using concepts_raja::LessThanComparable;
  using concepts_raja::GreaterThanComparable;
  using concepts_raja::LessEqualComparable;
  using concepts_raja::GreaterEqualComparable;

  using concepts_raja::Comparable;
  using concepts_raja::ComparableTo;

  using concepts_raja::Iterator;
  using concepts_raja::ForwardIterator;
  using concepts_raja::BidirectionalIterator;
  using concepts_raja::RandomAccessIterator;

} // end namespace concepts
} // end namespace RAJA

#endif
