#ifndef TRIGGER_STATE_H
#define TRIGGER_STATE_H
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename TriggerType, typename Output>
struct TriggerState : hh::AbstractState<1, TriggerType, Output> {
    TriggerState(std::shared_ptr<Output> output)
        : hh::AbstractState<1, TriggerType, Output>(), output_(output) {}

    void execute(std::shared_ptr<TriggerType>) override {
        this->addResult(output_);
    }

  private:
    std::shared_ptr<Output> output_ = nullptr;
};

} // namespace hhimg

#endif
