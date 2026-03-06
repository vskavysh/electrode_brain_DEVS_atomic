#include <iostream>
#include <memory>
#include "cadmium/core/modeling/coupled.hpp"
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "neuron.hpp"
#include "electrode.hpp"

using namespace cadmium;

struct ElectrodeHarness : public Coupled {
    Port<int> spike_out;

    ElectrodeHarness(const std::string& id, double theta) : Coupled(id) {
        spike_out = addOutPort<int>("spike_out");

        auto neuron = addComponent<Neuron>("neuron", -70.0, 30.0, 2.0, 0.1);
        auto electrode = addComponent<Electrode>("electrode", theta);

        addCoupling(neuron->potential_out, electrode->input_in);
        addCoupling(electrode->spike_out, spike_out);
    }
};

int main() {
    try {
        {
            auto model = std::make_shared<ElectrodeHarness>("elec_E1", 29.0);
            RootCoordinator rc(model);
            //rc.setLogger<CSVLogger>("electrode_threshold_crossing_log.csv", ";");
            rc.setLogger(std::make_shared<cadmium::CSVLogger>("electrode_threshold_crossing_log.csv", ";"));
            rc.start();
            rc.simulate(20.0);
            rc.stop();
            std::cout << "Electrode E1 done. See electrode_threshold_crossing_log.csv\n";
        }

        {
            auto model = std::make_shared<ElectrodeHarness>("elec_E2", 31.0);
            RootCoordinator rc(model);
            //rc.setLogger<CSVLogger>("electrode_high_threshold_log.csv", ";");
            rc.setLogger(std::make_shared<cadmium::CSVLogger>("electrode_high_threshold_log.csv", ";"));
            rc.start();
            rc.simulate(20.0);
            rc.stop();
            std::cout << "Electrode E2 done. See electrode_high_threshold_log.csv\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in test_electrode: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
