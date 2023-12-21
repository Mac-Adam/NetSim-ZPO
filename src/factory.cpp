//
// Created by student149 on 12.12.2023.
//

#include "factory.hpp"
#include "types.hpp"
#include "nodes.hpp"
#include <stdexcept>

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
/*
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

    try {
        for (auto& ramp: ramps) {
            has_reachable_storehouse(dynamic_cast<PackageSender*>(&ramp), node_colors);
  //from here
    for(auto &w : workers){
        auto worker_pref = w.receiver_preferences_;
        for(auto &worker_id : worker_pref){            
            if(worker_id.first -> get_id() == id){
                worker_pref.remove_receiver(worker_id.first);
            }
          //to here
        }
    }

    return true;
};
*/
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
