//
// Created by student149 on 12.12.2023.
//
#include "factory.hpp"
#include "types.hpp"
#include "nodes.hpp"
#include <stdexcept>
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

enum class NodeColor {
    UNVISITED, VISITED, VERIFIED
};

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    std::map<IPackageReceiver*, double> preferences = sender->receiver_preferences_.get_preferences();

    if (preferences.empty()) {
        throw std::logic_error("");
    }

    bool senderHasOtherReceiver = false; // does sender have a receiver other than himself?

    for (auto& pair: preferences) {
        ReceiverType pairType = pair.first->get_receiver_type();
        PackageSender* sendrecv_ptr = nullptr;

        if (pairType == ReceiverType::STOREHOUSE) {
            senderHasOtherReceiver = true;
        } else {
            IPackageReceiver* receiver_ptr = pair.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
        }

        if (sender == sendrecv_ptr) { continue; }
        senderHasOtherReceiver = true;

        if (node_colors[sendrecv_ptr] == NodeColor::VISITED) {
            has_reachable_storehouse(sendrecv_ptr, node_colors);
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;

    if (senderHasOtherReceiver) {
        return true;
    } else {
        throw std::logic_error("");
    }
}

bool Factory::is_consistent() {

    std::map<const PackageSender*, NodeColor> node_colors;

    for (auto& worker: workers) {
        node_colors[dynamic_cast<PackageSender*>(&worker)] = NodeColor::UNVISITED;
    }
    for (auto& ramp: ramps) {
        node_colors[dynamic_cast<PackageSender*>(&ramp)] = NodeColor::UNVISITED;
    }
    try {
        for (auto& ramp: ramps) {
            has_reachable_storehouse(dynamic_cast<PackageSender*>(&ramp), node_colors);
        }
    } catch (const std::logic_error&) {
        return false;
    }
    return true;
};

void Factory::remove_storehouse(ElementID id){
    storehouses.remove_by_id(id);

    for(auto &w : workers){
        auto worker_pref = w.receiver_preferences_;
        for(auto &worker_id : worker_pref){            
            if(worker_id.first -> get_id() == id){
                worker_pref.remove_receiver(worker_id.first);
            }
        }
    }
}

void Factory::remove_ramp(ElementID id){
    ramps.remove_by_id(id);
    for(auto &r : ramps){
        auto ramp_pref = r.receiver_preferences_;
        for(auto &ramp_id : ramp_pref){
            if(ramp_id.first -> get_id() == id){
                ramp_pref.remove_receiver(ramp_id.first);
            }
        }
    }
}

void Factory::remove_worker(ElementID id){
    workers.remove_by_id(id);

    for(auto &r : ramps){
        auto ramp_pref = r.receiver_preferences_;
        for(auto &ramp_id : ramp_pref){
            if(ramp_id.first -> get_id() == id){
                ramp_pref.remove_receiver(ramp_id.first);
            }
        }
    }
    for(auto &w : workers){
        auto worker_pref = w.receiver_preferences_;
        for(auto &worker_id : worker_pref){            
            if(worker_id.first -> get_id() == id){
                worker_pref.remove_receiver(worker_id.first);
            }
        }
    }
}