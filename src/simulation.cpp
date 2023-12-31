#include <stdexcept>
#include "simulation.hpp"

bool IntervalReportNotifier::should_generate_report(Time t) {
    float n = 1 + ((float)t - 1)/(float)to_;
    if (std::floor(n) == n) {
        return true;
    }
    return false;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    if (turns_.find(t) != turns_.end()) {
        return true;
    }
    return false;
}

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf) {
    if (!f.is_consistent()) {
        throw std::logic_error("");
    }

    for (Time simulationTime = 1; simulationTime <= d; simulationTime++) {
        f.do_deliveries(simulationTime);
        f.do_work(simulationTime);
        f.do_package_passing();
        rf(f, simulationTime);
    }
}