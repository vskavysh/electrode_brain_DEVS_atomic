#ifndef ADDER_HPP
#define ADDER_HPP

#include <iostream>
#include <limits>
#include <cmath>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct AdderState {
    double noise;   // mV
    double n1;      // mV
    double n2;      // mV
    double sum;     // mV
    double sigma;   // s

    explicit AdderState()
        : noise(0.0), n1(0.0), n2(0.0), sum(0.0),
          sigma(std::numeric_limits<double>::infinity()) {}
};

inline std::ostream& operator<<(std::ostream& out, const AdderState& state) {
    out << "{noise: " << state.noise
        << ", n1: " << state.n1
        << ", n2: " << state.n2
        << ", sum: " << state.sum
        << ", sigma: " << state.sigma << "}";
    return out;
}

class Adder : public Atomic<AdderState> {
public:
    Port<double> noise_in;
    Port<double> neuron1_in;
    Port<double> neuron2_in;
    Port<double> sum_out;

    explicit Adder(const std::string& id)
        : Atomic<AdderState>(id, AdderState()) {
        noise_in = addInPort<double>("noise_in");
        neuron1_in = addInPort<double>("neuron1_in");
        neuron2_in = addInPort<double>("neuron2_in");
        sum_out = addOutPort<double>("sum_out");
    }

    void externalTransition(AdderState& state, double e) const override {
        if (std::isfinite(state.sigma)) {
            state.sigma -= e;
            if (state.sigma < 0.0) state.sigma = 0.0;
        }

        // Noise is sampled every time it arrives
        if (!noise_in->empty()) {
            state.noise = noise_in->getBag().back();
        }

        state.n1 = 0.0;
        state.n2 = 0.0;

        if (!neuron1_in->empty()) {
            state.n1 = neuron1_in->getBag().back();
        }
        if (!neuron2_in->empty()) {
            state.n2 = neuron2_in->getBag().back();
        }

        state.sum = state.noise + 0.9 * state.n1 + 0.75 * state.n2;
        state.sigma = 0.0;
    }
    
    void internalTransition(AdderState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
    }

    void confluentTransition(AdderState& state, double e) const override {
        externalTransition(state, e);
    }

    void output(const AdderState& state) const override {
        sum_out->addMessage(state.sum);
    }

    [[nodiscard]] double timeAdvance(const AdderState& state) const override {
        return state.sigma;
    }
};

#endif // ADDER_HPP
