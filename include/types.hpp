//
// Created by adams on 28.11.2023.
//

#ifndef NETSIM_ZPO_TYPES_HPP
#define NETSIM_ZPO_TYPES_HPP

#include <functional>

using ElementID = int;

using ProbabilityGenerator = std::function<double()>;

using TimeOffset = int;
using Time = int;

#endif //NETSIM_ZPO_TYPES_HPP
