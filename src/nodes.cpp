
#include "nodes.hpp"

/*
Ramp& Ramp::operator=(Ramp&& other) {
    if (&other == this) {
        return *this;
    }

    this->id_ = other.id_;
    this->di_ = other.di_;
    this->last_delivery = other.last_delivery;

    return *this;
};

Worker& Worker::operator=(Worker&& other) {
    if (&other == this) {
        return *this;
    }

    this->id_ = other.id_;
    this->pd_ = other.pd_;
    if (other.buffer_.has_value()) {
        this->buffer_.emplace();
    }


    return *this;
};
*/

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    std::size_t receivers_ = preferences_.size();

    if (receivers_ == 0)
        preferences_[r] = 1.0;
    if (receivers_ > 0) {
        for (auto& i: preferences_)
            i.second = 1.0 / ((double) receivers_ + 1.0);
        preferences_[r] = 1.0 / ((double) receivers_ + 1.0);
    }
};

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    std::size_t receivers_ = preferences_.size();
    if (receivers_ > 1) {
        for (auto& re: preferences_) {
            if (re.first != r) {
                re.second = 1.0 / ((double) receivers_ - 1.0);
            }
        }
    }
    preferences_.erase(r);
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double probal = prob_gen_();
    double distribution = 0;
    for (auto& receiver: preferences_) {
        distribution += receiver.second;
        if (probal <= distribution) {
            return receiver.first;
        }
    }
    return nullptr;
}

void PackageSender::send_package() {
    IPackageReceiver* receiver;

    if (buffer_) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*buffer_));
        buffer_.reset();
    }
}


void Ramp::deliver_goods(Time t) {
    if (t >= last_delivery + di_) {
        push_package(Package());
        last_delivery = t;
    }
}

void Worker::receive_package(Package&& package) {
    q_->push(std::move(package));
}


void Worker::do_work(Time t) {

    if (!buffer_) {
        if (!q_->empty()) {
            buffer_.emplace(q_->pop());
            startTime_ = t;
        }
    }
    if (buffer_) {
        if (t + 1 >= startTime_ + pd_) {
            push_package(std::move(buffer_.value()));
            buffer_.reset();
        }
    }

}

void Storehouse::receive_package(Package&& p) {
    d_->push(std::move(p));
}


