#ifndef NETSIM_ZPO_FACTORY_HPP
#define NETSIM_ZPO_FACTORY_HPP

#include "nodes.hpp"
#include "types.hpp"
#include <vector>
#include <algorithm>


template<class NodeType>
class NodeCollection {
    using iterator = typename std::vector<NodeType>::iterator;
    using const_iterator = typename std::vector<NodeType>::const_iterator;
public:
    void add(NodeType&& node) { nodes_.emplace_back(node); };

    void remove_by_id(ElementID id) {
        nodes_.remove(std::find(begin(), end(), [id](NodeType x) { return x.get_id() == id; }));
    };

    iterator find_by_id(ElementID id) { std::find(begin(), end(), [id](NodeType x) { return x.get_id() == id; }); };

    const_iterator find_by_id(ElementID id) const {
        std::find(cbegin(), cend(), [id](NodeType x) { return x.get_id() == id; });
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
    void add_ramp(Ramp&& r);

    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator ramp_cbegin();

    NodeCollection<Ramp>::const_iterator ramp_cend();

    void add_worker(Worker&& w);

    void remove_worker(ElementID id);

    NodeCollection<Ramp>::iterator find_worker_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator worker_cbegin();

    NodeCollection<Ramp>::const_iterator worker_cend();

    void add_storehouse(Storehouse&& s);

    void remove_storehouse(ElementID id);

    NodeCollection<Ramp>::iterator find_storehouse_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator storehouse_cbegin();

    NodeCollection<Ramp>::const_iterator storehouse_cend();

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

