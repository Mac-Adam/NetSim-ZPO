//
// Created by student149 on 12.12.2023.
//

#include "factory.hpp"

void Factory::do_deliveries(Time t) {
    for (auto& ramp: ramps) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_work(Time t) {
    for (auto& worker: workers) {
        worker.do_work(t);
    }
}

void Factory::do_package_passing() {
    for (auto& ramp: ramps) {
        ramp.send_package();
    }

    for (auto& worker: workers) {
        worker.send_package();
    }
}

