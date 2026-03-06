#include <iostream>
#include <memory>
#include "cadmium/core/modeling/coupled.hpp"
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "neuron.hpp"

using namespace cadmium;

struct NeuronHarness : public Coupled {
    Port<double> potential_out;

    NeuronHarness(const std::string& id,
                  double rest_potential,
                  double spike_potential,
                  double refractory_duration,
                  double firing_probability)
        : Coupled(id) {
        potential_out = addOutPort<double>("potential_out");

        auto neuron = addComponent<Neuron>("neuron",
                                           rest_potential,
                                           spike_potential,
                                           refractory_duration,
                                           firing_probability);

        addCoupling(neuron->potential_out, potential_out);
    }
};

int main() {
    try {
        {
            auto model = std::make_shared<NeuronHarness>("neuron_always_harness",
                                                         -70.0, 30.0, 2.0, 1.0);
            RootCoordinator rc(model);
            rc.setLogger(std::make_shared<cadmium::CSVLogger>("neuron_always_fire_log.csv", ";"));
            rc.start();
            rc.simulate(5.0);
            rc.stop();
            std::cout << "Neuron Ne1 done. See neuron_always_fire_log.csv\n";
        }

        {
            auto model = std::make_shared<NeuronHarness>("neuron_never_harness",
                                                         -70.0, 30.0, 2.0, 0.0);
            RootCoordinator rc(model);
            rc.setLogger(std::make_shared<cadmium::CSVLogger>("neuron_never_fire_log.csv", ";"));
            rc.start();
            rc.simulate(5.0);
            rc.stop();
            std::cout << "Neuron Ne2 done. See neuron_never_fire_log.csv\n";
        }

        {
            auto model = std::make_shared<NeuronHarness>("neuron_default_harness",
                                                         -70.0, 30.0, 2.0, 0.1);
            RootCoordinator rc(model);
            rc.setLogger(std::make_shared<cadmium::CSVLogger>("neuron_default_log.csv", ";"));
            rc.start();
            rc.simulate(20.0);
            rc.stop();
            std::cout << "Neuron Ne3 done. See neuron_default_log.csv\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception in test_neuron: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
