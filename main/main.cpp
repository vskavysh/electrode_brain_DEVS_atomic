#include <iostream>
#include <memory>
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/csv.hpp"
#include "top.hpp"

using namespace cadmium;

int main() {
    try {
        auto top = std::make_shared<Top>("top");
        RootCoordinator rc(top);
	rc.setLogger<cadmium::CSVLogger>("electrode_brain_log.csv", ";");
        rc.start();
        rc.simulate(20.0);
        rc.stop();
        std::cout << "Simulation finished. Log written to electrode_brain_log.csv\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
