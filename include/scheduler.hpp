/*
 * scheduler.hpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */

#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <chrono>
#include "sqlite.hpp"
#include <functional>
#include <future>
#include "task.hpp"
#include "service_queue.hpp"
#include <mutex>
#include <thread>

#pragma once
namespace pts {

typedef task::TaskInterface TaskInterface;

class Scheduler {
    private:
        struct SchedulerTask {

            TaskInterface *task;
            std::chrono::system_clock::time_point exec_time;

            bool is_recurring;
            std::chrono::system_clock::duration interval;
            bool operator<(const struct SchedulerTask &t) const {
                return exec_time < t.exec_time;
            }
            bool operator>(const struct SchedulerTask &t) const {
                return exec_time > t.exec_time;
            }
        };
    public:
        Scheduler() : running(false), stop(false) {}
        ~Scheduler();
       void runEvery(TaskInterface *task,
                std::chrono::system_clock::duration interval);
        void runAt(TaskInterface *task,
                std::chrono::system_clock::time_point time);
        void start();
        void schedule();
    private:
        std::thread main_thread;
        bool running;
        bool stop;

        service_queue_l<struct SchedulerTask,
            std::vector<struct SchedulerTask>,
            std::greater<struct SchedulerTask> > wait_q;

        std::vector<std::pair<std::thread, std::future<void> > > active_tasks;
};

}




#endif /* SCHEDULER_HPP_ */
