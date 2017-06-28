#include "concepts.hpp"

namespace concepts {
namespace detail {

  RAJA_DEFINE_CONCEPT((T,U), Comparable, \
                      RAJA_CONCEPT_VAL(U) < RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) < RAJA_CONCEPT_VAL(U), \
                      RAJA_CONCEPT_VAL(U) <= RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) <= RAJA_CONCEPT_VAL(U), \
                      RAJA_CONCEPT_VAL(U) > RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) > RAJA_CONCEPT_VAL(U), \
                      RAJA_CONCEPT_VAL(U) >= RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) >= RAJA_CONCEPT_VAL(U), \
                      RAJA_CONCEPT_VAL(U) == RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) == RAJA_CONCEPT_VAL(U), \
                      RAJA_CONCEPT_VAL(U) != RAJA_CONCEPT_VAL(T), \
                      RAJA_CONCEPT_VAL(T) != RAJA_CONCEPT_VAL(U));

  RAJA_DEFINE_CONCEPT((T), Iterable,                \
                      RAJA_CONCEPT_REF(T).begin(),      \
                      RAJA_CONCEPT_REF(T).end(),        \
                      RAJA_CONCEPT_CREF(T).begin(),     \
                      RAJA_CONCEPT_CREF(T).end());

  RAJA_DEFINE_CONCEPT((T), HasMemberBegin, RAJA_CONCEPT_REF(T).begin());
  RAJA_DEFINE_CONCEPT((T), HasMemberEnd, RAJA_CONCEPT_REF(T).end());
  RAJA_DEFINE_CONCEPT((T), HasConstMemberBegin, RAJA_CONCEPT_CREF(T).begin());
  RAJA_DEFINE_CONCEPT((T), HasConstMemberEnd, RAJA_CONCEPT_CREF(T).end());

}

template <typename Container>
struct Range : RAJA::requires<detail::HasMemberEnd<Container>,
                              detail::HasMemberBegin<Container>,
                              detail::HasConstMemberEnd<Container>,
                              detail::HasConstMemberBegin<Container>> {
};

template <typename Container>
struct Iterable : detail::Iterable<Container> {
};

template <typename T, typename U = T>
struct Comparable : detail::Comparable<T,U> {
};

}

#include <iostream>
#include <vector>

struct MyThing {
  bool operator< (MyThing) { return true; }
  bool operator<= (MyThing) { return true; }
  bool operator> (MyThing) { return true; }
  bool operator>= (MyThing) { return true; }
  bool operator!= (MyThing) { return true; }
};

static_assert(concepts::Range<std::vector<int>>::value, "This should not fail");
static_assert(concepts::Range<int>::value, "This should fail");
static_assert(concepts::Iterable<std::vector<int>>::value, "This should not fail");
static_assert(concepts::Iterable<int>::value, "This should fail");
static_assert(concepts::Comparable<int,double>::value, "This should not fail");
static_assert(concepts::Comparable<MyThing, MyThing>::value, "This should fail");
static_assert(concepts::Comparable<int, double>::value, "This should not fail");
