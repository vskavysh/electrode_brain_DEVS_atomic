#include <iostream>
#include <memory>
#include "cadmium/core/modeling/coupled.hpp"
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "background_noise.hpp"

using namespace cadmium;

struct BackgroundNoiseHarness : public Coupled {
    Port<double> noise_out;

    explicit BackgroundNoiseHarness(const std::string& id) : Coupled(id) {
        noise_out = addOutPort<double>("noise_out");

        auto noise = addComponent<BackgroundNoise>("noise");
        addCoupling(noise->noise_out, noise_out);
    }
};

int main() {
    try {
        auto model = std::make_shared<BackgroundNoiseHarness>("noise_harness");
        RootCoordinator rc(model);

        rc.setLogger(std::make_shared<cadmium::CSVLogger>("noise_sampling_log.csv", ";"));
        rc.start();
        rc.simulate(200.0);
        rc.stop();

        std::cout << "BackgroundNoise test done. See noise_sampling_log.csv\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception in test_background_noise: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
