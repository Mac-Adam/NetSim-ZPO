#ifndef NETSIM_ZPO_FACTORY_HPP
#define NETSIM_ZPO_FACTORY_HPP

#include "nodes.hpp"

class Factory : public Worker, public Storehouse,  public Ramp {
public:
    void add_ramp(&&Ramp r);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator ramp_cbegin();
    NodeCollection<Ramp>::const_iterator ramp_cend();

    void add_worker(&&Worker w);
    void remove_worker(ElementID id);
    NodeCollection<Ramp>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator worker_cbegin();
    NodeCollection<Ramp>::const_iterator worker_cend();

    void add_storehouse(&&Storehouse s);
    void remove_storehouse(ElementID id);
    NodeCollection<Ramp>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator storehouse_cbegin();
    NodeCollection<Ramp>::const_iterator storehouse_cend();

    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);
    };
#endif //NETSIM_ZPO_FACTORY_HPP
