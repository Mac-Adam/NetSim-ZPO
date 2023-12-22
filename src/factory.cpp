//
// Created by student149 on 12.12.2023.
//
#include "factory.hpp"
#include "types.hpp"
#include "nodes.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <vector>
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

template<class NodeType>
void NodeCollection<NodeType>::remove_by_id(ElementID id) {
    if (find_by_id(id) != end()) {
        nodes_.erase(find_by_id(id));
    }
};

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
            if (sender == sendrecv_ptr) { continue; }
            senderHasOtherReceiver = true;
        }


        if (node_colors.find(sendrecv_ptr) != node_colors.end() and node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
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

void Factory::remove_storehouse(ElementID id) {

    for (auto& w: workers) {
        auto& worker_pref = w.receiver_preferences_;
        for (auto& worker_id: worker_pref) {
            if (worker_id.first->get_id() == id) {
                worker_pref.remove_receiver(worker_id.first);
            }
        }
    }
    storehouses.remove_by_id(id);

}

void Factory::remove_ramp(ElementID id) {

    for (auto& r: ramps) {
        auto& ramp_pref = r.receiver_preferences_;
        for (auto& ramp_id: ramp_pref) {
            if (ramp_id.first->get_id() == id) {
                ramp_pref.remove_receiver(ramp_id.first);
            }
        }
    }
    ramps.remove_by_id(id);
}

void Factory::remove_worker(ElementID id) {

    for (auto& r: ramps) {
        auto& ramp_pref = r.receiver_preferences_;
        for (auto& ramp_id: ramp_pref) {
            if (ramp_id.first->get_id() == id) {
                ramp_pref.remove_receiver(ramp_id.first);
                break;
            }
        }
    }
    for (auto& w: workers) {
        auto& worker_pref = w.receiver_preferences_;
        for (auto& worker_id: worker_pref) {
            if (worker_id.first->get_id() == id) {
                worker_pref.remove_receiver(worker_id.first);
            }
        }
    }
    workers.remove_by_id(id);
}

enum class ELEMENT_TYPE {
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLine {
    ELEMENT_TYPE elem_type;
    std::map<std::string, std::string> params;

};

ParsedLine parse_line(const std::string& line) {
    ParsedLine parsedLine;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        if (token == "LOADING_RAMP") {
            parsedLine.elem_type = ELEMENT_TYPE::RAMP;
        } else if (token == "WORKER") {
            parsedLine.elem_type = ELEMENT_TYPE::WORKER;
        } else if (token == "STOREHOUSE") {
            parsedLine.elem_type = ELEMENT_TYPE::STOREHOUSE;
        } else if (token == "LINK") {
            parsedLine.elem_type = ELEMENT_TYPE::LINK;
        } else {
            std::string key;
            std::string val;
            std::istringstream key_val_stream(token);
            std::getline(key_val_stream, key, '=');
            std::getline(key_val_stream, val, '=');
            parsedLine.params[key] = val;

        }

    }

    return parsedLine;
}

Factory load_factory_structure(std::istream& is) {
    Factory factory = Factory();
    std::string line;

    while (std::getline(is, line)) {
        if (line[0] == ';' || line == "\n" || line.empty()) {
            continue;
        }
        ParsedLine parsedLine = parse_line(line);
        ElementID ramp_id;
        TimeOffset di;
        ElementID worker_id;
        TimeOffset pt;
        ElementID storehouse_id;
        PackageQueueType queueType;
        switch (parsedLine.elem_type) {
            case ELEMENT_TYPE::RAMP:
                for (auto& key_val: parsedLine.params) {
                    if (key_val.first == "id") {
                        ramp_id = std::stoi(key_val.second);
                    } else if (key_val.first == "delivery-interval") {
                        di = std::stoi(key_val.second);
                    }
                }
                factory.add_ramp(Ramp(ramp_id, di));
                break;
            case ELEMENT_TYPE::WORKER:
                for (auto& key_val: parsedLine.params) {
                    if (key_val.first == "id") {
                        worker_id = std::stoi(key_val.second);
                    } else if (key_val.first == "processing-time") {
                        pt = std::stoi(key_val.second);
                    } else if (key_val.first == "queue-type") {
                        queueType = key_val.second == "LIFO" ? PackageQueueType::LIFO : PackageQueueType::FIFO;
                    }
                }
                factory.add_worker(Worker(worker_id, pt, std::make_unique<PackageQueue>(queueType)));
                break;
            case ELEMENT_TYPE::STOREHOUSE:
                for (auto& key_val: parsedLine.params) {
                    if (key_val.first == "id") {
                        storehouse_id = std::stoi(key_val.second);
                    }
                }
                factory.add_storehouse(Storehouse(storehouse_id));
                break;
            case ELEMENT_TYPE::LINK:
                IPackageReceiver* receiver;
                ElementID src_id;
                ELEMENT_TYPE src_type;
                for (auto& key_val: parsedLine.params) {
                    std::string node_type;
                    std::string node_id;
                    std::istringstream key_val_stream(key_val.second);
                    std::getline(key_val_stream, node_type, '-');
                    std::getline(key_val_stream, node_id, '-');
                    if (key_val.first == "dest") {
                        if (node_type == "worker") {
                            //This doesn't look like a proper way to do that, fix it if you can...
                            receiver = &(*factory.find_worker_by_id(std::stoi(node_id)));
                        } else if (node_type == "store") {
                            receiver = &(*factory.find_storehouse_by_id(std::stoi(node_id)));
                        }
                    } else if (key_val.first == "src") {
                        src_id = std::stoi(node_id);
                        if (node_type == "worker") {
                            src_type = ELEMENT_TYPE::WORKER;
                        } else if (node_type == "ramp") {
                            src_type = ELEMENT_TYPE::RAMP;
                        }
                    }
                }
                if (src_type == ELEMENT_TYPE::WORKER) {
                    factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
                } else if (src_type == ELEMENT_TYPE::RAMP) {
                    factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
                }
                break;
        }
    }

    return factory;
}

void save_link(std::ostream& os, ElementID curr_id, const std::string& element_type, IPackageReceiver* other) {
    ElementID otherId = other->get_id();
    std::string other_type;
    if (other->get_receiver_type() == ReceiverType::WORKER) {
        other_type = "worker";
    } else if (other->get_receiver_type() == ReceiverType::STOREHOUSE) {
        other_type = "store";
    }
    os << "LINK src=" << element_type << '-' << curr_id << " dest=" << other_type << "-" << otherId << std::endl;

}


void save_factory_structure(Factory& factory, std::ostream& os) {
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp) {
        os << "LOADING_RAMP id=" << ramp.get_id() << " delivery-interval=" << ramp.get_delivery_interval() << std::endl;
    });
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker) {
        os << "WORKER id=" << worker.get_id() << " processing-time=" << worker.get_processing_duration();
        os << " queue-type=" << ((worker.get_queue()->get_queue_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO")
           << std::endl;
    });
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse) {
        os << "STOREHOUSE id=" << storehouse.get_id() << std::endl;
    });

    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp) {
        for (const auto& pref: ramp.receiver_preferences_.get_preferences()) {
            save_link(os, ramp.get_id(), "ramp", pref.first);
        }
    });

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker) {
        for (const auto& pref: worker.receiver_preferences_.get_preferences()) {
            save_link(os, worker.get_id(), "worker", pref.first);
        }
    });

}