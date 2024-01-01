#include <stdexcept>
#include "simulation.hpp"

bool IntervalReportNotifier::should_generate_report(Time t) {
    return (t - 1) % to_ == 0;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    return turns_.find(t) != turns_.end();
}

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    if (!f.is_consistent()) {
        throw std::logic_error("");
    }

    for (Time simulationTime = 1; simulationTime <= d; simulationTime++) {

        f.do_deliveries(simulationTime);
        f.do_package_passing();
        f.do_work(simulationTime);

        rf(f, simulationTime);
    }
}