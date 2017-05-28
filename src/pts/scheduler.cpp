/*
 * scheduler.cpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */

#include <mutex>
#include "scheduler.hpp"

/* Pantheios Header Files */
/*#include <pantheios/pantheios.hpp>            // Pantheios C++ main header
#include <pantheios/inserters/args.hpp>       // for pantheios::args
#include <pantheios/inserters/exception.hpp>  // for pantheios::exception
#include <pantheios/backends/bec.file.h>      // be.file header
*/
//#define PSTR(x)         PANTHEIOS_LITERAL_STRING(x)
using namespace pts;




Scheduler::~Scheduler() {
    main_thread.join();

    // wait for worker threads to exit
    while (active_tasks.size() != 0) {
        auto it = active_tasks.begin();
        while(it != active_tasks.end()) {
            if(it->second.wait_for(std::chrono::milliseconds(0)) ==
                    std::future_status::ready) {
                it->first.join();
                it = active_tasks.erase(it);
            } else {
                it++;
            }
        }
    }
}

void Scheduler::start() {
    if (!running) {
        stop = false;
        main_thread = std::thread(&Scheduler::schedule, this);
        running = true;
    }
}

void Scheduler::runEvery(TaskInterface *task,
        std::chrono::system_clock::duration interval) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.is_recurring = true;
    stask.interval = interval;
    stask.exec_time = std::chrono::system_clock::now() + interval;
    wait_q.push(stask);
}

void Scheduler::runAt(TaskInterface *task,
        std::chrono::system_clock::time_point time) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.exec_time = time;
    wait_q.push(stask);
}


void Scheduler::schedule() {
    while(!stop) {
        std::mutex persist_lock;
        while (!wait_q.empty()) {
            struct SchedulerTask task = wait_q.top();
            if (task.exec_time < std::chrono::system_clock::now()) {
                std::packaged_task<void()> ptask([this, task, &persist_lock]() {
                        try {
                            auto data = task.task->run();
                            persist_lock.lock();
                            task.task->persist(data);
                            persist_lock.unlock();
                            if (!this->stop && task.is_recurring) {
                                this->runEvery(task.task, task.interval);
                            }
                        } catch (const char *e) {
                        	throw "Exception encountered";
                        	//pantheios::log_ERROR("Exception encountered: ", e);

                        } catch (...) {
                        	throw "default unhandled exception";
                        	//pantheios::logputs(pantheios::emergency, PSTR("default unhandled exception"));
                            }
                        return;
                    });
                std::future<void> future = ptask.get_future();

                active_tasks.push_back(
                        make_pair(std::move(std::thread(std::move(ptask))),
                            std::move(future)));
                wait_q.pop();
                continue;
            }
            break;
        }


        auto it = active_tasks.begin();
        while(it != active_tasks.end()) {
            if(it->second.wait_for(std::chrono::milliseconds(0)) ==
                    std::future_status::ready) {
                it->first.join();
                it = active_tasks.erase(it);
            } else {
                it++;
            }
        }

         std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}


