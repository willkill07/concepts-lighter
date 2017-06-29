# Concepts Lighter

Basic concept-like functionality in C++11

See `example.cpp` for potential usage

*NOTE:* Most of this work is accomplished through original work done by Eric Niebler with `range-v3` and his concepts implementation. I enhanced and simplified the dependence on `meta` to only include relevant metafunctions. This work also leverages the `detector` idiom proposed in N4502.

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
      concepts::val<T>() < concepts::val<T>())));
```

Please note that there are a few key components (and everything lives under `namespace concepts`:

* `concepts::valid_expr(...)` -- a function accepting a list of expressions to evaluate
* `concepts::convertible_to<T>(x)` -- a function accepting an expression that checks the return type to a passed type for conversion
* `concepts::val<T>()` -- a function accepting a type that returns a `declval` instance of `T`

There are additional components available as well:

* `concepts::has_type<T>(x)` -- a function accepting an expression that checks the return type and ensures it matches the passed type
* `concepts::models<T>()` -- a function accepting a fully-qualified concept name of which a new concept should model
* `concepts::ref<T>()` -- a function accepting a type that returns a `declval` instance of `T&`
* `concepts::cref<T>()` -- a function accepting a type that returns a `declval` instance of `T const &`
* `concepts::cval<T>()` -- a function accepting a type that returns a `declval` instance of `T const`


We are also able to easily check for total comparisons between any two types:

```cpp
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
```

Or providing a quick check for adhering to various Iterator concepts

```cpp

template <typename T>
using Iterator = decltype(valid_expr(
  *ref<T>(),                // checks dereference
  has_type<T &>(++ref<T>()) // checks return type from preincrement
));

template <typename T>
using ForwardIterator = decltype(valid_expr(
  models<Iterator<T>>(), // ForwardIterator models Iterator
  ref<T>()++,            // and provides post-increment
  *ref<T>()++            // and dereferencing a post-increment expression
));

template <typename T>
using BidirectionalIterator =
  decltype(valid_expr(
    models<ForwardIterator<T>>(),          // BidirectionalIterator models ForwardIterator
    has_type<T &>(--ref<T>()),             // and provides pre-decrement
    convertible_to<T const &>(ref<T>()--), // and post-decrement
    *ref<T>()--                            // and dereferencing a post-decrement
));


template <typename T>
using diff_t = decltype(val<T>() - val<T>());

template <typename T>
using RandomAccessIterator =
  decltype(valid_expr(models<BidirectionalIterator<T>>(),
                      has_type<T &>(ref<T>() += val<diff_t<T>>()),
                      has_type<T>(val<T>() + val<diff_t<T>>()),
                      has_type<T>(val<diff_t<T>>() + val<T>()),
                      has_type<T &>(ref<T>() -= val<diff_t<T>>()),
                      has_type<T>(val<T>() - val<diff_t<T>>()),
                      val<T>()[val<diff_t<T>>()]));
```

Further examples and usage patterns will be described in the future
