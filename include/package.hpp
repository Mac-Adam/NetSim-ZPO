//
// Created by adams on 28.11.2023.
//

#ifndef NETSIM_ZPO_PACKAGE_HPP
#define NETSIM_ZPO_PACKAGE_HPP

#include <set>
#include "types.hpp"

class Package {

public:

    Package();
    Package(ElementID);
    Package(Package&&);
    Package& operator=(Package&& other);
    ElementID get_id() const {return id_;}
    ~Package();

private:

    ElementID id_;

    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //NETSIM_ZPO_PACKAGE_HPP
