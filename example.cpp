#include "concepts.hpp"

namespace RAJA {
namespace concepts {
namespace detail {

using RAJA::concepts::detail::CRef;
using RAJA::concepts::detail::Ref;
using RAJA::concepts::detail::Val;

using RAJA::concepts::detail::meta::convertible_to;
using RAJA::concepts::detail::meta::has_type;
using RAJA::concepts::detail::meta::models;
using RAJA::concepts::detail::meta::returns;
using RAJA::concepts::detail::meta::valid_expr;

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
using RandomAccessIterator =
  decltype(valid_expr(models<BidirectionalIterator<T>>(),
                      has_type<T &>(Ref<T>() += (Val<T>() - Val<T>())),
                      has_type<T>(Val<T>() + (Val<T>() - Val<T>())),
                      has_type<T>((Val<T>() - Val<T>()) + Val<T>()),
                      has_type<T &>(Ref<T>() -= (Val<T>() - Val<T>())),
                      has_type<T>(Val<T>() - (Val<T>() - Val<T>())),
                      Val<T>()[(Val<T>() - Val<T>())]));

} // end namespace detail

using detail::Iterator;
using detail::ForwardIterator;
using detail::BidirectionalIterator;
using detail::RandomAccessIterator;
using detail::Comparable;
using detail::ComparableTo;

template <typename T, typename U> using DerivedFrom = std::is_base_of<U, T>;

template <typename T, typename U>
using ConvertibleTo = std::is_convertible<T, U>;

} // end namespace concepts

} // end namespace RAJA

#include <vector>

struct A {};

static_assert(std::is_same<RAJA::concepts::RandomAccessIterator<int*>, std::true_type>::value, "Should not fail");
static_assert(std::is_same<RAJA::concepts::RandomAccessIterator<std::vector<int>::iterator>, std::true_type>::value, "Should not fail");
static_assert(std::is_same<RAJA::concepts::Comparable<A>, std::true_type>::value, "Should fail");
