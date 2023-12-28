#ifndef NETSIM_ZPO_REPORTS_HPP
#define NETSIM_ZPO_REPORTS_HPP

#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

void generate_structure_report(const Factory &factory, std::ostream &os);
void generate_simulation_turn_report(const Factory &factory, std::ostream &os, Time time);

#endif