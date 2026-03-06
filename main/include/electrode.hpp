#ifndef ELECTRODE_HPP
#define ELECTRODE_HPP

#include <iostream>
#include <limits>
#include <cmath>
#include "cadmium/core/modeling/atomic.hpp"

using namespace cadmium;

struct ElectrodeState {
    double input;       // mV
    double threshold;   // mV
    double sigma;       // s

    explicit ElectrodeState()
        : input(-100.0), threshold(0.0),
          sigma(std::numeric_limits<double>::infinity()) {}
};

inline std::ostream& operator<<(std::ostream& out, const ElectrodeState& state) {
    out << "{input: " << state.input
        << ", threshold: " << state.threshold
        << ", sigma: " << state.sigma << "}";
    return out;
}

class Electrode : public Atomic<ElectrodeState> {
public:
    Port<double> input_in;
    Port<int> spike_out;   // emits 1 on spike events

    Electrode(const std::string& id, double threshold = 0.0)
        : Atomic<ElectrodeState>(id, ElectrodeState()) {
        spike_out = addOutPort<int>("spike_out");
        input_in = addInPort<double>("input_in");
        this->state.threshold = threshold;
    }

    void externalTransition(ElectrodeState& state, double e) const override {
        if (std::isfinite(state.sigma)) {
            state.sigma -= e;
            if (state.sigma < 0.0) state.sigma = 0.0;
        }

        if (!input_in->empty()) {
            state.input = input_in->getBag().back();
        }

        if (state.input >= state.threshold) {
            state.sigma = 0.0;
        } else {
            state.sigma = std::numeric_limits<double>::infinity();
        }
    }

    void internalTransition(ElectrodeState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
    }

    void confluentTransition(ElectrodeState& state, double e) const override {
        externalTransition(state, e);
    }

    void output(const ElectrodeState& state) const override {
        spike_out->addMessage(1);
    }

    [[nodiscard]] double timeAdvance(const ElectrodeState& state) const override {
        return state.sigma;
    }
};

#endif // ELECTRODE_HPP
