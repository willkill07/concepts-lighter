#include "detector.hpp"
#include "preprocessor.hpp"

#define __RAJA_INTERNAL_PREFIX_MANGLE(X) RAJA_CONCEPT_TYPE_ ## X
#define __RAJA_INTERNAL_PREFIX_TYPENAME(X) typename __RAJA_INTERNAL_PREFIX_MANGLE(X)

#define RAJA_CONCEPT_REF(X) std::declval<__RAJA_INTERNAL_PREFIX_MANGLE(X)&>()
#define RAJA_CONCEPT_CREF(X) std::declval<__RAJA_INTERNAL_PREFIX_MANGLE(X) const&>()
#define RAJA_CONCEPT_VAL(X) std::declval<__RAJA_INTERNAL_PREFIX_MANGLE(X)>()

#define __RAJA_INTERNAL_CONCEPT_TEMPLATE(...) \
  template<MAP_WITH_COMMA(__RAJA_INTERNAL_PREFIX_TYPENAME, __VA_ARGS__)>

#define __RAJA_INTERNAL_CONCEPT_INSTANCE(...) \
  MAP_WITH_COMMA(__RAJA_INTERNAL_PREFIX_MANGLE, __VA_ARGS__)

#define RAJA_DEFINE_CONCEPT(TYPES, TAG, ...)\
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)   \
    using ___ ## TAG ## _impl = decltype(__VA_ARGS__);      \
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)                   \
    using TAG = is_detected< ___ ## TAG ## _impl, EVAL(DEFER1(__RAJA_INTERNAL_CONCEPT_INSTANCE)TYPES) >

#define RAJA_DEFINE_CONCEPT_CONVERTIBLE(TYPES, TAG, RESULT, ...) \
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)   \
    using ___ ## TAG ## _impl = decltype(__VA_ARGS__);      \
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)                   \
    using TAG = is_detected_convertible< RESULT, ___ ## TAG ## _impl, EVAL(DEFER1(__RAJA_INTERNAL_CONCEPT_INSTANCE)TYPES) >

#define RAJA_DEFINE_CONCEPT_EXACT(TYPES, TAG, RESULT, ...) \
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)   \
    using ___ ## TAG ## _impl = decltype(__VA_ARGS__);      \
  EVAL(DEFER2(__RAJA_INTERNAL_CONCEPT_TEMPLATE)TYPES)                   \
    using TAG = is_detected_exact< RESULT, ___ ## TAG ## _impl, EVAL(DEFER1(__RAJA_INTERNAL_CONCEPT_INSTANCE)TYPES) >

namespace RAJA {
namespace impl {

template <bool...>
struct blist;

template <typename ... Bs>
using all_of_t = std::is_same<blist<true, Bs::value...>, blist<Bs::value..., true>>;

}

template <typename ... Ts>
using requires = impl::all_of_t<Ts...>;

}
