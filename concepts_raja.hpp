#ifndef RAJA_CONCEPTS_HPP_
#define RAJA_CONCEPTS_HPP_

#include "concepts.hpp"

namespace ___hidden_concepts_raja {

using namespace concepts;

template <typename T>
using LessThanComparable = DefineConcept(
  convertible_to<bool>(val<T>() < val<T>()));

template <typename T>
using GreaterThanComparable = DefineConcept(
  convertible_to<bool>(val<T>() > val<T>()));

template <typename T>
using LessEqualComparable = DefineConcept(
  convertible_to<bool>(val<T>() <= val<T>()));

template <typename T>
using GreaterEqualComparable = DefineConcept(
  convertible_to<bool>(val<T>() >= val<T>()));

template <typename T>
using EqualityComparable = DefineConcept(
  convertible_to<bool>(val<T>() == val<T>()));

template <typename T, typename U>
using ComparableTo = DefineConcept(
  convertible_to<bool>(val<U>() < val<T>()),
  convertible_to<bool>(val<T>() < val<U>()),
  convertible_to<bool>(val<U>() <= val<T>()),
  convertible_to<bool>(val<T>() <= val<U>()),
  convertible_to<bool>(val<U>() > val<T>()),
  convertible_to<bool>(val<T>() > val<U>()),
  convertible_to<bool>(val<U>() >= val<T>()),
  convertible_to<bool>(val<T>() >= val<U>()),
  convertible_to<bool>(val<U>() == val<T>()),
  convertible_to<bool>(val<T>() == val<U>()),
  convertible_to<bool>(val<U>() != val<T>()),
  convertible_to<bool>(val<T>() != val<U>()));

template <typename T> using Comparable = ComparableTo<T, T>;


template <typename T> using diff_t = decltype(val<T>() - val<T>());
template <typename T> using iterator_t = decltype(ref<T>().begin());

template <typename T>
using Iterator = DefineConcept(
  *val<T>(),
  has_type<T &>(++ref<T>()));

template <typename T>
using ForwardIterator = DefineConcept(
  models<Iterator<T>>(),
  ref<T>()++,
  *ref<T>()++);

template <typename T>
using BidirectionalIterator = DefineConcept(
  models<ForwardIterator<T>>(),
  has_type<T &>(--ref<T>()),
  convertible_to<T const &>(ref<T>()--),
  *ref<T>()--);

template <typename T>
using RandomAccessIterator = DefineConcept(
  models<BidirectionalIterator<T>>(),
  models<Comparable<T>>(),
  has_type<T &>(ref<T>() += val<diff_t<T>>()),
  has_type<T>(val<T>() + val<diff_t<T>>()),
  has_type<T>(val<diff_t<T>>() + val<T>()),
  has_type<T &>(ref<T>() -= val<diff_t<T>>()),
  has_type<T>(val<T>() - val<diff_t<T>>()),
  val<T>()[val<diff_t<T>>()]);

template <typename T>
using HasMemberBegin = DefineConcept(ref<T>().begin());

template <typename T>
using HasMemberEnd = DefineConcept(ref<T>().end());

template <typename T>
using HasBeginEnd = DefineConcept(
  models<HasMemberBegin<T>>(),
  models<HasMemberEnd<T>>());

template <typename T>
using Range = DefineConcept(
  models<HasBeginEnd<T>>(),
  models<Iterator<iterator_t<T>>>());

template <typename T>
using ForwardRange = DefineConcept(
  models<HasBeginEnd<T>>(),
  models<ForwardIterator<iterator_t<T>>>());

template <typename T>
using BidirectionalRange = DefineConcept(
  models<HasBeginEnd<T>>(),
  models<BidirectionalIterator<iterator_t<T>>>());

template <typename T>
using RandomAccessRange = DefineConcept(
  models<HasBeginEnd<T>>(),
  models<RandomAccessIterator<iterator_t<T>>>());

} // end namespace concepts_raja

namespace RAJA {
namespace concepts {

using ___hidden_concepts_raja::EqualityComparable;
using ___hidden_concepts_raja::LessThanComparable;
using ___hidden_concepts_raja::GreaterThanComparable;
using ___hidden_concepts_raja::LessEqualComparable;
using ___hidden_concepts_raja::GreaterEqualComparable;

using ___hidden_concepts_raja::Comparable;
using ___hidden_concepts_raja::ComparableTo;

using ___hidden_concepts_raja::Iterator;
using ___hidden_concepts_raja::ForwardIterator;
using ___hidden_concepts_raja::BidirectionalIterator;
using ___hidden_concepts_raja::RandomAccessIterator;

using ___hidden_concepts_raja::Range;
using ___hidden_concepts_raja::ForwardRange;
using ___hidden_concepts_raja::BidirectionalRange;
using ___hidden_concepts_raja::RandomAccessRange;

} // end namespace concepts
} // end namespace RAJA

#endif
