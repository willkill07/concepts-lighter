// concept framework header
#include "concepts.hpp"

// load some concepts useful to RAJA
#include "concepts_raja.hpp"

#include <vector>

struct A {};

// helper alias for extracting value_type from iterator
template <typename Iter>
using Value = decltype(*std::declval<Iter>());

// example for SFINAE aggregation of concrete concepts
// -- this is the TMP syntax
template <typename Iter>
using RandomAccessComparable = concepts::all_of<
  concepts::requires_<RAJA::concepts::RandomAccessIterator, Iter>,
  concepts::requires_<RAJA::concepts::Comparable, Value<Iter>>>;

// Alternatively here -- this is the "concepts" syntax
template <typename Iter>
using RandomAccessComparable2 = DefineConcept(
    // DefineConcept( --- alternatively write out decltype(valid_expr( and add matching rparen
    concepts::models<RAJA::concepts::RandomAccessIterator<Iter>>(),
    concepts::models<RAJA::concepts::Comparable<Value<Iter>>>());

#if 0
// For when you want verbose errors emitted
// -- note this is NOT SFINAE friendly
using CheckComparable = RAJA::concepts::Comparable<A>;
using CheckComparable2 = RandomAccessComparable2<A*>;
#endif

template <typename ConcreteConcept>
bool test () {
  return ConcreteConcept::value;
}

template <template <typename...> class UninstantiatedConcept, typename ... Args>
bool test () {
  return concepts::requires_<UninstantiatedConcept, Args...>::value;
}

#define START_TESTS                             \
  int main() {                                  \
  int totaltests=0;                             \
  int passedtests=0;                            \
  int failedtests=0;

#define END_TESTS \
  printf("\n\n[ %d / %d ] TESTS PASSED\n", passedtests, totaltests); \
  return passedtests != totaltests; \
}

#define TEST(EXPECTED, ...) \
  do { \
    bool expected = EXPECTED; \
    bool actual = test< __VA_ARGS__ >(); \
    char const* const options[2] = { "[ FAILED ]", "[ PASSED ]" }; \
    printf("%s " #__VA_ARGS__ "  -> " #EXPECTED "\n", options[expected == actual]); \
    ++totaltests; \
    passedtests += (expected == actual); \
    failedtests += (expected != actual); \
  } while(0)


START_TESTS {
TEST(true, RAJA::concepts::RandomAccessIterator, int*);
TEST(true, RAJA::concepts::RandomAccessIterator, std::vector<int>::iterator);
TEST(true, RAJA::concepts::Comparable, double);
TEST(false, RAJA::concepts::Comparable, A);
TEST(true, RandomAccessComparable<float*>);
TEST(false, RandomAccessComparable<A*>);
TEST(true, RandomAccessComparable2, float*);
TEST(false, RandomAccessComparable2, A*);
} END_TESTS

#if 0

// SFINAE example
static_assert(concepts::requires_<RAJA::concepts::RandomAccessIterator, int*>::value, "should pass");
static_assert(concepts::requires_<RAJA::concepts::RandomAccessIterator, std::vector<int>::iterator>::value, "should pass");
static_assert(concepts::requires_<RAJA::concepts::Comparable, double>::value, "should pass");
static_assert(concepts::requires_<RAJA::concepts::Comparable, A>::value, "should fail");

// Note: the TMP syntax of concrete concept aggregation can be tested with ::value
static_assert(RandomAccessComparable<float*>::value, "should pass");
static_assert(RandomAccessComparable<A*>::value, "should fail");

// Note: the concepts syntax typedef requires a wrap to concepts::requires_<T>::value
static_assert(concepts::requires_<RandomAccessComparable2, float*>::value, "should pass");
static_assert(concepts::requires_<RandomAccessComparable2, A*>::value, "should fail");

int main() {
  return 0;
}

#endif
