#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double receivers_ = preferences_.size();

    if (receivers_ == 0)
        preferences_[r] = 1.0;
    if (receivers_ > 0) {
        for (auto& r: preferences_)
            r.second = 1 / (receivers_ + 1);
    }
};

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {

}

void Ramp::deliver_goods(Time t) {
    if (t % di_ == 0) {
        push_package(Package());
    }
}


void Worker::do_work(Time t) {
    //Nie wiem jeszce jak to zrobić
    if (t % pd_ == 0) {
        send_package();
        recieve_package();
    }
}