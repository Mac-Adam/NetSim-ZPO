#ifndef NETSIM_ZPO_FACTORY_HPP
#define NETSIM_ZPO_FACTORY_HPP

#include "nodes.hpp"

class Factory : public Worker : public Storehouse :  public Ramp {
public:
    void add_ramp(&&Ramp r);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
};
#endif //NETSIM_ZPO_FACTORY_HPP
