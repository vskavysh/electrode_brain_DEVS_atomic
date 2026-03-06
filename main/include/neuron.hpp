#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <random>
#include <algorithm>
#include "cadmium/core/modeling/atomic.hpp"

using namespace cadmium;

struct NeuronState {
    bool emit_spike;              // whether the next output event is a spike
    double potential;             // mV (30 when firing, -70 otherwise, mainly for logging)
    double sigma;                 // s
    double refractory_remaining;  // s remaining until firing becomes possible again

    explicit NeuronState()
        : emit_spike(false), potential(-70.0), sigma(0.1), refractory_remaining(0.0) {}
};

inline std::ostream& operator<<(std::ostream& out, const NeuronState& state) {
    out << "{emit_spike: " << state.emit_spike
        << ", V: " << state.potential
        << ", sigma: " << state.sigma
        << ", refractory: " << state.refractory_remaining << "}";
    return out;
}

class Neuron : public Atomic<NeuronState> {
public:
    Port<double> potential_out;

    Neuron(const std::string& id,
           double rest_potential = -70.0,
           double spike_potential = 30.0,
           double refractory_duration = 2.0,
           double firing_probability = 0.1)
        : Atomic<NeuronState>(id, NeuronState()),
          rest(rest_potential),
          spike(spike_potential),
          refractory(refractory_duration),
          p_fire(firing_probability),
          gen(std::random_device{}()),
          uni01(0.0, 1.0) {
        potential_out = addOutPort<double>("potential_out");

        // Initialize the first scheduled event at t = 0.1 s
        double r = uni01(gen);
        this->state.emit_spike = (r < p_fire);
        this->state.potential = this->state.emit_spike ? spike : rest;
        this->state.refractory_remaining = this->state.emit_spike ? refractory : 0.0;
    }

    void externalTransition(NeuronState& state, double e) const override {
        // No inputs
        state.sigma -= e;
        if (state.sigma < 0.0) state.sigma = 0.0;
    }

    void internalTransition(NeuronState& state) const override {
        const double step = state.sigma;

        // If the currently scheduled event was a spike, or if the neuron was already
        // refractory, advance the refractory timer to the next scheduled event.
        if (state.refractory_remaining > 0.0) {
            state.refractory_remaining = std::max(0.0, state.refractory_remaining - step);
        }

        // Decide what happens at the next event time
        if (state.refractory_remaining > 0.0) {
            state.emit_spike = false;
            state.potential = rest;
        } else {
            double r = uni01(gen);
            if (r < p_fire) {
                state.emit_spike = true;
                state.potential = spike;
                state.refractory_remaining = refractory;
            } else {
                state.emit_spike = false;
                state.potential = rest;
            }
        }

        state.sigma = 0.1;
    }

    void confluentTransition(NeuronState& state, double e) const override {
        internalTransition(state);
        externalTransition(state, 0.0);
    }

    void output(const NeuronState& state) const override {
        // Event-only neuron: emit only when firing
        if (state.emit_spike) {
            potential_out->addMessage(spike);
        }
    }

    [[nodiscard]] double timeAdvance(const NeuronState& state) const override {
        return state.sigma;
    }

private:
    double rest;
    double spike;
    double refractory;
    double p_fire;
    mutable std::mt19937 gen;
    mutable std::uniform_real_distribution<double> uni01;
};

#endif // NEURON_HPP
