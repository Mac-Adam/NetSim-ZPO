
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
    double receivers_ = preferences_.size();
    if(receivers_ > 1){
        for(auto &re : preferences_){
            if(re.first != r){
                re.second = 1/(receivers_ - 1);
            }
        }
    }
    preferences_.erase(r);
}

IPackageReceiver *ReceiverPreferences::choose_receiver(){
    double probal = prob_gen_();
    double distribution = 0;
    for(auto &receiver : preferences_){
        distribution += receiver.second;
        if(probal <= distribution){
            return receiver.first;
        }
    }
}

void PackageSender::send_package(){
    IPackageReceiver *receiver;

    if(buffer_){
        receiver = receiver_preferences_.choose_receiver();
        receiver -> receive_package(std::move(*buffer_));
        buffer_.reset();
    }
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
    if (!buffer_) {
        if (!q_->empty()) {
            buffer_.emplace(q_->pop());
            startTime_ = t;
        }
    } else if (t >= startTime_ + pd_) {
        push_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}

void Storehouse::receive_package(Package&& p){
    d_.push(this);
}


