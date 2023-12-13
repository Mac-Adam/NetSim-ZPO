//
// Created by adams on 04.12.2023.
//

#ifndef NETSIM_ZPO_STORAGE_TYPES_HPP
#define NETSIM_ZPO_STORAGE_TYPES_HPP

#include "package.hpp"
#include <list>

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& package) = 0;

    virtual bool empty() const = 0;

    virtual std::size_t size() const = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() = 0;

    virtual ~IPackageQueue() override = default;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType t) : qType_(t) {};

    void push(Package&& package) override { q_.emplace_back(std::move(package)); }

    bool empty() const override { return q_.empty(); }

    std::size_t size() const override { return q_.size(); }

    const_iterator begin() const override { return q_.cbegin(); };

    const_iterator end() const override { return q_.cend(); };

    const_iterator cbegin() const override { return q_.cbegin(); };

    const_iterator cend() const override { return q_.cend(); };

    Package pop() override;

    PackageQueueType get_queue_type() override { return qType_; };

    ~PackageQueue() override = default;

private:
    PackageQueueType qType_;
    std::list<Package> q_;
};

#endif //NETSIM_ZPO_STORAGE_TYPES_HPP
