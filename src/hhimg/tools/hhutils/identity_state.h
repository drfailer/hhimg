#ifndef IDENTITY_STATE_H
#define IDENTITY_STATE_H
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename TriggerType, typename Output>
struct IdentityState : hh::AbstractState<1, TriggerType, Output> {
    void execute(std::shared_ptr<TriggerType> in) override {
        this->addResult(in);
    }
};

} // namespace hhimg

#endif
