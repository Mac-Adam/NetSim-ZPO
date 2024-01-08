#include "reports.hpp"
#include "types.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>


void generate_structure_report(const Factory& factory, std::ostream& os) {
    os << std::endl << "== LOADING RAMPS ==" << std::endl << std::endl;
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp) {
        std::size_t worker_amount = 0;
        std::size_t storehouses_amount = 0;

        std::vector<ElementID> workers_id_vec;
        std::vector<ElementID> storehouse_id_vec;

        os << "LOADING RAMP #" << ramp.get_id() << std::endl;
        os << "  " << "Delivery interval: " << ramp.get_delivery_interval() << std::endl;
        os << "  " << "Receivers:" << std::endl;

        for (auto& pref: ramp.receiver_preferences_.get_preferences()) {
            if (pref.first->get_receiver_type() == ReceiverType::WORKER) {
                worker_amount++;
                workers_id_vec.push_back(pref.first->get_id());
            }
            if (pref.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                storehouses_amount++;
                storehouse_id_vec.push_back(pref.first->get_id());
            }
        }
        std::sort(workers_id_vec.begin(), workers_id_vec.end());
        std::sort(storehouse_id_vec.begin(), storehouse_id_vec.end());
        if (storehouse_id_vec.size() > 0) {
            for (std::size_t i = 0; i < storehouses_amount; i++) {
                os << "    " << "storehouse #" << storehouse_id_vec[i] << std::endl;
            }
        }
        if (workers_id_vec.size() > 0) {
            for (std::size_t i = 0; i < worker_amount; i++) {
                os << "    " << "worker #" << workers_id_vec[i] << std::endl;
            }
        }

        os << std::endl;
    });
    os << std::endl << "== WORKERS ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker) {
        std::size_t worker_amount = 0;
        std::size_t storehouses_amount = 0;

        std::vector<ElementID> workers_id_vec;
        std::vector<ElementID> storehouse_id_vec;

        os << "WORKER #" << worker.get_id() << std::endl;
        os << "  " << "Processing time: " << worker.get_processing_duration() << std::endl;
        os << "  " << "Queue type: "
           << ((worker.get_queue()->get_queue_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO") << std::endl;
        os << "  " << "Receivers:" << std::endl;

        for (auto& pref: worker.receiver_preferences_.get_preferences()) {
            if (pref.first->get_receiver_type() == ReceiverType::WORKER) {
                worker_amount++;
                workers_id_vec.push_back(pref.first->get_id());
            }
            if (pref.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                storehouses_amount++;
                storehouse_id_vec.push_back(pref.first->get_id());
            }
        }
        std::sort(workers_id_vec.begin(), workers_id_vec.end());
        std::sort(storehouse_id_vec.begin(), storehouse_id_vec.end());
        if (storehouse_id_vec.size() > 0) {
            for (std::size_t i = 0; i < storehouses_amount; i++) {
                os << "    " << "storehouse #" << storehouse_id_vec[i] << std::endl;
            }
        }
        if (workers_id_vec.size() > 0) {
            for (std::size_t i = 0; i < worker_amount; i++) {
                os << "    " << "worker #" << workers_id_vec[i] << std::endl;
            }
        }

        os << std::endl;
    });
    os << std::endl << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse) {
        os << "STOREHOUSE #" << storehouse.get_id() << std::endl << std::endl;
    });
}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time time) {
    os << "=== [ Turn: " << time << " ] ===" << std::endl << std::endl;
    os << "== WORKERS ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os, time](const Worker& worker) {
        os << "WORKER #" << worker.get_id() << std::endl;
        os << "  " << "PBuffer: ";
        const std::optional<Package>& pBuff = worker.get_processing_buffer();
        if (pBuff.has_value()) {
            os << "#" << pBuff.value().get_id() << " (pt = " << worker.get_processing_time(time) << ")";
        } else {
            os << "(empty)";
        }
        os << std::endl;

        os << "  " << "Queue: ";
        bool empty = true;
        for (auto const& elem: *worker.get_queue()) {
            if (!empty) {
                os << " ";
            }
            os << "#" << elem.get_id();
            empty = false;
        }
        if (empty) {
            os << "(empty)";
        }
        os << std::endl;

        os << "  " << "SBuffer: ";
        const std::optional<Package>& sBuff = worker.get_sending_buffer();
        if (sBuff.has_value()) {
            os << "#" << sBuff.value().get_id();
        } else {
            os << "(empty)";
        }
        os << std::endl << std::endl;
    });
    os << std::endl << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse) {
        os << "STOREHOUSE #" << storehouse.get_id() << std::endl;
        os << "  " << "Stock: ";
        bool empty = true;
        std::for_each(storehouse.cbegin(), storehouse.cend(), [&os, &empty](auto const& elem) {
            if (!empty) {
                os << " ";
            }
            os << "#" << elem.get_id();
            empty = false;
        });
        if (empty) {
            os << "(empty)";
        }
        os << std::endl << std::endl;
    });


}