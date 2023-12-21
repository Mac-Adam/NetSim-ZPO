#ifndef NETSIM_ZPO_FACTORY_HPP
#define NETSIM_ZPO_FACTORY_HPP

#include "nodes.hpp"
#include "types.hpp"
#include <vector>
#include <algorithm>


template<class NodeType>
class NodeCollection {

public:
    using iterator = typename std::vector<NodeType>::iterator;
    using const_iterator = typename std::vector<NodeType>::const_iterator;

    void add(NodeType&& node) { nodes_.push_back(std::move(node)); };

    void remove_by_id(ElementID id) {
        nodes_.remove(std::find(begin(), end(), [id](NodeType x) { return x.get_id() == id; }));
    };

    iterator find_by_id(ElementID id) {
        return std::find_if(begin(), end(), [id](NodeType& x) { return x.get_id() == id; });
    };

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(cbegin(), cend(), [id](NodeType& x) { return x.get_id() == id; });
    };

    iterator begin() { return nodes_.begin(); }

    const_iterator cbegin() const { return nodes_.cbegin(); }

    iterator end() { return nodes_.end(); }

    const_iterator cend() const { return nodes_.cend(); }

private:
    std::vector<NodeType> nodes_;


};

class Factory : public Worker, public Storehouse, public Ramp {
public:
    Factory();

    void add_ramp(Ramp&& r) { ramps.add(std::move(r)); };

    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps.find_by_id(id); };

    NodeCollection<Ramp>::const_iterator ramp_cbegin() { return ramps.cbegin(); }

    NodeCollection<Ramp>::const_iterator ramp_cend() { return ramps.cend(); }

    void add_worker(Worker&& w) { workers.add(std::move(w)); };

    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers.find_by_id(id); };

    NodeCollection<Worker>::const_iterator worker_cbegin() { return workers.cbegin(); }

    NodeCollection<Worker>::const_iterator worker_cend() { return workers.cend(); }

    void add_storehouse(Storehouse&& s) { storehouses.add(std::move(s)); };

    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouses.find_by_id(id); };

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() { return storehouses.cbegin(); }

    NodeCollection<Storehouse>::const_iterator storehouse_cend() { return storehouses.cend(); }

    bool is_consistent();

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

private:
    NodeCollection<Worker> workers;
    NodeCollection<Ramp> ramps;
    NodeCollection<Storehouse> storehouses;
};

#endif //NETSIM_ZPO_FACTORY_HPP

