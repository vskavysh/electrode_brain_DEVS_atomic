#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <string>
#include "cadmium/core/modeling/coupled.hpp"
#include "background_noise.hpp"
#include "neuron.hpp"
#include "adder.hpp"

using namespace cadmium;

struct Brain : public Coupled {
    Port<double> brain_out;

    explicit Brain(const std::string& id) : Coupled(id) {
        brain_out = addOutPort<double>("brain_out");

        auto noise = addComponent<BackgroundNoise>("noise");
        auto neuron1 = addComponent<Neuron>("neuron1"); // default p=0.1
        auto neuron2 = addComponent<Neuron>("neuron2"); // default p=0.1
        auto adder = addComponent<Adder>("adder");

        addCoupling(noise->noise_out, adder->noise_in);
        addCoupling(neuron1->potential_out, adder->neuron1_in);
        addCoupling(neuron2->potential_out, adder->neuron2_in);

        addCoupling(adder->sum_out, brain_out);
    }
};

#endif // BRAIN_HPP
