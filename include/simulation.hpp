#ifndef NETSIM_ZPO_SIMULATION_HPP
#define NETSIM_ZPO_SIMULATION_HPP

#include "factory.hpp"
#include "types.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);


class IntervalReportNotifier {
public:

    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {};
    bool should_generate_report(Time t);

private:

    TimeOffset to_;

};


class SpecificTurnsReportNotifier {
public:

    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {};
    bool should_generate_report(Time t);

private:

    std::set<Time> turns_;

};

#endif //NETSIM_ZPO_SIMULATION_HPP
