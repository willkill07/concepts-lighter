#ifndef CONCEPTS_HPP_
#define CONCEPTS_HPP_

#include "metalib.hpp"
#include "detector.hpp"

namespace RAJA {
namespace concepts {
namespace detail {

template <typename T> auto Val() -> decltype(std::declval<T>()) {
  return std::declval<T>();
}
template <typename T> auto Ref() -> decltype(std::declval<T &>()) {
  return std::declval<T &>();
}
template <typename T> auto CRef() -> decltype(std::declval<T const &>()) {
  return std::declval<T const &>();
}

using meta::has_type;

} // detail

using meta::convertible_to;
using meta::models;
using meta::returns;
using meta::valid_expr;

template <template <typename...> class Thing, typename ... Args>
using requires_ = is_detected<Thing, Args...>;

template <typename ... Args>
using all_of = meta::all_of_t<Args...>;

} // concepts

} // RAJA

#endif
