// concept framework header
#include "concepts.hpp"

// load some concepts useful to RAJA
#include "concepts_raja.hpp"

#include <vector>

struct A {};

using namespace RAJA::concepts;

// helper alias for extracting value_type from iterator
template <typename Iter>
using Value = decltype(*std::declval<Iter>());

// example for SFINAE aggregation of concrete concepts
// -- this is the TMP syntax
template <typename Iter>
using RandomAccessComparable = all_of<
  requires_<RandomAccessIterator, Iter>,
  requires_<Comparable, Value<Iter>>>;

// Alternatively here -- this is the "concepts" syntax
template <typename Iter>
using RandomAccessComparable2 = decltype(meta::valid_expr(
    models<RandomAccessIterator<Iter>>(),
    models<Comparable<Value<Iter>>>()));

// For when you want verbose errors emitted
// -- note this is NOT SFINAE friendly
using CheckComparable = Comparable<A>;
using CheckComparable2 = RandomAccessComparable2<A*>;

// SFINAE example
static_assert(requires_<RandomAccessIterator, int*>::value, "should not fail");
static_assert(requires_<RandomAccessIterator, std::vector<int>::iterator>::value, "should not fail");
static_assert(requires_<Comparable, double>::value, "should pass");
static_assert(requires_<Comparable, A>::value, "should fail");
static_assert(RandomAccessComparable<float*>::value, "should pass");
static_assert(RandomAccessComparable<A*>::value, "should fail");
static_assert(requires_<RandomAccessComparable2, float*>::value, "should pass");
static_assert(requires_<RandomAccessComparable2, A*>::value, "should fail");

int
main() {
  return 0;
}
