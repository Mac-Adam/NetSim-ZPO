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

void Worker::recieve_package(Package&& package) {
    q_->push(std::move(package));
}


void Worker::do_work(Time t) {
    if (!working) {
        if (!q_->empty()) {
            push_package(q_->pop());
            startTime_ = t;
        }
    } else if (t >= startTime_ + pd_) {
        send_package();
    }
}