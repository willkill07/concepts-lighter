#ifndef RAJA_CONCEPTS_HPP_
#define RAJA_CONCEPTS_HPP_

#include "metalib.hpp"
#include "concepts.hpp"

namespace RAJA {
namespace concepts {
namespace detail {

template <typename T>
using LessThanComparable =
    decltype(valid_expr(convertible_to<bool>(Val<T>() < Val<T>())));

template <typename T>
using GreaterThanComparable =
    decltype(valid_expr(convertible_to<bool>(Val<T>() > Val<T>())));

template <typename T>
using LessEqualComparable =
    decltype(valid_expr(convertible_to<bool>(Val<T>() <= Val<T>())));

template <typename T>
using GreaterEqualComparable =
    decltype(valid_expr(convertible_to<bool>(Val<T>() >= Val<T>())));

template <typename T>
using EqualityComparable =
    decltype(valid_expr(convertible_to<bool>(Val<T>() == Val<T>())));

template <typename T, typename U>
using ComparableTo = decltype(valid_expr(
    convertible_to<bool>(Val<U>() <  Val<T>()),
    convertible_to<bool>(Val<T>() <  Val<U>()),
    convertible_to<bool>(Val<U>() <= Val<T>()),
    convertible_to<bool>(Val<T>() <= Val<U>()),
    convertible_to<bool>(Val<U>() >  Val<T>()),
    convertible_to<bool>(Val<T>() >  Val<U>()),
    convertible_to<bool>(Val<U>() >= Val<T>()),
    convertible_to<bool>(Val<T>() >= Val<U>()),
    convertible_to<bool>(Val<U>() == Val<T>()),
    convertible_to<bool>(Val<T>() == Val<U>()),
    convertible_to<bool>(Val<U>() != Val<T>()),
    convertible_to<bool>(Val<T>() != Val<U>())));

template <typename T> using Comparable = ComparableTo<T, T>;

template <typename T>
using Iterator = decltype(valid_expr(*Ref<T>(),
                                     has_type<T &>(++Ref<T>())));

template <typename T>
using ForwardIterator =
    decltype(valid_expr(models<Iterator<T>>(),
                        Ref<T>()++,
                        *Ref<T>()++));

template <typename T>
using BidirectionalIterator =
    decltype(valid_expr(models<ForwardIterator<T>>(),
                        has_type<T &>(--Ref<T>()),
                        convertible_to<T const &>(Ref<T>()--),
                        *Ref<T>()--));

  template <typename T>
  using _DifferenceT = decltype(Val<T>() - Val<T>());

template <typename T>
using RandomAccessIterator =
  decltype(valid_expr(models<BidirectionalIterator<T>>(),
                      has_type<T &>(Ref<T>() += Val<_DifferenceT<T>>()),
                      has_type<T>(Val<T>() + Val<_DifferenceT<T>>()),
                      has_type<T>(Val<_DifferenceT<T>>() + Val<T>()),
                      has_type<T &>(Ref<T>() -= Val<_DifferenceT<T>>()),
                      has_type<T>(Val<T>() - Val<_DifferenceT<T>>()),
                      Val<T>()[Val<_DifferenceT<T>>()]));

} // end namespace detail

// pull in relevant concepts from detail
using detail::Iterator;
using detail::ForwardIterator;
using detail::BidirectionalIterator;
using detail::RandomAccessIterator;
using detail::Comparable;
using detail::ComparableTo;

} // end namespace concepts

} // end namespace RAJA

#endif
