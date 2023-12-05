//
// Created by adams on 28.11.2023.
//
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {0};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (freed_IDs.empty()) {
        auto it = std::prev(std::end(assigned_IDs));
        ElementID id = *it + 1;
        assigned_IDs.insert(id);
        id_ = id;
    } else {
        ElementID id = *freed_IDs.begin();
        freed_IDs.erase(id);
        assigned_IDs.insert(id);
        id_ = id;
    }
}

Package::Package(ElementID id) : id_(id) {}

Package::Package(Package&& other) {
    id_ = other.id_;
    other.id_ = -1;
}

Package& Package::operator=(Package&& other) {
    if (&other == this) {
        return *this;
    }
    assigned_IDs.erase(this->id_);
    freed_IDs.insert(this->id_);
    this->id_ = other.id_;
    assigned_IDs.insert(this->id_);

    other.id_ = -1;
    return *this;
}

Package::~Package() {
    ElementID id = id_;
    if (id != -1) {
        assigned_IDs.erase(id);
        freed_IDs.insert(id);
    }
}
