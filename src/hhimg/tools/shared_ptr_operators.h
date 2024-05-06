#ifndef SHARED_PTR_OPERATORS_HPP
#define SHARED_PTR_OPERATORS_HPP
#include "image_operators.h"
#include "pixel_operators.h"
#include "concepts"
#include <memory>

#define SharedPtrAffectationOp(Op)                                             \
    template <hhimg::concepts::ValidType InnerType1,                           \
              hhimg::concepts::ValidType InnerType2>                           \
        requires(std::is_same_v<typename InnerType1::type,                     \
                                typename InnerType2::type>)                    \
    std::shared_ptr<InnerType1> const operator Op##=(                          \
        std::shared_ptr<InnerType1> lhs,                                       \
        std::shared_ptr<InnerType2> const rhs) {                               \
        InnerType1 &lhsValue = *lhs;                                           \
        InnerType2 const &rhsValue = *rhs;                                     \
        lhsValue Op## = rhsValue;                                              \
        return lhs;                                                            \
    }
SharedPtrAffectationOp(+) SharedPtrAffectationOp(-) SharedPtrAffectationOp(*)
    SharedPtrAffectationOp(/)
#undef SharedPtrAffectationOp

#define SharedPtrOp(Op)                                                        \
    template <hhimg::concepts::ValidType InnerType1,                           \
              hhimg::concepts::ValidType InnerType2>                           \
        requires(std::is_same_v<typename InnerType1::type,                     \
                                typename InnerType2::type>)                    \
    std::shared_ptr<hhimg::AbstractPixel<typename InnerType1::type>> const     \
    operator Op(std::shared_ptr<InnerType1> const lhs,                         \
                std::shared_ptr<InnerType2> const rhs) {                       \
        InnerType1 const &lhsValue = *lhs;                                     \
        InnerType2 const &rhsValue = *rhs;                                     \
        return std::make_shared<hhimg::PixelValue<typename InnerType1::type>>( \
            lhsValue Op rhsValue);                                             \
    }
        SharedPtrOp(+) SharedPtrOp(-) SharedPtrOp(*) SharedPtrOp(/)
#undef SharedPtrOp

#endif
