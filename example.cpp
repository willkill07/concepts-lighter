#include "concepts_raja.hpp"

#include <vector>

struct A {};

// For SFINAE friendliness
static_assert(RAJA::Requires<RAJA::concepts::RandomAccessIterator, int*>::value, "should not fail");
static_assert(RAJA::Requires<RAJA::concepts::RandomAccessIterator, std::vector<int>::iterator>::value, "should not fail");
static_assert(RAJA::Requires<RAJA::concepts::Comparable, A>::value, "should fail");

// And for when you want verbose errors
using CheckComparable = RAJA::concepts::Comparable<A>;
