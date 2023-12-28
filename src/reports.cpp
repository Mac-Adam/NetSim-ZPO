#include "reports.hpp"
#include "types.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>


void generate_structure_report(const Factory &factory, std::ostream &os){
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp) {
        int worker_amount = 0;
        int storehouses_amount = 0;

        std::vector<int> workers_id_vec;
        std::vector<int> storehouse_id_vec;

        os << "LOADING RAMP #" << ramp.get_id() << std::endl;
        os << "  " << "Delivery interval: " << ramp.get_delivery_interval() << std::endl;
        os << "  " << "Receivers: " << std::endl;

        for(auto &pref : ramp.receiver_preferences_.get_preferences()){
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

        if(workers_id_vec.size() > 0){
            for(int i = 0; i < worker_amount; i++){
                os  << "    " << "worker #" << workers_id_vec[i] << std::endl;
            }
        }
        if(storehouse_id_vec.size() > 0){
            for(int i = 0; i < storehouses_amount; i++){
                os  << "    " << "storehouse #" << storehouse_id_vec[i] << std::endl;
            }
        }
    });

    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Worker& worker) {
        int worker_amount = 0;
        int storehouses_amount = 0;

        std::vector<int> workers_id_vec;
        std::vector<int> storehouse_id_vec;

        os << "WORKER #" << worker.get_id() << std::endl;
        os << "  " << "Processing time: " << worker.get_processing_duration() << std::endl;
        os << "  " << "Queue type: " << ((worker.get_queue()->get_queue_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO") << std::endl;
        os << "  " << "Receivers: " << std::endl;

        for(auto &pref : worker.receiver_preferences_.get_preferences()){
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

        if(workers_id_vec.size() > 0){
            for(int i = 0; i < worker_amount; i++){
                os  << "    " << "worker #" << workers_id_vec[i] << std::endl;
            }
        }
        if(storehouse_id_vec.size() > 0){
            for(int i = 0; i < storehouses_amount; i++){
                os  << "    " << "storehouse #" << storehouse_id_vec[i] << std::endl;
            }
        }
    });
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse) {
        os << "STOREHOUSE #" << storehouse.get_id() << std::endl;
    });
}