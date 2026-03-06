#include <iostream>
#include <memory>
#include "cadmium/core/modeling/coupled.hpp"
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "background_noise.hpp"
#include "neuron.hpp"
#include "adder.hpp"

using namespace cadmium;

struct AdderHarness : public Coupled {
    Port<double> sum_out;

    explicit AdderHarness(const std::string& id) : Coupled(id) {
        sum_out = addOutPort<double>("sum_out");

        auto noise = addComponent<BackgroundNoise>("noise");
        auto n1 = addComponent<Neuron>("neuron1", -70.0, 30.0, 2.0, 0.1);
        auto n2 = addComponent<Neuron>("neuron2", -70.0, 30.0, 2.0, 0.1);
        auto adder = addComponent<Adder>("adder");

        addCoupling(noise->noise_out, adder->noise_in);
        addCoupling(n1->potential_out, adder->neuron1_in);
        addCoupling(n2->potential_out, adder->neuron2_in);

        addCoupling(adder->sum_out, sum_out);
    }
};

int main() {
    try {
        auto model = std::make_shared<AdderHarness>("adder_harness");
        RootCoordinator rc(model);
        //rc.setLogger<CSVLogger>("adder_simple_log.csv", ";");
        rc.setLogger(std::make_shared<cadmium::CSVLogger>("adder_simple_log.csv", ";"));
        rc.start();
        rc.simulate(5.0);
        rc.stop();
        std::cout << "Adder A1 done. See adder_simple_log.csv\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception in test_adder: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
