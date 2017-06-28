#include "concepts.hpp"

namespace RAJA {
namespace concepts {
namespace detail {
namespace iter {

// Iterator

RAJA_DEFINE_CONCEPT((T), Dereferencable, *RAJA_CONCEPT_REF(T));
RAJA_DEFINE_CONCEPT_EXACT((T), PreIncrementable, RAJA_CONCEPT_TYPE(T) &,
                          ++RAJA_CONCEPT_REF(T));

// ForwardIterator

RAJA_DEFINE_CONCEPT_EXACT((T), PostIncrementable, RAJA_CONCEPT_TYPE(T),
                          RAJA_CONCEPT_VAL(T)++);
RAJA_DEFINE_CONCEPT_EXACT((Iter, Ref), PostIncrementDereferencable,
                          RAJA_CONCEPT_TYPE(Ref), *RAJA_CONCEPT_VAL(Iter)++);

// BidirectionalIterator

RAJA_DEFINE_CONCEPT_EXACT((T), PreDecrementable, RAJA_CONCEPT_TYPE(T) &,
                          --RAJA_CONCEPT_VAL(T));
RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), PostDecrementable,
                                RAJA_CONCEPT_TYPE(T) const &,
                                RAJA_CONCEPT_VAL(T)--);
RAJA_DEFINE_CONCEPT_EXACT((Iter, Ref), PostDecrementDereferencable,
                          RAJA_CONCEPT_TYPE(Ref), *RAJA_CONCEPT_VAL(Iter)--);

// RandomAccessIterator

RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasPlusEquals,
                          RAJA_CONCEPT_TYPE(Iter) &,
                          RAJA_CONCEPT_REF(Iter) += RAJA_CONCEPT_VAL(DiffType));
RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasPlusLhs, RAJA_CONCEPT_TYPE(Iter),
                          RAJA_CONCEPT_VAL(Iter) + RAJA_CONCEPT_VAL(DiffType));
RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasPlusRhs, RAJA_CONCEPT_TYPE(Iter),
                          RAJA_CONCEPT_VAL(DiffType) + RAJA_CONCEPT_VAL(Iter));
RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasMinusEquals,
                          RAJA_CONCEPT_TYPE(Iter) &,
                          RAJA_CONCEPT_REF(Iter) -= RAJA_CONCEPT_VAL(DiffType));
RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasMinusLhs,
                          RAJA_CONCEPT_TYPE(Iter),
                          RAJA_CONCEPT_VAL(Iter) - RAJA_CONCEPT_VAL(DiffType));
RAJA_DEFINE_CONCEPT_EXACT((Iter, DiffType), HasMinusRhs,
                          RAJA_CONCEPT_TYPE(Iter),
                          RAJA_CONCEPT_VAL(DiffType) - RAJA_CONCEPT_VAL(Iter));
RAJA_DEFINE_CONCEPT_CONVERTIBLE((Iter, Ref, Diff), HasIndexOp,
                                RAJA_CONCEPT_TYPE(Ref),
                                RAJA_CONCEPT_VAL(Iter)[RAJA_CONCEPT_VAL(Diff)]);
} // end namespace iter

namespace comp {

  RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), LessThanComparable, bool,
                                  RAJA_CONCEPT_VAL(T) < RAJA_CONCEPT_VAL(T));
  RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), GreaterThanComparable, bool,
                                  RAJA_CONCEPT_VAL(T) > RAJA_CONCEPT_VAL(T));
  RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), LessEqualComparable, bool,
                                  RAJA_CONCEPT_VAL(T) <= RAJA_CONCEPT_VAL(T));
  RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), GreaterEqualComparable, bool,
                                  RAJA_CONCEPT_VAL(T) >= RAJA_CONCEPT_VAL(T));
  RAJA_DEFINE_CONCEPT_CONVERTIBLE((T), EqualityComparable, bool,
                                  RAJA_CONCEPT_VAL(T) == RAJA_CONCEPT_VAL(T));
} // end namespace comp

template <typename Iter>
using Iterator =
    RAJA::requires<iter::Dereferencable<Iter>, iter::PreIncrementable<Iter>>;

template <typename Iter>
using ForwardIterator =
    RAJA::requires<Iterator<Iter>, iter::PostIncrementable<Iter>,
                   iter::PostIncrementDereferencable<
                       Iter, typename std::iterator_traits<Iter>::reference>>;

template <typename Iter>
using BidirectionalIterator =
    RAJA::requires<ForwardIterator<Iter>, iter::PreDecrementable<Iter>,
                   iter::PostDecrementable<Iter>,
                   iter::PostIncrementDereferencable<
                       Iter, typename std::iterator_traits<Iter>::reference>>;

template <typename Iter, typename Diff, typename Ref>
using _RandomAccessIterator =
    RAJA::requires<BidirectionalIterator<Iter>, iter::HasPlusEquals<Iter, Diff>,
                   iter::HasPlusLhs<Iter, Diff>, iter::HasPlusRhs<Iter, Diff>,
                   iter::HasMinusEquals<Iter, Diff>,
                   iter::HasMinusLhs<Iter, Diff>, iter::HasMinusRhs<Iter, Diff>,
                   iter::HasIndexOp<Iter, Ref, Diff>,
                   comp::LessEqualComparable<Iter>,
                   comp::LessThanComparable<Iter>,
                   comp::GreaterEqualComparable<Iter>,
                   comp::GreaterThanComparable<Iter>>;

template <typename Iter>
using RandomAccessIterator =
    _RandomAccessIterator<Iter,
                          typename std::iterator_traits<Iter>::difference_type,
                          typename std::iterator_traits<Iter>::reference>;

RAJA_DEFINE_CONCEPT((T, U), Comparable,
                    RAJA_CONCEPT_VAL(U) < RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) < RAJA_CONCEPT_VAL(U),
                    RAJA_CONCEPT_VAL(U) <= RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) <= RAJA_CONCEPT_VAL(U),
                    RAJA_CONCEPT_VAL(U) > RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) > RAJA_CONCEPT_VAL(U),
                    RAJA_CONCEPT_VAL(U) >= RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) >= RAJA_CONCEPT_VAL(U),
                    RAJA_CONCEPT_VAL(U) == RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) == RAJA_CONCEPT_VAL(U),
                    RAJA_CONCEPT_VAL(U) != RAJA_CONCEPT_VAL(T),
                    RAJA_CONCEPT_VAL(T) != RAJA_CONCEPT_VAL(U));

} // end namespace detail

using detail::Iterator;
using detail::ForwardIterator;
using detail::BidirectionalIterator;
using detail::RandomAccessIterator;
using detail::Comparable;

} // end namespace concepts

} // end namespace RAJA
