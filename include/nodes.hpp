#ifndef NETSIM_ZPO_NODES_HPP
#define NETSIM_ZPO_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include <map>
#include <optional>
#include <memory>

enum class ReceiverType {
    WORKER, STOREHOUSE
};

class IPackageReceiver {
public:
    virtual void receive_package(Package&& package) = 0;

    virtual ElementID get_id() const = 0;

//#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    virtual ReceiverType get_receiver_type() const = 0;
//#endif

    virtual IPackageStockpile::const_iterator begin() const = 0;

    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;

    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : prob_gen_(std::move(pg)) {};

    const_iterator begin() const { return preferences_.begin(); }

    const_iterator end() const { return preferences_.end(); }

    const_iterator cbegin() const { return preferences_.cbegin(); }

    const_iterator cend() const { return preferences_.cend(); }


    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const { return preferences_; }

private:
    preferences_t preferences_;
    ProbabilityGenerator prob_gen_;
};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;

    PackageSender(PackageSender&& package_sender_) = default;

    void send_package();

    const std::optional<Package>& get_sending_buffer() const { return buffer_; }

    ~PackageSender() = default;

private:
    std::optional<Package> buffer_ = std::nullopt;
protected:
    void push_package(Package&& push_package) { buffer_.emplace(push_package.get_id()); }
};




class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di), last_delivery(-di_) {};

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const { return di_; }

    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
    Time last_delivery;

};

class Worker : public PackageSender, public IPackageReceiver {
public:

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
            : PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);

    TimeOffset get_processing_duration() const { return pd_; }

    Time get_package_processing_start_time() const { return startTime_; }

    ElementID get_id() const override { return id_; }

    IPackageStockpile::const_iterator begin() const override { return q_->begin(); };

    IPackageStockpile::const_iterator end() const override { return q_->end(); };

    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); };

    IPackageStockpile::const_iterator cend() const override { return q_->cend(); };

    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }

    void receive_package(Package&& package) override;


private:
    ElementID id_;
    TimeOffset pd_;

    Time startTime_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> buffer_ = std::nullopt;

};


class Storehouse : public IPackageReceiver {
public:

    Storehouse(ElementID id,
               std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO)) : id_(
            id), d_(std::move(d)) {};

    void receive_package(Package&& package) override;

    IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }

    IPackageStockpile::const_iterator cend() const override { return d_->cend(); }

    IPackageStockpile::const_iterator begin() const override { return d_->begin(); }

    IPackageStockpile::const_iterator end() const override { return d_->end(); }

    ElementID get_id() const override { return id_; }

    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

#endif