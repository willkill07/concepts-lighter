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

You will have noticed that there is one common idiom present in this implementation: all concepts are defined as a single alias assigned to the result of `decltype(valid_expr(...))`. There is a convenience macro, `DefineConcept(` which will expand to `decltype(concepts::valid_expr(` and automatically inject the additional closing parenthesis.

Below is an example of Iterator concepts using the convenience macro (and assuming `using namespace concepts`)
```cpp

template <typename T>
using Iterator = DefineConcept(
  *ref<T>(),                // checks dereference
  has_type<T &>(++ref<T>()) // checks return type from preincrement
);

template <typename T>
using ForwardIterator = DefineConcept(
  models<Iterator<T>>(), // ForwardIterator models Iterator
  ref<T>()++,            // and provides post-increment
  *ref<T>()++            // and dereferencing a post-increment expression
);

template <typename T>
using BidirectionalIterator = DefineConcept(
  models<ForwardIterator<T>>(),          // BidirectionalIterator models ForwardIterator
  has_type<T &>(--ref<T>()),             // and provides pre-decrement
  convertible_to<T const &>(ref<T>()--), // and post-decrement
  *ref<T>()--                            // and dereferencing a post-decrement
);

template <typename T>
using diff_t = decltype(val<T>() - val<T>()); // alias for difference_type

template <typename T>
using RandomAccessIterator = DefineConcept(
  models<BidirectionalIterator<T>>(),          // RandomAccessIterator models BidirectionalIterator
  has_type<T &>(ref<T>() += val<diff_t<T>>()), // allows for arbitrary advancement
  has_type<T>(val<T>() + val<diff_t<T>>()),    // addition to difference_type
  has_type<T>(val<diff_t<T>>() + val<T>()),    // --- where order does not matter
  has_type<T &>(ref<T>() -= val<diff_t<T>>()), // arbitrary decrement modifying
  has_type<T>(val<T>() - val<diff_t<T>>()),    // and by unmodified with expression
  val<T>()[val<diff_t<T>>()]                   // and allows indexing with operator[]
);
```

Further examples and usage patterns will be described in the future

## Design

### A concept:

* is a type alias which when fully evaluated *can* produce a compiler error
* provides a protected evaluation context of arbitrary code
* only detects if operations exist and emit the correct type

### What can a concept do?

* checks if it models another concept (`models<T>()`)
* checks if an expression is valid (`val<T>() == val<T>()`)
* checks if an expression has some result type (`has_type<T>(expr)`)
* checks if an expression has some result type convertible to another (`convertible_to<T>(expr)`)

### What can go in a concept expression?

* type helpers for creating instances of types which may not always be default constructible (`val<T>`, `cval<T>`, `ref<T>`, `cref<T>`
* any aribitrary code not dependent on any code out of the scope of the concept definition (can only use types)

### What is the syntax for a concept?

A concept first starts with defining the types of which the concept depends on. For example, if we are writing a concept that depends on an aribitrary value type, perhaps `typename Type` might be enough. If we are writing a concept to see if a type models an Iterator of some type, we may want to use `typename Iter`. If we need to check the relations between two types, we will need to define two types as template parameters: `typename T, typename U`

```cpp
template <typename Iter>
using Iterator = // ...
```

So far that's our concept definition. We specified our template arguments and the name of our concept. Next we will look at defining some number of expressions required by `Iterator`. First we need to indicate we are writing a concept instead of a type alias.

```cpp
template <typename Iter>
using Iterator = DefineConcept( ... );

// OR

template <typename Iter>
using Iterator = decltype(concepts::valid_expr( ... ));
```

Either syntax is acceptable (`DefineConcept` is a macro). Now we need to determine what are valid expressions in the concept of an `Iterator`. According to the Ranges TS, an `Iterator` requires the functionality of pre-incrementing (`operator++`) and dereferencing (`operator*`).

Let us first look at normal usages:

```cpp
Iter i; // we have an instance of Iter
++i; // preincrement -- return type should be Iter&
*i;  // dereference -- unspecified return type
```

Since we are not able to declare any variables in a type definition, we will leverage the `ref<T>()` component provided in this concepts framework:

```cpp
concepts::has_type<Iter&>(++concepts::ref<Iter>());
*concepts::ref<Iter>();
```

We no longer have any variable definitions, and can easily replace the semicolons of separate statements to be comma-separated for our expression list:

```cpp
template <typename Iter>
using Iterator = DefineConcept(
  concepts::has_type<Iter&>(++concepts::ref<Iter>()),
  *concepts::ref<Iter>()
);
```

This is a complete concept definition! Look further up at `ForwardIterator` to see extended implementations using `ref<T>()`, `has_type<T>(expr)`, and `models<Concept>()`.

## License

This software is provided under a dual-license scheme (Boost License + BSD 3-clause)

The Boost License is use for relevant code from `ericneibler/range-v3` while the BSD 3-clause license originates from `LLNL/RAJA`.
