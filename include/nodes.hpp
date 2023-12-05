#ifndef NETSIM_ZPO_NODES_HPP
#define NETSIM_ZPO_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include <map>
#include <optional>
#include <memory>

enum class ReciverType{
    WORKER, STOREHOUSE
};

class IPackageReceiver{
    virtual void package_receiver(Package &&package) = 0;

    virtual ElementID get_id() const = 0;

    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        virtual ReciverType get_receiver_type() const = 0;
    #endif

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences{
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator; 

    ReceiverPreferences(ProbabilityGenerator pg);

    const_iterator begin() const {return preferences_.begin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const {return preferences_.cend();}


    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver *choose_receiver();
    const preferences_t &get_preferences() const {return preferences_;}

private:
    preferences_t preferences_;
};

class PackageSender{
    ReceiverPreferences receiver_preferences_;

    PackageSender(PackageSender &&package_sender_) = default;

    void send_package();

    const std::optional<Package> &get_sending_buffer() const {return buffer_;}

private:
    std::optional<Package> buffer_ = std::nullopt;
protected:
    void push_package(Package &&push_package) {buffer_.emplace(push_package.get_id());}
};

class Storehouse : public IPackageReceiver{
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);

    void package_receiver(Package &&package) override;


};

#endif