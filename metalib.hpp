#ifndef METALIB_HPP_
#define METALIB_HPP_

#include <type_traits>

// Adopted from range-v3 and Eric Niebler's meta library
// Only extracted relevent parts
namespace metalib {

template <typename...>
struct list;

template <bool B>
using bool_ = std::integral_constant<bool, B>;

namespace impl {
#ifdef __clang__
// Clang is faster with this implementation
template <typename, typename = bool>
struct _if_ {};

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
template <typename, typename = std::true_type>
struct _if_ {};

template <typename If>
struct _if_<list<If>, bool_<If::type::value>> {
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

template <typename... Ts>
using if_ = typename impl::_if_<list<Ts...>>::type;

template <bool If, typename... Args>
using if_c = typename impl::_if_<list<bool_<If>, Args...>>::type;


// return std::true_type instead of int

template <typename Bool_>
constexpr auto is_true(Bool_) -> if_c<Bool_::value, std::true_type>;

template <typename Bool_>
constexpr auto is_false(Bool_) -> if_c<!Bool_::value, std::true_type>;

// concept checking functions

/// metafunction for use within decltype expression to validate aribitrary expressions
template <typename... T>
constexpr std::true_type
valid_expr(T &&...);

/// helper metafunction for use within decltype expression for validation
template <typename Ret, typename T>
constexpr Ret
returns(T const &);

/// metafunction for use within decltype expression to validate return type is convertible to given type
template <typename T, typename U>
constexpr auto
convertible_to(U &&u)
    -> decltype(returns<std::true_type>(static_cast<T>((U &&) u)));

/// metafunction for use within decltype expression to validate type of expression
template <typename T, typename U>
constexpr auto has_type(U &&) -> if_<std::is_same<T, U>, std::true_type>;

// useful utility to require all to match

template <bool...>
struct blist;

template <typename... Bs>
using all_of_t =
    std::is_same<blist<true, Bs::value...>, blist<Bs::value..., true>>;

/// helper metafunction for use within `valid_expr` for concept requirement aggregation
template <typename T>
auto
models() -> if_<std::is_same<std::true_type, T>, std::true_type>;

} // meta

#endif
