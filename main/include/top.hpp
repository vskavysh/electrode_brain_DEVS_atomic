#ifndef TOP_HPP
#define TOP_HPP

#include <string>
#include "cadmium/core/modeling/coupled.hpp"
#include "brain.hpp"
#include "electrode.hpp"

using namespace cadmium;

struct Top : public Coupled {
    Port<int> electrode_spike;

    explicit Top(const std::string& id) : Coupled(id) {
        electrode_spike = addOutPort<int>("electrode_spike");

        auto brain = addComponent<Brain>("brain");
        auto electrode = addComponent<Electrode>("electrode", 25.0); // theta=25.0 mV

        addCoupling(brain->brain_out, electrode->input_in);
        addCoupling(electrode->spike_out, electrode_spike);
    }
};

#endif // TOP_HPP
