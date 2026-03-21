#include <iostream>
#include <memory>

#include "cadmium/modeling/devs/coupled.hpp"
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/csv.hpp"

#include "background_noise.hpp"
#include "neuron.hpp"
#include "adder.hpp"
#include "electrode.hpp"

using namespace cadmium;

/**
 * Parameterized brain coupled model:
 *   noise + two neurons + adder
 */
struct BrainTest : public Coupled {
    Port<double> brain_out;

    BrainTest(const std::string& id, double neuron_p_fire) : Coupled(id) {
        brain_out = addOutPort<double>("brain_out");

        auto noise   = addComponent<BackgroundNoise>("noise");
        auto neuron1 = addComponent<Neuron>("neuron1", -70.0, 30.0, 2.0, neuron_p_fire);
        auto neuron2 = addComponent<Neuron>("neuron2", -70.0, 30.0, 2.0, neuron_p_fire);
        auto adder   = addComponent<Adder>("adder");

        addCoupling(noise->noise_out, adder->noise_in);
        addCoupling(neuron1->potential_out, adder->neuron1_in);
        addCoupling(neuron2->potential_out, adder->neuron2_in);

        addCoupling(adder->sum_out, brain_out);
    }
};

/**
 * Parameterized top model:
 *   BrainTest -> Electrode
 */
struct TopTest : public Coupled {
    Port<int> electrode_spike;

    TopTest(const std::string& id, double neuron_p_fire, double threshold)
        : Coupled(id) {

        electrode_spike = addOutPort<int>("electrode_spike");

        auto brain     = addComponent<BrainTest>("brain", neuron_p_fire);
        auto electrode = addComponent<Electrode>("electrode", threshold);

        addCoupling(brain->brain_out, electrode->input_in);
        addCoupling(electrode->spike_out, electrode_spike);
    }
};

int main() {
    try {
        // ============================
        // Experiment A:
        // neurons always fire, threshold = 45 mV
        // ============================
        {
            double p_fire = 1.0;
            double threshold = 45.0;

            auto model = std::make_shared<TopTest>("top_thresh_45", p_fire, threshold);
            RootCoordinator rc(model);

	    rc.setLogger<cadmium::CSVLogger>("top_threshold_45_log.csv", ";");
            rc.start();
            rc.simulate(5.0);
            rc.stop();

            std::cout << "Top experiment A done: p_fire = "
                      << p_fire << ", threshold = " << threshold
                      << ". See top_threshold_45_log.csv\n";
        }

        // ============================
        // Experiment B:
        // neurons always fire, threshold = 55 mV
        // ============================
        {
            double p_fire = 1.0;
            double threshold = 55.0;

            auto model = std::make_shared<TopTest>("top_thresh_55", p_fire, threshold);
            RootCoordinator rc(model);

            rc.setLogger<cadmium::CSVLogger>("top_threshold_55_log.csv", ";");
	    rc.start();
            rc.simulate(5.0);
            rc.stop();

            std::cout << "Top experiment B done: p_fire = "
                      << p_fire << ", threshold = " << threshold
                      << ". See top_threshold_55_log.csv\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception in test_top: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
