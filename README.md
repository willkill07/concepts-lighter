# Concepts Lighter

Basic concept-like functionality in C++11


See `example.cpp` for potential usage


## Goals

* Provide a concepts facility in C++11 without any third-party libraries
* Emulate the interface reasonably close
* Aim for high-throughput by prefering aliases to new struct definitions

## Non-Goals

* Provide a complete concepts replacement for use until Concepts are added to the C++ language
* Implement the entire Concepts TS and Ranges TS concept list

## Usage

A very simple usage pattern is illustrated below

```cpp
template <typename T>
using LessThanComparable =
  decltype(concepts::valid_expr(
    concepts::convertible_to<bool>(
      concepts::detail::Val<T>() < concepts::detail::Val<T>())));
```

Please note that there are a few key components:

* `concepts::valid_expr(...)` -- a function accepting a list of expressions to evaluate
* `concepts::convertible_to<T>(x)` -- a function accepting an expression that checks the return type to a passed type for conversion
* `concepts::detail::Val<T>()` -- a function accepting a type that returns a `declval` instance of `T`

There are additional components available as well:

* `concepts::has_type<T>(x)` -- a function accepting an expression that checks the return type and ensures it matches the passed type
* `concepts::models<T>()` -- a function accepting a fully-qualified concept name of which a new concept should model
* `concepts::detail::Ref<T>()` -- a function accepting a type that returns a `declval` instance of `T&`
* `concepts::detail::CRef<T>()` -- a function accepting a type that returns a `declval` instance of `T const &`
* `concepts::detail::CVal<T>()` -- a function accepting a type that returns a `declval` instance of `T const`


We are also able to easily check for total comparisons between any two types:

```cpp
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
```

Or providing a quick check for adhering to various Iterator concepts

```cpp

template <typename T>
using Iterator = decltype(valid_expr(
  *Ref<T>(),                 // checks dereference
  has_type<T &>(++Ref<T>()) // checks return type from preincrement
));

template <typename T>
using ForwardIterator = decltype(valid_expr(
  models<Iterator<T>>(), // ForwardIterator models Iterator
  Ref<T>()++,            // and provides post-increment
  *Ref<T>()++            // and dereferencing a post-increment expression
));

template <typename T>
using BidirectionalIterator =
  decltype(valid_expr(
    models<ForwardIterator<T>>(),  // BidirectionalIterator models ForwardIterator
    has_type<T &>(--Ref<T>()),     // and provides pre-decrement
    convertible_to<T const &>(Ref<T>()--), // and post-decrement
    *Ref<T>()--                            // and dereferencing a post-decrement
));


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
```

Further examples and usage patterns will be described in the future
