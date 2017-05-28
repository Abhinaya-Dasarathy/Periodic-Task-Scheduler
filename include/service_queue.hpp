/*
 * service_queue.hpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */
#include <queue>
#include <mutex>

#pragma once

namespace pts {
template <class T,
         class Container = std::vector<T>,
         class Compare = std::less<typename Container::value_type>
         >
class service_queue_l {
	private:
        std::mutex lock;
        std::priority_queue<T, Container, Compare> q;
    public:
        void push(T& item) {
                   std::lock_guard<std::mutex> g(lock);
                    q.push(item);
                    return;
               }
        void pop() {
                    std::lock_guard<std::mutex> g(lock);
                     q.pop();
                     return;
                }

        bool empty() {
            std::lock_guard<std::mutex> g(lock);
            return q.empty();
        }
        T top() {
            std::lock_guard<std::mutex> g(lock);
            return q.top();
        }


};
}
