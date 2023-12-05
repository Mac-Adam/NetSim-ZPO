#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r){
    double receivers_ = preferences_.size();

    if(receivers_ == 0)
        preferences_[r] = 1.0;
    if(receivers_ > 0){
        for(auto &r : preferences_)
            r.second = 1/(receivers_ + 1);       
    }
};

void ReceiverPreferences::remove_receiver(IPackageReceiver *r){

}