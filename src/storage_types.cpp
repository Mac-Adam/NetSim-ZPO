//
// Created by adams on 04.12.2023.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package temp;
    switch (qType_) {
        case PackageQueueType::FIFO:
            temp = std::move(q_.front());
            q_.pop_front();
            break;
        case PackageQueueType::LIFO:
            temp = std::move(q_.back());
            q_.pop_back();
    }
    return temp;
}
