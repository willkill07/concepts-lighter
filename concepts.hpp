#ifndef CONCEPTS_HPP_
#define CONCEPTS_HPP_

#include "detector.hpp"

namespace RAJA {
namespace concepts {
namespace detail {
namespace meta {

template <typename...> struct list;

template <bool B> using bool_ = std::integral_constant<bool, B>;

namespace impl {
#ifdef __clang__
// Clang is faster with this implementation
template <typename, typename = bool> struct _if_ {};

template <typename If>
struct _if_<list<If>, decltype(bool(If::type::value))>
    : std::enable_if<If::type::value> {};

template <typename If, typename Then>
struct _if_<list<If, Then>, decltype(bool(If::type::value))>
    : std::enable_if<If::type::value, Then> {};

template <typename If, typename Then, typename Else>
struct _if_<list<If, Then, Else>, decltype(bool(If::type::value))>
    : std::conditional<If::type::value, Then, Else> {};
#else
// GCC seems to prefer this implementation
template <typename, typename = std::true_type> struct _if_ {};

template <typename If> struct _if_<list<If>, bool_<If::type::value>> {
  using type = void;
};

template <typename If, typename Then>
struct _if_<list<If, Then>, bool_<If::type::value>> {
  using type = Then;
};

template <typename If, typename Then, typename Else>
struct _if_<list<If, Then, Else>, bool_<If::type::value>> {
  using type = Then;
};

template <typename If, typename Then, typename Else>
struct _if_<list<If, Then, Else>, bool_<!If::type::value>> {
  using type = Else;
};
#endif
} // namespace detail

template <typename... Ts> using if_ = typename impl::_if_<list<Ts...>>::type;

template <bool If, typename... Args>
using if_c = typename impl::_if_<list<bool_<If>, Args...>>::type;

constexpr struct valid_expr_t {
  template <typename... T> std::true_type operator()(T &&...) const;
} valid_expr{};

constexpr struct is_true_t {
  template <typename Bool_>
  auto operator()(Bool_) const -> if_c<Bool_::value, std::true_type>;
} is_true{};

constexpr struct is_false_t {
  template <typename Bool_>
  auto operator()(Bool_) const -> if_c<!Bool_::value, std::true_type>;
} is_false{};

template <typename Ret, typename T> Ret returns(T const &);

template <typename T, typename U>
auto convertible_to(U &&u) -> decltype(returns<std::true_type>(static_cast<T>((U &&) u)));

template <typename T, typename U>
auto has_type(U &&) -> if_<std::is_same<T, U>, std::true_type>;


template <bool...> struct blist;

template <typename... Bs>
using all_of_t =
    std::is_same<blist<true, Bs::value...>, blist<Bs::value..., true>>;

template <typename T>
auto models() -> if_<std::is_same<std::true_type, T>, std::true_type>;

} // meta

template <typename T> auto Val() -> decltype(std::declval<T>()) {
  return std::declval<T>();
}
template <typename T> auto Ref() -> decltype(std::declval<T &>()) {
  return std::declval<T &>();
}
template <typename T> auto CRef() -> decltype(std::declval<T const &>()) {
  return std::declval<T const &>();
}

} // detail
} // concepts

template <template <typename...> class Thing, typename ... Args>
using Requires = is_detected<Thing, Args...>;

} // RAJA

#endif
