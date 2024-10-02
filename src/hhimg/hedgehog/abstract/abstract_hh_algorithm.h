#ifndef ABSTRACT_HH_ALGORITHM_H
#define ABSTRACT_HH_ALGORITHM_H
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename Input, typename Output>
struct AbstractHHAlgorithm : hh::AbstractTask<1, Input, Output> {
    AbstractHHAlgorithm(size_t nbThreads = 1,
                        std::string const &name = "Unnamed Algorithm")
        : hh::AbstractTask<1, Input, Output>(name, nbThreads) {}

    void execute(std::shared_ptr<Input> in) override { this->operator()(in); }

    // can be overriden if a ghost region is required
    virtual size_t ghostRegionSize() const { return 0; }

    virtual void operator()(std::shared_ptr<Input> in) = 0;

    using InputType = Input;
    using OutputType = Output;
    using TaskType = hh::AbstractTask<1, Input, Output>;
};

} // end namespace hhimg::hdg

#endif
