#ifndef CONCEPTS_HPP_
#define CONCEPTS_HPP_

#include "impl/metalib.hpp"
#include "impl/detector.hpp"

namespace concepts {

/// metafunction to get instance of value type for concepts
template <typename T> auto val() -> decltype(std::declval<T>()) {
  return std::declval<T>();
}

/// metafunction to get instance of const type for concepts
template <typename T> auto cval() -> decltype(std::declval<T const>()) {
  return std::declval<T const>();
}

/// metafunction to get instance of reference type for concepts
template <typename T> auto ref() -> decltype(std::declval<T &>()) {
  return std::declval<T &>();
}

/// metafunction to get instance of const reference type for concepts
template <typename T> auto cref() -> decltype(std::declval<T const &>()) {
  return std::declval<T const &>();
}

using metalib::has_type;
using metalib::convertible_to;
using metalib::models;
using metalib::returns;
using metalib::valid_expr;

/// metaprogramming concept for SFINAE checking of concepts
template <template <typename...> class Thing, typename ... Args>
using requires_ = is_detected<Thing, Args...>;

/// metaprogramming concept for SFINAE checking of aggregating concepts
template <typename ... Args>
using all_of = metalib::all_of_t<Args...>;

} // concepts

/// Convenience macro which wraps `decltype(concepts::valid_expr( ... ))`
#define DefineConcept(...) \
  decltype(concepts::valid_expr( __VA_ARGS__ ))

#endif
