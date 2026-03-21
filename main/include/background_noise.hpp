#ifndef BACKGROUND_NOISE_HPP
#define BACKGROUND_NOISE_HPP

#include <iostream>
#include <random>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

struct BackgroundNoiseState {
    double value; // mV
    double sigma; // s
    explicit BackgroundNoiseState() : value(0.0), sigma(0.1) {}
};

inline std::ostream& operator<<(std::ostream& out, const BackgroundNoiseState& state) {
    out << "{value: " << state.value << ", sigma: " << state.sigma << "}";
    return out;
}

class BackgroundNoise : public Atomic<BackgroundNoiseState> {
public:
    Port<double> noise_out;

    explicit BackgroundNoise(const std::string& id)
        : Atomic<BackgroundNoiseState>(id, BackgroundNoiseState()),
          gen(std::random_device{}()),
          dist(0.0, 10.0) {
        noise_out = addOutPort<double>("noise_out");
    }

    void externalTransition(BackgroundNoiseState& state, double e) const override {
        state.sigma -= e;
        if (state.sigma < 0.0) state.sigma = 0.0;
    }

    void internalTransition(BackgroundNoiseState& state) const override {
        state.value = dist(gen);
        state.sigma = 0.1;
    }

    void confluentTransition(BackgroundNoiseState& state, double e) const override {
        internalTransition(state);
        externalTransition(state, 0.0);
    }

    void output(const BackgroundNoiseState& state) const override {
        noise_out->addMessage(state.value);
    }

    [[nodiscard]] double timeAdvance(const BackgroundNoiseState& state) const override {
        return state.sigma;
    }

private:
    mutable std::mt19937 gen;
    mutable std::normal_distribution<double> dist;
};

#endif // BACKGROUND_NOISE_HPP
