#include <iostream>
#include <memory>
#include "cadmium/core/simulation/root_coordinator.hpp"
#include "cadmium/core/logger/csv.hpp"
#include "top.hpp"

using namespace cadmium;

int main() {
    try {
        auto top = std::make_shared<Top>("top");
        RootCoordinator rc(top);
        //rc.setLogger<CSVLogger>("electrode_brain_log.csv", ";");
        rc.setLogger(std::make_shared<cadmium::CSVLogger>("electrode_brain_log.csv", ";"));
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
